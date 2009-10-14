
static const char fsm_copyright[]="Use FSMG V1.1 proposed by Ph. PRADOS\n";
#line 1
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








#line 26


#line 1 "skeleton.fsm"

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

#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

#include "all.h"
#include "autoyac.h"
#include "conv.h"
#include "skeleton.h"
#include "readske.h"
#include "tools.h"
#include "error.h"

#define PUTCH(c)	fputc(c,fsm.fout)
#define PUTS(s) 	fputs(s,fsm.fout)
#define UNGETCH(c)	ske_ungetc(c,yyin)
#define GETCH() 	ske_getc(yyin)

#define FSM_MAIN	automate
#define FSM_FNTYPE	static  
#define FSM_BEFORE	fsm_event=GETCH();

#ifndef NDEBUG
#define FSM_DEBUG
#endif

static FILE*  yyin;	/* Stream du squelette */


#line 27


#define FSM_NAME    "skeleton.fsm"
#define FSM_MAJOR     (1)
#define FSM_MINOR     (1)
#define FSM_NBEVENT   (10)
#define FSM_NBSTATE   (8)
#define FSM_NBTRANS   (9)
#define FSM_NBEXEC    (23)
#define FSM_MAXEXTERN (256)
#define FSM_ALL

#line 28



#line 29





#define FSM_COEFTRANS	 4	
#define FSM_COEFSWITCH	 3	
#define FSM_COEFFN	10	

#define FSM_SIZEOF_NEARF 2	
#define FSM_SIZEOF_FARF	 4	
#define FSM_SIZEOF_FN	 4	


#include <limits.h>			
#define FSM_EXP2(X) (X)*(X)	

#define FSM_SIZEOF_UCHR	1	


#if   (UCHAR_MAX==USHRT_MAX)
#define FSM_SIZEOF_USHRT 1
#elif (FSM_EXP2(UCHAR_MAX+1)==USHRT_MAX+1)
#define FSM_SIZEOF_USHRT 2
#elif (FSM_EXP2(FSM_EXP2(UCHAR_MAX+1))==USHRT_MAX+1)
#define FSM_SIZEOF_USHRT 4
#endif


#if   (UCHAR_MAX==UINT_MAX)
#define FSM_SIZEOF_UINT 1
#elif (FSM_EXP2(UCHAR_MAX+1)==UINT_MAX+1)
#define FSM_SIZEOF_UINT 2
#elif (FSM_EXP2(FSM_EXP2(UCHAR_MAX+1))==UINT_MAX+1)
#define FSM_SIZEOF_UINT 4
#endif



#define STATE_ERROR	(-1)	
#define STATE_RESERVED	(-2)	

#ifndef FSM_MAIN
#define FSM_MAIN	fsm	
#endif
#ifndef FSM_STATETYPE
#define FSM_STATETYPE	short	
#endif
#ifndef FSM_EVENTTYPE
#define FSM_EVENTTYPE	short	
#endif
#ifndef FSM_FNTYPE
#define FSM_FNTYPE		
#endif
#ifndef FSM_ARRTYPE
#define FSM_ARRTYPE			
#endif
#ifndef FSM_BEFORE
#define FSM_BEFORE		
#endif
#ifndef FSM_AFTER
#define FSM_AFTER		
#endif


#ifdef UNIX
#ifdef sun
#define fsm_name2(a,b)		ab
#else
#ifdef	BSD
#define fsm_name2(a,b)		a\
b
#else
#define fsm_name2(a,b)		ab
#endif

#endif
#else
#define fsm_name2(a,b)		_fsm_name2_aux(a,b)
#define _fsm_name2_aux(a,b)	a##b
#endif


#ifndef FSM_PERROR
#ifdef __cplusplus
extern "C" {
#include <stdio.h>
}
#else
#include <stdio.h>
#endif
#define FSM_PERROR(num,str)	\
  fprintf(stderr,"FSMG Run-time error %d in file " FSM_NAME " : %s\n",num,str)
#endif

#if defined(FSM_DEBUG) && defined(FSM_TRACE)

#define FSM_LSTATE(state,num)	#state,
#define FSM_LEVENT(event,num)	#event ,

static struct 
{ 
	FSM_STATETYPE	state;
	FSM_EVENTTYPE	event;
}    fsm_buf[FSM_TRACE];			
static int	   fsm_nb_buf=0;		
static const char* FSM_ARRTYPE fsm_str_state[]= 
{
  
FSM_LSTATE($IN_C,1)
FSM_LSTATE($IN_STR,2)
FSM_LSTATE($IN_CHAR,3)
FSM_LSTATE($IN_QCOMMENT,4)
FSM_LSTATE($IN_CPPCOMMENT,5)
FSM_LSTATE($IN_COMMENT,6)
FSM_LSTATE($IN_NCOMMENT,7)
FSM_LSTATE($IN_FCOMMENT,8)

#line 137

};
static const char* FSM_ARRTYPE fsm_str_event[]=	
{
  
FSM_LEVENT(-Other-,1)

#line 141

};

#ifndef FSM_PRINT_TRACE
#ifdef __cplusplus
extern "C" {
#include <ctype.h>
}
#else
#include <ctype.h>
#endif



static void fsm_print_trace()			
{ 
	int i;

	puts("Trace:");
	for (i=FSM_TRACE-fsm_nb_buf;i<FSM_TRACE;i++)
	{ 
		printf("[%8s",fsm_str_state[fsm_buf[i].state]);
		if (fsm_buf[i].event>=FSM_MAXEXTERN)
			printf("-%-8s]\n",fsm_str_event[fsm_buf[i].event-FSM_MAXEXTERN]);
		else
			printf((isalpha(fsm_buf[i].event) || isdigit(fsm_buf[i].event))
				? "-'%c'     ]\n"
				: "-%04X    ]\n",fsm_buf[i].event);
	}
}

