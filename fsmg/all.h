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

/*********/
/* all.h */
/*********/

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "debug.h"
#include "symbol.h"

#define VER_MAJOR	"1"
#define VER_MINOR	"1"
#define VERSION		"v" VER_MAJOR "." VER_MINOR ".0.0"

#define HFS 		"hfs"
#define EXT 		"fsm"
#define EPP 		"fpp"
#define EXX 		"fxx"

#define FSM_ERROR 		(0)
#define FSM_NOTHING 		(1)

typedef unsigned short ty_event;
typedef unsigned short ty_state;
typedef unsigned short ty_line;

#ifdef _MSC_VER // BUG
#define fpos_t long
#endif
/* ------------- Structure generales --------------- */

typedef struct st_headc 
{ 
	struct st_headc*	next;	/* Header C suivant */
	ty_line 		line; 	/* Premiere ligne du code */
	char*			buf;    /* Les lignes du header */
} ty_headc; 				/* Liste header C */

typedef struct st_exe 
{ 
	struct st_exe* 	next;		/* Exe suivant */
	ty_line 	line; 		/* Premiere ligne du code */
	char*		buf;		/* Les lignes du bloc */
	short 		exe;		/* Numero de l'exe */
} ty_exe;


typedef struct st_var_event
{ 
	struct st_var_event*	next;	/* Evenement suivant */
	ty_event 		num;	/* Numero d'identification */
	bool			dcl;	/* Declaration par "%extern" */
	struct st_triple*	triple;	/* Premier triple de l'event*/
} ty_var_event;

typedef struct st_var_state
{ 
	struct st_var_state*	next;	/* Etat suivant */
	ty_state		num;	/* Numero d'identification */
	struct st_triple*	triple;	/* Premier triple du state */
	bool			use;	/* Flag si utilise */
} ty_var_state;

typedef struct st_triple
{ 
	ty_var_event* 		event;		/* L'evenement */
	ty_var_state*		state; 		/* L'etat */
	ty_exe*			exe; 		/* L'execution */
	struct st_triple*	next_event;	/* Next du meme etat */
	struct st_triple*	next_state;
	ty_line 		line; 		/* Ligne de declaration */
} ty_triple;

typedef struct
{
	/* - Parametre d'initialisation - */
	char		major;			/* Version de FSM */
	char		minor;
	char		output[MAX_PATH];	/* Parametre output */
	char		namei[MAX_PATH];	/* Nom file input */
	char		nameo[MAX_PATH];	/* Nom file output */
	char		header[MAX_PATH];	/* Header des locaux */
	char		names[MAX_PATH];	/* Nom squelette */
	char		nameerr[MAX_PATH];	/* Nom fichier erreur */
	int 		mode; 			/* Squelette utilise 0:file */
	unsigned	max_ext;		/* Valeur maximum extern */
	struct 
	{ 
		unsigned char no_exec; 	/* 0:exec 1:No exec */
		unsigned char warn;	/* 1:Pas de warning */
		unsigned char line;	/* FALSE:No line TRUE:Line */
		unsigned char head;	/* 0:Pas, 1:Avec */
		unsigned char verbose; 	/* 0:No verbeux, 1:verbeux */
		unsigned char prio;	/* 0:State/Event 1:Event/State*/
		unsigned char opt; 	/* 0:Rien 1:Size 2:Len */
	 } fl;
	char*		bufdef;		/* Buffer des #defines */
	bool		incompile;

/* - Parametre de traitement - */
	ty_triple*	triple;		/* Triple courant */
	ty_line 	line; 		/* Ligne courante du source */

	unsigned	nb_state; 	/* Nb d'etat */
	unsigned	nb_event;	/* Nombre d'evenement */
	unsigned	nb_translate; 	/* Nb d'evenement externe */
	unsigned	nb_local; 	/* Nb local */
	bool		use_nothing;	/* Si utilise nothing */
	bool		use_error;	/* Si utilise error */
	ty_headc*	headc; 		/* Liste des headers C */

	ty_var_state*	first_state; 	/* Premier etat */
	ty_var_state*	last_state;	/* Dernier etat */
	SYMBOL*		symtbl_state;	/* Symbol etat */

	ty_var_event* 	first_event;	/* Premier even */
	ty_var_event* 	last_event; 	/* Dernier even */
	SYMBOL*		symtbl_event; 	/* Symbol even */

	unsigned	nb_exe; 	/* Nombre d'exec */
	ty_exe*		first_exe; 	/* Premier */
	ty_exe*		last_exe;	/* Dernier */

	ty_line 	last_line;	/* Derniere ligne utile */
	char*		tail;           /* Buffer de la fin du fichier */
	ty_line 	line_end; 	/* Num ligne de file */

	ty_line 	line_start; 	/* Ligne du start */
	ty_var_state*	start_state; 	/* L'etat de start */

	ty_line 	lineno; 	/* Numero de ligne source */
	FILE*		fin; 		/* Fichier source */
	FILE*		fout;		/* Fichier destination */
	FILE*		ferr;		/* Fichier erreur */

	int		pos_ske;	/* Position courante ske */
	ty_line 	sklineno; 	/* Ligne du skelette */

	enum {READING,WRITING,STOP} status;
	bool		error;		/* Flag si erreur */

	/* - buffer pour code C - */
	char*		bufC;
	size_t		maxC;
	size_t		lenC;

} ty_fsm;

extern ty_fsm 	fsm;
