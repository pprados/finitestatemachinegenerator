%{
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
#include <errno.h>
#include <assert.h>

#include "all.h"
#include "convexe.h"
#include "error.h"

#if (_MSC_VER>=8)
#pragma warning(disable:4102)
#endif

#undef  YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) fsm_fatal(fsm.namei,fsm.line,ERR_LEX,(msg))
#undef	YY_DECL
#define YY_DECL 	 static int exec_lex()

/* Gets input and stuffs it into "buf".  number of characters read, or YY_NULL,
 * is returned in "result".
 */
#define YY_INPUT(buf,result,max_size)					\
  if (!*current_buf) result=YY_NULL;					\
	else								\
	{	strncpy(buf,current_buf,max_size);			\
		result = (size_buf>max_size) ? max_size : size_buf;	\
		current_buf+=result;					\
		size_buf-=result;					\
	}

static const char* current_buf;
static int		size_buf;
static bool		in_line=false;

%}

alphanum	[a-zA-Z0-9_]

%x COMMENT

%option lex-compat
%option noyywrap

%%

"}" {			/* Teste la fin du bloque */
	ECHO;
}

\${alphanum}*[$]? {	/* detection de mot a convertir */
	ty_var_state *pts;

	if (!strcmp(yytext,"$state$"))
	{ fprintf(yyout,(in_line) ? " *fsm_state " : " fsm_state ");
	}
	else if (!strcmp(yytext,"$event$"))
	{ fprintf(yyout,(in_line) ? " *fsm_event " : " fsm_event ");
	}
	else
	{ pts=find_symbol(fsm.symtbl_state,yytext);
		if (pts==NULL)
		{ fsm_error(fsm.namei,fsm.lineno,ERR_CONV,yytext);
		}
		else
		{ fprintf(yyout,"%d",pts->num-1);
			pts->use=true;
		}
	}
}

(^[ \t]*#.*\n|"//".*\n) { /* Preprocesseur et des commentaires C++ */
	++fsm.lineno;
	ECHO;
}

\"([^"\\\n]|\\.)*\"|\'([^'\\\n]|\\.)*\' { /* Chaines et caracteres */
  ECHO;
}

"/*" {				/* Debut de commentaire */
	BEGIN(COMMENT);
	ECHO;
}

<COMMENT>"/*" {        	       	/* Commentaire imbrique */
  ECHO;
}

<COMMENT>([^*\n/]+|"*")      	{
  ECHO;
}

<COMMENT>\n {
  ++fsm.lineno;
  ECHO;
}

<COMMENT>"*/" {			/* Fin de commentaire */
  BEGIN(0);
  ECHO;
}

\n {				/* Fin de ligne */
  ++fsm.lineno;
  ECHO;
}

. {
	ECHO;
}

%%

/*************/
/* print_exe */
/*************/

void print_exe(ty_exe *exe,bool inl)
{
	in_line=inl;

	BEGIN(0);
	YY_NEW_FILE;
	yyout=fsm.fout;

	current_buf=exe->buf;
	size_buf=strlen(current_buf);

	fsm.lineno=exe->line;
	exec_lex();
}

#ifdef VLEX
/*************/
/* close_exe */
/*************/

/* - ferme les buffers - */

void close_exe(void)
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
}
#endif
