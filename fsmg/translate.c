/* Copyright (C) 1999 Free Software Foundation, Inc.
   Author : philippe@prados-obj.nom.fr <Ph. Prados>

This file is part of fsmg.

Fsmg is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

Fsmg is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Fsmg; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  
*/

#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <errno.h>
#include <signal.h>

#include <assert.h>

#include "all.h"
#include "translate.h"
#include "getopt.h"
#include "autolex.h"
#include "autoyac.h"
#include "skeleton.h"
#include "conv.h"
#include "convexe.h"
#include "clear.h"
#include "error.h"
#include "tools.h"

static void	(*last_sigint)(int sig);

/**********/
/* sigint */
/**********/

static void 	sigint(int sig)

{
	if (*fsm.header) unlink(fsm.header);	/* Efface le fichier header */
	if (*fsm.nameo)  unlink(fsm.nameo);	/* Et le fichier de sortie */
	last_sigint(sig);			/* Appel le signal precedant */
}

/*************/
/* clear_fsm */
/*************/

static void  	clear_fsm()

{ 
	clear_headc();				/* Efface les headers C */
	clear_triples();			/* Efface les triples memorisee */
	clear_exe();				/* Efface les exes */
	close_symbol(fsm.symtbl_state);		/* Efface les states */
	close_symbol(fsm.symtbl_event);		/* Efface les events */
	free(fsm.tail);
	fsm.tail=NULL;
	free_buf_C();				/* Efface le buffer C */
	if (fsm.fin!=NULL) fclose(fsm.fin);
}

/*************/
/* translate */
/*************/

static const char str_copy[]=
		"\nstatic const char fsm_copyright[]="
		"\"Use FSMG V%d.%d proposed by Ph. PRADOS\\n\";\n";

int 	translate(void)

{ 
	fsm.error=0;		/* Reset le flag d'erreur */
	fsm.lineno=1;
	fsm.status=READING;
	fsm.tail=NULL;
	
	free_buf_C();

/* - Ouvre le fichier source - */
	if ((fsm.fin=fopen(fsm.namei,"r" FTXT))==NULL)
	{ 
		fsm_error(fsm.namei,0,ERR_FILE,strerror(errno));
		return(errno);
	}

/* - Analyse du source et creation de triples - */
	fsm.start_state=NULL;

	if (trans_rules() || fsm.error )
	{ 
		clear_fsm();
		return(ERR_SYNTAX);
	}

	if ((fsm.start_state!=NULL) && (fsm.start_state->triple==NULL))
	{ 
		fsm_error(fsm.namei,fsm.line_start,ERR_BADSTART);
		clear_fsm();
		return(ERR_SYNTAX);
	}

/* - Test l'automate - */
	if (!fsm.nb_translate) fsm.max_ext=0;
	if (test_auto())
	{ 
		clear_fsm();
		return(ERR_SYNTAX);
	}

	fsm.status=WRITING;

/* - Prepare un break de l'utilisateur - */
	last_sigint=signal(SIGINT,sigint);

/* - Ecrit le fichier header - */
	if (fsm.fl.head)
	{ 
		FILE* sth;

		if (!*fsm.header)
		{ 
			strcpy(fsm.header,fsm.nameo);
			strcpy(strrchr(fsm.header,'.'),"." HFS);
		}
		if ((sth=fopen(fsm.header,"w" FTXT))==NULL)
		{ 
			fsm_error(fsm.header,0,ERR_FILE,strerror(errno));
			errno=0;
		}
		else
		{ 
			aff_event(sth);
			fclose(sth);
		}
	}

	if (!fsm.fl.no_exec)
	{
/* - Creation du fichier de sortie - */
		if (*fsm.nameo)
		{ 
			if ((fsm.fout=fopen(fsm.nameo,"w" FTXT))==NULL)
			{ 
				fsm_error(fsm.nameo,0,ERR_FILE,strerror(errno));
				fclose(fsm.fin);
				fsm.fin=NULL;
				return(errno);
			}
		}
		else
		{
			fsm.fout=stdout;
		}

/* - Ecriture du copyright - */
		fprintf(fsm.fout,str_copy,fsm.major,fsm.minor);

/* - Copie du squelette avec traduction des mots clefs - */
		if (fsm.fl.line) aff_line(fsm.fout,1,fsm.names);
		fsm.pos_ske=0;			/* Reset le squelette */
		conv_squelette();

/* - Verifie si a utilise tous les etats - */
		if (!fsm.fl.warn)
		{ 
			ty_var_state	*pts;

			assert(fsm.first_state!=NULL);
			assert(fsm.first_state->next!=NULL);	/* Saute le %start */
			for (pts=fsm.first_state->next->next;pts!=NULL;pts=pts->next)
			{ 
				if (!pts->use)
				{ 
					fsm_warning(fsm.namei,fsm.last_line,WAR_STATE,
						key_symbol(fsm.symtbl_state,pts));
				}
			}
		}

/* - Fin - */
		fclose(fsm.fin);
		fsm.fin=NULL;
		close_exe();
		if (fsm.fout!=stdout) fclose(fsm.fout);
	}

/* - Fin du break de l'utilisateur - */
	signal(SIGINT,last_sigint);

/* Netoie tous ! */
	free_buf_C();
	clear_fsm();
	return(fsm.error);
}

