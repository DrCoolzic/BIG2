/*! @file big2dial.c
@brief implementation of dialog box functions

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

#include "BIG2.H"
#include "BIG2DIAL.H"
#include "BIG2HELP.H"
#include "BIG2FOR1.H"
#include "BIG2FOR2.H"
#include "BIG2DIVR.H"
#include "BIG2EDIT.H"
#include "BIG2WINB.H"
#include "BIG2WIND.H"
#include "BIG2EVTW.H"

int topp = TRUE;
int dialog_light (int flags, OBJECT *address, int f,
									int fl_rb, int form_pu, MFDB *img, int fl_move);
/****** Functions de gestion des dialogues **********************/
/* #[ dialog () Dialogue :																				*/
int dialog (int flags, OBJECT *address, int f,
							int fl_rb, int form_pu, MFDB *img, int fl_move)
{
int retour, obj, dummy, dedans, topb, index, wh,
		fl_cla, fl_pal, fl_bdr, fl_hie;
static int topg;

	event = buf[0] = object = key = kbd = clik = BLANK;					/* R.A.Z. */
	retour = dummy = fl_cla = fl_pal = fl_bdr = fl_hie = FALSE;

	while (TRUE)
	{
		dedans = FALSE;
		event = evnt_multi (flags, 258, 3, 0,
												mouse.m_out,
												mouse.m_x, mouse.m_y, mouse.m_w, mouse.m_h,
												ZERO, ZERO, ZERO, ZERO, ZERO,
												buf, 10, 0, &mx, &my, &mk, &kbd, &key, &clik);

		if (event & MU_KEYBD)				/* Ev‚nement frappe clavier */
		{
			retour = dlg_keybd (address, f, &dedans, &fl_cla);
			if (retour)
				return retour;
		}

		if (event & MU_MESAG) 			/* Ev‚nement message (menu ou fenˆtre) */
		{
			retour = dlg_mesag ();
			if (retour)
				return retour;
		}

		if (event & MU_M1)					/* Ev‚nement de zone souris */
			dlg_mouse ();

		if (event & MU_BUTTON)			/* Ev‚nement clic souris */
		{
			if (mk == 1)
			{
				retour = dlg_button (address, f, dedans, fl_cla, img, fl_move, FALSE);
				if (retour)
					return retour;
			}
			else if (mk == 2)
			{
				if (fl_rb)	/* Clic droit et surveillance demand‚e */
				{
					if ((desc_hard_soft.multitache && topg > 1) ||
							(NOT desc_hard_soft.multitache && topg))			/* Si fenˆtre */
						index = find_index (topg);			/* A qui est-elle ? */
					else															/* Si pas fenˆtre */
						index = ZERO;										/* Comme si elle ‚tait … moi */
					if (index > BLANK)								/* Si fenˆtre … moi */
						fl_bdr = TRUE;									/* Ok */
				}
			}
		}

		if (event == MU_TIMER)
		{
			if (_app && adr_menu)			/* Traitement menus hi‚rarchiques */
			{
				obj = option_find (mx, my);
				if ((adr_menu[obj].ob_type >> 8) == B_HIERM)
					fl_hie = TRUE;
			}

			wind_get (0, WF_TOP, &topg, &dummy, &dummy, &dummy);
			topb = find_index (topg);		/* Index fenˆtre 1ø plan */
			if (topb != old_top)				/* Si fenˆtre 1ø plan chang‚e */
			{
				id_top = topb;
				if ((old_top > BLANK) && (win[old_top].w_hg > ZERO))	/* Si ancienne ouverte */
					draw_obj (win[old_top].w_fen, BW_MOVER);						/* Dessiner MOVER */
				if ((id_top > BLANK) && (win[id_top].w_hg > ZERO))		/* Si nouvelle existe */
					draw_obj (win[id_top].w_fen, BW_MOVER);							/* Dessiner MOVER */
				old_top = topb;																				/* Noter nouvelle */
			}

			/* MA 18/07/94 : gestion des bulles d'aide */
			if ((aide_en_ligne == BLANK) ||	/* Si aide toujours active ou masque de touches enfonc‚ */
					(((kbd & aide_en_ligne) == aide_en_ligne) && (aide_en_ligne > ZERO)))
			{
				if (f)
				{
					wh = wind_find (mx, my);	/* On est sur une fenˆtre ? */
					if ((wh) && (wh == topg) && menu_ok ())
					{
						index = find_index (wh);	/* Est-ce qu'elle nous appartient ? */
						if (index > BLANK)				/* Si oui, */
							inline_help (index, mx, my, topg, address);
					}
				}
				else
					inline_help (BLANK, mx, my, ZERO, address);
			}

			if (topg)			/* Traitement palette de couleurs */
			{
				index = find_index (topg);
				if ((index == BLANK) && (pal_img == TRUE))
					fl_pal = 1;
				else
				{
					if ((win[index].w_pal) && (pal_img == FALSE))
						fl_pal = 2;
					else if (NOT (win[index].w_pal) && (pal_img == TRUE))
						fl_pal = 3;
				}
			}
		}

		if (fl_hie)
		{
			if (menu_hierarch (obj))
			{
				event = MU_MESAG;
				return event;
			}
		}

		if (fl_bdr)
		{
			retour = free_popup (mx, my, form_pu, ZERO);
			if (retour)
			{
				event = MU_BUTTON;
				return event;
			}
		}

		if (fl_pal == 1)
		{
			set_palette (BLANK);
			wind_mouse ();
		}
		else if (fl_pal == 2)
			set_palette (index);
		else if (fl_pal == 3)
			set_palette (BLANK);
		return event;
	}
}
/* #] dialog () Dialogue :																				*/
/* #[ dialog_light () Dialogue all‚g‚ :														*/
int dialog_light (int flags, OBJECT *address, int f,
									int fl_rb, int form_pu, MFDB *img, int fl_move)
{
int retour, dummy, dedans, topb, fl_cla, index, fl_bdr;
static int topg;

	event = buf[0] = object = key = kbd = clik = BLANK;					/* R.A.Z. */
	retour = dummy = fl_cla = fl_bdr = FALSE;

	if (form_pu);		/* Pour ‚viter un warnings */

	while (TRUE)
	{
		dedans = FALSE;
		event = evnt_multi (flags, 258, 3, 0,
												mouse.m_out,
												mouse.m_x, mouse.m_y, mouse.m_w, mouse.m_h,
												ZERO, ZERO, ZERO, ZERO, ZERO,
												buf, 10, 0, &mx, &my, &mk, &kbd, &key, &clik);

		if (event & MU_KEYBD)				/* Ev‚nement frappe clavier */
		{
			retour = dlg_keybd (address, f, &dedans, &fl_cla);
			if (retour)
				return retour;
		}

		if (event & MU_MESAG) 			/* Ev‚nement message (menu ou fenˆtre) */
		{
			retour = dlg_mesag ();
			if (retour)
				return retour;
		}

		if (event & MU_M1)					/* Ev‚nement de zone souris */
			dlg_mouse ();

		if (event & MU_BUTTON)			/* Ev‚nement clic souris */
		{
			if (mk == 1)
			{
				retour = dlg_button (address, f, dedans, fl_cla, img, fl_move, FALSE);
				if (retour)
					return retour;
			}
			else if (mk == 2)
			{
				if (fl_rb)	/* Clic droit et surveillance demand‚e */
				{
					if ((desc_hard_soft.multitache && topg > 1) ||
							(NOT desc_hard_soft.multitache && topg))			/* Si fenˆtre */
						index = find_index (topg);			/* A qui est-elle ? */
					else															/* Si pas fenˆtre */
						index = ZERO;										/* Comme si elle ‚tait … moi */
					if (index > BLANK)								/* Si fenˆtre … moi */
						fl_bdr = TRUE;									/* Ok */
				}
			}
		}

		if (event == MU_TIMER)
		{
			wind_get (0, WF_TOP, &topg, &dummy, &dummy, &dummy);
			topb = find_index (topg);		/* Index fenˆtre 1ø plan */
			if (topb != old_top)				/* Si fenˆtre 1ø plan chang‚e */
			{
				id_top = topb;
				if ((old_top > BLANK) && (win[old_top].w_hg > ZERO))	/* Si ancienne ouverte */
					draw_obj (win[old_top].w_fen, BW_MOVER);						/* Dessiner MOVER */
				if ((id_top > BLANK) && (win[id_top].w_hg > ZERO))		/* Si nouvelle existe */
					draw_obj (win[id_top].w_fen, BW_MOVER);							/* Dessiner MOVER */
				old_top = topb;																				/* Noter nouvelle */
			}
			return event;
		}

		if (fl_bdr)
		{
			retour = free_popup (mx, my, form_pu, ZERO);
			if (retour)
			{
				event = MU_BUTTON;
				return event;
			}
		}
		return event;
	}
}
/* #] dialog_light () Dialogue all‚g‚ :														*/
/* #[ dlg_keybd () GŠre les ‚v‚nements clavier :									*/
int dlg_keybd (OBJECT *adresse, int f, int *dedans, int *fl_cla)
{
int i, champ, index, top, dummy, *ed, *po, xo, yo, hw;
OBJECT *adr = (OBJECT *)ZERO;
unsigned char touc;

	if (f == TRUE)
	{				/* Demander fenˆtre de 1ø plan */
		wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
		index = find_index (top);
	}

	if (NOT f)
	{
		adr = adresse;
		ed = &edit;
		po = &pos;
	}
	else if (index > BLANK)
	{
		if ((f) && (dedans) && (win[index].w_type == WTYP_FORM))
		{
			adr = win[index].w_cont.w_form.w_tree;
			ed = &win[index].w_cont.w_form.w_edit;
			po = &win[index].w_cont.w_form.w_pos;
		}
		else if ((f) && (dedans) && ((win[index].w_type & WTYP_TOOL) || (win[index].w_type & WTYP_MENU)))
		{
			adr = win[index].w_bar;
			ed = &edit;
			po = &pos;
		}
		else
		{
			adr = adresse;
			ed = &edit;
			po = &pos;
		}
	}
	else
	{
		adr = adresse;
		ed = &edit;
		po = &pos;
	}

	if (NOT adr)
		return FALSE;

	kbd = (int)Kbshift (BLANK);	/* Prendre ‚tat des touches sp‚ciales */
	kbd &= ~0x10;	/* Annuler bit CapsLock */
	if (*ed > BLANK)
	{
		stdkey (&touc);		/* Recherche code Ascii */
		if (kbd == K_CTRL)
		{
			if (touc == 'C')
			{
				copy (adr, *ed);
				return FALSE;
			}
			else if (touc == 'X')
			{
				cut (adr, *ed);
			}
			else if (touc == 'V')
			{
				paste (adr, *ed);
				if ((f) && (win[index].w_icon == BLANK))
				{
					draw_curs (adr, *ed, po, ED_END);		/* D‚sactiver */
					draw_object (*ed, index);
					*po = min ((int)strlen (adr[*ed].ob_spec.tedinfo->te_ptext),
										 (int)strlen (adr[*ed].ob_spec.tedinfo->te_pvalid));
					draw_curs (adr, *ed, po, ED_END);		/* R‚activer */
				}
				else if (NOT f)
				{
					draw_curs (adr, *ed, po, ED_END);		/* D‚sactiver */
					objc_offset (adr, *ed, &xo, &yo);
					objc_draw (adr, ROOT, MAX_DEPTH, xo, yo,
										 adr[*ed].ob_width, adr[*ed].ob_height);
					*po = min ((int)strlen (adr[*ed].ob_spec.tedinfo->te_ptext),
										 (int)strlen (adr[*ed].ob_spec.tedinfo->te_pvalid));
					draw_curs (adr, *ed, po, ED_END);		/* R‚activer */
				}
				return FALSE;
			}
		}
	}

	if (NOT (scan (adr, f)))
	{
		switch (key)							/* Selon la touche... */
		{
		case HELP :
			i = ZERO;
			do
			{
				if (((adr[i].ob_type >> 8) == B_HELP) && (NOT (adr[i].ob_state & DISABLED)))
				{
					objc_offset (adr, i, &mx, &my);
					mx++;
					my++;
					mk = 1;
					clik = 1;
					object = i;
					*dedans = TRUE;
					event = MU_BUTTON;
					*fl_cla = TRUE;
					return FALSE;
				}
			} while (NOT (adr[i++].ob_flags & LASTOB));
			break;
		case UNDO :
			i = ZERO;
			do
			{
				if (((adr[i].ob_type >> 8) == B_UNDO) && (NOT (adr[i].ob_state & DISABLED)))
				{
					objc_offset (adr, i, &mx, &my);
					mx++;
					my++;
					mk = 1;
					clik = 1;
					object = i;
					*dedans = TRUE;
					event = MU_BUTTON;
					*fl_cla = TRUE;
					return FALSE;
				}
			} while (NOT (adr[i++].ob_flags & LASTOB));
			break;
		case ENTER :
		case RETURN :
			i = ZERO;
			do
			{
				if ((adr[i].ob_flags & DEFAULT) && (NOT (adr[i].ob_state & DISABLED)))
				{
					objc_offset (adr, i, &mx, &my);
					mx++;
					my++;
					mk = 1;
					clik = 1;
					object = i;
					*dedans = TRUE;
					event = MU_BUTTON;
					*fl_cla = TRUE;
					return FALSE;
				}
			} while (NOT (adr[i++].ob_flags & LASTOB));
			break;
		case TAB :
			if (kbd == K_CTRL)
			{				/* Control-Tab : Cycler les fenˆtres */
				if (f && index > BLANK)
				{
					buf[3] = win[index].w_hg;
					(*win[index].w_cycle)();
				}
			}
			else if ((kbd == K_RSHIFT) || (kbd == K_LSHIFT))
			{				/* Shift-Tab : Editable pr‚c‚dent */
				if (*ed > BLANK)			/* Si dans un ‚ditable */
				{
					if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
						return FALSE;	/* Pas si fenˆtre ic“nis‚e */
					champ = prev (adr, *ed);
					if (champ > BLANK)
					{
						draw_curs (adr, *ed, po, ED_END);		/* D‚sactiver */
						*ed = champ;
						if ((adr[*ed].ob_type >> 8) == B_EDIT)
						{
							*po = min ((int)strlen (adr[*ed].ob_spec.tedinfo->te_ptext),
												 (int)strlen (adr[*ed].ob_spec.tedinfo->te_pvalid));
							draw_curs (adr, *ed, po, ED_END);		/* R‚activer */
						}
						else
						{
							draw_curs (adr, *ed, po, ED_INIT);		/* R‚activer */
						}
					}
				}
			}
			else if (kbd == ZERO)
			{				/* Tab : Editable suivant */
				if (*ed > BLANK)			/* Si dans un ‚ditable */
				{
					if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
						return FALSE;	/* Pas si fenˆtre ic“nis‚e */
					champ = next (adr, *ed);
					if (champ > BLANK)
					{
						draw_curs (adr, *ed, po, ED_END);		/* D‚sactiver */
						*ed = champ;
						if ((adr[*ed].ob_type >> 8) == B_EDIT)
						{
							*po = min ((int)strlen (adr[*ed].ob_spec.tedinfo->te_ptext),
												 (int)strlen (adr[*ed].ob_spec.tedinfo->te_pvalid));
							draw_curs (adr, *ed, po, ED_END);		/* R‚activer */
						}
						else
						{
							draw_curs (adr, *ed, po, ED_INIT);	/* R‚activer */
						}
					}
					return event;
				}
			}
			break;
		case ARDN :
			if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;	/* Pas si fenˆtre ic“nis‚e */
				champ = next (adr, *ed);
				if (champ > BLANK)
				{
					draw_curs (adr, *ed, po, ED_END);		/* D‚sactiver */
					*ed = champ;
					if ((adr[*ed].ob_type >> 8) == B_EDIT)
					{
						*po = min ((int)strlen (adr[*ed].ob_spec.tedinfo->te_ptext),
											 (int)strlen (adr[*ed].ob_spec.tedinfo->te_pvalid));
						draw_curs (adr, *ed, po, ED_END);		/* R‚activer */
					}
					else
						draw_curs (adr, *ed, po, ED_INIT);	/* R‚activer */
				}
				return event;
			}
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) && (win[index].w_icon == BLANK))
			{
				buf[3] = top;
				(*win[index].w_dnline)();
			}
			else
				return event;
			break;
		case ARUP :
			if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;	/* Pas si fenˆtre ic“nis‚e */
				champ = prev (adr, *ed);
				if (champ > BLANK)
				{
					draw_curs (adr, *ed, po, ED_END);		/* D‚sactiver */
					*ed = champ;
					if ((adr[*ed].ob_type >> 8) == B_EDIT)
					{
						*po = min ((int)strlen (adr[*ed].ob_spec.tedinfo->te_ptext),
											 (int)strlen (adr[*ed].ob_spec.tedinfo->te_pvalid));
						draw_curs (adr, *ed, po, ED_END);		/* R‚activer */
					}
					else
						draw_curs (adr, *ed, po, ED_INIT);		/* R‚activer */
				}
			}
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) && (win[index].w_icon == BLANK))
			{
				buf[3] = top;
				(*win[index].w_upline)();
			}
			else
				return event;
			break;
		case ARLF :
			if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;	/* Pas si fenˆtre ic“nis‚e */
				objc_xedit (adr, key, *ed, po, ED_CHAR, f, index);
			}
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) && (win[index].w_icon == BLANK))
			{
				buf[3] = top;
				(*win[index].w_lfline)();
			}
			else
				return event;
			break;
		case CT_ARLF :
			if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;	/* Pas si fenˆtre ic“nis‚e */
				*po = prev_word (adr, *ed, *po);
			}
			break;
		case ARRT :
			if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;	/* Pas si fenˆtre ic“nis‚e */
				objc_xedit (adr, key, *ed, po, ED_CHAR, f, index);
			}
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) && (win[index].w_icon == BLANK))
			{
				buf[3] = top;
				(*win[index].w_rtline)();
			}
			else
				return event;
			break;
		case CT_ARRT :
			if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;	/* Pas si fenˆtre ic“nis‚e */
				*po = next_word (adr, *ed, *po);
			}
			break;
		case SH_ARLF :
			if (*ed > BLANK)
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;	/* Pas si fenˆtre ic“nis‚e */
				if ((adr[*ed].ob_type >> 8) == B_EDIT)
					objc_xedit (adr, key, *ed, po, ED_CHAR, f, index);
				else
				{
					draw_curs (adr, *ed, po, ED_END);		/* D‚sactiver */
					*po = ZERO;
					draw_curs (adr, *ed, po, ED_END);		/* R‚activer */
				}
			}
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) && (win[index].w_icon == BLANK))
			{
				buf[3] = top;
				(*win[index].w_lfpage)();
			}
			else
				return event;
			break;
		case SH_ARRT :
			if (*ed > BLANK)
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;	/* Pas si fenˆtre ic“nis‚e */
				if ((adr[*ed].ob_type >> 8) == B_EDIT)
					objc_xedit (adr, key, *ed, po, ED_CHAR, f, index);
				else
				{
					draw_curs (adr, *ed, po, ED_END);		/* D‚sactiver */
					draw_curs (adr, *ed, po, ED_INIT);		/* R‚activer */
				}
			}
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) && (win[index].w_icon == BLANK))
			{
				buf[3] = top;
				(*win[index].w_rtpage)();
			}
			else
				return event;
			break;
		case SH_ARUP :
			if (*ed > BLANK)
				first (adr, ed, po);
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) && (win[index].w_icon == BLANK))
			{
				buf[3] = top;
				(*win[index].w_uppage)();
			}
			else
				return event;
			break;
		case SH_ARDN :
			if (*ed > BLANK)
				last (adr, ed, po);
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) && (win[index].w_icon == BLANK))
			{
				buf[3] = top;
				(*win[index].w_dnpage)();
			}
			else
				return event;
			break;
		case ESC :
			if ((index > BLANK) && (kbd == K_CTRL))
			{
				if (win[index].w_icon == BLANK)
					iconise (index);
				else if (win[index].w_icon > BLANK)
					uniconise (index);
			}
			else if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;	/* Pas si fenˆtre ic“nis‚e */
				objc_xedit (adr, key, *ed, po, ED_CHAR, f, index);	/* Ecrire key */
			}
			else if ((index > BLANK) && (win[index].w_flags & WFARROW))
			{
				buf[3] = top;
				if ((kbd == ZERO) && (win[index].w_icon == BLANK))
				{
					(*win[index].w_full)();
					wind_mouse ();
				}
				else if (((kbd == K_RSHIFT) || (kbd == K_LSHIFT)) && (win[index].w_icon == BLANK))
				{
					(*win[index].w_close)();
					wind_mouse ();
					event = MU_MESAG;
					buf[0] = WM_CLOSED;
					buf[3] = win[index].w_hg;
					return event;
				}
			}
			else
				return event;
			break;
		case CLR :
			if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				first (adr, ed, po);
				return event;
			}
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) &&
							 (win[index].w_icon == BLANK) && ((win[index].w_type & 0xFF) == WTYP_NORM))
			{
				if (win[index].w_lin > ZERO)			/* Si pas d‚j… au d‚but */
				{
					win[index].w_lin = ZERO;
					print_page (index);
					posi_vert (index, TRUE);
				}
			}
			break;
		case SH_CLR :
			if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				last (adr, ed, po);
				return event;
			}
			else if ((index > BLANK) && (win[index].w_flags & WFARROW) &&
							 (win[index].w_icon == BLANK) && ((win[index].w_type & 0xFF) == WTYP_NORM))
			{
				zone_work (index, &dummy, &dummy, &dummy, &hw);
				if ((win[index].w_lin + hw) < win[index].w_htot)	/* Si pas d‚j… … la fin */
				{
					win[index].w_lin = max (ZERO, win[index].w_htot + win[index].w_hunit - hw);
					win[index].w_lin = ((win[index].w_lin + (win[index].w_hunit / 2)) /
															win[index].w_hunit) * win[index].w_hunit;
					print_page (index);
					posi_vert (index, TRUE);
				}
			}
			break;
		default:											/* Sinon... */
			if (NOT touc)								/* Si aucun caractŠre Ascci r‚el */
				return event;							/* Retourner l'‚v‚nement */
			if (*ed > BLANK)			/* Si dans un ‚ditable */
			{
				if ((f) && (dedans) && (win[index].w_type == WTYP_FORM) && win[index].w_icon > BLANK)
					return FALSE;
				objc_xedit (adr, key, *ed, po, ED_CHAR, f, index);
				return event;
			}
			else if (event & MU_KEYBD)	/* Si pas ‚ditable & event inchang‚ */
				return event; 						/* Renvoyer seulement l'event */
		}
	}
	else
	{
		*fl_cla = TRUE;
		*dedans = TRUE;
	}
	return FALSE;
}
/* #] dlg_keybd () GŠre les ‚v‚nements clavier :									*/
/* #[ dlg_mesag () GŠre les ‚v‚nements message :									*/
int dlg_mesag (void)
{
int xtype, index, tree, obj, retour;
OBJECT *adrh, *adr = ZERO;
MFDB img;

	switch (buf[0])
	{
	case BEV_HIERM :
		tree = buf[5];
		adrh = ir_trindex[tree];
		xtype = adrh[buf[6]].ob_type >> 8;
		if (xtype == B_FNCP)	/* Si fonction associ‚e */
		{
			(*fnc[adrh[buf[6]].ob_state >> 8])();
			empty (MU_KEYBD);
		}
		break;
	case MN_SELECTED :
		xtype = (adr_menu[buf[4]].ob_type >> 8);
		if (xtype == B_FNCP)	/* Si fonction associ‚e */
		{
			empty (MU_KEYBD);
			(*fnc[adr_menu[buf[4]].ob_state >> 8])();
		}
		else if (xtype == B_HIERM)
			menu_tnormal (adr_menu, buf[3], TRUE);
		break;
	case WM_REDRAW :
		if (buf[4] + buf[6] > bureau.xd + bureau.wd)
			buf[6] = bureau.xd + bureau.wd - buf[4];
		if (buf[5] + buf[7] > bureau.yd + bureau.hd)
			buf[7] = bureau.yd + bureau.hd - buf[5];
		redraw ();
		break;
	case WM_CLOSED :
		index = find_index (buf[3]);
		if (index > BLANK)
		{
			(*win[index].w_close)();
			buf[3] = win[index].w_hg;
			wind_mouse ();
		}
		break;
	case WM_TOPPED :
		if (topp)
		{
			index = find_index (buf[3]);
			if (index > BLANK)
			{
				obj = objc_find (win[index].w_fen, ROOT, MAX_DEPTH, mx, my);	/* Sur quel objet ? */
				if ((NOT (kbd & K_RSHIFT)) && (NOT (kbd & K_LSHIFT)) &&	/* Si Shift */
						(obj > BLANK) &&							/* ou objet "neutre" */
						(obj != BW_BARRE) && (obj != BW_BKGR) &&
						(obj != BW_INFO) && (obj != BW_WORK) &&
						(obj != BW_VBAR) && (obj != BW_HBAR))
					no_top (obj, index);
				else
				{
					if (win[index].w_icon > BLANK)
					{
						(*win[index].w_top)();
						wind_mouse ();
					}
					else
					{
						if (win[index].w_type == WTYP_FORM)
							adr = win[index].w_cont.w_form.w_tree;
						else if (win[index].w_type & WTYP_TOOL)
							adr = win[index].w_bar;
						if ((adr > ZERO) && (adr->ob_flags & NOTOP) &&
								(NOT (kbd & K_RSHIFT)) && (NOT (kbd & K_LSHIFT)))
						{
							obj = objc_find (adr, ROOT, MAX_DEPTH, mx, my);	/* Sur quel objet ? */
							if ((obj > ZERO) &&
									(adr[obj].ob_type != G_FTEXT) && (adr[obj].ob_type != G_FBOXTEXT))
							{
								retour = dlg_button (adr, FALSE, FALSE, FALSE, &img, FALSE, TRUE);
								if (retour)
									return retour;
								else
								{
									(*win[index].w_top)();
									wind_mouse ();
								}
							}
							else
							{
								(*win[index].w_top)();
								wind_mouse ();
							}
						}
						else
						{
							(*win[index].w_top)();
							wind_mouse ();
						}
					}
				}
			}
		}
		else
		{
			event = ZERO;
			topp = TRUE;
		}
		break;
	case WM_FULLED :
		index = find_index (buf[3]);
		if (index > BLANK)
		{
			(*win[index].w_full)();
			wind_mouse ();
		}
		break;
	case WM_SIZED :
		index = find_index (buf[3]);
		if (index > BLANK)
		{
			(*win[index].w_size)();
			wind_mouse ();
		}
		break;
	case WM_MOVED :
		index = find_index (buf[3]);
		if (index > BLANK)
		{
			(*win[index].w_move)();
			wind_mouse ();
		}
		break;
	case WM_ARROWED :
		wm_arrow ();
		break;
	case WM_HSLID :
		index = find_index (buf[3]);
		(*win[index].w_hslid)();
		break;
	case WM_VSLID :
		index = find_index (buf[3]);
		(*win[index].w_vslid)();
		break;
	case WM_BOTTOM :
	case WM_CYCLED :
		index = find_index (buf[3]);
		(*win[index].w_cycle)();
		break;
	case WM_ICONIFY :
		index = find_index (buf[3]);
		iconise (index);
		break;
	case WM_UNICONIFY :
		index = find_index (buf[3]);
		uniconise (index);
		break;
	case WM_ALLICONIFY :
		index = find_index (buf[3]);
		alliconise (index);
		break;
	}
	return event;
}
/* #] dlg_mesag () GŠre les ‚v‚nements message :									*/
/* #[ dlg_button () GŠre les ‚v‚nements clic souris :							*/
int dlg_button (OBJECT *adresse, int f, int dedans, int fl_cla,
								MFDB *img, int fl_depl, int fl_notop)
{
int i, j, *ed, *po, obflags, obstate, xtype, dummy, x, y, top,
		index, wh = ZERO, w, h, xb, d, ok = TRUE, obj, km;
static int zicmu [] = {
	  0x32, 0x100, 0x2C8, 0x300, 0x400, 0x500, 0x600,
  	0x7FC, 0x810, 0x910, 0xA00, 0xB10, 0xC27, 0xD00,
	  0x101, 0x2C8, 0x301, 0x73C, 0x80C, 0x909, 0x80FA,
  	0x8100, 0xE700, 0x800, 0x900, 0xFF00
	};
OBJECT *adr, *adrh;

	km = mk;
	if ((f) && (NOT fl_cla))
		wh = wind_find (mx, my);		/* Y a-t'il une fenˆtre … cet endroit ? */

	if (wh)												/* Il y a une fenˆtre */
	{
		index = find_index (wh);		/* Index fenˆtre */
		if (index > BLANK)
		{
				/* Quel objet de fenˆtre est cliqu‚ ? */
			obj = objc_find (win[index].w_fen, ROOT, MAX_DEPTH, mx, my);
			if ((obj > BLANK) &&			/* S'il y a un objet */
					(obj != BW_BKGR) &&		/* ce n'est pas la racine */
					(obj != BW_INFO) &&		/* ni la ligne d'infos */
					(obj != BW_BARRE) &&	/* ni la barre de menu / ToolBar */
					(obj != BW_WORK) &&		/* ni la zone de travail */
					(obj != BW_VBAR) &&		/* ni la barre verticale */
					(obj != BW_HBAR))			/* ni la barre horizontale. */
			{
				no_top (obj, index);
				return MU_MESAG;					/* et sortir. */
			}
		}
	}
	else
		index = BLANK;

	if ((f) && (wh) && (NOT find_window (mx, my)))	/* Il y a une fenˆtre, mais pas zone de travail */
	{
		index = find_index (wh);		/* Index fenˆtre */
		if (index == BLANK)					/* Si pas fenˆtre du programme */
			ok = FALSE;								/* On le note */
		if (ok && ((win[index].w_type & WTYP_MENU) || (win[index].w_type & WTYP_TOOL)))
		{														/* Si fenˆtre BIG, avec menu ou toolbar */
			object = objc_find (win[index].w_bar, ROOT, MAX_DEPTH, mx, my);	/* Objet bar cliqu‚ */
			if (object == BLANK)			/* Si aucun objet (=> ‚l‚ment de bordure [Sizer]) */
				ok = FALSE;							/* On le note */
		}
		if (NOT ok)									/* Si cliqu‚ ‚l‚ment de fenˆtre */
		{
			while (km)
				graf_mkstate (&dummy, &dummy, &km, &dummy); /* Relacher bouton */
			object = BLANK;																/* Tout annuler */
			return FALSE;
		}
	}
	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	if ((f) && (top) && (wh))
	{
		index = find_index (wh);	/* Est-ce que la fenˆtre appartient au programme ? */
		if (index == BLANK)				/* Si non, */
			return FALSE;						/* sortir de suite. */
	}

	if (clik == 2)
	{
		if ((wh) && (wh == top))
		{
			index = find_index (wh);
			if (index > BLANK)
			{
				if (win[index].w_icon > BLANK)
				{
					uniconise (index);
					return FALSE;
				}
			}
		}
	}

	if (buf[0] == BEV_WMENU)				/* Si ‚v‚nement menu fenˆtre */
	{
		xtype = win[buf[5]].w_bar[buf[4]].ob_type >> 8;
		if (xtype == B_FNCP)	/* Si fonction associ‚e */
		{
			(*fnc[win[buf[5]].w_bar[buf[4]].ob_state >> 8])();
			empty (MU_KEYBD);
		}
		return event;
	}
	else if (buf[0] == BEV_WHIER)		/* Si ‚v‚nement hi‚rarchique fenˆtre */
	{
		adrh = ir_trindex[buf[5]];
		xtype = adrh[buf[6]].ob_type >> 8;
		if (xtype == B_FNCP)	/* Si fonction associ‚e */
		{
			(*fnc[adrh[buf[6]].ob_state >> 8])();
			empty (MU_KEYBD);
		}
		return event;
	}

	if (f == TRUE)
	{
		wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
		if (top)
		{
			index = find_index (top);
			if ((NOT dedans) && (index > BLANK))
				if (NOT (win[index].w_type & WTYP_TOOL))
					dedans = find_window (mx, my);
				else
				{
					if ((mx > win[index].w_bar->ob_x) &&
							(mx < win[index].w_bar->ob_x + win[index].w_bar->ob_width) &&
							(my > win[index].w_bar->ob_y) &&
							(my < win[index].w_bar->ob_y + win[index].w_bar->ob_height))
						dedans = top;
				}
		}
		else
			index = BLANK;
	}

	if (f && top && (index > BLANK))
		if (top != win[index].w_hg)
			return FALSE;

	if (NOT f)
	{
		adr = adresse;
		ed = &edit;
		po = &pos;
	}
	else if (index > BLANK)
	{
		if ((f) && (dedans) && (win[index].w_type == WTYP_FORM))
		{
			if (win[index].w_icon > BLANK)	/* Si la fenˆtre est iconifi‚e */
				return FALSE;		/* on s'en va. */

			adr = win[index].w_cont.w_form.w_tree;
			ed = &win[index].w_cont.w_form.w_edit;
			po = &win[index].w_cont.w_form.w_pos;
		}
		else if ((f) && (dedans) && (win[index].w_type & WTYP_TOOL))
		{
			if (win[index].w_icon > BLANK)	/* Si la fenˆtre est iconifi‚e */
				return FALSE;		/* on s'en va. */

			adr = win[index].w_bar;
			ed = &edit;
			po = &pos;
		}
		else
			adr = adresse;
	}
	else
		adr = adresse;

	if (fl_notop)
	{
		adr = adresse;
		f = TRUE;
		index = find_index (wind_find (mx, my));
	}

	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);
	kbd = (int)Kbshift (BLANK);		/* Prendre ‚tat des touches sp‚ciales */
	kbd &= ~0x10;									/* Annuler ‚ventuel bit CapsLock */
	if (index > BLANK)
	{
		wind = find_window (mx, my);
		if (wind != ZERO)	/* Chercher si on a cliqu‚ dans la fenˆtre active */
		{
			if (clik == 2)			/* Si double-clic, */
				event |= BIT15;		/* positionner le 15ø bit. */
		}
		else
			wind = BLANK;
	}
	else
		wind = BLANK;

	if (index > BLANK)
		if ((f) && (win[index].w_type & WTYP_MENU))
			if (menu_window (index, mx, my))
				return event;

	object = objc_find (adr, ROOT, MAX_DEPTH, mx, my); /* Objet cliqu‚ */

	if ((wind_find (mx, my)) && (wind == BLANK) && (adr == adr_desk))
	{
		while (km)
			graf_mkstate (&dummy, &dummy, &km, &dummy); /* Relacher bouton */
		return FALSE;
	}

	if ((object > BLANK) && NOT (adr[object].ob_flags & HIDETREE))
	{
		obflags = (adr[object].ob_flags);		/* Prend flags de l'objet */
		obstate = (adr[object].ob_state);							/* Prend state de l'objet */
		xtype = (adr[object].ob_type >> 8);						/* Type ‚tendu */

		if ((NOT (obflags & TOUCHEXIT)) && (adr != adr_desk) && (NOT fl_cla) && (NOT (xtype == B_EDIT)))
		{		/* Si ce n'est pas un TOUCHEXIT et qu'on n'est pas sur le bureau */
			while (km)				/* Attendre bouton relach‚ */
				graf_mkstate (&dummy, &dummy, &km, &dummy);
			vq_mouse (work_display.handle, &dummy, &mx, &my);	/* Reprendre position souris */
			if (objc_find (adr, ROOT, MAX_DEPTH, mx, my) != object)
			{										/* Si ce n'est plus le mˆme objet */
				object = BLANK;		/* Annuler object */
				return FALSE;			/* Sortir de suite */
			}
		}

		if ((((obflags & LASTOB) != LASTOB) && (obflags != NONE)) &&
				(NOT (obflags & TOUCHEXIT)) && (NOT (xtype == B_EDIT)))
		{							/* Si pas TOUCHEXIT ni B_EDIT, mais ob_flags quand mˆme, */
			while (km)	/* attendre bouton relach‚ */
				graf_mkstate (&dummy, &dummy, &km, &dummy); /* Relacher bouton */
		}

		if (xtype == B_FNCP)	/* Si fonction associ‚e */
		{
			(*fnc[adr[object].ob_state >> 8])();
			empty (MU_KEYBD);
			return event;
		}

		if ((obflags & EDITABLE) && NOT (obstate & DISABLED)) 	/* Si ‚ditable */
		{
			if (*ed != object)
			{
				draw_curs (adr, *ed, po, ED_END);
				*ed = object;
				draw_curs (adr, *ed, po, ED_END);
			}
			*po = pos_curs (adr, *ed, mx, f, index, *po);
		}

		if ((obflags & RBUTTON) && (obflags & SELECTABLE) &&
				NOT (obstate & DISABLED) &&
				NOT (obstate & SELECTED)) 	/* Si Radio, SELECTABLE et pas SELECTED */
		{
			j = object;											/* Partir de cet objet */
			adr[object].ob_state |= SELECTED;	/* Le s‚lectionner */
			if (f)
				draw_object (object, index);
			else
				objc_draw (adr, object, MAX_DEPTH, adr->ob_x, adr->ob_y,
									 adr->ob_width, adr->ob_height);
			i = parent (adr, j);					/* Chercher le pŠre */
			j = adr[i].ob_head;					/* Partir du 1ø enfant... */
			i = adr[i].ob_tail;					/* jusqu'au dernier. */
			do
			{
				if ((adr[j].ob_flags & RBUTTON) && (j != object) &&
						(adr[j].ob_state & SELECTED))
				{		/* Les mettre en normal si RBUTTON sauf l'objet cliqu‚. */
					adr[j].ob_state &= ~SELECTED;
					if (f)
						draw_object (j, index);
					else
						objc_draw (adr, j, MAX_DEPTH, adr->ob_x, adr->ob_y,
											 adr->ob_width, adr->ob_height);
				}
				j = adr[j].ob_next;												/* Passer au suivant... */
			} while ((j <= i) && (j > adr[i].ob_next));	/* jusqu'au dernier. */
			if ((f) && (win[index].w_type & WTYP_TOOL))
			{
				event = MU_BUTTON;
				buf[0] = BEV_TOOL;
				buf[3] = index;
				buf[4] = object;
			}
			if ((f) && (win[index].w_type & WTYP_FORM))
			{
				event = MU_BUTTON;
				buf[0] = BEV_WFORM;
				buf[3] = index;
				buf[4] = object;
			}
			if (NOT (obflags & DCDEFAULT))
				return event;
		}

		if ((obflags & SELECTABLE) && (NOT (obflags & RBUTTON))
				&& NOT (obstate & DISABLED))					/* Si Selectable seul */
		{
			adr[object].ob_state ^= SELECTED;
			if ((f) && (top) && (adr != adr_desk))
			{
				draw_object (object, index);
				if ((win[index].w_type & WTYP_TOOL) &&
						(NOT (adr[object].ob_state & SHADOWED)))
				{
					event = MU_BUTTON;
					buf[0] = BEV_TOOL;
					buf[3] = index;
					buf[4] = object;
					return event;
				}
				if ((win[index].w_type & WTYP_FORM) &&
						(NOT (adr[object].ob_state & SHADOWED)))
				{
					event = MU_BUTTON;
					buf[0] = BEV_WFORM;
					buf[3] = index;
					buf[4] = object;
				}
			}
			else if (adr == adr_desk)
				form_dial (FMD_FINISH, 0, 0, 0, 0,
									 adr_desk[object].ob_x + bureau.xd, adr_desk[object].ob_y + bureau.yd,
									 adr_desk[object].ob_width, adr_desk[object].ob_height);
			else
				objc_draw (adr, object, MAX_DEPTH, adr->ob_x, adr->ob_y,
									 adr->ob_width, adr->ob_height);

			if ((adr[object].ob_type >> 8) == B_HELP)	/* Si bouton Aide */
			{
				help (adr, object);		/* Appel routine Aide */
				adr[object].ob_state ^= SELECTED;
				if (f)
					draw_object (object, index);
				else
					objc_draw (adr, object, MAX_DEPTH, adr->ob_x, adr->ob_y,
										 adr->ob_width, adr->ob_height);
			}
		}
		else if ((obflags & SELECTABLE) && (NOT (obflags & RBUTTON)) &&
						 (obstate & DISABLED))					/* Si Selectable DISABLED */
		{
			event = ZERO;
			return event;
		}

		if ((obflags & SELECTABLE) && (obflags & DCDEFAULT) &&
				(clik == 2) && NOT (obstate & DISABLED))	/* Si double-clic entraine Default */
		{
			i = ZERO;	/* En partant de la racine */
			do				/* Pour chaque objet */
			{
				if (adr[i].ob_flags & DEFAULT)	/* Si objet d‚faut */
				{
					adr[i].ob_state |= SELECTED;	/* S‚lectionner l'objet */
					objc_draw (adr, i, MAX_DEPTH, /* Le redessiner */
										 adr->ob_x, adr->ob_y,
										 adr->ob_width, adr->ob_height);
					object = i;					/* Enregistrer l'objet */
					event = MU_BUTTON;	/* Modifier type d'‚v‚nement */
					buf[0] = BEV_WFORM;
					buf[3] = index;
					buf[4] = object;
					mx = my = mk = clik = ZERO;
					return (event);			/* Retourner l'‚v‚nement */
				}
			} while (NOT (adr[i++].ob_flags & LASTOB)); /* Jusqu'au dernier objet */
		}

		if ((obstate & SHADOWED) && NOT (obstate & OUTLINED)
				&& NOT (obstate & DISABLED))			/* Si bouton Shadow (= pop_up) */
		{
			if (kbd == ZERO)																	/* Si clic simple */
			{
				if ((NOT xtype) || (xtype == B_POPUP) || (xtype == B_LIST))
				{
					objc_offset (adr, object, &xb, &dummy);
					if (xtype)
					{
						if (h > 8)
							d = h;
						else
							d = w * 2;
					}
					else
						d = ZERO;
					if (mx > (xb + adr[object].ob_width - d))	/* Si clic ic“ne suivant */
						next_option (adr, object, BLANK);	/* Option suivante */
					else																			/* Sinon */
					{
						if ((NOT xtype) || (xtype == B_POPUP))
							pop_up (adr, object);							/* Routine Pop_Up */
						else if (xtype == B_LIST)
							pop_liste (adr, object);	/* Appel routine Pop-Liste */
					}
				}
			}
			else if ((kbd == K_RSHIFT) || (kbd == K_LSHIFT))	/* Si shift-clic */
				next_option (adr, object, BLANK);				/* option suivante */

			if (adr[object].ob_type != G_BOX)
				adr[object].ob_state ^= SELECTED;		/* Inverser SELECTED */
			if (f)
			{
				draw_object (object, index);
				if (win[index].w_type & WTYP_TOOL)
				{
					event = MU_BUTTON;
					buf[0] = BEV_TOOL;
					buf[3] = index;
					buf[4] = object;
				}
				if (win[index].w_type & WTYP_FORM)
				{
					event = MU_BUTTON;
					buf[0] = BEV_WFORM;
					buf[3] = index;
					buf[4] = object;
				}
				return event;
			}
			else
			{
				objc_offset (adr, object, &x, &y);	/* Position objet cliqu‚ */
				objc_draw (adr, ROOT, MAX_DEPTH, x, y,
									 adr[object].ob_width, adr[object].ob_height + 3);
			}
		}

		if ((obflags & EXIT) && (NOT (obstate & DISABLED))) /* Si bouton EXIT */
		{
			if (f)
			{
				buf[0] = BEV_WFORM;
				if (obflags & WFCLOSE)
				{
					buf[3] = win[index].w_hg;
					(*win[index].w_close)();
				}
				buf[1] = ap_id;
				buf[3] = index;
				buf[4] = object;
			}
			return event;
		}
		else if ((obflags & EXIT) && (obstate & DISABLED)) /* Si EXIT DISABLED */
		{
			event = ZERO;
			return event;
		}

		if ((obflags & TOUCHEXIT) && NOT (obstate & DISABLED))	/* Si TOUCHEXIT */
		{
			if (xtype == B_MOVE)
			{							/* Si on clique le bouton de d‚placement */
				move_form (adr, adr->ob_x, adr->ob_y, adr->ob_width, adr->ob_height, img, fl_depl);
			}
			else
			{
				if (f && (win[index].w_type & WTYP_TOOL))
				{
					event = MU_BUTTON;
					buf[0] = BEV_TOOL;
					buf[3] = index;
					buf[4] = object;
				}
				else if (f && (win[index].w_type & WTYP_FORM))
				{
					event = MU_BUTTON;
					buf[0] = BEV_WFORM;
					buf[3] = index;
					buf[4] = object;
				}
				if (clik == 2)			/* Si double-clic, */
					event |= BIT15;		/* positionner le 15ø bit. */
				return event;
			}
		}
		else if ((obflags & TOUCHEXIT) && (obstate & DISABLED))	/* Si TOUCHEXIT DISABLED */
		{
			event = ZERO;
			return event;
		}
	}
	else if ((NOT f) && (object == BLANK))
	{
		while (km)
			graf_mkstate (&dummy, &dummy, &km, &dummy); /* Relacher bouton */
		Dosound (zicmu);
	}
	return event;		/* Si c'est un objet quelconque */
}
/* #] dlg_button () GŠre les ‚v‚nements clic souris :							*/
/* #[ dlg_mouse () GŠre les ‚v‚nements de zone souris :						*/
void dlg_mouse (void)
{
int top, index, dummy;

	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	index = find_index (top);
	if (index > BLANK)
	{
		if (mouse.m_out == TRUE)							/* Si on sort de la zone */
			graf_mouse (ARROW, 0);							/* Souris -> Flˆche */
		else if (mouse.m_out == FALSE)				/* Si on entre dans la zone */
			graf_mouse (win[index].w_mouse, 0);	/* Souris -> forme enregistr‚e */
		mouse.m_out ^= 0x01;			/* Inverser ‚tat surveillance */
	}
}
/* #] dlg_mouse () GŠre les ‚v‚nements de zone souris :						*/
/* #[ option_find () Cherche sur quelle option de menu on est :		*/
int option_find (int xm, int ym)
{
int i = 3, x, y, w, h, retour = ZERO;

	do
	{
		if ((adr_menu[i].ob_type >> 8) == B_HIERM)
		{
			objc_offset (adr_menu, i, &x, &y);
			w = adr_menu[i].ob_width;
			h = adr_menu[i].ob_height;
			if ((xm >= x) && (xm < x + w) &&
					(ym >= y) && (ym < y + h))
			{
				if (adr_menu[i].ob_state & SELECTED)
					retour = i;
			}
		}
	} while ((retour == ZERO) && (NOT (adr_menu[i++].ob_flags & LASTOB)));
	return retour;
}
/* #] option_find () Cherche sur quelle option de menu on est :		*/
/* #[ menu_ok () Signale si aucun menu n'est d‚roul‚ :						*/
int menu_ok (void)
{
int o = 2, retour = TRUE;

	if (adr_menu)		/* S'il y a un menu */
	{
		do
		{		/* Examiner chaque titre */
			if (adr_menu[o].ob_state & SELECTED)	/* S'il est s‚lectionn‚ (d‚roul‚) */
				retour = FALSE;											/* C'est pas bon */
		} while (adr_menu[++o].ob_type == G_TITLE);	/* Titre suivant */
	}
	return retour;		/* Retourner le r‚sultat */
}
/* #] menu_ok () Signale si aucun menu n'est d‚roul‚ :						*/
/* #[ dial () Mini_dialog :																				*/
int dial (OBJECT *address, MFDB *img, int fl_move)
{
int retour, dedans, fl_cla;

	event = buf[0] = object = key = kbd = clik = BLANK;					/* R.A.Z. */
	retour = fl_cla = FALSE;

	while (TRUE)
	{
		dedans = FALSE;
		event = evnt_multi ((MU_KEYBD|MU_BUTTON), 2, 1, 1,
												ZERO, ZERO, ZERO, ZERO, ZERO,
												ZERO, ZERO, ZERO, ZERO, ZERO,
												buf, 10, 0, &mx, &my, &mk, &kbd, &key, &clik);
		if (event & MU_KEYBD)				/* Ev‚nement frappe clavier */
		{
			retour = dlg_keybd (address, FALSE, &dedans, &fl_cla);
			return MU_KEYBD;
		}
		if (event & MU_BUTTON)			/* Ev‚nement clic souris */
		{
			retour = dlg_button (address, FALSE, dedans, fl_cla, img, fl_move, FALSE);
			if (retour)
				return MU_BUTTON;
		}
	}
}
/* #] dial () Mini_dialog :																				*/

