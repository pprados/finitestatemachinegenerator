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
#include <malloc.h>
#include <errno.h>
#include <assert.h>

#include "all.h"
#include "conv.h"
#include "error.h"
#include "autolex.h"
#include "convexe.h"
#include "tools.h"

/*************/
/* test_auto */
/*************/

/* - Test l'automate pour la detection USEERROR,USENOTHING - */

bool	test_auto()
{ 
	ty_var_state*	pts;
	ty_var_event*	pte;
	ty_triple*	pt;	/* Prochain Triple connu de l'etape */
	unsigned	i,j,k;
	unsigned 	exe_def=~0U;

	fsm.use_nothing=fsm.use_error=false;

/* - Cherche les externes non utilise - */
	{ 
		ty_var_event* prev=NULL;

		assert(fsm.first_event!=NULL);
		for (pte=fsm.first_event->next;
				pte!=NULL;
				prev=pte,pte=pte->next)
		{ 
			if ((pte->dcl) && (pte->triple==NULL))
			{ 
				if (!fsm.fl.warn)
				{ 
					fsm_warning(fsm.namei,fsm.line,WAR_EXTERN,
						key_symbol(fsm.symtbl_event,pte));
				}
				/* - Efface l'evenement - */
				if (prev==NULL)
				{ 
					fsm.first_event=pte->next;
				}
				else
				{ 
					prev->next=pte->next;
				}
				sub_symbol(fsm.symtbl_event,key_symbol(fsm.symtbl_event,pte));
				pte=prev;
				if (pte==NULL) pte=fsm.first_event->next;
				fsm.nb_event--;
				assert(fsm.nb_event!=~0U);
				continue;
			}
		}
	}

/* - Verifie si une regle au moin existe - */
	if (fsm.first_state->next==NULL)		/* Pas d'etat ! */
	{ 
		fsm_error(fsm.namei,fsm.line,ERR_NOSTATE);
		fsm.error=true;
	}

	if (fsm.first_event->next->next==NULL)		/* Pas d'event ! */
	{ 
		fsm_error(fsm.namei,fsm.line,ERR_NOEVENT);
		fsm.error=true;
	}
	if (fsm.error) return(true);

	if ((fsm.first_event->triple!=NULL)
			&& (!fsm.first_event->triple->event->num)
			&& (!fsm.first_event->triple->state->num))
	{ 
		exe_def=(fsm.first_event->triple->exe==NULL) ? FSM_NOTHING
				: fsm.first_event->triple->exe->exe;
	}

	for (pts=fsm.first_state;pts!=NULL;pts=pts->next)
	{ 
		if (!pts->num) continue;
		pt=pts->triple;
		if (!pt->event->num) pt=pt->next_state;
		pte=fsm.first_event;
		if (!pte->num) pte=pte->next;
		for (k=1;k<fsm.nb_event;pte=pte->next,k++)
		{ 
			i=k;
			if (k>=fsm.nb_local) i=fsm.max_ext+k-fsm.nb_local;
			if ((pt!=NULL) && (pt->event->num==i))
			{ 
				if (pt->exe==NULL) fsm.use_nothing=true;
				pt=pt->next_state; /* Event suivant du meme state */
				goto Next;
			}
			else
			{ 
				for (j=0;j<2;j++)
				{ 
					if ((fsm.fl.prio) ? j : !j)
					{
						/* - Cherche si default du state - */
						if (pts->triple==NULL)		/* %extern not use */
							continue;
						assert(pts->triple->event!=NULL);
						if (!pts->triple->event->num)
						{ 
							if (pts->triple->exe==NULL) fsm.use_nothing=true;
							if ((pte->triple!=NULL)
									&& (!pte->triple->state->num)) /* Si autre */
								if (!fsm.fl.warn)
								{ 
									fsm_warning(fsm.namei,pte->triple->line,WAR_PRIO,
										key_symbol(fsm.symtbl_state,pts),
										key_symbol(fsm.symtbl_event,pte));
								}
							goto Next;
						}
					}
					else
					/* - Cherche si default de l'event - */
					{ 
						if (pte->triple==NULL)		/* %extern not use */
							continue;
						assert(pte->triple->state!=NULL);
						if (!pte->triple->state->num)
						{ 
							if (pte->triple->exe==NULL) fsm.use_nothing=true;
							if ((pts->triple!=NULL) && (!pts->triple->event->num))
								if (!fsm.fl.warn)
								{ fsm_warning(fsm.namei,pts->triple->line,WAR_PRIO,
										key_symbol(fsm.symtbl_event,pte),
										key_symbol(fsm.symtbl_state,pts));
								}
							goto Next;
						}
					}
				}
				/* - Cherche default global - */
				if (exe_def!=~0)
				{ 
					if (exe_def==FSM_NOTHING) fsm.use_nothing=true;
					goto Next;
				}
			}
			fsm.use_error=true;
Next:;

		}
	}
	return(false);
}

