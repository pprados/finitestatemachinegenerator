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


#ifdef _MSC_VER
#pragma warning(disable:4102)
#endif

#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

//#define DEBUG_0 /* Les regles niveaux yacc */
//#define DEBUG_1 /* Liste des etats et state apres chaque triple memorise */
//#define DEBUG_2 /* Recherche doublon */
//#define DEBUG_3 /* Final */
//#define DEBUG_4 /* Chaque triple memorise */
//#define DEBUG_5 /* Trace de l'effacement des triples */

#define YYDEBUG 1
#define YYERROR_VERBOSE

#include "all.h"
#include "ytree.h"
#include "autoyac.h"
#include "autolex.h"
#include "error.h"

dmem(yacc)
dmem(triple)

ty_temp		first_temp={&first_temp,&first_temp};

#define ADD_TEMP(pt) pt->t.nextm=first_temp.nextm; \
				 first_temp.nextm->prevm=&pt->t; \
				 first_temp.nextm=&pt->t; \
				 pt->t.prevm=&first_temp;

#define SUB_TEMP(pt) pt->t.nextm->prevm=pt->t.prevm; \
				 pt->t.prevm->nextm=pt->t.nextm;

/*************/
/* free_temp */
/*************/

/* - Efface les mallocs temporaire - */

void	free_temp()

{
	ty_temp	*p,*op;

	for (p=first_temp.nextm;p!=&first_temp;p=op)
	{
		op=p->nextm;
		free(p);
		ddec(yacc);
	}
	first_temp.nextm=first_temp.prevm=&first_temp;
}

#define  yyerror(s) fsm_error(fsm.namei,fsm.lineno,ERR_SYNTAX,s)

int			yyparse(void);

%}

%token MARK		/* Le marqueur "%%" */
%token CHEADER		/* Bloque C encadre par "%{"..."%}" */
%token CSTART		/* Le mot clef "%start" */
%token CEXTERN		/* Le mot clef "%extern" */
%token CPRIOEVENT	/* Le mot clef "%priority event" */
%token CPRIOSTATE	/* Le mot clef "%priority state" */
%token CMAX		/* Le mot clef "%max" */

%token STATE
%token EVENT
%token EXECUTION

%union {
	ty_rrule*     rrule;	/* Droite de la regle */
	ty_levent*    levent;	/* Liste d'evenement */
	ty_lstate*    lstate;	/* Liste d'etat */
	ty_var_state* state;	/* Numero du state */
	ty_var_event* event;	/* Numero de l'event */
	ty_exe*       exe;	/* Numero de l'exec */
	const char*   tail;     /* Buffer de fin de fichier */
}

%type <rrule>	rrule
%type <lstate>	states
%type <levent>	events
%type <exe>		EXECUTION exec
%type <event>	EVENT
%type <state>	STATE

%start prg

%%

/*******/
/* prg */
/*******/

/* - Le programme - */

prg	: defs MARK rules tail
	{
		d3(deb_event());d3(deb_state());
		free_temp();		/* Efface les parasites d'erreur */
		assert(dval(yacc)==0);	/* Tous est nettoye ? */
		fsm.tail=fsm.bufC;
		reset_buf_C();
		YYACCEPT;
#if 1 // BUG
	   /* clear buffer */
		free (yyss); // yyfree(yyss)
		yyss=0;
		free(yyvs);
		yyvs=0;
#endif

	}
	;

/********/
/* tail */
/********/

/* - Copie la fin du fichier - */

tail	: /* Pas de tail */
	| MARK
		{
			fsm.last_line=fsm.lineno;
			begin_tail();
			yylex();
		}
	;

/********/
/* defs */
/********/

/* - Les definitions avant les regles - */

defs	:
	| defs def	/* Une liste de definition */
	;

/*******/
/* def */
/*******/

/* - Une definition - */

