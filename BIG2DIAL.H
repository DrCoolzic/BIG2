/*! @file big2dial.h
@brief declaration of dialog box functions

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

#ifndef _BIG2DIAL_H_
#define _BIG2DIAL_H_

/* global variables */
extern int id_top;		/* Index fenˆtre 1ø plan */
extern int old_top;
extern int pal_img;
extern MOBLK mouse;
extern int ap_id;
extern int event;
extern int buf[];
extern int mx;
extern int my;
extern int mk;
extern int edit;
extern int pos;
extern int object;
extern int kbd;
extern int key;
extern int clik;
extern int wind;
extern t_display work_display;
extern t_desktop bureau;
extern OBJECT *adr_menu;
extern OBJECT *adr_desk;
extern OBJECT **ir_trindex;
extern FNCP *fnc;
extern BigWind	*win;

/* function prototype */
int dlg_keybd (OBJECT *adresse, int f, int *dedans, int *fl_cla);
int	dlg_mesag (void);
int	dlg_button (OBJECT *adresse, int f, int dedans, int fl_cla, 
				MFDB *img, int fl_depl, int fl_notop);
int option_find (int xm, int ym);
int	menu_ok (void);
int dial (OBJECT *address, MFDB *img, int fl_move);

#endif