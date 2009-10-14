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
#include <malloc.h>
#include <errno.h>
#include <assert.h>

//#define DEBUG_0
#include "all.h"
#include "ytree.h"
#include "autoyacc.h"
#include "autolex.h"
#include "error.h"
#include "tools.h"

#define	SIZE_HKEY	(20)
#define MAX_HASH	(40)

static int  		nb_comment;			/* Compteur de commentaire */
static int  		nb_aco;				/* Compteur d'accolade */
static ty_exe		exe;
static ty_headc*	headc;

static int	 	get_event(const char* text,bool dcl);
static int	 	get_state(const char* text);

ty_fsm		fsm;

#if (_MSC_VER>=8)
#pragma warning(disable:4102)
#endif

#undef  YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) fsm_fatal(fsm.namei,fsm.line,ERR_LEX,(msg))
#undef	YY_DECL
#define YY_DECL		int yylex()

dmem(exe)
dmem(headc)

/*************/
/* add_buf_C */
/*************/

void add_buf_C(const char* str)
{
	size_t l=strlen(str);
	if (l+fsm.lenC>=fsm.maxC)
	{
		char *obuf=fsm.bufC;
		fsm.bufC=realloc(fsm.bufC,fsm.maxC+=BUFSIZ);
		if (fsm.bufC==NULL)
		{
			free(obuf);
			fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
		}
	}
	strcpy(fsm.bufC+fsm.lenC,str);
	fsm.lenC+=l;
}


%}

num		[0-9]
alpha		[a-zA-Z_]
alphanum	[a-zA-Z0-9_]
blank		[ \t]

%x	CBEGIN
%x	CCBEGIN
%x	EXEC
%x	COMMENT
%x  TAIL

%option noyywrap

%%		/* - Debut du Lex - */


	/* - Detection des tokens du langage - */
"%%" {
		d0(dputs("MARK\n"));
		fsm.line_end=fsm.lineno;
		assert(fsm.bufC==NULL);
		return(MARK);
	}

<TAIL>.* {
		add_buf_C(yytext);
	}

<TAIL>\n {
		add_buf_C(yytext);
	}

"%start" {
		d0(dputs("CSTART\n"));
		return(CSTART);
	}

"%extern" {
		d0(dputs("CEXTERN\n"));
		return(CEXTERN);
	}

"%priority"[ \t\n]+"event" {
		d0(dputs("CPRIOEVENT\n"));
		return(CPRIOEVENT);
	}

"%priority"[ \t\n]+"state" { /* Pas de commentaire entre ! */
		d0(dputs("CPRIOSTATE\n"));
		return(CPRIOSTATE);
	}

"%max"[ \t\n]+[0-9]+ {
		fsm.max_ext=atou(yytext+4); /* Memorise le max */
		d0(dputs("CMAX\n"));
		return(CMAX);
	}

	/* - STATE - */
\${alphanum}+ {
		d0(dputs("state\n"));
		return(get_state(yytext));
	}

	/* - EVENT loc - */
{alpha}{alphanum}* {
		d0(dputs("event\n"));
		return(get_event(yytext,false));
	}

	/* - EVENT ext - */
(\'([^\'\\\n]|\\.)+\'|{num}+) {
		d0(dputs("event ext\n"));
		return(get_event(yytext,true));
	}

{blank}+ ;

\n	{
		++fsm.lineno;
	}

	/* - Gestion des commentaires FSM - */
	/* Commentaire une ligne */
"//"[^\n]*\n {
		++fsm.lineno;
	}

"/*" {
		++nb_comment;
		BEGIN(COMMENT);
	}

<COMMENT>"/*" {
		++nb_comment;
	}

<COMMENT>([^*\n/]+|"*") ;
<COMMENT>\n {
		++fsm.lineno;
	}

<COMMENT>"*/" {
		if (!(--nb_comment)) BEGIN(0);
	}

	/* - Detection d'un bloc C encadre par "%{" et "%}" - */
