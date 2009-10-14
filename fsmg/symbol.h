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

/************/
/* SYMBOL.H */
/************/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{ 
	int	nb_hash;	/* Nombre de H-Code  */
	size_t sclef;		/* Taille de la clef */
	char 	thash[1];
} SYMBOL;

SYMBOL* 	open_symbol(size_t sclef,int nb_hash);
void* 		add_symbol	(SYMBOL* tbl,const char* symbol,size_t size,void* pt);
bool		sub_symbol  (SYMBOL* tbl,const char* symbol);
void* 		find_symbol (SYMBOL* tbl,const char* symbol);
void	 	close_symbol(SYMBOL* tbl);

#define key_symbol(tbl,pt)			((char*)(pt)-(tbl)->sclef)

#ifdef __cplusplus
}
#endif
