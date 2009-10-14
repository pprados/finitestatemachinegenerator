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

#ifndef CONFIG_H
#define CONFIG_H

#include "conf.h"

#if STDC_HEADERS
#  include <string.h>
#else
# ifndef HAVE_STRCHR
#    define strchr index
#    define strrchr rindex
#  endif
char* strchr(),*strrchr();
#  ifndef HAVE_MEMCPY
#    define memcpy(d,s,n) bcopy((s),(d),(n))
#  endif
#endif

#define bool		int
#define true		(1)
#define false		(0)

#define sizeoftab(x) (sizeof(x)/sizeof((x)[0]))

#define MAX_PATH	255
#define MAX_DRIVE	2
#define MAX_DIR		255
#define MAX_FNAME	255
#define MAX_EXT		255

#ifdef _WIN32
#define FTXT		/*"t" /* For open file in text mode */
#else
#define FTXT		/* For open file in text mode */
#endif

#endif
