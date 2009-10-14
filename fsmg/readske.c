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


#include <assert.h>

#include "all.h"
#include "readske.h"

extern const unsigned char  All[];
extern const unsigned char  ArrSwitc[];
extern const unsigned char  FnArr[];
extern const unsigned char  ArrFnArr[];
extern const unsigned char  Switc[];

static const char* squelette[]={All,ArrSwitc,FnArr,ArrFnArr,Switc};

/* ---------------------------------------- Version Lex ------------------------------------------- */
#ifdef VLEX
#ifdef _MSC_VER
#pragma message("*** Use Lex for squelette. ***")
#endif
/************/
/* ske_gets */
/************/

/* - Lecture des squelettes interne ou externe - */

char* 	ske_gets(char* buf,size_t max_size,FILE* std)
{ 
	char		c;
	size_t i=0;

	if (std!=NULL) 
	  return(fgets(buf,max_size,std));

	while ((c=squelette[fsm.mode-1][fsm.pos_ske++]) && (i<max_size-1))
	{ 
		buf[i]=c;
		i++;
		if (c=='\n')
		{ buf[i]='\0';
			return(buf);
		}
	}
	buf[i]=EOF;
	buf[i+1]='\0';
	return(NULL);
}

#else

/* ------------------------------------------ Version FSM ------------------------------------------- */
static	char	mem_lastc=-1;

/************/
/* ske_getc */
/************/

int		ske_getc(FILE* std)
{ 
	char c;

	if (std!=NULL) return(fgetc(std));
	if ((c=mem_lastc)!=-1)
	{ 
		mem_lastc=-1;
		return(c);
	} 			 
	c=squelette[fsm.mode-1][fsm.pos_ske++];
	return((c!=0) ? c : EOF);
}

/**************/
/* ske_ungetc */
/**************/

int		ske_ungetc(int c,FILE* std)
{ 
	return((std!=NULL) ? ungetc((char)c,std) : (mem_lastc=(char)c));
}
#endif
