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



#include <malloc.h>
#include <assert.h>

//#define DEBUG_1
#include "all.h"

#ifndef NDEBUG
extern dmem(headc)
extern dmem(triple)
#endif

/*************/
/* clear_exe */
/*************/

void		clear_exe()
{ 
	ty_exe* exe;
	ty_exe*	next_exe;

	for (exe=fsm.first_exe;exe!=NULL;)
	{ 
		next_exe=exe->next;
		free(exe->buf);
		free(exe);
		exe=next_exe;
	}
	fsm.nb_exe=0;
	fsm.first_exe=fsm.last_exe=NULL;
}

/***************/
/* clear_headc */
/***************/

void		clear_headc()
{ 
	ty_headc* headc;
	ty_headc*	next_headc;

	for (headc=fsm.headc;headc!=NULL;)
	{ 
		next_headc=headc->next;
		free(headc->buf);
		free(headc);
		ddec(headc);
		headc=next_headc;
	}
	fsm.headc=NULL;
}

/*****************/
/* clear_triples */
/*****************/

/* - Nettoyage des triples - */

void		clear_triples()
{ 
	ty_triple*    triple;
	ty_triple*    next_triple;
	ty_var_event* event;

	for (event=fsm.first_event;event!=NULL;event=event->next)
	{
		d1(dprintf(("Parcourt event:%s\n",key_symbol(fsm.symtbl_event,event))));
		for (triple=event->triple;triple!=NULL;)
		{ 
			d1(dprintf(("free(%p)\n",triple)));
			next_triple=triple->next_event;
			free(triple); // BUG : _ffree(triple)
			triple=next_triple;
			ddec(triple);
		}
	}
	fsm.nb_state=fsm.nb_event=fsm.nb_translate=fsm.nb_local=0;
	fsm.triple=NULL;
	fsm.first_state=fsm.last_state=NULL;
	fsm.first_event=fsm.last_event=NULL;
	assert(dval(triple)==0);
}
