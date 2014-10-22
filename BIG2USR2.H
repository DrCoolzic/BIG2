/*! @file BIG2USR2.H
@brief declaration of functions for userdef objects

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

#ifndef _BIG2USR2_H_
#define _BIG2USR2_H_

#include "BIG2INCL.H"

/* global variables */
extern t_display work_display;
extern int id_top;
extern OBJECT *bi_trindex[];

typedef struct win_user
{
	int		mtxt_mover;		/* Mode texte MOVER */
	int		ctxt_mover;		/* Couleur texte MOVER */
	int		spat_mover;		/* Style trame MOVER */
	int		cpat_mover;		/* Couleur trame MOVER */
	int		ctxt_info;		/* Couleur texte INFO */
	int		spat_velev;		/* Style trame VELEV */
	int		cpat_velev;		/* Couleur trame VELEV */
	int		spat_helev;		/* Style trame HELEV */
	int		cpat_helev;		/* Couleur trame HELEV */
	int		spat_vslid;		/* Style trame VSLIDE */
	int		cpat_vslid;		/* Couleur trame VSLIDE */
	int		spat_hslid;		/* Style trame HSLIDE */
	int		cpat_hslid;		/* Couleur trame HSLIDE */
	int		cpat_gadgt;		/* Couleur fond gadgets */
	int		cicn_gadgt;		/* Couleur ic“ne gadgets */
} Win_user;

/* #[ Prototypage des fonctions : */
int		cdecl fond (PARMBLK *para);
int		cdecl gadget (PARMBLK *para);
int		cdecl titinf (PARMBLK *para);
int		cdecl elev (PARMBLK *para);
int		cdecl sld (PARMBLK *para);
int		cdecl work (PARMBLK *para);

#endif