/*************/
/* aff_event */
/*************/

void	aff_event(FILE *stream)
{ 
	ty_var_event* pte;

	for (pte=fsm.first_event;pte!=NULL;pte=pte->next)
	{ 
		if ((pte->num) && (!pte->dcl)
				&& (*key_symbol(fsm.symtbl_event,pte)!='~'))
			fprintf(stream,"#define %-11s (%d)\n",
		key_symbol(fsm.symtbl_event,pte),pte->num-1+fsm.max_ext);
	}
}

/***************/
/* conv_define */
/***************/

/* - Recopie les #defines de la ligne de commande - */

static void	 conv_define(void)
{ 
	if (!fsm.fl.no_exec)
	{ 
		char*  pt;
		char*  bufdef=fsm.bufdef;

		for (;*bufdef!='\0';)
		{ 
			fputs("#define\t",fsm.fout);
			if ((pt=strchr(bufdef,'='))!=NULL)
			{ 
				*pt='\0';
				fprintf(fsm.fout,"%s\t%s\n",bufdef,++pt);
				bufdef=pt+strlen(pt)+1;
			}
			else
			{ 
				fprintf(fsm.fout,"%s\n",bufdef);
				bufdef+=strlen(bufdef)+1;
			}
		}
	}
}

/***************/
/* conv_header */
/***************/

/* - Recopie les headers encadre de %{ et %} - */

static void	 conv_header(void)
{ 
	ty_headc*	pt;

	for (pt=fsm.headc;pt!=NULL;pt=pt->next)
	{ 
		if (fsm.fl.line)
			aff_line(fsm.fout,pt->line,fsm.namei);
		fputs(fsm.headc->buf,fsm.fout);
	}
}

/************/
/* conv_var */
/************/

/* - Ecrit les variables utils au squelette - */

static void	 conv_var(void)
{ static const char* str_squelette[]={
		"ALL",
		"ARRSWITCH",
		"ARRFNARR",
		"FNARR",
		"SWITCH"
	};
	char* pt=fsm.namei;

	fputs("#define FSM_NAME    \"",fsm.fout);
	while (*pt)
	{ 
		if (*pt=='\\') fputc('\\',fsm.fout);
		fputc(*pt++,fsm.fout);
	}
	fputs("\"\n",fsm.fout);
	fprintf(fsm.fout,"#define FSM_MAJOR     (%d)\n"
			 "#define FSM_MINOR     (%d)\n"
			 "#define FSM_NBEVENT   (%d)\n"
			 "#define FSM_NBSTATE   (%d)\n"
			 "#define FSM_NBTRANS   (%d)\n"
			 "#define FSM_NBEXEC    (%d)\n"
			 "#define FSM_MAXEXTERN (%d)\n",
			 fsm.major,fsm.minor,
			 fsm.nb_event-1,fsm.nb_state-1,fsm.nb_translate,
			 fsm.nb_exe,
			 (fsm.nb_translate) ? fsm.max_ext : 0);
	if (fsm.fl.opt) fprintf(fsm.fout,"#define FSM_%s\n",(fsm.fl.opt==1) ? "SIZE" : "TIME");
	if (fsm.use_error) fputs("#define FSM_USE_ERROR\n",fsm.fout);
	if (fsm.use_nothing) fputs("#define FSM_USE_NOTHING\n",fsm.fout);
	if (fsm.mode) fprintf(fsm.fout,"#define FSM_%s\n",str_squelette[fsm.mode-1]);

}

/********************/
/* conv_local_event */
/********************/

static void	 conv_local_event(void)
{ 
	aff_event(fsm.fout);
}

/**************/
/* conv_array */
/**************/

/* - Traduit les triples en un tableau 2 entree - */

#define AFF_FSM_ARR(Exe) \
	fprintf(fsm.fout,"FSM_ARR(%2d) ",(Exe==NULL) ? 1 : (Exe)->exe)

