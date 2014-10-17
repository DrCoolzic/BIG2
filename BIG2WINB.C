/*						>>> Big Is Gem <<<
		MODULE FONCTIONS DE GESTION DES FENETRES BIG
*/
/****** En-tˆte BIG ***********************************************/
#include <BIG2.H>
#include "BIG2WINB.H"
#include "BIG2WIND.H"
#include "BIG2USR2.H"
#include "BIG2EVTW.H"
#include "BIG2INCL.H"
extern Win_user bwin;
extern int topp;
/* #[ Prototypage des fonctions : 																*/
void rubberbox (int *xfin, int *yfin,
								int *wfin, int *hfin,
								int obj, int index);
void dragbox (int wr, int hr, int xr, int yr,
							int xz, int yz, int wz, int hz,
							int *xfin, int *yfin, int index);
/* #] Prototypage des fonctions : 																*/ 
/* #[ put_fen () Placer l'arbre de fenˆtre :											*/
void put_fen (int index)
{
int x, y, w, h, at;
OBJECT *adr;

		/* Copie du formulaire fenˆtre */
	memcpy (win[index].w_fen, bi_trindex[BIG_WIND], (NGAD * sizeof (OBJECT)));
	adr = win[index].w_fen;

	x = win[index].w_curr.g_x;
	y = win[index].w_curr.g_y;
	w = win[index].w_curr.g_w;
	h = win[index].w_curr.g_h;

	adr->ob_x = x;					/* Mettre tout … z‚ro */
	adr->ob_y = y;					/* comme si la fenˆtre */
	adr->ob_width = w;			/* poss‚dait tous les */
	adr->ob_height = h;			/* attributs. */
	adr[BW_SIZERUR].ob_x = w - adr[BW_SIZERUR].ob_width;
	adr[BW_FULLER].ob_x = adr[BW_SIZERUR].ob_x - adr[BW_FULLER].ob_width;
	adr[BW_CYCLER].ob_x = adr[BW_FULLER].ob_x - adr[BW_CYCLER].ob_width;
	adr[BW_SMALLER].ob_x = adr[BW_CYCLER].ob_x - adr[BW_SMALLER].ob_width;
	adr[BW_MOVER].ob_width = w - adr[BW_SIZERUL].ob_width -
													 adr[BW_CLOSER].ob_width -
													 adr[BW_SIZERUR].ob_width -
													 adr[BW_FULLER].ob_width -
													 adr[BW_CYCLER].ob_width -
													 adr[BW_SMALLER].ob_width + 3;
	adr[BW_INFO].ob_width = w;
	adr[BW_BARRE].ob_width = w;
	adr[BW_VBAR].ob_x = w - adr[BW_VBAR].ob_width;
	adr[BW_VBAR].ob_height = h - adr[BW_SIZERUR].ob_height -
													 adr[BW_INFO].ob_height -
													 adr[BW_SIZERDR].ob_height + 2;
	adr[BW_VELEV].ob_height = adr[BW_VBAR].ob_height -
														adr[BW_UPARROW].ob_height -
														adr[BW_DNARROW].ob_height;
	adr[BW_DNARROW].ob_y = adr[BW_VELEV].ob_y + adr[BW_VELEV].ob_height;
	adr[BW_SIZERDL].ob_y = h - adr[BW_SIZERDL].ob_height;
	adr[BW_HBAR].ob_y = h - adr[BW_HBAR].ob_height;
	adr[BW_HBAR].ob_width = w - adr[BW_SIZERDL].ob_width -
													 adr[BW_SIZERDR].ob_width + 1;
	adr[BW_HELEV].ob_width = adr[BW_HBAR].ob_width -
														adr[BW_LFARROW].ob_width -
														adr[BW_RTARROW].ob_width;
	adr[BW_RTARROW].ob_x = adr[BW_HELEV].ob_x + adr[BW_HELEV].ob_width;
	adr[BW_SIZERDR].ob_x = w - adr[BW_SIZERDR].ob_width;
	adr[BW_SIZERDR].ob_y = h - adr[BW_SIZERDR].ob_height;
	adr[BW_WORK].ob_width = w - adr[BW_VBAR].ob_width + 1;
	adr[BW_WORK].ob_height = h - adr[BW_MOVER].ob_height -
													 adr[BW_INFO].ob_height -
													 adr[BW_HBAR].ob_height + 3;

	if (win[index].w_flags & WFGROUP)
	{		/* Si groupement des flŠches */
		adr[BW_VELEV].ob_y = 1;
		adr[BW_UPARROW].ob_y = 1 + adr[BW_VELEV].ob_height;
		adr[BW_HELEV].ob_x = ZERO;
		adr[BW_LFARROW].ob_x = adr[BW_HELEV].ob_width;
	}

		/* Selon les attributs absents... */
	at = win[index].w_attr;

	if (NOT (at & ALLSIZER))
	{
		adr[BW_SIZERUL].ob_flags |= HIDETREE;
		adr[BW_SIZERUR].ob_flags |= HIDETREE;
		adr[BW_SIZERDL].ob_flags |= HIDETREE;
		adr[BW_CLOSER].ob_x -= adr[BW_SIZERUL].ob_width;
		adr[BW_MOVER].ob_x -= adr[BW_SIZERUL].ob_width;
		adr[BW_MOVER].ob_width += adr[BW_SIZERUL].ob_width;
		adr[BW_MOVER].ob_width += adr[BW_SIZERUR].ob_width;
		adr[BW_SMALLER].ob_x += adr[BW_SIZERUR].ob_width;
		adr[BW_CYCLER].ob_x += adr[BW_SIZERUR].ob_width;
		adr[BW_FULLER].ob_x += adr[BW_SIZERUR].ob_width;
		adr[BW_HBAR].ob_x -= adr[BW_SIZERDL].ob_width;
		adr[BW_HBAR].ob_width += adr[BW_SIZERDL].ob_width;
		adr[BW_HELEV].ob_width += adr[BW_SIZERDL].ob_width;
		adr[BW_RTARROW].ob_x += adr[BW_SIZERDL].ob_width;
	}
	if (NOT (at & CLOSER))
	{
		adr[BW_CLOSER].ob_flags |= HIDETREE;
		adr[BW_MOVER].ob_x -= adr[BW_CLOSER].ob_width;
		adr[BW_MOVER].ob_width += adr[BW_CLOSER].ob_width;
	}
	if (NOT (at & SMALLER))
	{
		adr[BW_SMALLER].ob_flags |= HIDETREE;
		adr[BW_MOVER].ob_width += adr[BW_SMALLER].ob_width;
	}
	if (NOT (at & CYCLER))
	{
		adr[BW_CYCLER].ob_flags |= HIDETREE;
		adr[BW_MOVER].ob_width += adr[BW_CYCLER].ob_width;
		adr[BW_SMALLER].ob_x += adr[BW_CYCLER].ob_width;
	}
	if (NOT (at & FULLER))
	{
		adr[BW_FULLER].ob_flags |= HIDETREE;
		adr[BW_MOVER].ob_width += adr[BW_FULLER].ob_width;
		adr[BW_SMALLER].ob_x += adr[BW_FULLER].ob_width;
		adr[BW_CYCLER].ob_x += adr[BW_FULLER].ob_width;
	}
	if (NOT (at & INFO))
	{
		adr[BW_INFO].ob_flags |= HIDETREE;
		adr[BW_WORK].ob_y -= (adr[BW_INFO].ob_height - 2);
		adr[BW_WORK].ob_height += (adr[BW_INFO].ob_height - 2);
		adr[BW_BARRE].ob_y -= (adr[BW_INFO].ob_height - 1);
		adr[BW_VBAR].ob_y -= (adr[BW_INFO].ob_height - 1);
		adr[BW_VBAR].ob_height += (adr[BW_INFO].ob_height - 1);
		adr[BW_VELEV].ob_height += (adr[BW_INFO].ob_height - 1);
		adr[BW_DNARROW].ob_y += (adr[BW_INFO].ob_height - 1);
	}
	if (NOT (at & UPARROW))
	{
		adr[BW_UPARROW].ob_flags |= HIDETREE;
		adr[BW_VELEV].ob_y -= (adr[BW_UPARROW].ob_height - 1);
		adr[BW_VELEV].ob_height += (adr[BW_UPARROW].ob_height + 1);
	}
	if (NOT (at & DNARROW))
	{
		adr[BW_DNARROW].ob_flags |= HIDETREE;
		adr[BW_VELEV].ob_height += adr[BW_DNARROW].ob_height;
	}
	if (NOT (at & VSLIDE))
	{
		adr[BW_VELEV].ob_flags |= HIDETREE;
	}
	if ((NOT (at & UPARROW)) &&
			(NOT (at & DNARROW)) &&
			(NOT (at & VSLIDE)))
	{
		adr[BW_VBAR].ob_flags |= HIDETREE;
		adr[BW_WORK].ob_width += adr[BW_VBAR].ob_width - 1;
	}
	if (NOT (at & LFARROW))
	{
		adr[BW_LFARROW].ob_flags |= HIDETREE;
		adr[BW_HELEV].ob_x -= (adr[BW_LFARROW].ob_width - 1);
		adr[BW_HELEV].ob_width += (adr[BW_LFARROW].ob_width + 1);
	}
	if (NOT (at & RTARROW))
	{
		adr[BW_RTARROW].ob_flags |= HIDETREE;
		adr[BW_HELEV].ob_width += adr[BW_RTARROW].ob_width;
	}
	if (NOT (at & HSLIDE))
	{
		adr[BW_HELEV].ob_flags |= HIDETREE;
	}
	if ((NOT (at & LFARROW)) &&
			(NOT (at & RTARROW)) &&
			(NOT (at & HSLIDE)))
	{
		if ((NOT (at & SIZER)) && (NOT (at & ALLSIZER)))
		{
			adr[BW_HBAR].ob_flags |= HIDETREE;
			adr[BW_WORK].ob_height += adr[BW_HBAR].ob_height - 1;
		}
		else
		{
			adr[BW_HELEV].ob_flags |= HIDETREE;
			adr[BW_LFARROW].ob_flags |= HIDETREE;
			adr[BW_RTARROW].ob_flags |= HIDETREE;
		}
	}
	if (NOT (at & SIZER))
	{
		adr[BW_SIZERDR].ob_flags |= HIDETREE;
		adr[BW_VBAR].ob_height += adr[BW_SIZERDR].ob_height;
		adr[BW_VELEV].ob_height += adr[BW_SIZERDR].ob_height;
		adr[BW_DNARROW].ob_y += adr[BW_SIZERDR].ob_height;
		adr[BW_HBAR].ob_width += adr[BW_SIZERDR].ob_width;
		adr[BW_HELEV].ob_width += adr[BW_SIZERDR].ob_width;
		adr[BW_RTARROW].ob_x += adr[BW_SIZERDR].ob_width;
	}

	if (win[index].w_flags & WFGROUP)
	{
		adr[BW_UPARROW].ob_y = (adr[BW_DNARROW].ob_y - adr[BW_UPARROW].ob_height + 1);
		adr[BW_LFARROW].ob_x = (adr[BW_RTARROW].ob_x - adr[BW_LFARROW].ob_width + 1);
	}
}
/* #] put_fen () Placer l'arbre de fenˆtre :											*/ 
/* #[ no_top () Manipulation en arriŠre-plan :										*/
void no_top (int obj, int index)
{
int top, dummy;

	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	switch (obj)
	{
	case BW_MOVER :
		if (win[index].w_attr & MOVER)
		{
			if ((clik == 2) && (win[index].w_icon == BLANK))
			{
				buf[3] = win[index].w_hg;
				(*win[index].w_full)();
				buf[0] = WM_FULLED;
				buf[1] = ap_id;
				buf[3] = win[index].w_hg;
				wind_mouse ();
			}
			else if ((clik == 1) && (kbd == K_CTRL))
			{
				buf[3] = win[index].w_hg;
				(*win[index].w_cycle)();
				buf[0] = WM_CYCLED;
				buf[1] = ap_id;
				buf[3] = win[index].w_hg;
				buf[4] = top;
			}
			else
			{
				movebw (index);
				buf[0] = WM_MOVED;
				buf[1] = ap_id;
				buf[3] = win[index].w_hg;
				buf[4] = win[index].w_curr.g_x;
				buf[5] = win[index].w_curr.g_y;
				buf[6] = win[index].w_curr.g_w;
				buf[7] = win[index].w_curr.g_h;
				wind_mouse ();
			}
		}
		break;
	case BW_SIZERUL :
	case BW_SIZERUR :
	case BW_SIZERDL :
	case BW_SIZERDR :
		sizebw (obj, index);
		buf[0] = WM_SIZED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		buf[4] = win[index].w_curr.g_x;
		buf[5] = win[index].w_curr.g_y;
		buf[6] = win[index].w_curr.g_w;
		buf[7] = win[index].w_curr.g_h;
		wind_mouse ();
		break;
	case BW_CLOSER :
		buf[3] = win[index].w_hg;
		(*win[index].w_close)();
		buf[0] = WM_CLOSED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		wind_mouse ();
		break;
	case BW_CYCLER :
		buf[3] = win[index].w_hg;
		(*win[index].w_cycle)();
		buf[0] = WM_CYCLED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		buf[4] = top;
		break;
	case BW_FULLER :
		buf[3] = win[index].w_hg;
		(*win[index].w_full)();
		buf[0] = WM_FULLED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		wind_mouse ();
		break;
	case BW_HSLIDE :
		slid (obj, index);
		buf[0] = WM_HSLID;
		buf[1] = ap_id;
		buf[3] = top;
		buf[4] = (int)((double)win[index].w_fen[BW_HSLIDE].ob_x /
									 ((double)win[index].w_fen[BW_HELEV].ob_width - (double)win[index].w_fen[BW_HSLIDE].ob_width) *
									 1000.0);
		break;
	case BW_VSLIDE :
		slid (obj, index);
		buf[0] = WM_VSLID;
		buf[1] = ap_id;
		buf[3] = top;
		buf[4] = (int)((double)win[index].w_fen[BW_VSLIDE].ob_y /
									 ((double)win[index].w_fen[BW_VELEV].ob_height - (double)win[index].w_fen[BW_VSLIDE].ob_height) *
									 1000.0);
		break;
	case BW_UPARROW :
		arrows (obj, index);
		buf[0] = WM_ARROWED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		buf[4] = WA_UPLINE;
		break;
	case BW_DNARROW :
		arrows (obj, index);
		buf[0] = WM_ARROWED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		buf[4] = WA_DNLINE;
		break;
	case BW_LFARROW :
		arrows (obj, index);
		buf[0] = WM_ARROWED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		buf[4] = WA_LFLINE;
		break;
	case BW_RTARROW :
		arrows (obj, index);
		buf[0] = WM_ARROWED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		buf[4] = WA_RTLINE;
		break;
	case BW_VELEV :
		if (asce (obj, index))
			buf[4] = WA_DNPAGE;
		else
			buf[4] = WA_UPPAGE;
		buf[0] = WM_ARROWED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		break;
	case BW_HELEV :
		if (asce (obj, index))
			buf[4] = WA_LFPAGE;
		else
			buf[4] = WA_RTPAGE;
		buf[0] = WM_ARROWED;
		buf[1] = ap_id;
		buf[3] = win[index].w_hg;
		break;
	case BW_SMALLER :
		if (kbd == K_CTRL)
		{
			alliconise (index);
			buf[0] = WM_ALLICONIFY;
			buf[1] = ap_id;
			buf[3] = win[index].w_hg;
		}
		else if (kbd == ZERO)
		{
			iconise (index);
			buf[0] = WM_ICONIFY;
			buf[1] = ap_id;
			buf[3] = win[index].w_hg;
		}
		break;
 	}
}
/* #] no_top () Manipulation en arriŠre-plan :										*/ 
/* #[ asce() Traitement des acenseurs :														*/
int asce (int obj, int index)
{
int xo, yo, s, a, sens, red = FALSE, dummy;

	if (obj == BW_VELEV)
		s = BW_VSLIDE;
	else if (obj == BW_HELEV)
		s = BW_HSLIDE;
	a = parent (win[index].w_fen, s);

	objc_offset (win[index].w_fen, s, &xo, &yo);
	if (obj == BW_VELEV)
	{
		if (my < yo)
			sens = FALSE;	/* Vers le haut */
		else
			sens = TRUE;	/* Vers le bas */
	}
	if (obj == BW_HELEV)
	{
		if (mx < xo)
			sens = FALSE;	/* Vers la gauche */
		else
			sens = TRUE;	/* Vers la droite */
	}

	buf[3] = win[index].w_hg;
	do
	{
		red = FALSE;
		if ((obj == BW_VELEV) && (sens == FALSE))				/* Haut */
		{
			(*win[index].w_uppage)();
			red = TRUE;
		}
		else if ((obj == BW_VELEV) && (sens == TRUE))		/* Bas */
		{
			(*win[index].w_dnpage)();
			red = TRUE;
		}
		else if ((obj == BW_HELEV) && (sens == FALSE))	/* Gauche */
		{
			(*win[index].w_lfpage)();
			red = TRUE;
		}
		else if ((obj == BW_HELEV) && (sens == TRUE))		/* Droite */
		{
			(*win[index].w_rtpage)();
			red = TRUE;
		}
  
		if (red)
			draw_obj (win[index].w_fen, a);						/* Redessiner l'ascenseur */
		graf_mkstate (&dummy, &dummy, &mk, &dummy);	/* Bouton souris */
	} while (mk);
	return sens;
}
/* #] asce() Traitement des acenseurs :														*/ 
/* #[ arrows () Traitement des flŠches :													*/
void arrows (int obj, int index)
{
int s, a, red = FALSE, dummy;

	if ((obj == BW_UPARROW) || (obj == BW_DNARROW))
		s = BW_VSLIDE;
	else if ((obj == BW_LFARROW) || (obj == BW_RTARROW))
		s = BW_HSLIDE;
	a = parent (win[index].w_fen, s);

	win[index].w_fen[obj].ob_state |= SELECTED;				/* S‚lectionner */
	draw_obj (win[index].w_fen, obj);									/* Redessiner l'objet */

	buf[3] = win[index].w_hg;
	do
	{
		red = FALSE;
		if (obj == BW_UPARROW)
		{
			(*win[index].w_upline)();
			red = TRUE;
		}
		else if (obj == BW_LFARROW)
		{
			(*win[index].w_lfline)();
			red = TRUE;
		}
		else if (obj == BW_DNARROW)
		{
			(*win[index].w_dnline)();
			red = TRUE;
		}
		else if (obj == BW_RTARROW)
		{
			(*win[index].w_rtline)();
			red = TRUE;
		}
  
		if (red)
			draw_obj (win[index].w_fen, a);						/* Redessiner l'ascenseur */
		graf_mkstate (&dummy, &dummy, &mk, &dummy);	/* Bouton souris */
	} while (mk);

	win[index].w_fen[obj].ob_state &= ~SELECTED;				/* D‚s‚lectionner */
	draw_obj (win[index].w_fen, obj);										/* Redessiner l'objet */
}
/* #] arrows () Traitement des flŠches :													*/ 
/* #[ slid () Mouvements sliders () :															*/
void slid (int obj, int index)
{
int xfin, yfin, pere, xr, yr, wr, hr, xz, yz, wz, hz, ox, oy;
MFORM slider_h_mouse =
	{	0x0007,0x0007,0x0001,0x0000,0x0002,
			/* Mask data */
		0x0000,0x0000,0x0810,0x1428,
		0x2424,0x47E2,0x8001,0x0000,
		0x8001,0x47E2,0x2424,0x1428,
		0x0810,0x0000,0x0000,0x0000,
			/* Cursor data */
		0x0000,0x0000,0x0000,0x0810,
		0x1818,0x381C,0x7FFE,0xFFFF,
		0x7FFE,0x381C,0x1818,0x0810,
		0x0000,0x0000,0x0000,0x0000
	},
	slider_v_mouse =
	{	0x0008,0x0007,0x0001,0x0000,0x0002,
			/* Mask data */
		0x0140,0x0220,0x0410,0x0808,
		0x1004,0x0E38,0x0220,0x02A0,
		0x0220,0x0220,0x0E38,0x1004,
		0x0808,0x0410,0x0220,0x0140,
			/* Cursor data */
		0x0080,0x01C0,0x03E0,0x07F0,
		0x0FF8,0x01C0,0x01C0,0x01C0,
		0x01C0,0x01C0,0x01C0,0x0FF8,
		0x07F0,0x03E0,0x01C0,0x0080
	};

	pere = parent (win[index].w_fen, obj);
	objc_offset (win[index].w_fen, obj, &xr, &yr);
	wr = win[index].w_fen[obj].ob_width;
	hr = win[index].w_fen[obj].ob_height;
	objc_offset (win[index].w_fen, pere, &xz, &yz);
	wz = win[index].w_fen[pere].ob_width;
	hz = win[index].w_fen[pere].ob_height;
	win[index].w_fen[obj].ob_state |= SELECTED;			/* S‚lectionner */
	draw_obj (win[index].w_fen, obj);								/* Redessiner l'objet */
	if (obj == BW_VSLIDE)
		graf_mouse (USER_DEF, &slider_v_mouse);
	else if (obj == BW_HSLIDE)
		graf_mouse (USER_DEF, &slider_h_mouse);
	objc_offset (win[index].w_fen, obj, &ox, &oy);
	dragbox (wr, hr, xr, yr,
					 xz, yz, wz, hz,
					 &xfin, &yfin, BLANK);
	graf_mouse (ARROW, ZERO);
	if ((xfin != ox) || (yfin != oy))
	{
		buf[3] = win[index].w_hg;
		if (obj == BW_VSLIDE)
		{
			win[index].w_fen[obj].ob_y = yfin - yz;
			vslid ();
		}
		else if (obj == BW_HSLIDE)
		{
			win[index].w_fen[obj].ob_x = xfin - xz;
			hslid ();
		}
	}
	win[index].w_fen[obj].ob_state &= ~SELECTED;	/* D‚s‚lectionner */
	draw_obj (win[index].w_fen, pere);						/* Redessiner l'objet */
}
/* #] slid () Mouvements sliders () :															*/ 
/* #[ draw_obj () Redessiner un objet :														*/
void draw_obj (OBJECT *adr, int obj)
{
int i = ZERO, index = BLANK;
GRECT rd, r;

	do
	{
		if (win[i].w_fen == adr)
			index = i;
	} while ((NOT (win[i++].w_flags & WFLAST)) && (index == BLANK));

	if (win[index].w_hg <= ZERO)
		return;

	objc_offset (win[index].w_fen, obj, &r.g_x, &r.g_y);
	r.g_w = win[index].w_fen[obj].ob_width;
	r.g_h = win[index].w_fen[obj].ob_height;
	wind_get (win[index].w_hg, WF_FIRSTXYWH,
						&rd.g_x, &rd.g_y, &rd.g_w, &rd.g_h);
	while ((rd.g_w > ZERO) && (rd.g_h > ZERO))
	{
		if (rc_intersect (&r, &rd))
			objc_draw (win[index].w_fen, obj, 1, rd.g_x, rd.g_y, rd.g_w, rd.g_h);
		wind_get (win[index].w_hg, WF_NEXTXYWH, &rd.g_x, &rd.g_y, &rd.g_w, &rd.g_h);
	}
}
/* #] draw_obj () Redessiner un objet :														*/ 
/* #[ draw_wind () Redessiner contour fenˆtre :										*/
void draw_wind (int index, GRECT *r)
{
register int i;
int tab[] = {BW_SIZERUL, BW_CLOSER, BW_MOVER, BW_SMALLER, BW_CYCLER, BW_FULLER,
						 BW_SIZERUR, BW_INFO, BW_VBAR, BW_SIZERDL, BW_HBAR, BW_SIZERDR};

	for (i = 0 ; i < 12 ; i++)
		if (NOT (win[index].w_fen[tab[i]].ob_flags & HIDETREE))
		{
			objc_draw (win[index].w_fen, tab[i], 3,	/* Dessiner ‚l‚ment */
								 r->g_x, r->g_y, r->g_w, r->g_h);
		}
}
/* #] draw_wind () Redessiner contour fenˆtre :										*/ 
/* #[ resize () Replace les objets :															*/
void resize (int index, int deltaw, int deltah)
{
	win[index].w_fen[BW_MOVER].ob_width += deltaw;
	win[index].w_fen[BW_SMALLER].ob_x += deltaw;
	win[index].w_fen[BW_CYCLER].ob_x += deltaw;
	win[index].w_fen[BW_FULLER].ob_x += deltaw;
	win[index].w_fen[BW_SIZERUR].ob_x += deltaw;
	win[index].w_fen[BW_INFO].ob_width += deltaw;
	win[index].w_fen[BW_BARRE].ob_width += deltaw;
	win[index].w_fen[BW_VBAR].ob_x += deltaw;
	win[index].w_fen[BW_VBAR].ob_height += deltah;
	win[index].w_fen[BW_VELEV].ob_height += deltah;
	win[index].w_fen[BW_DNARROW].ob_y += deltah;
	win[index].w_fen[BW_SIZERDL].ob_y += deltah;
	win[index].w_fen[BW_HBAR].ob_y += deltah;
	win[index].w_fen[BW_HBAR].ob_width += deltaw;
	win[index].w_fen[BW_HELEV].ob_width += deltaw;
	win[index].w_fen[BW_RTARROW].ob_x += deltaw;
	win[index].w_fen[BW_SIZERDR].ob_x += deltaw;
	win[index].w_fen[BW_SIZERDR].ob_y += deltah;
	win[index].w_fen[BW_WORK].ob_width += deltaw;
	win[index].w_fen[BW_WORK].ob_height += deltah;

	if (win[index].w_flags & WFGROUP)
	{
		win[index].w_fen[BW_UPARROW].ob_y =
				(win[index].w_fen[BW_DNARROW].ob_y - win[index].w_fen[BW_UPARROW].ob_height + 1);
		win[index].w_fen[BW_LFARROW].ob_x =
				(win[index].w_fen[BW_RTARROW].ob_x - win[index].w_fen[BW_LFARROW].ob_width + 1);
	}
}
/* #] resize () Replace les objets :															*/ 
/* #[ sizebw () Redimensionner fenˆtre :													*/
void sizebw (int obj, int index)
{
int xfin, yfin, wfin, hfin, oldw, oldh, xw, yw, ww, hw, dummy;
MFORM sizdl_ur_mouse =
	{	0x0007,0x0007,0x0001,0x0000,0x0002,
			/* Mask data */
		0x00FF,0x0101,0x0081,0x0041,
		0x0081,0x0101,0x0209,0x4415,
		0xA822,0x9040,0x8080,0x8100,
		0x8200,0x8100,0x8080,0xFF00,
			/* Cursor data */
		0x0000,0x00FE,0x007E,0x003E,
		0x007E,0x00FE,0x01F6,0x03E2,
		0x47C0,0x6F80,0x7F00,0x7E00,
		0x7C00,0x7E00,0x7F00,0x0000
	},
	sizul_dr_mouse =
	{	0x0007,0x0007,0x0001,0x0000,0x0002,
			/* Mask data */
		0xFF00,0x8080,0x8100,0x8200,
		0x8100,0x8080,0x9040,0xA822,
		0x4415,0x0209,0x0101,0x0081,
		0x0041,0x0081,0x0101,0x00FF,
			/* Cursor data */
		0x0000,0x7F00,0x7E00,0x7C00,
		0x7E00,0x7F00,0x6F80,0x47C0,
		0x03E2,0x01F6,0x00FE,0x007E,
		0x003E,0x007E,0x00FE,0x0000
	};

	win[index].w_fen[obj].ob_state |= SELECTED;					/* S‚lectionner */
	draw_obj (win[index].w_fen, obj);										/* Redessiner l'objet */
	oldw = win[index].w_fen->ob_width;
	oldh = win[index].w_fen->ob_height;
	if (obj == BW_SIZERUL)									/* Haut Gauche */
		graf_mouse (USER_DEF, &sizul_dr_mouse);
	else if (obj == BW_SIZERDR)							/* Bas Droit */
		graf_mouse (USER_DEF, &sizul_dr_mouse);
	else if (obj == BW_SIZERUR)							/* Haut Droit */
		graf_mouse (USER_DEF, &sizdl_ur_mouse);
	else if (obj == BW_SIZERDL)							/* Bas Gauche */
		graf_mouse (USER_DEF, &sizdl_ur_mouse);
	rubberbox (&xfin, &yfin, &wfin, &hfin, obj, index);

	if (win[index].w_flags & WFCALAG)
	{
		bwind_calc (WC_WORK, win[index].w_attr, BLANK, win[index].w_bar,
								xfin, yfin, wfin, hfin, &xw, &yw, &ww, &hw);
		ww = (ww + win[index].w_wunit - 1) / win[index].w_wunit * win[index].w_wunit;
		hw = (hw + win[index].w_hunit - 1) / win[index].w_hunit * win[index].w_hunit;
		bwind_calc (WC_BORDER, win[index].w_attr, BLANK, win[index].w_bar,
								xw, yw, ww, hw, &xfin, &yfin, &wfin, &hfin);
	}

	graf_mouse (ARROW, ZERO);
	win[index].w_fen[obj].ob_state &= ~SELECTED;				/* D‚s‚lectionner */
	win[index].w_curr.g_x = xfin;
	win[index].w_curr.g_y = yfin;
	win[index].w_curr.g_w = wfin;
	win[index].w_curr.g_h = hfin;

	if (((win[index].w_type & 0xFF) == WTYP_PICT) ||
			(win[index].w_type == WTYP_FORM))
	{
		bwind_calc (WC_WORK, win[index].w_attr, BLANK, win[index].w_bar,
								win[index].w_curr.g_x, win[index].w_curr.g_y,
								win[index].w_curr.g_w, win[index].w_curr.g_h,
								&xw, &yw, &ww, &hw);
		if (ww > win[index].w_wtot)
			ww = (int)win[index].w_wtot;
		if (hw > win[index].w_htot)
			hw = (int)win[index].w_htot;
		if ((win[index].w_type & WTYP_TOOL) || (win[index].w_type & WTYP_MENU))
			hw++;
		bwind_calc (WC_BORDER, win[index].w_attr, BLANK, win[index].w_bar,
								xw, yw, ww, hw,
								&win[index].w_curr.g_x, &win[index].w_curr.g_y,
								&wfin, &hfin);
		win[index].w_curr.g_w = wfin;
		win[index].w_curr.g_h = hfin;
	}

	resize (index, wfin - oldw, hfin - oldh);

	if (win[index].w_flags & WFDECAL)
	{
		zone_work (index, &dummy, &dummy, &ww, &hw);
		switch (obj)
		{				/* Corrections ligne et colonne */
		case BW_SIZERUL :		/* Haut Gauche */
			win[index].w_lin -= (hfin - oldh);
			if (win[index].w_lin < ZERO)
				win[index].w_lin = ZERO;
			if (win[index].w_lin > win[index].w_htot - hw)
				win[index].w_lin = win[index].w_htot - hw;
			win[index].w_lin = win[index].w_lin / win[index].w_hunit * win[index].w_hunit;
			win[index].w_col -= (wfin - oldw);
			if (win[index].w_col < ZERO)
				win[index].w_col = ZERO;
			if (win[index].w_col > win[index].w_wtot - ww)
				win[index].w_col = win[index].w_wtot - ww;
			win[index].w_col = win[index].w_col / win[index].w_wunit * win[index].w_wunit;
			break;
		case BW_SIZERUR :		/* Haut Droit */
			win[index].w_lin -= (hfin - oldh);
			if (win[index].w_lin < ZERO)
				win[index].w_lin = ZERO;
			if (win[index].w_lin > win[index].w_htot - hw)
				win[index].w_lin = win[index].w_htot - hw;
			win[index].w_lin = win[index].w_lin / win[index].w_hunit * win[index].w_hunit;
			break;
		case BW_SIZERDL :		/* Bas Gauche */
			win[index].w_col -= (wfin - oldw);
			if (win[index].w_col < ZERO)
				win[index].w_col = ZERO;
			if (win[index].w_col > win[index].w_wtot - ww)
				win[index].w_col = win[index].w_wtot - ww;
			win[index].w_col = win[index].w_col / win[index].w_wunit * win[index].w_wunit;
			break;
		}
	}

	buf[3] = win[index].w_hg;
	buf[4] = win[index].w_curr.g_x;
	buf[5] = win[index].w_curr.g_y;
	buf[6] = win[index].w_curr.g_w;
	buf[7] = win[index].w_curr.g_h;
	(*win[index].w_size)();
	if (NOT ((wfin == oldw) && (hfin == oldh)))		/* Si qq chose chang‚ tout redessiner */
	{
		buf[3] = win[index].w_hg;
		buf[4] = win[index].w_curr.g_x;
		buf[5] = win[index].w_curr.g_y;
		buf[6] = win[index].w_curr.g_w;
		buf[7] = win[index].w_curr.g_h;
		redraw ();
	}
	else																				/* Si rien chang‚ */
		draw_obj (win[index].w_fen, obj);					/* Redessiner l'objet */
	empty (MU_KEYBD);
}
/* #] sizebw () Redimensionner fenˆtre :													*/ 
/* #[ rubberbox () Comme graf_rubberbox du GEM :									*/
void rubberbox (int *xfin, int *yfin,
								int *wfin, int *hfin,
								int obj, int index)
{
int tab[4], pxyf[10], pxyw[10], l1[4], l2[4], old_mx, old_my, deltax, deltay,
		xw, yw, ww, hw, etat, dummy;

	if ((desc_hard_soft.multitache) && (index != id_top))
		etat = 0;
	else
	{
		evnt_timer (25, 0);		/* Petit d‚lai */
		graf_mkstate (&dummy, &dummy, &mk, &dummy);	/* Bouton souris */
		if (mk)
			etat = 1;
		else
			etat = 0;
	}

	fill_tab (tab, 4, bureau.xd, bureau.yd, bureau.xd + bureau.wd - 1, bureau.yd + bureau.hd - 1);
	vs_clip (work_display.handle, TRUE, tab);					/* Clipper */
	vswr_mode (work_display.handle, MD_XOR);				/* Dessin en mode Xor */
	vsl_width (work_display.handle, 1);
	vsl_ends (work_display.handle, SQUARE, SQUARE);
	vsl_udsty (work_display.handle, 0xAAAA);
	vsl_type (work_display.handle, USERLINE);
	vsl_color (work_display.handle, BLACK);
	old_mx = mx;
	old_my = my;
	bwind_calc (WC_WORK, win[index].w_attr, BLANK, win[index].w_bar,
							win[index].w_curr.g_x, win[index].w_curr.g_y,
							win[index].w_curr.g_w, win[index].w_curr.g_h,
							&xw, &yw, &ww, &hw);
	fill_tab (pxyf, 10,
						win[index].w_curr.g_x, win[index].w_curr.g_y,
						win[index].w_curr.g_x + win[index].w_curr.g_w - 1,
						win[index].w_curr.g_y,
						win[index].w_curr.g_x + win[index].w_curr.g_w - 1,
						win[index].w_curr.g_y + win[index].w_curr.g_h - 1,
						win[index].w_curr.g_x,
						win[index].w_curr.g_y + win[index].w_curr.g_h - 1,
						win[index].w_curr.g_x, win[index].w_curr.g_y);
	fill_tab (pxyw, 10,
						xw, yw, xw + ww - 1, yw,
						xw + ww - 1, yw + hw - 2, xw,
						yw + hw - 2, xw, yw);
	fill_tab (l1, 4, xw, yw, xw + ww - 1, yw + hw - 1);
	fill_tab (l2, 4, xw + ww - 1, yw, xw, yw + hw - 1);
	v_hide_c (work_display.handle);
	v_pline (work_display.handle, 5, pxyf);	/* Tracer rectangles */
	v_pline (work_display.handle, 5, pxyw);
	v_pline (work_display.handle, 2, l1);
	v_pline (work_display.handle, 2, l2);
	v_show_c (work_display.handle, TRUE);

	if (NOT mk)
		wind_update (BEG_UPDATE); 	/* Bloquer les fonctions de la souris */
	do
	{
		graf_mkstate (&mx, &my, &mk, &dummy);	/* Bouton souris */
		if (((mx != old_mx) || (my != old_my)) &&	/* Si souris d‚plac‚e */
				(mk == etat))													/* mais pas sortie */
		{
			deltax = mx - old_mx;				/* Noter amplitude d‚placement */
			deltay = my - old_my;
			old_mx = mx;								/* Enregistrer coordonn‚es */
			old_my = my;
			v_hide_c (work_display.handle);
			v_pline (work_display.handle, 5, pxyf);		/* Effacer rectangles */
			v_pline (work_display.handle, 5, pxyw);
			v_pline (work_display.handle, 2, l1);
			v_pline (work_display.handle, 2, l2);

			switch (obj)
			{
			case BW_SIZERUL :				/* Haut Gauche */
				pxyf[0] += deltax;
				pxyf[1] += deltay;
				pxyf[3] += deltay;
				pxyf[6] += deltax;
				pxyf[8] += deltax;
				pxyf[9] += deltay;
				if (pxyf[2] - pxyf[0] < win[index].w_wmini)
				{
					pxyf[0] = pxyf[2] - win[index].w_wmini;
					pxyf[6] = pxyf[2] - win[index].w_wmini;
					pxyf[8] = pxyf[2] - win[index].w_wmini;
				}
				if (pxyf[5] - pxyf[3] < win[index].w_hmini)
				{
					pxyf[1] = pxyf[5] - win[index].w_hmini;
					pxyf[3] = pxyf[5] - win[index].w_hmini;
					pxyf[9] = pxyf[5] - win[index].w_hmini;
				}
				if (pxyf[2] - pxyf[0] > win[index].w_wmaxi)
				{
					pxyf[0] = pxyf[2] - win[index].w_wmaxi;
					pxyf[6] = pxyf[2] - win[index].w_wmaxi;
					pxyf[8] = pxyf[2] - win[index].w_wmaxi;
				}
				if (pxyf[5] - pxyf[3] > win[index].w_hmaxi)
				{
					pxyf[1] = pxyf[5] - win[index].w_hmaxi;
					pxyf[3] = pxyf[5] - win[index].w_hmaxi;
					pxyf[9] = pxyf[5] - win[index].w_hmaxi;
				}
				break;
			case BW_SIZERUR :		/* Haut Droit */
				pxyf[1] += deltay;
				pxyf[2] += deltax;
				pxyf[3] += deltay;
				pxyf[4] += deltax;
				pxyf[9] += deltay;
				if (pxyf[2] - pxyf[0] < win[index].w_wmini)
				{
					pxyf[2] = pxyf[0] + win[index].w_wmini;
					pxyf[4] = pxyf[0] + win[index].w_wmini;
				}
				if (pxyf[5] - pxyf[3] < win[index].w_hmini)
				{
					pxyf[1] = pxyf[5] - win[index].w_hmini;
					pxyf[3] = pxyf[5] - win[index].w_hmini;
					pxyf[9] = pxyf[5] - win[index].w_hmini;
				}
				if (pxyf[2] - pxyf[0] > win[index].w_wmaxi)
				{
					pxyf[2] = pxyf[0] + win[index].w_wmaxi;
					pxyf[4] = pxyf[0] + win[index].w_wmaxi;
				}
				if (pxyf[5] - pxyf[3] > win[index].w_hmaxi)
				{
					pxyf[1] = pxyf[5] - win[index].w_hmaxi;
					pxyf[3] = pxyf[5] - win[index].w_hmaxi;
					pxyf[9] = pxyf[5] - win[index].w_hmaxi;
				}
				break;
			case BW_SIZERDR :		/* Bas Droit */
				pxyf[2] += deltax;
				pxyf[4] += deltax;
				pxyf[5] += deltay;
				pxyf[7] += deltay;
				if (pxyf[2] - pxyf[0] < win[index].w_wmini)
				{
					pxyf[2] = pxyf[0] + win[index].w_wmini;
					pxyf[4] = pxyf[0] + win[index].w_wmini;
				}
				if (pxyf[5] - pxyf[3] < win[index].w_hmini)
				{
					pxyf[5] = pxyf[3] + win[index].w_hmini;
					pxyf[7] = pxyf[3] + win[index].w_hmini;
				}
				if (pxyf[2] - pxyf[0] > win[index].w_wmaxi)
				{
					pxyf[2] = pxyf[0] + win[index].w_wmaxi;
					pxyf[4] = pxyf[0] + win[index].w_wmaxi;
				}
				if (pxyf[5] - pxyf[3] > win[index].w_hmaxi)
				{
					pxyf[5] = pxyf[3] + win[index].w_hmaxi;
					pxyf[7] = pxyf[3] + win[index].w_hmaxi;
				}
				break;
			case BW_SIZERDL :		/* Bas Gauche */
				pxyf[0] += deltax;
				pxyf[5] += deltay;
				pxyf[6] += deltax;
				pxyf[7] += deltay;
				pxyf[8] += deltax;
				if (pxyf[2] - pxyf[0] < win[index].w_wmini)
				{
					pxyf[0] = pxyf[2] - win[index].w_wmini;
					pxyf[6] = pxyf[2] - win[index].w_wmini;
					pxyf[8] = pxyf[2] - win[index].w_wmini;
				}
				if (pxyf[5] - pxyf[3] < win[index].w_hmini)
				{
					pxyf[5] = pxyf[3] + win[index].w_hmini;
					pxyf[7] = pxyf[3] + win[index].w_hmini;
				}
				if (pxyf[2] - pxyf[0] > win[index].w_wmaxi)
				{
					pxyf[0] = pxyf[2] - win[index].w_wmaxi;
					pxyf[6] = pxyf[2] - win[index].w_wmaxi;
					pxyf[8] = pxyf[2] - win[index].w_wmaxi;
				}
				if (pxyf[5] - pxyf[3] > win[index].w_hmaxi)
				{
					pxyf[5] = pxyf[3] + win[index].w_hmaxi;
					pxyf[7] = pxyf[3] + win[index].w_hmaxi;
				}
				break;
			}

			bwind_calc (WC_WORK, win[index].w_attr, BLANK, win[index].w_bar,
									pxyf[0], pxyf[1],
									pxyf[2] - pxyf[0] + 1, pxyf[5] - pxyf[3] + 1,
									&xw, &yw, &ww, &hw);
			fill_tab (pxyw, 10,
								xw, yw, xw + ww - 1, yw,
								xw + ww - 1, yw + hw - 2, xw,
								yw + hw - 2, xw, yw);
			fill_tab (l1, 4, xw, yw, xw + ww - 1, yw + hw - 1);
			fill_tab (l2, 4, xw + ww - 1, yw, xw, yw + hw - 1);
			v_pline (work_display.handle, 5, pxyf);		/* Retracer rectangles */
			v_pline (work_display.handle, 5, pxyw);
			v_pline (work_display.handle, 2, l1);
			v_pline (work_display.handle, 2, l2);
			v_show_c (work_display.handle, TRUE);
		}
	} while (mk == etat);
	if (mk)
		wind_update (END_UPDATE); 	/* Bloquer les fonctions de la souris */

	graf_mkstate (&mx, &my, &mk, &dummy);	/* Bouton souris */
	if ((mk) && (wind_find (mx, my)) && (etat == 0))
		topp = FALSE;	/* Pas de TOPPED si pos‚ sur une fenˆtre */
	while (mk)
		graf_mkstate (&mx, &my, &mk, &dummy);	/* Bouton souris */

	v_hide_c (work_display.handle);
	v_pline (work_display.handle, 5, pxyf);		/* Effacer rectangles */
	v_pline (work_display.handle, 5, pxyw);
	v_pline (work_display.handle, 2, l1);
	v_pline (work_display.handle, 2, l2);
	*xfin = pxyf[0];
	*yfin = pxyf[1];
	*wfin = pxyf[2] - pxyf[0] + 1;
	*hfin = pxyf[7] - pxyf[1] + 1;
	if (*yfin < bureau.yd)
	{
		deltay = bureau.yd - *yfin;
		*yfin = bureau.yd;
		*hfin -= deltay;
	}
	vs_clip (work_display.handle, FALSE, tab);		/* D‚clipper */
	v_show_c (work_display.handle, TRUE);
}
/* #] rubberbox () Comme graf_rubberbox du GEM :									*/ 
/* #[ movebw () D‚placer la fenˆtre :															*/
void movebw (int index)
{
int xfin, yfin;
MFORM mover_mouse =
	{	0x0008,0x0007,0x0001,0x0000,0x0002,
			/* Mask data */
		0xFC3F,0x8241,0x8421,0x8421,
		0x8241,0xB18D,0x4812,0x0420,
		0x0420,0x4810,0xB18D,0x8241,
		0x8421,0x8421,0x8241,0xFC3F,
			/* Cursor data */
		0x0000,0x7C3E,0x781E,0x781E,
		0x7C3E,0x4E72,0x07E0,0x03C0,
		0x03C0,0x07E0,0x4E72,0x7C3E,
		0x781E,0x781E,0x7C3E,0x0000
	};

	win[index].w_fen[BW_MOVER].ob_state |= SELECTED;					/* S‚lectionner */
	draw_obj (win[index].w_fen, BW_MOVER);										/* Redessiner l'objet */
	graf_mouse (USER_DEF, &mover_mouse);
	dragbox (win[index].w_curr.g_w, win[index].w_curr.g_h,
					 win[index].w_curr.g_x, win[index].w_curr.g_y,
					 bureau.xd - bureau.wd, bureau.yd, bureau.wd * 3, bureau.hd * 2,
					 &xfin, &yfin, index);
	graf_mouse (ARROW, ZERO);
	win[index].w_fen[BW_MOVER].ob_state &= ~SELECTED;					/* D‚s‚lectionner */
	draw_obj (win[index].w_fen, BW_MOVER);										/* Redessiner l'objet */
	win[index].w_curr.g_x = xfin;
	win[index].w_curr.g_y = yfin;
	buf[3] = win[index].w_hg;
	buf[4] = win[index].w_curr.g_x;
	buf[5] = win[index].w_curr.g_y;
	buf[6] = win[index].w_curr.g_w;
	buf[7] = win[index].w_curr.g_h;
	(*win[index].w_move)();
	empty (MU_KEYBD);
}
/* #] movebw () D‚placer la fenˆtre :															*/ 
/* #[ dragbox () Comme graf_dragbox du GEM :											*/
void dragbox (int wr, int hr, int xr, int yr,
							int xz, int yz, int wz, int hz,
							int *xfin, int *yfin, int index)
{
int tab[4], pxyf[10], pxyw[10], l1[4], l2[4], old_mx, old_my, deltax, deltay,
		xw, yw, ww, hw, etat, dummy;

	if ((desc_hard_soft.multitache) && (index != id_top))
	{
		etat = 0;
	}
	else
	{
		evnt_timer (25, 0);		/* Petit d‚lai */
		graf_mkstate (&dummy, &dummy, &mk, &dummy);	/* Bouton souris */
		if (mk)
			etat = 1;
		else
			etat = 0;
	}

	fill_tab (tab, 4, bureau.xd, bureau.yd, bureau.xd + bureau.wd - 1, bureau.yd + bureau.hd - 1);
	vs_clip (work_display.handle, TRUE, tab);				/* Clipper */
	vswr_mode (work_display.handle, MD_XOR);				/* Dessin en mode Xor */
	vsl_width (work_display.handle, 1);
	vsl_ends (work_display.handle, SQUARE, SQUARE);
	vsl_udsty (work_display.handle, 0xAAAA);
	vsl_type (work_display.handle, USERLINE);
	vsl_color (work_display.handle, BLACK);
	old_mx = mx;
	old_my = my;
	if ((index > BLANK) && (win[index].w_icon == BLANK))
	{
		bwind_calc (WC_WORK, win[index].w_attr, BLANK, win[index].w_bar,
								win[index].w_curr.g_x, win[index].w_curr.g_y,
								win[index].w_curr.g_w, win[index].w_curr.g_h,
								&xw, &yw, &ww, &hw);
		fill_tab (pxyw, 10,
							xw, yw, xw + ww - 1, yw,
							xw + ww - 1, yw + hw - 2, xw,
							yw + hw - 2, xw, yw);
		fill_tab (l1, 4, xw, yw, xw + ww - 1, yw + hw - 1);
		fill_tab (l2, 4, xw + ww - 1, yw, xw, yw + hw - 1);
	}
	fill_tab (pxyf, 10,
						xr, yr,
						xr + wr - 1, yr,
						xr + wr - 1, yr + hr - 1,
						xr, yr + hr - 1,
						xr, yr);
	v_hide_c (work_display.handle);
	v_pline (work_display.handle, 5, pxyf);	/* Tracer rectangle */
	if ((index > BLANK) && (win[index].w_icon == BLANK))
	{
		v_pline (work_display.handle, 5, pxyw);
		v_pline (work_display.handle, 2, l1);
		v_pline (work_display.handle, 2, l2);
	}
	v_show_c (work_display.handle, TRUE);
	if (NOT mk)
		wind_update (BEG_UPDATE); 	/* Bloquer les fonctions de la souris */
	do
	{
		graf_mkstate (&mx, &my, &mk, &dummy);	/* Bouton souris */
		if (((mx != old_mx) || (my != old_my)) &&	/* Si souris d‚plac‚e */
				(mk == etat))													/* mais pas sortie */
		{
			deltax = mx - old_mx;				/* Noter amplitude d‚placement */
			deltay = my - old_my;
			old_mx = mx;								/* Enregistrer coordonn‚es */
			old_my = my;
			v_hide_c (work_display.handle);
			v_pline (work_display.handle, 5, pxyf);		/* Effacer rectangle */
			if ((index > BLANK) && (win[index].w_icon == BLANK))
			{
				v_pline (work_display.handle, 5, pxyw);
				v_pline (work_display.handle, 2, l1);
				v_pline (work_display.handle, 2, l2);
			}
			pxyf[0] += deltax;
			pxyf[1] += deltay;
			pxyf[2] += deltax;
			pxyf[3] += deltay;
 			pxyf[4] += deltax;
			pxyf[5] += deltay;
			pxyf[6] += deltax;
			pxyf[7] += deltay;
			pxyf[8] += deltax;
			pxyf[9] += deltay;

			if (pxyf[0] < xz)						/* Si sortie … gauche */
			{
				deltax = xz - pxyf[0];
				pxyf[0] += deltax;
				pxyf[2] += deltax;
				pxyf[4] += deltax;
				pxyf[6] += deltax;
				pxyf[8] += deltax;
			}
			if (pxyf[1] < yz)						/* Si sortie en haut */
			{
				deltay = yz - pxyf[1];
				pxyf[1] += deltay;
				pxyf[3] += deltay;
				pxyf[5] += deltay;
				pxyf[7] += deltay;
				pxyf[9] += deltay;
			}
			if (pxyf[2] > xz + wz - 1)		/* Si sortie … droite */
			{
				deltax = pxyf[2] - (xz + wz - 1);
				pxyf[0] -= deltax;
				pxyf[2] -= deltax;
				pxyf[4] -= deltax;
				pxyf[6] -= deltax;
				pxyf[8] -= deltax;
			}
			if (pxyf[5] > yz + hz - 1)		/* Si sortie en bas */
			{
				deltay = pxyf[5] - (yz + hz - 1);
				pxyf[1] -= deltay;
				pxyf[3] -= deltay;
				pxyf[5] -= deltay;
				pxyf[7] -= deltay;
				pxyf[9] -= deltay;
			}
			if ((index > BLANK) && (win[index].w_icon == BLANK))
			{
				bwind_calc (WC_WORK, win[index].w_attr, BLANK, win[index].w_bar,
										pxyf[0], pxyf[1],
										pxyf[2] - pxyf[0] + 1, pxyf[5] - pxyf[3] + 1,
										&xw, &yw, &ww, &hw);
				fill_tab (pxyw, 10,
									xw, yw, xw + ww - 1, yw,
									xw + ww - 1, yw + hw - 2, xw,
									yw + hw - 2, xw, yw);
				fill_tab (l1, 4, xw, yw, xw + ww - 1, yw + hw - 1);
				fill_tab (l2, 4, xw + ww - 1, yw, xw, yw + hw - 1);
			}
			v_pline (work_display.handle, 5, pxyf);		/* Retracer rectangle */
			if ((index > BLANK) && (win[index].w_icon == BLANK))
			{
				v_pline (work_display.handle, 5, pxyw);
				v_pline (work_display.handle, 2, l1);
				v_pline (work_display.handle, 2, l2);
			}
			v_show_c (work_display.handle, TRUE);
		}
	} while (mk == etat);
	if (mk)
		wind_update (END_UPDATE); 	/* Bloquer les fonctions de la souris */

	graf_mkstate (&mx, &my, &mk, &dummy);	/* Bouton souris */
	if ((mk) && (wind_find (mx, my)) && (etat == 0))
		topp = FALSE;	/* Pas de TOPPED si pos‚ sur une fenˆtre */
	while (mk)
		graf_mkstate (&mx, &my, &mk, &dummy);	/* Bouton souris */

	v_hide_c (work_display.handle);
	v_pline (work_display.handle, 5, pxyf);		/* Effacer rectangle */
 	if ((index > BLANK) && (win[index].w_icon == BLANK))
	{
		v_pline (work_display.handle, 5, pxyw);
		v_pline (work_display.handle, 2, l1);
		v_pline (work_display.handle, 2, l2);
	}
	*xfin = pxyf[0];
	*yfin = pxyf[1];
	vs_clip (work_display.handle, FALSE, tab);		/* D‚clipper */
	v_show_c (work_display.handle, TRUE);
}
/* #] dragbox () Comme graf_dragbox du GEM :											*/ 
/* #[ set_work () Fixe trame et couleur zone de travail :					*/
void set_work (int index, int pattern, int colorp)
{
	/* Si un paramŠtre vaut -1, il reste inchang‚.
		 ppppcccc (octet haut de l'ob_state)
		 p : Motif pattern
		 c : Couleur pattern */
int temp;

	if (pattern != BLANK)
	{
		temp = win[index].w_fen[BW_WORK].ob_state & 0x0FFF;
		win[index].w_fen[BW_WORK].ob_state = (temp | (pattern << 12));
	}
	if (colorp != BLANK)
	{
		temp = win[index].w_fen[BW_WORK].ob_state & 0xF0FF;
		win[index].w_fen[BW_WORK].ob_state = (temp | (colorp << 8));
	}
	if (win[index].w_hg > ZERO)
		print_page (index);
}
/* #] set_work () Fixe trame et couleur zone de travail :					*/ 
/* #[ set_windows () Fixe ‚l‚ments graphiques des fenˆtres :			*/
void	set_windows (int mtxt_mover, int ctxt_mover, int spat_mover, int cpat_mover,
									 int ctxt_info,
									 int spat_velev,int cpat_velev, int spat_helev, int cpat_helev,
									 int spat_hslid, int cpat_hslid, int spat_vslid, int cpat_vslid,
									 int cpat_gadgt, int cicn_gadgt)
{
	if (mtxt_mover != BLANK)
		bwin.mtxt_mover = mtxt_mover;
	if (ctxt_mover != BLANK)
		bwin.ctxt_mover = ctxt_mover;
	if (spat_mover != BLANK)
		bwin.spat_mover = spat_mover;
	if (cpat_mover != BLANK)
		bwin.cpat_mover = cpat_mover;
	if (ctxt_info != BLANK)
		bwin.ctxt_info = ctxt_info;
	if (spat_velev != BLANK)
		bwin.spat_velev = spat_velev;
	if (cpat_velev != BLANK)
		bwin.cpat_velev = cpat_velev;
	if (spat_helev != BLANK)
		bwin.spat_helev = spat_helev;
	if (cpat_helev != BLANK)
		bwin.cpat_helev = cpat_helev;
	if (spat_hslid != BLANK)
		bwin.spat_hslid = spat_hslid;
	if (cpat_hslid != BLANK)
		bwin.cpat_hslid = cpat_hslid;
	if (spat_vslid != BLANK)
		bwin.spat_vslid = spat_vslid;
	if (cpat_vslid != BLANK)
		bwin.cpat_vslid = cpat_vslid;
	if (cpat_gadgt != BLANK)
		bwin.cpat_gadgt = cpat_gadgt;
	if (cicn_gadgt != BLANK)
		bwin.cicn_gadgt = cicn_gadgt;

	form_dial (FMD_FINISH, ZERO, ZERO, ZERO, ZERO,	/* Tout r‚afficher */
						 bureau.xd, bureau.yd, bureau.wd, bureau.hd);
}
/* #] set_windows () Fixe ‚l‚ments graphiques des fenˆtres :			*/ 
/* #[ bwind_calc () Equivalent de wind_calc du GEM :							*/
void bwind_calc (int request, int attr,
								 int tree, OBJECT *adrb,
								 int xd, int yd, int wd, int hd,
								 int *x, int *y, int *w, int *h)
{
OBJECT *adr = ZERO;

	if (tree > BLANK)					/* S'il y a un menu ou une ToolBar */
		adr = ir_trindex[tree];	/* Adresse formulaire */
	else if (adrb > ZERO)
		adr = adrb;

	if (request == WC_WORK)		/* Coord. fenˆtre -> Coord. zone travail */
	{
		*x = xd + 1;
		*y = yd + (bi_trindex[BIG_WIND][BW_MOVER].ob_height - 1);
		if (attr & INFO)
			*y += (bi_trindex[BIG_WIND][BW_INFO].ob_height - 1);
		*w = wd - 2;
		if ((attr & UPARROW) || (attr & DNARROW) || (attr & VSLIDE))
			*w -= (bi_trindex[BIG_WIND][BW_VBAR].ob_width - 1);
		*h = hd - (bi_trindex[BIG_WIND][BW_MOVER].ob_height - 1);
		if (attr & INFO)
			*h -= (bi_trindex[BIG_WIND][BW_INFO].ob_height - 1);
		if ((attr & LFARROW) || (attr & RTARROW) || (attr & HSLIDE) ||
				(attr & ALLSIZER) || (attr & SIZER))
			*h -= (bi_trindex[BIG_WIND][BW_HBAR].ob_height - 1);

		if (adr)
		{
			if ((adr[3].ob_type == G_TITLE) ||
					(adr[6].ob_type == G_TITLE))		/* Si c'est un menu */
			{
				*y += (adr[2].ob_height);
				*h -= (adr[2].ob_height);
			}
			else																/* Si c'est une ToolBar */
			{
				*y += (adr->ob_height);
				*h -= (adr->ob_height);
			}
		}
		(*y)++;
		(*h) -= 2;
	}
	else if (request == WC_BORDER)	/* Coord. zone travail -> Coord. fenˆtre */
	{
		*x = xd - 1;
		*y = yd - (bi_trindex[BIG_WIND][BW_MOVER].ob_height - 1);
		if (attr & INFO)
			*y -= (bi_trindex[BIG_WIND][BW_INFO].ob_height - 1);
		*w = wd + 2;
		if ((attr & UPARROW) || (attr & DNARROW) || (attr & VSLIDE))
			*w += (bi_trindex[BIG_WIND][BW_VBAR].ob_width - 1);
		*h = hd + (bi_trindex[BIG_WIND][BW_MOVER].ob_height - 1);
		if (attr & INFO)
			*h += (bi_trindex[BIG_WIND][BW_INFO].ob_height - 1);
		if ((attr & LFARROW) || (attr & RTARROW) || (attr & HSLIDE) ||
				(attr & ALLSIZER) || (attr & SIZER))
			*h += (bi_trindex[BIG_WIND][BW_HBAR].ob_height - 1);

		if (adr)
		{
			if ((adr[3].ob_type == G_TITLE) ||
					(adr[6].ob_type == G_TITLE))		/* Si c'est un menu */
			{
				*y -= (adr[2].ob_height);
				*h += (adr[2].ob_height);
			}
			else																/* Si c'est une ToolBar */
			{
				*y -= (adr->ob_height);
				*h += (adr->ob_height);
			}
		}
		(*y)--;
		(*h) += 2;
	}
}
/* #] bwind_calc () Equivalent de wind_calc du GEM :							*/ 
/* #[ draw_title () Redessine la barre de titre seulement :				*/
void draw_title (int index)
{
int x, y, ev, quit = FALSE, dummy;

	objc_offset (win[index].w_fen, BW_MOVER, &x, &y);
	form_dial (FMD_FINISH, ZERO, ZERO, ZERO, ZERO,
						 x, y,
						 win[index].w_fen[BW_MOVER].ob_width,
						 win[index].w_fen[BW_MOVER].ob_height);
	do
	{
		ev = evnt_multi ((MU_MESAG | MU_TIMER),
										 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										 buf, 10, 0, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy);
		if (ev & MU_MESAG)
		{
			if (buf[0] == WM_REDRAW)
			{
				if (buf[4] + buf[6] > bureau.xd + bureau.wd)
					buf[6] = bureau.xd + bureau.wd - buf[4];
				if (buf[5] + buf[7] > bureau.yd + bureau.hd)
					buf[7] = bureau.yd + bureau.hd - buf[5];
				redraw ();
			}
		}
		else if (ev == MU_TIMER)
			quit = TRUE;
	} while (! quit);
}
/* #] draw_title () Redessine la barre de titre seulement :				*/ 