"%{" {
		d0(dputs("CBEGIN\n"));
		BEGIN(CBEGIN);
		if ((headc=malloc(sizeof(*headc)))==NULL)
			fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
		dinc(headc);
		headc->line=fsm.lineno;
		headc->next=NULL;
	}

<CBEGIN>\n {
		++fsm.lineno;
		add_buf_C(yytext);
	}

<CBEGIN>^{blank}*#[.|\\\n]*\n {
		++fsm.lineno;
		add_buf_C(yytext);
	}

<CBEGIN>(\"([^"\\\n]|\\.)*\"|\'([^'\\\n]|\\.)+\'|"%") {
		add_buf_C(yytext);
	}

<CBEGIN>"%}" {
		ty_headc* pt;

		BEGIN(0);
		d0(dputs("X:CHEADER\n"));
		headc->buf=fsm.bufC;
		reset_buf_C();

		/* - Place le headeur a la fin de la liste - */
		pt=fsm.headc;
		if (pt==NULL)
		{
			fsm.headc=headc;
		}
		else
		{
			for (;pt->next!=NULL;pt=pt->next);
			pt->next=headc;
		}

		d0(dputs("CHEADER\n"));
		return(CHEADER);
	}

	/* - Gestion des commentaires dans CBEGIN - */
<CBEGIN>"//"[^\n]*\n {
		++fsm.lineno;
		add_buf_C(yytext);
	}

<CBEGIN>"/*" {
		++nb_comment;
		add_buf_C(yytext);
		BEGIN(CCBEGIN);
	}

<CBEGIN>. {
		add_buf_C(yytext);
	}

<CCBEGIN>"/*" {
		++nb_comment;
		add_buf_C(yytext);
  }
<CCBEGIN>[^*\n/]+ {
		add_buf_C(yytext);
	}
<CCBEGIN>\n {
		++fsm.lineno;
		add_buf_C(yytext);
  }
<CCBEGIN>"*/" {
		if (!(--nb_comment)) BEGIN(CBEGIN);
		add_buf_C(yytext);
  }

<CCBEGIN>. {
		add_buf_C(yytext);
	}

	/* - Detection d'un bloc C encadre par "{" et "}" - */
"{" {
		if ((yylval.exe=malloc(sizeof(*yylval.exe)))==NULL)
		fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
		dinc(exe);
		yylval.exe->line=fsm.lineno;
		yylval.exe->next=NULL;
		++nb_aco;
		add_buf_C(yytext);
		BEGIN(EXEC);
  }

<EXEC>"{" {
		++nb_aco;
		add_buf_C(yytext);
  }

<EXEC>^{blank}*#.*\n {
		++fsm.lineno;
		add_buf_C(yytext);
  }

<EXEC>(\"([^"\\\n]|\\.)*\"|\'([^'\\\n]|\\.)*\') { /* Si string */
		add_buf_C(yytext);
	}
<EXEC>\n {
		++fsm.lineno;
		add_buf_C(yytext);
	}
<EXEC>"}" {
		add_buf_C(yytext);
		if (!(--nb_aco))
		{
			if (fsm.first_exe==NULL)
			{
				fsm.first_exe=fsm.last_exe=yylval.exe;
			}
			else
			{
				fsm.last_exe->next=yylval.exe;
				fsm.last_exe=yylval.exe;
			}
			yylval.exe->exe=fsm.nb_exe++;
			yylval.exe->buf=fsm.bufC;
			reset_buf_C();
			BEGIN(0);
			d0(dputs("X:EXECUTION\n"));
			d0(dputs("EXECUTION\n"));
			return(EXECUTION);
		}
	}

<EXEC>.	{
		add_buf_C(yytext);
	}

	/* - Traitement par default - */
.	{
		return(yytext[0]);
	}
%%

/*************/
/* get_state */
/*************/

/* - Retourne le numero de l'event dans la liste. Modifie nb_state si */
/* necessaire.													 	  */


static int get_state(const char* text)