static void	 conv_array(void)
{ 
	ty_var_state* pts;
	ty_var_event* pte;
	ty_triple*    pt;		/* Prochain Triple connu de l'etape */
	unsigned	    i,j,k;
	unsigned	    exe_def=~0U;

	if ((fsm.first_event->triple!=NULL)
			&& (!fsm.first_event->triple->event->num)
			&& (!fsm.first_event->triple->state->num))
	{ 
		exe_def=(fsm.first_event->triple->exe==NULL) ? FSM_NOTHING
				: fsm.first_event->triple->exe->exe;
	}

/* - Commentaire du debut du tableau - */
	fputs("\n            /* ",fsm.fout);
	assert(fsm.first_event!=NULL);
	{ 
		char* pt_symbol;

		for (pte=fsm.first_event;pte!=NULL;pte=pte->next)
		{ 
			if (pte->num)
			{ 
				pt_symbol=key_symbol(fsm.symtbl_event,pte);
				fprintf(fsm.fout,"%-11s ",(*pt_symbol=='~') ? "-Other-" : pt_symbol);
			}
		}
	}
	fputs("*/\n",fsm.fout);

	for (pts=fsm.first_state;pts!=NULL;pts=pts->next)
	{ 
		if (!pts->num) continue;
		fprintf(fsm.fout,"/* %-8s */ ",key_symbol(fsm.symtbl_state,pts));
		pt=pts->triple;
		if (!pt->event->num) pt=pt->next_state;
		pte=fsm.first_event;
		if (!pte->num) pte=pte->next;
		for (k=1;k<fsm.nb_event;pte=pte->next,k++)
		{ 
			i=k;
			if (k>=fsm.nb_local) i=fsm.max_ext+k-fsm.nb_local;
			if ((pt!=NULL) && (pt->event->num==i))
			{ 
				AFF_FSM_ARR(pt->exe);
				pt=pt->next_state; /* Event suivant du meme state */
				goto Next;
			}
			else
			{ 
				for (j=0;j<2;j++)
				{ 
					if ((fsm.fl.prio) ? j : !j)
					{
						/* - Cherche si default du state - */
						if (pts->triple==NULL)		/* WARNING %extern not use */
							continue;
						assert(pts->triple!=NULL);
						assert(pts->triple->event!=NULL);
						if (!pts->triple->event->num)
						{ 
							AFF_FSM_ARR(pts->triple->exe);
							goto Next;
						}
					}
					else
					/* - Cherche si default de l'event - */
					{ 
						if (pte->triple==NULL)		/* WARNING %extern not use */
							continue;
						assert(pte->triple!=NULL);
						assert(pte->triple->state!=NULL);
						if (!pte->triple->state->num)
						{ 
							AFF_FSM_ARR(pte->triple->exe);
							goto Next;
						}
					}
				}
				/* - Cherche default global - */
				if (exe_def!=~0U)
				{ 
					fprintf(fsm.fout,"FSM_ARR(%2d) ",exe_def);
					goto Next;
				}
			}
			fputs("FSM_ARR( 0) ",fsm.fout);		/* Erreur */
Next:;

		}
		fputc('\n',fsm.fout);
	}
}

/******************/
/* conv_lfunction */
/******************/

static void  	conv_lfunction(void)
{ 
	unsigned	i;

	fputs((fsm.use_error) ? "\tFSM_FN(0)\n"	: "\tFSM_FNNO\n",
	fsm.fout);
	fputs((fsm.use_nothing) ? "\tFSM_FN(1)\n" : "\tFSM_FNNO\n",
	fsm.fout);
	for (i=2;i<fsm.nb_exe;i++)
	{ 
		fprintf(fsm.fout,"\tFSM_FN(%2d)\n",i);
	}
}

/*****************/
/* conv_function */
/*****************/

static void  conv_function(void)
{ 
	ty_exe* exe;

	for (exe=fsm.first_exe;exe!=NULL;exe=exe->next)
	{ 
		fprintf(fsm.fout,"FSM_DCLFN(%d) \n",exe->exe);
		if (fsm.fl.line)
			aff_line(fsm.fout,exe->line,fsm.namei);
		fputs("{ ",fsm.fout);
		print_exe(exe,true);
		fputs(" return(STATE_RESERVED); }\n",fsm.fout);
	}
}

/***************/
/* conv_lstate */
/***************/

/* - Affiche "FSM_LSTATE(str)" sans le '$' pour tous les evens - */

