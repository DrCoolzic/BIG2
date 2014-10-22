/*! @file BIG2DEMO.C
@brief BIG Demo program

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
#include <BIG_IMGS.H>

int				fgdos, fid, size;	/*!< work with Speedo */
t_image_dial	bimg;				/*!< desk image */
char*			zone = ZERO;		/*!< desk image load buffer */

#define INTGR 0						/*!< resource not integrated */
#define BIMAGE "BIG2DEMO.IMG"		/*!< Name of image to load */

#if INTGR							/*!< working with integrated resource */
#include "BIG2DEMO.RSH"
#define TRINDEX(OBJECT *)rs_trindex
#define FRSTR(char *)rs_frstr
#define NUM_TREE 22

#else								/*!< working with external resource */
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

/* functions prototype */
void main(void);
void set_bimg(void);
void set_form(void);
void ptr_fonctions(void);
void load_fontspeedo(void);
void event_mesag(int *flacc, int *quit);
void event_pmenu(int *quit);
void event_freepu(void);
void event_wform(void);
void event_wmenu(int *flacc);
void event_whier(void);
void event_tool(void);
void event_desk(void);
void aff_infos(void);
void formulaire(void);
void test_alerte(void);
void pform_wind(void);
void trame_desk(void);
void gform_wind(void);
void objets_desk(void);
void charge_texte(void);
void aff_texte(void);
void set_font(void);
void close_txt(void);
void charge_image(void);
void close_img(void);
void cree_histo(void);
void aff_histo(void);
void close_histo(void);
void close_wmenu(void);
void icone(void);
void winicn(void);
void clos_acc(void);
void acc_loop(void);
void red_acc(void);
void relief(void);


/*! Initialization and main loop */
void main(void) {
	int evnt, quit = FALSE, flacc = FALSE;
	MFDB img;

	/* Initialiser */
	if(initial("BIG2DEMO.RSC", MENU, BUREAU, NUM_TREE,
				TRINDEX, FRSTR, NB_FENETRES, TITREACC)) {
		if(_app) set_bimg();			/* Set image on the desk */
		ptr_fonctions();
		set_form();
		load_fontspeedo();

		if(work_display.n_color >= 16)	/* if we are in color mode */
			relief();
		aide_en_ligne = K_CTRL;			/* Help-bubble with Control key */

		do {
			if((_app == ZERO) &&(NOT flacc)) {	
				acc_loop();
				flacc = TRUE;
			}	/* we are running as an accessory */

			evnt = dialog(DLG_DESK, adr_desk, TRUE, TRUE, PU_LIBRE, &img, FALSE);
			
			if(evnt & MU_MESAG)
				event_mesag(&flacc, &quit);
			
			if((evnt & MU_BUTTON) &&(buf[0] == BEV_FREEPU))
				event_freepu();
			
			if((evnt & MU_BUTTON) &&(buf[0] == BEV_WFORM))
				event_wform();
			
			if((evnt & MU_BUTTON) &&(buf[0] == BEV_WMENU))
				event_wmenu(&flacc);
			
			if((evnt & MU_BUTTON) &&(buf[0] == BEV_WHIER))
				event_whier();
			
			if((evnt & MU_BUTTON) &&(buf[0] == BEV_TOOL))
				event_tool();
			
			if((evnt & MU_BUTTON) &&(NOT wind_find(mx, my)))
				event_desk();
		} while(NOT quit);
		
		if(fgdos)
			vst_unload_fonts(work_display.handle, 0);
		end();
	}	/* init OK */
}	/* end of main */


/*! place image on the desk */
void set_bimg(void) {

	if(img_attach(&bimg, BIMAGE, BUREAU, BU_IMG, TRUE, TRUE)) {
		adr_desk[BU_IMG].ob_flags &= ~HIDETREE;		/* object to draw */
		form_dial(FMD_FINISH, ZERO, ZERO, ZERO, ZERO,
				bureau.xd, bureau.yd, bureau.wd, bureau.hd);
	}	/* desk image found */
}	/* end of set_bimg */


/*! setup associated form */
void set_form(void) {
	form_attach(MENU, M_WWIND, MHIERA);
	form_attach(FORMULAIRE, FO_PUT, POP_UP2);
	form_attach(FORMULAIRE, FO_PUI, POPUP_ICN);
	form_attach(FORMULAIRE, P_AIDE, AIDE1);
	form_attach(INFOS, INF_INFOS, CONTACT);
	form_attach(INFOS, INF_MERCI, MERCI);
	form_attach(PETITFORM, PF_TRAME, TRAMES);
	form_attach(PETITFORM, PF_COLOR, COULEURS);
	form_attach(PETITFORM, PF_AIDE, AIDE2);
	form_attach(GRANDFORM, GF_PU1, POP_UP1);
	form_attach(GRANDFORM, GF_PL, POP_UP1);
	form_attach(GRANDFORM, GF_PU2, POP_UP2);
	form_attach(GRANDFORM, GF_ICN, POPUP_ICN);
	form_attach(GRANDFORM, GF_AIDE, AIDE2);
	form_attach(W_MENU, MW_AL, HIERWIND);
	form_attach(TOOL, TL_FONT, FONTS);
	form_attach(TOOL, TL_SIZE, SIZES);
	bulle_attach(FORMULAIRE, FO_PUT, BA_PU);
	bulle_attach(GRANDFORM, GF_PU2, BA_PU);
}	/* endof set_form */


/*! assign function to pointers */
void ptr_fonctions(void) {
	fnc[0] =(void *) aff_infos;			/* main menu Infos */
	fnc[1] =(void *) formulaire;		/* main menu Form */
	fnc[2] =(void *) pform_wind;		/* main menu small form */
	fnc[3] =(void *) gform_wind;		/* main menu large form */
	fnc[4] =(void *) test_alerte;		/* free form alert */
	fnc[5] =(void *) aff_infos;			/* menu in window info */
	fnc[6] =(void *) charge_texte;		/* hier menu load characters */
	fnc[7] =(void *) charge_image;		/* hier menu load image */
	fnc[8] =(void *) formulaire;		/* free popup in free form */
	fnc[9] =(void *) pform_wind;		/* small form in free popup */
	fnc[10] =(void *) gform_wind;		/* large form in free popup */
	fnc[11] =(void *) charge_texte;		/* load text in free popup */
	fnc[12] =(void *) charge_image;		/* load image in free popup */
	fnc[13] =(void *) cree_histo;		/* load histo in free popup */
	fnc[14] =(void *) icone;			/* Alert with icon choice */
	fnc[15] =(void *) icone;			/* Alert with icon choice */
	fnc[16] =(void *) icone;			/* Alert with icon choice */
	fnc[17] =(void *) icone;			/* Alert with icon choice */
	fnc[18] =(void *) icone;			/* Alert with icon choice */
	fnc[19] =(void *) icone;			/* Alert with icon choice */
	fnc[20] =(void *) icone;			/* Alert with icon choice */
	fnc[21] =(void *) icone;			/* Alert with icon choice */
}	/* end of ptr_fonctions */


