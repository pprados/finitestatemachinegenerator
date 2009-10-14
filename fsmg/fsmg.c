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

/*
  Pour la version 2.0, faire deux squelettes coupl�, un pour le .c et un pour le .h.
	Faire des squelettes pour le C++ ou l'�tat est un attribut. Voir l'h�ritage d'automate
	avec les pb d'ajout de nouveau evenement diff. des pr�c�dents.
	(Generation automatique par varaible globale ?)

	Faire des squelettes pour Java, avec la m�me philo que pour C++
 */
// TODO : bug si extern non utilis� dans un fsm
// TODO : bug si '\0' en evenement

// TODO : Ajouter la generation de filtresk, puis l'utilisation avant le creat
// TODO : Faire un exemple avec ftp et/ou POP3

#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <sys/stat.h>
#include <getopt.h>
#include <time.h>
#include <assert.h>

#include "all.h"
#ifdef HAVE_IO_H
#include <io.h>
#else
#ifdef HAVE_SYS_IO_H
#include <sys/io.h>
#endif
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#ifdef HAVE_SYS_DIRENT_H
#include <sys/dirent.h>
#endif
#endif

#include "tools.h"
#include "error.h"
#include "translate.h"

static const char _CopyRight[]="(C)opyright Ph. PRADOS 2000";
#define LOGO(x) printf("%s %s, %s\n\n",argv[0],(x),_CopyRight)

static const char  usage[]=
	"Usage: %s [-cehlstvw] [-D<s>] [-k<f>] [-m<n>] [-p[<f>]] [-r<f>] [-x<n>] [-o <outfile|dir|->] <infiles|dir>* \n"
	"\t-c       --check-only        : no write file, check syntax only\n"
	"\t-D<s>    --define <s>        : Add a \"#define\"\n"
	"\t-e       --priority-event    : event prio\n"
	"\t-h       --help              : print this help\n"
	"\t-k<f>    --skelette-file <f> : use file for skelete\n"
	"\t-l       --no-lines          : Supressed \"#line\" directives\n"
	"\t-m<0..4> --skelette <0..4>   : Select standard skelette.\n"
	"\t-m0      --All               : Select one (default)\n"
	"\t-m1      --ArrSwitch         : Array+switch\n"
	"\t-m2      --FnArr             : Array+function\n"
	"\t-m3      --ArrFnArr          : Array+Array+function\n"
	"\t-m4      --Switch            : Switch+switch\n"
	"\t-o<f>    --output            : Output file, directory or stdout\n"
	"\t-p<f>    --header <f>        : Produce a header file\n"
	"\t-r<f>    --error-file <f>    : Copy error and warning in file\n"
	"\t-s       --size              : favors size\n"
	"\t-t       --time              : favors time\n"
	"\t-v       --verbose           : print file name\n"
	"\t-V       --version           : print version\n"
	"\t-w       --no-warning        : no warning\n"
	"\t-x<n>    --max-external <n>  : Set maximum value for extern event\n"
	"\t<In>                         : Translate files <In> ...\n"
	"\t<Out|dir|->                  : ... to file <Out>,\n"
	"\t                               in directory <dir>, or to stdout\n"
	"\nReport bugs to philippe@prados-obj.nom.fr\n";

/**********/
/* sigint */
/**********/

/* - Gestion du Ctrl-Break - */

static RETSIGTYPE sigint(int sig)
{
	fputs("\nUser break !\n",stderr);       /* System re-entrant */
	exit(1);
}

ty_fsm     	fsm; /* Context d'execution */

/**************/
/* simplefsmg */
/**************/

static int simplefsmg(const char* namei,const char* nameo)
{
	struct stat		statbuf;
	static char		drive[MAX_DRIVE];
	static char		dir[MAX_DIR];
	static char		fname[MAX_FNAME];
	static char		ext[MAX_EXT];

	strcpy(fsm.namei,namei);
	add_ext(fsm.namei,"." EXT);

	// out is directory ?
	if (strcmp(nameo,"-"))
	{
		statbuf.st_mode=0;
		stat(nameo,&statbuf);
		if ((statbuf.st_mode & 0770000)==S_IFDIR)
		{
			_splitpath(namei,drive,dir,fname,ext);
			strcpy(fsm.nameo,nameo);
#if defined(_WIN32)
			strcat(fsm.nameo,"\\");
			strcat(fsm.nameo,fname);
			if (!stricmp(ext,"." EPP))      strcat(fsm.nameo,".cpp");
			else if (!stricmp(ext,"." EXX)) strcat(fsm.nameo,".cxx");
			else strcat(fsm.nameo,".c");
#else
			strcat(fsm.nameo,"/");
			strcat(fsm.nameo,fname);
			if (!strcmp(ext,"." EPP))       strcat(fsm.nameo,".cpp");
			else if (!strcmp(ext,"." EXX))  strcat(fsm.nameo,".cxx");
			else strcat(fsm.nameo,".c");
#endif

		}
		else
		{
			strcpy(fsm.nameo,nameo);
		}
	}
	else
	{
		*fsm.nameo='\0';
	}
	if (fsm.fl.verbose) fprintf(stderr,"convert %s to %s:\n",fsm.namei,fsm.nameo);
	return translate();                  /* Traduction du source .fsm en .c */
}