#define FSM_PRINT_TRACE fsm_print_trace()
#endif
#else
#define FSM_PRINT_TRACE
#endif


#ifndef FSM_ARRTYPE
#include <limits.h>
#if   (FSM_NBEXEC<=UCHAR_MAX)
#define FSM_ARRTYPE	unsigned char
#define FSM_SIZEOF_ARRTYPE FSM_SIZEOF_UCHR
#elif (FSM_NBEXEC<=USHRT_MAX)
#define FSM_ARRTYPE	unsigned short
#define FSM_SIZEOF_ARRTYPE FSM_SIZEOF_USHRT
#elif (FSM_NBEXEC<=UINT_MAX)
#define FSM_ARRTYPE	unsigned int
#define FSM_SIZEOF_ARRTYPE FSM_SIZEOF_UINT
#endif
#endif

#ifndef FSM_LFNTYPE
#define FSM_LFNTYPE
#define FSM_SIZEOF_LFNTYPE FSM_SIZEOF_FN
#endif


#ifndef FSM_NOSWITCH
#define FSM_SIZE_ARRSWITCH \
	  FSM_NBEVENT*FSM_NBSTATE*FSM_SIZEOF_ARRTYPE+\
	  FSM_NBTRANS*FSM_COEFTRANS+\
	  FSM_NBEXEC*FSM_COEFSWITCH
#else
#define FSM_SIZE_ARRSWITCH ~0U
#endif


#define FSM_SIZE_FNARR \
	  FSM_NBEVENT*FSM_NBSTATE*FSM_SIZEOF_LFNTYPE+\
	  FSM_NBTRANS*FSM_COEFTRANS+\
	  FSM_NBEXEC*FSM_COEFFN


#define FSM_SIZE_ARRFNARR \
	  FSM_NBEVENT*FSM_NBSTATE*FSM_SIZEOF_ARRTYPE+\
	  FSM_NBEXEC*FSM_SIZEOF_LFNTYPE+\
	  FSM_NBTRANS*FSM_COEFTRANS+ \
	  FSM_NBEXEC*FSM_COEFFN


#ifndef FSM_NOSWITCH
#define FSM_SIZE_SWITCH \
	  FSM_NBEXEC*FSM_COEFSWITCH
#else
#define FSM_SIZE_SWITCH ~0U
#endif

#if (!defined(FSM_ARRSWITCH) && \
     !defined(FSM_FNARR) &&	\
     !defined(FSM_ARRFNARR) &&	\
     !defined(FSM_SWITCH))


#ifdef FSM_SIZE
#if   ((FSM_SIZE_ARRSWITCH<FSM_SIZE_FNARR)    && \
       (FSM_SIZE_ARRSWITCH<FSM_SIZE_ARRFNARR) && \
       (FSM_SIZE_ARRSWITCH<FSM_SIZE_SWITCH))
#define FSM_ARRSWITCH

#elif ((FSM_SIZE_FNARR<FSM_SIZE_ARRFNARR) && \
       (FSM_SIZE_FNARR<FSM_SIZE_SWITCH))
#define FSM_FNARR

#elif (FSM_SIZE_ARRFNARR<FSM_SIZE_SWITCH)
#define FSM_ARRFNARR

#else
#define FSM_SWITCH
#endif
#endif


#ifdef FSM_TIME
#if ((FSM_NBTRANS!=0) && !defined(FSM_NOSWITCH))
#define FSM_SWITCH
#else
#define FSM_FNARR
#endif
#endif


#if (!defined(FSM_SIZE) && !defined(FSM_TIME))
#if ((FSM_NBTRANS!=0) && !defined(FSM_NOSWITCH))
#define FSM_SWITCH
#elif (FSM_NBEVENT*FSM_NBSTATE*FSM_SIZEOF_LFNTYPE< \
     (FSM_NBEVENT*FSM_NBSTATE*FSM_SIZEOF_ARRTYPE+\
      FSM_NBEXEC*FSM_SIZEOF_NEARF))
#define FSM_FNARR
#else
#define FSM_ARRFNARR
#endif
#endif
#endif


#if (defined(MSDOS) || (_MSC_VER>=6))
#if (_MSC_VER>=8)
#pragma warning(disable:4102)
#endif
#ifdef FSM_ARRSWITCH
#pragma message("Use \"ARRSWITCH\".")
#endif
#ifdef FSM_FNARR
#pragma message("Use \"FNARR\".")
#endif
#ifdef FSM_ARRFNARR
#pragma message("Use \"ARRFNARR\".")
#endif
#ifdef FSM_SWITCH
#pragma message("Use \"SWITCH\".")
#endif
#endif




#ifndef FSM_TRANSLATE
#define FSM_TRANSLATE(PUB,LOC) case PUB: fsm_cevent=FSM_MAXEXTERN+LOC; break;
#endif



#ifdef FSM_ARRSWITCH
#define FSM_ARR(EXE)	EXE,