/*! load Sppedo fonts */
void load_fontspeedo(void) {
	int nb_fonts, i;
	char fontname[128], temp[35];

	if(vq_vgdos() == GDOS_FSM) {
		graf_mouse(BUSYBEE , 0);
		nb_fonts = vst_load_fonts(work_display.handle, 0);
		fgdos = TRUE;
		i = 2;
		
		do {
			vqt_name(work_display.handle, i, temp);
			strcpy(fontname, "  ");
			strcat(fontname, temp);
			if(strlen(fontname) > 33)
				fontname[33] = '\0';
			set_text(ir_trindex[FONTS], i, fontname);
			ir_trindex[FONTS][i].ob_state &= ~DISABLED;
			i++;
		} while(((i - 1) <= nb_fonts) &&(i <= 41));
		graf_mouse(ARROW, 0);
		fid = 1;
	} /* if GDOS loaded */
	size = 13;
}	/* endof load_fontspeedo */


/*! message events */
void event_mesag(int *flacc, int *quit) {
	if(buf[0] == MN_SELECTED)
		event_pmenu(quit);
	else if(buf[0] == BEV_HIERM) {	
		if(buf [6] == MH_HISTO) {
			cree_histo();
			menu_tnormal(adr_menu, T_FENETRES, TRUE);
		}
	}	/* hier menu event */
	else if(buf[0] == AC_CLOSE) {
		clos_acc();
		*flacc = FALSE;
	}
	else if((buf[0] == WM_CLOSED) &&(buf[3] == win[WINDACC].w_hg))
		*flacc = FALSE;
}	/* endof event_mesag */


/*! event in main menu */
void event_pmenu(int *quit) {
	int i;

	switch(buf[4]) 	{
	case M_ALERTE :
		big_alert(2, ALERT, "", TRUE);
		menu_tnormal(adr_menu, T_FICHIER, 1);
		break;
	
	case M_WMENU :
		win[WINDMENU].w_pal =(Palette *)malloc(work_display.n_color *sizeof(Palette));
		for(i = 0 ; i < work_display.n_color ; i++) {
			win[WINDMENU].w_pal[i][0] = work_display.palette[i][0];
			win[WINDMENU].w_pal[i][1] = work_display.palette[i][1];
			win[WINDMENU].w_pal[i][2] = work_display.palette[i][2];
		}
		open_window(WINDMENU, WTYP_PICT, WATR_CURRB,
			bureau.xd +(bureau.wd / 5), bureau.yd +(bureau.hd / 5), 320, 150,
			100, 100, 200, 200, 16, 16,
			WFARROW, THIN_CROSS,
			winicn, ZERO, close_wmenu, get_string(TIT_MENU), "",
			(long) bureau.wd,(long) bureau.hd, TRUE, FALSE, W_MENU,
			10, BLACK);
		menu_tnormal(adr_menu, T_FENETRES, 1);
		break;
	
	case M_QUITTER :
		*quit = TRUE;
		menu_tnormal(adr_menu, T_FICHIER, 1);
		break;
	}	/* switch case */
}	/* endof event_pmenu */


/*! free popup event */
void event_freepu(void) {
	int i;

	switch(buf[4]) {
	case PUL_ALERT :
		if(_app) 
			menu_tnormal(adr_menu, T_FICHIER, 0);
		else {
			win[WINDACC].w_bar[T_FICHIER + 3].ob_state |= SELECTED;
			draw_object(T_FICHIER + 3, WINDACC);
		}
		big_alert(2, ALERT, "", TRUE);

		if(_app)
			menu_tnormal(adr_menu, T_FICHIER, 1);
		else {
			win[WINDACC].w_bar[T_FICHIER + 3].ob_state &= ~SELECTED;
			draw_object(T_FICHIER + 3, WINDACC);
		}
		break;
	
	case PUL_WMENU :
		if(_app)
			menu_tnormal(adr_menu, T_FENETRES, 0);
		else {
			win[WINDACC].w_bar[T_FENETRES + 3].ob_state |= SELECTED;
			draw_object(T_FENETRES + 3, WINDACC);
		}
		if(win[WINDMENU].w_hg <= ZERO) {
			win[WINDMENU].w_pal =(Palette *)malloc(work_display.n_color * sizeof(Palette));
			for(i = 0 ; i < work_display.n_color ; i++) {
				win[WINDMENU].w_pal[i][0] = work_display.palette[i][0];
				win[WINDMENU].w_pal[i][1] = work_display.palette[i][1];
				win[WINDMENU].w_pal[i][2] = work_display.palette[i][2];
			}
			open_window(WINDMENU, WTYP_PICT, WATR_CURRB,
				bureau.xd +(bureau.wd / 5), bureau.yd +(bureau.hd / 5), 320, 150,
				100, 100, 200, 200, 16, 16,
				WFARROW, THIN_CROSS,
				winicn, ZERO, close_wmenu, get_string(TIT_MENU), "",
				(long) bureau.wd,(long) bureau.hd,
				TRUE, FALSE, W_MENU,
				10, BLACK);
		}
		else {
			buf[3] = win[WINDMENU].w_hg;
			(*win[WINDMENU].w_top)();
		}
		if(_app)
			menu_tnormal(adr_menu, T_FENETRES, 1);
		else {
			win[WINDACC].w_bar[T_FENETRES + 3].ob_state &= ~SELECTED;
			draw_object(T_FENETRES + 3, WINDACC);
		}
		break;
	}	/* end switch case */
}	/* event_freepu */


