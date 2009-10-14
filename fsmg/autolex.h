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

/*************/
/* autolex.h */
/*************/

extern FILE* 		yyin;
extern FILE* 		yyout;

bool	open_lex(void);
int 	yylex(void);
void	chg_event(ty_var_event* pte);
void	begin_tail(void);
void    close_lex(void);

/***************/
/* reset_buf_C */
/***************/

#define reset_buf_C() fsm.bufC=NULL;fsm.maxC=fsm.lenC=0;

/**************/
/* free_buf_C */
/**************/

#define free_buf_C()	\
{						\
	free(fsm.bufC);		\
	reset_buf_C();		\
}

