/*								BIG2DEMO.C			>>> Big Is Gem <<<
		Programme de d‚mo de BIG2 : Librairie de d‚veloppement sous GEM
								cr‚‚e et d‚velopp‚e par Claude ATTARD
*/

/* #[ D‚finitions :																									*/
#include <BIG2.H>
#include <big_imgs.h>

int fgdos, fid, size;	/* Pour le travail avec Speedo */
t_image_dial bimg;		/* D‚finition image du bureau */
char *zone = ZERO;		/* Zone de chargement image du bureau */

#define INTGR 0
#define BIMAGE "BIG2DEMO.IMG"

#if INTGR						/* Si on travaille avec un ressource int‚gr‚ */
#include "BIG2DEMO.RSH"
#define TRINDEX (OBJECT *)rs_trindex
#define FRSTR (char *)rs_frstr
#define NUM_TREE 22
#else								/* Si on travaille avec un ressource externe */
#include "BIG2DEMO.H"
#define TRINDEX 0
#define FRSTR 0
#define NUM_TREE 0
#endif

#define NB_FENETRES 7
#define WINDACC 0
#define PETIT_FORM 1
#define GRAND_FORM 2
#define TEXTE 3
#define IMAGE 4
#define WINDMENU 5
#define HISTO 6

t_image_dial bimg;		/* Pour l'image de bureau */
/* #] D‚finitions :																									*/ 