/*! form in window event */
void event_wform(void) {
	if(buf[3] == PETIT_FORM) 	{
		switch(buf[4]) 		{
		case PF_CONFIRME :
		case PF_APPLIQUE :
			win[buf[3]].w_cont.w_form.w_tree[buf[4]].ob_state &= ~SELECTED;
			free(&win[PETIT_FORM].w_cont.w_form.w_bak);	/* free memory */
			bak_rsc(win[PETIT_FORM].w_cont.w_form.w_tree, &win[PETIT_FORM].w_cont.w_form.w_bak);
			if(_app)
				trame_desk();
			break;

		case PF_ANNULE :
		case PF_RAZ :
			win[buf[3]].w_cont.w_form.w_tree[buf[4]].ob_state &= ~SELECTED;
			if(buf[4] == PF_RAZ)
				objc_xedit(win[PETIT_FORM].w_cont.w_form.w_tree,	/* remove cursor */
					ZERO, win[PETIT_FORM].w_cont.w_form.w_edit,
					&win[PETIT_FORM].w_cont.w_form.w_pos,
					ED_END, TRUE, PETIT_FORM);
			res_rsc(win[PETIT_FORM].w_cont.w_form.w_tree, &win[PETIT_FORM].w_cont.w_form.w_bak);
			if(buf[4] == PF_RAZ)
				objc_xedit(win[PETIT_FORM].w_cont.w_form.w_tree,	/* put back cursor */
					ZERO, win[PETIT_FORM].w_cont.w_form.w_edit,
					&win[PETIT_FORM].w_cont.w_form.w_pos,
					ED_INIT, TRUE, PETIT_FORM);
			break;
		} /* end switch case */
		if(buf[4] == PF_RAZ)
			print_page(PETIT_FORM);
	}	/* small form */
	
	else if(buf[3] == GRAND_FORM) {
		switch(buf[4]) {
		case GF_CONFIRME :
		case GF_APPLIQUE :
			win[buf[3]].w_cont.w_form.w_tree[buf[4]].ob_state &= ~SELECTED;
			free(&win[GRAND_FORM].w_cont.w_form.w_bak);	/* Libérer mémoire */
			bak_rsc(win[GRAND_FORM].w_cont.w_form.w_tree, &win[GRAND_FORM].w_cont.w_form.w_bak);
			if(_app)
				objets_desk();
			break;
		
		case GF_ANNULE :
		case GF_RAZ :
			win[buf[3]].w_cont.w_form.w_tree[buf[4]].ob_state &= ~SELECTED;
			res_rsc(win[GRAND_FORM].w_cont.w_form.w_tree, &win[GRAND_FORM].w_cont.w_form.w_bak);
			break;
		}	/* end switch case */
		
		if((buf[4] == GF_RAZ) ||(buf[4] == GF_APPLIQUE))
			print_page(GRAND_FORM);
	}	/* large form */
}	/* endof event_wform */


/*! menu in window events */
void event_wmenu(int *flacc) {
	int i;

	if(buf[5] == WINDMENU) {
		switch(buf[4] - 3) {	/* 3 objets have been added to the menu */
		
		case MW_IMAGE :
			charge_image();
			break;
		
		case MW_ALERTE :
			big_alert(2, ALERT, "", TRUE);
			if(win[buf[5]].w_bar[buf[4]].ob_state & CHECKED)
				menu_icheck(win[buf[5]].w_bar, buf[4], FALSE);
			else
				menu_icheck(win[buf[5]].w_bar, buf[4], TRUE);
			win[buf[5]].w_bar[buf[3]].ob_state &= ~SELECTED;
			draw_object(buf[3], buf[5]);
			break;
		}	/* end switch */
	}	/* event for main menu not running as an accessory */
	else if(buf[5] == WINDACC) {		
		switch(buf[4] - 3)	{	/* 3 objets added to main menu */
		
		case M_ALERTE :
			big_alert(2, ALERT, "", TRUE);
			win[buf[5]].w_bar[buf[3]].ob_state &= ~SELECTED;
			draw_object(buf[3], buf[5]);
			break;
		
		case M_WMENU :
			win[buf[5]].w_bar[buf[3]].ob_state &= ~SELECTED;
			draw_object(buf[3], buf[5]);
			if(win[WINDMENU].w_hg <= ZERO) {
				win[WINDMENU].w_pal =(Palette *)malloc(work_display.n_color *sizeof(Palette));
				for(i = 0 ; i < work_display.n_color ; i++) {
					win[WINDMENU].w_pal[i][0] = work_display.palette[i][0];
					win[WINDMENU].w_pal[i][1] = work_display.palette[i][1];
					win[WINDMENU].w_pal[i][2] = work_display.palette[i][2];
				}
				open_window(WINDMENU, WTYP_PICT, WATR_CURRB,
					bureau.xd +(bureau.wd / 5), bureau.yd +(bureau.hd / 5), 320, 150,
					100, 100, 200, 200, 16, 16,
					WFARROW, THIN_CROSS,
					winicn, ZERO, close_wmenu, get_string(TIT_MENU), "",
					(long) bureau.wd,(long) bureau.hd,
					TRUE, FALSE, W_MENU, 10, BLACK);
			}
			else {
				buf[3] = win[WINDMENU].w_hg;
				(*win[WINDMENU].w_top)();
			}
			break;
		
		case M_QUITTER :
			clos_acc();
			win[buf[5]].w_bar[buf[3]].ob_state &= ~SELECTED;
			*flacc = FALSE;
			break;
		} /* end of switch */
	}	/* event for main menu running as accessory */
}	/* end of event_wmenu */

 
/*! hierarchical menu in windows events */
void event_whier(void) {
	if(buf[7] == WINDMENU) {
		switch(buf[6]) {
		case MWH_ICN0 :
			buf[6] = MWH_ICN0;
			icone();
			break;
		case MWH_ICN1 :
			buf[6] = MWH_ICN1;
			icone();
			break;
		}
	}
	
	else if(buf[7] == WINDACC) 	{
		if(buf[6] == MH_HISTO) {
			cree_histo();
			menu_tnormal(win[WINDACC].w_bar, T_FENETRES + 3, TRUE);
		}
	}
}	/* end of event_whier */


/*! Toolbar events */
void event_tool(void) {

	if(buf[3] == TEXTE) {
		if((buf[4] == TL_FONT) ||(buf[4] == TL_SIZE))
			set_font();
		if(buf[4] == TL_INV) {
			if(win[TEXTE].w_bar[TL_INV].ob_state & SELECTED)
				set_work(TEXTE, 8, BLACK);
			else
				set_work(TEXTE, ZERO, WHITE);
		}
		if(buf[4] != TL_INV)
			print_page(TEXTE);
	}
}	/* end of event_tool */


