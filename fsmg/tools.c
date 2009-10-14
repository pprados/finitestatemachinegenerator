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
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#include "all.h"
#include "tools.h"

/************/
/* aff_line */
/************/

void	aff_line(
	FILE* st,
	int line,
	const char* file
	)
{ 
	char		buf[MAX_PATH];
	char*		pt;

	strcpy(buf,file);
	for (pt=strchr(buf,'\\');pt!=NULL;pt=strchr(pt,'\\'))
	*pt++='/';
	fprintf(st,"#line %hd",line);
	if (*buf) fprintf(st," \"%s\"",buf);
	fputc('\n',st);
}

/********/
/* atou */
/********/

unsigned atou(const char* str)
{ 
	long		r=0;

	r=atol(str);
	if (r>UINT_MAX)
		r=UINT_MAX;
	return((unsigned)r);
}

/***********/
/* add_ext */
/***********/

char* add_ext(char* chaine,const char* ext)
{ 
	char*          pt;

#if defined(_WIN32)
	if ((pt=strrchr(chaine,'\\'))==NULL) pt=chaine;
#else
	if ((pt=strrchr(chaine,'/'))==NULL) pt=chaine;
#endif
	if (strrchr(pt,'.')==NULL)
	{ 
		strcat(chaine,ext); 
	}
#if defined(_WIN32)
		return(strlwr(chaine));
#else
		return(chaine);
#endif
}

