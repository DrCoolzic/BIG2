/*						>>> Big Is Gem <<<
				MODULE FONCTIONS DE GESTION DES FENETRES
*/
/****** En-tˆte BIG ***********************************************/
#include <BIG2.H>
#include "BIG2WIND.H"
#include "BIG2WINB.H"
#include "BIG2EVTW.H"
#include "BIG2FOR1.H"
#include "BIG2FOR2.H"
#include "BIG2EDIT.H"
extern int id_top;
/****** Fonctions g‚n‚rales pour les fenˆtres *********************/
/* #[ formw_draw () Dessine un formulaire en fenˆtre :						*/
int formw_draw (OBJECT *adr, int index, int title, int infos,
								 int wed, int attr, FNCP ricn)
{
int x, y, w, h, xf, yf, wf, hf, wh, i = ZERO,
		ok = FALSE, top, dummy, xw, yw;

	if (win[index].w_icon > BLANK)	/* Si la fenˆtre est ic“nifi‚e, */
		return FALSE;									/* on ne fait rien. */

	do		/* Chercher ‚ventuel objet B_MOVE et le cacher */
	{
		if ((adr[i].ob_type >> 8) == B_MOVE)
			adr[i].ob_flags |= HIDETREE;
	} while (NOT (adr[i++].ob_flags & LASTOB));
	adr->ob_state &= ~OUTLINED;					/* Forcer NOT OUTLINED */
	adr->ob_spec.obspec.framesize = -1;	/* 1 pixel … l'ext‚rieur */

	if (win[index].w_hg == BLANK)		/* Si fenˆtre jamais cr‚‚e */
	{
		x = adr->ob_x;				/* Positionner le formulaire. */
		y = adr->ob_y;				/* Il a ‚t‚ centr‚ par d‚faut */
		w = adr->ob_width;		/* par la fonction initial() */
		h = adr->ob_height;

			/* D‚duire les coordonn‚es de la fenˆtre */
		bwind_calc (WC_BORDER, attr, BLANK, ZERO,
								x, y, w, h, &xf, &yf, &wf, &hf);

		if (xf < bureau.xd)	/* Ne pas sortir du bureau */
			xf = bureau.xd;
		if (yf < bureau.yd)
			yf = bureau.yd;
		if (wf > bureau.wd)
			wf = bureau.wd;
		if (hf > bureau.hd)
			hf = bureau.hd;
		
		wh = wind_create (ZERO, xf, yf, wf, hf);	/* Cr‚er la fenˆtre */
		if (wh > BLANK)
		{		/* Initialiser toutes les valeurs de la fenˆtre */
			win[index].w_type = WTYP_FORM;
			win[index].w_attr = attr;

			win[index].w_curr.g_x = xf;
			win[index].w_curr.g_y = yf;
			win[index].w_curr.g_w = wf;
			win[index].w_curr.g_h = hf;
			win[index].w_wmini = min (160, w);				/* Calcul dimensions minimum */
			win[index].w_hmini = min (160, h);
			bwind_calc (WC_BORDER, attr, BLANK, ZERO,	/* Calcul dimensions maximum */
									x, y, w, h, &dummy, &dummy,
									&win[index].w_wmaxi, &win[index].w_hmaxi);

			win[index].w_wunit = 16;
			win[index].w_hunit = 16;

			win[index].w_cont.w_form.w_tree = adr;
			win[index].w_cont.w_form.w_edit = wed;
			win[index].w_cont.w_form.w_pos = ZERO;

			win[index].w_bar = (OBJECT *)ZERO;

			win[index].w_flags &= ~WFFULL;
			if (wed > BLANK)
				win[index].w_flags &= ~WFARROW;
			else if (wed == BLANK)
				win[index].w_flags |= WFARROW;
			win[index].w_mouse = ZERO;

			win[index].w_redicn = ricn;				/* Routine redraw si ic“ne */
			win[index].w_redraw = ZERO;
			win[index].w_close = closed;
			win[index].w_move = moved;
			win[index].w_size = size_wform;
			win[index].w_full = full_wform;

			if (title > BLANK)
				win[index].w_title = get_string (title);
			if (infos > BLANK)
				win[index].w_infos = get_string (infos);

			win[index].w_wtot = adr->ob_width;
			win[index].w_htot = adr->ob_height;
			win[index].w_lin = ZERO;
			win[index].w_col = ZERO;

				/* D‚duire les coordonn‚es de la zone de travail */
			bwind_calc (WC_WORK, attr, BLANK, ZERO,
									xf, yf, wf, hf, &xw, &yw, &dummy, &dummy);
			adr->ob_x = xw;
			adr->ob_y = yw;
			ok = TRUE;
		}
	}
	else if (win[index].w_hg == ZERO)	/* Si fenˆtre d‚j… cr‚‚e, puis ferm‚e */
	{
			/* Cr‚er la fenˆtre */
		wh = wind_create (ZERO,
											win[index].w_curr.g_x, win[index].w_curr.g_y,
											win[index].w_curr.g_w, win[index].w_curr.g_h);
		if (wh > BLANK)
		{
			xf = win[index].w_curr.g_x;
			yf = win[index].w_curr.g_y;
			wf = win[index].w_curr.g_w;
			hf = win[index].w_curr.g_h;

			if ((adr[wed].ob_type>> 8) == B_EDIT)
				win[index].w_cont.w_form.w_pos =
								min ((int)strlen (adr[wed].ob_spec.tedinfo->te_ptext),
										 (int)strlen (adr[wed].ob_spec.tedinfo->te_pvalid));
			else
				win[index].w_cont.w_form.w_pos =
						(int)strlen (get_text (win[index].w_cont.w_form.w_tree,
												 win[index].w_cont.w_form.w_edit));
			ok = TRUE;
		}
	}

	if (ok)
	{
		put_fen (index);
		win[index].w_fen[BW_BARRE].ob_flags |= HIDETREE;
		bak_rsc (win[index].w_cont.w_form.w_tree,
						 &win[index].w_cont.w_form.w_bak);	/* Enregistrer ‚tat form. */
		win[index].w_hg = wh;
		wind_open (wh, xf, yf, wf, hf);							/* Ouvrir la fenˆtre */

		set_palette (BLANK);
		wind_mouse ();
	}
	else if ((NOT ok) && (win[index].w_hg > ZERO))	/* Si elle est d‚j… ouverte */
	{
		wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
		if (top != win[index].w_hg)
		{
			dummy = buf[3];
			buf[3] = win[index].w_hg;
			(*win[index].w_top)();
			buf[3] = dummy;
		}
		ok = TRUE;
	}

	if (ok)
	{
		id_top = index;					/* Elle passe au premier plan */
		height_sliders (index, FALSE);
		return TRUE;
	}
	if (NOT ok)
	{
		big_alert (1, BLANK, bi_frstr[NOT_WINDOW], TRUE);
		return FALSE;
	}
	return FALSE;
}
/* #] formw_draw () Dessine un formulaire en fenˆtre :						*/ 
/* #[ dcl_windows () D‚clare et initialise les fenˆtres :					*/
void dcl_windows (int nbre)
{
int index;

		/* Reserver tableau de structures BigWind */
	win = (BigWind *)malloc (nbre * sizeof(BigWind));

	if (win)
	{
		for (index = ZERO ; index < nbre ; index++)
		{			/* Tous les ‚l‚ments en position "neutre" */
			win[index].w_hg = BLANK;
			win[index].w_type = WTYP_NORM;
			win[index].w_attr = WATR_CURRB;

			win[index].w_curr.g_x = ZERO;
			win[index].w_curr.g_y = ZERO;
			win[index].w_curr.g_w = 100;
			win[index].w_curr.g_h = 100;
			win[index].w_old.w_coord.g_x = ZERO;
			win[index].w_old.w_coord.g_y = ZERO;
			win[index].w_old.w_coord.g_w = ZERO;
			win[index].w_old.w_coord.g_h = ZERO;
			win[index].w_wmini = 100;
			win[index].w_hmini = 100;

			win[index].w_wunit = 1;
			win[index].w_hunit = 1;

			win[index].w_hprot = ZERO;
			win[index].w_vprot = ZERO;

			win[index].w_cont.w_adr = (char *)ZERO;
			win[index].w_pal = (Palette *)ZERO;

			win[index].w_bar = (OBJECT *)ZERO;

			win[index].w_flags = WFARROW;
			if (index == nbre - 1)
				win[index].w_flags |= WFLAST;
			win[index].w_mouse = ZERO;
			win[index].w_icon = BLANK;
			win[index].w_redicn = ZERO;

			win[index].w_redraw = ZERO;
			win[index].w_cycle = next_wind;
			win[index].w_top = topped;
			win[index].w_close = closed;
			win[index].w_move = moved;
			win[index].w_size = sized;
			win[index].w_full = fulled;
			win[index].w_hslid = hslid;
			win[index].w_vslid = vslid;
			win[index].w_uppage = uppage;
			win[index].w_dnpage = dnpage;
			win[index].w_lfpage = lfpage;
			win[index].w_rtpage = rtpage;
			win[index].w_upline = upline;
			win[index].w_dnline = dnline;
			win[index].w_lfline = lfline;
			win[index].w_rtline = rtline;

			win[index].w_title = (char *)ZERO;
			win[index].w_infos = (char *)ZERO;

			win[index].w_wtot = ZERO;
			win[index].w_htot = ZERO;
			win[index].w_lin = ZERO;
			win[index].w_col = ZERO;
		}
	}
}
/* #] dcl_windows () D‚clare et initialise les fenˆtres :					*/ 
/* #[ create_wmenu () Cr‚e un menu de fenˆtre :										*/
void create_wmenu (int tree, int index)
{
int nb_ob = ZERO, i, xb, yb, nb_titres = WM_BOXTITLES + 1, bmenu, xstate, oldy;
char *texte;
OBJECT *adr;

	adr = ir_trindex[tree];

	while (NOT (adr[nb_ob++].ob_flags & LASTOB)) ;	/* Compter les objets du menu */
	win[index].w_bar = (OBJECT *)malloc ((nb_ob + 3) * sizeof (OBJECT));
	win[index].w_type |= WTYP_MENU;

	memcpy (win[index].w_bar, adr, (2 * sizeof (OBJECT)));
	memcpy (&win[index].w_bar[2], bi_trindex[BIGARRM], (3 * sizeof (OBJECT)));
	memcpy (&win[index].w_bar[5], &adr[2], ((nb_ob - 2) * sizeof (OBJECT)));

	win[index].w_bar[WM_BKGR].ob_tail += 3;			/* G_IBOX de fond */
	win[index].w_bar[WM_BOXMENU].ob_next += 3;		/* G_BOX barre de menu */
	win[index].w_bar[WM_BOXMENU].ob_head = 2;
	win[index].w_bar[WM_BOXMENU].ob_tail = 5;
	win[index].w_bar[WM_BOXMENU].ob_spec.index = 0x1101L;
	win[index].w_bar[WM_BOXARROWS].ob_next = 5;	/* G_BOX boŒte des flˆches */
	win[index].w_bar[WM_BOXARROWS].ob_head = 3;
	win[index].w_bar[WM_BOXARROWS].ob_tail = 4;
	win[index].w_bar[WM_BOXARROWS].ob_height =
				win[index].w_bar[WM_BOXMENU].ob_height;
	win[index].w_bar[WM_LFARROW].ob_next = 4;		/* G_BOXCHAR flˆche gauche */
	win[index].w_bar[WM_LFARROW].ob_height =
				win[index].w_bar[WM_BOXMENU].ob_height;
	win[index].w_bar[WM_RTARROW].ob_next = 2;		/* G_BOXCHAR flˆche droite */
	win[index].w_bar[WM_RTARROW].ob_height =
				win[index].w_bar[WM_BOXMENU].ob_height;
	win[index].w_bar[WM_RTARROW].ob_flags &= ~LASTOB;
	win[index].w_bar[WM_BOXTITLES].ob_next = 1;	/* G_IBOX barre de titre */
	win[index].w_bar[WM_BOXTITLES].ob_head += 3;
	win[index].w_bar[WM_BOXTITLES].ob_tail += 3;

	win[index].w_fen[BW_BARRE].ob_height = win[index].w_bar[WM_BOXMENU].ob_height;
	oldy = win[index].w_fen[BW_WORK].ob_y;
	win[index].w_fen[BW_WORK].ob_y = (win[index].w_fen[BW_BARRE].ob_y + win[index].w_fen[BW_BARRE].ob_height);
	win[index].w_fen[BW_WORK].ob_height -= (win[index].w_fen[BW_WORK].ob_y - oldy);
	win[index].w_fen[BW_VBAR].ob_y = win[index].w_fen[BW_WORK].ob_y;
	win[index].w_fen[BW_VBAR].ob_height = win[index].w_fen[BW_WORK].ob_height;
	win[index].w_fen[BW_VELEV].ob_height -= (win[index].w_fen[BW_WORK].ob_y - oldy - 1);
	win[index].w_fen[BW_DNARROW].ob_y -= (win[index].w_fen[BW_WORK].ob_y - oldy - 1);
	if (win[index].w_flags & WFGROUP)
		win[index].w_fen[BW_UPARROW].ob_y =
				(win[index].w_fen[BW_DNARROW].ob_y - win[index].w_fen[BW_UPARROW].ob_height + 1);
	objc_offset (win[index].w_fen, BW_BARRE, &xb, &yb);
	win[index].w_bar[WM_BKGR].ob_x = xb;
	win[index].w_bar[WM_BKGR].ob_y = yb;
	win[index].w_bar[WM_BOXTITLES].ob_x =
				win[index].w_bar[WM_BOXARROWS].ob_width -
				win[index].w_bar[WM_BOXTITLES + 1].ob_x;
	i = WM_BOXTITLES + 1;			/* Chercher boŒte des menus */
	while (win[index].w_bar[i++].ob_type != G_IBOX) ;
	win[index].w_bar[i - 1].ob_x = win[index].w_bar[WM_BOXTITLES].ob_x -
				win[index].w_bar[i].ob_x;

	i = 6;
	do
	{
		if (win[index].w_bar[i].ob_next > ZERO)
			win[index].w_bar[i].ob_next += 3;
		if (win[index].w_bar[i].ob_head != -1)
			win[index].w_bar[i].ob_head += 3;
		if (win[index].w_bar[i].ob_tail != -1)
			win[index].w_bar[i].ob_tail += 3;
	} while (NOT (win[index].w_bar[i++].ob_flags & LASTOB));

		/* Mettre dans l'ob_flags ‚tendu des G_TITLEs le menu correspondant */
		/* Compter les G_TITLEs */
	while (win[index].w_bar[nb_titres++].ob_type == G_TITLE) ;
	nb_titres -= (WM_BOXTITLES + 2);
	if (nb_titres == 1)	/* S'il n'y en a qu'un, d‚sactiver les 2 flˆches */
		win[index].w_bar[WM_RTARROW].ob_state |= DISABLED;

	bmenu = win[index].w_bar[WM_BKGR].ob_tail;		/* Fond des menus */
	bmenu = win[index].w_bar[bmenu].ob_head;			/* 1ø boŒte de menus */
	xstate = bmenu << 8;
	win[index].w_bar[WM_BOXTITLES + 1].ob_state |= xstate;

	i = 2;
	while (win[index].w_bar[bmenu].ob_next > bmenu)
	{
		bmenu = win[index].w_bar[bmenu].ob_next;		/* BoŒte de menu suivante */
		xstate = bmenu << 8;
		win[index].w_bar[WM_BOXTITLES + i++].ob_state |= xstate;
	}

	i = 6;
	do
	{
		if ((win[index].w_bar[i].ob_type >> 8) == B_HIERM)
		{
			texte = (char *)(win[index].w_bar[i].ob_spec.userblk->ub_parm);
			win[index].w_bar[i].ob_type = G_STRING;
			win[index].w_bar[i].ob_type |= (B_HIERM << 8);
			win[index].w_bar[i].ob_spec.free_string = texte;
		}
	} while (NOT (win[index].w_bar[i++].ob_flags & LASTOB));
}
/* #] create_wmenu () Cr‚e un menu de fenˆtre :										*/ 
/* #[ menu_window () GŠre un menu de fenˆtre :										*/
int menu_window (int index, int xm, int ym)
{
GRECT rd, r;
int ob, xw, yw, ww, bmenu, pere, i, etat, retour, tree,
		nb_titres = WM_BOXTITLES + 1, titre1 = WM_BOXTITLES + 1,
		x, y, w, h, km, dummy, obj, old_obj = -1, tit,
		evnt, b[8], sortie = FALSE;
OBJECT *adr, *adrh;
MFDB img;

	objc_offset (win[index].w_fen, BW_BARRE, &xw, &yw);
	ww = win[index].w_fen[BW_WORK].ob_width - 1;
	if (NOT (win[index].w_fen[BW_VBAR].ob_flags & HIDETREE))
		ww += win[index].w_fen[BW_VBAR].ob_width;
	adr = win[index].w_bar;
	ob = objc_find (adr, WM_BOXARROWS, MAX_DEPTH, xm, ym);
	if (ob == BLANK)
		ob = objc_find (adr, WM_BOXTITLES, MAX_DEPTH, xm, ym);

	if ((xm > xw) && (xm < (xw + ww)))				/* Si on a cliqu‚ dans la zone */
	{
		if (NOT (adr[ob].ob_state & DISABLED) && NOT (adr[ob].ob_flags & HIDETREE))
		{																				/* et sur un objet */
			if (win[index].w_icon > BLANK)		/* Si la fenˆtre est iconifi‚e */
				return FALSE;		/* on s'en va. */

			while (adr[nb_titres++].ob_type == G_TITLE) ;		/* Compter les titres */
			nb_titres -= (WM_BOXTITLES + 2);
			while (adr[titre1++].ob_flags & HIDETREE) ;			/* Chercher 1ø titre */
			titre1--;

			create_hierm (adr);	/* FlŠches menus hi‚rarchiques */

			if (adr[ob].ob_type == G_BOXCHAR)			/* Si clic sur une flˆche */
			{
				if (ob == WM_LFARROW)					/* Si flˆche gauche */
				{
					titre1--;								/* Titre pr‚c‚dent */
					adr[titre1].ob_flags &= ~HIDETREE;
					adr[WM_RTARROW].ob_state &= ~DISABLED;	/* Activer flˆche droite */
					if (adr[titre1 - 1].ob_type == G_IBOX)	/* Si premier titre */
						adr[WM_LFARROW].ob_state |= DISABLED;	/* d‚sactiver fl. gauche */
				}
				else if (ob == WM_RTARROW)		/* Si flˆche droite */
				{
					adr[titre1].ob_flags |= HIDETREE;
					titre1++;								/* Titre suivant */
					adr[WM_LFARROW].ob_state &= ~DISABLED;	/* Activer flˆche gauche */
					if (adr[titre1].ob_next < titre1)				/* Si dernier titre */
						adr[WM_RTARROW].ob_state |= DISABLED;	/* d‚sactiver fl.droite */
				}
				adr[WM_BOXTITLES].ob_x = adr[WM_BOXARROWS].ob_width - adr[titre1].ob_x;
				i = WM_BOXTITLES + 1;			/* Chercher boŒte des menus */
				while (win[index].w_bar[i++].ob_type != G_IBOX) ;
				win[index].w_bar[i - 1].ob_x = win[index].w_bar[WM_BOXTITLES].ob_x -
							win[index].w_bar[i].ob_x;
				rd.g_x = xw;
				rd.g_y = yw;
				rd.g_w = ww;
				rd.g_h = adr[WM_BOXMENU].ob_height;
				wind_get (win[index].w_hg, WF_FIRSTXYWH,
									&r.g_x, &r.g_y, &r.g_w, &r.g_h);
				if (rc_intersect (&rd, &r))
					objc_draw (win[index].w_bar, WM_BOXMENU, MAX_DEPTH,
										 r.g_x, r.g_y, r.g_w, r.g_h);
			}
			else if (adr[ob].ob_type == G_TITLE)	/* Si clic sur un titre */
			{
				while (mk)
					graf_mkstate (&dummy, &dummy, &mk, &dummy); /* Relacher bouton */
				tit = ob;
				bmenu = adr[tit].ob_state >> 8;	/* Menu correspondant */
				w = adr[bmenu].ob_width;
				h = adr[bmenu].ob_height;
				adr[bmenu].ob_x = adr[tit].ob_x;
				adr[bmenu].ob_y = ZERO;
				objc_offset (adr, bmenu, &x, &y);
				if (x < (bureau.xd + 5))
				{
					adr[bmenu].ob_x += (bureau.xd + 5) - x;
					objc_offset (adr, bmenu, &x, &y);
				}
				if (x + w > bureau.xd + bureau.wd - 5)
				{
					adr[bmenu].ob_x -= ((x + w) - (bureau.xd + bureau.wd) + 5);
					objc_offset (adr, bmenu, &x, &y);
				}
				if (y + h > bureau.yd + bureau.hd - 5)
				{
					adr[bmenu].ob_y -= ((y + h) - (bureau.yd + bureau.hd) + 5);
					objc_offset (adr, bmenu, &x, &y);
				}

				wind_update (BEG_MCTRL);				/* Bloquer menu normal */
				win[index].w_bar[tit].ob_state |= SELECTED;
				draw_object (tit, index);
				get_bkgr (x, y, w, h, &img);
				objc_draw (adr, bmenu, MAX_DEPTH, x - 3, y - 3, w + 6, h + 6);

				do
				{
					evnt = evnt_multi (MU_BUTTON | MU_TIMER,
														 1, 1, 1, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO,
														 b, 10, 0, &xm, &ym, &km, &dummy, &dummy, &dummy);
					if (evnt & MU_BUTTON)
					{
						put_bkgr (x, y, w, h, &img);
						adr[bmenu].ob_x = adr[tit].ob_x;
						adr[bmenu].ob_y = ZERO;
						while (km)	/* Attend que le bouton de la souris soit relƒch‚ */
							graf_mkstate (&dummy, &dummy, &km, &dummy);
						sortie = TRUE;
					}
					else if (evnt == MU_TIMER)
					{
						graf_mkstate (&xm, &ym, &dummy, &dummy);
						obj = objc_find (adr, bmenu, MAX_DEPTH, xm, ym);
						if (obj == BLANK)
							obj = objc_find (adr, WM_BOXTITLES, 1, xm, ym);

						if ((adr[obj].ob_type == G_TITLE) && (xm < (xw + ww)))
						{
							if (old_obj != BLANK)
							{
								etat = adr[old_obj].ob_state & ~SELECTED;
								objc_change (adr, old_obj, ZERO, x, y, w, h, etat, TRUE);
								old_obj = BLANK;
							}
							if (obj != tit)
							{
								put_bkgr (x, y, w, h, &img);
								win[index].w_bar[tit].ob_state &= ~SELECTED;
								draw_object (tit, index);
								adr[bmenu].ob_x = adr[tit].ob_x;
								adr[bmenu].ob_y = ZERO;
								tit = obj;
								bmenu = adr[tit].ob_state >> 8;	/* Menu correspondant */
								w = adr[bmenu].ob_width;
								h = adr[bmenu].ob_height;
								adr[bmenu].ob_x = adr[tit].ob_x;
								adr[bmenu].ob_y = ZERO;
								objc_offset (adr, bmenu, &x, &y);
								if (x < (bureau.xd + 5))
								{
									adr[bmenu].ob_x += (bureau.xd + 5) - x;
									objc_offset (adr, bmenu, &x, &y);
								}
								if (x + w > bureau.xd + bureau.wd - 5)
								{
									adr[bmenu].ob_x -= ((x + w) - (bureau.xd + bureau.wd) + 5);
									objc_offset (adr, bmenu, &x, &y);
								}
								if (y + h > bureau.yd + bureau.hd - 5)
								{
									adr[bmenu].ob_y -= ((y + h) - (bureau.yd + bureau.hd) + 5);
									objc_offset (adr, bmenu, &x, &y);
								}
								win[index].w_bar[tit].ob_state |= SELECTED;
								draw_object (tit, index);
								get_bkgr (x, y, w, h, &img);
								objc_draw (adr, bmenu, MAX_DEPTH, x - 3, y - 3, w + 6, h + 6);
							}
						}
						else if (adr[obj].ob_type != G_TITLE)
						{
							pere = parent (adr, obj);
							if (old_obj != obj)
							{
								if ((old_obj != BLANK) && (NOT (adr[old_obj].ob_state & DISABLED)))
								{
									etat = adr[old_obj].ob_state & ~SELECTED;
									objc_change (adr, old_obj, ZERO, x, y, w, h, etat, TRUE);
								}
								if (pere == bmenu)
								{
									if (NOT (adr[obj].ob_state & DISABLED))
									{
										etat = adr[obj].ob_state | SELECTED;
										objc_change (adr, obj, ZERO, x, y, w, h, etat, TRUE);

										if ((adr[obj].ob_type >> 8) == B_HIERM)
										{
											if (adr[obj].ob_state & SELECTED)
											{
												retour = wind_hierarch (adr, index, obj);
												if (retour > ZERO)
												{
													event = MU_BUTTON;
													put_bkgr (x, y, w, h, &img);
													adr[bmenu].ob_x = adr[tit].ob_x;
													adr[bmenu].ob_y = ZERO;
													tree = adr[obj].ob_state >> 8;
													adrh = ir_trindex[tree];
													if ((adrh[buf[6]].ob_type >> 8) == B_FNCP)
													{
														(*fnc[adrh[buf[6]].ob_state >> 8])();
														empty (MU_KEYBD);
													}
													wind_update (END_MCTRL);		/* D‚bloquer menu */
													return event;
												}
												else if (retour == BLANK)
												{
													put_bkgr (x, y, w, h, &img);
													win[index].w_bar[tit].ob_state &= ~SELECTED;
													draw_object (tit, index);
													obj = BLANK;
													sortie = TRUE;
												}
											}
										}
									}
									old_obj = obj;
								}
								else
									old_obj = BLANK;
							}
						}
					}
				} while (NOT sortie);
				wind_update (END_MCTRL);		/* D‚bloquer menu normal */

				if ((old_obj != BLANK) && (obj != BLANK) && (NOT (adr[obj].ob_state & DISABLED)))
				{
					adr[obj].ob_state &= ~SELECTED;
					buf[0] = BEV_WMENU;
					buf[1] = ap_id;
					buf[3] = tit;
					buf[4] = obj;
					buf[5] = index;
					if ((win[index].w_bar[obj].ob_type >> 8) == B_FNCP)	/* Si FNCP */
					{
						(*fnc[win[index].w_bar[obj].ob_state >> 8])();
						empty (MU_KEYBD);
					}
					return evnt;
				}
				else
				{
					win[index].w_bar[tit].ob_state &= ~SELECTED;
					draw_object (tit, index);
					return FALSE;
				}
			}
		}
	}
	return FALSE;
}
/* #] menu_window () GŠre un menu de fenˆtre :										*/ 
/* #[ wind_hierarch () Menu hi‚rarchique en fenˆtre :							*/
int wind_hierarch (OBJECT *adr, int index, int option)
{
OBJECT *adrh, *old_adrh;
int x, y, w, h, xo, yo, xm, ym, km, b[8], evnt, ob_menu, menu, pxy[8],
		tree, dummy, ob = ZERO, old_ob = -1, sortie = FALSE, titre,
		xz, yz, etat, cote, op, nbre, fl_scroll = FALSE, xs, ys, cale = 1,
		naff, down, haut, i, xw, ww;
size_t taille;
char *text;
MOBLK zone;
MFDB ecran = {0}, image;

	evnt = evnt_timer (100, 0);			/* D‚lai 1/2 seconde */
	menu = parent (adr, option);
	graf_mkstate (&xm, &ym, &dummy, &dummy);
	if (objc_find (adr, menu, MAX_DEPTH, xm, ym) != option)
		return FALSE;				/* Si plus sur l'option, on sort */

	objc_offset (adr, option, &xo, &yo);	/* Position option */

	tree = adr[option].ob_state >> 8;
	adrh = ir_trindex[tree];

	objc_offset (adr, menu, &xz, &yz);

	titre = m_title (adr, option) + 3;

	zone.m_out = 1;		/* 0 : Entr‚e / 1 : Sortie */
	zone.m_x = xz;
	zone.m_y = yz;
	zone.m_w = adr[menu].ob_width;
	zone.m_h = adr[menu].ob_height;

	text = get_text (adr, option);
	if (text[strlen (text) - 2] == 3)
		x = adrh->ob_x = xo + adr[option].ob_width + 2;
	else if (text[strlen (text) - 2] == 4)
		x = adrh->ob_x = xo - adrh->ob_width - 5;
	y = adrh->ob_y = yo;
	w = adrh->ob_width;
	h = adrh->ob_height;
	if (y + h > bureau.yd + bureau.hd - 5)
		y = adrh->ob_y = (bureau.yd + bureau.hd) - h - 5;

	if (h > bureau.hd)													/* Si plus haut que le bureau */
	{
		if (y < bureau.yd)
			y = adrh->ob_y = bureau.yd + 15;
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
	image.fd_wdwidth = ((w + 3) / 16) + (((w + 3) % 16) != ZERO);
	image.fd_stand = 1;
	image.fd_nplanes = work_display.n_plane;
	pxy[0] = x - 1;
	pxy[1] = y - 1;
	pxy[2] = pxy[0] + w + 4;
	pxy[3] = pxy[1] + h + 4;
	pxy[4] = ZERO;
	pxy[5] = ZERO;
	pxy[6] = w + 4;
	pxy[7] = h + 4;
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
											 ZERO, ZERO, ZERO, ZERO, ZERO,
											 b, 10, 0, &xm, &ym, &km, &dummy, &dummy, &dummy);
		if (evnt & MU_BUTTON)
		{
			sortie = TRUE;
			while (km)	/* Attend que le bouton de la souris soit relƒch‚ */
				graf_mkstate (&dummy, &dummy, &km, &dummy);
			adr[option].ob_state &= ~0x00FF;
		}
		else if (evnt & MU_M1)
		{
			evnt = evnt_timer (50, 0);
			graf_mkstate (&xm, &ym, &dummy, &dummy);
			if (zone.m_out)
			{
				zone.m_out = 0;
				if (text[strlen (text) - 2] == 3)
					cote = (xm < zone.m_x);
				else if (text[strlen (text) - 2] == 4)
					cote = (xm > zone.m_x + zone.m_w);
				if (cote)
					sortie = TRUE;
				else
					adr[option].ob_state |= SELECTED;
			}
			else
			{
				ob_menu = objc_find (adr, menu, MAX_DEPTH, xm, ym);
				if (ob_menu != option)
				{
					etat = adr[option].ob_state & ~SELECTED;
					objc_change (adr, option, ZERO, xo, yo,
											 adr[option].ob_width,
											 adr[option].ob_height, etat, TRUE);
					adrh[ob].ob_state &= ~SELECTED;
					evnt = ZERO;
					sortie = TRUE;
				}
				else
				{
					zone.m_out = 1;
				}
			}
		}
		else if (evnt == MU_TIMER)
		{
			graf_mkstate (&xm, &ym, &dummy, &dummy);
			ob = objc_find (adrh, ROOT, MAX_DEPTH, xm, ym);

			if (ob == BLANK)
			{
				op = objc_find (adr, menu, MAX_DEPTH, xm, ym);
				if ((op > BLANK) && (op != option) && (op > menu))	/* Si autre option */
				{
					etat = adr[option].ob_state & ~SELECTED;
					objc_change (adr, option, ZERO, xo, yo,
											 adr[option].ob_width,
											 adr[option].ob_height, etat, TRUE);
					ob = ZERO;
					sortie = TRUE;
				}
				else
				{
					objc_offset (win[index].w_fen, BW_BARRE, &xw, &dummy);
					ww = win[index].w_fen[BW_WORK].ob_width - 1;
					if (NOT (win[index].w_fen[BW_VBAR].ob_flags & HIDETREE))
						ww += win[index].w_fen[BW_VBAR].ob_width;
					if ((xm > xw) && (xm < (xw + ww)))				/* Si on a cliqu‚ dans la zone */
					{
						op = objc_find (adr, WM_BOXTITLES, MAX_DEPTH, xm, ym);
						if ((op > BLANK) && (adr[op].ob_type == G_TITLE))
						{
							sortie = TRUE;
							evnt = MU_TIMER;
							ob = ZERO;
							etat = adr[option].ob_state & ~SELECTED;
							objc_change (adr, option, ZERO, xo, yo,
													 adr[option].ob_width,
													 adr[option].ob_height, etat, TRUE);
						}
					}
				}
			}

			if (NOT sortie)
			{
				ob = objc_find (adrh, ROOT, MAX_DEPTH, xm, ym);
				if (old_ob != ob)		/* Si objet diff‚rent */
				{
					if ((old_ob != -1) && (NOT (adrh[old_ob].ob_state & DISABLED)))
					{
						etat = adrh[old_ob].ob_state & ~SELECTED;
						objc_change (adrh, old_ob, ZERO, x, y, w, h, etat, TRUE);
					}
					if ((ob > ROOT) && (NOT (adrh[ob].ob_state & DISABLED)))
					{
						etat = adrh[ob].ob_state | SELECTED;
						objc_change (adrh, ob, ZERO, x, y, w, h, etat, TRUE);
					}
					old_ob = ob;
				}
				else								/* Si mˆme objet */
				{
					if ((fl_scroll) && (ob == 1))
					{		/* Si scroll : flˆche vers le haut */
						objc_offset (adrh, ROOT, &xs, &ys);
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
							pxy[0] = xs;
							pxy[1] = ys + haut;
							pxy[2] = pxy[0] + adrh->ob_width;
							pxy[3] = ys + (naff * haut);
							pxy[4] = pxy[0];
							pxy[5] = ys + (2 * haut);
							pxy[6] = pxy[2];
							pxy[7] = ys + ((naff + 1) * haut);
							v_hide_c (work_display.handle);
							objc_change (adrh, ob, ZERO, x, y, w, h, NORMAL, TRUE);
							vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecran, &ecran);
							objc_change (adrh, ob, ZERO, x, y, w, h, SELECTED, TRUE);
							v_show_c (work_display.handle, TRUE);
							objc_draw (adrh, ROOT, 2,
												 x + adrh[2].ob_x, y + adrh[2].ob_y,
												 adrh[2].ob_width, adrh[2].ob_height);
						}
					}
					else if ((fl_scroll) && (ob == down))
					{		/* Si scroll : flˆche vers le bas */
						objc_offset (adrh, ROOT, &xs, &ys);
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
							pxy[0] = xs;
							pxy[1] = ys + (2 * haut);
							pxy[2] = pxy[0] + adrh->ob_width;
							pxy[3] = ys + ((naff + 1) * haut);
							pxy[4] = pxy[0];
							pxy[5] = ys + haut;
							pxy[6] = pxy[2];
							pxy[7] = ys + (naff * haut);
							v_hide_c (work_display.handle);
							objc_change (adrh, ob, ZERO, x, y, w, h, NORMAL, TRUE);
							vro_cpyfm (work_display.handle, S_ONLY, pxy, &ecran, &ecran);
							objc_change (adrh, ob, ZERO, x, y, w, h, SELECTED, TRUE);
							v_show_c (work_display.handle, TRUE);
							objc_draw (adrh, ROOT, 2,
												 x + adrh[naff + 1].ob_x, y + adrh[naff + 1].ob_y,
												 adrh[naff + 1].ob_width, adrh[naff + 1].ob_height);
						}
					}
				}
			}
		}
	} while (NOT sortie);

	pxy[4] = x - 1;
	pxy[5] = y - 1;
	pxy[6] = pxy[4] + w + 4;
	pxy[7] = pxy[5] + h + 4;
	pxy[0] = ZERO;
	pxy[1] = ZERO;
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

	if ((ob > ZERO) && (NOT (adrh[ob].ob_state & DISABLED)) && (evnt))
	{
		evnt = TRUE;
		buf[0] = BEV_WHIER;
		buf[1] = ap_id;
		buf[3] = titre;
		buf[4] = option;
		buf[5] = tree;
		buf[6] = ob;
		buf[7] = index;
		etat = adrh[buf[6]].ob_state & 0xFF00;
		objc_change (adrh, buf[6], ZERO, x, y, w, h, etat, FALSE);
		return evnt;
	}
	 else if ((((evnt & MU_BUTTON) || (evnt == MU_TIMER)) && (ob < ZERO)) ||
						((((evnt & MU_BUTTON) || (evnt == MU_TIMER)) && (ob > ZERO) && (adrh[ob].ob_state & DISABLED))))
		return BLANK;
	else if ((evnt == MU_TIMER) && (ob == ZERO))
		return ZERO;
	else
		return FALSE;
}
/* #] wind_hierarch () Menu hi‚rarchique en fenˆtre :							*/ 
/* #[ open_window () Ouvre une fenˆtre :													*/
int		open_window (int index, int type, int attr,
									 int x, int y, int w, int h,
									 int wmi, int hmi, int wma, int hma,
									 int wu, int hu,
									 int flags, int fmouse,
									 FNCP ricn, FNCP redr, FNCP clos,
									 char *title, char *info,
									 long wt, long ht,
									 int fmenu, int ftool, int bar,
									 int pattern, int colorp)
{
int wh, top, ok = FALSE, retrace = FALSE, dummy, xb, yb,
		oldy, oldw, oldh, xw, yw, ww, hw, temp;

	if (win[index].w_icon > BLANK)	/* Si la fenˆtre est ic“nifi‚e, */
		return FALSE;									/* on ne fait rien. */

	if (win[index].w_hg == BLANK)		/* Si fenˆtre jamais ouverte */
	{						/* Initialiser la structure */
		win[index].w_type = type;					/* Type de fenˆtre */
		win[index].w_attr = attr;					/* Attributs de fenˆtre */
		win[index].w_curr.g_x = x;				/* Position fenˆtre */
		win[index].w_curr.g_y = y;
		win[index].w_curr.g_w = w;				/* Dimensions fenˆtre */
		win[index].w_curr.g_h = h;

			/* Calcul dimensions minimum */
		bwind_calc (WC_BORDER, attr, bar, ZERO,
								x, y, wmi, hmi, &dummy, &dummy, &wmi, &hmi);
		wmi = max (wmi, 150);
		win[index].w_wmini = wmi;					/* Largeur mini */
		win[index].w_hmini = hmi;					/* Hauteur mini */
		if (w < wmi)												/* Correction eventuelle */
			win[index].w_curr.g_w = w = wmi;	/* si les coordonn‚es */
		if (h < hmi)												/* de d‚part sont inf‚rieures */
			win[index].w_curr.g_h = h = hmi;	/* aux minimales. */

			/* Calcul dimensions maximum */
		if (wma > BLANK)
		{
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									x, y, wma, h, &dummy, &dummy, &wma, &dummy);
			win[index].w_wmaxi = wma;
		}
		else
			win[index].w_wmaxi = (bureau.wd * 2);
		if (hma > BLANK)
		{
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									x, y, w, hma, &dummy, &dummy, &dummy, &hma);
			win[index].w_hmaxi = hma;
		}
		else
			win[index].w_hmaxi = (bureau.hd * 2);
		if (w > win[index].w_wmaxi)					/* Correction eventuelle */
			win[index].w_curr.g_w = w = wma;	/* si les coordonn‚es */
		if (h > win[index].w_hmaxi)					/* de d‚part sont sup‚rieures */
			win[index].w_curr.g_h = h = hma;	/* aux maximales. */

		if (type == WTYP_PICT)
		{		/* Si fenˆtre image : pas plus grande que la taille totale */
			bwind_calc (WC_WORK, attr, bar, ZERO,
									x, y, w, h, &xw, &yw, &ww, &hw);
			if (ww > wt)
				ww = (int)wt;
			if (hw > ht)
				hw = (int)ht;
			if (ftool)
				hw++;
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									xw, yw, ww, hw, &x, &y, &w, &h);
			win[index].w_curr.g_w = w;
			win[index].w_curr.g_h = h;
		}

		win[index].w_wunit = wu;					/* Unit‚ largeur */
		win[index].w_hunit = hu;					/* Unit‚ hauteur */

			/* Mise en place des flags */
		if (flags & WFARROW)							/* Flag clavier */
			win[index].w_flags |= WFARROW;	/* Gestion au clavier */
		else
			win[index].w_flags &= ~WFARROW;	/* Pas gestion au clavier */

		if (flags & WFCALAG)							/* Calage taille et sliders */
		{
			win[index].w_flags |= WFCALAG;	/* Calage taille et sliders sur unit‚s */
			bwind_calc (WC_WORK, attr, bar, ZERO,
									x, y, w, h, &xw, &yw, &ww, &hw);
			ww = ww / wu * wu;
			hw = hw / hu * hu;
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									xw, yw, ww, hw,
									&win[index].w_curr.g_x, &win[index].w_curr.g_y,
									&win[index].w_curr.g_w, &win[index].w_curr.g_h);
		}
		else
			win[index].w_flags &= ~WFCALAG;

		if (flags & WFDECAL)							/* Ajuster w_lin et w_col */
			win[index].w_flags |= WFDECAL;	/* sur ‚v‚nement Sizer. */
		else
			win[index].w_flags &= ~WFDECAL;

		if (flags & WFGROUP)							/* Flag groupement flŠches */
			win[index].w_flags |= WFGROUP;	/* Groupement */
		else
			win[index].w_flags &= ~WFGROUP;	/* Pas groupement */

		if (fmouse > ZERO)
			win[index].w_mouse = fmouse;

		win[index].w_redicn = ricn;				/* Routine redraw si ic“ne */
		win[index].w_redraw = redr;				/* Routine redraw normale */
		win[index].w_close = clos;				/* Routine de fermeture */
		if (strlen (title))
			win[index].w_title = title;			/* Titre */
		if (strlen (info))
			win[index].w_infos = info;			/* Ligne infos */

				/* Cr‚ation fenˆtre */
		if ((wh = wind_create (ZERO,
											win[index].w_curr.g_x, win[index].w_curr.g_y,
											win[index].w_curr.g_w, win[index].w_curr.g_h)) > BLANK)
			ok = TRUE;
	}
	else if (win[index].w_hg == ZERO)	/* Si fenˆtre ouverte puis ferm‚e */
	{
		win[index].w_wunit = wu;		/* Rafraichir les paramŠtres */
		win[index].w_hunit = hu;
		win[index].w_mouse = fmouse;
		win[index].w_redicn = ricn;
		win[index].w_redraw = redr;
		win[index].w_close = clos;
		win[index].w_wtot = wt;
		win[index].w_htot = ht;
		win[index].w_flags = (win[index].w_flags & WFLAST) | flags;
			/* Calcul dimensions minimum */
		bwind_calc (WC_BORDER, attr, bar, ZERO,
								x, y, wmi, hmi, &dummy, &dummy, &wmi, &hmi);
		wmi = max (wmi, 150);
		win[index].w_wmini = wmi;					/* Largeur mini */
		win[index].w_hmini = hmi;					/* Hauteur mini */
		if (win[index].w_curr.g_w < wmi)			/* Correction eventuelle */
			win[index].w_curr.g_w = w = wmi;		/* si les coordonn‚es */
		if (win[index].w_curr.g_h < hmi)			/* de d‚part sont inf‚rieures */
			win[index].w_curr.g_h = h = hmi;		/* aux minimales. */

		if (wma > BLANK)	/* Calcul dimensions maximum */
		{
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									x, y, wma, h, &dummy, &dummy, &wma, &dummy);
			win[index].w_wmaxi = wma;
		}
		else
			win[index].w_wmaxi = (bureau.wd * 2);
		if (hma > BLANK)
		{
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									x, y, w, hma, &dummy, &dummy, &dummy, &hma);
			win[index].w_hmaxi = hma;
		}
		else
			win[index].w_hmaxi = (bureau.hd * 2);
		if (win[index].w_curr.g_w > win[index].w_wmaxi)			/* Correction eventuelle */
			win[index].w_curr.g_w = w = win[index].w_wmaxi;		/* si les coordonn‚es */
		if (win[index].w_curr.g_h > win[index].w_hmaxi)			/* de d‚part sont sup‚rieures */
			win[index].w_curr.g_h = h = win[index].w_hmaxi;		/* aux maximales. */
		temp = win[index].w_fen[BW_WORK].ob_state & 0x0FFF;
		win[index].w_fen[BW_WORK].ob_state = (temp | (pattern << 12));
		temp = win[index].w_fen[BW_WORK].ob_state & 0xF0FF;
		win[index].w_fen[BW_WORK].ob_state = (temp | (colorp << 8));

		if (type == WTYP_PICT)
		{		/* Si fenˆtre image : pas plus grande que la taille totale */
			bwind_calc (WC_WORK, attr, bar, ZERO,
									win[index].w_curr.g_x, win[index].w_curr.g_y,
									win[index].w_curr.g_w, win[index].w_curr.g_h,
									&xw, &yw, &ww, &hw);
			if (ww > wt)
				ww = (int)wt;
			if (hw > ht)
				hw = (int)ht;
			if (win[index].w_type & WTYP_TOOL)
				hw++;
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									xw, yw, ww, hw,
									&win[index].w_curr.g_x, &win[index].w_curr.g_y,
									&win[index].w_curr.g_w, &win[index].w_curr.g_h);
		}

				/* Cr‚ation fenˆtre */
		if ((wh = wind_create (ZERO,
											win[index].w_curr.g_x, win[index].w_curr.g_y,
											win[index].w_curr.g_w, win[index].w_curr.g_h)) > BLANK)
			ok = TRUE;
	}

	if (ok)																		/* Si elle vient d'ˆtre cr‚‚e */
	{
		put_fen (index);
			/* Trame et couleur zone de travail */
		temp = win[index].w_fen[BW_WORK].ob_state & 0x0FFF;
		win[index].w_fen[BW_WORK].ob_state = (temp | (pattern << 12));
		temp = win[index].w_fen[BW_WORK].ob_state & 0xF0FF;
		win[index].w_fen[BW_WORK].ob_state = (temp | (colorp << 8));

		win[index].w_wtot = wt;		/* Largeur totale */
		win[index].w_htot = ht;		/* Hauteur totale */

		win[index].w_hg = wh;			/* Enregistrer handle GEM */
		if (fmenu)										/* Si fenˆtre menu */
			create_wmenu (bar, index);	/* Cr‚er le menu */
		else if (ftool)								/* Si fenˆtre ToolBar */
		{
			win[index].w_type |= WTYP_TOOL;
			win[index].w_bar = ir_trindex[bar];
			win[index].w_bar->ob_state &= ~OUTLINED;					/* Pas Outline */
			win[index].w_bar->ob_spec.obspec.framesize = 0;		/* Pas de cadre */
			win[index].w_bar->ob_spec.obspec.textmode = 1;		/* Objet racine opaque */
			win[index].w_bar->ob_width = (bureau.wd * 2);

			win[index].w_fen[BW_BARRE].ob_height = win[index].w_bar->ob_height;
			oldy = win[index].w_fen[BW_WORK].ob_y;
			win[index].w_fen[BW_WORK].ob_y = (win[index].w_fen[BW_BARRE].ob_y + win[index].w_fen[BW_BARRE].ob_height);
			win[index].w_fen[BW_WORK].ob_height -= (win[index].w_fen[BW_WORK].ob_y - oldy);
			win[index].w_fen[BW_VBAR].ob_y = win[index].w_fen[BW_WORK].ob_y;
			win[index].w_fen[BW_VBAR].ob_height = win[index].w_fen[BW_WORK].ob_height;
			win[index].w_fen[BW_VELEV].ob_height -= (win[index].w_fen[BW_WORK].ob_y - oldy - 1);
			win[index].w_fen[BW_DNARROW].ob_y -= (win[index].w_fen[BW_WORK].ob_y - oldy - 1);
			if (win[index].w_flags & WFGROUP)
				win[index].w_fen[BW_UPARROW].ob_y =
						(win[index].w_fen[BW_DNARROW].ob_y - win[index].w_fen[BW_UPARROW].ob_height + 1);
			objc_offset (win[index].w_fen, BW_BARRE, &xb, &yb);
			win[index].w_bar->ob_x = xb;
			win[index].w_bar->ob_y = yb;
		}
		else		/* Si ni menu, ni ToolBar */
			win[index].w_fen[BW_BARRE].ob_flags |= HIDETREE;

					/* Ouvrir la fenˆtre */
		wind_open (wh, win[index].w_curr.g_x, win[index].w_curr.g_y,
							 win[index].w_curr.g_w, win[index].w_curr.g_h);
	}
	else if ((NOT ok) && (win[index].w_hg > ZERO))	/* Si elle est d‚j… ouverte */
	{
		win[index].w_wunit = wu;		/* Rafraichir les paramŠtres */
		win[index].w_hunit = hu;
		win[index].w_mouse = fmouse;
		win[index].w_redicn = ricn;
		win[index].w_redraw = redr;
		win[index].w_close = clos;
		win[index].w_wtot = wt;
		win[index].w_htot = ht;
			/* Calcul dimensions minimum */
		bwind_calc (WC_BORDER, attr, bar, ZERO,
								x, y, wmi, hmi, &dummy, &dummy, &wmi, &hmi);
		wmi = max (wmi, 150);
		win[index].w_wmini = wmi;					/* Largeur mini */
		win[index].w_hmini = hmi;					/* Hauteur mini */
		if (win[index].w_curr.g_w < wmi)			/* Correction eventuelle */
			win[index].w_curr.g_w = w = wmi;		/* si les coordonn‚es */
		if (win[index].w_curr.g_h < hmi)			/* de d‚part sont inf‚rieures */
			win[index].w_curr.g_h = h = hmi;		/* aux minimales. */

		if (wma > BLANK)	/* Calcul dimensions maximum */
		{
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									x, y, wma, h, &dummy, &dummy, &wma, &dummy);
			win[index].w_wmaxi = wma;
		}
		else
			win[index].w_wmaxi = (bureau.wd * 2);
		if (hma > BLANK)
		{
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									x, y, w, hma, &dummy, &dummy, &dummy, &hma);
			win[index].w_hmaxi = hma;
		}
		else
			win[index].w_hmaxi = (bureau.hd * 2);
		if (win[index].w_curr.g_w > win[index].w_wmaxi)			/* Correction eventuelle */
			win[index].w_curr.g_w = w = win[index].w_wmaxi;		/* si les coordonn‚es */
		if (win[index].w_curr.g_h > win[index].w_hmaxi)			/* de d‚part sont sup‚rieures */
			win[index].w_curr.g_h = h = win[index].w_hmaxi;		/* aux maximales. */
		temp = win[index].w_fen[BW_WORK].ob_state & 0x0FFF;
		win[index].w_fen[BW_WORK].ob_state = (temp | (pattern << 12));
		temp = win[index].w_fen[BW_WORK].ob_state & 0xF0FF;
		win[index].w_fen[BW_WORK].ob_state = (temp | (colorp << 8));

		if (type == WTYP_PICT)
		{		/* Si fenˆtre image : pas plus grande que la taille totale */
			oldw = win[index].w_fen->ob_width;
			oldh = win[index].w_fen->ob_height;
			bwind_calc (WC_WORK, attr, bar, ZERO,
									win[index].w_curr.g_x, win[index].w_curr.g_y,
									win[index].w_curr.g_w, win[index].w_curr.g_h,
									&xw, &yw, &ww, &hw);
			if (ww > wt)
				ww = (int)wt;
			if (hw > ht)
				hw = (int)ht;
			bwind_calc (WC_BORDER, attr, bar, ZERO,
									xw, yw, ww, hw,
									&win[index].w_curr.g_x, &win[index].w_curr.g_y,
									&win[index].w_curr.g_w, &win[index].w_curr.g_h);
			wind_set (win[index].w_hg, WF_CURRXYWH,
								win[index].w_curr.g_x, win[index].w_curr.g_y,
								win[index].w_curr.g_w, win[index].w_curr.g_h);
			win[index].w_fen->ob_width = win[index].w_curr.g_w;
			win[index].w_fen->ob_height = win[index].w_curr.g_h;
			resize (index, win[index].w_curr.g_w - oldw, win[index].w_curr.g_h - oldh);
			retrace = TRUE;
		}

		wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
		if (top != win[index].w_hg)
		{
			dummy = buf[3];
			buf[3] = win[index].w_hg;
			(*win[index].w_top)();
			buf[3] = dummy;
		}
		print_page (index);
		ok = TRUE;
	}

	if (ok)						/* Si la fenˆtre est ouverte */
	{
		id_top = index;					/* Elle passe au premier plan */
		height_sliders (index, FALSE);	/* R‚gler taille sliders */
		if (retrace)
			print_wind (index);

		if (((win[index].w_type & 0xFF) == WTYP_PICT)
		|| ((win[index].w_type & 0xFF) == WTYP_FORM))
			set_palette (index);
		else
			set_palette (BLANK);

		wind_mouse ();
	}
	else
		big_alert (1, BLANK, bi_frstr[NOT_WINDOW], TRUE);

	return ok;
}
/* #] open_window () Ouvre une fenˆtre :													*/ 
/* #[ find_index () Cherche index BIG fenˆtre GEM :								*/
int find_index (int wh)
{
register int i = ZERO;

	if (wh == ZERO)
		return BLANK;

	if (wh > ZERO)	/* Si on cherche une fenˆtre */
	{
		do
		{
			if (wh == win[i].w_hg)
				return i;
		} while (NOT (win[i++].w_flags & WFLAST));
	}
	else						/* Si on cherche premiŠre fenˆtre libre */
	{
		do
		{
			if (win[i].w_hg == BLANK)
				return i;
		} while (NOT (win[i++].w_flags & WFLAST));
	}
	return BLANK;
}
/* #] find_index () Cherche index BIG fenˆtre GEM :								*/ 
/* #[ find_window () Cherche si on est dans la zone de travail :	*/
int find_window (int mx, int my)
{
int xw, yw, ww, hw, fenetre, index;

	fenetre = wind_find (mx, my);	/* On a cliqu‚ sur une fenˆtre ? */
	if (fenetre)										/* Si oui */
	{
		index = find_index (fenetre);							/* Prendre l'index */
		if (index > BLANK)
		{
			zone_work (index, &xw, &yw, &ww, &hw);		/* Zone de travail */
			if ((mx > xw) && (mx < (xw + ww)))				/* Si on a cliqu‚ */
				if ((my > yw) && (my < (yw + hw)))			/* dans la zone */
					return fenetre;												/* retourner Handle GEM */
		}
	}
	return FALSE;																/* Sinon FALSE */
}
/* #] find_window () Cherche si on est dans la zone de travail :	*/ 
/* #[ zone_work () Retourne la zone de travail :									*/
void zone_work (int index, int *xw, int *yw, int *ww, int *hw)
{
	if (win[index].w_icon == BLANK)
	{
		bwind_calc (WC_WORK, win[index].w_attr,
								 BLANK, win[index].w_bar,
								 win[index].w_curr.g_x, win[index].w_curr.g_y,
								 win[index].w_curr.g_w, win[index].w_curr.g_h,
								 xw, yw, ww, hw);
		if ((win[index].w_type & WTYP_TOOL) || (win[index].w_type & WTYP_MENU))
		{
			(*yw)++;
			(*hw)--;
		}
	}
	else
	{
		wind_get (win[index].w_hg, WF_WORKXYWH, xw, yw, ww, hw);
		*yw += win[index].w_fen[BW_MOVER].ob_height;
		*hw -= win[index].w_fen[BW_MOVER].ob_height;
		if ((win[index].w_bar) && (NOT win[index].w_redicn))
		{
			*yw += win[index].w_fen[BW_BARRE].ob_height;
			*hw -= win[index].w_fen[BW_BARRE].ob_height;
		}
	}
}
/* #] zone_work () Retourne la zone de travail :									*/ 
/* #[ wind_mouse () Ajuste les ‚v‚nement de zone souris :					*/
void wind_mouse (void)
{
int top, index, dummy;

	wind_get (0, WF_TOP, &top, &dummy, &dummy, &dummy);
	if (top)
	{
		index = find_index (top);
		if ((index > BLANK) && (win[index].w_mouse) && (win[index].w_icon == BLANK))
		{
			mouse.m_out = ZERO;
			zone_work (index, &mouse.m_x, &mouse.m_y, &mouse.m_w, &mouse.m_h);
			return;
		}
	}
	mouse.m_out = ZERO;
	mouse.m_x = mouse.m_y = mouse.m_w = mouse.m_h = ZERO;
	graf_mouse (ARROW, ZERO);							/* Souris -> Flˆche */
}
/* #] wind_mouse () Ajuste les ‚v‚nement de zone souris :					*/ 
/* #[ height_vert () Taille slider vertical :											*/
void height_vert (int index, int fredraw)
{
int xw, yw, ww, hw, ha, hs, hcell, mini, dummy;

	zone_work (index, &xw, &yw, &ww, &hw);	/* Coordonn‚es zone de travail */
	ha = win[index].w_fen[BW_VELEV].ob_height;	/* Hauteur ascenseur */
	hs = (int)((long)ha * (long)hw / (long)win[index].w_htot);
	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &dummy, &hcell);
	mini = hcell > 8 ? 16 : 8;
	if (hs < mini)
		hs = mini;
	if (hs > ha)
		hs = ha;
	win[index].w_fen[BW_VSLIDE].ob_height = hs;

	win[index].w_lin = min (win[index].w_lin, win[index].w_htot - hw);
	win[index].w_lin = max (ZERO, win[index].w_lin);

	if ((win[index].w_type & 0xFF) != WTYP_NORM)
		if (win[index].w_lin + hw > win[index].w_htot)
			win[index].w_lin = win[index].w_htot - hw;
	if (fredraw)
		draw_obj (win[index].w_fen, BW_VELEV);
}
/* #] height_vert () Taille slider vertical :											*/ 
/* #[ height_hori () Taille slider horizontal :										*/
void height_hori (int index, int fredraw)
{
int xw, yw, ww, hw, wa, ws;

	zone_work (index, &xw, &yw, &ww, &hw);
	wa = win[index].w_fen[BW_HELEV].ob_width;	/* Largeur ascenseur */
	ws = (int)((long)wa * (long)ww / (long)win[index].w_wtot);
	if (ws < 16)
		ws = 16;
	if (ws > wa)
		ws = wa;
	win[index].w_fen[BW_HSLIDE].ob_width = ws;

	win[index].w_col = min (win[index].w_col, win[index].w_wtot - ww);
	win[index].w_col = max (ZERO, win[index].w_col);

	if ((win[index].w_type & 0xFF) != WTYP_NORM)
		if (win[index].w_col + ww > win[index].w_wtot)
			win[index].w_col = win[index].w_wtot - ww;
	if (fredraw)
		draw_obj (win[index].w_fen, BW_HELEV);
}
/* #] height_hori () Taille slider horizontal :										*/ 
/* #[ posi_vert () Pos. slider vertical :													*/
void posi_vert (int index, int fredraw)
{
int xw, yw, ww, hw, ha, slide;

	zone_work (index, &xw, &yw, &ww, &hw);

	if (win[index].w_htot > hw)
	{
		ha = win[index].w_fen[BW_VELEV].ob_height -	/* Hauteur ascenseur */
				 win[index].w_fen[BW_VSLIDE].ob_height;
		slide = (int)((long)win[index].w_lin * (long)ha /
						(long)(win[index].w_htot - hw));
		slide = max (min (slide, ha), ZERO);
	}
	else
		slide = ZERO;
	
	win[index].w_fen[BW_VSLIDE].ob_y = slide;
	if (fredraw)
		draw_obj (win[index].w_fen, BW_VELEV);
}
/* #] posi_vert () Pos. slider vertical :													*/ 
/* #[ posi_hori () Pos. slider horizontal :												*/
void posi_hori (int index, int fredraw)
{
int xw, yw, ww, hw, wa, slide;

	zone_work (index, &xw, &yw, &ww, &hw);

	if (win[index].w_wtot > ww)
	{
		wa = win[index].w_fen[BW_HELEV].ob_width -	/* Largeur ascenseur */
				 win[index].w_fen[BW_HSLIDE].ob_width;
		slide = (int)((long)win[index].w_col * (long)wa /
						(long)(win[index].w_wtot - ww));
		slide = max (min (slide, wa), ZERO);
	}
	else
		slide = ZERO;

	win[index].w_fen[BW_HSLIDE].ob_x = slide;
	if (fredraw)
		draw_obj (win[index].w_fen, BW_HELEV);
}
/* #] posi_hori () Pos. slider horizontal :												*/ 
/* #[ height_sliders () taille/Position sliders : 								*/
void height_sliders (int index, int fredraw)
{
	if (win[index].w_hg <= ZERO)
		return;
	if (win[index].w_attr & VSLIDE)
	{
		height_vert (index, FALSE);
		posi_vert (index, FALSE);
		if (fredraw)
			draw_obj (win[index].w_fen, BW_VELEV);
	}
	if (win[index].w_attr & HSLIDE)
	{
		height_hori (index, FALSE);
		posi_hori (index, FALSE);
		if (fredraw)
			draw_obj (win[index].w_fen, BW_HELEV);
	}
}
/* #] height_sliders () taille/Position sliders : 								*/ 
/* #[ draw_object () Dessin objet dans fenˆtre formulaire :				*/
void draw_object (int object, int index)
{
GRECT rd, r, z;
OBJECT *adr;

	if (win[index].w_hg <= ZERO)
		return;
	if ((win[index].w_type & WTYP_MENU) || (win[index].w_type & WTYP_TOOL))
		adr = win[index].w_bar;
	else if (win[index].w_type == WTYP_FORM)
		adr = win[index].w_cont.w_form.w_tree;

	wind_get (win[index].w_hg, WF_FIRSTXYWH,
						&rd.g_x, &rd.g_y, &rd.g_w, &rd.g_h);
	objc_offset (adr, object, &r.g_x, &r.g_y);
	r.g_w = adr[object].ob_width;
	r.g_h = adr[object].ob_height;
	if (win[index].w_type == WTYP_FORM)
		zone_work (index, &z.g_x, &z.g_y, &z.g_w, &z.g_h);
	else if ((win[index].w_type & WTYP_MENU) || (win[index].w_type & WTYP_TOOL))
		wind_get (win[index].w_hg, WF_WORKXYWH, &z.g_x, &z.g_y, &z.g_w, &z.g_h);
	while((rd.g_w > ZERO) && (rd.g_h > ZERO))
	{
		if (rc_intersect (&r, &rd))
		{
			rc_intersect (&z, &rd);
			if ((rd.g_w > ZERO) && (rd.g_h > ZERO))
				objc_draw (adr, ROOT, MAX_DEPTH, rd.g_x, rd.g_y, rd.g_w, rd.g_h);
		}
		wind_get (win[index].w_hg, WF_NEXTXYWH,
						&rd.g_x, &rd.g_y, &rd.g_w, &rd.g_h);
	}
}
/* #] draw_object () Dessin objet dans fenˆtre formulaire :				*/ 
/* #[ print_line () Affiche une ligne :														*/
void print_line (int f, int index)
{
int xw, yw, ww, hw;

	zone_work (index, &xw, &yw, &ww, &hw);
	if (xw < bureau.xd)
	{
		ww -= (bureau.xd - xw);
		xw = bureau.xd;
	}
	yw = max (yw, bureau.yd);
	ww = min (ww, bureau.xd + bureau.wd - xw);
	hw = min (hw, bureau.yd + bureau.hd - yw);

	buf[4] = xw;
	if (f)
		buf[5] = yw + hw - win[index].w_hunit;
	else
		buf[5] = yw + win[index].w_vprot;
	buf[6] = ww;
	buf[7] = win[index].w_hunit;

	buf[3] = win[index].w_hg;
	redraw ();
}
/* #] print_line () Affiche une ligne :														*/ 
/* #[ print_colum () Affiche une colonne :												*/
void print_colum (int f, int index)
{
int xw, yw, ww, hw;

	zone_work (index, &xw, &yw, &ww, &hw);
	if (xw < bureau.xd)
	{
		ww -= (bureau.xd - xw);
		xw = bureau.xd;
	}
	yw = max (yw, bureau.yd);
	ww = min (ww, bureau.xd + bureau.wd - xw);
	hw = min (hw, bureau.yd + bureau.hd - yw);

	if (f)
		buf[4] = xw + ww - win[index].w_wunit;
	else
		buf[4] = xw + win[index].w_hprot;
	buf[5] = yw;
	buf[6] = win[index].w_wunit;
	buf[7] = hw;

	buf[3] = win[index].w_hg;
	redraw ();
}
/* #] print_colum () Affiche une colonne :												*/ 
/* #[ print_page () Affiche une page (zone de travail) :					*/
void print_page (int index)
{
int xw, yw, ww, hw;

	if (win[index].w_hg <= ZERO)
		return;
	zone_work (index, &xw, &yw, &ww, &hw);	/* Zone de travail fenˆtre */
	buf[3] = win[index].w_hg;		/* Remplir le buffer d'‚v‚nements */
	buf[4] = xw;
	buf[5] = yw;
	buf[6] = ww;
	buf[7] = hw;
	redraw ();									/* Appel redraw */
}
/* #] print_page () Affiche une page (zone de travail) :					*/ 
/* #[ print_wind () R‚affiche une fenˆtre entiŠre :								*/
void print_wind (int index)
{
	if (win[index].w_hg <= ZERO)
		return;
	buf[3] = win[index].w_hg;		/* Remplir le buffer d'‚v‚nements */
	buf[4] = win[index].w_curr.g_x;
	buf[5] = win[index].w_curr.g_y;
	buf[6] = win[index].w_curr.g_w;
	buf[7] = win[index].w_curr.g_h;
	redraw ();									/* Appel redraw */
}
/* #] print_wind () R‚affiche une fenˆtre entiŠre :								*/ 
/* #[ next_wind () Cycle vers fenˆtre suivante :									*/
void	next_wind (void)
{
int i, index;

	index = find_index (buf[3]);
	if (NOT (win[index].w_flags & WFLAST))
		i = index + 1;		/* Partir de la fenˆtre suivante */
	else
		i = ZERO;
	do
	{
		if (win[i].w_hg > ZERO)
		{				/* La 1ø fenˆtre trouv‚e est mise au 1ø plan */
			buf[3] = win[i].w_hg;
			(*win[i].w_top)();

			if ( ((win[i].w_type & 0xFF) == WTYP_PICT)
				|| ((win[index].w_type & 0xFF) == WTYP_FORM))
				set_palette (i);
			else
				set_palette (BLANK);

			wind_mouse ();
			i = index - 1;		/* Pour sortir de la boucle */
		}
		else if (win[i].w_flags & WFLAST)		/* Si on arrive au bout, */
			i = BLANK;												/* repartir du d‚but. */
	} while (++i != index);
}
/* #] next_wind () Cycle vers fenˆtre suivante :									*/ 