/*! desk events */
void event_desk(void) {
	int old_x, old_y, new_x, new_y, dummy;

	evnt_timer(70, 0);		/* timer delay */
	graf_mkstate(&dummy, &dummy, &mk, &dummy);

	if((object == BU_ICN) &&(mk == 1)) {													/* icon still cliked -> move */
		old_x = adr_desk[object].ob_x;
		old_y = adr_desk[object].ob_y;
		graf_dragbox(adr_desk[object].ob_width, adr_desk[object].ob_height,
			adr_desk[object].ob_x + bureau.xd, adr_desk[object].ob_y + bureau.yd,
			bureau.xd, bureau.yd, bureau.wd, bureau.hd, &new_x, &new_y);
		adr_desk[object].ob_x = new_x - bureau.xd;
		adr_desk[object].ob_y = new_y - bureau.yd;
		form_dial(FMD_FINISH, 0, 0, 0, 0, old_x + bureau.xd, old_y + bureau.yd,
			adr_desk[object].ob_width, adr_desk[object].ob_height);
		form_dial(FMD_FINISH, 0, 0, 0, 0,
			adr_desk[object].ob_x + bureau.xd, adr_desk[object].ob_y + bureau.yd,
			adr_desk[object].ob_width, adr_desk[object].ob_height);
	}
	
	else if(((object == BU_BOXTXT) ||(object == BU_TXT1) 
			||(object == BU_TXT2)) &&(mk == 1)) {		
		/* text stiil cliked -> move */
		old_x = adr_desk[BU_BOXTXT].ob_x;
		old_y = adr_desk[BU_BOXTXT].ob_y;
		graf_dragbox(adr_desk[BU_BOXTXT].ob_width, adr_desk[BU_BOXTXT].ob_height,
			adr_desk[BU_BOXTXT].ob_x + bureau.xd, adr_desk[BU_BOXTXT].ob_y + bureau.yd,
			bureau.xd, bureau.yd, bureau.wd, bureau.hd, &new_x, &new_y);
		adr_desk[BU_BOXTXT].ob_x = new_x - bureau.xd;
		adr_desk[BU_BOXTXT].ob_y = new_y - bureau.yd;
		form_dial(FMD_FINISH, 0, 0, 0, 0, old_x + bureau.xd - 3, old_y + bureau.yd - 3,
			adr_desk[BU_BOXTXT].ob_width + 6, adr_desk[BU_BOXTXT].ob_height + 6);
		form_dial(FMD_FINISH, 0, 0, 0, 0,
			adr_desk[BU_BOXTXT].ob_x + bureau.xd - 3, 
			adr_desk[BU_BOXTXT].ob_y + bureau.yd - 3,
			adr_desk[BU_BOXTXT].ob_width + 6, adr_desk[BU_BOXTXT].ob_height + 6);
	}
	
	else if(((object == ZERO) ||(object == BU_IMG)) &&
		(adr_desk[BU_ICN].ob_state & SELECTED) &&(mk == 0)) {		
		/* desk clicked -> deselect */
		adr_desk[BU_ICN].ob_state &= ~SELECTED;
		form_dial(FMD_FINISH, 0, 0, 0, 0,
			adr_desk[BU_ICN].ob_x + bureau.xd, adr_desk[BU_ICN].ob_y + bureau.yd,
			adr_desk[BU_ICN].ob_width, adr_desk[BU_ICN].ob_height);
	}
	
	else if((object == BU_ICN) && NOT(adr_desk[BU_ICN].ob_state & SELECTED) 
			&& (mk == 0)) {		
		/* simple click icon -> select */
		adr_desk[BU_ICN].ob_state |= SELECTED;
		form_dial(FMD_FINISH, 0, 0, 0, 0,
			adr_desk[BU_ICN].ob_x + bureau.xd, adr_desk[BU_ICN].ob_y + bureau.yd,
			adr_desk[BU_ICN].ob_width, adr_desk[BU_ICN].ob_height);
	}

}	/* end of event_desk() */ 


/*! Free form */
void formulaire(void) {
	OBJECT *adr;
	int quit = FALSE;
	char *adr_bakrsc = 0L;			/* ptr form state */
	MFDB img;

	img.fd_addr = NULL;
	if(_app)
		menu_tnormal(adr_menu, T_FICHIER, 0); 	/* Inverser titre */
	else {
		win[WINDACC].w_bar[T_FICHIER + 3].ob_state |= SELECTED;
		draw_object(T_FICHIER + 3, WINDACC);
	}
	wind_update(BEG_MCTRL);								/* Bloquer menu */
	adr = ir_trindex[FORMULAIRE];
	if(work_display.n_color >= 16) {
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
		adr[TXT1].ob_spec.tedinfo->te_color = 8;
		adr[TXT1].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[TXT1].ob_spec.tedinfo->te_color |=(1 << 8);
		adr[TXT2].ob_spec.tedinfo->te_color = 8;
		adr[TXT2].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[TXT2].ob_spec.tedinfo->te_color |=(1 << 8);
	}
	bak_rsc(adr, &adr_bakrsc);
	formm_draw(adr, TXT1, TRUE, &img);
	do {
		if(dialog(DLG_FORM, adr, FALSE, FALSE, BLANK, &img, TRUE)) {
			switch(object) {
			case P_CONFIRME :
				quit = TRUE;
				break;
			case P_ANNULE :
				quit = TRUE;
				res_rsc(adr, &adr_bakrsc);
				break;
			}
		}
	} while(NOT quit);
	formm_undraw(adr, &img);
	if(adr_bakrsc)							/* Si état formulaire sauvegardé */
		free(adr_bakrsc);
	wind_update(END_MCTRL);								/* Débloquer menu */
	if(_app)
		menu_tnormal(adr_menu, T_FICHIER, 1); 	/* Inverser titre */
	else {
		win[WINDACC].w_bar[T_FICHIER + 3].ob_state &= ~SELECTED;
		draw_object(T_FICHIER + 3, WINDACC);
	}
}	/* end of formulaire() */


/*! Display info about BIG */
void aff_infos(void) {
	OBJECT *adr;
	int quit = FALSE, index, tit, menuf = FALSE;
	MFDB img;

	if(buf[0] == BEV_WMENU) {
		index = buf[5];
		tit = buf[3];
		menuf = TRUE;
	}
	if(_app)
		menu_tnormal(adr_menu, T_BUREAU, 0); /* Inverser titre */
	wind_update(BEG_MCTRL);							/* Bloquer menu */
	adr = ir_trindex[INFOS];
	if(work_display.n_color >= 16) {
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
	}
	formm_draw(adr, BLANK, FALSE, &img);
	do {
		dialog(DLG_FORM, adr, FALSE, FALSE, BLANK, &img, FALSE);
		if(object == INF_SORTIE)
			quit = TRUE;
	} while(NOT quit);
	formm_undraw(adr, &img);
	wind_update(END_MCTRL);							/* Débloquer menu */
	if(_app)
		menu_tnormal(adr_menu, T_BUREAU, 1); /* Inverser titre */
	if(menuf) {
		win[index].w_bar[tit].ob_state &= ~SELECTED;
		draw_object(tit, index);
	}
}	/* endof aff_infos() */ 


/*! Display BIG alert */
void test_alerte(void) {
	big_alert(1, AL_POINTEUR, "", TRUE);
}	/* endof test_alerte() */ 