def	:
	| CHEADER	/* Copie le CHEADER sur out (via Lex) */
	| CSTART STATE	/* "%start	<state>" */
	{
		if (fsm.start_state==NULL)
		{
			fsm.start_state=$2;
			fsm.line_start=fsm.lineno;
		}
		else
		{
			fsm_error(fsm.namei,fsm.lineno,ERR_STARTTWICE);
			YYERROR;
		}
	}
	| CPRIOEVENT	/* "%priority event" */
	{
		fsm.fl.prio=1;
	}
	| CPRIOSTATE	/* "%priority state" */
	{
		fsm.fl.prio=2;
	}
	| CEXTERN nlist	/* "%extern <name>[[,]<...>]*" */
	| CMAX		/* "%max <num> */
	| error
	{
		fsm.error=true;		/* Resynchronise sur le mot suivant */
	}
	;

/*********/
/* nlist */
/*********/

/* - La liste des evenements exterieurs - */

nlist	: EVENT			/* Un evenement exterieur */
	{
		chg_event($1); 	/* convertie en extern */
	}
	| nlist virgule EVENT	/* Une suite d'evenement exterieur */
	{
		chg_event($3);
	}
	;

/***********/
/* virgule */
/***********/

/* - Virgule optionelle - */

virgule :
	| ','
	;

/*********/
/* rules */
/*********/

/* - Ensemble de regle - */

rules	: rule			/* Une regle */
	| rules rule		/* Une regle suivit d'autres */
	;

/********/
/* rule */
/********/

/* - Une regle - */

