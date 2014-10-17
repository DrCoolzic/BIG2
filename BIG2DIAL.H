/*	BIG2DIAL.H */

/* #[ Variables globales : */
extern int id_top;		/* Index fenˆtre 1ø plan */
extern int old_top;
extern int pal_img;
extern MOBLK mouse;
extern int ap_id;
extern int event;
extern int buf[];
extern int mx;
extern int my;
extern int mk;
extern int edit;
extern int pos;
extern int object;
extern int kbd;
extern int key;
extern int clik;
extern int wind;
extern t_display work_display;
extern t_desktop bureau;
extern OBJECT *adr_menu;
extern OBJECT *adr_desk;
extern OBJECT **ir_trindex;
extern FNCP *fnc;
extern BigWind	*win;
/* #] Variables globales : */ 
/* #[ Prototypage des fonctions : */
int		dlg_keybd (OBJECT *adresse, int f, int *dedans, int *fl_cla);
int		dlg_mesag (void);
int		dlg_button (OBJECT *adresse, int f, int dedans, int fl_cla,
									MFDB *img, int fl_depl, int fl_notop);
int option_find (int xm, int ym);
int		menu_ok (void);
int dial (OBJECT *address, MFDB *img, int fl_move);
/* #] Prototypage des fonctions : */ 

