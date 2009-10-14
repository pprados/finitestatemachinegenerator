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
#include "autoyac.h"
#include "conv.h"
#include "skeleton.h"
#include "readske.h"
#include "tools.h"
#include "error.h"

#if (_MSC_VER>=8)
#pragma warning(disable:4102)
#endif

#undef YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) fsm_fatal(fsm.namei,fsm.line,ERR_LEX,(msg))

#undef	YY_DECL
#define YY_DECL 	static int   squelette_lex()

#undef	YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if (ske_gets(buf, max_size, yyin) != NULL) \
	    result = strlen(buf); \
	else if ((yyin==NULL) || !ferror(yyin)) \
	    result = YY_NULL; \
	else \
	    YY_FATAL_ERROR("fgets() in flex scanner failed");


%}

alphanum	[a-zA-Z0-9_]

%x COMMENT
%option noyywrap

%%		/* - Debut du Lex - */

\${alphanum}*\$	{
		conv_token(yytext);
		fprintf(yyout,"\n");
		if (fsm.fl.line) aff_line(yyout,fsm.sklineno,fsm.names);
	}

^[ \t]*#.*\n {
		fsm.sklineno++;
		if (((!fsm.mode) && (fsm.fl.line)) || strncmp("#line",strchr(yytext,'#'),5))
			ECHO;
	}

(\n|"//".*\n) {
		fsm.sklineno++;
		ECHO;
	}

\"([^"\\\n]|\\.)*\"|\'([^'\\\n]|\\.)*\' {
    ECHO;
  }

"/*" {
		ECHO;
		BEGIN(COMMENT);
	}
<COMMENT>"/*" {
		ECHO;
	}
<COMMENT>([^*\n/]+|"*") {
		ECHO;
	}
<COMMENT>\n {
		fsm.sklineno++;
		ECHO;
	}
<COMMENT>"*/" {
		ECHO;
		BEGIN(0);
	}
<COMMENT>. ECHO;

. ECHO;

%%

/******************/
/* conv_squelette */
/******************/

/* - Copie le squelette et convertie les cles "$...$" ou "$..." - */

void		conv_squelette()
{
	yyout=fsm.fout;
	if (!fsm.mode)
	{
		if ((yyin=fopen(fsm.names,"r" FTXT))==NULL)
		{
			fsm_error(fsm.names,0,ERR_FILE,strerror(errno));
			return;
		}
	}
	else
	{
		yyin=NULL;
	}
	fsm.sklineno=1;

	BEGIN(0);
	YY_NEW_FILE;
	squelette_lex();
	if (yyin!=NULL) fclose(yyin);
	yy_delete_buffer(YY_CURRENT_BUFFER);
}