static void  	conv_lstate(void)
{ 
	ty_var_state* pts;

	for (pts=fsm.first_state;pts!=NULL;pts=pts->next)
	{ 
		if (pts->num)
			fprintf(fsm.fout,
				"FSM_LSTATE(%s,%d)\n",
				key_symbol(fsm.symtbl_state,pts),
				pts->num);
	}
}

/**************/
/* conv_leven */
/**************/

/* - Affiche "FSM_LEVENT(x)" pour tous les evens declare dans "FSM" - */

static void  	conv_levent(void)
{ 
	ty_var_event* pte;
	char*        pt;

	for (pte=fsm.first_event;pte!=NULL;pte=pte->next)
	{ 
		if ((pte->num) && (!pte->dcl))
		{ 
			pt=key_symbol(fsm.symtbl_event,pte);
			fprintf(fsm.fout,"FSM_LEVENT(%s,%d)\n",
				(*pt=='~') ? "-Other-" : pt,
				pte->num);
		}
	}
}

/*************/
/* conv_exec */
/*************/

static void  	conv_exec(void)
{ 
	ty_exe* exe;

	for (exe=fsm.first_exe;exe!=NULL;exe=exe->next)
	{ 
		if (fsm.fl.line)
			aff_line(fsm.fout,exe->line,fsm.namei);
		fprintf(fsm.fout,"case %d : ",exe->exe);
		print_exe(exe,false);
		fputs(" break;\n",fsm.fout);
	}
}

/******************/
/* conv_translate */
/******************/

static void  	conv_translate(void)
{ 
	ty_var_event* pte;

	for (pte=fsm.first_event;pte!=NULL;pte=pte->next)
	{ 
		if ((pte->num) && (pte->dcl))
			fprintf(fsm.fout,"FSM_TRANSLATE(%s,%d)\n",
				key_symbol(fsm.symtbl_event,pte),
				pte->num-fsm.max_ext+fsm.nb_local-1);
	}
}

/***************/
/* conv_switch */
/***************/

/* Reunir les case pour meme exe */
/* Voir :TIMER {} si par goto */

static void	 conv_switch(void)
{ 
	ty_var_state*	pts;
	ty_var_event*	pte;
	ty_exe*		ptx;
	ty_triple*	pt;
	bool		find;
	bool		def;
	unsigned	exe_def=~0U;
	ty_exe*		pt_exe_def=NULL;
	int*		lexe;		/* Liste des exe utilisee */
	int		i;

	if ((lexe=malloc(fsm.nb_exe*sizeof(*lexe)))==NULL)
	{ 
		fsm_fatal(fsm.nameo,0,ERR_MEMORY);
		return;
	}
	memset(lexe,0,fsm.nb_exe*sizeof(*lexe));
	if ((fsm.first_event->triple!=NULL)
			&& (!fsm.first_event->triple->event->num)
			&& (!fsm.first_event->triple->state->num))
	{ 
		pt_exe_def=fsm.first_event->triple->exe;
		exe_def=(fsm.first_event->triple->exe==NULL) ? FSM_NOTHING
			: fsm.first_event->triple->exe->exe;
	}

/* - Parcourt les etats - */
	fputs("switch(fsm_state)\n{\n",fsm.fout);
	for (pts=fsm.first_state;pts!=NULL;pts=pts->next)
	{ 
		if (!pts->num) continue;
		fprintf(fsm.fout,"  case %d : /* %s */\n",
		pts->num-1,key_symbol(fsm.symtbl_state,pts));
		fputs("    switch (fsm_event)\n"
		"    {\n",fsm.fout);

/* - Parcourt les exec - */
		def=false;
		ptx=NULL;
		do
		{
/* - Cherche les traitements par defaut pour les evenements - */
			find=false;
			if ((fsm.fl.prio) ||
					(*key_symbol(fsm.symtbl_event,pts->triple->event)!='\0'))
			{ 
				for (pte=fsm.first_event;pte!=NULL;pte=pte->next)
				{ 
					if (*key_symbol(fsm.symtbl_event,pte)=='\0') continue;
					if ((pte->triple!=NULL) &&			 /* Trouve */
							(*key_symbol(fsm.symtbl_state,pte->triple->state)=='\0') &&
							(pte->triple->exe==ptx) )
					{ 
						/* Verifie que l'evenement n'est pas utilise */
						for (pt=pts->triple;pt!=NULL;pt=pt->next_state)
						{ 
							if (pt->event==pte) break;
						}
						if (pt==NULL)
						{ 
							fprintf(fsm.fout,"      case %s :\n",key_symbol(fsm.symtbl_event,pte));
							find=true;
						}
					}
				}
			}

/* - Parcourt les triple de l'etat - */
			for (pt=pts->triple;pt!=NULL;pt=pt->next_state)
			{ 
				if (pt->exe==ptx)
				{ 
					const char	*str=key_symbol(fsm.symtbl_event,pt->event);
					if (*str)
						fprintf(fsm.fout,"      case %s :\n",str);
					else
					{ 
						fputs(					 "      default :\n",fsm.fout);
						def=true;
					}
					find=true;
				}
			}
			if (find)
			{
/* - Cherche si l'exe est utilise - */
				if (ptx==NULL)
				{ 
					fputs("        break;\n",fsm.fout);
				}
				else
				{ 
					if (lexe[i=ptx->exe]==0)
					{ /* Exe non encore utilise */
						if (ptx!=NULL)
						{ 
							fprintf(fsm.fout,"ex_%d:\n",i);
							if (fsm.fl.line)	 aff_line(fsm.fout,ptx->line,fsm.namei);
							print_exe(ptx,false);
						}
						fputs("\n        break;\n",fsm.fout);
						lexe[i]=1;
					}
					else
					{ 
						fprintf(fsm.fout,"        goto ex_%d;\n",ptx->exe);
					}
				}
			}
			if (ptx==NULL) ptx=fsm.first_exe;
			else ptx=ptx->next;
		} while (ptx!=NULL);

		if ((!def) && (pt_exe_def!=NULL))
		{ 
			fputs("      default :\n",fsm.fout);
			if (exe_def==~0U)
			{ 
				fprintf(fsm.fout,"       	 goto ex_%d;\n",FSM_ERROR);
			}
			else
			{ 
				if (fsm.fl.line) aff_line(fsm.fout,pt_exe_def->line,fsm.namei);
				print_exe(pt_exe_def,false);
				fputs("\n       break;\n",fsm.fout);
			}
		}
		fputs("    }\n"
					"    break;\n",fsm.fout);
	}
	fputs("}\n",fsm.fout);
	free(lexe);
}