static FSM_ARRTYPE	FSM_ARRTYPE fsm_tbl_fsm[FSM_NBSTATE][FSM_NBEVENT]=
{ 
	

            /* -Other-     EOF         '\\'        '"'         '\''        '/'         '#'         '$'         '*'         '\n'        */
/* $IN_C    */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 5) FSM_ARR( 6) FSM_ARR( 7) FSM_ARR( 8) FSM_ARR( 9) FSM_ARR( 2) FSM_ARR( 2) 
/* $IN_STR  */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(10) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) 
/* $IN_CHAR */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 2) FSM_ARR(11) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) 
/* $IN_QCOMMENT */ FSM_ARR(14) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(14) FSM_ARR(14) FSM_ARR(12) FSM_ARR(14) FSM_ARR(14) FSM_ARR(13) FSM_ARR(14) 
/* $IN_CPPCOMMENT */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR(15) 
/* $IN_COMMENT */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR(17) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR(16) FSM_ARR( 2) 
/* $IN_NCOMMENT */ FSM_ARR(19) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(19) FSM_ARR(19) FSM_ARR(19) FSM_ARR(19) FSM_ARR(19) FSM_ARR(18) FSM_ARR(19) 
/* $IN_FCOMMENT */ FSM_ARR(22) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(22) FSM_ARR(22) FSM_ARR(20) FSM_ARR(22) FSM_ARR(22) FSM_ARR(21) FSM_ARR(22) 

#line 309

};
#endif 



#ifdef FSM_FNARR
#define FSM_FNL(EXE)	fsm_name2(fsm_fn,EXE)


#define FSM_FTYPE FSM_STATETYPE (FSM_LFNTYPE *)(\
			 FSM_STATETYPE* state, \
			 FSM_EVENTTYPE* event)

#define FSM_DCLFN(n) \
  static FSM_STATETYPE FSM_LFNTYPE FSM_FNL(n)(\
			 FSM_STATETYPE* fsm_state,\
			 FSM_EVENTTYPE* fsm_event)

#ifdef FSM_USE_ERROR
FSM_DCLFN(0) { FSM_PRINT_TRACE; FSM_PERROR(0,"unknow state or event"); return(STATE_RESERVED); }
#endif

#ifdef FSM_USE_NOTHING

FSM_DCLFN(1) { return(STATE_RESERVED); }
#endif


