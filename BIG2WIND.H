/*! @file BIG2WIND.H
@brief declaration of functions to manage windows

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

#ifndef _BIG2WIND_H_
#define _BIG2WIND_H_

#include "BIG2INCL.H"

/* global variables */
extern MOBLK mouse;
extern int ap_id;
extern int event;
extern int buf[];
extern int mk;
extern t_display work_display;
extern t_desktop bureau;
extern OBJECT *scroll;
extern OBJECT **ir_trindex;
extern FNCP *fnc;
extern BigWind	*win;
extern char *bi_frstr[];
extern OBJECT *bi_trindex[];

/* function prototype */
void	dcl_windows (int nbre);
void	create_wmenu (int tree, int index);
int		menu_window (int index, int xm, int ym);
int		wind_hierarch (OBJECT *adr, int index, int option);
void	wind_mouse (void);
void	print_line (int f, int index);
void	print_colum (int f, int index);
void	print_wind (int index);

#endif