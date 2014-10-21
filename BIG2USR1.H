/*! @file BIG2USR1.H
@brief declaration of functions for userdef objects

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

#ifndef _BIG2USR1_H_
#define _BIG2USR1_H_

#include "BIG2INCL.H"

	/* Nombre de gadgets USERDEFs dans les fenˆtres BIG : */
#define NU 12
	/* Nombre total de USERDEFs pour les fenˆtres BIG : */
#define NTU NU + 8
	/* Nombre d'objets USERDEFs dans les autres formulaires */
#define NFU 4

/* global variables */
extern t_display work_display;
extern t_desktop bureau;
extern OBJECT *adr_menu;
extern USERBLK *adr_usr;
extern char *xedit;
extern t_image_dial *xpict;
extern int nb_pict;
extern OBJECT **ir_trindex;
extern char **ir_frstr;
extern FNCP *fnc;
extern OBJECT *bi_trindex[];

typedef struct obj_user
{
	int		under;				/* Soulignement oui / non */
	int		cunder;				/* Couleur caractŠre soulign‚ */
	int		ctxt_under;		/* Couleur texte B_UNDER */
	int		cicn_coche;		/* Couleur ic“ne B_CHECKB */
	int		ctxt_coche;		/* Couleur texte B_CHECKB */
	int		cicn_radio;		/* Couleur ic“ne B_ROUNDB */
	int		ctxt_radio;		/* Couleur texte B_ROUNDB */
	int		cicn_popup;		/* Couleur ic“ne B_POPUP / B_LIST */
	int		ctxt_popup;		/* Couleur texte B_POPUP / B_LIST */
	int		cmove;				/* Couleur B_MOVE */
	int		cgrastr;			/* Couleur texte B_GRASTR */
	int		clin_frame;		/* Couleur lignes B_FRAME */
	int		ctxt_frame;		/* Couleur texte B_FRAME */
	int		cniceline;		/* Couleur B_NICELINE */
} Obj_user;


/* #[ Prototypage des fonctions : */
void	userdef (char *rsc, int *nb_form, int nb_tree);
int		cdecl bcoche (PARMBLK *para);
int		cdecl brond (PARMBLK *para);
int		cdecl bunder (PARMBLK *para);
int		cdecl bcadre (PARMBLK *para);
int		cdecl bmove (PARMBLK *para);
int		cdecl bgrastr (PARMBLK *para);
int		cdecl bsmstr (PARMBLK *para);
int		cdecl bpopup (PARMBLK *para);
int		cdecl bpict (PARMBLK *para);
int		cdecl bmhier (PARMBLK *para);
int		cdecl bniceline (PARMBLK *para);

#endif