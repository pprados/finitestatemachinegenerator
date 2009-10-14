/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     MARK = 258,
     CHEADER = 259,
     CSTART = 260,
     CEXTERN = 261,
     CPRIOEVENT = 262,
     CPRIOSTATE = 263,
     CMAX = 264,
     STATE = 265,
     EVENT = 266,
     EXECUTION = 267
   };
#endif
/* Tokens.  */
#define MARK 258
#define CHEADER 259
#define CSTART 260
#define CEXTERN 261
#define CPRIOEVENT 262
#define CPRIOSTATE 263
#define CMAX 264
#define STATE 265
#define EVENT 266
#define EXECUTION 267




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 98 "./autoyacc.y"
{
	ty_rrule*     rrule;	/* Droite de la regle */
	ty_levent*    levent;	/* Liste d'evenement */
	ty_lstate*    lstate;	/* Liste d'etat */
	ty_var_state* state;	/* Numero du state */
	ty_var_event* event;	/* Numero de l'event */
	ty_exe*       exe;	/* Numero de l'exec */
	const char*   tail;     /* Buffer de fin de fichier */
}
/* Line 1489 of yacc.c.  */
#line 83 "autoyacc.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

