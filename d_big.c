
#ifdef WIN32									/* compile under Windoz */
#define cdecl
#pragma warning(disable : 4996)					/* deprecated functions */
#pragma warning(disable : 4129)					/* invalid escape sequence */
#endif

#include <AES.H>
#include <VDI.H>
#include <TOS.H>
#include <big2.h>

int		initial (char *rsc, int menu, int desk, int nb_tree,
							 OBJECT *rs_tree, char *rs_str, int n_wind, int acc){return 1;}
void	end (void){return;}

void	formm_draw (OBJECT *adr, int ed, int flmove, MFDB *img){return;}
void	formm_undraw (OBJECT *adr, MFDB *img){return;}
void	formf_draw (OBJECT *adr, int ed){return;}
void	formf_undraw (OBJECT *adr){return;}
void	create_hierm (OBJECT *adr){return;}
int		dialog (int flags, OBJECT *address, int f,
							int fl_rb, int form_pu, MFDB *img, int fl_move){return 1;}
int		match (OBJECT *adr, int button){return 1;}
void	set_popup (OBJECT *adr, int button, int option){return;}
OBJECT *create_popup (int nbre, char **liste){return (OBJECT*)1;}
int		free_popup (int posx, int posy, int form, OBJECT *adr){return 1;}
void	get_bkgr (int of_x, int of_y, int of_w, int of_h, MFDB *img){return;}
void	put_bkgr (int of_x, int of_y, int of_w, int of_h, MFDB *img){return;}
int		big_alert (int button, int number, char *str, int fl_alrt){return 1;}
char	*get_string (int number){return (char*)1;}
void	form_attach (int tree, int obj, char formular){return;}
void	bulle_attach (int tree, int obj, char bulle){return;}
void	set_interface (int under, int cunder, int ctxt_under,
										 int cicn_coche, int ctxt_coche,
										 int cicn_radio, int ctxt_radio,
										 int cicn_popup, int ctxt_popup,
										 int cmove, int cgrastr,
										 int clin_frame, int ctxt_frame, int cniceline){return;}

int cdecl parent (OBJECT *adr, int object){return 1;}
void	bak_rsc (OBJECT *tree, char **bak){return;}
void	res_rsc (OBJECT *tree, char **bak){return;}
void	dlg_mouse (void){return;}

void objc_xedit (OBJECT *adr, int car, int ed, int *po, int action, int f, int index){return;}
void	set_text (OBJECT *adr, int object, char *string){return;}
char	*get_text (OBJECT *adr, int object){return (char*)1;}

char	*path (char *pat){return (char*)1;}
long	exist (char *name, int att){return 1;}
void	set_palette (int index){return;}
long	timer (void){return 1;}
char	*trim (char *str){return (char*)1;}
void	send_mesag (int type, int wi, int x, int y, int w, int h){return;}
int		selector (char *pat, char *ext, char *file, char *title){return 1;}
void	extension (char *filename, char *ext){return;}
int		rc_intersect (GRECT *p1, GRECT *p2){return 1;}
void	clipp (int clip_flag, GRECT *area){return;}
COOKIE *first_cookie (void){return (COOKIE*)1;}				/* Le premier cookie */
COOKIE *next_cookie (COOKIE *ck){return(COOKIE*)1;}		/* Cookie suivant */
COOKIE *vq_cookie (long id){return(COOKIE*)1;}				/* Cherche un cookie par id */
COOKIE *vq_cookie_c (char *c){return(COOKIE*)1;}			/* Cherche un cookie par chaîne */
void fill_tab (int *pxy, int n, ...){return;}
void empty (int evnmt){return;}

int		formw_draw (OBJECT *adr, int index, int title, int infos,
									int wed, int attr, FNCP ricn){return 1;}
int		open_window (int index, int type, int attr,
									 int x, int y, int w, int h,
									 int wmi, int hmi, int wma, int hma,
									 int wu, int hu,
									 int flags, int fmouse,
									 FNCP ricn, FNCP redr, FNCP clos,
									 char *title, char *info,
									 long wt, long ht,
									 int fmenu, int ftool, int bar,
									 int pattern, int colorp){return 1;}
int		find_index (int wh){return 1;}
int		find_window (int mx, int my){return 1;}
void	zone_work (int index, int *xw, int *yw, int *ww, int *hw){return;}
void	height_sliders (int index, int fredraw){return;}
void	posi_vert (int index, int fredraw){return;}
void	posi_hori (int index, int fredraw){return;}
void	height_vert (int index, int fredraw){return;}
void	height_hori (int index, int fredraw){return;}
void	draw_object (int object, int index){return;}
void	print_page (int index){return;}
void	next_wind (void){return;}
void	closed (void){return;}
void	iconise (int index){return;}
void	uniconise (int index){return;}
void	alliconise (int index){return;}
void	set_work (int index, int pattern, int colorp){return;}
void	set_windows (int mtxt_mover, int ctxt_mover, int spat_mover, int cpat_mover,
									 int ctxt_info,
									 int spat_velev, int cpat_velev, int spat_helev, int cpat_helev,
									 int spat_vslid, int cpat_vslid, int spat_hslid, int cpat_hslid,
									 int cpat_gadgt, int cicn_gadgt){return;}
void bwind_calc (int request, int attr,
								 int tree, OBJECT *adrb,
								 int xd, int yd, int wd, int hd,
								 int *x, int *y, int *w, int *h){return;}


OBJECT *adr_menu, *adr_desk;	/* Adresses formulaires réservées */
OBJECT **ir_trindex;

int cliq_in;
GRECT zclip;		/* Zone de clipping à redessiner si redraw */
FNCP *fnc;	/* Pointeur sur les pointeurs de fonction liés aux objets */
int ap_id;							/* Numéro de l'application */
int buf[8];						/* Buffer d'évènements */
int mx, my, mk;				/* Position et état souris */
int edit, pos, object; /* Editable courant, pos. curseur, ob. cliqué */
int kbd, key, clik; 		/* Etat touches spéciales, touches, clics */
int wind;							/* Fenêtre cliquée */
int intgr;							/* Flag de ressource intégré ou non */
char pathapp[];				/* Chemin de l'application */
BigWind *win;				/* Pointeur sur la zone des fenêtres */
t_ident_hard_soft desc_hard_soft;
t_display work_display;
t_desktop bureau;
int aide_en_ligne;
char pathapp[1];				/* Chemin de l'application */
int _app;


