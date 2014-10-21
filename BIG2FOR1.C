/*! @file BIG2FOR1.C
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

/****** En-tˆte BIG ***********************************************/
#include "big2.h"
#include "BIG2DIAL.H"
#include "BIG2FOR1.H"
#include "BIG2FOR2.H"
#include "BIG2EDIT.H"
#include "BIG2WIND.H"
/****** Fonctions de gestion des formulaires **********************/

/* #[ formm_draw () Dessine un formulaire d‚pla‡able :						*/

void formm_draw (OBJECT *adr, int ed, int flmove, MFDB *img)
{
int x, y, w, h, pxy[8];
size_t taille;
MFDB nul = {0};

	graf_mouse (ARROW, 0);							/* Souris -> Flˆche */
	if (flmove == TRUE)
	{
		if (screen.fd_addr)
		{
			free (screen.fd_addr);		/* Lib‚rer le tampon par s‚curit‚ */
			screen.fd_addr = NULL;
		}
			/* Taille ‚cran */
		taille = ((size_t)(bureau.xd + bureau.wd) / 8 * (size_t)work_display.n_plane) * (size_t)(bureau.yd + bureau.hd);
		screen.fd_addr = malloc (taille);										/* R‚server tampon */
		if (screen.fd_addr)
		{
			screen.fd_w = bureau.xd + bureau.wd;
			screen.fd_h = bureau.yd + bureau.hd;
			screen.fd_wdwidth = (bureau.xd + bureau.wd) / 16;
			screen.fd_stand = 1;
			screen.fd_nplanes = work_display.n_plane;

			fill_tab (pxy, 8,
								ZERO, ZERO,
								bureau.xd + bureau.wd - 1,
								bureau.yd + bureau.hd - 1,
								ZERO, ZERO,
								bureau.xd + bureau.wd - 1,
								bureau.yd + bureau.hd - 1);

			v_hide_c (work_display.handle);
			vro_cpyfm (work_display.handle, S_ONLY, pxy, &nul, &screen);	/* Copier l'‚cran */
			v_show_c (work_display.handle, TRUE);
		}
	}

	x = adr->ob_x;
	y = adr->ob_y;
	w = adr->ob_width;
	h = adr->ob_height;
	get_bkgr (x, y, w, h, img);					/* Sauver le fond */
	objc_draw (adr, ROOT, MAX_DEPTH, x - 3, y - 3, w + 6, h + 6);
	if (ed > BLANK)
	{
		edit = ed;
		if ((adr[ed].ob_type >> 8) == B_EDIT)
		{
			pos = min ((int)strlen (adr[edit].ob_spec.tedinfo->te_ptext),
								 (int)strlen (adr[edit].ob_spec.tedinfo->te_pvalid));
			draw_curs (adr, edit, &pos, ED_END);		/* R‚activer */
		}
		else
		{
			pos = (int)strlen (get_text (adr, edit));
			draw_curs (adr, edit, &pos, ED_INIT);	/* Activer curseur */
		}
	}
}
/* #] formm_draw () Dessine un formulaire d‚pla‡able :						*/ 

/* #[ formm_undraw () LibŠre un formulaire d‚pla‡able :						*/
void formm_undraw (OBJECT *adr, MFDB *img)
{
int pxy[8], x, y, w, h;
MFDB ecran = {ZERO};

	edit = BLANK;
	if (screen.fd_addr)
	{		/* Si possible, restaurer le fond par copie raster */
		x = adr->ob_x;
		y = adr->ob_y;
		w = adr->ob_width;
		h = adr->ob_height;
		fill_tab (pxy, 8,
							x - 3, y - 3,
							x + w + 2, y + h + 2,
							x - 3, y - 3,
							x + w + 2, y + h + 2);
		Vsync ();
		v_hide_c (work_display.handle);
		vro_cpyfm (work_display.handle, S_ONLY, pxy, &screen, &ecran);	/* Restaurer fond */
		v_show_c (work_display.handle, TRUE);
		free (screen.fd_addr);
		screen.fd_addr = NULL;
	}
	else	/* Sinon, faire un form_dial */
		form_dial (FMD_FINISH, 0, 0, 0, 0,
							 adr->ob_x - 3, adr->ob_y - 3,
							 adr->ob_width + 6, adr->ob_height + 6);
	if (img->fd_addr)
		free (img->fd_addr);
	adr[object].ob_state &= ~SELECTED;
	wind_mouse ();
}
/* #] formm_undraw () LibŠre un formulaire d‚pla‡able :						*/ 

/* #[ formf_draw () Dessine un formulaire fixe :									*/