/*! Display small form in window */
void pform_wind(void) {
	OBJECT *adr;

	adr = ir_trindex[PETITFORM];
	if(work_display.n_color >= 16) {
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
		adr[PF_CHAMP1].ob_spec.tedinfo->te_color = 8;
		adr[PF_CHAMP1].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[PF_CHAMP1].ob_spec.tedinfo->te_color |=(1 << 8);
		adr[PF_CHAMP2].ob_spec.tedinfo->te_color = 8;
		adr[PF_CHAMP2].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[PF_CHAMP2].ob_spec.tedinfo->te_color |=(1 << 8);
		adr[PF_CHAMP3].ob_spec.tedinfo->te_color = 8;
		adr[PF_CHAMP3].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[PF_CHAMP3].ob_spec.tedinfo->te_color |=(1 << 8);
		adr[PF_CHAMP4].ob_spec.tedinfo->te_color = 8;
		adr[PF_CHAMP4].ob_spec.tedinfo->te_color |= 7 << 4;
		adr[PF_CHAMP4].ob_spec.tedinfo->te_color |=(1 << 8);
	}
	if(_app) {
		adr[PF_TRAME].ob_spec.obspec.fillpattern = adr_desk->ob_spec.obspec.fillpattern;
		adr[PF_COLOR].ob_spec.obspec.interiorcol = adr_desk->ob_spec.obspec.interiorcol;
	}
	formw_draw(adr, PETIT_FORM, TITRE_PF, BLANK, PF_CHAMP1, WATR_FORMB, winicn);
	if(_app)
		menu_tnormal(adr_menu, T_FENETRES, 1);
}	/* endof pform_wind() */ 


/*! modify pattern on desk */
void trame_desk(void) {
	OBJECT *adr;

	adr = win[PETIT_FORM].w_cont.w_form.w_tree;
	adr_desk->ob_spec.obspec.fillpattern = adr[PF_TRAME].ob_spec.obspec.fillpattern;
	adr_desk->ob_spec.obspec.interiorcol = adr[PF_COLOR].ob_spec.obspec.interiorcol;
	form_dial(FMD_FINISH, 0, 0, 0, 0, bureau.xd, bureau.yd, bureau.wd, bureau.hd);
}	/* endof trame_desk() */


/*! Display large form in window */
void gform_wind(void) {
	OBJECT *adr;

	adr = ir_trindex[GRANDFORM];
	if(work_display.n_color >= 16) {
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
	}
	formw_draw(adr, GRAND_FORM, TITRE_GF, BLANK, BLANK, WATR_CURRB, winicn);
	if(_app)
		menu_tnormal(adr_menu, T_FENETRES, 1);
}	/* endof gform_wind() */ 


/*! Modifies objects on desk */
void objets_desk(void) {
	OBJECT *adr;

	adr = win[GRAND_FORM].w_cont.w_form.w_tree;
	set_text(adr_desk, BU_TXT1, get_text(adr, GF_PU1));
	set_text(adr_desk, BU_TXT2, get_text(adr, GF_PU2));
	form_dial(FMD_FINISH, 0, 0, 0, 0,
		adr_desk[BU_TXT1 - 1].ob_x, adr_desk[BU_TXT1 - 1].ob_y + bureau.yd,
		adr_desk[BU_TXT1 - 1].ob_width, adr_desk[BU_TXT1 - 1].ob_height);
	adr_desk[BU_ICN].ob_spec.iconblk->ib_pdata = adr[GF_ICN].ob_spec.bitblk->bi_pdata;
	form_dial(FMD_FINISH, 0, 0, 0, 0,
		adr_desk[BU_ICN].ob_x, adr_desk[BU_ICN].ob_y + bureau.yd,
		adr_desk[BU_ICN].ob_width, adr_desk[BU_ICN].ob_height);
}	/* endof objets_desk() */ 


/*! loads ASCII text in  window */
void charge_texte(void) {
	char chemin[125], fname[13] = "", fichier[125], *selec, *ptr;
	long longueur;
	int fh, count = 1, dummy, w_cell, h_cell;

	if(win[TEXTE].w_icon > BLANK)
		return;

	if((fgdos) &&(fid != 1)) {	
		/* take size of character box */
		vst_font(work_display.handle, fid);
		vst_arbpt(work_display.handle, size, &dummy, &dummy, &dummy, &h_cell);
		vst_setsize(work_display.handle, size, &dummy, &dummy, &w_cell, &dummy);
	}
	
	else
		vst_height(work_display.handle, size, &dummy, &dummy, &w_cell, &h_cell);
	strcpy(chemin, pathapp);					/* Chemin de l'application */
	selec = get_string(CHARGETXT);		/* Récupérer message sélecteur */
	if(selector(chemin, "\*.TXT", fname, selec) == TRUE && strlen(fname)) {	
		/* if file selected */
		strcpy(fichier, chemin);
		strcat(fichier, fname);					/* recreate access path */
		longueur = exist(fichier, ZERO);
		if(longueur) {							/* if exist */
			if(win[TEXTE].w_cont.w_adr)
				free(win[TEXTE].w_cont.w_adr);	/* Libérer le précédent s'il y en a */
			win[TEXTE].w_cont.w_adr =(char *)malloc(longueur);	/* Réserver mémoire */
			if(win[TEXTE].w_cont.w_adr) {
				fh =(int)Fopen(fichier, FO_READ);	/* open file */
				Fread(fh, longueur, win[TEXTE].w_cont.w_adr);	/* load */
				Fclose(fh);							/* close file */

				for(ptr = win[TEXTE].w_cont.w_adr ; 
						ptr < win[TEXTE].w_cont.w_adr + longueur ; ptr++) {
					if(*ptr == '\r') {
						*ptr = '\0';
						count++;
					}
				}
				open_window(TEXTE, WTYP_NORM, WATR_CURRB,
					bureau.xd + 10, bureau.yd +(bureau.hd / 4), bureau.wd - 20, 
					bureau.hd / 2,
					100, 100, BLANK, BLANK, w_cell, h_cell,
					(WFARROW|WFGROUP), TEXT_CRSR,
					winicn, aff_texte, close_txt, get_string(TIT_TXT), "",
					(long)((long) MAX_LEN *(long) w_cell),
					(long)((long) count *(long) h_cell),
					FALSE, TRUE, TOOL, ZERO, WHITE);
			}
		}
	}
	if(_app)
		menu_tnormal(adr_menu, T_FENETRES, 1);
}	/* endof charge_texte() */ 


