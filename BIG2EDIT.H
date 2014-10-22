/*! @file BIG2EDIT.H
@brief declaration of functions handling editable fields

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

#ifndef _BIG2EDIT_H_
#define _BIG2EDIT_H_

/* global variables */
extern int edit;
extern int object;
extern int kbd;
extern int key;
extern t_display work_display;
extern char *buffer;
 
/* function prototypes */
int		exist_edit (OBJECT *adr);
void	empty_edit (OBJECT *adr);
int		pos_curs (OBJECT *adr, int ed, int mx, int f, int index, int po);
int		prev (OBJECT *adr, int ob);
int		next (OBJECT *adr, int ob);
void	first (OBJECT *adr, int *editable, int *position);
void	last (OBJECT *adr, int *editable, int *position);
int		next_word (OBJECT *adr, int ob, int posi);
int		prev_word (OBJECT *adr, int ob, int posi);
int		valide (char *ascii, char *valid, int p);
char	*start_edit (char *pt);
void	draw_curs (OBJECT *adr, int ed, int *po, int action);
void	copy (OBJECT *adr, int ed);
void	cut (OBJECT *adr, int ed);
void	paste (OBJECT *adr, int ed);

#endif