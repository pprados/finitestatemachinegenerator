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

/***********/
/* DEBUG.H */
/***********/

#ifndef __DEBUG_H
#define __DEBUG_H

#ifndef NDEBUG
#define dmem(x) 			int _mem_cnt_##x;	
#define dinc(x) 			(_mem_cnt_##x++)	
#define ddec(x) 			(_mem_cnt_##x--)	
#define dval(x) 			(_mem_cnt_##x)		

#define 	dprintf(x)		fprintf x
#define		dif(tst,cmd)	if (tst) cmd		

#ifdef DEBUG_0
#define d0(fn)	fn
#else
#define d0(fn)
#endif
#ifdef DEBUG_1
#define d1(fn)	fn
#else
#define d1(fn)
#endif
#ifdef DEBUG_2
#define d2(fn)	fn
#else
#define d2(fn)
#endif
#ifdef DEBUG_3
#define d3(fn)	fn
#else
#define d3(fn)
#endif
#ifdef DEBUG_4
#define d4(fn)	fn
#else
#define d4(fn)
#endif
#else
  #define dprintf(fmt)
  #define d0(fn)
  #define d1(fn)
  #define d2(fn)
  #define d3(fn)
  #define d4(fn)

  #define dmem(x)
  #define dinc(x)
  #define ddec(x)
  #define dval(x)

  #define dif(tst,cmd)
#endif
#endif
