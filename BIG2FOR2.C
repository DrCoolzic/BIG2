/*! @file BIG2FOR2.C
@brief implementation of functions handling forms

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

#include "big2.h"
#include "BIG2FOR2.H"
#include "BIG2USR1.H"
#include "BIG2DIAL.H"
#include "BIG2DIVR.H"
#include "BIG2EDIT.H"

/* #[ D‚finitions locales :																				*/
#define THELP "Help"
#define TUNDO "Undo"
#define TBACKSP "Backsp"
#define TBKSP "Bksp"
#define TDEL "Del"
#define TDELETE "Delete"
#define TINSERT "Insert"
#define TCLR "Clr"
#define TF1 "F1"
#define TF2 "F2"
#define TF3 "F3"
#define TF4 "F4"
#define TF5 "F5"
#define TF6 "F6"
#define TF7 "F7"
#define TF8 "F8"
#define TF9 "F9"
#define TF10 "F10"
/* #] D‚finitions locales :																				*/ 
extern Obj_user look;
/****** Fonctions de gestion des formulaires **********************/
/* #[ get_bkgr () Sauve le fond d'un formulaire :									*/
void	get_bkgr (int of_x, int of_y, int of_w, int of_h, MFDB *img)
{
int pxy[8];
size_t taille;
MFDB ecr = {0};

	of_x -= 3;
	of_y -= 3;
	of_w += 5;
	of_h += 5;

			/* Taille tampon de copie fond */
	taille = ((((size_t)(of_w / 16) + 1) * 2 * (size_t)work_display.n_plane) * (size_t)of_h) + 256;
	img->fd_addr = malloc (taille);							/* R‚server tampon */
	img->fd_w = of_w;
	img->fd_h = of_h;
	img->fd_wdwidth = (of_w / 16) + ((of_w % 16) != ZERO);
	img->fd_stand = 1;
	img->fd_nplanes = work_display.n_plane;

	fill_tab (pxy, 8,
						of_x, of_y, of_x + of_w, of_y + of_h,
						ZERO, ZERO, of_w, of_h);
	v_hide_c (work_display.handle);
	vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecr, img);
	v_show_c (work_display.handle, TRUE);
}
/* #] get_bkgr () Sauve le fond d'un formulaire :									*/ 
/* #[ put_bkgr () Restaure le fond d'un formulaire :							*/
void	put_bkgr (int of_x, int of_y, int of_w, int of_h, MFDB *img)
{
int pxy[8];
MFDB ecr = {0};

	of_x -= 3;
	of_y -= 3;
	of_w += 5;
	of_h += 5;

	fill_tab (pxy, 8,
						ZERO, ZERO, of_w, of_h,
						of_x, of_y, of_x + of_w, of_y + of_h);
	v_hide_c (work_display.handle);
	Vsync ();
	vro_cpyfm (work_display.handle, S_ONLY, pxy, img, &ecr);
	v_show_c (work_display.handle, TRUE);
	free (img->fd_addr);		/* Lib‚rer le tampon */
	img->fd_addr = NULL;
}
/* #] put_bkgr () Restaure le fond d'un formulaire :							*/ 
/* #[ scan () Tester ‚quivalents-clavier :												*/
int scan (OBJECT *adr, int f)
{
int top, index, fl_wmenu = FALSE, fl_wtool = FALSE, fl_wform = FALSE,
		retour, dummy;
unsigned char touc;

	if ((key == ARDN) || (key == ARUP) ||
			(key == ARLF) || (key == ARRT) ||
			(key == CT_ARLF) || (key == CT_ARRT) ||
			(key == CT_ARUP) || (key == CT_ARDN)||
			(key == SH_ARLF) || (key == SH_ARRT) ||
			(key == SH_ARUP) || (key == SH_ARDN) ||
			(key == ESPACE))
		return FALSE;

	stdkey (&touc);		/* Recherche code Ascii */

	if (f)													/* Si on travaille sur le bureau */
	{
		wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
		index = find_index (top);
	}
	else
		index = BLANK;

	if (index > BLANK)																	/* Si fenˆtre */
	{
		if ((win[index].w_type & 0xFF) == WTYP_FORM)		/* Fenˆtre formulaire ? */
			fl_wform = TRUE;
		if (win[index].w_type & WTYP_MENU)							/* Fenˆtre menu ? */
			fl_wmenu = TRUE;
		else if (win[index].w_type & WTYP_TOOL)					/* Fenˆtre toolbar ? */
			fl_wtool = TRUE;
	}

	if (NOT f)																	/* Si pas de fenˆtre */
		return (scan_form (touc, adr));						/* Scanner formulaire */
	else if (f)															/* Si fenˆtre */
	{
		if (fl_wmenu)														/* Si fenˆtre menu */
		{
			retour = scan_wmenu (index, touc);	/* Scanner menu fenˆtre */
			if (NOT retour)
				retour = scan_menu (touc);			/* Scanner menu */
			return retour;													/* Renvoyer r‚sultat */
		}
		if (fl_wtool)														/* Si fenˆtre toolbar */
		{
			retour = scan_wtool (index, touc);			/* Scanner toolbar fenˆtre */
			if (NOT retour)
				retour = scan_menu (touc);			/* Scanner menu */
			return retour;													/* Renvoyer r‚sultat */
		}
		else if (fl_wform)											/* Si fenˆtre formulaire */
		{
			if ((win[index].w_cont.w_form.w_edit == BLANK) || (win[index].w_icon > BLANK))
			{																							/* Si pas d'‚ditable */
				retour = scan_menu (touc);				/* Scanner menu */
				if (NOT retour)
					retour = scan_form (touc, adr);				/* Scanner formulaire */
				return retour;													/* Renvoyer r‚sultat */
			}
			else																					/* Si ‚ditable */
			{		/* Ne pas scanner le menu */
				retour = scan_form (touc, adr);					/* Scanner formulaire */
				return retour;													/* Renvoyer r‚sultat */
			}
		}
		else																		/* Si autre fenˆtre */
		{
			retour = scan_menu (touc);				/* Scanner menu */
			return retour;													/* Renvoyer r‚sultat */
		}
	}

	return FALSE;
}
/* #] scan () Tester ‚quivalents-clavier :												*/ 
/* #[ scan_wmenu () Raccourcis-clavier dans un menu de fenˆtre :	*/
int scan_wmenu (int index, char touc)
{
int tree, k, i = ZERO, j, spe, touche;
char chaine[MAX_LEN], car;
OBJECT *adr_wmenu, *adrh;

	if (win[index].w_icon > BLANK)
		return FALSE;

	kbd &= ~0x10;	/* Annuler ‚ventuel bit CapsLock */
	if (kbd & K_RSHIFT)		/* Si shift droit */
	{
		kbd &= ~K_RSHIFT;		/* Le transformer en Shift gauche */
		kbd |= K_LSHIFT;		/* puisqu'ils sont ‚gaux pour simplifier la suite */
	}
	adr_wmenu = win[index].w_bar;

	do			/* Passer tous les objets du menu */
	{
		if (((adr_wmenu[i].ob_type & 0xFF) == G_STRING) ||
				((adr_wmenu[i].ob_type & 0xFF) == G_USERDEF))
		{
			if ((adr_wmenu[i].ob_type >> 8) == B_HIERM)
			{
				tree = adr_wmenu[i].ob_state >> 8;
				adrh = ir_trindex[tree];

				k = 1;
				do
				{
					if ((adrh[k].ob_type & 0xFF) == G_STRING)
					{
						strcpy (chaine, get_text (adrh, k));
						trim (chaine);
						touche = find_touche (chaine, &j);
						if (touche == ZERO)
						{
							j = (int)strlen (chaine) - 1;
							car = chaine[j];
						}
						find_special (chaine, &spe, j);
						if ((kbd == ZERO) && (car == touc))
							return FALSE;		/* Pas de raccourcis avec une touche normale et pas de sp‚ciale */
						if ((kbd == spe) && ((car == touc) || (touche != ZERO)))
						{
							if (NOT (adrh[k].ob_state & DISABLED))
							{
								event = MU_BUTTON;
								buf[0] = BEV_WHIER;
								buf[3] = m_title (adr_wmenu, i) + 3;
								buf[4] = i;
								buf[5] = tree;
								buf[6] = k;
								buf[7] = index;
								mk = 1;
								win[index].w_bar[buf[3]].ob_state |= SELECTED;
								draw_object (buf[3], index);
								return TRUE;
							}
						}
					}
				} while (NOT (adrh[k++].ob_flags & LASTOB));
			}
			else if ((adr_wmenu[i].ob_type & 0xff) != G_USERDEF)
			{
				strcpy (chaine, get_text (adr_wmenu, i));
				trim (chaine);
				touche = find_touche (chaine, &j);
				if (touche == ZERO)
				{
					j = (int)strlen (chaine) - 1;
					car = chaine[j];
				}
				find_special (chaine, &spe, j);
				if ((kbd == ZERO) && (car == touc))
					return FALSE;		/* Pas de raccourcis avec une touche normale et pas de sp‚ciale */
				if ((kbd == spe) && ((car == touc) || (touche != ZERO)))
				{
					if (NOT (adr_wmenu[i].ob_state & DISABLED))
					{
						event = MU_BUTTON;
						buf[0] = BEV_WMENU;
						buf[3] = m_title (adr_wmenu, i) + 3;
						buf[4] = i;
						buf[5] = index;
						mk = 1;
						win[index].w_bar[buf[3]].ob_state |= SELECTED;
						draw_object (buf[3], index);
						return TRUE;
					}
				}
			}
		}
	} while (NOT (adr_wmenu[i++].ob_flags & LASTOB));

	return FALSE;
}
/* #] scan_wmenu () Raccourcis-clavier dans un menu de fenˆtre :	*/ 
/* #[ scan_wtool () Raccourcis-clavier dans une toolbar :					*/
int scan_wtool (int index, char touc)
{
int i = ZERO, j;
char chaine[MAX_LEN], c;
OBJECT *adr;

	adr = win[index].w_bar;
	kbd &= ~0x10;
	if (kbd == K_ALT)
	{
		do
		{
			if ((adr[i].ob_flags & SELECTABLE)
					&& ((adr[i].ob_type & 0xff) == G_USERDEF))
			{
				if (NOT (adr[i].ob_state & DISABLED) && NOT (adr[i].ob_state & HIDETREE))
				{
					strcpy (chaine, (char *)adr[i].ob_spec.userblk->ub_parm);
					for (j = ZERO ; j <= (int)strlen (chaine) ; j++)
					{
						if (chaine[j] == '[')
						{
							c = toupper (chaine[j + 1]);
							if (touc == c)
							{
								objc_offset (adr, i, &mx, &my);
								mx++;
								my++;
								clik = 1;
								object = i;
								event = MU_BUTTON;
								mk = 1;
								return TRUE;
							}
						}
					}
				}
			}
		} while (NOT (adr[i++].ob_flags & LASTOB));
	}
	return FALSE;
}
/* #] scan_wtool () Raccourcis-clavier dans une toolbar :					*/ 
/* #[ scan_menu () Raccourcis-clavier dans les options de menu :	*/
int scan_menu (char touc)
{
int i = ZERO, k, j, tree, spe, touche, tit;
char chaine[MAX_LEN], car;
OBJECT *adrh;

	if (NOT _app)				/* Si l'application tourne en accessoire */
		return FALSE;			/* Retourner de suite */
	if (NOT adr_menu)			/* Si pas de menu */
		return FALSE;			/* Retourner de suite */

	kbd &= ~0x10;	/* Annuler ‚ventuel bit CapsLock */
	if (kbd & K_RSHIFT)		/* Si shift droit */
	{
		kbd &= ~K_RSHIFT;		/* Le transformer en Shift gauche */
		kbd |= K_LSHIFT;		/* puisqu'ils sont ‚gaux pour simplifier la suite */
	}

	do			/* Passer tous les objets du menu */
	{
		if (((adr_menu[i].ob_type & 0xFF) == G_STRING) ||
				((adr_menu[i].ob_type & 0xFF) == G_USERDEF))
		{
			if ((adr_menu[i].ob_type >> 8) == B_HIERM)
			{
				tree = adr_menu[i].ob_state >> 8;
				adrh = ir_trindex[tree];

				k = 1;
				do
				{
					if ((adrh[k].ob_type & 0xFF) == G_STRING)
					{
						strcpy (chaine, get_text (adrh, k));
						trim (chaine);
						touche = find_touche (chaine, &j);
						if (touche == ZERO)
						{
							j = (int)strlen (chaine) - 1;
							car = chaine[j];
						}
						find_special (chaine, &spe, j);
						if ((kbd == ZERO) && (car == touc))
							return FALSE;		/* Pas de raccourcis avec une touche normale et pas de sp‚ciale */
						if ((kbd == spe) && ((car == touc) || (touche != ZERO)))
						{
							if (NOT (adrh[k].ob_state & DISABLED))
							{
								tit = m_title (adr_menu, i);
								event = MU_MESAG;
								buf[0] = BEV_HIERM;
								buf[3] = tit;
								buf[4] = i;
								buf[5] = tree;
								buf[6] = k;
								menu_tnormal (adr_menu, tit, FALSE);
								return TRUE;
							}
						}
					}
				} while (NOT (adrh[k++].ob_flags & LASTOB));
			}
			else if ((adr_menu[i].ob_type & 0xff) != G_USERDEF)
			{
				strcpy (chaine, get_text (adr_menu, i));
				trim (chaine);
				touche = find_touche (chaine, &j);
				if (touche == ZERO)
				{
					j = (int)strlen (chaine) - 1;
					car = chaine[j];
				}
				find_special (chaine, &spe, j);
				if ((kbd == ZERO) && (car == touc))
					return FALSE;		/* Pas de raccourcis avec une touche normale et pas de sp‚ciale */
				if ((kbd == spe) && ((car == touc) || (touche != ZERO)))
				{
					if (NOT (adr_menu[i].ob_state & DISABLED))
					{
						tit = m_title (adr_menu, i);
						event = MU_MESAG;
						buf[0] = MN_SELECTED;
						buf[3] = tit;
						buf[4] = i;
						menu_tnormal (adr_menu, tit, FALSE);
						return TRUE;
					}
				}
			}
		}
	} while (NOT (adr_menu[i++].ob_flags & LASTOB));
	return FALSE;
}
/* #] scan_menu () Raccourcis-clavier dans les options de menu :	*/ 
/* #[ scan_form () Raccourcis-clavier dans un formulaire :				*/
int scan_form (char touc, OBJECT *adr)
{
int i = ZERO, j;
char chaine[MAX_LEN], c;

	kbd &= ~0x10;
	if (kbd == K_ALT)
	{
		do
		{
			if ((adr[i].ob_flags & SELECTABLE)
					&& ((adr[i].ob_type & 0xff) == G_USERDEF))
			{
				if (NOT (adr[i].ob_state & DISABLED) && NOT (adr[i].ob_state & HIDETREE))
				{
					strcpy (chaine, (char *)adr[i].ob_spec.userblk->ub_parm);
					for (j = ZERO ; j <= (int)strlen (chaine) ; j++)
					{
						if (chaine[j] == '[')
						{
							c = toupper (chaine[j + 1]);
							if (touc == c)
							{
								objc_offset (adr, i, &mx, &my);
								mx++;
								my++;
								clik = 1;
								object = i;
								event = MU_BUTTON;
								mk = 1;
								return TRUE;
							}
						}
					}
				}
			}
		} while (NOT (adr[i++].ob_flags & LASTOB));
	}
	return FALSE;
}
/* #] scan_form () Raccourcis-clavier dans un formulaire :				*/ 
/* #[ find_touche () Cherche la touche press‚e :									*/
int find_touche (char *chaine, int *position)
{
int i;
char lettre, *cle;

	i = (int)strlen (chaine) - 2;	/* Le dernier caractŠre est Ascii, ‡a commence … l'avant-dernier */
	lettre = chaine[i];						/* On lit l'avant-dernier */
	if ((lettre == '^') || (lettre == '\7') ||	/* Si c'est un sp‚cial ou l'espace, */
			(lettre == '\1') || ( lettre == ' '))
		return ZERO;															/* on se tire. */
	else																				/* Sinon... */
	{
		do		/* Remonter jusqu'au d‚but du mot */
		{
			i--;
			lettre = chaine[i];
		} while ((lettre != '^') && (lettre != '\7') &&
						 (lettre != '\1') && ( lettre != ' ') &&
						 (i > ZERO));
		if (i == ZERO)											/* Si on est au d‚but du texte, */
			return ZERO;											/* on se tire. */
		i++;																/* Ajuster la position */
		*position = i;											/* Enregistrer la position de ce mot */
		cle =  (chaine + i);								/* Et le noter */
		if (stricmp (cle, THELP) == ZERO)								/* Touche Help */
		{
			if (key == HELP)
				return HELP;
		}
		else if (stricmp (cle, TUNDO) == ZERO)					/* Touche Undo */
		{
			if (key == UNDO)
				return UNDO;
		}
		else if ((stricmp (cle, TBACKSP) == ZERO) ||		/* Touche Backspace */
						 (stricmp (cle, TBKSP) == ZERO))
		{
			if (key == BACKSPC)
				return BACKSPC;
		}
		else if ((stricmp (cle, TDEL) == ZERO) ||			/* Touche Delete */
						 (stricmp (cle, TDELETE) == ZERO))
		{
			if ((key == DELETE) || (key == SH_DELETE))
				return DELETE;
		}
		else if (stricmp (cle, TINSERT) == ZERO)				/* Touche Insert */
		{
			if ((key == INSERT) || (key == SH_INSERT))
				return INSERT;
		}
		else if (stricmp (cle, TCLR) == ZERO)					/* Touche Clr */
		{
			if ((key == CLR) || (key == SH_CLR) || (key == CT_CLR))
				return CLR;
		}
		else if (stricmp (cle, TF1) == ZERO)						/* Touche F1 */
		{
			if ((key == F1) || (key == SH_F1))
				return F1;
		}
		else if (stricmp (cle, TF2) == ZERO)						/* Touche F2 */
		{
			if ((key == F2) || (key == SH_F2))
				return F2;
		}
		else if (stricmp (cle, TF3) == ZERO)						/* Touche F3 */
		{
			if ((key == F3) || (key == SH_F3))
				return F3;
		}
		else if (stricmp (cle, TF4) == ZERO)						/* Touche F4 */
		{
			if ((key == F4) || (key == SH_F4))
				return F4;
		}
		else if (stricmp (cle, TF5) == ZERO)						/* Touche F5 */
		{
			if ((key == F5) || (key == SH_F5))
				return F5;
		}
		else if (stricmp (cle, TF6) == ZERO)						/* Touche F6 */
		{
			if ((key == F6) || (key == SH_F6))
				return F6;
		}
		else if (stricmp (cle, TF7) == ZERO)						/* Touche F7 */
		{
			if ((key == F7) || (key == SH_F7))
				return F7;
		}
		else if (stricmp (cle, TF8) == ZERO)						/* Touche F8 */
		{
			if ((key == F8) || (key == SH_F8))
				return F8;
		}
		else if (stricmp (cle, TF9) == ZERO)						/* Touche F9 */
		{
			if ((key == F9) || (key == SH_F9))
				return F9;
		}
		else if (stricmp (cle, TF10) == ZERO)					/* Touche F10 */
		{
			if ((key == F10) || (key == SH_F10))
				return F10;
		}
		else							/* Si aucune correspondance, */
			return ZERO;		/* on retourne ZERO. */
	}
	return ZERO;
}
/* #] find_touche () Cherche la touche press‚e :									*/ 
/* #[ find_special () Cherche les touches sp‚ciales press‚es :		*/
void find_special (char *chaine, int *spe, int j)
{
int s;
char ctl;

	j--;
	ctl = chaine[j];
	s = ZERO;
	while (ctl != ' ')					/* tant qu'il y a un symb“le de touche */
	{
		if (ctl == '^')						/* On le teste */
			s |= K_CTRL;						/* Et on note la config */
		else if (ctl == '\7')
			s |= K_ALT;
		else if (ctl == '\1')
			s |= K_LSHIFT;
		j--;
		ctl = chaine[j];
	}
	*spe = s;										/* On copie le r‚sultat dans la variable de retour */
}
/* #] find_special () Cherche les touches sp‚ciales press‚es :		*/ 
/* #[ m_title () Cherche le titre de menu d'une option :					*/
int m_title (OBJECT *adresse, int option)
{
register menu = 1, k = 2;
int pere, titre;

	pere = parent (adresse, option);
	while (adresse[(k++) + 1].ob_type != G_BOX) ;	/* Chercher 1ø G_BOX */

	while (k != pere)
	{
		k = adresse[k].ob_next;			/* Chercher menu correspondant */
		menu++;											/* Les compter */
	}

	k = 3;
	do
	{
		titre = k++;				/* L'affecter */
	} while ((k - 3) != menu);

	return titre;
}
/* #] m_title () Cherche le titre de menu d'une option :					*/ 
/* #[ parent () Cherche le pŠre d'un objet :											*/
int cdecl parent (OBJECT *adr, int object)
{
register int i;

	i = object;											/* Partir de cet objet */
	do
	{
		i = adr[i].ob_next;						/* Passer au suivant... */
	} while (i > object);						/* Jusqu'… revenir au pŠre. */

	return i;												/* Retourner le pŠre */
}
/* #] parent () Cherche le pŠre d'un objet :											*/ 
/* #[ bak_rsc () Sauver ‚tat d'un arbre :													*/
void bak_rsc (OBJECT *tree, char **bak)
{
register int i = ZERO;
int taille = ZERO, val;
char *ptr, *pt;

		/* Calculer la taille n‚cessaire pour sauver l'‚tat du formulaire
			 EDITABLEs		: longueur champ + 1 (pour le '\0' final)
			 SELECTABLEs	: 1
			 POP_UP TEXTE	: Longueur du texte + 1 (octet nul)
			 POP_UP IMAGE	: Taille pointeur sur BitBlk
			 POP-UP BOX		: 1 octet pour le type de trame , 1 pour la couleur */
	do
	{
		if (tree[i].ob_flags & EDITABLE)
		{
			if ((tree[i].ob_type >> 8) == B_EDIT)
			{
				pt = (tree[i].ob_spec.tedinfo->te_ptext);
				pt = start_edit (pt);
				tree[i].ob_spec.tedinfo->te_ptext = pt;
				taille += (tree[i].ob_state >> 8) + 1;
			}
			else
				taille += ((int)strlen (tree[i].ob_spec.tedinfo->te_pvalid) + 1);
		}
		else if ((tree[i].ob_state & SHADOWED) &&
						 ((tree[i].ob_type & 0xFF) == G_BUTTON))
			taille += ((int)strlen (get_text (tree, i)) + 1);
		else if ((tree[i].ob_state & SHADOWED) &&
						 ((tree[i].ob_type & 0xFF) == G_IMAGE))
			taille += (int)sizeof (OBSPEC);
		else if ((tree[i].ob_state & SHADOWED) &&
						 ((tree[i].ob_type & 0xFF) == G_BOX) &&
						 ((tree[i].ob_state >> 8) != ZERO))
			taille += 2;
		else if (((tree[i].ob_type >> 8) == B_POPUP) ||
						 ((tree[i].ob_type >> 8) == B_LIST))
			taille += ((int)strlen (get_text (tree, i)) + 1);
		else if (tree[i].ob_flags & SELECTABLE)
			taille++;
	} while (NOT (tree[i++].ob_flags & LASTOB));
	taille++;

	*bak = (char *)malloc (taille);
	ptr = *bak;

	i = ZERO;
	do
	{
		if (tree[i].ob_flags & EDITABLE)
		{						/* Si ‚ditable, */
			strcpy (ptr, get_text (tree, i));			/* sauver la chaŒne. */
			ptr += (strlen (ptr) + 1);
		}
		else if ((tree[i].ob_state & SHADOWED) &&
						 ((tree[i].ob_type & 0xFF) == G_BUTTON))
		{						/* Si Pop-Up Texte */
			strcpy (ptr, get_text (tree, i));			/* sauver la chaŒne. */
			ptr += (strlen (ptr) + 1);
		}
		else if ((tree[i].ob_state & SHADOWED) &&
						 ((tree[i].ob_type & 0xFF) == G_IMAGE))
		{						/* Si Pop-Up Image */
			memcpy (ptr, &tree[i].ob_spec, sizeof (tree[i].ob_spec));
			ptr += sizeof (OBSPEC);									/* Sauver le pointeur */
		}
		else if ((tree[i].ob_state & SHADOWED) &&
						 ((tree[i].ob_type & 0xFF) == G_BOX) &&
						 ((tree[i].ob_state >> 8) != ZERO))
		{						/* Si Pop-Up Box */
			val = tree[i].ob_spec.obspec.fillpattern;
			*(ptr++) = val;
			val = tree[i].ob_spec.obspec.interiorcol;
			*(ptr++) = val;
		}
		else if (((tree[i].ob_type >> 8) == B_POPUP) ||
						 ((tree[i].ob_type >> 8) == B_LIST))
		{						/* Si Pop-Up Texte */
			strcpy (ptr, get_text (tree, i));			/* sauver la chaŒne. */
			ptr += (strlen (ptr) + 1);
		}
		else if (tree[i].ob_flags & SELECTABLE)
		{						/* Si s‚lectable, */
			if (tree[i].ob_state & SELECTED)				/* sauver l'‚tat. */
				*ptr++ = '1';
			else
				*ptr++ = '0';
		}
	} while (NOT (tree[i++].ob_flags & LASTOB));		/* Jusqu'au dernier objet */
	*ptr = '\0';	/* Pour marquer la fin */
}
/* #] bak_rsc () Sauver ‚tat d'un arbre :													*/ 
/* #[ res_rsc () Restaurer un arbre :															*/
void res_rsc (OBJECT *tree, char **bak)
{
register int i = ZERO;
int val;
char *ptr, *pt;

	ptr = *bak;		/* Pointeur au d‚but de la chaine */
	do
	{
		if (tree[i].ob_flags & EDITABLE)
		{					/* Si ‚ditable, */
			if ((tree[i].ob_type >> 8) == B_EDIT)
			{
				pt = (tree[i].ob_spec.tedinfo->te_ptext);
				pt = start_edit (pt);
				tree[i].ob_spec.tedinfo->te_ptext = pt;
			}
			set_text (tree, i, ptr);							/* restaurer la chaŒne. */
			ptr += (strlen (ptr) + 1);
		}
		else if ((tree[i].ob_state & SHADOWED) &&
						 ((tree[i].ob_type & 0xFF) == G_BUTTON))
		{					/* Si Pop-Up Texte */
			set_text (tree, i, ptr);							/* restaurer la chaŒne. */
			ptr += (strlen (ptr) + 1);
		}
		else if ((tree[i].ob_state & SHADOWED) &&
						 ((tree[i].ob_type & 0xFF) == G_IMAGE))
		{					/* Si Pop-Up Image */
			memcpy (&tree[i].ob_spec, ptr, sizeof (tree[i].ob_spec));
			ptr += sizeof (tree[i].ob_spec);				/* Restaurer le pointeur */
		}
		else if ((tree[i].ob_state & SHADOWED) &&
						 ((tree[i].ob_type & 0xFF) == G_BOX) &&
						 ((tree[i].ob_state >> 8) != ZERO))
		{					/* Si Pop-Up Box */
			val = *(ptr++);
			tree[i].ob_spec.obspec.fillpattern = val;
			val = *(ptr++);
			tree[i].ob_spec.obspec.interiorcol = val;
		}
		else if (((tree[i].ob_type >> 8) == B_POPUP) ||
						 ((tree[i].ob_type >> 8) == B_LIST))
		{					/* Si Pop-Up Texte */
			set_text (tree, i, ptr);							/* restaurer la chaŒne. */
			ptr += (strlen (ptr) + 1);
		}
		else if (tree[i].ob_flags & SELECTABLE)
		{					/* Si s‚lectable, */
			if (*ptr == '1')												/* restaurer l'‚tat. */
				tree[i].ob_state |= SELECTED;
			else if (*ptr == '0')
				tree[i].ob_state &= ~SELECTED;
			ptr++;
		}
	} while (NOT (tree[i++].ob_flags & LASTOB));		/* Jusqu'au dernier objet */
	free (bak);		/* Lib‚rer la m‚moire de sauvegarde */
}
/* #] res_rsc () Restaurer un arbre :															*/ 
/* #[ help () Routine de gestion d'une aide en ligne :						*/
void help (OBJECT *adr, int obj)
{
OBJECT *adresse;
int tree, x, y, w, h, xpere, ypere, wpere, hpere,
		sortie = FALSE, old_objet, old_edit, etat, evnt;
MFDB image;

	tree = (adr[obj].ob_state) >> 8;	/* R‚cupŠre nø formulaire associ‚ */
	adresse = ir_trindex[tree];						/* Adresse formulaire aide */

	if (tree)
	{
		w = adresse->ob_width;	/* Le centrer sur le formulaire appelant */
		h = adresse->ob_height;
		xpere = adr->ob_x;
		ypere = adr->ob_y;
		wpere = adr->ob_width;
		hpere = adr->ob_height;
		x = (w > wpere ? xpere - ((w - wpere) / 2) : xpere + ((wpere - w) / 2));
		y = (h > hpere ? ypere - ((h - hpere) / 2) : ypere + ((hpere - h) / 2));
		adresse->ob_x = x;
		adresse->ob_y = y;

		if (x + w > bureau.xd + bureau.wd - 5)		/* Le replacer s'il sort du bureau */
			x = adresse->ob_x = bureau.wd + bureau.xd - w - 5;
		if (x < bureau.xd + 5)
			x = adresse->ob_x = bureau.xd + 5;
		if (y + h > bureau.yd + bureau.hd - 5)
			y = adresse->ob_y = bureau.hd + bureau.yd - h - 5;
		if (y < bureau.yd + 5)
			y = adresse->ob_y = bureau.yd + 5;

		get_bkgr (x, y, w, h, &image);
		objc_draw (adresse, ROOT, MAX_DEPTH, x - 3, y - 3, w + 6, h + 6);

		old_objet = object;
		object = ZERO;
		old_edit = edit;
		edit = BLANK;
		wind_update (BEG_MCTRL); 	/* Bloquer le menu */
		do
		{
			evnt = dial (adresse, &image, FALSE);
			if ((evnt & MU_BUTTON) && (mk == 1))
			{
				if ((object > ZERO) && (adresse[object].ob_flags & (EXIT | DEFAULT)))
					sortie = TRUE;
			}
			else if (evnt & MU_KEYBD)
			{
				if ((key == RETURN) || (key == ENTER))
					sortie = TRUE;
			}
		} while (NOT sortie);
		wind_update (END_MCTRL); 	/* D‚bloquer le menu */
		etat = adresse[object].ob_state & 0xFF00;
		objc_change (adresse, object, ZERO,
								 adresse->ob_x, adresse->ob_y,
								 adresse->ob_width, adresse->ob_height,
								 etat, FALSE);
		object = old_objet;
		edit = old_edit;

		put_bkgr (x, y, w, h, &image);
		free (image.fd_addr);	/* Lib‚rer tampon image */
	}
}
/* #] help () Routine de gestion d'une aide en ligne :						*/ 
/* #[ move_form () D‚placement d'un formulaire sur l'‚cran :			*/
void move_form (OBJECT *adr, int x, int y, int w, int h, MFDB *img, int fl_depl)
{
int pxy[8], xm, ym, km, old_xm, old_ym, x_fin, y_fin, delta_x, delta_y, dummy;
size_t taille;
MFDB ecran = {0}, image;

		/* Taille tampon image */
	taille = (((((size_t)(w + 5) / 16) + 1) * 2 * (size_t)work_display.n_plane) * (size_t)(h + 5)) + 256;
	image.fd_addr = malloc (taille);
	image.fd_w = w + 5;
	image.fd_h = h + 5;
	image.fd_wdwidth = ((w + 5) / 16) + (((w + 5) % 16) != ZERO);
	image.fd_stand = 1;
	image.fd_nplanes = work_display.n_plane;
	fill_tab (pxy, 8,
						x - 3, y - 3,
						x + w + 2, y + h + 2,
						ZERO, ZERO, w + 5, h + 5);
	v_hide_c (work_display.handle);
	vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecran, &image);	/* Sauver le formulaire */
	graf_mouse (FLAT_HAND, ZERO);		/* Souris -> main … plat */
	v_show_c (work_display.handle, TRUE);

	if ((fl_depl) && (screen.fd_addr))
	{
		graf_mkstate (&xm, &ym, &km, &dummy);
		delta_x = xm - x;
		delta_y = ym - y;
		old_xm = xm;
		old_ym = ym;

		while (km)				/* Attendre bouton souris relach‚ */
		{
			if (xm - delta_x - 5 < bureau.xd)	/* Ne pas sortir du bureau */
				xm = bureau.xd + delta_x + 5;
			if (ym - delta_y - 5 < bureau.yd)
				ym = bureau.yd + delta_y + 5;
			if ((xm - delta_x) + w + 10 > bureau.xd + bureau.wd)
				xm = (bureau.xd + bureau.wd) - (w + 10) + delta_x + 5;
			if ((ym - delta_y) + h + 10 > bureau.yd + bureau.hd)
				ym = (bureau.yd + bureau.hd) - (h + 10) + delta_y + 5;

			if ((xm != old_xm) || (ym != old_ym))
			{
				pxy[0] = (x - 3) + (old_xm > xm ? w + 5 - (old_xm - xm) : ZERO);
				pxy[1] = y - 3;
				pxy[2] = pxy[0] + (old_xm > xm ? old_xm - xm : xm - old_xm);
				pxy[3] = pxy[1] + h + 5;
				pxy[4] = pxy[0];
				pxy[5] = pxy[1];
				pxy[6] = pxy[2];
				pxy[7] = pxy[3];
				Vsync ();
				v_hide_c (work_display.handle);
				vro_cpyfm (work_display.handle, S_ONLY, pxy, &screen, &ecran);	/* Restaurer fond */
				v_show_c (work_display.handle, TRUE);

				pxy[0] = x - 3;
				pxy[1] = (y - 3) + (old_ym > ym ? h + 5 - (old_ym - ym) : ZERO);
				pxy[2] = pxy[0] + w + 5;
				pxy[3] = pxy[1] + (old_ym > ym ? old_ym - ym : ym - old_ym);
				pxy[4] = pxy[0];
				pxy[5] = pxy[1];
				pxy[6] = pxy[2];
				pxy[7] = pxy[3];
				Vsync ();
				v_hide_c (work_display.handle);
				vro_cpyfm (work_display.handle, S_ONLY, pxy, &screen, &ecran);	/* Restaurer fond */
				v_show_c (work_display.handle, TRUE);

				x = adr->ob_x = xm - delta_x;
				y = adr->ob_y = ym - delta_y;
				old_xm = xm;
				old_ym = ym;
				fill_tab (pxy, 8,
									ZERO, ZERO, w + 5, h + 5,
									x - 3, y - 3,
									x + w + 2, y +h + 2);
				v_hide_c (work_display.handle);
				Vsync ();
				vro_cpyfm (work_display.handle, S_ONLY, pxy, &image, &ecran);	/* Restaurer image */
				v_show_c (work_display.handle, TRUE);
			}
			graf_mkstate (&xm, &ym, &km, &dummy);
		}
		free (image.fd_addr);
	}
	else
	{
		if (exist_edit (adr))
			draw_curs (adr, edit, &pos, ED_END);
		put_bkgr (x, y, w, h, img);
		graf_dragbox (w, h, x, y, bureau.xd + 5, bureau.yd + 5, bureau.wd - 10, bureau.hd - 10,
									&x_fin, &y_fin);
		x = adr->ob_x = x_fin;
		y = adr->ob_y = y_fin;
		get_bkgr (x, y, w, h, img);
		objc_draw (adr, ROOT, MAX_DEPTH, x - 3, y - 3, w + 6, h + 6);
		if (exist_edit (adr))
			draw_curs (adr, edit, &pos, ED_END);
	}

	if (image.fd_addr)
		free (image.fd_addr);
	graf_mouse (ARROW, ZERO);				/* Souris -> flˆche */
}
/* #] move_form () D‚placement d'un formulaire sur l'‚cran :			*/ 
/* #[ create_hierm () Met en place cot‚ menus hi‚rarchiques :			*/
void create_hierm (OBJECT *adr)
{
register int i = ZERO;
int tree, xoption, woption, whier, dummy;
OBJECT *adrh;
char *text;

	do
	{
		if ((adr[i].ob_type >> 8) == B_HIERM)			/* Si c'est un B_HIERM */
		{
			woption = adr[i].ob_width;							/* Largeur de l'option */
			tree = adr[i].ob_state >> 8;						/* Formulaire associ‚ */
			adrh = ir_trindex[tree];
			whier = adrh->ob_width;									/* Largeur pop-up */
			objc_offset (adr, i, &xoption, &dummy);	/* Position pop-up */
			text = get_text (adr, i);
			if (xoption + woption + whier > bureau.xd + bureau.wd - 7)	/* Pas place … droite */
				text[strlen (text) - 2] = 4;					/* FlŠche … gauche */
			else																		/* Sinon */
				text[strlen (text) - 2] = 3;					/* FlŠche … droite */
			set_text (adr, i, text);
		}
	} while (NOT (adr[i++].ob_flags & LASTOB));	/* Jusqu'au dernier objet */
}
/* #] create_hierm () Met en place cot‚ menus hi‚rarchiques :			*/ 
/* #[ big_alert () Cr‚e et gŠre une alerte comme formulaire :			*/
int big_alert (int button, int number, char *str, int fl_alrt)
{
char *alrt, str_lignes[MAX_ALRT], str_boutons[MAX_ALRT],
		 *ptr, *ligne[5], *bouton[3], icn[] = {'\0', '\0'};
register int i;
int icone, x, y, w, h, maxi = ZERO, quit = FALSE, evnt,
		nb_lignes = ZERO, nb_boutons = ZERO, dummy, fl_ecran, etat;
OBJECT *adr = bi_trindex[BIGALERTE];
MFDB img;

	if (number > BLANK)	/* MA 18/07/94 : les adresses des FREE STRINGS sont dans ir_trindex[] */
		alrt = ir_frstr[number];
	else
		alrt = str;

	if (NOT fl_alrt)	/* Si alerte GEM normale */
		return (form_alert (button, alrt));
	else
	{
		vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);	/* BoŒtes caractŠres */

		for (i = ZERO ; i < 5 ; i++)		/* Vider toutes les lignes */
			ligne[i] = '\0';
		for (i = ZERO ; i < 3 ; i++)		/* Vider tous les boutons */
			bouton[i] = '\0';

		strcpy (str_lignes, alrt + 4);
		ptr = strchr (str_lignes, ']');		/* Copier texte des lignes */
		*ptr = '\0';
		strcpy (str_boutons, ptr + 2);		/* Copier texte des boutons */
		icn[0] = alrt[1];									/* Retenir l'ic“ne */
		icone = atoi (icn);

		i = ZERO;
		ligne[i++] = strtok (str_lignes, "|]");		/* Texte 1ø ligne */
		do
			ligne[i] = strtok (ZERO, "|]");					/* Textes lignes suivantes */
		while ((i < 4) && (ligne[i++]));

		i = ZERO;
		bouton[i++] = strtok (str_boutons, "|]");	/* Texte 1ø bouton */
		do
			bouton[i] = strtok (ZERO, "|]");				/* Textes boutons suivants */
		while ((i < 2) && (bouton[i++]));

		while ((nb_lignes < 6) && (ligne[nb_lignes++])) ;		/* Compter lignes */
		nb_lignes--;
		while ((nb_boutons < 4) && (bouton[nb_boutons++])) ;	/* Compter boutons */
		nb_boutons--;

		for (i = ZERO ; i < nb_lignes ; i++)		/* Ecrire les lignes du formulaire */
			set_text (adr, AL_ST1 + i, ligne[i]);
		for (i = ZERO ; i < nb_boutons ; i++)	/* Ecrire les boutons du formulaire */
			set_text (adr, AL_BU1 + i, bouton[i]);

		if (icone)	/* Affecter l'ic“ne en fonction de la r‚solution */
		{
			adr[AL_IC].ob_spec = bi_trindex[BIGICN][icone].ob_spec;
			adr[AL_IC].ob_width = bi_trindex[BIGICN][icone].ob_width;
			adr[AL_IC].ob_height = bi_trindex[BIGICN][icone].ob_height;
			adr[AL_IC].ob_flags &= ~HIDETREE;
		}
		else
			adr[AL_IC].ob_flags |= HIDETREE;

		if (NOT icone)		/* Positionner les chaines en x */
		{
			for (i = ZERO ; i < nb_lignes ; i++)
				adr[AL_ST1 + i].ob_x = 2 * w;
		}
		else
		{
			for (i = ZERO ; i < nb_lignes ; i++)
				adr[AL_ST1 + i].ob_x = (4 * w) + adr[AL_IC].ob_width;
		}

		for (i = ZERO ; i < nb_boutons ; i++)	/* Bouton le plus large */
			maxi = max (maxi, (int)strlen (bouton[i]) * w);
		for (i = ZERO ; i < nb_boutons ; i++)	/* Tous boutons de mˆme largeur */
			adr[AL_BU1 + i].ob_width = maxi;

		maxi = (nb_boutons * (maxi + w)) + w;

		for (i = ZERO ; i < nb_lignes ; i++)
			maxi = max (maxi, adr[AL_ST1 + i].ob_x
												+ ((int)strlen (ligne[i]) * w) + (2 * w));

			/* Largeur boŒte racine */
		adr->ob_width = maxi;
			/* Position bouton d‚placement */
		adr[AL_MOVE].ob_x = maxi - adr[AL_MOVE].ob_width;

			/* Positionner les boutons en x */
		adr[AL_BU1].ob_x = (maxi - ((nb_boutons * adr[AL_BU1].ob_width)
												+ ((nb_boutons - 1) * w))) / 2;
		for (i = 1 ; i < nb_boutons ; i++)
			adr[AL_BU1 + i].ob_x = adr[AL_BU1 + i - 1].ob_x +
					adr[AL_BU1 + i - 1].ob_width + w;

			/* Positionner les boutons en y */
		maxi = (max (adr[AL_IC].ob_y + adr[AL_IC].ob_height,
						adr[AL_ST1].ob_y + (nb_lignes * adr[AL_ST1].ob_y))) +
								adr[AL_ST1].ob_y;
		for (i = ZERO ; i < nb_boutons ; i++)
			adr[AL_BU1 + i].ob_y = maxi;

			/* Hauteur boŒte racine */
		adr->ob_height = adr[AL_BU1].ob_y + (2 * adr[AL_BU1].ob_height);

		for (i = ZERO ; i < nb_lignes ; i++)	/* Montrer les lignes actives */
			adr[AL_ST1 + i].ob_flags &= ~HIDETREE;
		for (i = nb_lignes ; i < 5 ; i++)	/* Cacher les lignes inactives */
			adr[AL_ST1 + i].ob_flags |= HIDETREE;
		for (i = ZERO ; i < nb_boutons ; i++)	/* Montrer boutons actifs */
			adr[AL_BU1 + i].ob_flags &= ~(HIDETREE | DEFAULT);
		for (i = nb_boutons ; i < 3 ; i++)	/* Cacher boutons inactifs */
		{
			adr[AL_BU1 + i].ob_flags |= HIDETREE;
			adr[AL_BU1 + i].ob_flags &= ~DEFAULT;
		}
			/* Bouton par d‚faut */
		button = AL_BU1 - 1 + button;
		adr[button].ob_flags |= DEFAULT;

		wind_update (BEG_MCTRL);							/* Bloquer menu */
		form_center (adr, &dummy, &dummy, &dummy, &dummy);
		if (screen.fd_addr == NULL)
		{
			formm_draw (adr, BLANK, TRUE, &img);
			fl_ecran = TRUE;
		}
		else
		{
			formm_draw (adr, BLANK, FALSE, &img);
			fl_ecran = FALSE;
		}
		do
		{
			evnt = dial (adr, &img, fl_ecran);
			if (evnt == MU_BUTTON)
			{
				if (adr[object].ob_flags & EXIT)
					quit = TRUE;
			}
			else if (evnt == MU_KEYBD)
			{
				if ((key == ARRT) && (nb_boutons > 1))
				{
					adr[button].ob_flags &= ~DEFAULT;
					objc_offset (adr, button, &x, &y);
					objc_draw (adr, ROOT, MAX_DEPTH, x - 3, y - 3,
										 adr[button].ob_width + 6, adr[button].ob_height + 6);
					button++;
					if (button > AL_BU1 + nb_boutons - 1)
						button = AL_BU1;
					adr[button].ob_flags |= DEFAULT;
					objc_offset (adr, button, &x, &y);
					objc_draw (adr, button, 2, x - 3, y - 3,
										 adr[button].ob_width + 6, adr[button].ob_height + 6);
				}
				else if ((key == ARLF) && (nb_boutons > 1))
				{
					adr[button].ob_flags &= ~DEFAULT;
					objc_offset (adr, button, &x, &y);
					objc_draw (adr, ROOT, MAX_DEPTH, x - 3, y - 3,
										 adr[button].ob_width + 6, adr[button].ob_height + 6);
					button--;
					if (button < AL_BU1)
						button = AL_BU1 + nb_boutons - 1;
					adr[button].ob_flags |= DEFAULT;
					objc_offset (adr, button, &x, &y);
					objc_draw (adr, button, 2, x - 3, y - 3,
										 adr[button].ob_width + 6, adr[button].ob_height + 6);
				}
				else if ((key == RETURN) || (key == ENTER))
				{
					object = button;
					quit = TRUE;
				}
			}
		} while (NOT quit);

		if (fl_ecran == FALSE)
		{
			put_bkgr (adr->ob_x, adr->ob_y, adr->ob_width, adr->ob_height, &img);
			if (img.fd_addr)
				free (img.fd_addr);
			etat = adr[object].ob_state & 0xFF00;
			objc_change (adr, object, ZERO,
									 adr->ob_x, adr->ob_y, adr->ob_width, adr->ob_height,
									 etat, FALSE);
		}
		else
			formm_undraw (adr, &img);
		wind_update (END_MCTRL);							/* D‚bloquer menu */
		return (object - AL_BU1 + 1);
	}
}
/* #] big_alert () Cr‚e et gŠre une alerte comme formulaire :			*/ 
/* #[ get_string () Chercher adresse chaŒne libre :								*/
char	*get_string (int number)
{
char *str;

	str = ir_frstr[number];							/* Pointeur sur le texte */
	return str;
}
/* #] get_string () Chercher adresse chaŒne libre :								*/ 
/* #[ create_popup () Cr‚er un pop-up en m‚moire :								*/
OBJECT *create_popup (int nbre, char *liste)
{
int i, wcell, hcell, maxi = ZERO, dummy;
char *ptr;
OBJECT *adr;

	adr = (OBJECT *)malloc ((nbre + 1) * sizeof (OBJECT));	/* R‚server m‚moire du pop-up */
	if (adr)					/* Si bien r‚serv‚e, */
	{
		vst_height (work_display.handle, HNORM, &dummy, &dummy, &wcell, &hcell);
		adr->ob_next = BLANK;				/* Construire objet racine */
		adr->ob_head = 1;
		adr->ob_tail = nbre;
		adr->ob_type = G_BOX;
		if (work_display.n_plane >= 4)
			adr->ob_flags = BKGR;
		else
			adr->ob_flags = NONE;
		adr->ob_state = SHADOWED;
		adr->ob_spec.index = 0x00FF1100L;
		if (work_display.n_plane >= 4)
		{
			adr->ob_spec.obspec.fillpattern = 7;
			adr->ob_spec.obspec.interiorcol = 8;
		}
		adr->ob_x = ZERO;
		adr->ob_y = ZERO;
		adr->ob_height = nbre * hcell;
		ptr = liste;
		for (i = 1 ; i <= nbre ; i++)
		{														/* Construire chaque option */
			adr[i].ob_next = i + 1;
			adr[i].ob_head = BLANK;
			adr[i].ob_tail = BLANK;
			adr[i].ob_type = G_STRING;
			adr[i].ob_flags = TOUCHEXIT;
			adr[i].ob_state = NORMAL;
			adr[i].ob_spec.free_string = ptr;
			maxi = max (maxi, (int)strlen (ptr));	/* Chercher l'option la plus longue */
			ptr += (strlen (ptr) + 1);				/* Texte option suivante */
			adr[i].ob_x = ZERO;
			adr[i].ob_y = (i - 1) * hcell;
			adr[i].ob_height = hcell;
		}
		maxi++;					/* Un caractŠre de plus en largeur */
		adr->ob_width = maxi * wcell;			/* Largeur de la racine */
		for (i = 1 ; i <= nbre ; i++)		/* Largeur des options */
			adr[i].ob_width = maxi * wcell;
		adr[nbre].ob_next = ZERO;		/* Corriger derniŠre option */
		adr[nbre].ob_flags |= LASTOB;
	}
	return adr;									/* Retourner adresse */
}
/* #] create_popup () Cr‚er un pop-up en m‚moire :								*/ 
/* #[ form_attach () Lie un form. … un Popup, Help ou Hierm :			*/
void form_attach (int tree, int obj, char formular)
{
OBJECT *adr;
int state;

	adr = ir_trindex[tree];			/* Adresse du formulaire */
	if ((adr[obj].ob_state & SHADOWED) && NOT (adr[obj].ob_state & OUTLINED) ||	/* Si bouton Shadow (= pop_up) */
			((adr[obj].ob_type >> 8) == B_HIERM) ||	/* ou si entr‚e menu hi‚rarchique */
			((adr[obj].ob_type >> 8) == B_HELP))		/* ou si bouton d'aide */
	{
		state = adr[obj].ob_state & 0x00FF;							/* Lire son ob_state normal seul */
		adr[obj].ob_state = (state | (formular << 8));	/* Nø form. associ‚ dans l'octet haut */
	}
	if ((adr[obj].ob_type >> 8) == B_HIERM)
		create_hierm (adr);
}
/* #] form_attach () Lie un form. … un Popup, Help ou Hierm :			*/ 
/* #[ bulle_attach () Lie une bulle d'aide … un objet :						*/
void bulle_attach (int tree, int obj, char bulle)
{
OBJECT *adr, *adrpu;
int state;

	adr = ir_trindex[tree];			/* Adresse du formulaire */
	if ((NOT ((adr[obj].ob_type >> 8) == B_GRASTR)) &&
			(NOT ((adr[obj].ob_type >> 8) == B_FRAME)) &&
			(NOT ((adr[obj].ob_type >> 8) == B_HELP)) &&
			(NOT ((adr[obj].ob_type >> 8) == B_POPUP)) &&
			(NOT ((adr[obj].ob_type >> 8) == B_LIST)) &&
			(NOT ((adr[obj].ob_type >> 8) == B_PICT)) &&
			(NOT ((adr[obj].ob_type >> 8) == B_EDIT)) &&
			(NOT ((adr[obj].ob_type >> 8) == B_HIERM)) &&
			(NOT ((adr[obj].ob_type >> 8) == B_FNCP)))
	{
		state = adr[obj].ob_state & 0x00FF;					/* Lire son ob_state normal seul */
		adr[obj].ob_state = (state | (bulle << 8));	/* Nø bulle associ‚e dans l'octet haut */
		adr[obj].ob_flags |= AIDE_OBJET;
	}
	else if (((adr[obj].ob_type >> 8) == B_POPUP) ||
					 ((adr[obj].ob_type >> 8) == B_LIST))
	{
		adrpu = ir_trindex[adr[obj].ob_state >> 8];
		state = adrpu->ob_state & 0x00FF;						/* Lire son ob_state normal seul */
		adrpu->ob_state = (state | (bulle << 8));		/* Nø bulle associ‚e dans l'octet haut */
		adr[obj].ob_flags |= AIDE_OBJET;
	}
}
/* #] bulle_attach () Lie une bulle d'aide … un objet :						*/ 
/* #[ set_interface () Fixe paramŠtres des objets UserDefs :			*/
void set_interface (int under, int cunder, int ctxt_under,
										int cicn_coche, int ctxt_coche,
										int cicn_radio, int ctxt_radio,
										int cicn_popup, int ctxt_popup,
										int cmove, int cgrastr,
										int clin_frame, int ctxt_frame, int cniceline)
{
int i = ZERO;

	if (under != BLANK)
		look.under = under;
	if (cunder != BLANK)
		look.cunder = cunder;
	if (ctxt_under != BLANK)
		look.ctxt_under = ctxt_under;
	if (cicn_coche != BLANK)
		look.cicn_coche = cicn_coche;
	if (ctxt_coche != BLANK)
		look.ctxt_coche = ctxt_coche;
	if (cicn_radio != BLANK)
		look.cicn_radio = cicn_radio;
	if (ctxt_radio != BLANK)
		look.ctxt_radio = ctxt_radio;
	if (cicn_popup != BLANK)
		look.cicn_popup = cicn_popup;
	if (ctxt_popup != BLANK)
		look.ctxt_popup = ctxt_popup;
	if (cmove != BLANK)
		look.cmove = cmove;
	if (cgrastr != BLANK)
		look.cgrastr = cgrastr;
	if (clin_frame != BLANK)
		look.clin_frame = clin_frame;
	if (ctxt_frame != BLANK)
		look.ctxt_frame = ctxt_frame;
	if (cniceline != BLANK)
		look.cniceline = cniceline;

	do
	{
		if (win[i].w_hg > ZERO)
		{
			if (win[i].w_type == WTYP_FORM)
				print_page (i);
			else if (win[i].w_type & WTYP_TOOL)
				draw_object (ROOT, i);
		}
	} while (NOT (win[i++].w_flags & WFLAST));
}
/* #] set_interface () Fixe paramŠtres des objets UserDefs :			*/ 
/* #[ set_radio () S‚lectionne un radio-bouton dans un groupe :		*/
void set_radio (OBJECT *adr, int rb)
{
int i, j;

	i = parent (adr, rb);				/* Chercher le pŠre */
	j = adr[i].ob_head;					/* Partir du 1ø enfant... */
	i = adr[i].ob_tail;					/* jusqu'au dernier. */
	do
	{
		if ((adr[j].ob_flags & RBUTTON) &&	/* Tous les RBUTTONs en normal */
				(adr[j].ob_state & SELECTED))
			adr[j].ob_state &= ~SELECTED;
		j = adr[j].ob_next;												/* Passer au suivant... */
	} while ((j <= i) && (j > adr[i].ob_next));	/* jusqu'au dernier. */

	adr[rb].ob_state |= SELECTED;	/* S‚lectionner le bouton demand‚ */
}
/* #] set_radio () S‚lectionne un radio-bouton dans un groupe :		*/ 
/* #[ get_radio () retourne le nø du radio-bouton s‚lectionn‚ :		*/
int get_radio (OBJECT *adr, int rb)
{
int i, j, r = ZERO;

	i = parent (adr, rb);				/* Chercher le pŠre */
	j = adr[i].ob_head;					/* Partir du 1ø enfant... */
	i = adr[i].ob_tail;					/* jusqu'au dernier. */
	do
	{
		if (adr[j].ob_flags & RBUTTON)			/* Passer tous les RBUTTONs */
		{
			if (adr[j].ob_state & SELECTED)		/* Si s‚lectionn‚ */
				r = j;													/* Trouv‚ ! */
		}
		j = adr[j].ob_next;																	/* Passer au suivant... */
	} while ((j <= i) && (j > adr[i].ob_next) && NOT r);	/* jusqu'au dernier. */
	return r;
}
/* #] get_radio () retourne le nø du radio-bouton s‚lectionn‚ :		*/ 
/* #[ set_attributes () Fixe attr. graphiques BGRASTR & BSMSTR :	*/
void set_attributes (OBJECT *adr, int ob, int attr)
{
int xtype, state;

	xtype = adr[ob].ob_type >> 8;
	if ((xtype != B_GRASTR) && (xtype != B_SMSTR))
		return;
	state = (adr[ob].ob_state & 0xFF);
	adr[ob].ob_state = (state | (attr << 8));
}
/* #] set_attributes () Fixe attr. graphiques BGRASTR & BSMSTR :	*/ 