/*! Display text */
void aff_texte(void) {
	char *ptr;
	int i, x, y, count = ZERO, lignes, xw, yw, ww, hw, dummy, 
		id, wcell, hcell, extent[8], largeur;
	char fontname [35];

	if(win[TEXTE].w_cont.w_adr) {
		if((fgdos) &&(fid > 1)) {
			vst_font(work_display.handle, fid);
			vst_arbpt(work_display.handle, size, &dummy, &dummy, &dummy, &hcell);
			vst_setsize(work_display.handle, size, &dummy, &dummy, &wcell, &dummy);
		}
		else
			vst_height(work_display.handle, size, &dummy, &dummy, &wcell, &hcell);
		lignes =(int)(win[TEXTE].w_htot /(long)win[TEXTE].w_hunit) - 1;	/* nb lines */
		vst_alignment(work_display.handle, HLEFT, VTOP, &dummy, &dummy);/* top left */
		vst_effects(work_display.handle, EFNONE);		/* remove all effects */
		ptr = win[TEXTE].w_cont.w_adr;					/* start of text */
		for(i = 0 ; i <(win[TEXTE].w_lin / win[TEXTE].w_hunit) ; i++) {
			ptr +=(strlen(ptr) + 2);		/* jump start of display */
			count++;
		}
		zone_work(TEXTE, &xw, &yw, &ww, &hw);
		y = yw;									/* set line display */
		vswr_mode(work_display.handle, MD_XOR);		/* draw mode Xor */
		while((y < yw + hw) &&(count < lignes)) {
			if((fgdos) &&(fid > 1))
				vqt_f_extent(work_display.handle, ptr, extent);
			else
				vqt_extent(work_display.handle, ptr, extent);
			largeur = extent[2] - extent[0];
			if(win[TEXTE].w_bar[TL_LEFT].ob_state & SELECTED)
				x = xw -(int)win[TEXTE].w_col;
			else if(win[TEXTE].w_bar[TL_CENTER].ob_state & SELECTED)
				x = xw +(((int)win[TEXTE].w_wtot - largeur) / 2) -(int)win[TEXTE].w_col;
			else if(win[TEXTE].w_bar[TL_RIGHT].ob_state & SELECTED)
				x = xw +((int)win[TEXTE].w_wtot - largeur) -(int)win[TEXTE].w_col;

			if((fgdos) &&(fid > 1))	/* write each line */
				v_ftext(work_display.handle, x, y, ptr);
			else
				v_gtext(work_display.handle, x, y, ptr);

			count++;
			ptr += strlen(ptr) + 2;
			y += hcell;
		}
		if((fgdos) &&(fid > 1)) {
			id = vqt_name(work_display.handle, 1, fontname);
			vst_font(work_display.handle, id);
			vst_arbpt(work_display.handle, 13, &dummy, &dummy, &dummy, &hcell);
			vst_setsize(work_display.handle, 13, &dummy, &dummy, &wcell, &dummy);
		}
	}
} /* endof aff_texte() */ 


/*! select font and size */
void set_font(void) {
	int id, ident, wcell, hcell, dummy;
	char chaine[35];

	size = atoi(get_text(win[TEXTE].w_bar, TL_SIZE));
	ident = match(win[TEXTE].w_bar, TL_FONT);
	if(ident) {
		fid = vqt_name(work_display.handle, ident, chaine);
		vst_font(work_display.handle, fid);
		if(ident > 1) {
			vst_arbpt(work_display.handle, size, &dummy, &dummy, &dummy, &hcell);
			vst_setsize(work_display.handle, size, &dummy, &dummy, &wcell, &dummy);
		}
		else
			vst_height(work_display.handle, size, &dummy, &dummy, &wcell, &hcell);
		win[TEXTE].w_htot =(win[TEXTE].w_htot /(long)win[TEXTE].w_hunit) * hcell;
		win[TEXTE].w_col =(win[TEXTE].w_col /(long)win[TEXTE].w_wunit) * wcell;
		win[TEXTE].w_lin =(win[TEXTE].w_lin /(long)win[TEXTE].w_hunit) * hcell;
		win[TEXTE].w_wunit = wcell;
		win[TEXTE].w_hunit = hcell;
		id = vqt_name(work_display.handle, 1, chaine);
		vst_font(work_display.handle, id);
	}
	height_sliders(TEXTE, FALSE);
}	/* endof set_font() */ 

/*! close text window */
void close_txt(void) {
	if(win[TEXTE].w_hg > 0) {
		buf[3] = win[TEXTE].w_hg;
		closed();
		if(win[TEXTE].w_cont.w_adr) {
			free(win[TEXTE].w_cont.w_adr);
			win[TEXTE].w_cont.w_adr = 0L;
		}
	}
}	/* endof close_txt() */


/*! load Degas image in window */
void charge_image(void) {
	char chemin[125], fname[13] = "", fichier[125], *selec;
	int longueur = 32000, reso, fh, index, fmenu, menu, souris, i, pal;
	static int table[] = {0, 2, 3, 6, 4, 7, 5, 8, 9, 10, 11, 14, 12, 15, 13, 1};

	if(buf[0] == BEV_WMENU) {
		index = WINDMENU;
		fmenu = TRUE;
		menu = W_MENU;
		souris = THIN_CROSS;
	}
	
	else if(buf[0] == BEV_WHIER) {
		index = IMAGE;
		win[buf[7]].w_bar[buf[3]].ob_state &= ~SELECTED;
		draw_object(buf[3], buf[7]);
		fmenu = FALSE;
		menu = MENU;
		souris = POINT_HAND;
	}

	else {
		index = IMAGE;
		menu_tnormal(adr_menu, T_FENETRES, 0);
		fmenu = FALSE;
		menu = BLANK;
		souris = POINT_HAND;
	}

	if((index == IMAGE) &&(win[index].w_icon > BLANK)) {
		if(_app)
			menu_tnormal(adr_menu, T_FENETRES, 1);
		else {
			win[WINDACC].w_bar[T_FENETRES + 3].ob_state &= ~SELECTED;
			draw_object(T_FENETRES + 3, WINDACC);
			win[WINDACC].w_bar[T_FENETRES + 3].ob_state &= ~SELECTED;
			draw_object(T_FENETRES + 3, WINDACC);
		}
		return;
	}

	strcpy(chemin, pathapp);			/* application path */
	selec = get_string(CHARGEIMG);		/* get selector message */
	if(selector(chemin, "\*.PI?", fname, selec) == TRUE && strlen(fname)) {	
		/* file has been choosen */
		strcpy(fichier, chemin);
		strcat(fichier, fname);					/* rebuild access path */
		if(exist(fichier, 0))	{				/* exist */
			if(win[index].w_cont.w_img.fd_addr)
				free(win[index].w_cont.w_img.fd_addr);	/* free previous if exist */
			win[index].w_cont.w_img.fd_addr =(char *)malloc(longueur);	/* reserve memory */
			if(win[index].w_cont.w_img.fd_addr) {
				fh =(int)Fopen(fichier, FO_READ);	/* open file */
				Fread(fh, 2, &reso);
				win[index].w_cont.w_img.fd_w = 640 -((reso == 0) * 320);
				win[index].w_cont.w_img.fd_h = 200 +((reso == 2) * 200);
				win[index].w_cont.w_img.fd_wdwidth = win[index].w_cont.w_img.fd_w / 16;
				win[index].w_cont.w_img.fd_stand = 1;
				win[index].w_cont.w_img.fd_nplanes = work_display.n_plane;
				if(index == WINDMENU)
					free(win[WINDMENU].w_pal);
				win[index].w_pal =(Palette *)malloc(16 * sizeof(Palette));
				for(i = 0 ; i < 16 ; i++) {				
					/* load pallet - change in VDI indexes */
					Fread(fh, 2, &pal);
					win[index].w_pal[table[i]][0] =((pal >> 8) * 143) - 72;
					win[index].w_pal[table[i]][1] =(((pal >> 4) & 0xf) * 143) - 72;
					win[index].w_pal[table[i]][2] =((pal & 0xf) * 143) - 72;
				}

				Fread(fh, longueur, win[index].w_cont.w_img.fd_addr);	/* load image */
				Fclose(fh);

				if(open_window(index, WTYP_PICT, WATR_CURRB,
					bureau.xd +(bureau.wd / 3), bureau.yd +(bureau.hd / 3), 320, 150,
					100, 100,
					win[index].w_cont.w_img.fd_w,
					win[index].w_cont.w_img.fd_h,
					16, 16, WFARROW, souris,
					winicn, ZERO, close_img, get_string(TIT_IMG), "",
					(long) win[index].w_cont.w_img.fd_w,
					(long) win[index].w_cont.w_img.fd_h,
					fmenu, FALSE, menu, ZERO, WHITE)) {
					if(index == WINDMENU)
						win[index].w_close = close_wmenu;
				}
			}
		}
	}
	if(index == WINDMENU) {
		win[WINDMENU].w_bar[TW_OPTIONS + 3].ob_state &= ~SELECTED;
		draw_object(TW_OPTIONS + 3, WINDMENU);
		height_sliders(index, FALSE);
	}
	else {
		if(_app)
			menu_tnormal(adr_menu, T_FENETRES, 1);
	}
}	/* endof charge_image() */ 


