/*! @file BIG2FOR1.H
@brief declaration of functions handling forms

@verbatim
BIG = "BIG Is GEM" - A high level GEM library. 
Initial Development by Claude ATTARD, Maintenance by Jean LOUIS-GUERIN
Copyright (c) 1993-2014 Claude ATTARD
Copyright (c) 2010-2014 Jean LOUIS-GUERIN

website: http://info-coach.fr/atari/software/system/big.php
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

#ifndef _BIG2FOR1_H_
#define _BIG2FOR1_H_

#include "BIG2INCL.H"

/* global variables */
extern int ap_id;
extern int buf[];
extern int edit;
extern int pos;
extern int object;
extern t_display work_display;
extern t_desktop bureau;
extern OBJECT *adr_menu;
extern OBJECT *scroll;
extern MFDB screen;
extern OBJECT **ir_trindex;
extern FNCP *fnc;
extern OBJECT *bi_trindex[];

/* function prototype */
int		menu_hierarch (int option);
void	pop_up (OBJECT *adr, int object);
void	pop_liste (OBJECT *adr, int object);
void	next_option (OBJECT *adr, int object, int form);
int		create_scroll (OBJECT *pu, int y, int *nb, int first);

#endif