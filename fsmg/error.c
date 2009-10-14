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
#include <stdarg.h>
#include <assert.h>

#include "all.h"
#include "error.h"

#define NUM_FATAL	(9000)

/**************/
/* fsm_afferr */
/**************/

static void   fsm_afferr(
	const char* file,
	int     line,
	fsm_err num,
	va_list marker
	)
{ 
	static const char* fsm_strerr[]=
	{"not enought memory !\n",
	 "syntax error : %s\n",
	 "parser stack overflow !\n",
	 "%s/%s defined more than one !\n",
	 "\"%s\" isn't a state !\n",
	 "\"%s\" unknow !\n",
	 "no state defined !\n",
	 "no event defined !\n",
	 "%%start to no state !\n",
	 "%%start defined more than one !\n",
	 "%s\n", /* Erreur fichier */
	 "scanner failled :%s\n",
	};

	assert(num-1<sizeoftab(fsm_strerr));
	fprintf(fsm.ferr,"fsmg:%s:%hd: error FSM%04d : ",file,line,num);
	vfprintf(fsm.ferr,fsm_strerr[num-1],marker);
}

/*************/
/* fsm_error */
/*************/

void 	fsm_error(const char* file,int line,fsm_err num,...)
{ 
	va_list	marker;

 	va_start(marker,num);
	fsm_afferr(file,line,num,marker);
	va_end(marker);
	fsm.error=true;
}

/*************/
/* fsm_fatal */
/*************/

void 	fsm_fatal(const char* file,int line,fsm_err num,...)
{ 
	va_list	marker;

	va_start(marker,num);
	fsm_afferr(file,line,num,marker);
	va_end(marker);
	exit(1);
}

/****************/
/* fsm_internal */
/****************/

void 	fsm_internal(const char *file,int line,fsm_int num,...)
{ 
	va_list	marker;
	FILE*   stream;

	static const char* fsm_strint[]=
	{"scanner corrupted !\n",
	 "scanner saw EOF twice !\n",
	 "scanner jammed !\n",
	};

	assert(num-1<sizeoftab(fsm_strint));
	va_start(marker,num);
	fprintf(fsm.ferr,"fsmg:%s:%hd: sorry, internal error FSM%04d : ",file,line,
		NUM_FATAL+num);
	vfprintf(fsm.ferr,fsm_strint[num-1],marker);
	if ((stream=fopen("error.txt","w" FTXT))==NULL)
	{
		fprintf(fsm.ferr,
			"Send \"%s\" to pprados@club-internet.fr\a\n",file);
	}
	else
	{
		fputs("Fsmg creat \"error.txt\" file...\n",fsm.ferr);
		fprintf(stream,"fsmg:%s:%hd: sorry, internal error FSM%04d : ",file,line,
			NUM_FATAL+num);
		vfprintf(stream,fsm_strint[num-1],marker);
		fclose(stream);
		fprintf(fsm.ferr,"Send \"%s\" to pprados@club-internet.fr\a\n",file);
	}
	va_end(marker);
	exit(1);
}

/***************/
/* fsm_warning */
/***************/

void 	fsm_warning(const char* file,int line,fsm_war num,...)
{ 
	static const char* fsm_strwar[]=
	{"choose \"%s\" and not \"%s\"\n",
	 "%%extern \"%s\" not use !\n",
	 "state \"%s\" not use !\n",
	};
	va_list	marker;

	assert(num-1<sizeoftab(fsm_strwar));
	va_start(marker,num);
	fprintf(fsm.ferr,"fsmg:%s:%hd: warning FSM%04d : ",file,line,num);
	vfprintf(fsm.ferr,fsm_strwar[num-1],marker);
	va_end(marker);
}