/*! close image in window */
void close_img(void) {
	if(win[IMAGE].w_hg > 0) {
		buf[3] = win[IMAGE].w_hg;
		closed();
		if(win[IMAGE].w_cont.w_img.fd_addr) {
			free(win[IMAGE].w_cont.w_img.fd_addr);
			win[IMAGE].w_cont.w_img.fd_addr = 0L;
			free(win[IMAGE].w_pal);	/* Libérer la mémoire palette */
			win[IMAGE].w_pal =(Palette *)ZERO;
		}
	}
} /* endof close_img() */ 


/*! Create histogram and open windows */
void cree_histo(void) {
	int i;

	if(win[HISTO].w_icon > BLANK)
		return;						/* not if iconified win */
	if(win[HISTO].w_cont.w_adr)
		free(win[HISTO].w_cont.w_adr);		/* free previous if exist */
	win[HISTO].w_cont.w_adr = malloc(8);
	for(i = ZERO ; i < 8 ; i++)
		*(win[HISTO].w_cont.w_adr + i) =(rand() & 0xFF);
	open_window(HISTO, WTYP_NORM,
		(NAME|CLOSER|MOVER|SIZER|FULLER|INFO|SMALLER|CYCLER|ALLSIZER),
		bureau.xd +(bureau.wd / 6), bureau.yd +(bureau.hd / 6),
		bureau.wd / 2, bureau.hd / 2,
		100, 100, BLANK, BLANK, 1, 1, WFARROW, ZERO,
		winicn, aff_histo, close_histo,
		get_string(TIT_HIST), get_string(INF_HIST),
		1L, 1L, FALSE, FALSE, BLANK, 9, BLACK);
}	/* endof cree_histo() */ 


/*! Display histogram */
void aff_histo(void) {
	int pxy[4], xw, yw, ww, hw, i, largeur, hauteur, maxi = ZERO, datas[8];

	for(i = ZERO ; i < 8 ; i++)
		datas[i] = *(win[HISTO].w_cont.w_adr + i);
	zone_work(HISTO, &xw, &yw, &ww, &hw);
	/* 8 pixels on each side and between histo */
	largeur =(ww -(16) -(7 * 8)) / 8;
	for(i = 0 ; i < 8 ; i++)
		maxi = max(maxi,(int)win[HISTO].w_cont.w_adr[i]);
	vswr_mode(work_display.handle, MD_REPLACE);		/* draw in replace mode */
	vsf_perimeter(work_display.handle, TRUE);		/* draw perimeter */
	vsf_color(work_display.handle, 2);				/* red color */
	vsf_interior(work_display.handle, FIS_PATTERN);	/* pattern filling */
	fill_tab(pxy, 4, xw, yw, xw + ww, yw + hw - 8);	/* prepare draw histo */
	for(i = 0 ; i < 8 ; i++) {
		vsf_style(work_display.handle, i + 1);		/* the pattern */
		pxy[0] += 8;
		hauteur =(int)((double)win[HISTO].w_cont.w_adr[i] /(double)maxi *((double)hw - 16));
		pxy[1] = yw + hw - 8 - hauteur;
		pxy[2] = pxy[0] + largeur;
		v_bar(work_display.handle, pxy);			/* draw histo */
		pxy[0] += largeur;
	}
}	/* endof aff_histo() */ 


/*! Close histogram window */
void close_histo(void) {
	if(win[HISTO].w_hg > 0) {
		buf[3] = win[HISTO].w_hg;
		closed();
		if(win[HISTO].w_cont.w_adr) {
			free(win[HISTO].w_cont.w_adr);
			win[HISTO].w_cont.w_adr = 0L;
		}
	}
}	/* endof close_histo() */ 


/*! Close window with menu */
void close_wmenu(void) {
	if(win[WINDMENU].w_hg > 0) 	{
		buf[3] = win[WINDMENU].w_hg;
		closed();
		if(win[WINDMENU].w_cont.w_img.fd_addr) {
			free(win[WINDMENU].w_cont.w_img.fd_addr);
			win[WINDMENU].w_cont.w_img.fd_addr = 0L;
			free(win[WINDMENU].w_pal);	/* Libérer la mémoire palette */
			win[WINDMENU].w_pal =(Palette *)ZERO;
		}
	}
}	/* endof close_wmenu() */ 


/*! Alert with icon choice */
void icone(void) {
	char icn;
	char *alrt;

	icn = buf[6] + 47;
	alrt = get_string(AL_ICONES);
	alrt[1] = icn;
	big_alert(1, BLANK, alrt, TRUE);
	win[buf[7]].w_bar[buf[3]].ob_state &= ~SELECTED;
	draw_object(buf[3], buf[7]);
}	/* endof icone() */