/************/
/* conv_end */
/************/

/* - Ecrit la fin du source - */

static void	 conv_end(void)
{ 
	if (fsm.fl.line) aff_line(fsm.fout,fsm.line_end,fsm.namei);
	if (fsm.tail!=NULL)
		fputs(fsm.tail,fsm.fout);
}  

/**************/
/* conv_token */
/**************/

/* - Cherche le mot clef et execute la fonction - */

static const struct 
{ 
	const char* name;
	void ( *fn)(void);
} keywords[]=
{
	{"array$",	conv_array},
	{"define$",	conv_define},
	{"end$",	conv_end},
	{"exec$",	conv_exec},
	{"function$",	conv_function},
	{"header$",	conv_header},
	{"levent$",	conv_levent},
	{"lfunction$",	conv_lfunction},
	{"local_event$",conv_local_event},
	{"lstate$",	conv_lstate},
	{"switch$",	conv_switch},
	{"translate$",	conv_translate},
	{"var$",	conv_var},
};

void		conv_token(const char* str)
{ 
	int     mid;
	int 	cc, hi, lo;

	#ifndef NDEBUG		/* - Test si la liste est triee - */
	static int first=0;
	if (!first)
	{ 
		int i;
		first=1;
		for (i=0;i<sizeoftab(keywords)-1;i++)
		{ 
			if (strcmp(keywords[i].name,keywords[i+1].name)>=0)
			{
				#ifndef CODECHECK
				fprintf(stderr,__FILE__ "(%d):keywords not sorted !!\n",__LINE__);
				#endif
				abort();
			}
		}
	}
	#endif
	str++;			/* Saute le $ de debut */
	lo = 0;
	hi = sizeoftab(keywords)-1;
	while (lo<=hi)
	{ 
		mid = (lo+hi)/2;
		if ((cc=strcmp(str,keywords[mid].name))==0)
		{ 
			fputc('\n',fsm.fout);
			(keywords[mid].fn)();
			return;
		}
		if (cc<0) hi=mid-1;
		else lo=mid+1;
	}
	fsm_error(fsm.names,fsm.sklineno,ERR_SKE,str); /* Erreur du squelette */
}
