/*! @file BIG2WINB.H
@brief declaration of functions to  manage BIG windows

@verbatim
BIG = "BIG Is GEM" - A high level GEM library. 
Initial Development by Claude ATTARD, Maintenance by Jean LOUIS-GUERIN
Copyright (c) 1993-2014 Claude ATTARD
Copyright (c) 2010-2014 Jean LOUIS-GUERIN

website: http://info-coach.fr/atari/software/projects/big.php
forum:   http://www.atari-forum.com/viewtopic.php?f=16&t=27060

The BIG library may be used and distributed without restriction provided that 
this copyright statement is not removed from the file and that any derivative 
work contains the original copyright notice and the associated disclaimer.

The BIG library  is free software; you can redistribute it and/or modify  it 
under the terms of the GNU General Public License as published by the Free 
Software Foundation; either version 3 of the License, or (at your option) any 
later version.

The BIG library is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
HxCFloppyEmulator; if not, write to the Free Software Foundation, Inc., 51 
Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
@endverbatim

*/

#ifndef _BIG2WINB_H_
#define _BIG2WINB_H_

/* global variables */
extern OBJECT *bi_trindex[];

/* function prototype */
void put_fen (int index);
void no_top (int obj, int index);
int asce (int obj, int index);
void arrows (int obj, int index);
void slid (int obj, int index);
void sizebw (int obj, int index);
void movebw (int index);
void resize (int index, int deltaw, int deltah);
void draw_obj (OBJECT *adr, int obj);
void draw_wind (int index, GRECT *r);

#endif