/*! redraw iconified windows */
void winicn(void) {
	int index, xw, yw, ww, hw, pxy[8], w, h, color[2];
	BITBLK *img;
	MFDB nul = {0}, pic;

	index = find_index(buf[3]);

#if INTGR
	if(win[index].w_type == WTYP_FORM)		/* get image */
		img = rs_frimg[ICN_WF];
	else
		img = rs_frimg[ICN_WN];
#else
	if(win[index].w_type == WTYP_FORM)
		rsrc_gaddr(R_FRIMG, ICN_WF, &img);	/* adr image (prt on ptr) */
	else
		rsrc_gaddr(R_FRIMG, ICN_WN, &img);
#endif
	img = *((BITBLK **)img);				/* pointer on image */

	zone_work(index, &xw, &yw, &ww, &hw);

	w =(img->bi_wb * 8) - 1;				/* prepare image display */
	h = img->bi_hl - 1;
	fill_tab(pxy, 8,
		ZERO, ZERO, w, h,
		xw +((ww - w) / 2), yw +((hw - h) / 2),
		xw +((ww - w) / 2) + w, yw +((hw - h) / 2) + h);
	pic.fd_addr =(void *) img->bi_pdata;
	pic.fd_w = w;
	pic.fd_h = h;
	pic.fd_wdwidth = img->bi_wb / 2;
	pic.fd_stand = 0;
	pic.fd_nplanes = 1;

	color[0] = BLUE;
	color[1] = WHITE;
	vrt_cpyfm(work_display.handle, MD_REPLACE, pxy, &pic, &nul, color);
}	/* endof winicn() */ 


/* close main window if accessory */
void clos_acc(void) {
	int i = ZERO;

	do {		/* for each window */
	
		if(((win[i].w_hg > ZERO) ||(win[i].w_icon > BLANK)) &&(i != WINDACC)) {	
			/* Fclose each window */
			if(NOT(win[i].w_icon > BLANK)) {	/* not iconified */
				buf[3] = win[i].w_hg;
				(*win[i].w_close)();
			}
			else if(win[i].w_icon > BLANK) {	/* iconified */
			
				win[i].w_attr = win[i].w_old.w_att;				/* restore attrib */
				win[i].w_curr.g_x = win[i].w_old.w_coord.g_x;	/* restore position */
				win[i].w_curr.g_y = win[i].w_old.w_coord.g_y;
				win[i].w_curr.g_w = win[i].w_old.w_coord.g_w;
				win[i].w_curr.g_h = win[i].w_old.w_coord.g_h;
				win[i].w_old.w_coord.g_x = win[i].w_old.w_coord.g_y =
					win[i].w_old.w_coord.g_w = win[i].w_old.w_coord.g_h = ZERO;
				if(win[i].w_icon == i) {		/* iconified alone */
					buf[3] = win[i].w_hg;		/* call its closing function */
					(*win[i].w_close)();
				}
				win[i].w_icon = BLANK;
			}
		}
	} while(NOT(win[i++].w_flags & WFLAST));
	
	if(win[WINDACC].w_icon > BLANK) {
		win[WINDACC].w_attr = win[WINDACC].w_old.w_att;
		win[WINDACC].w_curr.g_x = win[WINDACC].w_old.w_coord.g_x;
		win[WINDACC].w_curr.g_y = win[WINDACC].w_old.w_coord.g_y;
		win[WINDACC].w_curr.g_w = win[WINDACC].w_old.w_coord.g_w;
		win[WINDACC].w_curr.g_h = win[WINDACC].w_old.w_coord.g_h;
		win[WINDACC].w_old.w_coord.g_x = win[WINDACC].w_old.w_coord.g_y =
			win[WINDACC].w_old.w_coord.g_w = win[WINDACC].w_old.w_coord.g_h = ZERO;
		win[WINDACC].w_icon = BLANK;
	}
	if(buf[0] != AC_CLOSE) {
		wind_close(win[WINDACC].w_hg);
		wind_delete(win[WINDACC].w_hg);
	}
	win[WINDACC].w_hg = ZERO;
}	/* endof clos_acc() */


/*! event waiting loop accessory */
void acc_loop(void) {
	int accrun = FALSE, i;

	do {
		evnt_mesag(buf);
		if(buf[0] == AC_OPEN) {
			/* Cacher les emplacements des accessoires de bureau */
			for(i =(M_INFOS + 1) ; i <(M_INFOS + 8) ; i++)
				ir_trindex[MENU][i].ob_flags |= HIDETREE;
			/* Réduire la hauteur de la boîte du 1° menu */
			ir_trindex[MENU][M_INFOS - 1].ob_height = ir_trindex[MENU][M_INFOS].ob_height;

			if(open_window(WINDACC, WTYP_NORM,
				(NAME|CLOSER|FULLER|MOVER|SIZER|SMALLER|CYCLER|ALLSIZER),
				bureau.xd + 20, bureau.yd +(bureau.hd / 3), bureau.wd / 2, bureau.hd / 3,
				100, 100, BLANK, BLANK, 10, 10, WFARROW, ZERO,
				winicn, red_acc, clos_acc,
				get_string(TW_ACC), "",
				(long) bureau.wd,(long) bureau.hd,
				TRUE, FALSE, MENU, 14, BLUE))
				accrun = TRUE;
		}
	} while(NOT accrun);
}	/* endof acc_loop() */ 


/*! dummy redraw function for accessory window */
void red_acc(void) {
}	/* endof red_acc() */ 


/*! 3D effect when running in color	*/
void relief(void) {
	OBJECT *adr;
	int i, j, tab1[] = {AIDE1, CONTACT, AIDE2, MERCI, TOOL,
		POP_UP1, POP_UP2, PU_LIBRE, SIZES, FONTS},
		tab2[] = {FORMULAIRE, PETITFORM, GRANDFORM, TOOL};
	/* root-boxes grey 100% */
	for(i = 0 ; i < 10 ; i++) {
		adr = ir_trindex[tab1[i]];
		adr->ob_spec.obspec.interiorcol = 8;
		adr->ob_spec.obspec.fillpattern = 7;
	}
	/* B_SELEC must be BackGround */
	for(i = 0 ; i < 5 ; i++) {
		adr = ir_trindex[tab2[i]];
		j = ZERO;
		do {				/* if UserDef and SELECTABLE */
			if((adr[j].ob_type == G_USERDEF) &&
				(adr[j].ob_flags & SELECTABLE))
				adr[j].ob_flags |= BKGR;
		} while(NOT(adr[j++].ob_flags & LASTOB));
	}
}	/* endof relief() */ 