void formf_draw (OBJECT *adr, int ed)
{
	graf_mouse (ARROW, 0);							/* Souris -> Flˆche */
	objc_draw (adr, ROOT, MAX_DEPTH, adr->ob_x - 3, adr->ob_y - 3,
						 adr->ob_width + 6, adr->ob_height + 6);
	if (ed > BLANK)
	{
		edit = ed;
		if ((adr[ed].ob_type >> 8) == B_EDIT)
		{
			pos = min ((int)strlen (adr[edit].ob_spec.tedinfo->te_ptext),
								 (int)strlen (adr[edit].ob_spec.tedinfo->te_pvalid));
			draw_curs (adr, edit, &pos, ED_END);		/* R‚activer */
		}
		else
		{
			pos = (int)strlen (get_text (adr, edit));
			draw_curs (adr, edit, &pos, ED_INIT);	/* Activer curseur */
		}
	}
}
/* #] formf_draw () Dessine un formulaire fixe :									*/ 
/* #[ formf_undraw () LibŠre un formulaire fixe :									*/
void formf_undraw (OBJECT *adr)
{
	edit = BLANK;
	form_dial (FMD_FINISH, 0, 0, 0, 0,
						 adr->ob_x - 3, adr->ob_y - 3,
						 adr->ob_width + 6, adr->ob_height + 6);
	adr[object].ob_state &= ~SELECTED;
	wind_mouse ();
}
/* #] formf_undraw () LibŠre un formulaire fixe :									*/ 
/* #[ menu_hierarch () GŠre un menu hi‚rarchique :								*/
int menu_hierarch (int option)
{
OBJECT *adrh, *old_adrh;
int x, y, w, h, xo, yo, xm, ym, km, b[8], evnt, ob_menu, menu, pxy[8],
		tree, dummy, ob = ZERO, old_ob = BLANK, sortie = FALSE, titre,
		xz, yz, etat, cote, nbre, fl_scroll = FALSE, xs, ys, cale = 1,
		naff, down, haut, i;
size_t taille;
char *text;
MOBLK zone;
MFDB ecran = {0}, image;

	evnt = evnt_timer (100, 0);			/* D‚lai 1/2 seconde */
	graf_mkstate (&xm, &ym, &dummy, &dummy);
	if (option_find (xm, ym) != option)
		return FALSE;			/* Si plus sur l'option, on sort */

	objc_offset (adr_menu, option, &xo, &yo);	/* Position option */

	tree = adr_menu[option].ob_state >> 8;
	adrh = ir_trindex[tree];

	menu = parent (adr_menu, option);
	objc_offset (adr_menu, menu, &xz, &yz);

	titre = m_title (adr_menu, option);

		/* 0 : Entr‚e ; 1 : Sortie */
	zone.m_out = 1;
	zone.m_x = xz;
	zone.m_y = yz;
	zone.m_w = adr_menu[menu].ob_width;
	zone.m_h = adr_menu[menu].ob_height;

	text = get_text (adr_menu, option);
	if (text[strlen (text) - 2] == 3)
		x = adrh->ob_x = xo + adr_menu[option].ob_width + 2;
	else if (text[strlen (text) - 2] == 4)
		x = adrh->ob_x = xo - adrh->ob_width - 5;
	y = adrh->ob_y = yo;
	w = adrh->ob_width;
	h = adrh->ob_height;

	if ((y + h) > bureau.hd)													/* Si plus haut que le bureau */
	{
		naff = create_scroll (adrh, y, &nbre, ZERO);		/* cr‚er un scroll-menu */
		down = naff + 2;
		old_adrh = adrh;
		adrh = scroll;
		adrh->ob_x = x;
		adrh->ob_y = y;
		haut = adrh[1].ob_height;
		fl_scroll = TRUE;
	}

	taille = (((((size_t)(w + 3) / 16) + 1) * 2 * (size_t)work_display.n_plane) *
						(size_t)(h + 3)) + 256;
	image.fd_addr = malloc (taille);
	image.fd_w = w + 3;
	image.fd_h = h + 3;
	image.fd_wdwidth = ((w + 3) / 16) + (((w + 3) % 16) != 0);
	image.fd_stand = 1;
	image.fd_nplanes = work_display.n_plane;
	fill_tab (pxy, 8,
						x - 1, y - 1,
						x + w + 3, y + h + 3,
						ZERO, ZERO, w + 4, h + 4);
	v_hide_c (work_display.handle);
	vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecran, &image);	/* Sauver le fond */
	v_show_c (work_display.handle, TRUE);
	objc_draw (adrh, ROOT, MAX_DEPTH, x - 1, y - 1, w + 4, h + 4);

	do
	{
		evnt = evnt_multi (MU_BUTTON | MU_M1 | MU_TIMER,
											 1, 1, 1,
											 zone.m_out, zone.m_x, zone.m_y,
											 zone.m_w, zone.m_h,
											 0, 0, 0, 0, 0,
											 b, 10, 0, &xm, &ym, &km, &dummy, &dummy, &dummy);
		if (evnt & MU_BUTTON)
		{
			sortie = TRUE;
			etat = adr_menu[option].ob_state & 0xFF00;
			objc_change (adr_menu, option, 0, xo, yo, adr_menu[option].ob_width,
									 adr_menu[option].ob_height, etat, FALSE);
			menu_tnormal (adr_menu, titre, 1);
		}
		else if (evnt & MU_M1)
		{
			graf_mkstate (&xm, &ym, &dummy, &dummy);
			if (zone.m_out)						/* Si sortie de la zone */
			{
				zone.m_out = 0;					/* Surveiller l'entr‚e */
				if (text[strlen (text) - 2] == 3)				/* Si cot‚ droit */
					cote = (xm < zone.m_x + zone.m_w);
				else if (text[strlen (text) - 2] == 4)	/* Si cot‚ gauche */
					cote = (xm > zone.m_x + zone.m_y);
				if (cote)						/* Si pas sortie cot‚ pop-up */
					sortie = TRUE;		/* Fini */
				else
					adr_menu[option].ob_state |= SELECTED;
			}
			else											/* Si entr‚e dans la zone */
			{
				ob_menu = objc_find (adr_menu, ROOT, MAX_DEPTH, xm, ym);
				if (ob_menu != option)	/* Si autre option */
				{
					etat = adr_menu[option].ob_state & ~SELECTED;
					objc_change (adr_menu, option, 0, xo, yo,
											 adr_menu[option].ob_width,
											 adr_menu[option].ob_height, etat, TRUE);
					ob = BLANK;
					sortie = TRUE;				/* Fini */
				}
				else										/* Si mˆme option */
				{
					zone.m_out = 1;				/* Surveiller sortie */
				}
			}
		}
		else if (evnt == MU_TIMER)
		{
			if ((NOT (adr_menu[titre].ob_state & SELECTED)) ||
					(NOT (adr_menu[option].ob_state & SELECTED)))
			{
				etat = adr_menu[option].ob_state & 0xFF00;
				objc_change (adr_menu, option, 0, xo, yo, adr_menu[option].ob_width,
										 adr_menu[option].ob_height, etat, FALSE);
				ob = BLANK;
				sortie = TRUE;
			}

			graf_mkstate (&xm, &ym, &dummy, &dummy);
			ob = objc_find (adrh, ROOT, MAX_DEPTH, xm, ym);

			if (old_ob != ob)		/* Si objet diff‚rent */
			{
				if ((old_ob != -1) && (NOT (adrh[old_ob].ob_state & DISABLED)))
				{
					etat = adrh[old_ob].ob_state & ~SELECTED;
					objc_change (adrh, old_ob, 0, x, y, w, h, etat, TRUE);
				}
				if ((ob > ROOT) && (NOT (adrh[ob].ob_state & DISABLED)))
				{
					etat = adrh[ob].ob_state | SELECTED;
					objc_change (adrh, ob, 0, x, y, w, h, etat, TRUE);
				}
				old_ob = ob;
			}
			else								/* Si mˆme objet */
			{
				if ((fl_scroll) && (ob == 1))
				{		/* Si scroll : flˆche vers le haut */
					objc_offset (adrh, 0, &xs, &ys);
					if (cale > 1)
					{
						evnt_timer (50, 0);
						cale--;
						for (i = naff + 1 ; i > 2 ; i--)
						{
							adrh[i].ob_spec = adrh[i - 1].ob_spec;
							adrh[i].ob_state = adrh[i - 1].ob_state;
						}
						adrh[2].ob_spec = old_adrh[cale].ob_spec;
						adrh[2].ob_state = old_adrh[cale].ob_state;
						fill_tab (pxy, 8,
											xs, ys + haut,
											xs + adrh->ob_width, ys + (naff * haut),
											xs, ys + (2 * haut),
											xs + adrh->ob_width, ys + ((naff + 1) * haut));
						v_hide_c (work_display.handle);
						objc_change (adrh, ob, 0, x, y, w, h, NORMAL, TRUE);
						vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecran, &ecran);
						objc_change (adrh, ob, 0, x, y, w, h, SELECTED, TRUE);
						v_show_c (work_display.handle, TRUE);
						objc_draw (adrh, ROOT, 2,
											 x + adrh[2].ob_x, y + adrh[2].ob_y,
											 adrh[2].ob_width, adrh[2].ob_height);
					}
				}
				else if ((fl_scroll) && (ob == down))
				{		/* Si scroll : flˆche vers le bas */
					objc_offset (adrh, 0, &xs, &ys);
					if ((cale + naff) <= nbre)
					{
						evnt_timer (50, 0);
						cale++;
						for (i = 2 ; i < (naff + 1) ; i++)
						{
							adrh[i].ob_spec = adrh[i + 1].ob_spec;
							adrh[i].ob_state = adrh[i + 1].ob_state;
						}
						adrh[naff + 1].ob_spec = old_adrh[cale + naff - 1].ob_spec;
						adrh[naff + 1].ob_state = old_adrh[cale + naff - 1].ob_state;
						fill_tab (pxy, 8,
											xs, ys + (2 * haut),
											xs + adrh->ob_width, ys + ((naff + 1) * haut),
											xs, ys + haut,
											xs + adrh->ob_width, ys + (naff * haut));
						v_hide_c (work_display.handle);
						objc_change (adrh, ob, 0, x, y, w, h, NORMAL, TRUE);
						vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecran, &ecran);
						objc_change (adrh, ob, 0, x, y, w, h, SELECTED, TRUE);
						v_show_c (work_display.handle, TRUE);
						objc_draw (adrh, ROOT, 2,
											 x + adrh[naff + 1].ob_x, y + adrh[naff + 1].ob_y,
											 adrh[naff + 1].ob_width, adrh[naff + 1].ob_height);
					}
				}
			}

			if ((ob == -1) && (ym < yo))
			{
				sortie = TRUE;
				etat = adr_menu[option].ob_state & 0xFF00;
				objc_change (adr_menu, option, 0, xo, yo, adr_menu[option].ob_width,
										 adr_menu[option].ob_height, etat, TRUE);
			}
		}
	} while (NOT sortie);
	pxy[4] = x - 1;
	pxy[5] = y - 1;
	pxy[6] = pxy[4] + w + 4;
	pxy[7] = pxy[5] + h + 4;
	pxy[0] = 0;
	pxy[1] = 0;
	pxy[2] = w + 4;
	pxy[3] = h + 4;
	v_hide_c (work_display.handle);
	vro_cpyfm (work_display.handle, S_ONLY, pxy, &image, &ecran);	/* Restaurer le fond */
	v_show_c (work_display.handle, TRUE);
	free (image.fd_addr);	/* Lib‚rer tampon image */
	if (fl_scroll)
	{
		free (scroll);
		scroll = (OBJECT *) ZERO;
		adrh = old_adrh;
		if (ob > ZERO)
			ob = ob - 2 + cale;
	}

	if ((ob > 0) && (NOT (adrh[ob].ob_state & DISABLED)))
	{
		evnt = TRUE;
		buf[0] = BEV_HIERM;
		buf[1] = ap_id;
		buf[3] = titre;
		buf[4] = option;
		buf[5] = tree;
		buf[6] = ob;
		etat = adrh[buf[6]].ob_state & 0xFF00;
		objc_change (adrh, buf[6], 0, x, y, w, h, etat, FALSE);
		menu_tnormal (adr_menu, titre, 0);
		if ((adrh[buf[6]].ob_type >> 8) == B_FNCP)	/* Si fonction associ‚e */
		{
			(*fnc[adrh[buf[6]].ob_state >> 8])();
			empty (MU_KEYBD);
		}
		return evnt;
	}
	else
		return FALSE;
}
/* #] menu_hierarch () GŠre un menu hi‚rarchique :								*/ 
/* #[ pop_up () Routine de gestion de menu Pop-Up :								*/
void pop_up (OBJECT *adr, int object)
{
OBJECT *adresse, *old_adresse;
int i, tree, x, y, w, h, dummy, old_ob = -1, ob = 0, b[8], xm, ym, km,
		evnt, sortie = FALSE, etat, old = 0, fl_scroll = FALSE, naff,
		cale, nbre, down, pxy[8], haut, xs, ys, cadre[10], top, index,
		jaune[3], old_jaune[3], noir[3], old_noir[3], f3d = FALSE;
MFDB nul = {0}, image;
char chaine[MAX_LEN];

	tree = (adr[object].ob_state) >> 8;	/* R‚cupŠre nø formulaire associ‚ */
	adresse = ir_trindex[tree];
	if (adresse->ob_flags & BKGR)
		f3d = TRUE;

	if (tree)
	{
		objc_offset (adr, object, &x, &y);		/* Position bouton cliqu‚ */
		y += (adr[object].ob_height + 2);			/* Se placer dessous */
		adresse->ob_x = x;										/* Positionner le */
		adresse->ob_y = y;										/* formulaire pop_up. */
		w = adresse->ob_width;								/* Prendre ses */
		h = adresse->ob_height;								/* dimensions. */

		if (h > bureau.hd)										/* Si plus haut que le bureau */
		{
			if ((y + (6 * bi_trindex[BIGARRS][SC_UP].ob_height)) > (bureau.yd + bureau.hd - 3))
				y = adresse->ob_y = bureau.yd + 3;

			i = match (adr, object);
			if (i)
			{
				adresse[i].ob_state |= CHECKED;
				old = i;
			}

			naff = create_scroll (adresse, y, &nbre, old);	/* cr‚er scroll-menu */
			cale = max (old, 1);
			if ((cale + naff) > (nbre + 1))
				cale = nbre - naff + 1;
			down = naff + 2;
			old_adresse = adresse;
			adresse = scroll;
			adresse->ob_x = x;
			adresse->ob_y = y;
			w = adresse->ob_width;
			h = adresse->ob_height;
			haut = adresse[1].ob_height;
			fl_scroll = TRUE;
		}
		else
		{
				/* Aligner le pop_up avec le bouton */
			if (((adr[object].ob_type & 0xFF) == G_BUTTON) ||
					((adr[object].ob_type & 0xFF) == G_USERDEF))	/* Si Pop-Up de STRINGs */
			{
				i = match (adr, object);
				if (i)
				{
					x = adresse->ob_x = x - adresse[i].ob_x;
					y = adresse->ob_y = y - adresse[i].ob_y - adr[object].ob_height;
					old = i;
					adresse[old].ob_state |= CHECKED;
				}
			}
			else if ((adr[object].ob_type & 0xFF) == G_IMAGE)	/* Si Pop-Up d'IMAGEs */
			{
				i = match (adr, object);
				if (i)
				{
					x = adresse->ob_x = x - adresse[i].ob_x;
					y = adresse->ob_y = y - adresse[i].ob_y - adr[object].ob_height;
				}
			}
			else if ((adr[object].ob_type & 0xFF) == G_BOX)		/* Si Pop-Up de BOXs */
			{
				i = match (adr, object);
				if (i)
				{
					x = adresse->ob_x = x - adresse[i].ob_x;
					y = adresse->ob_y = y - adresse[i].ob_y - adr[object].ob_height;
					old = i;
					adresse[old].ob_state |= CHECKED;
				}
			}
		}

		if (x + w > bureau.xd + bureau.wd - 5)
			x = adresse->ob_x = bureau.wd + bureau.xd - w - 5;
		if (x < bureau.xd + 5)
			x = adresse->ob_x = bureau.xd + 5;
		if (y + h > bureau.yd + bureau.hd - 5)
			y = adresse->ob_y = bureau.hd + bureau.yd - h - 5;
		if (y < bureau.yd + 5)
			y = adresse->ob_y = bureau.yd + 5;

		get_bkgr (x, y, w, h, &image);

		objc_draw (adresse, ROOT, MAX_DEPTH, x - 3, y - 3, w + 6, h + 6);

		wind_update (BEG_MCTRL); 	/* Bloquer le menu */
		if (f3d)
		{
			wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
			if ((adr[object].ob_type & 0xFF) != G_BOX)
			{
				if (top)	/* Restaurer provisoirement la palette par d‚faut */
					set_palette (BLANK);
				jaune[0] = 1000;
				jaune[1] = 1000;
				jaune[2] = 0;		/* Pr‚parer le jaune */
				noir[0] = 0;
				noir[1] = 0;
				noir[2] = 0;		/* Pr‚parer le noir */
				vq_color (work_display.handle, 6, 1, old_jaune);	/* Sauver l'ancien "jaune" */
				vq_color (work_display.handle, 15, 1, old_noir);	/* Sauver l'ancien "noir" */
				vs_color (work_display.handle, 6, jaune);					/* Fixer le jaune */
				vs_color (work_display.handle, 15, noir);					/* Fixer le noir */
			}
			vsl_width (work_display.handle, 1);				/* Traits normaux */
			vsl_ends (work_display.handle, SQUARE, SQUARE);
			vsl_type (work_display.handle, SOLID);
			vsl_color (work_display.handle, BLACK);
			vst_font (work_display.handle, 1);
			vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &dummy, &dummy);
			vst_effects (work_display.handle, EFNONE);		/* D‚sactiver tous les effets */
			vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);	/* Align‚ en haut … gauche */
			vst_color (work_display.handle, 15);	/* Texte en noir */
			vsf_perimeter (work_display.handle, FALSE);
			vsf_interior (work_display.handle, FIS_SOLID);
			if (work_display.n_color >= 16)
				vsf_color (work_display.handle, YELLOW);
			else
				vsf_color (work_display.handle, WHITE);
		}
		do
		{
			evnt = evnt_multi (MU_BUTTON | MU_TIMER,
												 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
												 b, 10, 0, &xm, &ym, &km, &dummy, &dummy, &dummy);
			if (evnt & MU_BUTTON)
			{
				while (km)	/* Attend que le bouton de la souris soit relƒch‚ */
					graf_mkstate (&dummy, &dummy, &km, &dummy);
				sortie = TRUE;
			}
			else if (evnt == MU_TIMER)
			{
				graf_mkstate (&xm, &ym, &dummy, &dummy);
				ob = objc_find (adresse, ROOT, MAX_DEPTH, xm, ym);
				if (old_ob != ob)	/* Si objet diff‚rent */
				{
					if ((old_ob != -1) && (NOT (adresse[old_ob].ob_state & DISABLED)))
					{
						if ((adresse[old_ob].ob_type & 0xFF) == G_BOX)
						{
							vswr_mode (work_display.handle, MD_XOR);	/* Dessin En Xor */
							objc_offset (adresse, old_ob, &xs, &ys);
							cadre[0] = xs - 3;
							cadre[1] = ys - 3;
							cadre[2] = xs + adresse[old_ob].ob_width + 2;
							cadre[3] = ys - 3;
							cadre[4] = xs + adresse[old_ob].ob_width + 2;
							cadre[5] = ys + adresse[old_ob].ob_height + 2;
							cadre[6] = xs - 3;
							cadre[7] = ys + adresse[old_ob].ob_height + 2;
							cadre[8] = xs - 3;
							cadre[9] = ys - 3;
							v_hide_c (work_display.handle);
							v_pline (work_display.handle, 5, cadre);
							v_show_c (work_display.handle, TRUE);
						}
						else
						{
							etat = adresse[old_ob].ob_state & 0xFF04;
							objc_change (adresse, old_ob, 0, x, y, w, h, etat, FALSE);
							objc_offset (adresse, old_ob, &xs, &ys);
							objc_draw (adresse, ROOT, MAX_DEPTH, xs, ys,
												 adresse[old_ob].ob_width, adresse[old_ob].ob_height);
						}
					}
					if ((ob > ROOT) && (NOT (adresse[ob].ob_state & DISABLED)))
					{
						if ((adresse[ob].ob_type & 0xFF) == G_BOX)
						{
							vswr_mode (work_display.handle, MD_XOR);	/* Dessin En Xor */
							objc_offset (adresse, ob, &xs, &ys);
							cadre[0] = xs - 3;
							cadre[1] = ys - 3;
							cadre[2] = xs + adresse[ob].ob_width + 2;
							cadre[3] = ys - 3;
							cadre[4] = xs + adresse[ob].ob_width + 2;
							cadre[5] = ys + adresse[ob].ob_height + 2;
							cadre[6] = xs - 3;
							cadre[7] = ys + adresse[ob].ob_height + 2;
							cadre[8] = xs - 3;
							cadre[9] = ys - 3;
							v_hide_c (work_display.handle);
							v_pline (work_display.handle, 5, cadre);
							v_show_c (work_display.handle, TRUE);
							etat = adresse[ob].ob_state;
						}
						else if ((adresse[ob].ob_type & 0xFF) == G_STRING)
						{
							if (((adresse->ob_spec.obspec.fillpattern != ZERO) ||
										(adresse->ob_spec.obspec.interiorcol != ZERO)) &&
										(f3d))
							{
								strcpy (chaine, get_text (adresse, ob));
								objc_offset (adresse, ob, &xs, &ys);
								v_hide_c (work_display.handle);
								vswr_mode (work_display.handle, MD_REPLACE);	/* Dessin En Remplacement */
								cadre[0] = xs;
								cadre[1] = ys;
								cadre[2] = xs + adresse[ob].ob_width - 1;
								cadre[3] = ys + adresse[ob].ob_height - 1;
								v_bar (work_display.handle, cadre);				/* Rectangle */
								cadre[0] = xs;
								cadre[1] = ys + adresse[ob].ob_height - 1;
								cadre[2] = xs;
								cadre[3] = ys;
								cadre[4] = xs + adresse[ob].ob_width - 1;
								cadre[5] = ys;
								v_pline (work_display.handle, 3, cadre);	/* Ligne d'ombre noire */
								vswr_mode (work_display.handle, MD_TRANS);	/* Texte en Transparent */
								v_gtext (work_display.handle, xs, ys, chaine);	/* Ecrire le texte */
								if (adresse[ob].ob_state & CHECKED)								/* Si option check‚e, */
									v_gtext (work_display.handle, xs + 2, ys, "");	/* mettre une marque. */
								v_show_c (work_display.handle, TRUE);
								etat = adresse[ob].ob_state;
							}
							else
								etat = adresse[ob].ob_state | SELECTED;
						}
						else
							etat = adresse[ob].ob_state | SELECTED;
						objc_change (adresse, ob, 0, x, y, w, h, etat, TRUE);
					}
					old_ob = ob;
				}
				else							/* Si mˆme objet */
				{
					if ((fl_scroll) && (ob == 1))
					{									/* Si scroll : flˆche vers le haut */
						objc_offset (adresse, 0, &xs, &ys);
						if (cale > 1)
						{
							cale--;
							for (i = naff + 1 ; i > 2 ; i--)
							{
								adresse[i].ob_spec = adresse[i - 1].ob_spec;
								adresse[i].ob_state = adresse[i - 1].ob_state;
							}
							adresse[2].ob_spec = old_adresse[cale].ob_spec;
							adresse[2].ob_state = old_adresse[cale].ob_state;
							fill_tab (pxy, 8,
												xs, ys + haut,
												xs + adresse->ob_width, ys + (naff * haut),
												xs, ys + (2 * haut),
												xs + adresse->ob_width, ys + ((naff + 1) * haut));
							v_hide_c (work_display.handle);
							etat = adresse[ob].ob_state &= ~SELECTED;
							objc_change (adresse, ob, 0, x, y, w, h, etat, TRUE);
							vro_cpyfm (work_display.handle, S_ONLY, pxy, &nul, &nul);
							etat = adresse[ob].ob_state |= SELECTED;
							objc_change (adresse, ob, 0, x, y, w, h, etat, TRUE);
							v_show_c (work_display.handle, TRUE);
							objc_draw (adresse, ROOT, 2,
												 x + adresse[2].ob_x, y + adresse[2].ob_y,
												 adresse[2].ob_width, adresse[2].ob_height);
						}
					}
					else if ((fl_scroll) && (ob == down))
					{									/* Si scroll : flˆche vers le bas */
						objc_offset (adresse, 0, &xs, &ys);
						if ((cale + naff) <= nbre)
						{
							cale++;
							for (i = 2 ; i < (naff + 1) ; i++)
							{
								adresse[i].ob_spec = adresse[i + 1].ob_spec;
								adresse[i].ob_state = adresse[i + 1].ob_state;
							}
							adresse[naff + 1].ob_spec = old_adresse[cale + naff - 1].ob_spec;
							adresse[naff + 1].ob_state = old_adresse[cale + naff - 1].ob_state;
							fill_tab (pxy, 8,
												xs, ys + (2 * haut),
												xs + adresse->ob_width, ys + ((naff + 1) * haut) - 1,
												xs, ys + haut,
												xs + adresse->ob_width, ys + (naff * haut) - 1);
							v_hide_c (work_display.handle);
							etat = adresse[ob].ob_state &= ~SELECTED;
							objc_change (adresse, ob, 0, x, y, w, h, etat, TRUE);
							vro_cpyfm (work_display.handle, S_ONLY, pxy, &nul, &nul);
							etat = adresse[ob].ob_state |= SELECTED;
							objc_change (adresse, ob, 0, x, y, w, h, etat, TRUE);
							v_show_c (work_display.handle, TRUE);
							objc_draw (adresse, ROOT, 2,
												 x + adresse[naff + 1].ob_x, y + adresse[naff + 1].ob_y,
												 adresse[naff + 1].ob_width, adresse[naff + 1].ob_height);
						}
					}
				}
			}
		} while (NOT sortie);
		if ((adr[object].ob_type & 0xFF) != G_BOX)
		{
			if (f3d)
			{
				vs_color (work_display.handle, 6, old_jaune);	/* Restaurer le jaune */
				vs_color (work_display.handle, 15, old_noir);	/* Restaurer le noir */
				if (top)
				{		/* Restaurer la palette courante */
					index = find_index (top);
					set_palette (index);
				}
			}
		}
		wind_update (END_MCTRL); 	/* D‚bloquer le menu */

		put_bkgr (x, y, w, h, &image);
		free (image.fd_addr);	/* Lib‚rer tampon image */

		if ((ob > ZERO) && (NOT (adresse[ob].ob_state & DISABLED)))
		{
			etat = adresse[ob].ob_state & 0xFF00;
			objc_change (adresse, ob, 0, x, y, w, h, etat, FALSE);
			if ((adresse[ob].ob_type & 0xFF) == G_STRING)
			{
				strcpy (chaine, get_text (adresse, ob));
				set_text (adr, object, trim (chaine));
			}
			else if ((adresse[ob].ob_type & 0xFF) == G_IMAGE)
				adr[object].ob_spec = adresse[ob].ob_spec;
			else if ((adresse[ob].ob_type & 0xFF) == G_BOX)
			{
				adr[object].ob_spec.obspec.fillpattern = adresse[ob].ob_spec.obspec.fillpattern;
				adr[object].ob_spec.obspec.interiorcol = adresse[ob].ob_spec.obspec.interiorcol;
			}
		}
		/* MA 16/07/94 */
		if (ob < ZERO)		/* Clic dehors */
			cliq_in = FALSE;
		else							/* Clic dedans */
			cliq_in = TRUE;
		if (old)
		{
			if (fl_scroll)
				old_adresse[old].ob_state &= ~CHECKED;
			else
				adresse[old].ob_state &= ~CHECKED;
		}
		if (fl_scroll)
		{
			free (scroll);
			scroll = (OBJECT *) ZERO;
		}
	}
}
/* #] pop_up () Routine de gestion de menu Pop-Up :								*/ 
/* #[ pop_liste () Routine de gestion de menu Pop-Liste :					*/
void pop_liste (OBJECT *adr, int object)
{
OBJECT *adresse, *pop;
int i, tree, x, y, w, h, dummy, old_ob = -1, ob = 0, b[8], xm, ym, km,
		evnt, sortie = FALSE, etat, old = ZERO, cale, nbre = ZERO, haut, maxw,
		oldw_r, oldw_b, oldx_a, ysl, oym, oys, delta, yas, pxy[8], xbk, ybk, ha,
		cadre[6], xs, ys, jaune[3], old_jaune[3], noir[3], old_noir[3], f3d = FALSE, top, index;
MFDB image, nul = {0};
char chaine[MAX_LEN];

	tree = (adr[object].ob_state) >> 8;	/* R‚cupŠre nø formulaire associ‚ */
	adresse = ir_trindex[tree];
	if (adresse->ob_flags & BKGR)
		f3d = TRUE;

	if (tree)
	{
		do {} while (NOT (adresse[nbre++].ob_flags & LASTOB));	/* Compter les options */
		nbre--;
		if (nbre < 9)
		{
			pop_up (adr, object);
			return;
		}
		pop = bi_trindex[BIGLIST];
		i = match (adr, object);
		if (i)
		{
			adresse[i].ob_state |= CHECKED;
			old = i;
		}
		cale = max (old, 1);
		if ((cale + 8) > nbre)
			cale = nbre - 7;
		haut = pop[BL_S1].ob_height;

		oldw_r = pop->ob_width;
		oldw_b = pop[BL_BKGR].ob_width;
		oldx_a = pop[BL_BA].ob_x;
		maxw = adresse->ob_width;
		pop->ob_width += maxw;
		pop[BL_BKGR].ob_width = maxw;
		pop[BL_BA].ob_x += (maxw + 1);
		pop[BL_BKGR].ob_spec.obspec.interiorcol = adresse->ob_spec.obspec.interiorcol;
		pop[BL_BKGR].ob_spec.obspec.fillpattern = adresse->ob_spec.obspec.fillpattern;
		objc_offset (adr, object, &x, &y);		/* Position bouton cliqu‚ */
		w = pop->ob_width;		/* Prendre ses */
		h = pop->ob_height;		/* dimensions. */
		if (y + h + adr[object].ob_height < bureau.hd + 3)		/* S'il y a la place */
			y += (adr[object].ob_height + 2);						/* Se placer dessous */
		else																					/* Sinon */
			y -= (h + 3);																/* Se placer dessus */
		pop->ob_x = x;				/* Positionner le */
		pop->ob_y = y;				/* formulaire pop_up. */

		if (x + w > bureau.xd + bureau.wd - 5)
			x = pop->ob_x = bureau.wd + bureau.xd - w - 5;
		if (x < bureau.xd + 5)
			x = pop->ob_x = bureau.xd + 5;

		ha = pop[BL_ASC].ob_height;
		pop[BL_SL].ob_height = (ha * (8 * haut) / (haut * nbre));
		pop[BL_SL].ob_y = (int)(((double)ha / ((double)nbre) * (double)(cale - 1)));

		for (i = BL_S1 ; i <= BL_S8 ; i++)	/* Remplir les chaŒnes */
		{
			pop[i].ob_spec = adresse[cale + i - BL_S1].ob_spec;
			pop[i].ob_state = adresse[cale + i - BL_S1].ob_state;
			pop[i].ob_width = maxw;
		}

		objc_offset (pop, BL_BKGR, &xbk, &ybk);
		get_bkgr (x, y, w, h, &image);

		objc_draw (pop, ROOT, MAX_DEPTH, x - 3, y - 3, w + 6, h + 6);

		wind_update (BEG_MCTRL); 	/* Bloquer le menu */
		if (f3d)
		{
			wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
			if (top)	/* Restaurer provisoirement la palette par d‚faut */
				set_palette (BLANK);
			jaune[0] = 1000;
			jaune[1] = 1000;
			jaune[2] = 0;		/* Pr‚parer le jaune */
			noir[0] = 0;
			noir[1] = 0;
			noir[2] = 0;		/* Pr‚parer le noir */
			vq_color (work_display.handle, 6, 1, old_jaune);	/* Sauver l'ancien "jaune" */
			vq_color (work_display.handle, 15, 1, old_noir);	/* Sauver l'ancien "noir" */
			vs_color (work_display.handle, 6, jaune);					/* Fixer le jaune */
			vs_color (work_display.handle, 15, noir);					/* Fixer le noir*/
			vsl_width (work_display.handle, 1);								/* Traits normaux */
			vsl_ends (work_display.handle, SQUARE, SQUARE);
			vsl_type (work_display.handle, SOLID);
			vsl_color (work_display.handle, BLACK);
			vst_font (work_display.handle, 1);
			vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &dummy, &dummy);
			vst_effects (work_display.handle, EFNONE);		/* D‚sactiver tous les effets */
			vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);	/* Align‚ en haut … gauche */
			vst_color (work_display.handle, 15);	/* Texte en noir */
			vsf_perimeter (work_display.handle, FALSE);
			vsf_interior (work_display.handle, FIS_SOLID);
			if (work_display.n_color >= 16)
				vsf_color (work_display.handle, YELLOW);
			else
				vsf_color (work_display.handle, WHITE);
		}
		do
		{
			evnt = evnt_multi (MU_BUTTON | MU_TIMER,
												 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
												 b, 10, 0, &xm, &ym, &km, &dummy, &dummy, &dummy);
			if (evnt & MU_BUTTON)
			{
				if ((ob >= BL_UP) && (ob <= BL_SL))
				{
					switch (ob)
					{
					case BL_ASC :
						objc_offset (pop, BL_SL, &dummy, &ysl);		/* Position slider */
						while (km)
						{
							evnt_timer (100, 0);
							if (ym < ysl)			/* Si page haut */
							{
								if (cale > 1)
								{
									cale -= 8;
									if (cale < 1)
										cale = 1;
									for (i = BL_S1 ; i <= BL_S8 ; i++)	/* Remplir les chaŒnes */
									{
										pop[i].ob_spec = adresse[cale + i - BL_S1].ob_spec;
										pop[i].ob_state = adresse[cale + i - BL_S1].ob_state;
									}
									pop[BL_SL].ob_y = (int)(((double)ha / ((double)nbre) * (double)(cale - 1)));
									objc_draw (pop, BL_ASC, 2,
														 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
														 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
									objc_draw (pop, BL_BKGR, 2,
														 x + pop[BL_BKGR].ob_x, y + pop[BL_BKGR].ob_y,
														 pop[BL_BKGR].ob_width, pop[BL_BKGR].ob_height);
								}
							}
							else							/* Si page bas */
							{
								if (cale < nbre - 7)
								{
									cale += 8;
									if (cale > nbre - 7)
										cale = nbre - 7;
									for (i = BL_S1 ; i <= BL_S8 ; i++)	/* Remplir les chaŒnes */
									{
										pop[i].ob_spec = adresse[cale + i - BL_S1].ob_spec;
										pop[i].ob_state = adresse[cale + i - BL_S1].ob_state;
									}
									pop[BL_SL].ob_y = (int)(((double)ha / ((double)nbre) * (double)(cale - 1)));
									objc_draw (pop, BL_ASC, 2,
														 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
														 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
									objc_draw (pop, BL_BKGR, 2,
														 x + pop[BL_BKGR].ob_x, y + pop[BL_BKGR].ob_y,
														 pop[BL_BKGR].ob_width, pop[BL_BKGR].ob_height);
								}
							}
							graf_mkstate (&dummy, &dummy, &km, &dummy);
						}
						break;
					case BL_SL :
						objc_offset (pop, BL_SL, &dummy, &ysl);		/* Position slider */
						objc_offset (pop, BL_ASC, &dummy, &yas);	/* Position ascenceur */
						oym = ym;
						oys = pop[BL_SL].ob_y;
						pop[BL_SL].ob_state = SELECTED;
						objc_draw (pop, BL_SL, 2,
											 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
											 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
						while (km)
						{
							graf_mkstate (&dummy, &ym, &dummy, &dummy);
							delta = ym - ysl;
							if (ym != oym)
							{
								if ((ym - delta >= yas) &&
										(ym - delta + pop[BL_SL].ob_height <= yas + pop[BL_ASC].ob_height))
								{
									if ((ym > oym) && (ym > ysl))
									{
										pop[BL_SL].ob_y += (ym - oym);
										pop[BL_SL].ob_y = min (pop[BL_SL].ob_y, ha - pop[BL_SL].ob_height);
									}
									else if ((ym < oym) && (ym < ysl + pop[BL_SL].ob_height))
									{
										pop[BL_SL].ob_y -= (oym - ym);
										pop[BL_SL].ob_y = max (pop[BL_SL].ob_y, 0);
									}
									if (oys != pop[BL_SL].ob_y)
									{
										cale = (int)((double)nbre /
													 ((double)ha / (double)pop[BL_SL].ob_y)) + 1;
										objc_draw (pop, BL_ASC, 2,
															 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
															 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
										objc_offset (pop, BL_SL, &dummy, &ysl);		/* Position slider */
										for (i = BL_S1 ; i <= BL_S8 ; i++)	/* Remplir les chaŒnes */
										{
											pop[i].ob_spec = adresse[cale + i - BL_S1].ob_spec;
											pop[i].ob_state = adresse[cale + i - BL_S1].ob_state;
										}
										objc_draw (pop, BL_BKGR, 2,
															 x + pop[BL_BKGR].ob_x, y + pop[BL_BKGR].ob_y,
															 pop[BL_BKGR].ob_width, pop[BL_BKGR].ob_height);
										oys = pop[BL_SL].ob_y;
									}
								}
							}
							oym = ym;
							graf_mkstate (&dummy, &dummy, &km, &dummy);
						}
						pop[BL_SL].ob_state = NORMAL;
						objc_draw (pop, BL_SL, 2,
											 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
											 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
						break;
					case BL_UP :
						pop[BL_UP].ob_state = SELECTED;
						objc_draw (pop, BL_UP, 2,
											 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
											 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
						while (km)
						{
							if (cale > 1)
							{
								evnt_timer (50, 0);
								cale--;
								for (i = BL_S8 ; i > (BL_S1) ; i--)
								{
									pop[i].ob_spec = pop[i - 1].ob_spec;
									pop[i].ob_state = pop[i - 1].ob_state;
								}
								pop[BL_S1].ob_spec = adresse[cale].ob_spec;
								pop[BL_S1].ob_state = adresse[cale].ob_state;
								fill_tab (pxy, 8,
													xbk, ybk,
													xbk + maxw, ybk + (7 * haut),
													xbk, ybk + haut,
													xbk + maxw, ybk + (8 * haut));
								v_hide_c (work_display.handle);
								vro_cpyfm (work_display.handle, S_ONLY, pxy, &nul, &nul);
								v_show_c (work_display.handle, TRUE);
								objc_draw (pop, ROOT, 2,
													 x + pop[BL_S1].ob_x, y + pop[BL_S1].ob_y,
													 pop[BL_S1].ob_width, pop[BL_S1].ob_height);
								pop[BL_SL].ob_y = (int)(((double)ha / ((double)nbre) * (double)(cale - 1)));
								objc_draw (pop, BL_ASC, 2,
													 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
													 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
							}
							graf_mkstate (&dummy, &dummy, &km, &dummy);
						}
						pop[BL_UP].ob_state = NORMAL;
						objc_draw (pop, BL_UP, 2,
											 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
											 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
						break;
					case BL_DN :
						pop[BL_DN].ob_state = SELECTED;
						objc_draw (pop, BL_DN, 2,
											 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
											 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
						while (km)
						{
							if (cale + 7 < nbre)
							{
								evnt_timer (50, 0);
								cale++;
								for (i = BL_S1 ; i < (BL_S8) ; i++)
								{
									pop[i].ob_spec = pop[i + 1].ob_spec;
									pop[i].ob_state = pop[i + 1].ob_state;
								}
								pop[BL_S8].ob_spec = adresse[cale + 7].ob_spec;
								pop[BL_S8].ob_state = adresse[cale + 7].ob_state;
								fill_tab (pxy, 8,
													xbk, ybk + haut,
													xbk + maxw, ybk + (8 * haut) - 1,
													xbk, ybk,
													xbk + maxw, ybk + (7 * haut) - 2);
								v_hide_c (work_display.handle);
								vro_cpyfm (work_display.handle, S_ONLY, pxy, &nul, &nul);
								v_show_c (work_display.handle, TRUE);
								objc_draw (pop, ROOT, 2,
													 x + pop[BL_S8].ob_x, y + pop[BL_S8].ob_y,
													 pop[BL_S8].ob_width, pop[BL_S8].ob_height);
								pop[BL_SL].ob_y = (int)(((double)ha / ((double)nbre) * (double)(cale - 1)));
								objc_draw (pop, BL_ASC, 2,
													 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
													 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
							}
							graf_mkstate (&dummy, &dummy, &km, &dummy);
						}
						pop[BL_DN].ob_state = NORMAL;
						objc_draw (pop, BL_DN, 2,
											 x + pop[BL_BA].ob_x, y + pop[BL_BA].ob_y,
											 pop[BL_BA].ob_width, pop[BL_BA].ob_height);
						break;
					}
				}
				else
				{
					while (km)	/* Attend que le bouton de la souris soit relƒch‚ */
						graf_mkstate (&dummy, &dummy, &km, &dummy);
					sortie = TRUE;
				}
			}
			else if (evnt == MU_TIMER)
			{
				graf_mkstate (&xm, &ym, &dummy, &dummy);
				ob = objc_find (pop, ROOT, MAX_DEPTH, xm, ym);
				if (old_ob != ob)	/* Si objet diff‚rent */
				{
					if ((old_ob != -1) && (NOT (pop[old_ob].ob_state & DISABLED)) &&
							(old_ob >= BL_S1) && (old_ob <= BL_S8))
					{
						if (((pop[BL_BKGR].ob_spec.obspec.fillpattern != ZERO) ||
									(pop[BL_BKGR].ob_spec.obspec.interiorcol != ZERO)) &&
									(f3d))
						{
							objc_offset (pop, old_ob, &xs, &ys);
							objc_draw (pop, ROOT, MAX_DEPTH, xs, ys,
												 pop[old_ob].ob_width, pop[old_ob].ob_height);
						}
						else
						{
							etat = pop[old_ob].ob_state & 0xFF00;
							objc_change (pop, old_ob, 0, x, y, w, h, etat, TRUE);
						}
					}
					if ((ob >= BL_S1) && (ob <= BL_S8) && (NOT (pop[ob].ob_state & DISABLED)))
					{
						if (((pop[BL_BKGR].ob_spec.obspec.fillpattern != ZERO) ||
									(pop[BL_BKGR].ob_spec.obspec.interiorcol != ZERO)) &&
									(f3d))
						{
							strcpy (chaine, get_text (pop, ob));
							objc_offset (pop, ob, &xs, &ys);
							v_hide_c (work_display.handle);
							vswr_mode (work_display.handle, MD_REPLACE);	/* Dessin En Remplacement */
							cadre[0] = xs;
							cadre[1] = ys;
							cadre[2] = xs + pop[ob].ob_width - 1;
							cadre[3] = ys + pop[ob].ob_height - 1;
							v_bar (work_display.handle, cadre);				/* Rectangle */
							cadre[0] = xs;
							cadre[1] = ys + pop[ob].ob_height - 1;
							cadre[2] = xs;
							cadre[3] = ys;
							cadre[4] = xs + pop[ob].ob_width - 1;
							cadre[5] = ys;
							v_pline (work_display.handle, 3, cadre);	/* Ligne d'ombre noire */
							vswr_mode (work_display.handle, MD_TRANS);	/* Texte en Transparent */
							v_gtext (work_display.handle, xs, ys, chaine);	/* Ecrire le texte */
							if (pop[ob].ob_state & CHECKED)								/* Si option check‚e, */
								v_gtext (work_display.handle, xs + 2, ys, "");	/* mettre une marque. */
							v_show_c (work_display.handle, TRUE);
						}
						else
						{
							etat = pop[ob].ob_state | SELECTED;
							objc_change (pop, ob, 0, x, y, w, h, etat, TRUE);
						}
					}
					old_ob = ob;
				}
			}
		} while (NOT sortie);
		if (f3d)
		{
			vs_color (work_display.handle, 6, old_jaune);	/* Restaurer le jaune */
			vs_color (work_display.handle, 15, old_noir);	/* Restaurer le noir */
			if (top)
			{		/* Restaurer la palette courante */
				index = find_index (top);
				set_palette (index);
			}
		}
		wind_update (END_MCTRL); 	/* D‚bloquer le menu */

		put_bkgr (x, y, w, h, &image);
		pop->ob_width = oldw_r;
		pop[BL_BKGR].ob_width = oldw_b;
		pop[BL_BA].ob_x = oldx_a;
		free (image.fd_addr);	/* Lib‚rer tampon image */

		if ((ob >= BL_S1) && (ob <= BL_S8) && (NOT (pop[ob].ob_state & DISABLED)))
		{
			etat = pop[ob].ob_state & 0xFF00;
			objc_change (pop, ob, 0, x, y, w, h, etat, FALSE);
			strcpy (chaine, get_text (pop, ob));
			set_text (adr, object, trim (chaine));
		}
		if (old)
			adresse[old].ob_state &= ~CHECKED;

		/* MA 16/07/94 */
		if (ob < ZERO)		/* Clic dehors */
			cliq_in = FALSE;
		else							/* Clic dedans */
			cliq_in = TRUE;
	}
}
/* #] pop_liste () Routine de gestion de menu Pop-Liste :					*/ 
/* #[ match () Cherche correspondance bouton et pop-up :					*/
int match (OBJECT *adr, int button)
{
int i = 1, tree, trame, color;
char bouton[MAX_LEN], option[MAX_LEN];
OBJECT *adrpu;
BITBLK *imag;

	if (NOT (adr[button].ob_state & SHADOWED))	/* Si pas pop-up */
		return ZERO;															/* Retourner 0 */

	tree = (adr[button].ob_state) >> 8;	/* R‚cup‚rer nø formulaire associ‚ */
	if (NOT tree)												/* S'il n'y en a pas */
		return ZERO;											/* retourner 0 */

	adrpu = ir_trindex[tree];

	if (((adr[button].ob_type & 0xFF) == G_BUTTON) ||
			((adr[button].ob_type & 0xFF) == G_USERDEF))	/* Si Pop-Up de STRINGs */
	{
		strcpy (bouton, get_text (adr, button));
		trim (bouton);
		do
		{
			strcpy (option, get_text (adrpu, i));
			trim (option);
			if ((adrpu[i].ob_type & 0xFF) == G_STRING)
			{
				if (strcmp (bouton, option) == ZERO)		/* Si occurence */
					return i;															/* retourner nø option */
			}
		} while (NOT (adrpu[i++].ob_flags & LASTOB));
	}
	else if ((adr[button].ob_type & 0xFF) == G_IMAGE)	/* Si Pop-Up d'IMAGEs */
	{
		imag = adr[button].ob_spec.bitblk;
		do
		{
			if ((adrpu[i].ob_type & 0xFF) == G_IMAGE)
			{
				if (imag == adrpu[i].ob_spec.bitblk)	/* Si occurence */
					return i;														/* retourner nø option */
			}
		} while (NOT (adrpu[i++].ob_flags & LASTOB));
	}
	else if ((adr[button].ob_type & 0xFF) == G_BOX)		/* Si Pop-Up de BOXs  */
	{
		trame = adr[button].ob_spec.obspec.fillpattern;
		color = adr[button].ob_spec.obspec.interiorcol;
		do
		{
			if ((adrpu[i].ob_type & 0xFF) == G_BOX)
			{
				if ((trame == adrpu[i].ob_spec.obspec.fillpattern) &&
						(color == adrpu[i].ob_spec.obspec.interiorcol))	/* Si occurence */
					return i;															/* retourner nø option */
			}
		} while (NOT (adrpu[i++].ob_flags & LASTOB));
	}
	return ZERO;														/* Si pas occurence, retourner 0 */
}
/* #] match () Cherche correspondance bouton et pop-up :					*/ 
/* #[ set_popup () Fixe une option de bouton pop_up :							*/
void set_popup (OBJECT *adr, int button, int option)
{
int tree;
char texte[MAX_LEN];
OBJECT *adrpu;

	if (NOT (adr[button].ob_state & SHADOWED))	/* Si pas pop-up */
		return;																		/* Ressortir */

	tree = (adr[button].ob_state) >> 8;	/* R‚cup‚rer nø formulaire associ‚ */
	if (NOT tree)												/* S'il n'y en a pas */
		return;														/* ressortir */

	adrpu = ir_trindex[tree];

	if (((adr[button].ob_type & 0xFF) == G_BUTTON) ||
			((adr[button].ob_type & 0xFF) == G_USERDEF))	/* Si Pop-Up de STRINGs */
	{
		strcpy (texte, get_text (adrpu, option));
		set_text (adr, button, trim (texte));
	}
	else if ((adr[button].ob_type & 0xFF) == G_IMAGE)	/* Si Pop-Up d'IMAGEs */
	{
		adr[button].ob_spec = adrpu[option].ob_spec;
	}
	else if ((adr[button].ob_type & 0xFF) == G_BOX)		/* Si Pop-Up de BOXs  */
	{
		adr[button].ob_spec.obspec.fillpattern = adrpu[option].ob_spec.obspec.fillpattern;
		adr[button].ob_spec.obspec.interiorcol = adrpu[option].ob_spec.obspec.interiorcol;
	}
}
/* #] set_popup () Fixe une option de bouton pop_up :							*/ 
/* #[ next_option () Option suivante d'un pop-up :								*/
void next_option (OBJECT *adr, int object, int form)
{
OBJECT *adresse;
int i = 1, tree, ob, trouve = FALSE, trame, color;
BITBLK *imag;
char chaine[MAX_LEN], bouton[MAX_LEN], option[MAX_LEN];

	tree = (adr[object].ob_state) >> 8;	/* R‚cupŠre nø formulaire associ‚ */
	if (NOT tree)					/* Si pas de formulaire associ‚ */
		tree = form;				/* Prendre celui qui est transmis */

	adresse = ir_trindex[tree];

	if (tree)
	{
			/* Chercher la correspondance */
		if (((adr[object].ob_type & 0xFF) == G_BUTTON) ||
				((adr[object].ob_type >> 8) == B_POPUP) ||
				((adr[object].ob_type >> 8) == B_LIST))		/* Si Pop-Up de STRINGs */
		{
			strcpy (bouton, get_text (adr, object));
			trim (bouton);
			do
			{
				if ((adresse[i].ob_type & 0xFF) == G_STRING)
				{
					strcpy (option, get_text (adresse, i));
					trim (option);
					if (strcmp (bouton, option) == ZERO)
						trouve = i;
				}
			} while ((NOT (adresse[i++].ob_flags & LASTOB)) && (NOT trouve));
		}
		else if ((adr[object].ob_type & 0xFF) == G_IMAGE)	/* Si Pop-Up d'IMAGEs */
		{
			imag = adr[object].ob_spec.bitblk;
			do
			{
				if ((adresse[i].ob_type & 0xFF) == G_IMAGE)
				{
					if (imag == adresse[i].ob_spec.bitblk)
						trouve = i;
				}
			} while ((NOT (adresse[i++].ob_flags & LASTOB)) && (NOT trouve));
		}
		else if ((adr[object].ob_type & 0xFF) == G_BOX)		/* Si Pop-Up de BOXs */
		{
			trame = adr[object].ob_spec.obspec.fillpattern;
			color = adr[object].ob_spec.obspec.interiorcol;
			do
			{
				if ((adresse[i].ob_type & 0xFF) == G_BOX)
				{
					if ((trame == adresse[i].ob_spec.obspec.fillpattern) &&
							(color == adresse[i].ob_spec.obspec.interiorcol))
						trouve = i;
				}
			} while ((NOT (adresse[i++].ob_flags & LASTOB)) && (NOT trouve));
		}
		if (trouve)
		{
			if (NOT (adresse[trouve].ob_flags & LASTOB))
				ob = trouve + 1;
			else
				ob = 1;
		}
		else
			ob = 1;

		while ((adresse[ob].ob_state & DISABLED) && (ob != trouve))
		{
			ob++;
			if (adresse[ob].ob_flags & LASTOB)
				ob = 1;
		}

		if ((adresse[ob].ob_type & 0xFF) == G_STRING)
		{
			strcpy (chaine, get_text (adresse, ob));
			set_text (adr, object, trim (chaine));
		}
		else if ((adresse[ob].ob_type & 0xFF) == G_IMAGE)
		{
			adr[object].ob_spec = adresse[ob].ob_spec;
		}
		else if ((adresse[ob].ob_type & 0xFF) == G_BOX)
		{
			adr[object].ob_spec.obspec.fillpattern = adresse[ob].ob_spec.obspec.fillpattern;
			adr[object].ob_spec.obspec.interiorcol = adresse[ob].ob_spec.obspec.interiorcol;
		}
	}
}
/* #] next_option () Option suivante d'un pop-up :								*/ 
/* #[ create_scroll () Construit un scroll_menu :									*/
int create_scroll (OBJECT *pu, int y, int *nb, int first)
{
int maxw, nbre = ZERO, naff, i = ZERO;

	first = max (first, 1);
	maxw = pu->ob_width;		/* Largeur du pop-up */
	do {} while (NOT (pu[nbre++].ob_flags & LASTOB));	/* Compter les options */
	nbre--;
	*nb = nbre;
	naff = ((bureau.hd - y - 3) / bi_trindex[BIGARRS][SC_UP].ob_height) - 1;
	if ((first + naff) > nbre)
		first = nbre - naff + 1;
	bi_trindex[BIGARRS]->ob_width = maxw;		/* Ajuster les largeurs */
	bi_trindex[BIGARRS][SC_UP].ob_width = maxw;
	bi_trindex[BIGARRS][SC_DN].ob_width = maxw;
	bi_trindex[BIGARRS]->ob_height = (naff + 2) * bi_trindex[BIGARRS][SC_UP].ob_height;
	bi_trindex[BIGARRS]->ob_spec.obspec.interiorcol = pu->ob_spec.obspec.interiorcol;
	bi_trindex[BIGARRS]->ob_spec.obspec.fillpattern = pu->ob_spec.obspec.fillpattern;
	bi_trindex[BIGARRS][SC_UP].ob_spec.obspec.interiorcol = pu->ob_spec.obspec.interiorcol;
	bi_trindex[BIGARRS][SC_UP].ob_spec.obspec.fillpattern = pu->ob_spec.obspec.fillpattern;
	bi_trindex[BIGARRS][SC_DN].ob_spec.obspec.interiorcol = pu->ob_spec.obspec.interiorcol;
	bi_trindex[BIGARRS][SC_DN].ob_spec.obspec.fillpattern = pu->ob_spec.obspec.fillpattern;
	scroll = (OBJECT *)malloc ((nbre + 3) * sizeof (OBJECT));
	memcpy (scroll, bi_trindex[BIGARRS], (2 * sizeof (OBJECT)));
	memcpy (scroll + 2, pu + first, (naff * sizeof (OBJECT)));
	memcpy (scroll + 2 + naff, bi_trindex[BIGARRS] + 2, sizeof (OBJECT));
	scroll->ob_tail += naff;
	for (i = 1 ; i <= (naff + 1) ; i++)
		scroll[i].ob_next = i + 1;
	scroll[i - 1].ob_flags &= ~LASTOB;
	scroll[i].ob_flags |= LASTOB;
	for (i = 2 ; i <= (naff + 2) ; i++)
		scroll[i].ob_y = scroll[i - 1].ob_y + scroll[i - 1].ob_height;
	return naff;
}
/* #] create_scroll () Construit un scroll_menu :									*/ 
/* #[ free_popup () Gestion d'un Pop-Up librement positionn‚ :		*/
int free_popup (int posx, int posy, int tree, OBJECT *adr)
{
int x, y, w, h, xm, ym, km, dummy, ob, old_ob = -1, evnt, b[8],
		sortie = FALSE, etat, nbre, fl_scroll = FALSE, pxy[8], xs, ys,
		cale = 1, naff, down, haut, i, cadre[6], top, index,
		jaune[3], old_jaune[3], noir[3], old_noir[3], f3d = FALSE;
char chaine[MAX_LEN];
OBJECT *adresse, *old_adresse;
MFDB img, nul = {0};

	if (tree > BLANK)
		adresse = ir_trindex[tree];						/* Adresse formulaire pop_up */
	else
		adresse = adr;
	if (adresse->ob_flags & BKGR)
		f3d = TRUE;

	w = adresse->ob_width;
	h = adresse->ob_height;
	x = adresse->ob_x = posx - (w / 2);
	y = adresse->ob_y = posy - (h / 2);
	if (x + w > bureau.xd + bureau.wd - 5)
		x = adresse->ob_x = bureau.wd + bureau.xd - w - 5;
	if (x < bureau.xd + 5)
		x = adresse->ob_x = bureau.xd + 5;
	if (y + h > bureau.yd + bureau.hd - 5)
		y = adresse->ob_y = bureau.hd + bureau.yd - h - 5;
	if (y < bureau.yd + 5)
		y = adresse->ob_y = bureau.yd + 5;

	if (h > bureau.hd)													/* Si plus haut que le bureau */
	{
		if (y < bureau.yd)
			y = adresse->ob_y = bureau.yd + 15;

		naff = create_scroll (adresse, y, &nbre, ZERO);		/* cr‚er un scroll-menu */
		down = naff + 2;
		old_adresse = adresse;
		adresse = scroll;
		adresse->ob_x = x;
		adresse->ob_y = y;
		w = adresse->ob_width;
		h = adresse->ob_height;
		haut = adresse[1].ob_height;
		fl_scroll = TRUE;
	}

	graf_mouse (ARROW, 0);							/* Souris -> Flˆche */
	wind_update (BEG_MCTRL);						/* Bloquer menu */
	if (f3d)
	{
		wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
		if (top)	/* Restaurer provisoirement la palette par d‚faut */
			set_palette (BLANK);
		fill_tab (jaune, 3, 1000, 1000, ZERO);	/* ...Mais fixer le jaune et le noir */
		fill_tab (noir, 3, ZERO, ZERO, ZERO);
		vq_color (work_display.handle, 6, 1, old_jaune);	/* Sauver l'ancien "jaune" */
		vq_color (work_display.handle, 15, 1, old_noir);	/* Sauver l'ancien "noir" */
		vs_color (work_display.handle, 6, jaune);					/* Fixer le jaune */
		vs_color (work_display.handle, 15, noir);					/* Fixer le noir */
		vsl_width (work_display.handle, 1);				/* Traits normaux */
		vsl_ends (work_display.handle, SQUARE, SQUARE);
		vsl_type (work_display.handle, SOLID);
		vsl_color (work_display.handle, BLACK);
		vst_font (work_display.handle, 1);
		vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &dummy, &dummy);
		vst_effects (work_display.handle, EFNONE);		/* D‚sactiver tous les effets */
		vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);	/* Align‚ en haut … gauche */
		vst_color (work_display.handle, 15);	/* Texte en noir */
		vsf_perimeter (work_display.handle, FALSE);
		vsf_interior (work_display.handle, FIS_SOLID);
		if (work_display.n_color >= 16)
			vsf_color (work_display.handle, YELLOW);
		else
			vsf_color (work_display.handle, WHITE);
	}
	get_bkgr (x, y, w, h, &img);
	objc_draw (adresse, ROOT, MAX_DEPTH, x - 3, y - 3, w + 6, h + 6);

	do
	{
		evnt = evnt_multi (MU_BUTTON | MU_TIMER,
											 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
											 b, 10, 0, &xm, &ym, &km, &dummy, &dummy, &dummy);
		if (evnt & MU_BUTTON)
		{
			while (km)	/* Attend que le bouton de la souris soit relƒch‚ */
				graf_mkstate (&dummy, &dummy, &km, &dummy);
			sortie = TRUE;
		}
		else if (evnt == MU_TIMER)
		{
			graf_mkstate (&xm, &ym, &dummy, &dummy);
			ob = objc_find (adresse, ROOT, MAX_DEPTH, xm, ym);
			if (old_ob != ob)		/* Si objet diff‚rent */
			{
				if ((old_ob != -1) && (NOT (adresse[old_ob].ob_state & DISABLED)))
				{
					etat = adresse[old_ob].ob_state & 0xFF04;
					objc_change (adresse, old_ob, 0, x, y, w, h, etat, FALSE);
					objc_offset (adresse, old_ob, &xs, &ys);
					objc_draw (adresse, ROOT, MAX_DEPTH, xs, ys,
										 adresse[old_ob].ob_width, adresse[old_ob].ob_height);
				}
				if ((ob > ROOT) && (NOT (adresse[ob].ob_state & DISABLED)))
				{
					if ((adresse[ob].ob_type & 0xFF) == G_STRING)
					{
						if (((adresse->ob_spec.obspec.fillpattern != ZERO) ||
									(adresse->ob_spec.obspec.interiorcol != ZERO)) &&
									(f3d))
						{
							strcpy (chaine, get_text (adresse, ob));
							objc_offset (adresse, ob, &xs, &ys);
							v_hide_c (work_display.handle);
							vswr_mode (work_display.handle, MD_REPLACE);	/* Dessin En Remplacement */
							fill_tab (cadre, 4,
												xs, ys,
												xs + adresse[ob].ob_width - 1,
												ys + adresse[ob].ob_height - 1);
							v_bar (work_display.handle, cadre);				/* Rectangle */
							fill_tab (cadre, 6,
												xs, ys + adresse[ob].ob_height - 1,
												xs, ys,
												xs + adresse[ob].ob_width - 1, ys);
							v_pline (work_display.handle, 3, cadre);	/* Ligne d'ombre noire */
							vswr_mode (work_display.handle, MD_TRANS);	/* Texte en Transparent */
							v_gtext (work_display.handle, xs, ys, chaine);	/* Ecrire le texte */
							if (adresse[ob].ob_state & CHECKED)								/* Si option check‚e, */
								v_gtext (work_display.handle, xs + 2, ys, "");	/* mettre une marque. */
							v_show_c (work_display.handle, TRUE);
							etat = adresse[ob].ob_state;
						}
						else
						{
							etat = adresse[ob].ob_state | SELECTED;
							objc_change (adresse, ob, 0, x, y, w, h, etat, TRUE);
						}
					}
					else if ((adresse[ob].ob_type & 0xFF) == G_IMAGE)
					{
						etat = adresse[ob].ob_state | SELECTED;
						objc_change (adresse, ob, 0, x, y, w, h, etat, TRUE);
					}
				}
				old_ob = ob;
			}
			else								/* Si mˆme objet */
			{
				if ((fl_scroll) && (ob == 1))
				{		/* Si scroll : flˆche vers le haut */
					objc_offset (adresse, 0, &xs, &ys);
					if (cale > 1)
					{
						evnt_timer (50, 0);
						cale--;
						for (i = naff + 1 ; i > 2 ; i--)
						{
							adresse[i].ob_spec = adresse[i - 1].ob_spec;
							adresse[i].ob_state = adresse[i - 1].ob_state;
						}
						adresse[2].ob_spec = old_adresse[cale].ob_spec;
						adresse[2].ob_state = old_adresse[cale].ob_state;
						fill_tab (pxy, 8,
											xs, ys + haut,
											xs + adresse->ob_width, ys + (naff * haut),
											xs, ys + (2 * haut),
											xs + adresse->ob_width, ys + ((naff + 1) * haut));
						v_hide_c (work_display.handle);
						objc_change (adresse, ob, 0, x, y, w, h, NORMAL, TRUE);
						vro_cpyfm (work_display.handle, S_ONLY, pxy, &nul, &nul);
						objc_change (adresse, ob, 0, x, y, w, h, SELECTED, TRUE);
						v_show_c (work_display.handle, TRUE);
						objc_draw (adresse, ROOT, 2,
											 x + adresse[2].ob_x, y + adresse[2].ob_y,
											 adresse[2].ob_width, adresse[2].ob_height);
					}
				}
				else if ((fl_scroll) && (ob == down))
				{		/* Si scroll : flˆche vers le bas */
					objc_offset (adresse, 0, &xs, &ys);
					if ((cale + naff) <= nbre)
					{
						evnt_timer (50, 0);
						cale++;
						for (i = 2 ; i < (naff + 1) ; i++)
						{
							adresse[i].ob_spec = adresse[i + 1].ob_spec;
							adresse[i].ob_state = adresse[i + 1].ob_state;
						}
						adresse[naff + 1].ob_spec = old_adresse[cale + naff - 1].ob_spec;
						adresse[naff + 1].ob_state = old_adresse[cale + naff - 1].ob_state;
						fill_tab (pxy, 8,
											xs, ys + (2 * haut),
											xs + adresse->ob_width, ys + ((naff + 1) * haut),
											xs, ys + haut,
											xs + adresse->ob_width, ys + (naff * haut));
						v_hide_c (work_display.handle);
						objc_change (adresse, ob, 0, x, y, w, h, NORMAL, TRUE);
						vro_cpyfm (work_display.handle, S_ONLY, pxy, &nul, &nul);
						objc_change (adresse, ob, 0, x, y, w, h, SELECTED, TRUE);
						v_show_c (work_display.handle, TRUE);
						objc_draw (adresse, ROOT, 2,
											 x + adresse[naff + 1].ob_x, y + adresse[naff + 1].ob_y,
											 adresse[naff + 1].ob_width, adresse[naff + 1].ob_height);
					}
				}
			}
		}
	} while (NOT sortie);

	put_bkgr (x, y, w, h, &img);
	if (f3d)
	{
		vs_color (work_display.handle, 6, old_jaune);	/* Restaurer le jaune */
		vs_color (work_display.handle, 15, old_noir);	/* Restaurer le noir */
		if (top)
		{		/* Restaurer la palette courante */
			index = find_index (top);
			set_palette (index);
		}
	}
	wind_update (END_MCTRL);								/* D‚bloquer menu */
	wind_mouse ();
	if (fl_scroll)
	{
		free (scroll);
		scroll = (OBJECT *) ZERO;
		adresse = old_adresse;
		if (ob > ZERO)
			ob = ob - 2 + cale;
	}

	if ((ob > 0) && (NOT (adresse[ob].ob_state & DISABLED)))
	{
		buf[0] = BEV_FREEPU;
		buf[1] = ap_id;
		buf[3] = tree;
		buf[4] = ob;
		etat = adresse[buf[4]].ob_state & 0xFF00;
		objc_change (adresse, buf[4], 0, x, y, w, h, etat, FALSE);
		if ((adresse[buf[4]].ob_type >> 8) == B_FNCP)	/* Si fonction associ‚e */
		{
			(*fnc[adresse[buf[4]].ob_state >> 8])();
			empty (MU_KEYBD);
		}
		return TRUE;
	}
	else
		return FALSE;
}
/* #] free_popup () Gestion d'un Pop-Up librement positionn‚ :		*/ 

