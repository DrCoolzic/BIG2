/*! @file BIG2EDIT.C
@brief implementation of functions handling editable fields

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

#include "BIG2.H"
#include "BIG2EDIT.H"

/****** Fonctions pour les EDITABLEs ******************************/
/* #[ exist_edit () Cherche s'il y a un Çditable :								*/
int exist_edit (OBJECT *adr)
{
register int i = ZERO;
int type;

	if (edit == BLANK)
		return FALSE;
	else
	{
		do
		{
			type = (adr[i].ob_type) & 0xFF;
			if ((type == G_FTEXT) || (type == G_FBOXTEXT))
				return TRUE;
		} while (NOT (adr[i++].ob_flags & LASTOB));
		return FALSE;
	}
}
/* #] exist_edit () Cherche s'il y a un Çditable :								*/ 
/* #[ set_text () Ecrire une G_STRING ou un G_TEXT :							*/
void set_text (OBJECT *adr, int object, char *string)
{
int type;
char *pt;

	type = adr[object].ob_type & 0xff;
	if ((type == G_STRING) || (type == G_BUTTON))
		strcpy (adr[object].ob_spec.free_string, string);
	else if (type == G_USERDEF)
		strcpy ((char *)adr[object].ob_spec.userblk->ub_parm, string);
	if ((type == G_TEXT) || (type == G_BOXTEXT))
		strcpy (adr[object].ob_spec.tedinfo->te_ptext, string);
	if ((type == G_FTEXT) || (type == G_FBOXTEXT))
	{
		pt = (adr[object].ob_spec.tedinfo->te_ptext);
		pt = start_edit (pt);
		adr[object].ob_spec.tedinfo->te_ptext = pt;
		strcpy (adr[object].ob_spec.tedinfo->te_ptext, string);
	}
}
/* #] set_text () Ecrire une G_STRING ou un G_TEXT :							*/ 
/* #[ get_text () Lire une G_STRING ou un G_TEXT:									*/
char *get_text (OBJECT *adr, int object)
{
int type;
char *retour = "";

	type = adr[object].ob_type & 0xff;

	if ((type == G_STRING) || (type == G_BUTTON))
		retour = (adr[object].ob_spec.free_string);
	else if (type == G_USERDEF)
		if ((adr[object].ob_type >> 8) != ZERO)
			retour = (char *)(adr[object].ob_spec.userblk->ub_parm);
		else
			retour = "";
	else if ((type == G_TEXT) || (type == G_BOXTEXT))
		retour = (adr[object].ob_spec.tedinfo->te_ptext);
	else if ((type == G_FTEXT) || (type == G_FBOXTEXT))
		retour = start_edit (adr[object].ob_spec.tedinfo->te_ptext);

	return retour;
}
/* #] get_text () Lire une G_STRING ou un G_TEXT:									*/ 
/* #[ empty_edit () Vider les Çditables :													*/
void empty_edit (OBJECT *adr)
{
register int i = ZERO;

	do
	{
		if (adr[i].ob_flags & EDITABLE)
			set_text (adr, i, "");
	} while (NOT (adr[i++].ob_flags & LASTOB));		/* Jusqu'au dernier objet */
}
/* #] empty_edit () Vider les Çditables :													*/ 
/* #[ pos_curs () Position curseur dans Çditable cliquÇ :					*/
int pos_curs (OBJECT *adr, int ed, int mx, int f, int index, int po)
{
int x, w, delta, ob_x, new_pos, dummy, i, largv = ZERO;
char *texte, *tmplt, car, *val;
TEDINFO *ted;

	ted = adr[object].ob_spec.tedinfo;		/* Adresse TEDINFO */
	if (ted->te_font == IBM)
		vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &dummy);
	else
		vst_height (work_display.handle, HMINUS, &dummy, &dummy, &w, &dummy);
	texte = (ted->te_ptext);						/* Texte du champ */
	tmplt = (ted->te_ptmplt);						/* Texte template */
	for (i = ZERO ; i < strlen (tmplt) ; i++)
	{
		if (tmplt[i] == '_')
			largv++;
	}
	objc_offset (adr, ed, &x, &dummy);	/* CoordonnÇes Çditable */
		/* DiffÇrence de largeur entre l'objet et le template */
	delta = adr[ed].ob_width - ((int)strlen (tmplt) * w);
	ob_x = mx - x;							/* Position cliquage dans l'objet */
	switch (ted->te_just)				/* Correction selon justif */
	{
	case TE_RIGHT :			/* A droite */
		ob_x += delta;
		break;
	case TE_CNTR :			/* CentrÇ */
		ob_x += (delta / 2);
		break;
	}
	ob_x /= w;
	car = tmplt[ob_x];
	if ((car == '_') || (NOT car))
	{
		new_pos = ob_x - ((int)strlen (tmplt) - largv);
		val = strchr (tmplt, '_');
		for (i = ZERO ; i < strlen (val) ; i++)
		{
			if (val[i] != '_')
				if (i > new_pos)
					new_pos++;
		}
		if (new_pos > (int)strlen (texte))
			new_pos = (int)strlen (texte);
		draw_curs (adr, ed, &po, ED_END);
		draw_curs (adr, ed, &new_pos, ED_END);
	}
	else if ((adr[ed].ob_type >> 8) == B_EDIT)
	{
		if (car == '\4')
		{
			key = ARLF;
			draw_curs (adr, ed, &po, ED_END);
			new_pos = po = ZERO;
			draw_curs (adr, ed, &po, ED_END);
			objc_xedit (adr, key, ed, &new_pos, ED_CHAR, f, index);
		}
		else if (car == '\3')
		{
			key = ARRT;
			draw_curs (adr, ed, &po, ED_END);
			new_pos = po = min (largv, (int)strlen (texte));
			draw_curs (adr, ed, &po, ED_END);
			objc_xedit (adr, key, ed, &new_pos, ED_CHAR, f, index);
		}
		else
		{
			new_pos = ZERO;
			draw_curs (adr, ed, &po, ED_END);
			draw_curs (adr, ed, &new_pos, ED_END);
		}
	}
	else if ((adr[ed].ob_type >> 8) == ZERO)
	{
		draw_curs (adr, ed, &po, ED_END);
		new_pos = ZERO;
		draw_curs (adr, ed, &new_pos, ED_END);
	}

	return new_pos;
}
/* #] pos_curs () Position curseur dans Çditable cliquÇ :					*/ 
/* #[ prev () Champ prÇcÇdent :																		*/
int prev (OBJECT *adr, int ob)
{
int pere, vu;

	while (ob-- > ZERO)
	{
		vu = TRUE;
		pere = parent (adr, ob);
		while ((pere > ZERO) && (vu))
		{
			if (adr[pere].ob_flags & HIDETREE)
				vu = FALSE;
			pere = parent (adr, pere);
		}
		if (vu)
			if ((adr[ob].ob_flags & EDITABLE) && (NOT (adr[ob].ob_flags & HIDETREE)) && (NOT (adr[ob].ob_state & DISABLED)))
				return ob;
	}
	return BLANK;
}
/* #] prev () Champ prÇcÇdent :																		*/ 
/* #[ next () Champ suivant :																			*/
int next (OBJECT *adr, int ob)
{
int pere, vu;

	while (NOT (adr[ob++].ob_flags & LASTOB))
	{
		vu = TRUE;
		pere = parent (adr, ob);
		while ((pere > ZERO) && (vu))
		{
			if (adr[pere].ob_flags & HIDETREE)
				vu = FALSE;
			pere = parent (adr, pere);
		}
		if (vu)
			if ((adr[ob].ob_flags & EDITABLE) && (NOT (adr[ob].ob_flags & HIDETREE)) && (NOT (adr[ob].ob_state & DISABLED)))
				return ob;
	}
	return BLANK;
}
/* #] next () Champ suivant :																			*/ 
/* #[ first () Amäne le curseur sur le premier Çditable :					*/
void first (OBJECT *adr, int *editable, int *position)
{
int ed = ZERO, pere, vu;

	while (NOT (adr[ed].ob_flags & LASTOB))
	{
		vu = FALSE;
		if (adr[ed].ob_flags & EDITABLE)
		{
			vu = TRUE;
			pere = parent (adr, ed);
			while ((pere > ZERO) && (vu))
			{
				if (adr[pere].ob_flags & HIDETREE)
					vu = FALSE;
				pere = parent (adr, pere);
			}
		}
		if (vu)
		{
			if ((ed < *editable) && NOT (adr[ed].ob_state & DISABLED) && NOT (adr[ed].ob_state & HIDETREE))
			{
				draw_curs (adr, *editable, position, ED_END);
				*editable = ed;
				if ((adr[*editable].ob_type >> 8) == B_EDIT)
				{
					*position = min ((int)strlen (adr[ed].ob_spec.tedinfo->te_ptext),
													 (int)strlen (adr[ed].ob_spec.tedinfo->te_pvalid));
					draw_curs (adr, *editable, position, ED_END);
				}
				else
					draw_curs (adr, *editable, position, ED_INIT);
				return;
			}
		}
		ed++;
	}
}
/* #] first () Amäne le curseur sur le premier Çditable :					*/ 
/* #[ last () Amäne le curseur sur le dernier Çditable :					*/
void last (OBJECT *adr, int *editable, int *position)
{
int ed = *editable, pere, vu;

	while (NOT (adr[ed].ob_flags & LASTOB))
		ed++;
	while (ed > ZERO)
	{
		vu = FALSE;
		if (adr[ed].ob_flags & EDITABLE)
		{
			vu = TRUE;
			pere = parent (adr, ed);
			while ((pere > ZERO) && (vu))
			{
				if (adr[pere].ob_flags & HIDETREE)
					vu = FALSE;
				pere = parent (adr, pere);
			}
		}
		if (vu)
		{
			if ((ed > *editable) && NOT (adr[ed].ob_state & DISABLED) && NOT (adr[ed].ob_state & HIDETREE))
			{
				draw_curs (adr, *editable, position, ED_END);
				*editable = ed;
				if ((adr[*editable].ob_type >> 8) == B_EDIT)
				{
					*position = min ((int)strlen (adr[ed].ob_spec.tedinfo->te_ptext),
													 (int)strlen (adr[ed].ob_spec.tedinfo->te_pvalid));
					draw_curs (adr, *editable, position, ED_END);
				}
				else
					draw_curs (adr, *editable, position, ED_INIT);
				return;
			}
		}
		ed--;
	}
}
/* #] last () Amäne le curseur sur le dernier Çditable :					*/ 
/* #[ next_word () Place curseur au mot suivant :									*/
int next_word (OBJECT *adr, int ob, int posi)
{
char chaine [256], *debut;
int l, po, flag = FALSE;

	po = posi;
	po--; 												/* Si on est dÇjÖ Ö la fin d'un mot ! */
	strcpy (chaine, get_text (adr, ob));	/* Chaine de travail */
	debut = strstr (chaine, adr[ob].ob_spec.tedinfo->te_ptext);
	strcpy (chaine, debut);
	chaine[strlen (adr[ob].ob_spec.tedinfo->te_pvalid)] = '\0';
	l = (int)strlen (chaine);					/* Longueur chaine */
	while ((++po < l) && (flag == FALSE))	/* Tant qu'on n'est pas Ö la fin... */
	{
		if (*(chaine + po) == ' ')	/* Si espace, */
			flag = TRUE;
	}
	if (po > l)
		po = l;
	draw_curs (adr, ob, &posi, ED_END);		/* DÇsactiver le curseur */
	posi = po;
	draw_curs (adr, ob, &posi, ED_END);		/* RÇactiver le curseur */
	return posi;
}
/* #] next_word () Place curseur au mot suivant :									*/ 
/* #[ prev_word () Place curseur au mot prÇcÇdent :								*/
int prev_word (OBJECT *adr, int ob, int posi)
{
char chaine[256], *debut;
int po, flag = FALSE;

	po = posi;
	po--; 												/* Si on est dÇjÖ au dÇbut d'un mot ! */
	strcpy (chaine, get_text (adr, ob));	/* Chaine de travail */
	debut = strstr (chaine, adr[ob].ob_spec.tedinfo->te_ptext);
	strcpy (chaine, debut);
	while ((--po > ZERO) && (flag == FALSE))	/* Tant qu'on n'est pas au dÇbut... */
	{
		if (*(chaine + po) == ' ')	/* Si espace, */
		{
			po += 2;
			flag = TRUE;
		}
	}
	if (po < ZERO)
		po = ZERO;
	draw_curs (adr, ob, &posi, ED_END);		/* DÇsactiver le curseur */
	posi = po;
	draw_curs (adr, ob, &posi, ED_END);		/* RÇactiver le curseur */
	return posi;
}
/* #] prev_word () Place curseur au mot prÇcÇdent :								*/ 
/* #[ objc_xedit () Gestion compläte des champs Çditables :				*/
void objc_xedit (OBJECT *adr, int car, int ed, int *po, int action, int f, int index)
{
int largeur = ZERO, longueur, i;
char *texte, ascii, *ptr, *pt, *tmplt, *p, *valid;
TEDINFO *ted;

	ted = adr[ed].ob_spec.tedinfo;		/* Adresse TEDINFO */
	tmplt = (ted->te_ptmplt);					/* Texte template */
	valid = (ted->te_pvalid);					/* Texte filtre */
	for (i = ZERO ; i < strlen (tmplt) ; i++)
	{
		if (tmplt[i] == '_')
			largeur++;
	}
	longueur = adr[ed].ob_state >> 8;
	texte = (ted->te_ptext);						/* Texte du champ */

	if ((action == ED_INIT) || (action == ED_END))	/* SI ACTION DE CURSEUR */
		draw_curs (adr, ed, po, action);
	else if (action == ED_CHAR)											/* SI ACTION DE CARACTERE */
	{
		switch (car)
		{
		case ARLF :
			if ((adr[ed].ob_type >> 8) == B_EDIT)
			{
				if (*po == ZERO)
				{
					pt = (ted->te_ptext);
					pt = start_edit (pt);
					if (ted->te_ptext > pt)
					{
						(ted->te_ptext)--;
						draw_curs (adr, ed, po, ED_END);
						if (f)
							draw_object (ed, index);
						else
							objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
												 adr->ob_width, adr->ob_height);
						draw_curs (adr, ed, po, ED_END);
					}
				}
				else
				{
					draw_curs (adr, ed, po, ED_END);
					(*po)--;
					draw_curs (adr, ed, po, ED_END);
				}
			}
			else
			{
				if (*po > ZERO)
				{
					draw_curs (adr, ed, po, ED_END);
					(*po)--;
					draw_curs (adr, ed, po, ED_END);
				}
			}
			break;
		case ARRT :
			if ((adr[ed].ob_type >> 8) == B_EDIT)
			{
				if (*po == largeur)
				{
					pt = (ted->te_ptext);
					pt = start_edit (pt);
					if (ted->te_ptext - pt + largeur <
							(int)strlen (pt))
					{
						(ted->te_ptext)++;
						draw_curs (adr, ed, po, ED_END);
						if (f)
							draw_object (ed, index);
						else
							objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
												 adr->ob_width, adr->ob_height);
						draw_curs (adr, ed, po, ED_END);
					}
				}
				else if (*po < strlen (texte))
				{
					draw_curs (adr, ed, po, ED_END);
					(*po)++;
					draw_curs (adr, ed, po, ED_END);
				}
			}
			else
			{
				if (*po < strlen (texte))
				{
					draw_curs (adr, ed, po, ED_END);
					(*po)++;
					draw_curs (adr, ed, po, ED_END);
				}
			}
			break;
		case CT_ARLF :	/* DÇjÖ traitÇ */
			break;
		case CT_ARRT :	/* DÇjÖ traitÇ */
			break;
		case SH_ARLF :
			if (*po > ZERO)
			{
				draw_curs (adr, ed, po, ED_END);
				*po = ZERO;
				draw_curs (adr, ed, po, ED_END);
			}
			else
			{
				pt = (ted->te_ptext);
				pt = start_edit (pt);
				ted->te_ptext = pt;
				draw_curs (adr, ed, po, ED_END);
				if (f)
					draw_object (ed, index);
				else
					objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
										 adr->ob_width, adr->ob_height);
				draw_curs (adr, ed, po, ED_END);
			}
			break;
		case SH_ARRT :
			if (*po < largeur)
			{
				draw_curs (adr, ed, po, ED_END);
				*po = min (largeur, (int)strlen (ted->te_ptext));
				draw_curs (adr, ed, po, ED_END);
			}
			else
			{
				pt = (ted->te_ptext);
				pt = start_edit (pt);
				ted->te_ptext = pt + (int)strlen (pt) - min ((int)strlen (ted->te_ptext), largeur);
				draw_curs (adr, ed, po, ED_END);
				if (f)
					draw_object (ed, index);
				else
					objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
										 adr->ob_width, adr->ob_height);
				draw_curs (adr, ed, po, ED_END);
			}
			break;
		case ESC :
			pt = (ted->te_ptext);
			pt = start_edit (pt);
			*pt = '\0';
			ted->te_ptext = pt;
			draw_curs (adr, ed, po, ED_END);
			*po = ZERO;
			if (f)
				draw_object (ed, index);
			else
				objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
									 adr->ob_width, adr->ob_height);
			draw_curs (adr, ed, po, ED_END);
			break;
		case BACKSPC :
			if (*po > ZERO)
			{
				draw_curs (adr, ed, po, ED_END);
				memcpy (texte + *po - 1, texte + *po, largeur - *po + 1);
				(*po)--;
				if (f)
					draw_object (ed, index);
				else
					objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
										 adr->ob_width, adr->ob_height);
				draw_curs (adr, ed, po, ED_END);
			}
			break;
		case DELETE :
			if (*po < strlen (get_text (adr, ed)))
			{
				draw_curs (adr, ed, po, ED_END);
				memcpy (texte + *po, texte + *po + 1, strlen (texte) - *po);
				if (f)
					draw_object (ed, index);
				else
					objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
										 adr->ob_width, adr->ob_height);
				draw_curs (adr, ed, po, ED_END);
			}
			break;
		default :
			ascii = (char)(car & 0xFF);
			if (ascii)
			{
				if ((adr[ed].ob_type >> 8) == B_EDIT)			/* Si Çditable Çtendu */
				{
					if (NOT (valide (&ascii, valid, *po)))		/* Si pas bon filtrage avec pvalid */
						return;																	/* On se barre de suite */
					pt = (ted->te_ptext);
					pt = start_edit (pt);
					if (*po == largeur)
					{			/* Si on est au bout de l'Çditable */
						if ((int)strlen (pt) < longueur)	/* Si on n'est pas au bout du champ */
						{
							draw_curs (adr, ed, po, ED_END);
							memcpy (ted->te_ptext + *po + 1, texte + *po, strlen (ted->te_ptext) - *po + 1);
							*(ted->te_ptext + (*po)) = ascii;
							(ted->te_ptext)++;
							if (f)
								draw_object (ed, index);
							else
								objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
													 adr->ob_width, adr->ob_height);
							draw_curs (adr, ed, po, ED_END);
						}
						else if ((int)strlen (pt) == longueur)	/* Si on est au bout du champ */
						{
							draw_curs (adr, ed, po, ED_END);
							*(ted->te_ptext + largeur - 1) = ascii;
							if (f)
								draw_object (ed, index);
							else
								objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
													 adr->ob_width, adr->ob_height);
							draw_curs (adr, ed, po, ED_END);
						}
					}
					else if ((int)strlen (pt) < longueur)
					{			/* Si on est au bout ni de l'Çditable, ni du champ */
						draw_curs (adr, ed, po, ED_END);
						memcpy (texte + *po + 1, texte + *po, strlen (texte) - *po + 1);
						*(texte + (*po)) = ascii;
						(*po)++;
						if (f)
							draw_object (ed, index);
						else
							objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
												 adr->ob_width, adr->ob_height);
						draw_curs (adr, ed, po, ED_END);
					}
					else
					{			/* Si on est au milieu et que le champ est plein */
						draw_curs (adr, ed, po, ED_END);
						*(ted->te_ptext + (*po)) = ascii;
						(*po)++;
						if (f)
							draw_object (ed, index);
						else
							objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
												 adr->ob_width, adr->ob_height);
						draw_curs (adr, ed, po, ED_END);
					}
				}
				else			/* Si Çditable normal */
				{
					if (*po < largeur)					/* Si on n'est pas au bout du champ */
					{
						if (ascii != '_')
						{
								/* Il faut commencer par chercher si le caractäre tapÇ n'est pas dans le template */
							ptr = pt = strchr (tmplt, (int)'_');		/* DÇbut zone de saisie du template */
							i = ZERO;
							while (i < *po)							/* Sauter les caractäres dÇjÖ saisis */
							{
								if (pt[0] == '_')
								{
									i++;
									pt++;
								}
								else
									pt++;
							}
							if (pt[0] != '_')
								pt++;
							p = pt;																	/* Noter pointeur actuel */
							pt = strchr (p, (int)ascii);						/* Chercher le caractäre */
							if ((pt) && (*(pt + 1) == '_') && (*(pt - 1) == '_'))	/* Si on le trouve */
							{
								if ((pt - ptr) < largeur)								/* Si on n'est pas au bout */
								{
									draw_curs (adr, ed, po, ED_END);			/* Virer le curseur */
      
									for ( ; *p != ascii ; p++)						/* Pour chaque caractäre */
									{
										*(texte + (*po)) = ' ';
										(*po)++;														/* Augmenter la position */
									}
									*(texte + (*po)) = '\0';							/* Terminer la chaåne */
									if (f)
										draw_object (ed, index);						/* Redessiner l'objet */
									else
										objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
															 adr->ob_width, adr->ob_height);
									draw_curs (adr, ed, po, ED_END);			/* Remettre le curseur */
									return;																/* On repart de suite */
								}
							}
						}
  
						if (NOT (valide (&ascii, valid, *po)))		/* Si pas bon filtrage avec pvalid */
							return;																	/* On se barre de suite */
						if (strlen (get_text (adr, ed)) < largeur)	/* Si le champ n'est pas plein */
						{
							draw_curs (adr, ed, po, ED_END);
							memcpy (texte + *po + 1, texte + *po, strlen (texte) - *po + 1);
							*(texte + (*po)) = ascii;
							(*po)++;
							if (f)
								draw_object (ed, index);
							else
								objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
													 adr->ob_width, adr->ob_height);
							draw_curs (adr, ed, po, ED_END);
						}
						else						/* Si le champ est plein */
						{
							draw_curs (adr, ed, po, ED_END);
							texte[*po] = ascii;
							(*po)++;
							if (f)
								draw_object (ed, index);
							else
								objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
													 adr->ob_width, adr->ob_height);
							draw_curs (adr, ed, po, ED_END);
						}
					}
					else if (*po == largeur)		/* Si curseur au bout du champ */
					{
						if (NOT (valide (&ascii, valid, *po)))		/* Si pas bon filtrage avec pvalid */
							return;																	/* On se barre de suite */
						draw_curs (adr, ed, po, ED_END);
						texte[*po - 1] = ascii;
						if (f)
							draw_object (ed, index);
						else
							objc_draw (adr, ed, MAX_DEPTH, adr->ob_x, adr->ob_y,
												 adr->ob_width, adr->ob_height);
						draw_curs (adr, ed, po, ED_END);
					}
				}
			}
			break;
		}
	}
}
/* #] objc_xedit () Gestion compläte des champs Çditables :				*/ 
/* #[ valide () Filtrage saisie champ Çditable :									*/
int valide (char *ascii, char *valid, int p)
{
int retour = FALSE, car;
int filtre;

	car = (int)*ascii;
	if (p == strlen (valid))	/* Si on est au bout du champ, */
		p--;										/* considÇrer le dernier caractäre de filtre. */
	if (p < strlen (valid))		/* Si on est dans le filtre, */
		filtre = (int)valid[p];	/* ConsidÇrer le caractäre correspondant. */
	else		/* Si on est apräs le filtre (possible avec ORCS), considÇrer le */
		filtre = (int)valid[strlen (valid) - 1];	/* dernier caractäre de filtre. */
	switch ((int) filtre)
	{
		case '9' :	/* Tous les chiffres [0-9] */
			if (isdigit (car))
				retour = TRUE;
			break;
		case 'A' :	/* Espaces et majuscules (il faut shifter) [ ]+[A-Z] */
			if ((car == ' ') || (isupper (car)))
				retour = TRUE;
			break;
		case 'a' :	/* Lettres et espaces [ ]+[A-Z]+[a-z] */
			if ((car == ' ') || (isalpha (car)) ||
					((car >= 'Å') && (car <= 'ö')))
				retour = TRUE;
			break;
		case 'N' :	/* Espaces, majuscules et chiffres [ ]+[A-Z]+[0-9] */
			if ((car == ' ') || (isupper (car)) || isdigit (car))
				retour = TRUE;
			break;
		case 'n' :	/* Espaces, lettres et chiffres [ ]+[A-Z]+[a-z]+[0-9] */
			if ((car == ' ') || (isalpha (car)) || isdigit (car) ||
					((car >= 'Å') && (car <= 'ö')))
				retour = TRUE;
			break;
		case 'F' :	/* Caractäres nom de fichier, plus '?', '*' et ':' */
			car = toupper (car);
			if ((isupper (car)) || (car == '_') || (car == '?') || (car == '*') || (car == ':') || (isdigit (car)))
				retour = TRUE;
			break;
		case 'f' :	/* Caractäres nom de fichier, plus ':' */
			car = toupper (car);
			if ((isupper (car)) || (car == '_') || (car == ':') || (isdigit (car)))
				retour = TRUE;
			break;
		case 'P' :	/* Caractäres de chemin (dont '\', ':'), plus '?' et, '*' */
			car = toupper (car);
			if ((isupper (car)) || (car == '_') || (car == '?') || (car == '*') || (car == ':') || (car == '\\') || (isdigit (car)))
				retour = TRUE;
			break;
		case 'p' :	/* Caractäres de chemin (dont '\', ':') */
			car = toupper (car);
			if ((isupper (car)) || (car == '_') || (car == ':') || (car == '\\') ||(isdigit (car)))
				retour = TRUE;
			break;
		case 'X' :	/* Tous les caractäres */
			retour = TRUE;
			break;
		/* Ajouts de BIG */
		case 'x' :	/* Tous les caractäres, mis en majuscules */
			car = toupper (car);
			retour = TRUE;
			break;
		case 'H' :	/* Tous chiffres hexa [0-9]+[a-f]+[A-F] */
			if (isxdigit (car))
				retour = TRUE;
			break;
		case 'S' :	/* Chiffre ou signe '+' ou '-' */
			if ((isdigit (car)) || (car == '+') || (car == '-'))
				retour = TRUE;
			break;
	}
	*ascii = (char)car;
	return retour;
}
/* #] valide () Filtrage saisie champ Çditable :									*/ 
/* #[ start_edit () Indique ptr sur dÇbut chaåne Çdit Çtendu :		*/
char *start_edit (char *pt)
{
	while (*(pt - 1) != '\0')
		pt--;
	return pt;
}
/* #] start_edit () Indique ptr sur dÇbut chaåne Çdit Çtendu :		*/ 
/* #[ draw_curs () Trace le curseur d'un champ Çditable :					*/
void draw_curs (OBJECT *adr, int ed, int *po, int action)
{
int hcur, pxy[4], dummy, x, y, w, h, delta, i;
char *tmplt, *ptr, *valid;
TEDINFO *ted;

	if (ed <= ZERO)	/* S'il n'y a pas d'Çditable */
		return;				/* on sort de suite */

	ted = adr[ed].ob_spec.tedinfo;		/* Adresse TEDINFO */
	tmplt = (ted->te_ptmplt);					/* Texte template */
	valid = (ted->te_pvalid);					/* Texte valid */
	if (ted->te_font == IBM)
		vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);
	else
		vst_height (work_display.handle, HMINUS, &dummy, &dummy, &w, &h);
	hcur = h + 4;
	vsl_color (work_display.handle, BLACK);
	vsl_width (work_display.handle, 1);
	vsl_ends (work_display.handle, SQUARE, SQUARE);
	vsl_type (work_display.handle, SOLID);
	vswr_mode (work_display.handle, MD_XOR);		/* TracÇ en mode Xor */

	objc_offset (adr, ed, &x, &y);	/* CoordonnÇes Çditable */
	ptr = strchr (tmplt, (int)'_');	/* 1¯ occurence de '_' */
	x += (int)((ptr - tmplt) * w);				/* Se placer lÖ */
		/* DiffÇrence de largeur entre l'objet et le template */
	delta = adr[ed].ob_width - ((int)strlen (tmplt) * w);
	switch (ted->te_just)				/* Correction selon justif */
	{
	case TE_RIGHT :			/* A droite */
		x += delta;
		break;
	case TE_CNTR :			/* CentrÇ */
		x += (delta / 2);
		break;
	}

	pxy[1] = y + ((adr[ed].ob_height - h) / 2) - 2;
	pxy[3] = pxy[1] + hcur;
	if (action == ED_INIT)
		*po = (int)strlen (ted->te_ptext);

	if (NOT ((adr[ed].ob_type >> 8) == B_EDIT))
	{				/* Balayer le template depuis le dÇbut des '_' */
		for (i = ZERO ; (i <= (*po)) && (i < strlen (valid)) && (ptr[0] != '\0') ; ptr++)
		{
			if (ptr[0] != '_')		/* Si diffÇrent de '_' */
				x += w;							/* Ajouter la largeur d'un caractäre */
			else									/* Sinon */
				i++;								/* Compter un caractäre */
		}
	}
	pxy[0] = x + (w * (*po));
	pxy[2] = pxy[0];

	v_hide_c (work_display.handle);
	v_pline (work_display.handle, 2, pxy);
	v_show_c (work_display.handle, TRUE);
}
/* #] draw_curs () Trace le curseur d'un champ Çditable :					*/ 
/* #[ copy () Copier un champ Çditable :													*/
void copy (OBJECT *adr, int ed)
{
char chaine[256];

	if (buffer)				/* S'il y a dÇjÖ un buffer */
		free (buffer);	/* Le libÇrer */
	strcpy (chaine, get_text (adr, ed));
	buffer = malloc (strlen (chaine) + 1);
	strcpy (buffer, chaine);
}
/* #] copy () Copier un champ Çditable :													*/ 
/* #[ cut () Couper un champ Çditable :														*/
void cut (OBJECT *adr, int ed)
{
char chaine[256];

	if (buffer)				/* S'il y a dÇjÖ un buffer */
		free (buffer);	/* Le libÇrer */
	strcpy (chaine, get_text (adr, ed));
	buffer = malloc (strlen (chaine) + 1);
	strcpy (buffer, chaine);
	key = ESC;
	kbd = ZERO;
}
/* #] cut () Couper un champ Çditable :														*/ 
/* #[ paste () Coller un champ Çditable :													*/
void paste (OBJECT *adr, int ed)
{
int lchamp, lchaine;
char chaine[256];

	if (NOT buffer)				/* S'il n'y a pas de buffer */
		return;							/* Sortir de suite */
	set_text (adr, ed, "");		/* Vider le champ */
	if ((adr[ed].ob_type == G_FTEXT) || (adr[ed].ob_type == G_FBOXTEXT))	/* Si Çditable normal */
		lchamp = (int)strlen (adr[ed].ob_spec.tedinfo->te_pvalid);	/* Longueur maxi du champ */
	else if ((adr[ed].ob_type >> 8) == B_EDIT)														/* Si Çditable Çtendu */
		lchamp = adr[ed].ob_state >> 8;															/* Longueur totale du champ */
	lchaine = (int)strlen (buffer);		/* Longueur du buffer */

	if (lchaine <= lchamp)					/* S'il y a la place pour le texte */
		set_text (adr, ed, buffer);			/* Copier texte */
	else														/* Si champ trop court */
	{
		strcpy (chaine, buffer);
		chaine[lchamp] = '\0';					/* Tronquer le texte */
		set_text (adr, ed, chaine);			/* Le copier */
	}
}
/* #] paste () Coller un champ Çditable :													*/ 