rule	: error ';'
	{ fsm.error=true;	/* Resynchronise sur le ';' */
	}
	| states ':' { fsm.line=fsm.lineno; } events exec rrule
	{
		ty_rule*   rule;
		ty_rrule*  rrule;
		ty_lstate* left;
		ty_levent* levent;

		ty_rrule*  next_rrule;	/* next de rrule avant free */
		ty_levent* next_event;	/* next de levent avant free */
		ty_lstate* next_left;

		if ((rrule=malloc(sizeof(*rrule)))==NULL)
		{
			fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
			YYERROR;
		}
		dinc(yacc);
		ADD_TEMP(rrule);
		rrule->levent=$4;
		rrule->exe=$5;
		rrule->next=$6;

		if ((rule=malloc(sizeof(*rule)))==NULL)
		{
			fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
			YYERROR;
		}
		dinc(yacc);
		ADD_TEMP(rule);
		rule->left=$1;
		rule->right=rrule;

		if (fsm.error) goto Clear;

	/* - Traite une regle - */
		d0(dprintf(("%p:rule\tleft=%p right=%p\n",rule,rule->left,rule->right)));
		left=rule->left;
		do
		{
			d0(dif(left!=NULL,dprintf(("%p:left\tstate=%s next=%p\n",left,key_symbol(fsm.symtbl_state,left->state),left->next))));

	/* - Traite la droite d'une regle - */
			for (rrule=rule->right;rrule!=NULL;rrule=rrule->next)
			{
				d0(dprintf(("%p:rrule\tlevent=%p exe=%ld..%ld next=%p\n",rrule,rrule->levent,rrule->exe->begin,rrule->exe->end,rrule->next)));

	/* - Traite les evens de la droite d'une regle - */
				levent=rrule->levent;
				do
				{

				/* - Memorise un triple - */
					d0(dif(levent!=NULL,dprintf(("%p:levent\tevent=%s next=%p\n",levent,key_symbol(fsm.symtbl_event,levent->event),levent->next))));

					fsm.triple=malloc(sizeof(*fsm.triple));
					if (fsm.triple==NULL)
					{
						fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
						YYERROR;
					}
					dinc(triple);
					if (left!=NULL)
					{
						fsm.triple->state=left->state;			/* Etat du triple */
					}
					else /* Pas d'etat, default d'un event */
					{
						fsm.triple->state=find_symbol(fsm.symtbl_state,""); /* Pas d'etat */
					}
					if (levent!=NULL)
					{
						fsm.triple->event=levent->event;
					}
					else /* Pas d'event, default d'un etat */
					{
						fsm.triple->event=find_symbol(fsm.symtbl_event,"");	/* Pas d'event */
					}
					fsm.triple->line=fsm.line;

				/* - Cherche si pas de doublon - */
					{
						ty_triple* pt;

						for (pt=fsm.triple->event->triple;pt!=NULL;pt=pt->next_state)
						{
							d2(dprintf(("Test:%s %s\n",
								key_symbol(fsm.symtbl_state,fsm.triple->state),
								key_symbol(fsm.symtbl_event,fsm.triple->event))));
							if ((pt->event==fsm.triple->event) && (pt->state==fsm.triple->state))
							{
								fsm_error(fsm.namei,fsm.lineno,
													ERR_MORE,
													key_symbol(fsm.symtbl_state,fsm.triple->state),
													key_symbol(fsm.symtbl_event,fsm.triple->event));
								free(fsm.triple);
								ddec(triple);
								YYERROR;
							}
						}
					}
				/* - Classe le nouveau triple dans la liste du meme state - */
					{
						ty_triple* ostate=NULL;
						ty_triple* cstate;

						for(cstate=fsm.triple->state->triple; /* Premier triple du state */
							(cstate!=NULL && (cstate->event->num<fsm.triple->event->num));
							ostate=cstate,cstate=cstate->next_state);
						fsm.triple->next_state=cstate;
						if (ostate!=NULL) ostate->next_state=fsm.triple;
						else fsm.triple->state->triple=fsm.triple;
					}

				/* - Classe le nouveau triple dans la liste du meme event - */
					{
						ty_triple* oevent=NULL;
						ty_triple* cevent;

						for(cevent=fsm.triple->event->triple; /* Premier triple du event */
							  (cevent!=NULL && (cevent->state->num<fsm.triple->state->num));
							  oevent=cevent,cevent=cevent->next_event);
						fsm.triple->next_event=cevent;
						if (oevent!=NULL) oevent->next_event=fsm.triple;
						else fsm.triple->event->triple=fsm.triple;
					}

					fsm.triple->exe=rrule->exe;

					d4(dprintf(("%p nevent=%p nstate=%p s=[%d]%-8s e=[%d]%-8s exe=[%d]%ld..%ld\n",
						fsm.triple,fsm.triple->next_event,fsm.triple->next_state,
						(fsm.triple->state!=NULL) ? fsm.triple->state->num : -1,
						(fsm.triple->state!=NULL) ? key_symbol(fsm.symtbl_state,fsm.triple->state) : "(NULL)",
						(fsm.triple->event!=NULL)	? fsm.triple->event->num : -1,
						(fsm.triple->event!=NULL)	? key_symbol(fsm.symtbl_event,fsm.triple->event) : "(NULL)",
						fsm.triple->exe->exe,fsm.triple->exe->begin,fsm.triple->exe->end));)
					if (levent!=NULL) levent=levent->next;
					d1(deb_state());d1(deb_event());
				} while (levent!=NULL);
			}
			if (left!=NULL) left=left->next;
		} while(left!=NULL);

	/* - Efface une regle - */
Clear:

	/* - Efface la droite d'une regle - */
		for (next_rrule=rrule=rule->right;next_rrule!=NULL;rrule=next_rrule)
		 {
			next_rrule=rrule->next;
			for (next_event=levent=rrule->levent;next_event!=NULL;levent=next_event)
			{
				next_event=levent->next;
				d0(dprintf(("levent:free(%p);\n",levent)));
				SUB_TEMP(levent);
				free(levent);
				ddec(yacc);
			}
			d0(dprintf(("rrule:free(%p);\n",rrule)));
			SUB_TEMP(rrule);
			free(rrule);
			ddec(yacc);
		}

	/* - Efface la gauche d'une regle - */
		for (next_left=left=rule->left;next_left!=NULL;left=next_left)
		{
			next_left=left->next;
			d0(dprintf(("left:free(%p);\n",left)));
			SUB_TEMP(left);
			free(left);
			ddec(yacc);
		}

	/* - Efface la regle - */
		d0(dprintf(("rule:free(%p);\n",rule)));
		SUB_TEMP(rule);
		free(rule);
		ddec(yacc);
	}
	;

/*********/
/* rrule */
/*********/

/* - La partie droite d'une regle - */
rrule	: ';'
{
	$$=NULL;
}
	| '|' events exec rrule
{
	if (($$=malloc(sizeof(*$$)))==NULL)
	{
		fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
		YYERROR;
	}
	dinc(yacc);
	ADD_TEMP($$);
	$$->levent=$2;
	$$->exe=$3;
	$$->next=$4;
}
	;

