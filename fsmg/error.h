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

/***********/
/* error.h */
/***********/

typedef enum 
{ 
	ERR_MEMORY=1,ERR_SYNTAX,ERR_YACCOVERFLOW,
	ERR_MORE,ERR_CONV,ERR_SKE,
	ERR_NOSTATE,ERR_NOEVENT,ERR_BADSTART,ERR_STARTTWICE,
	ERR_FILE,
	ERR_LEX
} fsm_err;
typedef enum { ERR_LEX_INT=1,ERR_LEX_EOF,ERR_LEX_JAMMED } fsm_int; /* internal */
typedef enum { WAR_PRIO=1,WAR_EXTERN, WAR_STATE } fsm_war;

void  	fsm_warning (const char* file,int line,fsm_war num,...);
void  	fsm_error 	(const char* file,int line,fsm_err num,...);
void  	fsm_fatal 	(const char* file,int line,fsm_err num,...);
void  	fsm_internal(const char* file,int line,fsm_int num,...);