/* #[ Prototypage :																									*/
void main (void);
void set_bimg (void);
void set_form (void);
void ptr_fonctions (void);
void load_fontspeedo (void);
void event_mesag (int *flacc, int *quit);
void event_pmenu (int *quit);
void event_freepu (void);
void event_wform (void);
void event_wmenu (int *flacc);
void event_whier (void);
void event_tool (void);
void event_desk (void);
void aff_infos (void);
void formulaire (void);
void test_alerte (void);
void pform_wind (void);
void trame_desk (void);
void gform_wind (void);
void objets_desk (void);
void charge_texte (void);
void aff_texte (void);
void set_font (void);
void close_txt (void);
void charge_image (void);
void close_img (void);
void cree_histo (void);
void aff_histo (void);
void close_histo (void);
void close_wmenu (void);
void icone (void);
void winicn (void);
void clos_acc (void);
void acc_loop (void);
void red_acc (void);
void relief (void);
/* #] Prototypage :																									*/ 
/* #[ main () Initialisations et gestion :													*/
void main (void)
{
int evnt, quit = FALSE, flacc = FALSE;
MFDB img;

			/* Initialiser */
	if (initial ("BIG2DEMO.RSC", MENU, BUREAU, NUM_TREE,
							 TRINDEX, FRSTR, NB_FENETRES, TITREACC))
	{
		if (_app)
			set_bimg ();	/* Placer l'image sur le bureau */
		ptr_fonctions ();
		set_form ();
		load_fontspeedo ();

		if (work_display.n_color >= 16)	/* Si on est en couleurs */
			relief ();
		aide_en_ligne = K_CTRL;	/* Bulles d'aide avec touche Control */

		do
		{
			if ((_app == ZERO) && (NOT flacc))	/* Si on tourne en accessoire */
			{
				acc_loop ();
				flacc = TRUE;
			}

			evnt = dialog (DLG_DESK, adr_desk, TRUE, TRUE, PU_LIBRE, &img, FALSE);
			if (evnt & MU_MESAG)
				event_mesag (&flacc, &quit);
			if ((evnt & MU_BUTTON) && (buf[0] == BEV_FREEPU))
				event_freepu ();
			if ((evnt & MU_BUTTON) && (buf[0] == BEV_WFORM))
				event_wform ();
			if ((evnt & MU_BUTTON) && (buf[0] == BEV_WMENU))
				event_wmenu (&flacc);
			if ((evnt & MU_BUTTON) && (buf[0] == BEV_WHIER))
				event_whier ();
			if ((evnt & MU_BUTTON) && (buf[0] == BEV_TOOL))
				event_tool ();
			if ((evnt & MU_BUTTON) && (NOT wind_find (mx, my)))
				event_desk ();
		} while (NOT quit);
		if (fgdos)
			vst_unload_fonts (work_display.handle, 0);
		end ();
	}
}
/* #] main () Initialisations et gestion :													*/ 
/* #[ set_bimg () Placer l'image du bureau :												*/
void set_bimg (void)
{
	if (img_attach (&bimg, BIMAGE, BUREAU, BU_IMG, TRUE, TRUE))	/* Image de bureau */
	{
		adr_desk[BU_IMG].ob_flags &= ~HIDETREE;		/* Montrer l'objet et le dessiner */
		form_dial (FMD_FINISH, ZERO, ZERO, ZERO, ZERO,
							 bureau.xd, bureau.yd, bureau.wd, bureau.hd);
	}
}
/* #] set_bimg () Placer l'image du bureau :												*/ 
/* #[ set_form () Met en place les formulaires associ‚s :						*/
void set_form (void)
{
	form_attach (MENU, M_WWIND, MHIERA);
	form_attach (FORMULAIRE, FO_PUT, POP_UP2);
	form_attach (FORMULAIRE, FO_PUI, POPUP_ICN);
	form_attach (FORMULAIRE, P_AIDE, AIDE1);
	form_attach (INFOS, INF_INFOS, CONTACT);
	form_attach (INFOS, INF_MERCI, MERCI);
	form_attach (PETITFORM, PF_TRAME, TRAMES);
	form_attach (PETITFORM, PF_COLOR, COULEURS);
	form_attach (PETITFORM, PF_AIDE, AIDE2);
	form_attach (GRANDFORM, GF_PU1, POP_UP1);
	form_attach (GRANDFORM, GF_PL, POP_UP1);
	form_attach (GRANDFORM, GF_PU2, POP_UP2);
	form_attach (GRANDFORM, GF_ICN, POPUP_ICN);
	form_attach (GRANDFORM, GF_AIDE, AIDE2);
	form_attach (W_MENU, MW_AL, HIERWIND);
	form_attach (TOOL, TL_FONT, FONTS);
	form_attach (TOOL, TL_SIZE, SIZES);
	bulle_attach (FORMULAIRE, FO_PUT, BA_PU);
	bulle_attach (GRANDFORM, GF_PU2, BA_PU);
}
/* #] set_form () Met en place les formulaires associ‚s :						*/ 
/* #[ ptr_fonctions () Affecter fonctions aux pointeurs :						*/
void ptr_fonctions (void)
{
	fnc[0] = (void *) aff_infos;			/* Infos du menu principal */
	fnc[1] = (void *) formulaire;			/* Formulaire du menu principal */
	fnc[2] = (void *) pform_wind;			/* Petit form. menu principal */
	fnc[3] = (void *) gform_wind;			/* Grand form. menu principal */
	fnc[4] = (void *) test_alerte;		/* Alerte formulaire libre */
	fnc[5] = (void *) aff_infos;			/* Infos menu en fenˆtre */
	fnc[6] = (void *) charge_texte;		/* Charger texte menu hierarchique */
	fnc[7] = (void *) charge_image;		/* Charger image menu hierarchique */
	fnc[8] = (void *) formulaire;			/* Formulaire libre pop-up libre */
	fnc[9] = (void *) pform_wind;			/* Petit form. pop-up libre */
	fnc[10] = (void *) gform_wind;		/* Grand form. pop-up libre */
	fnc[11] = (void *) charge_texte;	/* Charger texte pop-up libre */
	fnc[12] = (void *) charge_image;	/* Charger image pop-up libre */
	fnc[13] = (void *) cree_histo;		/* Ouvrir histo pop-up libre */
	fnc[14] = (void *) icone;					/* Alerte avec choix ic“ne */
	fnc[15] = (void *) icone;					/* Alerte avec choix ic“ne */
	fnc[16] = (void *) icone;					/* Alerte avec choix ic“ne */
	fnc[17] = (void *) icone;					/* Alerte avec choix ic“ne */
	fnc[18] = (void *) icone;					/* Alerte avec choix ic“ne */
	fnc[19] = (void *) icone;					/* Alerte avec choix ic“ne */
	fnc[20] = (void *) icone;					/* Alerte avec choix ic“ne */
	fnc[21] = (void *) icone;					/* Alerte avec choix ic“ne */
}
/* #] ptr_fonctions () Affecter fonctions aux pointeurs :						*/ 
/* #[ load_fontspeedo () : Chargement fontes Speedo :								*/
void load_fontspeedo (void)
{
int nb_fonts, i;
char fontname[128], temp[35];

	if (vq_vgdos () == GDOS_FSM)
	{
		graf_mouse (BUSYBEE , 0);
		nb_fonts = vst_load_fonts (work_display.handle, 0);
		fgdos = TRUE;
		i = 2;
		do
		{
			vqt_name (work_display.handle, i, temp);
			strcpy (fontname, "  ");
			strcat (fontname, temp);
			if (strlen (fontname) > 33)
				fontname[33] = '\0';
			set_text (ir_trindex[FONTS], i, fontname);
			ir_trindex[FONTS][i].ob_state &= ~DISABLED;
			i++;
		} while (((i - 1) <= nb_fonts) && (i <= 41));
		graf_mouse (ARROW, 0);
		fid = 1;
	}
	size = 13;
}
/* #] load_fontspeedo () : Chargement fontes Speedo :								*/ 
/* #[ event_mesag () Ev‚nements de messages :												*/
void event_mesag (int *flacc, int *quit)
{
	if (buf[0] == MN_SELECTED)
		event_pmenu (quit);
	else if (buf[0] == BEV_HIERM)	/* Ev‚nement menu hi‚rarchique */
	{
		if (buf [6] == MH_HISTO)
		{
			cree_histo ();
			menu_tnormal (adr_menu, T_FENETRES, TRUE);
		}
	}
	else if (buf[0] == AC_CLOSE)
	{
		clos_acc ();
		*flacc = FALSE;
	}
	else if ((buf[0] == WM_CLOSED) && (buf[3] == win[WINDACC].w_hg))
		*flacc = FALSE;
}
/* #] event_mesag () Ev‚nements de messages :												*/ 
/* #[ event_pmenu () Ev‚nements menu principal :										*/
void event_pmenu (int *quit)
{
int i;

	switch (buf[4])
	{
	case M_ALERTE :
		big_alert (2, ALERTE, "", TRUE);
		menu_tnormal (adr_menu, T_FICHIER, 1);
		break;
	case M_WMENU :
		win[WINDMENU].w_pal = (Palette *)malloc (work_display.n_color *sizeof (Palette));
		for (i = 0 ; i < work_display.n_color ; i++)
		{
			win[WINDMENU].w_pal[i][0] = work_display.palette[i][0];
			win[WINDMENU].w_pal[i][1] = work_display.palette[i][1];
			win[WINDMENU].w_pal[i][2] = work_display.palette[i][2];
		}
		open_window (WINDMENU, WTYP_PICT, WATR_CURRB,
								 bureau.xd + (bureau.wd / 5), bureau.yd + (bureau.hd / 5), 320, 150,
								 100, 100, 200, 200, 16, 16,
								 WFARROW, THIN_CROSS,
								 winicn, ZERO, close_wmenu, get_string (TIT_MENU), "",
								 (long) bureau.wd, (long) bureau.hd, TRUE, FALSE, W_MENU,
								 10, BLACK);
		menu_tnormal (adr_menu, T_FENETRES, 1);
		break;
	case M_QUITTER :
		*quit = TRUE;
		menu_tnormal (adr_menu, T_FICHIER, 1);
		break;
	}
}
/* #] event_pmenu () Ev‚nements menu principal :										*/ 
/* #[ event_freepu () Ev‚nements de pop-up libre :									*/
void event_freepu (void)
{
int i;

	switch (buf[4])
	{
		case PUL_ALERT :
			if (_app)
				menu_tnormal (adr_menu, T_FICHIER, 0);
			else
			{
				win[WINDACC].w_bar[T_FICHIER + 3].ob_state |= SELECTED;
				draw_object (T_FICHIER + 3, WINDACC);
			}
			big_alert (2, ALERTE, "", TRUE);

			if (_app)
				menu_tnormal (adr_menu, T_FICHIER, 1);
			else
			{
				win[WINDACC].w_bar[T_FICHIER + 3].ob_state &= ~SELECTED;
				draw_object (T_FICHIER + 3, WINDACC);
			}
			break;
		case PUL_WMENU :
			if (_app)
				menu_tnormal (adr_menu, T_FENETRES, 0);
			else
			{
				win[WINDACC].w_bar[T_FENETRES + 3].ob_state |= SELECTED;
				draw_object (T_FENETRES + 3, WINDACC);
			}
			if (win[WINDMENU].w_hg <= ZERO)
			{
				win[WINDMENU].w_pal = (Palette *)malloc (work_display.n_color * sizeof (Palette));
				for (i = 0 ; i < work_display.n_color ; i++)
				{
					win[WINDMENU].w_pal[i][0] = work_display.palette[i][0];
					win[WINDMENU].w_pal[i][1] = work_display.palette[i][1];
					win[WINDMENU].w_pal[i][2] = work_display.palette[i][2];
				}
				open_window (WINDMENU, WTYP_PICT, WATR_CURRB,
										 bureau.xd + (bureau.wd / 5), bureau.yd + (bureau.hd / 5), 320, 150,
										 100, 100, 200, 200, 16, 16,
										 WFARROW, THIN_CROSS,
										 winicn, ZERO, close_wmenu, get_string (TIT_MENU), "",
										 (long) bureau.wd, (long) bureau.hd,
										 TRUE, FALSE, W_MENU,
										 10, BLACK);
			}
			else
			{
				buf[3] = win[WINDMENU].w_hg;
				(*win[WINDMENU].w_top)();
			}
			if (_app)
				menu_tnormal (adr_menu, T_FENETRES, 1);
			else
			{
				win[WINDACC].w_bar[T_FENETRES + 3].ob_state &= ~SELECTED;
				draw_object (T_FENETRES + 3, WINDACC);
			}
			break;
	}
}
/* #] event_freepu () Ev‚nements de pop-up libre :									*/ 
/* #[ event_wform () Ev‚nements formulaires en fenˆtre :						*/
void event_wform (void)
{
	if (buf[3] == PETIT_FORM)
	{
		switch (buf[4])
		{
		case PF_CONFIRME :
		case PF_APPLIQUE :
			win[buf[3]].w_cont.w_form.w_tree[buf[4]].ob_state &= ~SELECTED;
			free (&win[PETIT_FORM].w_cont.w_form.w_bak);	/* Lib‚rer m‚moire */
			bak_rsc (win[PETIT_FORM].w_cont.w_form.w_tree, &win[PETIT_FORM].w_cont.w_form.w_bak);
			if (_app)
				trame_desk ();
			break;
		case PF_ANNULE :
		case PF_RAZ :
			win[buf[3]].w_cont.w_form.w_tree[buf[4]].ob_state &= ~SELECTED;
			if (buf[4] == PF_RAZ)
				objc_xedit (win[PETIT_FORM].w_cont.w_form.w_tree,	/* Virer le curseur */
										ZERO, win[PETIT_FORM].w_cont.w_form.w_edit,
										&win[PETIT_FORM].w_cont.w_form.w_pos,
										ED_END, TRUE, PETIT_FORM);
			res_rsc (win[PETIT_FORM].w_cont.w_form.w_tree, &win[PETIT_FORM].w_cont.w_form.w_bak);
			if (buf[4] == PF_RAZ)
				objc_xedit (win[PETIT_FORM].w_cont.w_form.w_tree,	/* Remettre le curseur */
										ZERO, win[PETIT_FORM].w_cont.w_form.w_edit,
										&win[PETIT_FORM].w_cont.w_form.w_pos,
										ED_INIT, TRUE, PETIT_FORM);
			break;
		}
		if (buf[4] == PF_RAZ)
			print_page (PETIT_FORM);
	}
	else if (buf[3] == GRAND_FORM)
	{
		switch (buf[4])
		{
		case GF_CONFIRME :
		case GF_APPLIQUE :
			win[buf[3]].w_cont.w_form.w_tree[buf[4]].ob_state &= ~SELECTED;
			free (&win[GRAND_FORM].w_cont.w_form.w_bak);	/* Lib‚rer m‚moire */
			bak_rsc (win[GRAND_FORM].w_cont.w_form.w_tree, &win[GRAND_FORM].w_cont.w_form.w_bak);
			if (_app)
				objets_desk ();
			break;
		case GF_ANNULE :
		case GF_RAZ :
			win[buf[3]].w_cont.w_form.w_tree[buf[4]].ob_state &= ~SELECTED;
			res_rsc (win[GRAND_FORM].w_cont.w_form.w_tree, &win[GRAND_FORM].w_cont.w_form.w_bak);
			break;
		}
		if ((buf[4] == GF_RAZ) || (buf[4] == GF_APPLIQUE))
			print_page (GRAND_FORM);
	}
}
/* #] event_wform () Ev‚nements formulaires en fenˆtre :						*/ 
/* #[ event_wmenu () Ev‚nements menu en fenˆtre :										*/
void event_wmenu (int *flacc)
{
int i;

	if (buf[5] == WINDMENU)
	{
		switch (buf[4] - 3)		/* 3 objets ont ‚t‚ ajout‚s au menu */
		{
		case MW_IMAGE :
			charge_image ();
			break;
		case MW_ALERTE :
			big_alert (2, ALERTE, "", TRUE);
			if (win[buf[5]].w_bar[buf[4]].ob_state & CHECKED)
				menu_icheck (win[buf[5]].w_bar, buf[4], FALSE);
			else
				menu_icheck (win[buf[5]].w_bar, buf[4], TRUE);
			win[buf[5]].w_bar[buf[3]].ob_state &= ~SELECTED;
			draw_object (buf[3], buf[5]);
			break;
		}
	}
	else if (buf[5] == WINDACC)
	{		/* Ev‚nements du menu principal, si on est en accessoire : */
		switch (buf[4] - 3)		/* 3 objets ont ‚t‚ ajout‚s au menu */
		{
		case M_ALERTE :
			big_alert (2, ALERTE, "", TRUE);
			win[buf[5]].w_bar[buf[3]].ob_state &= ~SELECTED;
			draw_object (buf[3], buf[5]);
			break;
		case M_WMENU :
			win[buf[5]].w_bar[buf[3]].ob_state &= ~SELECTED;
			draw_object (buf[3], buf[5]);
			if (win[WINDMENU].w_hg <= ZERO)
			{
				win[WINDMENU].w_pal = (Palette *)malloc (work_display.n_color *sizeof (Palette));
				for (i = 0 ; i < work_display.n_color ; i++)
				{
					win[WINDMENU].w_pal[i][0] = work_display.palette[i][0];
					win[WINDMENU].w_pal[i][1] = work_display.palette[i][1];
					win[WINDMENU].w_pal[i][2] = work_display.palette[i][2];
				}
				open_window (WINDMENU, WTYP_PICT, WATR_CURRB,
										 bureau.xd + (bureau.wd / 5), bureau.yd + (bureau.hd / 5), 320, 150,
										 100, 100, 200, 200, 16, 16,
										 WFARROW, THIN_CROSS,
										 winicn, ZERO, close_wmenu, get_string (TIT_MENU), "",
										 (long) bureau.wd, (long) bureau.hd,
										 TRUE, FALSE, W_MENU, 10, BLACK);
			}
			else
			{
				buf[3] = win[WINDMENU].w_hg;
				(*win[WINDMENU].w_top)();
			}
			break;
		case M_QUITTER :
			clos_acc ();
			win[buf[5]].w_bar[buf[3]].ob_state &= ~SELECTED;
			*flacc = FALSE;
			break;
		}
	}
}
/* #] event_wmenu () Ev‚nements menu en fenˆtre :										*/ 
/* #[ event_whier () Ev‚nements menu hi‚rarchique de fenˆtre :			*/
void event_whier (void)
{
	if (buf[7] == WINDMENU)
	{
		switch (buf[6])
		{
		case MWH_ICN0 :
			buf[6] = MWH_ICN0;
			icone ();
			break;
		case MWH_ICN1 :
			buf[6] = MWH_ICN1;
			icone ();
			break;
		}
	}
	else if (buf[7] == WINDACC)
	{
		if (buf[6] == MH_HISTO)
		{
			cree_histo ();
			menu_tnormal (win[WINDACC].w_bar, T_FENETRES + 3, TRUE);
		}
	}
}
/* #] event_whier () Ev‚nements menu hi‚rarchique de fenˆtre :			*/ 
/* #[ event_tool () Ev‚nements de ToolBar :													*/
void event_tool (void)
{
	if (buf[3] == TEXTE)
	{
		if ((buf[4] == TL_FONT) || (buf[4] == TL_SIZE))
			set_font ();
		if (buf[4] == TL_INV)
		{
			if (win[TEXTE].w_bar[TL_INV].ob_state & SELECTED)
				set_work (TEXTE, 8, BLACK);
			else
				set_work (TEXTE, ZERO, WHITE);
		}
		if (buf[4] != TL_INV)
			print_page (TEXTE);
	}
}
/* #] event_tool () Ev‚nements de ToolBar :													*/ 
/* #[ event_desk () Ev‚nements sur le bureau :											*/
void event_desk (void)
{
int old_x, old_y, new_x, new_y, dummy;

	evnt_timer (70, 0);		/* D‚lai */
	graf_mkstate (&dummy, &dummy, &mk, &dummy);

	if ((object == BU_ICN) && (mk == 1))
	{													/* Ic“ne encore cliqu‚e -> d‚placement */
		old_x = adr_desk[object].ob_x;
		old_y = adr_desk[object].ob_y;
		graf_dragbox (adr_desk[object].ob_width, adr_desk[object].ob_height,
									adr_desk[object].ob_x + bureau.xd, adr_desk[object].ob_y + bureau.yd,
									bureau.xd, bureau.yd, bureau.wd, bureau.hd, &new_x, &new_y);
		adr_desk[object].ob_x = new_x - bureau.xd;
		adr_desk[object].ob_y = new_y - bureau.yd;
		form_dial (FMD_FINISH, 0, 0, 0, 0, old_x + bureau.xd, old_y + bureau.yd,
							 adr_desk[object].ob_width, adr_desk[object].ob_height);
		form_dial (FMD_FINISH, 0, 0, 0, 0,
							 adr_desk[object].ob_x + bureau.xd, adr_desk[object].ob_y + bureau.yd,
							 adr_desk[object].ob_width, adr_desk[object].ob_height);
	}
	else if (((object == BU_BOXTXT) || (object == BU_TXT1) || (object == BU_TXT2)) && (mk == 1))
	{		/* textes encore cliqu‚s -> d‚placement */
		old_x = adr_desk[BU_BOXTXT].ob_x;
		old_y = adr_desk[BU_BOXTXT].ob_y;
		graf_dragbox (adr_desk[BU_BOXTXT].ob_width, adr_desk[BU_BOXTXT].ob_height,
									adr_desk[BU_BOXTXT].ob_x + bureau.xd, adr_desk[BU_BOXTXT].ob_y + bureau.yd,
									bureau.xd, bureau.yd, bureau.wd, bureau.hd, &new_x, &new_y);
		adr_desk[BU_BOXTXT].ob_x = new_x - bureau.xd;
		adr_desk[BU_BOXTXT].ob_y = new_y - bureau.yd;
		form_dial (FMD_FINISH, 0, 0, 0, 0, old_x + bureau.xd - 3, old_y + bureau.yd - 3,
							 adr_desk[BU_BOXTXT].ob_width + 6, adr_desk[BU_BOXTXT].ob_height + 6);
		form_dial (FMD_FINISH, 0, 0, 0, 0,
							 adr_desk[BU_BOXTXT].ob_x + bureau.xd - 3, adr_desk[BU_BOXTXT].ob_y + bureau.yd - 3,
							 adr_desk[BU_BOXTXT].ob_width + 6, adr_desk[BU_BOXTXT].ob_height + 6);
	}
	else if (((object == ZERO) || (object == BU_IMG)) &&
					 (adr_desk[BU_ICN].ob_state & SELECTED) && (mk == 0))
	{		/* Bureau cliqu‚ -> d‚s‚lectionner */
		adr_desk[BU_ICN].ob_state &= ~SELECTED;
		form_dial (FMD_FINISH, 0, 0, 0, 0,
						 adr_desk[BU_ICN].ob_x + bureau.xd, adr_desk[BU_ICN].ob_y + bureau.yd,
						 adr_desk[BU_ICN].ob_width, adr_desk[BU_ICN].ob_height);
	}
	else if ((object == BU_ICN) && NOT (adr_desk[BU_ICN].ob_state & SELECTED) && (mk == 0))
	{		/* Ic“ne clic simple -> s‚lectionner */
		adr_desk[BU_ICN].ob_state |= SELECTED;
		form_dial (FMD_FINISH, 0, 0, 0, 0,
						 adr_desk[BU_ICN].ob_x + bureau.xd, adr_desk[BU_ICN].ob_y + bureau.yd,
						 adr_desk[BU_ICN].ob_width, adr_desk[BU_ICN].ob_height);
	}
}
/* #] event_desk () Ev‚nements sur le bureau :											*/ 
/* #[ formulaire () Formulaire libre :															*/
void formulaire (void)
{
OBJECT *adr;
int quit = FALSE;
char *adr_bakrsc = 0L;			/* Ptr sur ‚tat formulaire */
MFDB img;

	img.fd_addr = NULL;
	if (_app)
		menu_tnormal (adr_menu, T_FICHIER, 0); 	/* Inverser titre */
	else
	{
		win[WINDACC].w_bar[T_FICHIER + 3].ob_state |= SELECTED;
		draw_object (T_FICHIER + 3, WINDACC);
	}
	wind_update (BEG_MCTRL);								/* Bloquer menu */
	adr = ir_trindex[FORMULAIRE];
	if (work_display.n_color >= 16)
	{
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
		adr[TXT1].ob_spec.tedinfo->te_color = 8;
		adr[TXT1].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[TXT1].ob_spec.tedinfo->te_color |= (1 << 8);
		adr[TXT2].ob_spec.tedinfo->te_color = 8;
		adr[TXT2].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[TXT2].ob_spec.tedinfo->te_color |= (1 << 8);
	}
	bak_rsc (adr, &adr_bakrsc);
	formm_draw (adr, TXT1, TRUE, &img);
	do
	{
		if (dialog (DLG_FORM, adr, FALSE, FALSE, BLANK, &img, TRUE))
		{
			switch (object)
			{
			case P_CONFIRME :
				quit = TRUE;
				break;
			case P_ANNULE :
				quit = TRUE;
				res_rsc (adr, &adr_bakrsc);
				break;
			}
		}
	} while (NOT quit);
	formm_undraw (adr, &img);
	if (adr_bakrsc)							/* Si ‚tat formulaire sauvegard‚ */
		free (adr_bakrsc);
	wind_update (END_MCTRL);								/* D‚bloquer menu */
	if (_app)
		menu_tnormal (adr_menu, T_FICHIER, 1); 	/* Inverser titre */
	else
	{
		win[WINDACC].w_bar[T_FICHIER + 3].ob_state &= ~SELECTED;
		draw_object (T_FICHIER + 3, WINDACC);
	}
}
/* #] formulaire () Formulaire libre :															*/ 
/* #[ aff_infos () Infos sur BIG :																	*/
void aff_infos (void)
{
OBJECT *adr;
int quit = FALSE, index, tit, menuf = FALSE;
MFDB img;

	if (buf[0] == BEV_WMENU)
	{
		index = buf[5];
		tit = buf[3];
		menuf = TRUE;
	}
	if (_app)
		menu_tnormal (adr_menu, T_BUREAU, 0); /* Inverser titre */
	wind_update (BEG_MCTRL);							/* Bloquer menu */
	adr = ir_trindex[INFOS];
	if (work_display.n_color >= 16)
	{
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
	}
	formm_draw (adr, BLANK, FALSE, &img);
	do
	{
		dialog (DLG_FORM, adr, FALSE, FALSE, BLANK, &img, FALSE);
		if (object == INF_SORTIE)
			quit = TRUE;
	} while (NOT quit);
	formm_undraw (adr, &img);
	wind_update (END_MCTRL);							/* D‚bloquer menu */
	if (_app)
		menu_tnormal (adr_menu, T_BUREAU, 1); /* Inverser titre */
	if (menuf)
	{
		win[index].w_bar[tit].ob_state &= ~SELECTED;
		draw_object (tit, index);
	}
}
/* #] aff_infos () Infos sur BIG :																	*/ 
/* #[ test_alerte () Affichage d'alertes BIG :											*/
void test_alerte (void)
{
	big_alert (1, AL_POINTEUR, "", TRUE);
}
/* #] test_alerte () Affichage d'alertes BIG :											*/ 
/* #[ pform_wind () Affichage petit formulaire en fenˆtre :					*/
void pform_wind (void)
{
OBJECT *adr;

	adr = ir_trindex[PETITFORM];
	if (work_display.n_color >= 16)
	{
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
		adr[PF_CHAMP1].ob_spec.tedinfo->te_color = 8;
		adr[PF_CHAMP1].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[PF_CHAMP1].ob_spec.tedinfo->te_color |= (1 << 8);
		adr[PF_CHAMP2].ob_spec.tedinfo->te_color = 8;
		adr[PF_CHAMP2].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[PF_CHAMP2].ob_spec.tedinfo->te_color |= (1 << 8);
		adr[PF_CHAMP3].ob_spec.tedinfo->te_color = 8;
		adr[PF_CHAMP3].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[PF_CHAMP3].ob_spec.tedinfo->te_color |= (1 << 8);
		adr[PF_CHAMP4].ob_spec.tedinfo->te_color = 8;
		adr[PF_CHAMP4].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[PF_CHAMP4].ob_spec.tedinfo->te_color |= (1 << 8);
	}
	if (_app)
	{
		adr[PF_TRAME].ob_spec.obspec.fillpattern = adr_desk->ob_spec.obspec.fillpattern;
		adr[PF_COLOR].ob_spec.obspec.interiorcol = adr_desk->ob_spec.obspec.interiorcol;
	}
	formw_draw (adr, PETIT_FORM, TITRE_PF, BLANK, PF_CHAMP1, WATR_FORMB, winicn);
	if (_app)
		menu_tnormal (adr_menu, T_FENETRES, 1);
}
/* #] pform_wind () Affichage petit formulaire en fenˆtre :					*/ 
/* #[ trame_desk ()Modification trame du bureau :										*/
void trame_desk (void)
{
OBJECT *adr;

	adr = win[PETIT_FORM].w_cont.w_form.w_tree;
	adr_desk->ob_spec.obspec.fillpattern = adr[PF_TRAME].ob_spec.obspec.fillpattern;
	adr_desk->ob_spec.obspec.interiorcol = adr[PF_COLOR].ob_spec.obspec.interiorcol;
	form_dial (FMD_FINISH, 0, 0, 0, 0, bureau.xd, bureau.yd, bureau.wd, bureau.hd);
}
/* #] trame_desk ()Modification trame du bureau :										*/ 
/* #[ gform_wind () Affichage grand formulaire en fenˆtre :					*/
void gform_wind (void)
{
OBJECT *adr;

	adr = ir_trindex[GRANDFORM];
	if (work_display.n_color >= 16)
	{
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
	}
	formw_draw (adr, GRAND_FORM, TITRE_GF, BLANK, BLANK, WATR_CURRB, winicn);
	if (_app)
		menu_tnormal (adr_menu, T_FENETRES, 1);
}
/* #] gform_wind () Affichage grand formulaire en fenˆtre :					*/ 
/* #[ objets_desk () Modifications objets du bureau :								*/
void objets_desk (void)
{
OBJECT *adr;

	adr = win[GRAND_FORM].w_cont.w_form.w_tree;
	set_text (adr_desk, BU_TXT1, get_text (adr, GF_PU1));
	set_text (adr_desk, BU_TXT2, get_text (adr, GF_PU2));
	form_dial (FMD_FINISH, 0, 0, 0, 0,
						 adr_desk[BU_TXT1 - 1].ob_x, adr_desk[BU_TXT1 - 1].ob_y + bureau.yd,
						 adr_desk[BU_TXT1 - 1].ob_width, adr_desk[BU_TXT1 - 1].ob_height);
	adr_desk[BU_ICN].ob_spec.iconblk->ib_pdata = adr[GF_ICN].ob_spec.bitblk->bi_pdata;
	form_dial (FMD_FINISH, 0, 0, 0, 0,
						 adr_desk[BU_ICN].ob_x, adr_desk[BU_ICN].ob_y + bureau.yd,
						 adr_desk[BU_ICN].ob_width, adr_desk[BU_ICN].ob_height);
}
/* #] objets_desk () Modifications objets du bureau :								*/ 
/* #[ charge_texte () Chargement texte Ascii en fenˆtre :						*/
void charge_texte (void)
{
char chemin[125], fname[13] = "", fichier[125], *selec, *ptr;
long longueur;
int fh, count = 1, dummy, w_cell, h_cell;

	if (win[TEXTE].w_icon > BLANK)
		return;

	if ((fgdos) && (fid != 1))		/* Prendre la taille boŒtes caractŠres */
	{
		vst_font (work_display.handle, fid);
		vst_arbpt (work_display.handle, size, &dummy, &dummy, &dummy, &h_cell);
		vst_setsize (work_display.handle, size, &dummy, &dummy, &w_cell, &dummy);
	}
	else
		vst_height (work_display.handle, size, &dummy, &dummy, &w_cell, &h_cell);
	strcpy (chemin, pathapp);					/* Chemin de l'application */
	selec = get_string (CHARGETXT);		/* R‚cup‚rer message s‚lecteur */
	if (selector (chemin, "\*.TXT", fname, selec) == TRUE && strlen (fname))
	{					/* Si un fichier a ‚t‚ choisi */
		strcpy (fichier, chemin);
		strcat (fichier, fname);					/* Reconstituer son chemin d'accŠs */
		longueur = exist (fichier, ZERO);
		if (longueur)											/* S'il existe */
		{
			if (win[TEXTE].w_cont.w_adr)
				free (win[TEXTE].w_cont.w_adr);	/* Lib‚rer le pr‚c‚dent s'il y en a */
			win[TEXTE].w_cont.w_adr = (char *)malloc (longueur);	/* R‚server m‚moire */
			if (win[TEXTE].w_cont.w_adr)
			{
				fh = (int)Fopen (fichier, FO_READ);							/* Ouvrir le fichier */
				Fread (fh, longueur, win[TEXTE].w_cont.w_adr);	/* Le charger */
				Fclose (fh);																		/* Refermer */

				for (ptr = win[TEXTE].w_cont.w_adr ; ptr < win[TEXTE].w_cont.w_adr + longueur ; ptr++)
				{
					if (*ptr == '\r')
					{
						*ptr = '\0';
						count++;
					}
				}
				open_window (TEXTE, WTYP_NORM, WATR_CURRB,
										 bureau.xd + 10, bureau.yd + (bureau.hd / 4), bureau.wd - 20, bureau.hd / 2,
										 100, 100, BLANK, BLANK, w_cell, h_cell,
										 (WFARROW|WFGROUP), TEXT_CRSR,
										 winicn, aff_texte, close_txt, get_string (TIT_TXT), "",
										 (long) ((long) MAX_LEN * (long) w_cell),
										 (long) ((long) count * (long) h_cell),
										 FALSE, TRUE, TOOL, ZERO, WHITE);
			}
		}
	}
	if (_app)
		menu_tnormal (adr_menu, T_FENETRES, 1);
}
/* #] charge_texte () Chargement texte Ascii en fenˆtre :						*/ 
/* #[ aff_texte () Affichage du texte :															*/
void aff_texte (void)
{
char *ptr;
int i, x, y, count = ZERO, lignes, xw, yw, ww, hw, dummy, 
		id, wcell, hcell, extent[8], largeur;
char fontname [35];

	if (win[TEXTE].w_cont.w_adr)
	{
		if ((fgdos) && (fid > 1))
		{
			vst_font (work_display.handle, fid);
			vst_arbpt (work_display.handle, size, &dummy, &dummy, &dummy, &hcell);
			vst_setsize (work_display.handle, size, &dummy, &dummy, &wcell, &dummy);
		}
		else
			vst_height (work_display.handle, size, &dummy, &dummy, &wcell, &hcell);
		lignes = (int)(win[TEXTE].w_htot / (long)win[TEXTE].w_hunit) - 1;	/* Nbre de lignes */
		vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);	/* En haut … gauche */
		vst_effects (work_display.handle, EFNONE);		/* D‚sactiver tous les effets */
		ptr = win[TEXTE].w_cont.w_adr;								/* D‚but du texte */
		for (i = 0 ; i < (win[TEXTE].w_lin / win[TEXTE].w_hunit) ; i++)
		{
			ptr += (strlen (ptr) + 2);		/* Sauter au d‚but de l'affichage */
			count++;
		}
		zone_work (TEXTE, &xw, &yw, &ww, &hw);
		y = yw;										/* Positionner l'affichage des lignes */
		vswr_mode (work_display.handle, MD_XOR);		/* Dessin en mode Xor */
		while ((y < yw + hw) && (count < lignes))
		{
			if ((fgdos) && (fid > 1))
				vqt_f_extent (work_display.handle, ptr, extent);
			else
				vqt_extent (work_display.handle, ptr, extent);
			largeur = extent[2] - extent[0];
			if (win[TEXTE].w_bar[TL_LEFT].ob_state & SELECTED)
				x = xw - (int)win[TEXTE].w_col;
			else if (win[TEXTE].w_bar[TL_CENTER].ob_state & SELECTED)
				x = xw + (((int)win[TEXTE].w_wtot - largeur) / 2) - (int)win[TEXTE].w_col;
			else if (win[TEXTE].w_bar[TL_RIGHT].ob_state & SELECTED)
				x = xw + ((int)win[TEXTE].w_wtot - largeur) - (int)win[TEXTE].w_col;

			if ((fgdos) && (fid > 1))	/* Ecrire chaque ligne */
				v_ftext (work_display.handle, x, y, ptr);
			else
				v_gtext (work_display.handle, x, y, ptr);

			count++;
			ptr += strlen (ptr) + 2;
			y += hcell;
		}
		if ((fgdos) && (fid > 1))
		{
			id = vqt_name (work_display.handle, 1, fontname);
			vst_font (work_display.handle, id);
			vst_arbpt (work_display.handle, 13, &dummy, &dummy, &dummy, &hcell);
			vst_setsize (work_display.handle, 13, &dummy, &dummy, &wcell, &dummy);
		}
	}
}
/* #] aff_texte () Affichage du texte :															*/ 
/* #[ set_font () Choix fonte et taille :														*/
void set_font (void)
{
int id, ident, wcell, hcell, dummy;
char chaine[35];

	size = atoi (get_text (win[TEXTE].w_bar, TL_SIZE));
	ident = match (win[TEXTE].w_bar, TL_FONT);
	if (ident)
	{
		fid = vqt_name (work_display.handle, ident, chaine);
		vst_font (work_display.handle, fid);
		if (ident > 1)
		{
			vst_arbpt (work_display.handle, size, &dummy, &dummy, &dummy, &hcell);
			vst_setsize (work_display.handle, size, &dummy, &dummy, &wcell, &dummy);
		}
		else
			vst_height (work_display.handle, size, &dummy, &dummy, &wcell, &hcell);
		win[TEXTE].w_htot = (win[TEXTE].w_htot / (long)win[TEXTE].w_hunit) * hcell;
		win[TEXTE].w_col = (win[TEXTE].w_col / (long)win[TEXTE].w_wunit) * wcell;
		win[TEXTE].w_lin = (win[TEXTE].w_lin / (long)win[TEXTE].w_hunit) * hcell;
		win[TEXTE].w_wunit = wcell;
		win[TEXTE].w_hunit = hcell;
		id = vqt_name (work_display.handle, 1, chaine);
		vst_font (work_display.handle, id);
	}
	height_sliders (TEXTE, FALSE);
}
/* #] set_font () Choix fonte et taille :														*/ 
/* #[ close_txt () Ferme la fenˆtre texte :													*/
void close_txt (void)
{
	if (win[TEXTE].w_hg > 0)
	{
		buf[3] = win[TEXTE].w_hg;
		closed ();
		if (win[TEXTE].w_cont.w_adr)
		{
			free (win[TEXTE].w_cont.w_adr);
			win[TEXTE].w_cont.w_adr = 0L;
		}
	}
}
/* #] close_txt () Ferme la fenˆtre texte :													*/ 
/* #[ charge_image () Chargement image Degas en fenˆtre :						*/
void charge_image (void)
{
char chemin[125], fname[13] = "", fichier[125], *selec;
int longueur = 32000, reso, fh, index, fmenu, menu, souris, i, pal;
static int table[] = {0, 2, 3, 6, 4, 7, 5, 8, 9, 10, 11, 14, 12, 15, 13, 1};

	if (buf[0] == BEV_WMENU)
	{
		index = WINDMENU;
		fmenu = TRUE;
		menu = W_MENU;
		souris = THIN_CROSS;
	}
	else if (buf[0] == BEV_WHIER)
	{
		index = IMAGE;
		win[buf[7]].w_bar[buf[3]].ob_state &= ~SELECTED;
		draw_object (buf[3], buf[7]);
		fmenu = FALSE;
		menu = MENU;
		souris = POINT_HAND;
	}
	else
	{
		index = IMAGE;
		menu_tnormal (adr_menu, T_FENETRES, 0);
		fmenu = FALSE;
		menu = BLANK;
		souris = POINT_HAND;
	}

	if ((index == IMAGE) && (win[index].w_icon > BLANK))
	{
		if (_app)
			menu_tnormal (adr_menu, T_FENETRES, 1);
		else
		{
			win[WINDACC].w_bar[T_FENETRES + 3].ob_state &= ~SELECTED;
			draw_object (T_FENETRES + 3, WINDACC);
			win[WINDACC].w_bar[T_FENETRES + 3].ob_state &= ~SELECTED;
			draw_object (T_FENETRES + 3, WINDACC);
		}
		return;
	}

	strcpy (chemin, pathapp);					/* Chemin de l'application */
	selec = get_string (CHARGEIMG);		/* R‚cup‚rer message s‚lecteur */
	if (selector (chemin, "\*.PI?", fname, selec) == TRUE && strlen (fname))
	{					/* Si un fichier a ‚t‚ choisi */
		strcpy (fichier, chemin);
		strcat (fichier, fname);					/* Reconstituer son chemin d'accŠs */
		if (exist (fichier, 0))					/* S'il existe */
		{
			if (win[index].w_cont.w_img.fd_addr)
				free (win[index].w_cont.w_img.fd_addr);	/* Lib‚rer le pr‚c‚dent s'il y en a */
			win[index].w_cont.w_img.fd_addr = (char *)malloc (longueur);	/* R‚server m‚moire */
			if (win[index].w_cont.w_img.fd_addr)
			{
				fh = (int)Fopen (fichier, FO_READ);								/* Ouvrir le fichier */
				Fread (fh, 2, &reso);
				win[index].w_cont.w_img.fd_w = 640 - ((reso == 0) * 320);
				win[index].w_cont.w_img.fd_h = 200 + ((reso == 2) * 200);
				win[index].w_cont.w_img.fd_wdwidth = win[index].w_cont.w_img.fd_w / 16;
				win[index].w_cont.w_img.fd_stand = 1;
				win[index].w_cont.w_img.fd_nplanes = work_display.n_plane;
				if (index == WINDMENU)
					free (win[WINDMENU].w_pal);
				win[index].w_pal = (Palette *)malloc (16 * sizeof (Palette));
				for (i = 0 ; i < 16 ; i++)
				{				/* Charger la palette et la changer en indexs VDI */
					Fread (fh, 2, &pal);
					win[index].w_pal[table[i]][0] = ((pal >> 8) * 143) - 72;
					win[index].w_pal[table[i]][1] = (((pal >> 4) & 0xf) * 143) - 72;
					win[index].w_pal[table[i]][2] = ((pal & 0xf) * 143) - 72;
				}

				Fread (fh, longueur, win[index].w_cont.w_img.fd_addr);	/* Charger image */
				Fclose (fh);																	/* Refermer fichier */

				if (open_window (index, WTYP_PICT, WATR_CURRB,
												 bureau.xd + (bureau.wd / 3), bureau.yd + (bureau.hd / 3), 320, 150,
												 100, 100,
												 win[index].w_cont.w_img.fd_w,
												 win[index].w_cont.w_img.fd_h,
												 16, 16, WFARROW, souris,
												 winicn, ZERO, close_img, get_string (TIT_IMG), "",
												 (long) win[index].w_cont.w_img.fd_w,
												 (long) win[index].w_cont.w_img.fd_h,
												 fmenu, FALSE, menu, ZERO, WHITE))
				{
					if (index == WINDMENU)
						win[index].w_close = close_wmenu;
				}
			}
		}
	}
	if (index == WINDMENU)
	{
		win[WINDMENU].w_bar[TW_OPTIONS + 3].ob_state &= ~SELECTED;
		draw_object (TW_OPTIONS + 3, WINDMENU);
		height_sliders (index, FALSE);
	}
	else
	{
		if (_app)
			menu_tnormal (adr_menu, T_FENETRES, 1);
	}
}
/* #] charge_image () Chargement image Degas en fenˆtre :						*/ 
/* #[ close_img () Ferme la fenˆtre image :													*/
void close_img (void)
{
	if (win[IMAGE].w_hg > 0)
	{
		buf[3] = win[IMAGE].w_hg;
		closed ();
		if (win[IMAGE].w_cont.w_img.fd_addr)
		{
			free (win[IMAGE].w_cont.w_img.fd_addr);
			win[IMAGE].w_cont.w_img.fd_addr = 0L;
			free (win[IMAGE].w_pal);	/* Lib‚rer la m‚moire palette */
			win[IMAGE].w_pal = (Palette *)ZERO;
		}
	}
}
/* #] close_img () Ferme la fenˆtre image :													*/ 
/* #[ cree_histo () Cr‚er histo et ouvrir fenˆtre :									*/
void cree_histo (void)
{
int i;

	if (win[HISTO].w_icon > BLANK)
		return;						/* Pas si fenˆtre ic“nis‚e */
	if (win[HISTO].w_cont.w_adr)
		free (win[HISTO].w_cont.w_adr);		/* Lib‚rer le pr‚c‚dent s'il y en a */
	win[HISTO].w_cont.w_adr = malloc (8);
	for (i = ZERO ; i < 8 ; i++)
		*(win[HISTO].w_cont.w_adr + i) = (rand () & 0xFF);
	open_window (HISTO, WTYP_NORM,
							 (NAME|CLOSER|MOVER|SIZER|FULLER|INFO|SMALLER|CYCLER|ALLSIZER),
							 bureau.xd + (bureau.wd / 6), bureau.yd + (bureau.hd / 6),
							 bureau.wd / 2, bureau.hd / 2,
							 100, 100, BLANK, BLANK, 1, 1, WFARROW, ZERO,
							 winicn, aff_histo, close_histo,
							 get_string (TIT_HIST), get_string (INF_HIST),
							 1L, 1L, FALSE, FALSE, BLANK, 9, BLACK);
}
/* #] cree_histo () Cr‚er histo et ouvrir fenˆtre :									*/ 
/* #[ aff_histo () Affichage de l'histogramme :											*/
void aff_histo (void)
{
int pxy[4], xw, yw, ww, hw, i, largeur, hauteur, maxi = ZERO, datas[8];

	for (i = ZERO ; i < 8 ; i++)
		datas[i] = *(win[HISTO].w_cont.w_adr + i);
	zone_work (HISTO, &xw, &yw, &ww, &hw);
		/* 8 pixels de chaque cot‚ et entre chaque histo */
	largeur = (ww - (16) - (7 * 8)) / 8;
	for (i = 0 ; i < 8 ; i++)
		maxi = max (maxi, (int)win[HISTO].w_cont.w_adr[i]);
	vswr_mode (work_display.handle, MD_REPLACE);			/* Dessin en mode Remplacement */
	vsf_perimeter (work_display.handle, TRUE);				/* Dessiner le p‚rimŠtre */
	vsf_color (work_display.handle, 2);								/* Couleur rouge */
	vsf_interior (work_display.handle, FIS_PATTERN);	/* Motif de remplissage tram‚ */
	fill_tab (pxy, 4, xw, yw, xw + ww, yw + hw - 8);	/* Pr‚parer trac‚ histos */
	for (i = 0 ; i < 8 ; i++)
	{
		vsf_style (work_display.handle, i + 1);	/* La trame */
		pxy[0] += 8;
		hauteur = (int)((double)win[HISTO].w_cont.w_adr[i] / (double)maxi * ((double)hw - 16));
		pxy[1] = yw + hw - 8 - hauteur;
		pxy[2] = pxy[0] + largeur;
		v_bar (work_display.handle, pxy);			/* Dessiner l'histo */
		pxy[0] += largeur;
	}
}
/* #] aff_histo () Affichage de l'histogramme :											*/ 
/* #[ close_histo () Ferme la fenˆtre histo :												*/
void close_histo (void)
{
	if (win[HISTO].w_hg > 0)
	{
		buf[3] = win[HISTO].w_hg;
		closed ();
		if (win[HISTO].w_cont.w_adr)
		{
			free (win[HISTO].w_cont.w_adr);
			win[HISTO].w_cont.w_adr = 0L;
		}
	}
}
/* #] close_histo () Ferme la fenˆtre histo :												*/ 
/* #[ close_wmenu () Ferme la fenˆtre image avec menu :							*/
void close_wmenu (void)
{
	if (win[WINDMENU].w_hg > 0)
	{
		buf[3] = win[WINDMENU].w_hg;
		closed ();
		if (win[WINDMENU].w_cont.w_img.fd_addr)
		{
			free (win[WINDMENU].w_cont.w_img.fd_addr);
			win[WINDMENU].w_cont.w_img.fd_addr = 0L;
			free (win[WINDMENU].w_pal);	/* Lib‚rer la m‚moire palette */
			win[WINDMENU].w_pal = (Palette *)ZERO;
		}
	}
}
/* #] close_wmenu () Ferme la fenˆtre image avec menu :							*/ 
/* #[ icone () Alerte avec choix ic“ne :														*/
void icone (void)
{
char icn;
char *alrt;

	icn = buf[6] + 47;
	alrt = get_string (AL_ICONES);
	alrt[1] = icn;
	big_alert (1, BLANK, alrt, TRUE);
	win[buf[7]].w_bar[buf[3]].ob_state &= ~SELECTED;
	draw_object (buf[3], buf[7]);
}
/* #] icone () Alerte avec choix ic“ne :														*/ 
/* #[ winicn () Routine de redraw de fenˆtre ic“nifi‚e :						*/
void winicn (void)
{
int index, xw, yw, ww, hw, pxy[8], w, h, color[2];
BITBLK *img;
MFDB nul = {0}, pic;

	index = find_index (buf[3]);

	#if INTGR
	if (win[index].w_type == WTYP_FORM)		/* R‚cup‚rer l'image */
		img = rs_frimg[ICN_WF];
	else
		img = rs_frimg[ICN_WN];
	#else
	if (win[index].w_type == WTYP_FORM)
		rsrc_gaddr (R_FRIMG, ICN_WF, &img);	/* Adr image pointeur sur pointeur */
	else
		rsrc_gaddr (R_FRIMG, ICN_WN, &img);
	#endif
	img = *((BITBLK **)img);							/* Pointeur sur l'image */

	zone_work (index, &xw, &yw, &ww, &hw);

	w = (img->bi_wb * 8) - 1;	/* Pr‚parer affichage de l'image */
	h = img->bi_hl - 1;
	fill_tab (pxy, 8,
						ZERO, ZERO, w, h,
						xw + ((ww - w) / 2), yw + ((hw - h) / 2),
						xw + ((ww - w) / 2) + w, yw + ((hw - h) / 2) + h);
	pic.fd_addr = (void *) img->bi_pdata;
	pic.fd_w = w;
	pic.fd_h = h;
  pic.fd_wdwidth = img->bi_wb / 2;
	pic.fd_stand = 0;
	pic.fd_nplanes = 1;

	color[0] = BLUE;
	color[1] = WHITE;
  vrt_cpyfm (work_display.handle, MD_REPLACE, pxy, &pic, &nul, color);
}
/* #] winicn () Routine de redraw de fenˆtre ic“nifi‚e :						*/ 
/* #[ clos_acc () Fermeture fenˆtre ppale si accessoire :						*/
void clos_acc (void)
{
int i = ZERO;

	do			/* Pour chaque fenˆtre */
	{
		if (((win[i].w_hg > ZERO) || (win[i].w_icon > BLANK)) && (i != WINDACC))
		{				/* Fermer chaque fenˆtre */
			if (NOT (win[i].w_icon > BLANK))	/* Si pas ic“nis‚e */
			{
				buf[3] = win[i].w_hg;
				(*win[i].w_close)();
			}
			else if (win[i].w_icon > BLANK)		/* Si ic“nis‚e */
			{
				win[i].w_attr = win[i].w_old.w_att;						/* Restaurer attributs */
				win[i].w_curr.g_x = win[i].w_old.w_coord.g_x;	/* Restaurer coordonn‚es */
				win[i].w_curr.g_y = win[i].w_old.w_coord.g_y;
				win[i].w_curr.g_w = win[i].w_old.w_coord.g_w;
				win[i].w_curr.g_h = win[i].w_old.w_coord.g_h;
				win[i].w_old.w_coord.g_x = win[i].w_old.w_coord.g_y =
						win[i].w_old.w_coord.g_w = win[i].w_old.w_coord.g_h = ZERO;
				if (win[i].w_icon == i)					/* Si ic“nis‚e seule */
				{
					buf[3] = win[i].w_hg;		/* appeler sa fonction de fermeture */
					(*win[i].w_close)();
				}
				win[i].w_icon = BLANK;
			}
		}
	} while (NOT (win[i++].w_flags & WFLAST));
	if (win[WINDACC].w_icon > BLANK)
	{
		win[WINDACC].w_attr = win[WINDACC].w_old.w_att;
		win[WINDACC].w_curr.g_x = win[WINDACC].w_old.w_coord.g_x;
		win[WINDACC].w_curr.g_y = win[WINDACC].w_old.w_coord.g_y;
		win[WINDACC].w_curr.g_w = win[WINDACC].w_old.w_coord.g_w;
		win[WINDACC].w_curr.g_h = win[WINDACC].w_old.w_coord.g_h;
		win[WINDACC].w_old.w_coord.g_x = win[WINDACC].w_old.w_coord.g_y =
				win[WINDACC].w_old.w_coord.g_w = win[WINDACC].w_old.w_coord.g_h = ZERO;
		win[WINDACC].w_icon = BLANK;
	}
	if (buf[0] != AC_CLOSE)
	{
		wind_close (win[WINDACC].w_hg);
		wind_delete (win[WINDACC].w_hg);
	}
	win[WINDACC].w_hg = ZERO;
}
/* #] clos_acc () Fermeture fenˆtre ppale si accessoire :						*/ 
/* #[ acc_loop () Boucle d'attente appel accessoire :								*/
void acc_loop (void)
{
int accrun = FALSE, i;

	do
	{
		evnt_mesag (buf);
		if (buf[0] == AC_OPEN)
		{
			/* Cacher les emplacements des accessoires de bureau */
			for (i = (M_INFOS + 1) ; i < (M_INFOS + 8) ; i++)
				ir_trindex[MENU][i].ob_flags |= HIDETREE;
			/* R‚duire la hauteur de la boŒte du 1ø menu */
			ir_trindex[MENU][M_INFOS - 1].ob_height = ir_trindex[MENU][M_INFOS].ob_height;

			if (open_window (WINDACC, WTYP_NORM,
											 (NAME|CLOSER|FULLER|MOVER|SIZER|SMALLER|CYCLER|ALLSIZER),
											 bureau.xd + 20, bureau.yd + (bureau.hd / 3), bureau.wd / 2, bureau.hd / 3,
											 100, 100, BLANK, BLANK, 10, 10, WFARROW, ZERO,
											 winicn, red_acc, clos_acc,
											 get_string (TW_ACC), "",
											 (long) bureau.wd, (long) bureau.hd,
											 TRUE, FALSE, MENU, 14, BLUE))
				accrun = TRUE;
		}
	} while (NOT accrun);
}
/* #] acc_loop () Boucle d'attente appel accessoire :								*/ 
/* #[ red_acc () Fausse fonction de redraw fenˆtre d'accessoire :		*/
void red_acc (void)
{
}
/* #] red_acc () Fausse fonction de redraw fenˆtre d'accessoire :		*/ 
/* #[ relief () effet de relief en couleurs :												*/
void relief (void)
{
OBJECT *adr;
int i, j, tab1[] = {AIDE1, CONTACT, AIDE2, MERCI, TOOL,
										POP_UP1, POP_UP2, PU_LIBRE, SIZES, FONTS},
		tab2[] = {FORMULAIRE, PETITFORM, GRANDFORM, TOOL};
	/* Les boŒtes-racine doivent ˆtre en gris 100% */
	for (i = 0 ; i < 10 ; i++)
	{
		adr = ir_trindex[tab1[i]];
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
	}
	/* Les B_SELEC doivent ˆtre BackGround */
	for (i = 0 ; i < 5 ; i++)
	{
		adr = ir_trindex[tab2[i]];
		j = ZERO;
		do
		{				/* Si UserDef et SELECTABLE */
			if ((adr[j].ob_type == G_USERDEF) &&
					(adr[j].ob_flags & SELECTABLE))
				adr[j].ob_flags |= BKGR;
		} while (NOT (adr[j++].ob_flags & LASTOB));
	}
}
/* #] relief () effet de relief en couleurs :												*/ 

