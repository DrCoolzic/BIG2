/*						>>> Big Is Gem <<<
				MODULE FONCTIONS D'OBJETS USERDEF
*/
/****** En-tàte BIG ***********************************************/
#include <BIG2.H>
#include "BIG2USR2.H"
/* #[ Look par dÇfaut des fenàtres BIG :													*/
Win_user bwin = {MD_TRANS,		/* Mode texte MOVER */
								 BLACK,				/* Couleur texte MOVER */
								 2,						/* Style trame MOVER */
								 BLACK,				/* Couleur trame MOVER */
								 BLACK,				/* Couleur texte INFO */
								 1,						/* Style trame VELEV */
								 BLACK,				/* Couleur trame VELEV */
								 1,						/* Style trame HELEV */
								 BLACK,				/* Couleur trame HELEV */
								 1,						/* Style trame VSLIDE */
								 WHITE,				/* Couleur trame VSLIDE */
								 1,						/* Style trame HSLIDE */
								 WHITE,				/* Couleur trame HSLIDE */
								 WHITE,				/* Couleur fond gadgets */
								 BLACK};			/* Couleur icìne gadgets */
/* #] Look par dÇfaut des fenàtres BIG :													*/ 
/* #[ fond () Routine dessin racine fenàtre :											*/
int cdecl fond (PARMBLK *para)
{
int tab[4], rec[4];

	fill_tab (tab, 4,
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);				/* Clipper */
	vswr_mode (work_display.handle, MD_REPLACE);		/* Dessin en mode Remplacement */
	vsf_interior (work_display.handle, FIS_HOLLOW);	/* Fond blanc */
	vsf_color (work_display.handle, WHITE);
	vsf_perimeter (work_display.handle, FALSE);			/* Ne pas dessiner le pÇrimätre */
	fill_tab (rec, 4,
						para->pb_x, para->pb_y,
						para->pb_x + para->pb_w - 1,
						para->pb_y + para->pb_h - 1);
	v_bar (work_display.handle, rec);								/* Tracer le rectangle blanc */

	vs_clip (work_display.handle, FALSE, tab);			/* DÇclipper */
	return ZERO;
}
/* #] fond () Routine dessin racine fenàtre :											*/ 
/* #[ gadget () Routine dessin objets USERDEFs :									*/
int cdecl gadget (PARMBLK *para)
{
int tab[4], rec[8], color[2], obj, h, dummy;
MFDB nul = {0}, pic, msk;

	fill_tab (tab, 4,
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);				/* Clipper */

	vswr_mode (work_display.handle, MD_REPLACE);			/* Dessin en mode Remplacement */
	vsf_interior (work_display.handle, FIS_PATTERN);	/* Fond tramÇ */
	vsf_style (work_display.handle, 8);								/* Trame 100 % */
	vsf_color (work_display.handle, bwin.cpat_gadgt);	/* Couleur fond */
	vsf_perimeter (work_display.handle, FALSE);				/* Ne pas dessiner le pÇrimätre */
	fill_tab (rec, 4,
						para->pb_x, para->pb_y,
						para->pb_x + para->pb_w - 1,
						para->pb_y + para->pb_h - 1);
	v_bar (work_display.handle, rec);									/* Tracer le rectangle */

	vswr_mode (work_display.handle, MD_TRANS);
	vsf_interior (work_display.handle, FIS_HOLLOW);	/* Fond blanc */
	vsf_color (work_display.handle, BLACK);
	vsf_perimeter (work_display.handle, TRUE);			/* Dessiner le pÇrimätre */
	v_bar (work_display.handle, rec);								/* Tracer le rectangle */

	vswr_mode (work_display.handle, MD_REPLACE);			/* Dessin en mode Remplacement */
	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &dummy, &h);	/* Boåtes caractäres */
	switch (para->pb_obj)		/* Selon le gadget */
	{												/* Choisir la bonne G_IMAGE */
	case BW_SIZERUL :
		obj = h > 8 ? GA_SULH : GA_SULM;
		break;
	case BW_CLOSER :
		obj = h > 8 ? GA_CLOH : GA_CLOM;
		break;
	case BW_SMALLER :
		obj = h > 8 ? GA_SMAH : GA_SMAM;
		break;
	case BW_CYCLER :
		obj = h > 8 ? GA_CYCH : GA_CYCM;
		break;
	case BW_FULLER :
		obj = h > 8 ? GA_FULH : GA_FULM;
		break;
	case BW_SIZERUR :
		obj = h > 8 ? GA_SURH : GA_SURM;
		break;
	case BW_UPARROW :
		obj = h > 8 ? GA_AUPH : GA_AUPM;
		break;
	case BW_DNARROW :
		obj = h > 8 ? GA_ADNH : GA_ADNM;
		break;
	case BW_SIZERDL :
		obj = h > 8 ? GA_SDLH : GA_SDLM;
		break;
	case BW_LFARROW :
		obj = h > 8 ? GA_ALFH : GA_ALFM;
		break;
	case BW_RTARROW :
		obj = h > 8 ? GA_ARTH : GA_ARTM;
		break;
	case BW_SIZERDR :
		obj = h > 8 ? GA_SDRH : GA_SDRM;
		break;
	}
	pic.fd_addr = (void *) bi_trindex[GADGETS][obj].ob_spec.iconblk->ib_pdata;	/* Adresse datas */
	msk.fd_addr = (void *) bi_trindex[GADGETS][obj].ob_spec.iconblk->ib_pmask;	/* Adresse masque */
	pic.fd_w = msk.fd_w = 16;				/* Largeur image */
	if (h > 8)
		pic.fd_h = msk.fd_h = 16;			/* Hauteur image selon rÇsolution */
	else
		pic.fd_h = msk.fd_h = 8;
	pic.fd_wdwidth = msk.fd_wdwidth = 1;		/* Largeur en mots */
	pic.fd_nplanes = msk.fd_nplanes = 1;		/* Nbre de plans */
	pic.fd_stand = msk.fd_stand = ZERO;			/* Systäme coordonnÇes */

	fill_tab (rec, 8,			/* Tableau des coordonnÇes */
						ZERO, ZERO, pic.fd_w - 1, pic.fd_h - 1,
						para->pb_x + 2, para->pb_y + 2,
						para->pb_x + pic.fd_w + 1,
						para->pb_y + pic.fd_h + 1);
	if (para->pb_currstate & SELECTED)
	{
		rec[4]++;
		rec[5]++;
		rec[6]++;
		rec[7]++;
	}

	color[0] = WHITE;
	color[1] = WHITE;
	vrt_cpyfm (work_display.handle, MD_TRANS, rec, &msk, &nul, color);
	color[0] = bwin.cicn_gadgt;
	vrt_cpyfm (work_display.handle, MD_TRANS, rec, &pic, &nul, color);

	vsl_width (work_display.handle, 1);
	vsl_ends (work_display.handle, SQUARE, SQUARE);
	vsl_type (work_display.handle, SOLID);
	vsl_color (work_display.handle, BLACK);
	if (para->pb_currstate & SELECTED)
		fill_tab (rec, 6,										/* CoordonnÇes lignes d'ombre */
							para->pb_x + 1, para->pb_y + para->pb_h - 3,
							para->pb_x + 1, para->pb_y + 1,
							para->pb_x + para->pb_w - 3, para->pb_y + 1);
	else
		fill_tab (rec, 6,										/* CoordonnÇes lignes d'ombre */
							para->pb_x + para->pb_w - 2, para->pb_y + 2,
							para->pb_x + para->pb_w - 2, para->pb_y + para->pb_h - 2,
							para->pb_x + 2, para->pb_y + para->pb_h - 2);
	v_pline (work_display.handle, 3, rec);				/* Tracer lignes d'ombre */

	vs_clip (work_display.handle, FALSE, tab);		/* DÇclipper */
	return ZERO;
}
/* #] gadget () Routine dessin objets USERDEFs :									*/ 
/* #[ titinf () Routine dessin Titre et Info :										*/
int cdecl titinf (PARMBLK *para)
{
int tab[4], rec[6], x, y, wcell, hcell, dummy, i = ZERO, index = BLANK;
char *texte;

	fill_tab (tab, 4,
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);			/* Clipper */

	do
	{
		if (win[i].w_fen == para->pb_tree)
			index = i;
	} while ((NOT (win[i++].w_flags & WFLAST)) && (index == BLANK));

	vswr_mode (work_display.handle, MD_REPLACE);	/* Dessin en mode Remplacement */
	if (para->pb_obj == BW_INFO)					/* Si ligne d'info */
	{
		vsf_interior (work_display.handle, FIS_HOLLOW);		/* Fond blanc */
		vsf_color (work_display.handle, BLACK);						/* Couleur trame et pÇrimätre */
		vsf_perimeter (work_display.handle, TRUE);				/* Dessiner le pÇrimätre */
	}
	else if (para->pb_obj == BW_MOVER)							/* Si barre de titre */
	{
		if (para->pb_tree == win[id_top].w_fen)				/* Si fenàtre 1¯ plan */
		{
			vsf_interior (work_display.handle, FIS_PATTERN);	/* Trame */
			vsf_style (work_display.handle, bwin.spat_mover);	/* Motif de trame */
			vsf_color (work_display.handle, bwin.cpat_mover);	/* Couleur trame */
			vsf_perimeter (work_display.handle, TRUE);				/* Dessiner le pÇrimätre */
		}
		else																					/* Si pas fenàtre 1¯ plan */
		{
			vsf_interior (work_display.handle, FIS_HOLLOW);		/* Fond blanc */
			vsf_color (work_display.handle, BLACK);						/* Couleur trame et pÇrimätre */
			vsf_perimeter (work_display.handle, TRUE);				/* Dessiner le pÇrimätre */
		}
	}
	fill_tab (rec, 4,														/* CoordonnÇes rectangle */
						para->pb_x, para->pb_y,
						para->pb_x + para->pb_w - 1,
						para->pb_y + para->pb_h - 1);
	v_bar (work_display.handle, rec);									/* Tracer rectangle */

	if (para->pb_obj == BW_MOVER)
	{
		texte = (char *)malloc (strlen (win[index].w_title) + 1);
		strcpy (texte, win[index].w_title);	/* Copier le texte */
	}
	else
	{
		texte = (char *)malloc (strlen (win[index].w_infos) + 1);
		strcpy (texte, win[index].w_infos);	/* Copier le texte */
	}
	if (strlen (texte))
	{
		vst_effects (work_display.handle, EFNONE);	/* DÇsactiver tous les effets */
		if (para->pb_obj == BW_INFO)						/* Si ligne d'info */
		{
			vst_color (work_display.handle, bwin.ctxt_info);	/* Couleur texte */
			vswr_mode (work_display.handle, MD_REPLACE);
			vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);	/* Caler Ö gauche */
			x = para->pb_x + 8;									/* Sauter un caractäre */
			y = para->pb_y + 1;									/* Position en y */
		}
		else																	/* Si barre de titre */
		{
			vst_color (work_display.handle, bwin.ctxt_mover);	/* Couleur texte */
			vswr_mode (work_display.handle, bwin.mtxt_mover);	/* Mode Çcriture */
			vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &wcell, &hcell);
			if (win[index].w_icon > BLANK)
			{
				if (hcell > 8)
					vst_height (work_display.handle, HPETIT, &dummy, &dummy, &wcell, &hcell);		/* Taille caractäres */
				else
					vst_height (work_display.handle, HMINUS, &dummy, &dummy, &wcell, &hcell);		/* Taille caractäres */
			}
			if (strlen ((char *)texte) * wcell <= para->pb_w)	/* S'il y a la place */
			{
				vst_alignment (work_display.handle, HCENT, VTOP, &dummy, &dummy);	/* Centrer le texte */
				x = para->pb_x + (para->pb_w / 2);
			}
			else																	/* Si pas la place */
			{
				texte[para->pb_w / wcell] = '\0';		/* Couper le texte */
				vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);	/* Caler Ö gauche */
				x = para->pb_x;
			}
			y = para->pb_y + ((para->pb_h - hcell) / 2);
			if (para->pb_currstate & SELECTED)
			{
				x++;
				y++;
			}
		}
		v_gtext (work_display.handle, x, y, texte);	/* Ecrire le texte */
		free (texte);										/* LibÇrer mÇmoire texte */
	}

	if (para->pb_obj == BW_MOVER)		/* Si barre de titre */
	{
		vswr_mode (work_display.handle, MD_REPLACE);
		vsl_width (work_display.handle, 1);		/* PrÇparer lignes d'ombre */
		vsl_ends (work_display.handle, SQUARE, SQUARE);
		vsl_type (work_display.handle, SOLID);
		if (para->pb_tree == win[id_top].w_fen)				/* Si fenàtre 1¯ plan */
			vsl_color (work_display.handle, bwin.cpat_mover);
		else
			vsl_color (work_display.handle, BLACK);
		if (para->pb_currstate & SELECTED)
			fill_tab (rec, 6,										/* CoordonnÇes lignes d'ombre */
								para->pb_x + 1, para->pb_y + para->pb_h - 3,
								para->pb_x + 1, para->pb_y + 1,
								para->pb_x + para->pb_w - 3, para->pb_y + 1);
		else
			fill_tab (rec, 6,										/* CoordonnÇes lignes d'ombre */
								para->pb_x + para->pb_w - 2, para->pb_y + 2,
								para->pb_x + para->pb_w - 2, para->pb_y + para->pb_h - 2,
								para->pb_x + 2, para->pb_y + para->pb_h - 2);
		v_pline (work_display.handle, 3, rec);		/* Lignes d'ombre */
	}

	vs_clip (work_display.handle, FALSE, tab);	/* DÇclipper */
	return ZERO;
}
/* #] titinf () Routine dessin Titre et Info :										*/ 
/* #[ elev () Routine dessin ascenseurs :													*/
int cdecl elev (PARMBLK *para)
{
int tab[4], rec[4];

	fill_tab (tab, 4,
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);				/* Clipper */

	vswr_mode (work_display.handle, MD_REPLACE);		/* Dessin en mode Remplacement */
	vsf_interior (work_display.handle, FIS_PATTERN);		/* Fond tramÇ */
	if (para->pb_obj == BW_VELEV)
	{
		vsf_style (work_display.handle, bwin.spat_velev);	/* Motif trame */
		vsf_color (work_display.handle, bwin.cpat_velev);	/* Couleur trame */
	}
	else if (para->pb_obj == BW_HELEV)
	{
		vsf_style (work_display.handle, bwin.spat_helev);	/* Motif trame */
		vsf_color (work_display.handle, bwin.cpat_helev);	/* Couleur trame */
	}
	vsf_perimeter (work_display.handle, TRUE);			/* Dessiner le pÇrimätre */
	fill_tab (rec, 4,										/* CoordonnÇes rectangle */
						para->pb_x, para->pb_y,
						para->pb_x + para->pb_w - 1,
						para->pb_y + para->pb_h - 1);
	v_bar (work_display.handle, rec);								/* Tracer rectangle */

	vswr_mode (work_display.handle, MD_TRANS);
	vsf_interior (work_display.handle, FIS_HOLLOW);	/* Fond blanc */
	vsf_color (work_display.handle, BLACK);
	v_bar (work_display.handle, rec);								/* Tracer le rectangle */

	vs_clip (work_display.handle, FALSE, tab);		/* DÇclipper */
	return ZERO;
}
/* #] elev () Routine dessin ascenseurs :													*/ 
/* #[ sld () Routine dessin sliders :															*/
int cdecl sld (PARMBLK *para)
{
int tab[4], rec[8], color[2], h, dummy;
MFDB nul = {0}, pic, msk;

	fill_tab (tab, 4,
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);				/* Clipper */

	vswr_mode (work_display.handle, MD_REPLACE);		/* Dessin en mode Remplacement */
	if (para->pb_obj == BW_VSLIDE)
	{
		if (bwin.cpat_vslid == WHITE)
		{
			vsf_interior (work_display.handle, FIS_HOLLOW);		/* Fond blanc */
			vsf_color (work_display.handle, BLACK);						/* Couleur pÇrimätre */
		}
		else
		{
			vsf_interior (work_display.handle, FIS_PATTERN);	/* Fond tramÇ */
			vsf_style (work_display.handle, bwin.spat_vslid);	/* Motif trame */
			vsf_color (work_display.handle, bwin.cpat_vslid);	/* Couleur trame */
		}
	}
	else if (para->pb_obj == BW_HSLIDE)
	{
		if (bwin.cpat_hslid == WHITE)
		{
			vsf_interior (work_display.handle, FIS_HOLLOW);		/* Fond blanc */
			vsf_color (work_display.handle, BLACK);						/* Couleur pÇrimätre */
		}
		else
		{
			vsf_interior (work_display.handle, FIS_PATTERN);	/* Fond tramÇ */
			vsf_style (work_display.handle, bwin.spat_hslid);	/* Motif trame */
			vsf_color (work_display.handle, bwin.cpat_hslid);	/* Couleur trame */
		}
	}
	vsf_perimeter (work_display.handle, TRUE);			/* Dessiner le pÇrimätre */
	fill_tab (rec, 4,										/* CoordonnÇes rectangle */
						para->pb_x, para->pb_y,
						para->pb_x + para->pb_w - 1,
						para->pb_y + para->pb_h - 1);
	v_bar (work_display.handle, rec);								/* Tracer rectangle */

	vswr_mode (work_display.handle, MD_TRANS);
	vsf_interior (work_display.handle, FIS_HOLLOW);	/* Fond blanc */
	vsf_color (work_display.handle, BLACK);
	vsf_perimeter (work_display.handle, TRUE);			/* Dessiner le pÇrimätre */
	v_bar (work_display.handle, rec);								/* Tracer le rectangle */

	vsl_width (work_display.handle, 1);
	vsl_ends (work_display.handle, SQUARE, SQUARE);
	vsl_type (work_display.handle, SOLID);
	if (para->pb_currstate & SELECTED)
		fill_tab (rec, 6,										/* CoordonnÇes lignes d'ombre */
							para->pb_x + 1, para->pb_y + para->pb_h - 3,
							para->pb_x + 1, para->pb_y + 1,
							para->pb_x + para->pb_w - 3, para->pb_y + 1);
	else
		fill_tab (rec, 6,										/* CoordonnÇes lignes d'ombre */
							para->pb_x + para->pb_w - 2, para->pb_y + 2,
							para->pb_x + para->pb_w - 2, para->pb_y + para->pb_h - 2,
							para->pb_x + 2, para->pb_y + para->pb_h - 2);
	v_pline (work_display.handle, 3, rec);							/* Tracer lignes d'ombre */

	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &dummy, &h);	/* Boåtes caractäres */
	if (para->pb_obj == BW_VSLIDE)				/* Adresse datas */
	{
		if (h > 8)
		{
			pic.fd_addr = (void *) bi_trindex[GADGETS][GA_SLVH].ob_spec.iconblk->ib_pdata;
			msk.fd_addr = (void *) bi_trindex[GADGETS][GA_SLVH].ob_spec.iconblk->ib_pmask;
		}
		else
		{
			pic.fd_addr = (void *) bi_trindex[GADGETS][GA_SLVM].ob_spec.iconblk->ib_pdata;
			msk.fd_addr = (void *) bi_trindex[GADGETS][GA_SLVM].ob_spec.iconblk->ib_pmask;
		}
	}
	else
	{
		if (h > 8)
		{
			pic.fd_addr = (void *) bi_trindex[GADGETS][GA_SLHH].ob_spec.iconblk->ib_pdata;
			msk.fd_addr = (void *) bi_trindex[GADGETS][GA_SLHH].ob_spec.iconblk->ib_pmask;
		}
		else
		{
			pic.fd_addr = (void *) bi_trindex[GADGETS][GA_SLHM].ob_spec.iconblk->ib_pdata;
			msk.fd_addr = (void *) bi_trindex[GADGETS][GA_SLHM].ob_spec.iconblk->ib_pmask;
		}
	}
	pic.fd_w = msk.fd_w = 16;				/* Largeur image */
	if (h > 8)
		pic.fd_h = msk.fd_h = 16;			/* Hauteur image selon rÇsolution */
	else
		pic.fd_h = msk.fd_h = 8;
	pic.fd_wdwidth = msk.fd_wdwidth = 1;		/* Largeur en motes */
	pic.fd_nplanes = msk.fd_nplanes = 1;		/* Nbre de plans */
	pic.fd_stand = msk.fd_stand = ZERO;			/* Systäme coordonnÇes */

	fill_tab (rec, 8,			/* Tableau des coordonnÇes */
						ZERO, ZERO, pic.fd_w - 1, pic.fd_h - 1,
						para->pb_x + (para->pb_w / 2) - (pic.fd_w / 2),
						para->pb_y + (para->pb_h / 2) - (pic.fd_h / 2),
						para->pb_x + (para->pb_w / 2) - (pic.fd_w / 2) + pic.fd_w - 1,
						para->pb_y + (para->pb_h / 2) - (pic.fd_h / 2) + pic.fd_h - 1);
	if (para->pb_currstate & SELECTED)
	{
		rec[4]++;
		rec[5]++;
		rec[6]++;
		rec[7]++;
	}
	color[0] = WHITE;
	color[1] = WHITE;
	vrt_cpyfm (work_display.handle, MD_TRANS, rec, &msk, &nul, color);
	color[0] = bwin.cicn_gadgt;
	vrt_cpyfm (work_display.handle, MD_TRANS, rec, &pic, &nul, color);

	vs_clip (work_display.handle, FALSE, tab);		/* DÇclipper */
	return ZERO;
}
/* #] sld () Routine dessin sliders :															*/ 
/* #[ work () Routine dessin zone de travail :										*/
int cdecl work (PARMBLK *para)
{
int tab[4], rec[4];

	fill_tab (tab, 4,
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);						/* Clipper */

	vswr_mode (work_display.handle, MD_REPLACE);				/* Dessin en mode Remplacement */
	if (NOT (para->pb_tree[para->pb_obj].ob_state & 0x0F00))
	{
		vsf_interior (work_display.handle, FIS_HOLLOW);		/* Fond couleur 0 */
		vsf_color (work_display.handle, WHITE);						/* Couleur trame */
	}
	else
	{
		vsf_interior (work_display.handle, FIS_PATTERN);	/* Trame */
		vsf_style (work_display.handle,		/* Motif de trame */
							 ((para->pb_tree[para->pb_obj].ob_state & 0xF000) >> 12));
		vsf_color (work_display.handle,		/* Couleur trame */
							 ((para->pb_tree[para->pb_obj].ob_state & 0x0F00) >> 8));
	}
	vsf_perimeter (work_display.handle, FALSE);			/* Ne pas dessiner le pÇrimätre */
	fill_tab (rec, 4,											/* CoordonnÇes rectangle */
						para->pb_x + 1, para->pb_y + 1,
						para->pb_x + para->pb_w - 2,
						para->pb_y + para->pb_h - 2);
	v_bar (work_display.handle, rec);									/* Tracer rectangle */

	vs_clip (work_display.handle, FALSE, tab);				/* DÇclipper */
	return ZERO;
}
/* #] work () Routine dessin zone de travail :										*/ 

