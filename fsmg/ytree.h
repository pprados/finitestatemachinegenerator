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
/* ytree.h */
/***********/

/* ----------------------- Structures de generation -------------------- */
typedef struct st_temp	 
{ 
	struct st_temp* nextm;
	struct st_temp* prevm;
} ty_temp;

typedef struct st_lstate 
{ 
	ty_temp			t;
	ty_var_state* 		state;
	struct st_lstate* 	next;
} ty_lstate;

/* - Liste d'evenement - */
typedef struct st_levent  
{ 
	ty_temp			t;
	ty_var_event*		event;
	struct st_levent*	next;
} ty_levent;

/* - Liste de droite d'une regles - */
typedef struct st_rrule 
{ 
	ty_temp 		t;
	ty_levent*		levent;
	ty_exe*			exe;
	struct st_rrule* 	next;
} ty_rrule;

/* - Une regle - */
typedef struct 
{ 
	ty_temp			t;
	ty_lstate*		left;
	ty_rrule* 		right;
} ty_rule;
