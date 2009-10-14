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

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#include "all.h"


/*
	- "open_symbol" cree une structure SYMBOL en ajoutant a la fin un tableau
		de type ty_hsym qui est la table de H-Code. "thash" doit etre caste car
		pour eviter de declarer pour l'exterieur "ty_hsym" il est declare dans
		"symbol.h" en char. Il est donc possible avec ces routines de creer
		plusieur table de symbol avec un H-Code adapte et donc d'adapter la
		compilation au contexte. La fonction "add_symbol" permet de rajouter
		un symbol dans la table et de lui ajouter "size" element qui seront
		recopier depuis "pt". Un pointeur sur ces elements serat fournie par
		"find_symbol" pour pouvoir les exploiter. Ceci permet de rajouter
		un type ou toutes informations a associer a une variable.
		"close_symbol" efface tous les mallocs utilisees pour la table.
*/

typedef int ty_hash;		/* Type valide pour le H-code */
typedef struct st_sym 
{ 
	struct st_sym* next;
	char		clef[1];
} ty_hsym;/* Liens sur les symbols */
#define SYM(tbl,sym) ((char*)(&(sym)->clef)+(tbl)->sclef)

/********/
/* hash */
/********/

/* Calcul du H-code */

static ty_hash   hash(const char* key,int nb_hash)
{ 
	ty_hash	hashval;

	for (hashval=0;*key!='\0';) hashval+=*key++;
	return(hashval % nb_hash);
}

/***************/
/* open_symbol */
/***************/

/* Creation d'une liste de symbol gere par un H-code de "nb_hash" valeur */

SYMBOL* 	open_symbol(size_t	sclef,int	nb_hash)
{ 
	int	  i;
	SYMBOL* tbl;
	if ((tbl=malloc(sizeof(*tbl)+nb_hash*(sizeof(ty_hsym)+sclef)))==NULL) // 884
	{ 
		return(NULL);
	}

	tbl->nb_hash=nb_hash;
	tbl->sclef=sclef;
	for (i=0;i<nb_hash;i++) ((ty_hsym **)(tbl->thash))[i]=NULL;
	return(tbl);
}      

/**************/
/* add_symbol */
/**************/

/* - Rajoute un symbol dans la table "tbl" avec une copie de *pt - */

void* 	add_symbol(
	SYMBOL* tbl,
	const char* symbol,
	size_t	size,
	void*  pt
	)
{ 
	ty_hsym* sym;
	ty_hash  hashcode=hash(symbol,tbl->nb_hash);

	sym=malloc(sizeof(*sym)+tbl->sclef+size);
	if (sym==NULL) return(NULL);
	sym->next=((ty_hsym **)(tbl->thash))[hashcode];
	sym->clef[tbl->sclef]='\0';
	strncpy(sym->clef,symbol,tbl->sclef-1);
	if (size) memcpy(SYM(tbl,sym),pt,size);
	((ty_hsym **)(tbl->thash))[hashcode]=sym;
	return(SYM(tbl,sym));
}

/***************/
/* find_symbol */
/***************/

/* - Retourne un pointeur sur la donnee du symbole si il existe dans "tbl",
		 sinon retourne NULL - */

void* 	find_symbol(
	SYMBOL* tbl,
	const char* symbol
	)
{ 
	ty_hsym* sym;
	
	for (sym=((ty_hsym **)(tbl->thash))[hash(symbol,tbl->nb_hash)];
			 (sym!=NULL) && strcmp(sym->clef,symbol);
			 sym=sym->next);
	return((sym!=NULL) ? SYM(tbl,sym) : NULL);
}

/**************/
/* sub_symbol */
/**************/

/* - Suprime un symbol dans la table "tbl" avec une copie de *pt - */

bool 	sub_symbol(
	SYMBOL* tbl,
	const char* symbol
	)
{ 
	ty_hsym* sym;
	ty_hsym* prev=NULL;
	ty_hash  hashcode=hash(symbol,tbl->nb_hash);

	for (sym=((ty_hsym **)(tbl->thash))[hashcode];
			 (sym!=NULL) && strcmp(sym->clef,symbol);
			 prev=sym,sym=sym->next);
	if (sym!=NULL)
	{ 
		if (prev==NULL)
		{ 
			((ty_hsym **)(tbl->thash))[hashcode]=sym->next;
		}
		else
		{ 
			prev->next=sym->next;
		}
		free(sym);
		return(false);
	}
	return(true);
}

/****************/
/* close_symbol */
/****************/

/* - Efface une table de symboles - */

void 	close_symbol(SYMBOL* tbl)
{ 
	int    i;
	ty_hsym* sym;
	ty_hsym* nsym;

	for (i=0;i<tbl->nb_hash;i++)
	{ 
		for (sym=((ty_hsym **)(tbl->thash))[i];sym!=NULL;)
		{ 
			nsym=sym->next;
			free(sym);
			sym=nsym;
		}
	}
	free(tbl);
}