/**************/
/* gere_param */
/**************/

/*
   return 1 if error
   2 if help
   3 if version
*/
int 	gere_param(int argc, char* argv[])

{ 
	int		i,opt;
	int		size_bufdef=0;		/* Taille du buffer bufdef */
	void*	back;

	fsm.major=(char)atoi(VER_MAJOR);
	fsm.minor=(char)atoi(VER_MINOR);
	fsm.max_ext=256;

	fsm.bufdef=NULL;
	fsm.ferr=stderr;
	fsm.fl.line=true;
	fsm.mode=1;				/* Mode par defaut */
	*fsm.header='\0';

	while ((opt=getopt(argc,argv,"CcD:d:EeHh?K:k:LlM:m:O:o:P:p:R:r:SsTtVvWwX:x:-:"))!=EOF)
	{ 
		switch (opt)
		{ 
			case '\0':
				Error:
				return(1);

			case '-' :
				     if (!strcmp(optarg,"check-only"))		{ optarg=argv[optind++]; goto Check; }
				else if (!strcmp(optarg,"define"))		{ optarg=argv[optind++]; goto Def; }
				else if (!strcmp(optarg,"priority-event"))	{ optarg=argv[optind++]; goto Event; }
				else if (!strcmp(optarg,"help"))		{ optarg=argv[optind++]; goto Help; }
				else if (!strcmp(optarg,"skelette-file"))	{ optarg=argv[optind++]; goto SkeletteFile; }
				else if (!strcmp(optarg,"no-lines"))		{ optarg=argv[optind++]; goto Line; }
				else if (!strcmp(optarg,"skelette"))		{ optarg=argv[optind++]; goto Skelette; }
				else if (!strcmp(optarg,"error-file"))		{ optarg=argv[optind++]; goto ErrorFile; }
				else if (!strcmp(optarg,"output"))		{ optarg=argv[optind++]; goto Output; }
				else if (!strcmp(optarg,"header"))		{ optarg=argv[optind++]; goto Header; }
				else if (!strcmp(optarg,"size"))		{ optarg=argv[optind++]; goto Size; }
				else if (!strcmp(optarg,"time"))		{ optarg=argv[optind++]; goto Time; }
				else if (!strcmp(optarg,"verbose"))		{ optarg=argv[optind++]; goto Verbose; }
				else if (!strcmp(optarg,"version"))		{ optarg=argv[optind++]; goto Version; }
				else if (!strcmp(optarg,"no-warning"))		{ optarg=argv[optind++]; goto Warn; }
				else if (!strcmp(optarg,"max-external"))	{ optarg=argv[optind++]; goto MaxExternal; }
				else if (!strcmp(optarg,"All"))			{ optarg=argv[optind++]; fsm.mode=1; break; }
				else if (!strcmp(optarg,"ArrSwitch"))		{ optarg=argv[optind++]; fsm.mode=2; break; }
				else if (!strcmp(optarg,"FnArr"))		{ optarg=argv[optind++]; fsm.mode=3; break; }
				else if (!strcmp(optarg,"ArrFnArr"))		{ optarg=argv[optind++]; fsm.mode=4; break; }
				else if (!strcmp(optarg,"Switch"))		{ optarg=argv[optind++]; fsm.mode=5; break; }
				goto Error;

			case 'c' :
			case 'C' :	/* --check-only */
				Check:
				fsm.fl.no_exec=true;
				break;

			case 'd' :
			case 'D' : /* --define */
				Def:
				i=strlen(optarg)+1;
				if ((fsm.bufdef=realloc(back=fsm.bufdef,size_bufdef+i))==NULL)
				{ 
					free(back);
					fsm_fatal("",0,ERR_MEMORY);
					return(1);
				}
				strcpy(fsm.bufdef+size_bufdef,optarg);
				size_bufdef+=i;
				break;

			case 'e' :
			case 'E' :	/* --priority-event */
				Event:
				fsm.fl.prio=true;
				break;

			case 'h' :
			case 'H' : // --help
			case '?' :
				Help:
				return(2);

			case 'k' :
			case 'K' :	/* --skelette-file ... */
				SkeletteFile:
				strcpy(fsm.names,optarg);
				add_ext(fsm.names,".skl");
				fsm.mode=0;	/* Utilise un fichier */
				break;

			case 'l' :
			case 'L' :	/* --no-lines */
				Line:
				fsm.fl.line=false;
				break;

			case 'm' :
			case 'M' :	/* --skelette ... */
				Skelette:
				if ((optarg[1]!='\0') || (optarg[0]<'0') || (optarg[0]>'4'))
				{ 
					errno=EINVAL;
					goto Error;
				}
				fsm.mode=optarg[0]-'0'+1;
				break;

			case 'o' :
			case 'O' : /* --output */
				Output:
				strcpy(fsm.output,optarg);
				break;

			case 'p' :
			case 'P' :	/* --header */
				Header:
				fsm.fl.head=true;
				if (*optarg=='-') 
				{
					*fsm.header='\0';
					--optind;
				}
				else
				{
					strcpy(fsm.header,optarg);
					add_ext(fsm.header,"." HFS);
				}
				break;

			case 'r' :
			case 'R' : /* --error-file */
				ErrorFile:
				strcpy(fsm.nameerr,optarg);
				break;

			case 's' :
			case 'S' :	/* --size */
				Size:
				fsm.fl.opt=1;
				break;

			case 't' :
			case 'T' :	/* --time */
				Time:
				fsm.fl.opt=2;
				break;

			case 'V' : /* --version */
				Version:
				return 3;
				break;

			case 'v' : /* -- verbose */
				Verbose:
				fsm.fl.verbose=true;
				break;

			case 'w' :
			case 'W' :	/* --no-warning */
				Warn:
				fsm.fl.warn=true;
				break;

			case 'x' :
			case 'X' :	/* --max-external */
				MaxExternal:
				fsm.max_ext=atou(optarg);
				break;

			default :
				assert(0);
				break;
		}
	}

//if (fsm.bufdef==NULL) fsm.bufdef=malloc(1);
	if ((fsm.bufdef=realloc(back=fsm.bufdef,size_bufdef+1))==NULL)
	{ 
		free(back);
		fsm_fatal("",0,ERR_MEMORY);
		return(1);
	}
	fsm.bufdef[size_bufdef]='\0';
	return(0);
}