{
	ty_var_state	var_state;

	if ((yylval.state=find_symbol(fsm.symtbl_state,text))==NULL)
	{
		var_state.num=fsm.nb_state++;
		var_state.next=NULL;
		var_state.triple=NULL;
		var_state.use=false;
		yylval.state=add_symbol(fsm.symtbl_state,
					text,sizeof(var_state),&var_state);
		if (yylval.state==NULL) fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
		if (fsm.first_state==NULL)
		{
			fsm.first_state=fsm.last_state=yylval.state;
		}
		else
		{
			fsm.last_state->next=yylval.state;
		}
		fsm.last_state=yylval.state;
	}
	return(STATE);
}

/*************/
/* add_event */
/*************/

static void	 add_event(ty_var_event* pte)
{
	if (fsm.first_event==NULL)
	{
		fsm.first_event=fsm.last_event=pte;
	}
	else
	{
		ty_var_event* oevent;
		ty_var_event* vevent;

		for (oevent=NULL,vevent=fsm.first_event;
			 vevent!=NULL;
			 oevent=vevent,vevent=vevent->next)
		{
			if (yylval.event->num<vevent->num) break;
		}
		if (oevent==NULL)
		{
			fsm.first_event=pte;
		}
		else
		{
			oevent->next=pte;
		}
		pte->next=vevent;

	}
}

/*************/
/* get_event */
/*************/

/* - Retourne le numero de l'event dans la liste. Modifie nb_even si */
/* necessaire. */

static int get_event(const char* text,
					 bool	dcl	/* true si externe, false sinon */
				    )
{
	ty_var_event	var_event;

	if ((yylval.event=find_symbol(fsm.symtbl_event,text))==NULL)
	{
		++fsm.nb_event;
		var_event.num=(dcl) ? fsm.max_ext+fsm.nb_translate++ : fsm.nb_local++;
		var_event.dcl=dcl;
		var_event.triple=NULL;
		var_event.next=NULL;
		yylval.event=add_symbol(fsm.symtbl_event,text,sizeof(var_event),&var_event);
		if (yylval.event==NULL) fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
		add_event(yylval.event);
	}
	return(EVENT);
}

/*************/
/* chg_event */
/*************/

/* - Change le status d'un evenement - */

void		chg_event(ty_var_event* pte)
{
	ty_var_event* oevent;
	ty_var_event* vevent;

	for (oevent=NULL,vevent=fsm.first_event;
			 vevent!=pte;
			 oevent=vevent,vevent=vevent->next);
	if (oevent==NULL) fsm.first_event=pte->next;
	else oevent->next=pte->next;
	pte->next=NULL;
	pte->dcl=true;
	fsm.nb_local--;
	pte->num=fsm.max_ext+fsm.nb_translate++;
	if (fsm.first_event==pte) fsm.first_event=NULL;
	add_event(pte);
}

/************/
/* open_lex */
/************/

/* - Ouvre les 2 tables de symboles en H-code - */

bool open_lex()

{
	nb_comment=0;			/* Compteur de commentaire */
	nb_aco=0;				/* Compteur d'accolade */

	BEGIN(0);

	yyin=fsm.fin;
	yyout=fsm.fout;

	YY_NEW_FILE;

	fsm.symtbl_event=open_symbol(SIZE_HKEY,MAX_HASH);
	if (fsm.symtbl_event==NULL)
	{
		fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
		return(true);
	}
	fsm.symtbl_state=open_symbol(SIZE_HKEY,MAX_HASH);
	if (fsm.symtbl_state==NULL)
	{
		fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
		return(true);
	}
	fsm.nb_exe=2; 			/* 0 : Error, 1 : Nothing */
	get_state("");			/* Creation de l'event vide */
	get_event("",false);	/* Creation de l'etat vide */
	get_event("~",false);	/* Creation de l'event "autre" */

	reset_buf_C();

	return(false);
}

/**************/
/* begin_tail */
/**************/

void begin_tail()
{
	BEGIN(TAIL);
}

/*************/
/* close_lex */
/*************/

/* - ferme les buffers - */

void close_lex(void)
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
	free_buf_C();
}
