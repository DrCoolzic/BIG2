/*! @file BIG2EVTW.C
@brief implementation of functions handling windows events  

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
#include "BIG2EVTW.H"
#include "BIG2EDIT.H"
#include "BIG2WIND.H"
#include "BIG2WINB.H"

GRECT zclip;		/* Zone de clipping … redessiner si redraw */
/****** Fonctions d'ev‚nements fenˆtre ****************************/
/* #[ wm_arrow () GŠre message de flˆche ou ascenseur fenˆtre :		*/
void wm_arrow (void)
{
int index, message;

	index = find_index (buf[3]);
	if (index > BLANK)
	{
		message = buf[4];

		switch (message)
		{
		case WA_UPPAGE :
			(*win[index].w_uppage)();
			break;
		case WA_DNPAGE :
			(*win[index].w_dnpage)();
			break;
		case WA_UPLINE :
			(*win[index].w_upline)();
			break;
		case WA_DNLINE :
			(*win[index].w_dnline)();
			break;
		case WA_LFPAGE :
			(*win[index].w_lfpage)();
			break;
		case WA_RTPAGE :
			(*win[index].w_rtpage)();
			break;
		case WA_LFLINE :
			(*win[index].w_lfline)();
			break;
		case WA_RTLINE :
			(*win[index].w_rtline)();
			break;
		}
	}
}
/* #] wm_arrow () GŠre message de flˆche ou ascenseur fenˆtre :		*/ 
/* #[ redraw () Redraw :																					*/
void redraw (void)
{
GRECT r, rd, z;
MFDB nul = {0};
int index, pxy[8], xw, yw, ww, hw, dummy, color[2] = {BLACK, WHITE};

	index = find_index (buf[3]);
	if (index == BLANK)
		return;

	empty (MU_KEYBD);
	rd.g_x = buf[4];
	rd.g_y = buf[5];
	rd.g_w = buf[6];
	rd.g_h = buf[7];

	v_hide_c (work_display.handle);
	graf_mkstate (&dummy, &dummy, &mk, &dummy);	/* Bouton souris */
	if (! mk)
		wind_update (BEG_UPDATE); 	/* Bloquer les fonctions de la souris */

	zone_work (index, &xw, &yw, &ww, &hw);
	zone_work (index, &z.g_x, &z.g_y, &z.g_w, &z.g_h);
		/* Demande les coord. et dimensions du 1ø rectangle de la liste */
	wind_get (buf[3], WF_FIRSTXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h);
	while (r.g_w && r.g_h)			/* Tant qu'il y a largeur ou hauteur... */
	{
		if (rc_intersect (&rd, &r))
		{
			if (NOT ((win[index].w_type & 0xFF) == WTYP_PICT))
				objc_draw (win[index].w_fen, ROOT, MAX_DEPTH, r.g_x, r.g_y, r.g_w, r.g_h);
			else
			{
				if (NOT (win[index].w_cont.w_img.fd_addr))
					objc_draw (win[index].w_fen, ROOT, MAX_DEPTH, r.g_x, r.g_y, r.g_w, r.g_h);
				else
					draw_wind (index, &r);
			}

			if ((win[index].w_icon == index) && (win[index].w_redicn))
			{
				clipp (TRUE, &r);
				(*win[index].w_redicn)();
				clipp (FALSE, &r);
			}
			else
			{
				if (win[index].w_type & WTYP_MENU)
					objc_draw (win[index].w_bar, WM_BOXMENU, MAX_DEPTH,
										 r.g_x, r.g_y, r.g_w, r.g_h);
				else if (win[index].w_type & WTYP_TOOL)
					objc_draw (win[index].w_bar, ROOT, MAX_DEPTH,
										 r.g_x, r.g_y, r.g_w, r.g_h);
  
				rc_intersect (&z, &r);
				if ((r.g_w > ZERO) && (r.g_h > ZERO))
				{
					if ((win[index].w_type & 0xFF) == WTYP_FORM)
					{
						draw_curs (win[index].w_cont.w_form.w_tree,
											 win[index].w_cont.w_form.w_edit,
											 &win[index].w_cont.w_form.w_pos, ED_END);
						objc_draw (win[index].w_cont.w_form.w_tree, ROOT, MAX_DEPTH,
											 r.g_x, r.g_y, r.g_w, r.g_h);
						draw_curs (win[index].w_cont.w_form.w_tree,
											 win[index].w_cont.w_form.w_edit,
											 &win[index].w_cont.w_form.w_pos, ED_END);
					}
					else if ((win[index].w_type & 0xFF) == WTYP_PICT)
					{
						if (win[index].w_cont.w_img.fd_addr)
						{
							fill_tab (pxy, 8,
												(int)win[index].w_col + (r.g_x - xw),
												(int)win[index].w_lin + (r.g_y - yw),
												(int)win[index].w_col + (r.g_x - xw) + r.g_w - 1,
												(int)win[index].w_lin + (r.g_y - yw) + r.g_h - 1,
												r.g_x, r.g_y,
												r.g_x + r.g_w - 1,
												r.g_y + r.g_h - 1);
							if (win[index].w_cont.w_img.fd_nplanes == 1)
								vrt_cpyfm (work_display.handle, 1, pxy, &win[index].w_cont.w_img, &nul, color);
							else
								vro_cpyfm (work_display.handle, S_ONLY, pxy, &win[index].w_cont.w_img, &nul);
						}
					}
					else
					{
						if ((r.g_w > ZERO) && (r.g_h > ZERO))
						{
							zclip.g_x = r.g_x;
							zclip.g_y = r.g_y;
							zclip.g_w = r.g_w;
							zclip.g_h = r.g_h;
							clipp (TRUE, &r);
							(*win[index].w_redraw)();
							clipp (FALSE, &r);
						}
					}
				}
  		}
		}
			/* Rectangle suivant */
		wind_get (buf[3], WF_NEXTXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h);
	}
	if (! mk)
		wind_update (END_UPDATE); 	/* D‚bloquer les fonctions de la souris */
	v_show_c (work_display.handle, TRUE);
}
/* #] redraw () Redraw :																					*/ 
/* #[ topped () Mettre une fenˆtre en premier plan :							*/
void topped (void)
{
int index;

	index = find_index (buf[3]);							/* Chercher l'index BIG */
	if (index == BLANK)
		return;

	wind_set (win[index].w_hg, WF_TOP);
	if (((win[index].w_type & 0xFF) == WTYP_PICT)
		|| ((win[index].w_type & 0xFF) == WTYP_FORM))
		set_palette (index);
	else
		set_palette (BLANK);
}
/* #] topped () Mettre une fenˆtre en premier plan :							*/ 
/* #[ closed () Fermer une fenˆtre :															*/
void closed (void)
{
int index, top, dummy;

	index = find_index (buf[3]);							/* Chercher l'index BIG */
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & CLOSER))
		return ;
	if (buf[0] != AC_CLOSE)						/* Si AC_CLOSE : d‚j… ferm‚e */
	{
		if (win[index].w_icon > BLANK)					/* Si ic“nifi‚e */
			uniconise (index);
		wind_close (buf[3]);										/* Fermer la fenˆtre */
		wind_delete (buf[3]);										/* D‚truire la fenˆtre */
	}
	win[index].w_hg = ZERO;										/* Enregistrer la fermeture */
	win[index].w_flags &= ~WFFULL;						/* Annuler flag pleine ouverture */
	if (win[index].w_type == WTYP_FORM)				/* Si fenˆtre formulaire */
	{
		if (buf[0] != BEV_WFORM)
		{
			res_rsc (win[index].w_cont.w_form.w_tree,
							 &win[index].w_cont.w_form.w_bak);	/* Restaurer ‚tat form. */
			free (win[index].w_cont.w_form.w_bak);			/* Lib‚rer la m‚moire ‚tat */
		}
	}
	if (win[index].w_type & WTYP_MENU)					/* Si fenˆtre avec menu */
		free (win[index].w_bar);									/* Lib‚rer la m‚moire menu */
	else if (win[index].w_type & WTYP_TOOL)			/* Si fenˆtre toolbar */
		win[index].w_bar = (OBJECT *)ZERO;				/* Annuler pointeur */

	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	if (top)
	{
		index = find_index (top);
		if (index > BLANK)
		{
			if (((win[index].w_type & 0xFF) == WTYP_PICT)
			|| ((win[index].w_type & 0xFF) == WTYP_FORM))
				set_palette (index);
			else
				set_palette (BLANK);
		}
		else
			set_palette (BLANK);
	}
	else
		set_palette (BLANK);
}
/* #] closed () Fermer une fenˆtre :															*/ 
/* #[ fulled () Fenˆtre plein pot :																*/
void	fulled (void)
{
int index, oldx, oldy, oldw, oldh, x, y, w, h, xb, yb, xw, yw, ww, hw;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & FULLER))
		return ;

	oldx = win[index].w_fen->ob_x;
	oldy = win[index].w_fen->ob_y;
	oldw = win[index].w_fen->ob_width;
	oldh = win[index].w_fen->ob_height;
	if (win[index].w_flags & WFFULL)
	{
		x = win[index].w_old.w_coord.g_x;
		y = win[index].w_old.w_coord.g_y;
		w = win[index].w_old.w_coord.g_w;
		h = win[index].w_old.w_coord.g_h;
		win[index].w_fen->ob_x = x;
		win[index].w_fen->ob_y = y;
		graf_mouse (ARROW, ZERO);
	}
	else
	{
		win[index].w_old.w_coord.g_x = win[index].w_curr.g_x;
		win[index].w_old.w_coord.g_y = win[index].w_curr.g_y;
		win[index].w_old.w_coord.g_w = win[index].w_curr.g_w;
		win[index].w_old.w_coord.g_h = win[index].w_curr.g_h;
		x = win[index].w_fen->ob_x = bureau.xd;
		y = win[index].w_fen->ob_y = bureau.yd;
		w = min (bureau.wd, win[index].w_wmaxi);
		h = min (bureau.hd, win[index].w_hmaxi);
		if ((win[index].w_type & 0xFF) == WTYP_PICT)
		{		/* Si fenˆtre image : pas plus grande que la taille totale */
			bwind_calc (WC_WORK, win[index].w_attr, BLANK, win[index].w_bar,
									x, y, w, h, &xw, &yw, &ww, &hw);
			if (ww > win[index].w_wtot)
				ww = (int)win[index].w_wtot;
			if (hw > win[index].w_htot)
				hw = (int)win[index].w_htot;
			if ((win[index].w_type & WTYP_TOOL) || (win[index].w_type & WTYP_MENU))
				hw++;
			bwind_calc (WC_BORDER, win[index].w_attr, BLANK, win[index].w_bar,
									xw, yw, ww, hw, &x, &y, &w, &h);
		}
		if (win[index].w_flags & WFCALAG)
		{
			bwind_calc (WC_WORK, win[index].w_attr, BLANK, win[index].w_bar,
									x, y, w, h, &xw, &yw, &ww, &hw);
			ww = ww / win[index].w_wunit * win[index].w_wunit;
			hw = hw / win[index].w_hunit * win[index].w_hunit;
			bwind_calc (WC_BORDER, win[index].w_attr, BLANK, win[index].w_bar,
									xw, yw, ww, hw, &x, &y, &w, &h);
		}
	}
	resize (index, w - oldw, h - oldh);
	win[index].w_flags ^= WFFULL;
	wind_set (win[index].w_hg, WF_CURRXYWH, x, y, w, h);
	wind_get (win[index].w_hg, WF_CURRXYWH,
						&win[index].w_curr.g_x, &win[index].w_curr.g_y,
						&win[index].w_curr.g_w, &win[index].w_curr.g_h);
	win[index].w_fen->ob_width = win[index].w_curr.g_w;
	win[index].w_fen->ob_height = win[index].w_curr.g_h;
	height_sliders (index, FALSE);
	if (((oldx == win[index].w_curr.g_x) && (oldy == win[index].w_curr.g_y)) &&
			((oldw > win[index].w_curr.g_w) || (oldh > win[index].w_curr.g_h)))
		print_wind (index);

	objc_offset (win[index].w_fen, BW_BARRE, &xb, &yb);
	if ((win[index].w_type & WTYP_MENU) || (win[index].w_type & WTYP_TOOL))
	{
		win[index].w_bar->ob_x = xb;
		win[index].w_bar->ob_y = yb;
	}
	wind_mouse ();
}
/* #] fulled () Fenˆtre plein pot :																*/ 
/* #[ uppage () Page vers le haut fenˆtre :												*/
void uppage (void)
{
int index, xw, yw, ww, hw;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & VSLIDE))
		return;
	zone_work (index, &xw, &yw, &ww, &hw);
	if (win[index].w_lin > ZERO)	/* Si on n'est pas d‚j… au d‚but */
	{
		win[index].w_lin = max (win[index].w_lin - hw, ZERO);
		if (win[index].w_lin < ZERO)
			win[index].w_lin = ZERO;
		if ((win[index].w_type & 0xFF) == WTYP_NORM)
			win[index].w_lin = ((win[index].w_lin + (win[index].w_hunit / 2)) / win[index].w_hunit) * win[index].w_hunit;
		if ((win[index].w_type & 0xFF) == WTYP_FORM)
			win[index].w_cont.w_form.w_tree->ob_y = yw - (int)win[index].w_lin;
		print_page (index);
		posi_vert (index, TRUE);
	}
}
/* #] uppage () Page vers le haut fenˆtre :												*/ 
/* #[ dnpage () Page vers le bas fenˆtre :												*/
void dnpage (void)
{
int index, xw, yw, ww, hw;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & VSLIDE))
		return;
	zone_work (index, &xw, &yw, &ww, &hw);
	if ((win[index].w_lin + hw) < win[index].w_htot)	/* Si pas d‚j… … la fin */
	{
		win[index].w_lin = min (win[index].w_lin + hw, win[index].w_htot - hw);
		if ((win[index].w_type & 0xFF) == WTYP_NORM)
		{
			win[index].w_lin = ((win[index].w_lin + (win[index].w_hunit / 2)) / win[index].w_hunit) * win[index].w_hunit;
			if (win[index].w_lin + hw + win[index].w_hunit >= win[index].w_htot)
				win[index].w_lin += win[index].w_hunit;
		}
		if ((win[index].w_type & 0xFF) == WTYP_FORM)
			win[index].w_cont.w_form.w_tree->ob_y = yw - (int)win[index].w_lin;
		print_page (index);
		posi_vert (index, TRUE);
	}
}
/* #] dnpage () Page vers le bas fenˆtre :												*/ 
/* #[ upline () Ligne vers le haut fenˆtre :											*/
void upline (void)
{
int index, top, pxy[8], xw, yw, ww, hw, dummy, decal;
MFDB ecr = {0};

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & UPARROW))
		return;
	zone_work (index, &xw, &yw, &ww, &hw);
	if (hw <= win[index].w_hunit)
	{
		uppage ();
		return;
	}
	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	if (win[index].w_lin > ZERO)	/* Si on n'est pas d‚j… au d‚but */
	{
		if ((win[index].w_type & 0xFF) != WTYP_NORM)
			decal = (int)min ((long)win[index].w_hunit, win[index].w_lin);
		else
			decal = win[index].w_hunit;
		if (xw < bureau.xd)
		{
			ww -= (bureau.xd - xw);
			xw = bureau.xd;
		}
		ww = min (ww, bureau.xd + bureau.wd - xw);
		hw = min (hw, bureau.yd + bureau.hd - yw);

		if ((win[index].w_type & 0xFF) == WTYP_PICT)
		{
			win[index].w_lin -= decal;
			print_page (index);
			posi_vert (index, TRUE);
			return;
		}

		if (win[index].w_hg == top)
		{
			fill_tab (pxy, 8,
								xw, yw + win[index].w_vprot,
								xw + ww - 1, yw + hw - decal - 1,
								xw, yw + win[index].w_vprot + decal,
								xw + ww - 1, yw + hw - 1);
			v_hide_c (work_display.handle);
			vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecr, &ecr);
			v_show_c (work_display.handle, TRUE);
			win[index].w_lin -= decal;
			if (win[index].w_lin < ZERO)
				win[index].w_lin = ZERO;
			if ((win[index].w_type & 0xFF) == WTYP_FORM)
				win[index].w_cont.w_form.w_tree->ob_y = yw - (int)win[index].w_lin;
			print_line (FALSE, index);
		}
		else
		{
			win[index].w_lin -= decal;
			if (win[index].w_lin < ZERO)
				win[index].w_lin = ZERO;
			if ((win[index].w_type & 0xFF) == WTYP_FORM)
				win[index].w_cont.w_form.w_tree->ob_y = yw - (int)win[index].w_lin;
			print_page (index);
		}
		posi_vert (index, TRUE);
	}
}
/* #] upline () Ligne vers le haut fenˆtre :											*/ 
/* #[ dnline () Ligne vers le bas fenˆtre :												*/
void dnline (void)
{
int index, top, pxy[8], xw, yw, ww, hw, dummy, decal;
MFDB ecr = {0};

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & DNARROW))
		return;
	zone_work (index, &xw, &yw, &ww, &hw);
	if (hw <= win[index].w_hunit)
	{
		dnpage ();
		return;
	}
	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	if ((win[index].w_lin + hw) < win[index].w_htot)	/* Si pas d‚j… … la fin */
	{
		if ((win[index].w_type & 0xFF) != WTYP_NORM)
			decal = (int)min ((long)win[index].w_hunit,
												win[index].w_htot - (win[index].w_lin + (long)hw));
		else
			decal = win[index].w_hunit;
		if (xw < bureau.xd)
		{
			ww -= (bureau.xd - xw);
			xw = bureau.xd;
		}
		ww = min (ww, bureau.xd + bureau.wd - xw);
		hw = min (hw, bureau.yd + bureau.hd - yw);

		if ((win[index].w_type & 0xFF) == WTYP_PICT)
		{
			win[index].w_lin += decal;
			print_page (index);
			posi_vert (index, TRUE);
			return;
		}

		if (win[index].w_hg == top)
		{
			fill_tab (pxy, 8,
								xw, yw + win[index].w_vprot + decal,
								xw + ww - 1, yw + hw - 1,
								xw, yw + win[index].w_vprot ,
								xw + ww - 1, yw + hw - 1 - decal);
			v_hide_c (work_display.handle);
			vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecr, &ecr);
			v_show_c (work_display.handle, TRUE);
			win[index].w_lin += decal;
			if ((win[index].w_type & 0xFF) != WTYP_NORM)
				if (win[index].w_lin + hw > win[index].w_htot)
					win[index].w_lin = win[index].w_htot - hw;
			if ((win[index].w_type & 0xFF) == WTYP_FORM)
				win[index].w_cont.w_form.w_tree->ob_y = yw - (int)win[index].w_lin;
			print_line (TRUE, index);
		}
		else
		{
			win[index].w_lin += decal;
			if ((win[index].w_type & 0xFF) != WTYP_NORM)
				if (win[index].w_lin + hw > win[index].w_htot)
					win[index].w_lin = win[index].w_htot - hw;
			if ((win[index].w_type & 0xFF) == WTYP_FORM)
				win[index].w_cont.w_form.w_tree->ob_y = yw - (int)win[index].w_lin;
			print_page (index);
		}
		posi_vert (index, TRUE);
	}
}
/* #] dnline () Ligne vers le bas fenˆtre :												*/ 
/* #[ lfpage () Page vers la gauche fenˆtre :											*/
void lfpage (void)
{
int index, xw, yw, ww, hw;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & HSLIDE))
		return;
	zone_work (index, &xw, &yw, &ww, &hw);
	if (win[index].w_col > ZERO)	/* Si on n'est pas d‚j… au d‚but */
	{
		win[index].w_col = max (win[index].w_col - ww, ZERO);
		if (win[index].w_col < ZERO)
			win[index].w_col = ZERO;
		if ((win[index].w_type & 0xFF) == WTYP_NORM)
			win[index].w_col = ((win[index].w_col + (win[index].w_wunit / 2)) / win[index].w_wunit) * win[index].w_wunit;
		if ((win[index].w_type & 0xFF) == WTYP_FORM)
			win[index].w_cont.w_form.w_tree->ob_x = xw - (int)win[index].w_col;
		print_page (index);
		posi_hori (index, TRUE);
	}
}
/* #] lfpage () Page vers la gauche fenˆtre :											*/ 
/* #[ rtpage () Page vers la droite fenˆtre :											*/
void rtpage (void)
{
int index, xw, yw, ww, hw;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & HSLIDE))
		return;
	zone_work (index, &xw, &yw, &ww, &hw);
	if ((win[index].w_col + ww) < win[index].w_wtot)	/* Si pas d‚j… … la fin */
	{
		win[index].w_col = min (win[index].w_col + ww, win[index].w_wtot - ww);
		if ((win[index].w_type & 0xFF) != WTYP_NORM)
			if (win[index].w_col + ww > win[index].w_wtot)
				win[index].w_col = win[index].w_wtot - ww;
		if ((win[index].w_type & 0xFF) == WTYP_NORM)
		{
			win[index].w_col = ((win[index].w_col + (win[index].w_wunit / 2)) / win[index].w_wunit) * win[index].w_wunit;
			if (win[index].w_col + ww + win[index].w_wunit >= win[index].w_wtot)
				win[index].w_col += win[index].w_wunit;
		}
		if ((win[index].w_type & 0xFF) == WTYP_FORM)
			win[index].w_cont.w_form.w_tree->ob_x = xw - (int)win[index].w_col;
		print_page (index);
		posi_hori (index, TRUE);
	}
}
/* #] rtpage () Page vers la droite fenˆtre :											*/ 
/* #[ lfline () Ligne vers la gauche fenˆtre formulaire :					*/
void lfline (void)
{
int index, top, pxy[8], xw, yw, ww, hw, dummy, decal, oldx;
MFDB ecr = {0};

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & LFARROW))
		return;
	zone_work (index, &xw, &yw, &ww, &hw);
	oldx = xw;
	if (ww <= win[index].w_wunit)
	{
		lfpage ();
		return;
	}
	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	if (win[index].w_col > ZERO)	/* Si on n'est pas d‚j… au d‚but */
	{
		if ((win[index].w_type & 0xFF) != WTYP_NORM)
			decal = (int)min ((long)win[index].w_wunit, win[index].w_col);
		else
			decal = win[index].w_wunit;
		if (xw < bureau.xd)
		{
			ww -= (bureau.xd - xw);
			xw = bureau.xd;
		}
		ww = min (ww, bureau.xd + bureau.wd - xw);
		hw = min (hw, bureau.yd + bureau.hd - yw);

		if ((win[index].w_type & 0xFF) == WTYP_PICT)
		{
			win[index].w_col -= decal;
			print_page (index);
			posi_hori (index, TRUE);
			return;
		}

		if (win[index].w_hg == top)
		{
			fill_tab (pxy, 8,
								xw + win[index].w_hprot, yw,
								xw + ww - 1 - decal, yw + hw - 1,
								xw + win[index].w_hprot + decal, yw,
								xw + ww - 1, yw + hw - 1);
			v_hide_c (work_display.handle);
			vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecr, &ecr);
			v_show_c (work_display.handle, TRUE);
			win[index].w_col -= decal;
			if (win[index].w_col < ZERO)
				win[index].w_col = ZERO;
			if ((win[index].w_type & 0xFF) == WTYP_FORM)
				win[index].w_cont.w_form.w_tree->ob_x = oldx - (int)win[index].w_col;
			print_colum (FALSE, index);
		}
		else
		{
			win[index].w_col -= decal;
			if (win[index].w_col < ZERO)
				win[index].w_col = ZERO;
			if ((win[index].w_type & 0xFF) == WTYP_FORM)
				win[index].w_cont.w_form.w_tree->ob_x = oldx - (int)win[index].w_col;
			print_page (index);
		}
		posi_hori (index, TRUE);
	}
}
/* #] lfline () Ligne vers la gauche fenˆtre formulaire :					*/ 
/* #[ rtline () Ligne vers la droite fenˆtre formulaire :					*/
void rtline (void)
{
int index, top, pxy[8], xw, yw, ww, hw, dummy, decal, oldx;
MFDB ecr = {0};

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & RTARROW))
		return;
	zone_work (index, &xw, &yw, &ww, &hw);
	oldx = xw;
	if (ww <= win[index].w_wunit)
	{
		rtpage ();
		return;
	}
	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	if ((win[index].w_col + ww) < win[index].w_wtot)	/* Si pas d‚j… … la fin */
	{
		if ((win[index].w_type & 0xFF) != WTYP_NORM)
			decal = (int)min ((long)win[index].w_wunit,
												win[index].w_wtot - (win[index].w_col + (long)ww));
		else
			decal = win[index].w_wunit;
		if (xw < bureau.xd)
		{
			ww -= (bureau.xd - xw);
			xw = bureau.xd;
		}
		ww = min (ww, bureau.xd + bureau.wd - xw);
		hw = min (hw, bureau.yd + bureau.hd - yw);

		if ((win[index].w_type & 0xFF) == WTYP_PICT)
		{
			win[index].w_col += decal;
			print_page (index);
			posi_hori (index, TRUE);
			return;
		}

		if (win[index].w_hg == top)
		{
			fill_tab (pxy, 8,
								xw + win[index].w_hprot + decal, yw,
								xw + ww - 1, yw + hw - 1,
								xw + win[index].w_hprot, yw,
								xw + ww - 1 - decal, yw + hw - 1);
			v_hide_c (work_display.handle);
			vro_cpyfm (work_display.handle, 3, pxy, &ecr, &ecr);
			v_show_c (work_display.handle, TRUE);
			win[index].w_col += decal;
			if ((win[index].w_type & 0xFF) != WTYP_NORM)
				if (win[index].w_col + ww > win[index].w_wtot)
					win[index].w_col = win[index].w_wtot - ww;
			if ((win[index].w_type & 0xFF) == WTYP_FORM)
				win[index].w_cont.w_form.w_tree->ob_x = oldx - (int)win[index].w_col;
			print_colum (TRUE, index);
		}
		else
		{
			win[index].w_col += decal;
			if ((win[index].w_type & 0xFF) != WTYP_NORM)
				if (win[index].w_col + ww > win[index].w_wtot)
					win[index].w_col = win[index].w_wtot - ww;
			if ((win[index].w_type & 0xFF) == WTYP_FORM)
				win[index].w_cont.w_form.w_tree->ob_x = oldx - (int)win[index].w_col;
			print_page (index);
		}
		posi_hori (index, TRUE);
	}
}
/* #] rtline () Ligne vers la droite fenˆtre formulaire :					*/ 
/* #[ hslid () Slider horizontal fenˆtre :												*/
void hslid (void)
{
int index, slide, xw, yw, ww, hw, wa;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;

	slide = win[index].w_fen[BW_HSLIDE].ob_x;
	wa = win[index].w_fen[BW_HELEV].ob_width -	/* Largeur ascenseur */
			 win[index].w_fen[BW_HSLIDE].ob_width;
	zone_work (index, &xw, &yw, &ww, &hw);
	win[index].w_col = ((long)slide * (win[index].w_wtot - (long)ww) / (long)wa);
	if ((win[index].w_type & 0xFF) != WTYP_NORM)
	{
		if (win[index].w_col + ww > win[index].w_wtot)
			win[index].w_col = win[index].w_wtot - ww;
	}
	if ((win[index].w_type & 0xFF) == WTYP_NORM)
		win[index].w_col = ((win[index].w_col + (win[index].w_wunit / 2)) / win[index].w_wunit) * win[index].w_wunit;
	if (win[index].w_col < ZERO)
		win[index].w_col = ZERO;
	if (win[index].w_flags & WFCALAG)
		win[index].w_col = win[index].w_col /
											 win[index].w_wunit * win[index].w_wunit;
	if ((win[index].w_type & 0xFF) == WTYP_FORM)
		win[index].w_cont.w_form.w_tree->ob_x = xw - (int)win[index].w_col;
	slide = (int)((long)win[index].w_col * (long)wa /
								(long)(win[index].w_wtot - ww));
	slide = max (min (slide, wa), ZERO);
	print_page (index);
}
/* #] hslid () Slider horizontal fenˆtre :												*/ 
/* #[ vslid () Slider vertical fenˆtre :													*/
void vslid (void)
{
int index, slide, xw, yw, ww, hw, ha;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;

	slide = win[index].w_fen[BW_VSLIDE].ob_y;
	ha = win[index].w_fen[BW_VELEV].ob_height -	/* Hauteur ascenseur */
			 win[index].w_fen[BW_VSLIDE].ob_height;
	zone_work (index, &xw, &yw, &ww, &hw);
	win[index].w_lin = ((long)slide * (win[index].w_htot - (long)hw) / (long)ha);
	if ((win[index].w_type & 0xFF) != WTYP_NORM)
	{
		if (win[index].w_lin + hw > win[index].w_htot)
			win[index].w_lin = win[index].w_htot - hw;
	}
	if ((win[index].w_type & 0xFF) == WTYP_NORM)
		win[index].w_lin = ((win[index].w_lin + (win[index].w_hunit / 2)) / win[index].w_hunit) * win[index].w_hunit;
	if (win[index].w_lin < ZERO)
		win[index].w_lin = ZERO;
	if (win[index].w_flags & WFCALAG)
		win[index].w_lin = win[index].w_lin /
											 win[index].w_hunit * win[index].w_hunit;
	if ((win[index].w_type & 0xFF) == WTYP_FORM)
		win[index].w_cont.w_form.w_tree->ob_y = yw - (int)win[index].w_lin;
	slide = (int)((long)win[index].w_lin * (long)ha /
								(long)(win[index].w_htot - hw));
	slide = max (min (slide, ha), ZERO);
	print_page (index);
}
/* #] vslid () Slider vertical fenˆtre :													*/ 
/* #[ sized () Redimensionnement fenˆtre :												*/
void sized (void)
{
int index, x, y, w, h, xb, yb;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	x = buf[4];
	y = buf[5];
	w = buf[6];
	h = buf[7];
	if (w < win[index].w_wmini)
		w = win[index].w_wmini;
	if (h < win[index].w_hmini)
		h = win[index].w_hmini;

	win[index].w_fen->ob_x = x;
	win[index].w_fen->ob_y = y;
	win[index].w_fen->ob_width = w;
	win[index].w_fen->ob_height = h;
	if ((win[index].w_type & WTYP_MENU) || (win[index].w_type & WTYP_TOOL))
	{
		objc_offset (win[index].w_fen, BW_BARRE, &xb, &yb);
		win[index].w_bar->ob_x = xb;
		win[index].w_bar->ob_y = yb;
	}

	wind_set (buf[3], WF_CURRXYWH, x, y, w, h);
	win[index].w_curr.g_w = w;
	win[index].w_curr.g_h = h;
	win[index].w_flags &= ~WFFULL;
	height_sliders (index, FALSE);
}
/* #] sized () Redimensionnement fenˆtre :												*/ 
/* #[ moved () D‚placement fenˆtre :															*/
void moved (void)
{
int index, xw, yw, xb, yb, xf, yf, wf, hf, dummy;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;

	xf = buf[4];
	yf = buf[5];
	wf = buf[6];
	hf = buf[7];

	win[index].w_fen->ob_x = xf;
	win[index].w_fen->ob_y = yf;
	win[index].w_curr.g_x = xf;
	win[index].w_curr.g_y = yf;
	win[index].w_flags &= ~WFFULL;

	if (desc_hard_soft.v_tos < 0x300)
		wind_set (buf[3], WF_CURRXYWH, max (xf, ZERO), yf, wf + min (ZERO, xf), hf);
	else
		wind_set (buf[3], WF_CURRXYWH, xf, yf, wf, hf);
	if ((win[index].w_type & WTYP_MENU) || (win[index].w_type & WTYP_TOOL))
	{
		objc_offset (win[index].w_fen, BW_BARRE, &xb, &yb);
		win[index].w_bar->ob_x = xb;
		win[index].w_bar->ob_y = yb;
	}
	if ((win[index].w_type & 0xFF) == WTYP_FORM)
	{
		zone_work (index, &xw, &yw, &dummy, &dummy);
		win[index].w_cont.w_form.w_tree->ob_x = xw - (int)win[index].w_col;
		win[index].w_cont.w_form.w_tree->ob_y = yw - (int)win[index].w_lin;
	}
}
/* #] moved () D‚placement fenˆtre :															*/ 
/* #[ iconise () Ic“nise une fenˆtre :														*/
void iconise (int index)
{
int i, xb, yb, xw, yw, dummy,
		tab[] = {BW_SIZERUL, BW_CLOSER, BW_SMALLER, BW_CYCLER,
						 BW_FULLER, BW_SIZERUR, BW_INFO, BW_VBAR, BW_UPARROW, BW_VELEV,
						 BW_VSLIDE, BW_DNARROW, BW_SIZERDL, BW_HBAR, BW_LFARROW, BW_HELEV,
						 BW_HSLIDE, BW_RTARROW, BW_SIZERDR};

	if (NOT (win[index].w_attr & SMALLER))
		return;

	win[index].w_old.w_att = win[index].w_fen[BW_WORK].ob_state;	/* Enregistrer couleurs */
	for (i = ZERO ; i < 19 ; i++)
		win[index].w_fen[tab[i]].ob_flags |= HIDETREE;

	win[index].w_old.w_coord.g_x = win[index].w_curr.g_x;	/* Anciennes coordonn‚es */
	win[index].w_old.w_coord.g_y = win[index].w_curr.g_y;
	win[index].w_old.w_coord.g_w = win[index].w_curr.g_w;
	win[index].w_old.w_coord.g_h = win[index].w_curr.g_h;
	win[index].w_icon = index;

	win[index].w_fen->ob_x = win[index].w_fen->ob_x + ((win[index].w_fen->ob_width - 72) / 2);
	if (win[index].w_fen->ob_x < bureau.xd)
		win[index].w_fen->ob_x = bureau.xd;
	if (win[index].w_fen->ob_x > bureau.xd + bureau.wd - 72)
		win[index].w_fen->ob_x = bureau.xd + bureau.wd - 72;
	win[index].w_fen->ob_y = bureau.yd + bureau.hd - 72;
	win[index].w_fen->ob_width = 72;
	win[index].w_fen->ob_height = 72;
	win[index].w_fen[BW_MOVER].ob_x = ZERO;
	win[index].w_fen[BW_MOVER].ob_width = 72;
	win[index].w_fen[BW_WORK].ob_y = win[index].w_fen[BW_MOVER].ob_height - 1;
	win[index].w_fen[BW_WORK].ob_width = 72;
	win[index].w_fen[BW_WORK].ob_height = 72 - win[index].w_fen[BW_MOVER].ob_height + 1;
	if ((win[index].w_type & WTYP_MENU) || (win[index].w_type & WTYP_TOOL))
	{
		objc_offset (win[index].w_fen, BW_BARRE, &xb, &yb);
		win[index].w_bar->ob_x = xb;
		win[index].w_bar->ob_y = yb;
	}

	wind_set (win[index].w_hg, WF_CURRXYWH,
						win[index].w_fen->ob_x, win[index].w_fen->ob_y,
						win[index].w_fen->ob_width, win[index].w_fen->ob_height);
	wind_get (win[index].w_hg, WF_CURRXYWH,
						&win[index].w_curr.g_x, &win[index].w_curr.g_y,
						&win[index].w_curr.g_w, &win[index].w_curr.g_h);
	if ((win[index].w_type & 0xFF) == WTYP_FORM)
	{
		zone_work (index, &xw, &yw, &dummy, &dummy);
		win[index].w_cont.w_form.w_tree->ob_x = xw;
		win[index].w_cont.w_form.w_tree->ob_y = yw;
	}

	wind_mouse ();
	set_palette (BLANK);
	win[index].w_flags &= ~WFFULL;
	print_wind (index);
}
/* #] iconise () Ic“nise une fenˆtre :														*/ 
/* #[ uniconise () D‚sic“nise une fenˆtre :												*/
void uniconise (int index)
{
int xf, yf, wf, hf, dummy, i = ZERO, top, oldy, xb, yb, xw, yw;

	xf = win[index].w_old.w_coord.g_x;
	yf = win[index].w_old.w_coord.g_y;
	wf = win[index].w_old.w_coord.g_w;
	hf = win[index].w_old.w_coord.g_h;

	win[index].w_curr.g_x = win[index].w_old.w_coord.g_x;	/* Restaurer coordonn‚es */
	win[index].w_curr.g_y = win[index].w_old.w_coord.g_y;
	win[index].w_curr.g_w = win[index].w_old.w_coord.g_w;
	win[index].w_curr.g_h = win[index].w_old.w_coord.g_h;
	win[index].w_old.w_coord.g_x = win[index].w_old.w_coord.g_y =
			win[index].w_old.w_coord.g_w = win[index].w_old.w_coord.g_h = ZERO;
	win[index].w_icon = BLANK;

	wind_set (win[index].w_hg, WF_CURRXYWH, xf, yf, wf, hf);
	put_fen (index);
	win[index].w_fen[BW_WORK].ob_state = win[index].w_old.w_att;	/* Restaurer couleurs */
	if (win[index].w_type & WTYP_MENU)
	{
		win[index].w_fen[BW_BARRE].ob_height = win[index].w_bar[WM_BOXMENU].ob_height;
		oldy = win[index].w_fen[BW_WORK].ob_y;
		win[index].w_fen[BW_WORK].ob_y = (win[index].w_fen[BW_BARRE].ob_y + win[index].w_fen[BW_BARRE].ob_height + 1);
		win[index].w_fen[BW_WORK].ob_height -= (win[index].w_fen[BW_WORK].ob_y - oldy);
		win[index].w_fen[BW_VBAR].ob_y = win[index].w_fen[BW_WORK].ob_y;
		win[index].w_fen[BW_VBAR].ob_height = win[index].w_fen[BW_WORK].ob_height;
		win[index].w_fen[BW_VELEV].ob_height -= (win[index].w_fen[BW_WORK].ob_y - oldy - 1);
		win[index].w_fen[BW_DNARROW].ob_y -= (win[index].w_fen[BW_WORK].ob_y - oldy - 1);
		objc_offset (win[index].w_fen, BW_BARRE, &xb, &yb);
		win[index].w_bar[WM_BKGR].ob_x = xb;
		win[index].w_bar[WM_BKGR].ob_y = yb;
	}
	else if (win[index].w_type & WTYP_TOOL)
	{
		win[index].w_fen[BW_BARRE].ob_height = win[index].w_bar->ob_height;
		oldy = win[index].w_fen[BW_WORK].ob_y;
		win[index].w_fen[BW_WORK].ob_y = (win[index].w_fen[BW_BARRE].ob_y + win[index].w_fen[BW_BARRE].ob_height);
		win[index].w_fen[BW_WORK].ob_height -= (win[index].w_fen[BW_WORK].ob_y - oldy);
		win[index].w_fen[BW_VBAR].ob_y = win[index].w_fen[BW_WORK].ob_y;
		win[index].w_fen[BW_VBAR].ob_height = win[index].w_fen[BW_WORK].ob_height;
		win[index].w_fen[BW_VELEV].ob_height -= (win[index].w_fen[BW_WORK].ob_y - oldy - 1);
		win[index].w_fen[BW_DNARROW].ob_y -= (win[index].w_fen[BW_WORK].ob_y - oldy - 1);
		objc_offset (win[index].w_fen, BW_BARRE, &xb, &yb);
		win[index].w_bar->ob_x = xb;
		win[index].w_bar->ob_y = yb;
	}
	else if (win[index].w_type & WTYP_FORM)
	{
		zone_work (index, &xw, &yw, &dummy, &dummy);
		win[index].w_cont.w_form.w_tree->ob_x = xw;
		win[index].w_cont.w_form.w_tree->ob_y = yw;
	}
	if (win[index].w_flags & WFGROUP)
	{
		win[index].w_fen[BW_UPARROW].ob_y =
				(win[index].w_fen[BW_DNARROW].ob_y - win[index].w_fen[BW_UPARROW].ob_height + 1);
		win[index].w_fen[BW_LFARROW].ob_x =
				(win[index].w_fen[BW_RTARROW].ob_x - win[index].w_fen[BW_LFARROW].ob_width + 1);
	}
	height_sliders (index, FALSE);

	do						/* Pour toutes les fenˆtres */
	{
		if (win[i].w_icon == index)	/* Si ic“ne associ‚e */
		{
			win[i].w_curr.g_x = win[i].w_old.w_coord.g_x;	/* Restaurer coordonn‚es */
			win[i].w_curr.g_y = win[i].w_old.w_coord.g_y;
			win[i].w_curr.g_w = win[i].w_old.w_coord.g_w;
			win[i].w_curr.g_h = win[i].w_old.w_coord.g_h;
			win[i].w_old.w_coord.g_x = win[i].w_old.w_coord.g_y =
					win[i].w_old.w_coord.g_w = win[i].w_old.w_coord.g_h = ZERO;
			win[i].w_icon = BLANK;

			win[i].w_hg = wind_create (ZERO,
																 win[i].w_curr.g_x, win[i].w_curr.g_y,
																 win[i].w_curr.g_w, win[i].w_curr.g_h);
			if (win[i].w_hg)
			{
				put_fen (i);
				win[i].w_fen[BW_WORK].ob_state = win[i].w_old.w_att;	/* Restaurer couleurs */
				if (win[i].w_type & WTYP_MENU)
				{
					win[i].w_fen[BW_BARRE].ob_height = win[i].w_bar[WM_BOXMENU].ob_height;
					oldy = win[i].w_fen[BW_WORK].ob_y;
					win[i].w_fen[BW_WORK].ob_y = (win[i].w_fen[BW_BARRE].ob_y + win[i].w_fen[BW_BARRE].ob_height + 1);
					win[i].w_fen[BW_WORK].ob_height -= (win[i].w_fen[BW_WORK].ob_y - oldy);
					win[i].w_fen[BW_VBAR].ob_y = win[i].w_fen[BW_WORK].ob_y;
					win[i].w_fen[BW_VBAR].ob_height = win[i].w_fen[BW_WORK].ob_height;
					win[i].w_fen[BW_VELEV].ob_height -= (win[i].w_fen[BW_WORK].ob_y - oldy - 1);
					win[i].w_fen[BW_DNARROW].ob_y -= (win[i].w_fen[BW_WORK].ob_y - oldy - 1);
					objc_offset (win[i].w_fen, BW_BARRE, &xb, &yb);
					win[i].w_bar[WM_BKGR].ob_x = xb;
					win[i].w_bar[WM_BKGR].ob_y = yb;
				}
				else if (win[i].w_type & WTYP_TOOL)
				{
					win[i].w_fen[BW_BARRE].ob_height = win[i].w_bar->ob_height;
					oldy = win[i].w_fen[BW_WORK].ob_y;
					win[i].w_fen[BW_WORK].ob_y = (win[i].w_fen[BW_BARRE].ob_y + win[i].w_fen[BW_BARRE].ob_height);
					win[i].w_fen[BW_WORK].ob_height -= (win[i].w_fen[BW_WORK].ob_y - oldy);
					win[i].w_fen[BW_VBAR].ob_y = win[i].w_fen[BW_WORK].ob_y;
					win[i].w_fen[BW_VBAR].ob_height = win[i].w_fen[BW_WORK].ob_height;
					win[i].w_fen[BW_VELEV].ob_height -= (win[i].w_fen[BW_WORK].ob_y - oldy - 1);
					win[i].w_fen[BW_DNARROW].ob_y -= (win[i].w_fen[BW_WORK].ob_y - oldy - 1);
					objc_offset (win[i].w_fen, BW_BARRE, &xb, &yb);
					win[i].w_bar->ob_x = xb;
					win[i].w_bar->ob_y = yb;
				}
				else if (win[i].w_type & WTYP_FORM)
				{
					zone_work (i, &xw, &yw, &dummy, &dummy);
					win[i].w_cont.w_form.w_tree->ob_x = xw;
					win[i].w_cont.w_form.w_tree->ob_y = yw;
				}
				if (win[index].w_flags & WFGROUP)
				{
					win[index].w_fen[BW_UPARROW].ob_y =
							(win[index].w_fen[BW_DNARROW].ob_y - win[index].w_fen[BW_UPARROW].ob_height + 1);
					win[index].w_fen[BW_LFARROW].ob_x =
							(win[index].w_fen[BW_RTARROW].ob_x - win[index].w_fen[BW_LFARROW].ob_width + 1);
				}
				height_sliders (i, FALSE);
				wind_open (win[i].w_hg, win[i].w_curr.g_x, win[i].w_curr.g_y,
									 win[i].w_curr.g_w, win[i].w_curr.g_h);
			}
		}
	} while (NOT (win[i++].w_flags & WFLAST));

	wind_mouse ();
	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	index = find_index (top);
	if (index > BLANK)
	{
		if (((win[index].w_type & 0xFF) == WTYP_PICT)
		|| ((win[index].w_type & 0xFF) == WTYP_FORM))
			set_palette (index);
		else
			set_palette (BLANK);
	}
}
/* #] uniconise () D‚sic“nise une fenˆtre :												*/ 
/* #[ alliconise () Ic“nise une fenˆtre :													*/
void alliconise (int index)
{
int i = ZERO;

	if (NOT (win[index].w_attr & SMALLER))
		return;

	iconise (index);	/* Ic“niser cette fenˆtre */

	do								/* Pour toutes les fenˆtres */
	{
		if (win[i].w_attr & SMALLER)
		{
			if ((win[i].w_hg > ZERO) && (win[i].w_icon == BLANK))	/* Si ouverte et pas ic“ne */
			{
				wind_close (win[i].w_hg);
				wind_delete (win[i].w_hg);
				win[i].w_hg = ZERO;
				win[i].w_icon = index;
				win[i].w_old.w_coord.g_x = win[i].w_curr.g_x;	/* Anciennes coordonn‚es */
				win[i].w_old.w_coord.g_y = win[i].w_curr.g_y;
				win[i].w_old.w_coord.g_w = win[i].w_curr.g_w;
				win[i].w_old.w_coord.g_h = win[i].w_curr.g_h;
				win[i].w_old.w_att = win[i].w_fen[BW_WORK].ob_state;	/* Enregistrer couleurs */
			}
		}
	} while (NOT (win[i++].w_flags & WFLAST));
}
/* #] alliconise () Ic“nise une fenˆtre :													*/ 
/****** Fonctions d'ev‚nements fenˆtres formulaire ****************/
/* #[ full_wform () Fenˆtre formulaire plein pot :								*/
void full_wform (void)
{
int index, x, y, w, h, xw, yw, ww, hw, xf, yf, wf, hf, oldx, oldy, oldw, oldh;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;
	if (NOT (win[index].w_attr & FULLER))
		return ;

	oldx = win[index].w_fen->ob_x;
	oldy = win[index].w_fen->ob_y;
	oldw = win[index].w_fen->ob_width;
	oldh = win[index].w_fen->ob_height;
	if (win[index].w_flags & WFFULL)
	{
		x = win[index].w_old.w_coord.g_x;
		y = win[index].w_old.w_coord.g_y;
		w = win[index].w_old.w_coord.g_w;
		h = win[index].w_old.w_coord.g_h;
		win[index].w_fen->ob_x = x;
		win[index].w_fen->ob_y = y;
		win[index].w_fen->ob_width = w;
		win[index].w_fen->ob_height = h;
		wind_set (win[index].w_hg, WF_CURRXYWH, x, y, w, h);
	}
	else
	{
		win[index].w_old.w_coord.g_x = win[index].w_curr.g_x;
		win[index].w_old.w_coord.g_y = win[index].w_curr.g_y;
		win[index].w_old.w_coord.g_w = win[index].w_curr.g_w;
		win[index].w_old.w_coord.g_h = win[index].w_curr.g_h;
		win[index].w_col = ZERO;
		win[index].w_lin = ZERO;
		form_center (win[index].w_cont.w_form.w_tree, &x, &y, &w, &h);

			/* D‚duire les coordonn‚es de la fenˆtre */
		bwind_calc (WC_BORDER, win[index].w_attr, BLANK, ZERO,
								x, y, w, h, &xf, &yf, &wf, &hf);

		if (xf < bureau.xd)	/* Ne pas sortir du bureau */
			xf = bureau.xd;
		if (yf < bureau.yd)
			yf = bureau.yd;
		if (wf > bureau.wd)
			wf = bureau.wd;
		if (hf > bureau.hd)
			hf = bureau.hd;

		win[index].w_fen->ob_x = xf;
		win[index].w_fen->ob_y = yf;
		win[index].w_fen->ob_width = w = wf;
		win[index].w_fen->ob_height = h = hf;

		wind_set (win[index].w_hg, WF_CURRXYWH, xf, yf, wf, hf);
	}
	resize (index, w - oldw, h - oldh);
	win[index].w_flags ^= WFFULL;
	wind_get (win[index].w_hg, WF_CURRXYWH,
						&win[index].w_curr.g_x, &win[index].w_curr.g_y,
						&win[index].w_curr.g_w, &win[index].w_curr.g_h);
	zone_work (index, &xw, &yw, &ww, &hw);
	win[index].w_cont.w_form.w_tree->ob_x = (int)((long)xw - win[index].w_col);
	win[index].w_cont.w_form.w_tree->ob_y = (int)((long)yw - win[index].w_lin);
	height_sliders (index, FALSE);
	if (((oldx == win[index].w_curr.g_x) && (oldy == win[index].w_curr.g_y)) &&
			((oldw > win[index].w_curr.g_w) && (oldh > win[index].w_curr.g_h)))
		print_wind (index);
}
/* #] full_wform () Fenˆtre formulaire plein pot :								*/ 
/* #[ size_wform () Redimensionnement fenˆtre formulaire :				*/
void size_wform (void)
{
int index, xw, yw, dummy;

	index = find_index (buf[3]);
	if (index == BLANK)
		return;

	win[index].w_fen->ob_x = buf[4];
	win[index].w_fen->ob_y = buf[5];
	win[index].w_fen->ob_width = buf[6];
	win[index].w_fen->ob_height = buf[7];
	wind_set (buf[3], WF_CURRXYWH, buf[4], buf[5], buf[6], buf[7]);
	wind_get (win[index].w_hg, WF_CURRXYWH,
						&win[index].w_curr.g_x, &win[index].w_curr.g_y,
						&win[index].w_curr.g_w, &win[index].w_curr.g_h);
	height_sliders (index, FALSE);
	zone_work (index, &xw, &yw, &dummy, &dummy);
	win[index].w_cont.w_form.w_tree->ob_x = xw - (int)win[index].w_col;
	win[index].w_cont.w_form.w_tree->ob_y = yw - (int)win[index].w_lin;
	win[index].w_flags &= ~WFFULL;
}
/* #] size_wform () Redimensionnement fenˆtre formulaire :				*/ 