/********/
/* fsmg */
/********/

int fsmg(const char* namei,const char* nameo)
{
	struct stat	statbuf;
	char*		pt;
	char*		pt2;
	int		rc=0;
	bool		filtre=false;
	int		r=0;

#if defined(HAVE_SYS_DIRENT_H) || defined(HAVE_DIRENT_H)
	DIR*		handle;
	struct dirent*	ent;
#endif
#ifdef _MSC_VER
	struct _finddata_t	fileinfo;
	long			handle;
	int			ret;
#endif

	// in is directory ?
	statbuf.st_mode=0;
	stat(namei,&statbuf);
#ifdef _MSC_VER
	if ((strchr(namei,'*')!=NULL) || (strchr(namei,'?')!=NULL))
	{
		filtre=true;
	}
#endif
	if (filtre || (statbuf.st_mode & 0770000)==S_IFDIR)
	{
		char		name[MAX_PATH];

		strcpy(name,namei);

		if (filtre)
		{
			pt=strrchr(name,'/');
#ifdef _WIN32
			if (pt==NULL)
			{
				pt=strrchr(name,'\\');
			}
#endif
			if (pt==NULL)
				pt=strchr(name,'\0');
			else
				++pt;
		}
		else
			pt=strchr(name,'\0');

		if (!filtre)
		{
			if ((*(pt-1)!='\\') && (*(pt-1)!='/'))
			{
#ifdef _WIN32
				*pt++='\\';
#else
				*pt++='/';
#endif
			}
		}

#ifdef HAVE_SYS_DIRENT_H
		// Find *.fsm
		for (handle=opendir(namei),ent=readdir(handle);
			 (ent!=NULL);
			 ent=readdir(handle)
			)
		{
			pt2=strrchr(ent->d_name,'.');
			if ((pt2==NULL) || (stricmp(pt2,"." EXT)))
				continue;
			strcpy(pt,ent->d_name);
			rc=simplefsmg(name,nameo);
			if (rc) r=rc;
		}
		closedir(handle);
#endif // HAVE_DIRENT_H
#ifdef _MSC_VER
		if (!filtre) strcpy(pt,"*.fsm");
		for (handle=_findfirst(name,&fileinfo),ret=handle;
				 ret!=-1;
				 ret=_findnext(handle,&fileinfo))
		{
			pt2=strrchr(fileinfo.name,'.');
			if ((pt2==NULL) || (stricmp(pt2,"." EXT)))
				continue;
			strcpy(pt,fileinfo.name);
			rc=simplefsmg(name,nameo);
			if (rc)	r=rc;
		}
		_findclose(handle);
#endif
		return r;
	}
	else
	{
		rc=simplefsmg(namei,nameo);
	}
	return rc;
}

/********/
/* main */
/********/

int main(int argc,char* argv[])
{
	int		i;
	int     retmain=0;
	char*   bufout=NULL;
	char*	out;					/* Repertoire destination */
	char*	_name=argv[0];

	signal(SIGINT,sigint);

	fsm.output[0]='\0'; // No output
	switch(gere_param(argc,argv))
	{
		case 1 :
Error:
			errno=EINVAL;
			perror(_name);
			fprintf(stderr,usage,_name);
			free(fsm.bufdef);               /* Efface les defines */
			return(EINVAL);

		case 2 :
			LOGO(VERSION);
			printf(usage,_name);
			free(fsm.bufdef);               /* Efface les defines */
			return(0);

		case 3 :
			LOGO(VERSION);
			free(fsm.bufdef);               /* Efface les defines */
			return(0);
	}

	argc-=optind-1;
	argv+=optind-1;

	if (argc<2)
	{
		errno=EINVAL;
		goto Error;
	}

/* - Calcul les noms des fichiers In et Out - */
/* Si pas de out,                  */
/*   Si plus de 1, sortie fichier, */
/*   Si 1, sortie stdout           */

	out=fsm.output;
	if (!*out)
	{
		if (argc>2) out="";
		else out="-";
	}
	*fsm.nameo='\0';

#ifdef _MSC_VER
	printf(""); /* Bug with leasy init of Microsoft */
#endif


	if ((out[0]=='-') && (out[1]=='\0'))	/* Out sur stdout */
	{
		struct stat statbuf;

		fstat(fileno(stdout), &statbuf);
		if (statbuf.st_mode & S_IFREG)
		{
			bufout=malloc(BUFSIZ);	/* Use buffer if pipe */
			if (bufout!=NULL) setbuf(stdout,bufout);
		}
	}
	if (*fsm.nameerr)
	{
		freopen(fsm.nameerr,"w" FTXT,stderr);
	}

	/* - Boucle de gestions des fichiers - */
	while (--argc)
	{
		if ((i=fsmg(argv[1],out))!=0)
			retmain=i;
		argv++;
	}
	free(fsm.bufdef);                     /* Efface les defines */
	if (bufout!=NULL) free(bufout);       /* Si buffer pour pipe, vide */

	return(retmain);
}
