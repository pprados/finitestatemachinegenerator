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
#include <string.h>
#include <assert.h>

#include "config.h"

/**************/
/* _splitpath */
/**************/

#if defined(__UNIX__) || defined(__GNUC__)
void    _splitpath(
	const char* path,char* drive,
	char* dir,char* name,char* ext)
{ 
	const char    *ptd,*ptf,*ptm;
	int           size;

	if (drive!=NULL) drive[0]='\0';

/* - Recuperation du dir - */
	ptd=path;
	ptf=strrchr(ptd,'/');
	if (ptf==NULL) ptf=strrchr(ptd,'\\');
	if (ptf==NULL) 
	{  
		if (dir!=NULL) *dir='\0'; 
	}
	else
	{ 
		if (dir!=NULL)
		{ 
			strncpy(dir,ptd,size=1+ptf-ptd);
			dir[size]='\0';
		}
		ptd=ptf+1;
	}
  
/* - Recuperation du name - */
	ptf=strchr(ptd,'.');
	if (name!=NULL)
	{ 
		if (ptf==NULL) strcpy(name,ptd);
		else
		{ 
			strncpy(name,ptd,size=ptf-ptd);
			name[size]='\0';
		}
	}
  
/* - Recuperation de ext - */
	if (ext!=NULL)
	{
		if (ptf==NULL) *ext='\0';
		else strcpy(ext,ptf);
	}
}
#endif