FSM_DCLFN(2) 
#line 63 "skeleton.fsm"
{ { 
	PUTCH( *fsm_event );
	if ( *fsm_event =='\n') fsm.sklineno++;
} return(STATE_RESERVED); }
FSM_DCLFN(3) 
#line 76 "skeleton.fsm"
{ { 
	return(-1);
} return(STATE_RESERVED); }
FSM_DCLFN(4) 
#line 87 "skeleton.fsm"
{ { 
	char  c;

	PUTCH( *fsm_event );
	if ((c=GETCH())==EOF) return(-1); /* Consomme un caractere */
	if (c=='\n') fsm.sklineno++;
	PUTCH(c);
} return(STATE_RESERVED); }
FSM_DCLFN(5) 
#line 105 "skeleton.fsm"
{ { 
	 *fsm_state =1;      /* entre dans l'etat $IN_STR */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(6) 
#line 110 "skeleton.fsm"
{ {
	 *fsm_state =2;     /* entre dans l'etat $IN_CHAR */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(7) 
#line 115 "skeleton.fsm"
{ { 
	 *fsm_state =3;     /* entre dans l'etat $IN_QCOMMENT */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(8) 
#line 120 "skeleton.fsm"
{ { 
	char    yytext[6];
	size_t  pos=1;
	char  c;

	yytext[0]='#';
	while ((c=GETCH())!='\n')
	{
		if (c==EOF)
		{ 
			yytext[pos]='\0';
			PUTS(yytext);
			return(-1);
		}
		if (c=='\\')				/* Escape */
		{
			if (pos<=sizeoftab(yytext))
			{ 
				yytext[pos++]=c;	/* Sauve l'evenement */
			}
			c=GETCH();
			if (c=='\n') fsm.sklineno++;
		}
		if (pos<sizeoftab(yytext)-1)
		{
			yytext[pos++]=c;		/* Sauve l'evenement */
		}
		if (pos==sizeoftab(yytext)-1)
		{
			if (((!fsm.mode) && (fsm.fl.line)) || strncmp("#line",yytext,5))
			{
				yytext[pos]='\0';
				PUTS(yytext);
				while ((c=GETCH())!='\n')
				{
					if (c==EOF) return(-1);
					if (c=='\\')
					{
						PUTCH(c);
						if ((c=GETCH())=='\n') fsm.sklineno++;
					}
       				PUTCH(c);
				}
				PUTCH('\n');
			}
			else /* Supprime le #line si !fsm.fl.line */
			{
				while ((c=GETCH())!='\n')
				{
					if (c==EOF) return(-1);
					if (c=='\\')
       				{
						if ((c=GETCH())=='\n') fsm.sklineno++;
       				}
				}
				PUTCH('\n');
			}
			break;
		}
	}
	fsm.sklineno++;
} return(STATE_RESERVED); }
FSM_DCLFN(9) 
#line 183 "skeleton.fsm"
{ { 
	char  yytext[16];
	short pos=1;
	char  c;

	yytext[0]='$';
	while (isalnum(c=GETCH()) || (c=='_') || (c=='$'))
	{
		if (pos<sizeoftab(yytext)-1)
		{
			yytext[pos++]=c;        /* Sauve l'evenement */
		}
	}
	UNGETCH(c);				/* Remet le dernier */
	yytext[pos]='\0';			/* Buf=le mot a convertir */

	{
		conv_token(yytext);
		fprintf(fsm.fout,"\n");
		if (fsm.fl.line) aff_line(fsm.fout,fsm.sklineno,fsm.names);
	}
} return(STATE_RESERVED); }
FSM_DCLFN(10) 
#line 214 "skeleton.fsm"
{ { 
	 *fsm_state =0;		/* retourne a l'etat $IN_C */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(11) 
#line 227 "skeleton.fsm"
{ {
	 *fsm_state =0;		/* retourne a l'etat $IN_C */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(12) 
#line 240 "skeleton.fsm"
{ { 
	 *fsm_state =4;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(13) 
#line 245 "skeleton.fsm"
{ { 
	 *fsm_state =5;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(14) 
#line 250 "skeleton.fsm"
{ { 
	 *fsm_state =0;		/* Retourne en $IN_C */
	UNGETCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(15) 
#line 263 "skeleton.fsm"
{ { 
	 *fsm_state =0;
	PUTCH( *fsm_event );
	fsm.sklineno++;
} return(STATE_RESERVED); }
FSM_DCLFN(16) 
#line 277 "skeleton.fsm"
{ { 
	 *fsm_state =7;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(17) 
#line 282 "skeleton.fsm"
{ { 
	 *fsm_state =6;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(18) 
#line 295 "skeleton.fsm"
{ { 
	 *fsm_state =5;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(19) 
#line 300 "skeleton.fsm"
{ { 
	 *fsm_state =5;
	PUTCH( *fsm_event );
	if ( *fsm_event =='\n') fsm.sklineno++;
} return(STATE_RESERVED); }
FSM_DCLFN(20) 
#line 314 "skeleton.fsm"
{ { 
	 *fsm_state =0;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(21) 
#line 319 "skeleton.fsm"
{ { 
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(22) 
#line 323 "skeleton.fsm"
{ { 
	 *fsm_state =5;	/* Pas la fin */
	PUTCH( *fsm_event );
	if ( *fsm_event =='\n') fsm.sklineno++;
} return(STATE_RESERVED); }

#line 337


#define FSM_ARR(EXE)	FSM_FNL(EXE),	

static FSM_STATETYPE (FSM_LFNTYPE * FSM_ARRTYPE fsm_tbl_fsm[FSM_NBSTATE][FSM_NBEVENT])(
		  FSM_STATETYPE* state,
		  FSM_EVENTTYPE* event)=

{ 
	

            /* -Other-     EOF         '\\'        '"'         '\''        '/'         '#'         '$'         '*'         '\n'        */
/* $IN_C    */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 5) FSM_ARR( 6) FSM_ARR( 7) FSM_ARR( 8) FSM_ARR( 9) FSM_ARR( 2) FSM_ARR( 2) 
/* $IN_STR  */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(10) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) 
/* $IN_CHAR */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 2) FSM_ARR(11) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) 
/* $IN_QCOMMENT */ FSM_ARR(14) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(14) FSM_ARR(14) FSM_ARR(12) FSM_ARR(14) FSM_ARR(14) FSM_ARR(13) FSM_ARR(14) 
/* $IN_CPPCOMMENT */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR(15) 
/* $IN_COMMENT */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR(17) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR(16) FSM_ARR( 2) 
/* $IN_NCOMMENT */ FSM_ARR(19) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(19) FSM_ARR(19) FSM_ARR(19) FSM_ARR(19) FSM_ARR(19) FSM_ARR(18) FSM_ARR(19) 
/* $IN_FCOMMENT */ FSM_ARR(22) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(22) FSM_ARR(22) FSM_ARR(20) FSM_ARR(22) FSM_ARR(22) FSM_ARR(21) FSM_ARR(22) 

#line 346

};
#endif 



#ifdef FSM_ARRFNARR
#define FSM_FNL(EXE)	fsm_name2(fsm_fn,EXE)


#define FSM_DCLFN(n) \
  static FSM_STATETYPE FSM_LFNTYPE FSM_FNL(n)(\
			 FSM_STATETYPE* fsm_state,\
			 FSM_EVENTTYPE* fsm_event)

#ifdef FSM_USE_ERROR
FSM_DCLFN(0) { FSM_PRINT_TRACE; FSM_PERROR(0,"unknow state or event"); return(STATE_RESERVED); }
#endif

#ifdef FSM_USE_NOTHING
FSM_DCLFN(1) { return(STATE_RESERVED); }
#endif


FSM_DCLFN(2) 
#line 63 "skeleton.fsm"
{ { 
	PUTCH( *fsm_event );
	if ( *fsm_event =='\n') fsm.sklineno++;
} return(STATE_RESERVED); }
FSM_DCLFN(3) 
#line 76 "skeleton.fsm"
{ { 
	return(-1);
} return(STATE_RESERVED); }
FSM_DCLFN(4) 
#line 87 "skeleton.fsm"
{ { 
	char  c;

	PUTCH( *fsm_event );
	if ((c=GETCH())==EOF) return(-1); /* Consomme un caractere */
	if (c=='\n') fsm.sklineno++;
	PUTCH(c);
} return(STATE_RESERVED); }
FSM_DCLFN(5) 
#line 105 "skeleton.fsm"
{ { 
	 *fsm_state =1;      /* entre dans l'etat $IN_STR */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(6) 
#line 110 "skeleton.fsm"
{ {
	 *fsm_state =2;     /* entre dans l'etat $IN_CHAR */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(7) 
#line 115 "skeleton.fsm"
{ { 
	 *fsm_state =3;     /* entre dans l'etat $IN_QCOMMENT */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(8) 
#line 120 "skeleton.fsm"
{ { 
	char    yytext[6];
	size_t  pos=1;
	char  c;

	yytext[0]='#';
	while ((c=GETCH())!='\n')
	{
		if (c==EOF)
		{ 
			yytext[pos]='\0';
			PUTS(yytext);
			return(-1);
		}
		if (c=='\\')				/* Escape */
		{
			if (pos<=sizeoftab(yytext))
			{ 
				yytext[pos++]=c;	/* Sauve l'evenement */
			}
			c=GETCH();
			if (c=='\n') fsm.sklineno++;
		}
		if (pos<sizeoftab(yytext)-1)
		{
			yytext[pos++]=c;		/* Sauve l'evenement */
		}
		if (pos==sizeoftab(yytext)-1)
		{
			if (((!fsm.mode) && (fsm.fl.line)) || strncmp("#line",yytext,5))
			{
				yytext[pos]='\0';
				PUTS(yytext);
				while ((c=GETCH())!='\n')
				{
					if (c==EOF) return(-1);
					if (c=='\\')
					{
						PUTCH(c);
						if ((c=GETCH())=='\n') fsm.sklineno++;
					}
       				PUTCH(c);
				}
				PUTCH('\n');
			}
			else /* Supprime le #line si !fsm.fl.line */
			{
				while ((c=GETCH())!='\n')
				{
					if (c==EOF) return(-1);
					if (c=='\\')
       				{
						if ((c=GETCH())=='\n') fsm.sklineno++;
       				}
				}
				PUTCH('\n');
			}
			break;
		}
	}
	fsm.sklineno++;
} return(STATE_RESERVED); }
FSM_DCLFN(9) 
#line 183 "skeleton.fsm"
{ { 
	char  yytext[16];
	short pos=1;
	char  c;

	yytext[0]='$';
	while (isalnum(c=GETCH()) || (c=='_') || (c=='$'))
	{
		if (pos<sizeoftab(yytext)-1)
		{
			yytext[pos++]=c;        /* Sauve l'evenement */
		}
	}
	UNGETCH(c);				/* Remet le dernier */
	yytext[pos]='\0';			/* Buf=le mot a convertir */

	{
		conv_token(yytext);
		fprintf(fsm.fout,"\n");
		if (fsm.fl.line) aff_line(fsm.fout,fsm.sklineno,fsm.names);
	}
} return(STATE_RESERVED); }
FSM_DCLFN(10) 
#line 214 "skeleton.fsm"
{ { 
	 *fsm_state =0;		/* retourne a l'etat $IN_C */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(11) 
#line 227 "skeleton.fsm"
{ {
	 *fsm_state =0;		/* retourne a l'etat $IN_C */
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(12) 
#line 240 "skeleton.fsm"
{ { 
	 *fsm_state =4;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(13) 
#line 245 "skeleton.fsm"
{ { 
	 *fsm_state =5;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(14) 
#line 250 "skeleton.fsm"
{ { 
	 *fsm_state =0;		/* Retourne en $IN_C */
	UNGETCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(15) 
#line 263 "skeleton.fsm"
{ { 
	 *fsm_state =0;
	PUTCH( *fsm_event );
	fsm.sklineno++;
} return(STATE_RESERVED); }
FSM_DCLFN(16) 
#line 277 "skeleton.fsm"
{ { 
	 *fsm_state =7;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(17) 
#line 282 "skeleton.fsm"
{ { 
	 *fsm_state =6;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(18) 
#line 295 "skeleton.fsm"
{ { 
	 *fsm_state =5;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(19) 
#line 300 "skeleton.fsm"
{ { 
	 *fsm_state =5;
	PUTCH( *fsm_event );
	if ( *fsm_event =='\n') fsm.sklineno++;
} return(STATE_RESERVED); }
FSM_DCLFN(20) 
#line 314 "skeleton.fsm"
{ { 
	 *fsm_state =0;
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(21) 
#line 319 "skeleton.fsm"
{ { 
	PUTCH( *fsm_event );
} return(STATE_RESERVED); }
FSM_DCLFN(22) 
#line 323 "skeleton.fsm"
{ { 
	 *fsm_state =5;	/* Pas la fin */
	PUTCH( *fsm_event );
	if ( *fsm_event =='\n') fsm.sklineno++;
} return(STATE_RESERVED); }

#line 369


#define FSM_ARR(EXE)	EXE,		

#define FSM_FTYPE FSM_STATETYPE (FSM_LFNTYPE*)(  \
			    FSM_STATETYPE* state, \
			    FSM_EVENTTYPE* event)
#define FSM_FN(EXE)	FSM_FNL(EXE),	
#define FSM_FNNO	0,

static FSM_STATETYPE (FSM_LFNTYPE* FSM_ARRTYPE fsm_tbl_fnfsm[])(
		    FSM_STATETYPE* state,
		    FSM_EVENTTYPE* event)=
{ 
	
	FSM_FNNO
	FSM_FNNO
	FSM_FN( 2)
	FSM_FN( 3)
	FSM_FN( 4)
	FSM_FN( 5)
	FSM_FN( 6)
	FSM_FN( 7)
	FSM_FN( 8)
	FSM_FN( 9)
	FSM_FN(10)
	FSM_FN(11)
	FSM_FN(12)
	FSM_FN(13)
	FSM_FN(14)
	FSM_FN(15)
	FSM_FN(16)
	FSM_FN(17)
	FSM_FN(18)
	FSM_FN(19)
	FSM_FN(20)
	FSM_FN(21)
	FSM_FN(22)

#line 383

};



static FSM_ARRTYPE	FSM_ARRTYPE fsm_tbl_fsm[FSM_NBSTATE][FSM_NBEVENT]=
{ 
	

            /* -Other-     EOF         '\\'        '"'         '\''        '/'         '#'         '$'         '*'         '\n'        */
/* $IN_C    */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 5) FSM_ARR( 6) FSM_ARR( 7) FSM_ARR( 8) FSM_ARR( 9) FSM_ARR( 2) FSM_ARR( 2) 
/* $IN_STR  */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(10) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) 
/* $IN_CHAR */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 2) FSM_ARR(11) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) 
/* $IN_QCOMMENT */ FSM_ARR(14) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(14) FSM_ARR(14) FSM_ARR(12) FSM_ARR(14) FSM_ARR(14) FSM_ARR(13) FSM_ARR(14) 
/* $IN_CPPCOMMENT */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR(15) 
/* $IN_COMMENT */ FSM_ARR( 2) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR(17) FSM_ARR( 2) FSM_ARR( 2) FSM_ARR(16) FSM_ARR( 2) 
/* $IN_NCOMMENT */ FSM_ARR(19) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(19) FSM_ARR(19) FSM_ARR(19) FSM_ARR(19) FSM_ARR(19) FSM_ARR(18) FSM_ARR(19) 
/* $IN_FCOMMENT */ FSM_ARR(22) FSM_ARR( 3) FSM_ARR( 4) FSM_ARR(22) FSM_ARR(22) FSM_ARR(20) FSM_ARR(22) FSM_ARR(22) FSM_ARR(21) FSM_ARR(22) 

#line 390

};
#endif 







FSM_STATETYPE FSM_FNTYPE FSM_MAIN(FSM_STATETYPE fsm_state,
				  FSM_EVENTTYPE fsm_event)

{
#if defined(FSM_FNARR) || defined(FSM_ARRFNARR)
	FSM_STATETYPE fsm_ret;
#endif

#if defined(FSM_ARRSWITCH) || defined(FSM_FNARR) || defined(FSM_ARRFNARR)
#if (FSM_NBTRANS>0)				
	FSM_EVENTTYPE	fsm_cevent;
#else
#define fsm_cevent fsm_event
#endif
#endif

	while (fsm_state>=0)			
	{ 
		FSM_BEFORE;


#if defined(FSM_DEBUG) && defined(FSM_TRACE)
		{ 
			memcpy(&fsm_buf[0],&fsm_buf[1],sizeof(fsm_buf)-sizeof(*fsm_buf));
			fsm_buf[FSM_TRACE-1].state=fsm_state;
			fsm_buf[FSM_TRACE-1].event=fsm_event;
			if (fsm_nb_buf!=FSM_TRACE) fsm_nb_buf++;
		}
#endif


#ifdef FSM_DEBUG
		if ((fsm_state>=FSM_NBSTATE) ||
			((fsm_event>FSM_MAXEXTERN)
			&& (fsm_event>=FSM_MAXEXTERN+FSM_NBEVENT)))
		{ 
			FSM_PRINT_TRACE; FSM_PERROR(0,"unknow state or event");
			return(STATE_ERROR);
		}
#endif


#if !defined(FSM_SWITCH)
#if (FSM_NBTRANS>0)				
		switch(fsm_event)		
		{
			
FSM_TRANSLATE(EOF,1)
FSM_TRANSLATE('\\',2)
FSM_TRANSLATE('"',3)
FSM_TRANSLATE('\'',4)
FSM_TRANSLATE('/',5)
FSM_TRANSLATE('#',6)
FSM_TRANSLATE('$',7)
FSM_TRANSLATE('*',8)
FSM_TRANSLATE('\n',9)

#line 446


			default :
				fsm_cevent=(fsm_event>=FSM_MAXEXTERN) ? fsm_event : FSM_MAXEXTERN;
				break;
		}
#endif
#endif



#ifdef FSM_ARRSWITCH
		switch(fsm_tbl_fsm[fsm_state][fsm_cevent-FSM_MAXEXTERN])
		{
#ifdef FSM_USE_ERROR
			case 0 : 
				FSM_PRINT_TRACE; FSM_PERROR(0,"unknow state or event");
				return(STATE_ERROR);
#endif

#ifdef FSM_USE_NOTHING
			case 1 : 
				break;
#endif

			
#line 63 "skeleton.fsm"
case 2 : { 
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
} break;
#line 76 "skeleton.fsm"
case 3 : { 
	return(-1);
} break;
#line 87 "skeleton.fsm"
case 4 : { 
	char  c;

	PUTCH( fsm_event );
	if ((c=GETCH())==EOF) return(-1); /* Consomme un caractere */
	if (c=='\n') fsm.sklineno++;
	PUTCH(c);
} break;
#line 105 "skeleton.fsm"
case 5 : { 
	 fsm_state =1;      /* entre dans l'etat $IN_STR */
	PUTCH( fsm_event );
} break;
#line 110 "skeleton.fsm"
case 6 : {
	 fsm_state =2;     /* entre dans l'etat $IN_CHAR */
	PUTCH( fsm_event );
} break;
#line 115 "skeleton.fsm"
case 7 : { 
	 fsm_state =3;     /* entre dans l'etat $IN_QCOMMENT */
	PUTCH( fsm_event );
} break;
#line 120 "skeleton.fsm"
case 8 : { 
	char    yytext[6];
	size_t  pos=1;
	char  c;

	yytext[0]='#';
	while ((c=GETCH())!='\n')
	{
		if (c==EOF)
		{ 
			yytext[pos]='\0';
			PUTS(yytext);
			return(-1);
		}
		if (c=='\\')				/* Escape */
		{
			if (pos<=sizeoftab(yytext))
			{ 
				yytext[pos++]=c;	/* Sauve l'evenement */
			}
			c=GETCH();
			if (c=='\n') fsm.sklineno++;
		}
		if (pos<sizeoftab(yytext)-1)
		{
			yytext[pos++]=c;		/* Sauve l'evenement */
		}
		if (pos==sizeoftab(yytext)-1)
		{
			if (((!fsm.mode) && (fsm.fl.line)) || strncmp("#line",yytext,5))
			{
				yytext[pos]='\0';
				PUTS(yytext);
				while ((c=GETCH())!='\n')
				{
					if (c==EOF) return(-1);
					if (c=='\\')
					{
						PUTCH(c);
						if ((c=GETCH())=='\n') fsm.sklineno++;
					}
       				PUTCH(c);
				}
				PUTCH('\n');
			}
			else /* Supprime le #line si !fsm.fl.line */
			{
				while ((c=GETCH())!='\n')
				{
					if (c==EOF) return(-1);
					if (c=='\\')
       				{
						if ((c=GETCH())=='\n') fsm.sklineno++;
       				}
				}
				PUTCH('\n');
			}
			break;
		}
	}
	fsm.sklineno++;
} break;
#line 183 "skeleton.fsm"
case 9 : { 
	char  yytext[16];
	short pos=1;
	char  c;

	yytext[0]='$';
	while (isalnum(c=GETCH()) || (c=='_') || (c=='$'))
	{
		if (pos<sizeoftab(yytext)-1)
		{
			yytext[pos++]=c;        /* Sauve l'evenement */
		}
	}
	UNGETCH(c);				/* Remet le dernier */
	yytext[pos]='\0';			/* Buf=le mot a convertir */

	{
		conv_token(yytext);
		fprintf(fsm.fout,"\n");
		if (fsm.fl.line) aff_line(fsm.fout,fsm.sklineno,fsm.names);
	}
} break;
#line 214 "skeleton.fsm"
case 10 : { 
	 fsm_state =0;		/* retourne a l'etat $IN_C */
	PUTCH( fsm_event );
} break;
#line 227 "skeleton.fsm"
case 11 : {
	 fsm_state =0;		/* retourne a l'etat $IN_C */
	PUTCH( fsm_event );
} break;
#line 240 "skeleton.fsm"
case 12 : { 
	 fsm_state =4;
	PUTCH( fsm_event );
} break;
#line 245 "skeleton.fsm"
case 13 : { 
	 fsm_state =5;
	PUTCH( fsm_event );
} break;
#line 250 "skeleton.fsm"
case 14 : { 
	 fsm_state =0;		/* Retourne en $IN_C */
	UNGETCH( fsm_event );
} break;
#line 263 "skeleton.fsm"
case 15 : { 
	 fsm_state =0;
	PUTCH( fsm_event );
	fsm.sklineno++;
} break;
#line 277 "skeleton.fsm"
case 16 : { 
	 fsm_state =7;
	PUTCH( fsm_event );
} break;
#line 282 "skeleton.fsm"
case 17 : { 
	 fsm_state =6;
	PUTCH( fsm_event );
} break;
#line 295 "skeleton.fsm"
case 18 : { 
	 fsm_state =5;
	PUTCH( fsm_event );
} break;
#line 300 "skeleton.fsm"
case 19 : { 
	 fsm_state =5;
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
} break;
#line 314 "skeleton.fsm"
case 20 : { 
	 fsm_state =0;
	PUTCH( fsm_event );
} break;
#line 319 "skeleton.fsm"
case 21 : { 
	PUTCH( fsm_event );
} break;
#line 323 "skeleton.fsm"
case 22 : { 
	 fsm_state =5;	/* Pas la fin */
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
} break;

#line 471


    }
#endif 



#ifdef FSM_ARRFNARR
		if ((fsm_ret=fsm_tbl_fnfsm[fsm_tbl_fsm[fsm_state]
			[fsm_cevent-FSM_MAXEXTERN]](&fsm_state,&fsm_event))!=-2)
		{ 
			return(fsm_ret);
		}
#endif 


#ifdef FSM_FNARR
		if ((fsm_ret=fsm_tbl_fsm[fsm_state]
			[fsm_cevent-FSM_MAXEXTERN](&fsm_state,&fsm_event))!=-2)
		{ 
			return(fsm_ret);
		}
#endif 


#ifdef FSM_SWITCH
		
switch(fsm_state)
{
  case 0 : /* $IN_C */
    switch (fsm_event)
    {
      case EOF :
ex_3:
#line 76 "skeleton.fsm"
{ 
	return(-1);
}
        break;
      case '\\' :
ex_4:
#line 87 "skeleton.fsm"
{ 
	char  c;

	PUTCH( fsm_event );
	if ((c=GETCH())==EOF) return(-1); /* Consomme un caractere */
	if (c=='\n') fsm.sklineno++;
	PUTCH(c);
}
        break;
      case '"' :
ex_5:
#line 105 "skeleton.fsm"
{ 
	 fsm_state =1;      /* entre dans l'etat $IN_STR */
	PUTCH( fsm_event );
}
        break;
      case '\'' :
ex_6:
#line 110 "skeleton.fsm"
{
	 fsm_state =2;     /* entre dans l'etat $IN_CHAR */
	PUTCH( fsm_event );
}
        break;
      case '/' :
ex_7:
#line 115 "skeleton.fsm"
{ 
	 fsm_state =3;     /* entre dans l'etat $IN_QCOMMENT */
	PUTCH( fsm_event );
}
        break;
      case '#' :
ex_8:
#line 120 "skeleton.fsm"
{ 
	char    yytext[6];
	size_t  pos=1;
	char  c;

	yytext[0]='#';
	while ((c=GETCH())!='\n')
	{
		if (c==EOF)
		{ 
			yytext[pos]='\0';
			PUTS(yytext);
			return(-1);
		}
		if (c=='\\')				/* Escape */
		{
			if (pos<=sizeoftab(yytext))
			{ 
				yytext[pos++]=c;	/* Sauve l'evenement */
			}
			c=GETCH();
			if (c=='\n') fsm.sklineno++;
		}
		if (pos<sizeoftab(yytext)-1)
		{
			yytext[pos++]=c;		/* Sauve l'evenement */
		}
		if (pos==sizeoftab(yytext)-1)
		{
			if (((!fsm.mode) && (fsm.fl.line)) || strncmp("#line",yytext,5))
			{
				yytext[pos]='\0';
				PUTS(yytext);
				while ((c=GETCH())!='\n')
				{
					if (c==EOF) return(-1);
					if (c=='\\')
					{
						PUTCH(c);
						if ((c=GETCH())=='\n') fsm.sklineno++;
					}
       				PUTCH(c);
				}
				PUTCH('\n');
			}
			else /* Supprime le #line si !fsm.fl.line */
			{
				while ((c=GETCH())!='\n')
				{
					if (c==EOF) return(-1);
					if (c=='\\')
       				{
						if ((c=GETCH())=='\n') fsm.sklineno++;
       				}
				}
				PUTCH('\n');
			}
			break;
		}
	}
	fsm.sklineno++;
}
        break;
      case '$' :
ex_9:
#line 183 "skeleton.fsm"
{ 
	char  yytext[16];
	short pos=1;
	char  c;

	yytext[0]='$';
	while (isalnum(c=GETCH()) || (c=='_') || (c=='$'))
	{
		if (pos<sizeoftab(yytext)-1)
		{
			yytext[pos++]=c;        /* Sauve l'evenement */
		}
	}
	UNGETCH(c);				/* Remet le dernier */
	yytext[pos]='\0';			/* Buf=le mot a convertir */

	{
		conv_token(yytext);
		fprintf(fsm.fout,"\n");
		if (fsm.fl.line) aff_line(fsm.fout,fsm.sklineno,fsm.names);
	}
}
        break;
      default :
#line 63 "skeleton.fsm"
{ 
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
}
       break;
    }
    break;
  case 1 : /* $IN_STR */
    switch (fsm_event)
    {
      case EOF :
        goto ex_3;
      case '\\' :
        goto ex_4;
      case '"' :
ex_10:
#line 214 "skeleton.fsm"
{ 
	 fsm_state =0;		/* retourne a l'etat $IN_C */
	PUTCH( fsm_event );
}
        break;
      default :
#line 63 "skeleton.fsm"
{ 
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
}
       break;
    }
    break;
  case 2 : /* $IN_CHAR */
    switch (fsm_event)
    {
      case EOF :
        goto ex_3;
      case '\\' :
        goto ex_4;
      case '\'' :
ex_11:
#line 227 "skeleton.fsm"
{
	 fsm_state =0;		/* retourne a l'etat $IN_C */
	PUTCH( fsm_event );
}
        break;
      default :
#line 63 "skeleton.fsm"
{ 
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
}
       break;
    }
    break;
  case 3 : /* $IN_QCOMMENT */
    switch (fsm_event)
    {
      case EOF :
        goto ex_3;
      case '\\' :
        goto ex_4;
      case '/' :
ex_12:
#line 240 "skeleton.fsm"
{ 
	 fsm_state =4;
	PUTCH( fsm_event );
}
        break;
      case '*' :
ex_13:
#line 245 "skeleton.fsm"
{ 
	 fsm_state =5;
	PUTCH( fsm_event );
}
        break;
      default :
ex_14:
#line 250 "skeleton.fsm"
{ 
	 fsm_state =0;		/* Retourne en $IN_C */
	UNGETCH( fsm_event );
}
        break;
    }
    break;
  case 4 : /* $IN_CPPCOMMENT */
    switch (fsm_event)
    {
      case EOF :
        goto ex_3;
      case '\\' :
        goto ex_4;
      case '\n' :
ex_15:
#line 263 "skeleton.fsm"
{ 
	 fsm_state =0;
	PUTCH( fsm_event );
	fsm.sklineno++;
}
        break;
      default :
#line 63 "skeleton.fsm"
{ 
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
}
       break;
    }
    break;
  case 5 : /* $IN_COMMENT */
    switch (fsm_event)
    {
      case EOF :
        goto ex_3;
      case '\\' :
        goto ex_4;
      case '*' :
ex_16:
#line 277 "skeleton.fsm"
{ 
	 fsm_state =7;
	PUTCH( fsm_event );
}
        break;
      case '/' :
ex_17:
#line 282 "skeleton.fsm"
{ 
	 fsm_state =6;
	PUTCH( fsm_event );
}
        break;
      default :
#line 63 "skeleton.fsm"
{ 
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
}
       break;
    }
    break;
  case 6 : /* $IN_NCOMMENT */
    switch (fsm_event)
    {
      case EOF :
        goto ex_3;
      case '\\' :
        goto ex_4;
      case '*' :
ex_18:
#line 295 "skeleton.fsm"
{ 
	 fsm_state =5;
	PUTCH( fsm_event );
}
        break;
      default :
ex_19:
#line 300 "skeleton.fsm"
{ 
	 fsm_state =5;
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
}
        break;
    }
    break;
  case 7 : /* $IN_FCOMMENT */
    switch (fsm_event)
    {
      case EOF :
        goto ex_3;
      case '\\' :
        goto ex_4;
      case '/' :
ex_20:
#line 314 "skeleton.fsm"
{ 
	 fsm_state =0;
	PUTCH( fsm_event );
}
        break;
      case '*' :
ex_21:
#line 319 "skeleton.fsm"
{ 
	PUTCH( fsm_event );
}
        break;
      default :
ex_22:
#line 323 "skeleton.fsm"
{ 
	 fsm_state =5;	/* Pas la fin */
	PUTCH( fsm_event );
	if ( fsm_event =='\n') fsm.sklineno++;
}
        break;
    }
    break;
}

#line 497


#ifdef FSM_USE_ERROR
		goto next;
    ex_0 :	
		FSM_PRINT_TRACE; FSM_PERROR(0,"unknow state or event");
		return(STATE_ERROR);
    next:
#endif

#endif 



		FSM_AFTER;
	}
	return(STATE_ERROR);
}


#line 330 "skeleton.fsm"


/******************/
/* conv_squelette */
/******************/

/* - Copie le squelette et convertie les cles "$...$" ou "$..." - */

void   conv_squelette()

{ 
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
	automate(0,0);
	if (yyin!=NULL) fclose(yyin);
}

#line 516