/**********/
/* events */
/**********/

/* - Une liste d'evenements - */

events	:					/* Pas d'evenement */
	{
		$$=NULL;
	}
	| EVENT					/* Un evenement */
	{
		if (($$=malloc(sizeof(*$$)))==NULL)
		{
			fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
			YYERROR;
		}
		dinc(yacc);
		ADD_TEMP($$);
		$$->event=$1;
		$$->next=NULL;
	}
	| events virgule EVENT	/* Un evenement suivit d'autre */
	{
		if (($$=malloc(sizeof(*$$)))==NULL)
		{
			fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
			YYERROR;
		}
		dinc(yacc);
		ADD_TEMP($$);
		$$->event=$3;
		$$->next=$1;
	}
	;

/**********/
/* states */
/**********/

/* - Une liste d'etats - */

states	:					/* Pas d'etat */
	{
		$$=NULL;
	}
	| STATE 				/* Un etat */
	{
		if (($$=malloc(sizeof(*$$)))==NULL)
		{
			fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
			YYERROR;
		}
		dinc(yacc);
		ADD_TEMP($$);
		$$->state=$1;
		$$->next=NULL;
	}
	| states virgule STATE
	{
		if (($$=malloc(sizeof(*$$)))==NULL)
		{
			fsm_fatal(fsm.namei,fsm.lineno,ERR_MEMORY);
			YYERROR;
		}
		dinc(yacc);
		ADD_TEMP($$);
		$$->state=$3;
		$$->next=$1;
	}
	;

/********/
/* exec */
/********/

/* - La commande d'execution de la regle - */

exec	:					/* Pas d'exec */
	{
		$$=NULL;
	}
	| EXECUTION				/* Un execution */
	{
		$$=$1;
	}
	;

%%

/***************/
/* trans_rules */
/***************/

/* - Interpretation des regles -*/

bool	trans_rules()
{
	int		status;

	if (open_lex())
		return(true);
	status=yyparse();

/* - Fin - */
	close_lex();

	return((status) ? true : false);
}

#ifndef NDEBUG
/*************/
/* deb_event */
/*************/

void	deb_event()
/* - Affiche la liste des states pour chaque event - */
{
	ty_var_state*   pts;
	ty_triple*		pt;

	fprintf(stderr,"--Liste des events par states\n");
	for (pts=fsm.first_state;pts!=NULL;pts=pts->next)
	{
		for (pt=pts->triple;pt!=NULL;pt=pt->next_state)
		{
			assert(pt!=NULL);
			fprintf(stderr,"S:%p nevent=%p nstate=%p S=[%d]%-8s e=[%d]%-8s exe=[%d]%s\n",
							pt,pt->next_event,pt->next_state,
							(pt->state==NULL) ? -1 : pt->state->num,
							(pt->state==NULL) ? "(NULL)" :key_symbol(fsm.symtbl_state,pt->state),
							(pt->event==NULL) ? -1 : pt->event->num,
							(pt->event==NULL) ? "(NULL)" : key_symbol(fsm.symtbl_event,pt->event),
							pt->exe->exe,pt->exe->buf);
		}
	}
}

/*************/
/* deb_state */
/*************/

void	deb_state()
/* - Affiche la liste des events pour chaque state - */
{
	ty_var_event*	pts;
	ty_triple*		pt;

	fprintf(stderr,"--Liste des states par events\n");
	for (pts=fsm.first_event;pts!=NULL;pts=pts->next)
	{
		for (pt=pts->triple;pt!=NULL;pt=pt->next_event)
		{
			fprintf(stderr,"E:%p nevent=%p nstate=%p E=[%d]%-8s s=[%d]%-8s exe=[%d]%s\n",
							pt,pt->next_event,pt->next_state,
							(pt->event==NULL) ? -1 :pt->event->num,
							(pt->event==NULL) ? "(NULL)" : key_symbol(fsm.symtbl_event,pt->event),
							(pt->state==NULL) ? -1 : pt->state->num,
							(pt->state==NULL) ? "(NULL)": key_symbol(fsm.symtbl_state,pt->state),
							pt->exe->exe,pt->exe->buf);
		}
	}
}
#endif
