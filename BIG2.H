/*			BIG2.H			>>> Big Is Gem <<<
				Librairie de d‚veloppement sous GEM
				Cr‚‚e et d‚velopp‚e par Claude ATTARD
				version 2.13 du 11/02/99
*/

#if  !defined( __BIG__ )
#define __BIG__

/* #[ D‚finitions :																								*/
/****** Fichiers annexes ******************************************/

#include <STRING.H>
#include <AES.H>
#include <VDI.H>
#include <TOS.H>
#include <STDIO.H>
#include <STDLIB.H>
#include <CTYPE.H>

/****** D‚finitions ***********************************************/
	/* D‚finition pour le traitement sans le timer */
#if defined(LIGHT)
#define dialog dialog_light
#endif

	/* D‚finitions diverses */
enum {FALSE, TRUE};
#define NOT !
#define BLANK -1
#define ZERO 0
#define MAX_ALRT 250

	/* D‚finition pour le 15ø bit positionn‚ */
#define BIT15 0x8000

	/* Macros Maximum et Minimum */
#define max(A, B) ((A) > (B) ? (A) : (B))
#define min(A, B) ((A) < (B) ? (A) : (B))

	/* D‚finitions pour les ic“nes couleur (pas toujours d‚finis par les compilateurs) */
#if !defined( G_CICON )
#define G_CICON 33

typedef struct cicon_data {
	int num_planes;
	int *col_data;
	int *col_mask;
	int *sel_data;
	int *sel_mask;
	struct cicon_data *next_res;
} CICON;

typedef struct cicon_blk {
	ICONBLK monoblk;
	CICON *mainlist;
} CICONBLK;
#endif

	/* D‚finitions pour l'ic“nification */
#if !defined( WM_ICONIFY )
#define WM_ICONIFY 34
#define WM_UNICONIFY 35
#define WM_ALLICONIFY 36
#endif

	/* D‚finitions pour fenˆtre suivante */
#define WM_CYCLED 38
#if !defined( WM_BOTTOM )
#define WM_BOTTOM 33
#endif

	/* D‚finitions pour les UserDefs */
#define B_MOVE 17
#define B_SELEC 18
#define B_GRASTR 19
#define B_FRAME 20
#define B_HELP 21
#define B_POPUP 22
#define B_LIST 23
#define B_PICT 24
#define B_EDIT 25
#define B_SMSTR 26
#define B_UNDER 30
#define B_UNDO 31
#define B_HIERM 41
#define B_FNCP 51

	/* D‚finitions pour les ev‚nements */
#define DLG_DESK (MU_KEYBD | MU_BUTTON | MU_M1 | MU_MESAG | MU_TIMER)
#define DLG_FORM (MU_KEYBD | MU_BUTTON | MU_TIMER)
#define BEV_HIERM 150
#define BEV_FREEPU 151
#define BEV_WFORM 152
#define BEV_WMENU 153
#define	BEV_WHIER 154
#define BEV_TOOL 155

	/* Nbre d'objets fenˆtre BIG */
#define NGAD 23

	/* Nouveaux attributs de fenˆtre BIG */
#define SMALLER 0x4000
#define CYCLER 0x2000
#define ALLSIZER 0x8000

	/* D‚finitions pour les attributs de fenˆtres */
#define WATR_ALLG (NAME|CLOSER|FULLER|MOVER|INFO|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE)
#define WATR_ALLB (CLOSER|FULLER|MOVER|INFO|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE|CYCLER|SMALLER|ALLSIZER)
#define WATR_CURRG (NAME|CLOSER|FULLER|MOVER|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE)
#define WATR_CURRB (CLOSER|FULLER|MOVER|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE|CYCLER|SMALLER|ALLSIZER)
#define WATR_FORMG (NAME|CLOSER|MOVER)
#define WATR_FORMB (CLOSER|MOVER|CYCLER|SMALLER)

	/* D‚finitions pour les types de fenˆtres */
#define WTYP_NORM 1
#define WTYP_FORM 2
#define WTYP_PICT 3
#define WTYP_TOOL 0x4000
#define WTYP_MENU 0x8000

	/* D‚finitions pour les flags de fenˆtres */
			/* Plein pot */
#define WFFULL	0x0001
			/* Manipulation au clavier */
#define WFARROW	0x0002
			/* Calage taille et sliders sur les unit‚s */
#define WFCALAG	0x0004
			/* D‚calage w_lin et w_col sur ‚v‚nement Sizer */
#define WFDECAL 0x0008
			/* Groupement flŠches ascenseurs */
#define WFGROUP	0x0010
			/* DerniŠre fenˆtre */
#define WFLAST	0x0080

	/* D‚finition pour la gestion de la 3D (bit 10 ob_flags) */
#define BKGR 0x0400

	/* Pour les boutons entrainant le bouton par d‚faut si double-clic (bit 11 ob_flags) */
#define DCDEFAULT 0x0800

	/* Pour les boutons de fermeture de fenˆtres formulaires (bit 12 ob_flags) */
#define WFCLOSE 0x1000

	/* D‚fininiton pour caler … droite les B_SELEC (bit 13 ob_flags) */
#define BS_RIGHT 0x2000

	/* Bit (14 ob_flags) indiquant l'aide en ligne */
#define AIDE_OBJET 0x4000

	/* D‚finition manipulation fenˆtre formulaire arriŠre-plan (bit 15 ob_flags) */
#define NOTOP 0x8000

	/* D‚finitions pour les menus en fenˆtre */
#define WM_BKGR 0
#define WM_BOXMENU 1
#define WM_BOXARROWS 2
#define WM_LFARROW 3
#define WM_RTARROW 4
#define WM_BOXTITLES 5

	/* D‚finitions pour le clavier */
#define HELP 0x6200
#define UNDO 0x6100
#define ENTER 0x1C0D
#define RETURN 0x720D
#define BACKSPC 0x0E08
#define DELETE 0x537F
#define SH_DELETE 0x537F
#define INSERT 0x5200
#define SH_INSERT 0x5230
#define TAB 0x0F09
#define ARDN 0x5000
#define ARUP 0x4800
#define ARLF 0x4B00
#define ARRT 0x4D00
#define CT_ARLF 0x7300
#define CT_ARRT 0x7400
#define CT_ARUP 0x4800
#define CT_ARDN 0x5000
#define SH_ARDN 0x5032
#define SH_ARUP 0x4838
#define SH_ARLF 0x4B34
#define SH_ARRT 0x4D36
#define ESC 0x011B
#define CLR 0x4700
#define CT_CLR 0x7700
#define SH_CLR 0x4737
#define ESPACE 0x3920
#define F1 0x3B00
#define SH_F1 0x5400
#define F2 0x3C00
#define SH_F2 0x5500
#define F3 0x3D00
#define SH_F3 0x5600
#define F4 0x3E00
#define SH_F4 0x5700
#define F5 0x3F00
#define SH_F5 0x5800
#define F6 0x4000
#define SH_F6 0x5900
#define F7 0x4100
#define SH_F7 0x5A00
#define F8 0x4200
#define SH_F8 0x5B00
#define F9 0x4300
#define SH_F9 0x5C00
#define F10 0x4400
#define SH_F10 0x5D00

	/* D‚finition de facilit‚ (!) */
#define ADR ir_trindex
#define get_popup match

	/* D‚finitions de compl‚ment … VDI.H */
/* Ligne pointill‚e (. . . .)
… utiliser avec vsl_type (int handle, USERLINE);
et vsl_udsty (int handle, int pattern); */
#define POINTIL 0x8888

/* Forme de marqueur (vsm_type) */
#define MTPOINT		1
#define MTPLUS		2
#define MTETOILE	3
#define MTCARRE		4
#define MTCROIX		5
#define MTLOSANGE	6

/* Attributs d'effets (vst_effects) */
#define EFNONE	0x00
#define EFGRAS	0x01
#define EFGRIS	0x02
#define EFITAL	0x04
#define EFSOULI	0x08
#define EFCONT	0x10

/* Attributs d'alignement (vst_alignment) */
#define HLEFT		0
#define HCENT		1
#define HRIGHT	2
#define VDOWN		0
#define VLINE		3
#define VTOP		5

/* Attributs de taille (vst_height) */
#define HNORM		13
#define HPETIT	6
#define HMINUS	4

/* Attributs de rotation (vst_rotation) */
#define RONORM	0
#define RO90		900
#define RO180		1800
#define RO270		2700
/* #] D‚finitions :																								*/ 
/* #[ Variables globales :																				*/
extern int ap_id;							/* Num‚ro de l'application */
extern int buf[8];						/* Buffer d'‚vŠnements */
extern int mx, my, mk;				/* Position et ‚tat souris */
extern int edit, pos, object; /* Editable courant, pos. curseur, ob. cliqu‚ */
extern int kbd, key, clik; 		/* Etat touches sp‚ciales, touches, clics */
extern int wind;							/* Fenˆtre cliqu‚e */
extern int intgr;							/* Flag de ressource int‚gr‚ ou non */

extern char pathapp[];				/* Chemin de l'application */

typedef int Palette[3];

extern Palette	bpal[16];				/* Palette interne au Prg : 16 couleurs */
extern OBJECT *adr_menu, *adr_desk;	/* Adresses formulaires r‚serv‚es */

	/* Toutes les adresses d'arborescences sont dans ir_trindex */
extern OBJECT **ir_trindex;

extern int cliq_in;		/* MA 16/07/94 : Pour savoir si le clic de fermeture
												 d'un menu liste, popup ou hi‚rarchique
												 s'est deroule dans ou dehors le menu */

extern GRECT zclip;		/* Zone de clipping … redessiner si redraw */

	/* Cr‚ation des types "pointeur sur fonction" */
typedef void (*FNCP)(void);
extern FNCP *fnc;	/* Pointeur sur les pointeurs de fonction li‚s aux objets */

	/* Structures de travail avec les fenˆtres */
typedef struct form {			/* DEFINITION FENETRE FORMULAIRE */
	OBJECT	*w_tree;			/* Adresse arbre */
	int			w_edit;				/* EDITABLE courant */
	int			w_pos;				/* Position du curseur */
	char		*w_bak;				/* Sauvegarde ‚tat formulaire */
} Form;

typedef union cont {			/* DEFINITION DU CONTENU D'UNE FENETRE */
	Form		w_form;				/* Si fenˆtre formulaire */
	MFDB		w_img;				/* Si image */
	char		*w_adr;				/* Si texte ou autre adresse donn‚es */
} Cont;

typedef struct oldw {			/* DEFINITION DES DONNEES AVANT ICONIFICATION */
	GRECT		w_coord;			/* Anciennes coordonn‚es */
	int			w_att;				/* Anciens attributs */
} Oldw;

typedef struct bigwind {		/* DEFINITION D'UNE FENETRE */
	int			w_hg;					/* Handle GEM de la fenˆtre (ou -1 ou 0) */
	int			w_type;				/* Type BIG de la fenˆtre */
	int			w_attr;				/* Attributs de fenˆtre (wind_create) */

	GRECT		w_curr;				/* Coordonn‚es courantes de la fenˆtre */
	Oldw		w_old;				/* Donn‚es avant ic“nification */
	int			w_wmini;			/* Largeur minimum */
	int			w_hmini;			/* Hauteur minimum */
	int			w_wmaxi;			/* Largeur maximum */
	int			w_hmaxi;			/* Hauteur maximum */

	int			w_wunit;			/* Unit‚ de d‚calage horizontale */
	int			w_hunit;			/* Unit‚ de d‚calage verticale */

	int			w_hprot;			/* Nbre points prot‚g‚s … gauche */
	int			w_vprot;			/* Nbre points prot‚g‚s en haut */

	Cont		w_cont;				/* Selon le contenu de la fenˆtre */
	Palette	*w_pal;				/* Pointeur sur la palette de couleurs */

	OBJECT	*w_bar;				/* Si fenˆtre avec menu ou toolbar: adresse arbre */

	OBJECT	w_fen[NGAD];		/* Copie du formulaire fenˆtre */

	int			w_flags;			/* Flags Plein pot, Clavier actif, DerniŠre fenˆtre */
	int			w_mouse;			/* Forme de souris dans la fenˆtre */
	int			w_icon;				/* Ic“nification : index fenˆtre associ‚e */

	FNCP		w_redicn;			/* Pointeur sur routine de redraw si fenˆtre ic“nifi‚e */
	FNCP		w_cycle;			/* Pointeur sur routine de fenˆtre suivante */
	FNCP		w_redraw;			/* Pointeur sur routine de redraw */
	FNCP		w_top;				/* Pointeur sur routine de mise en premier plan */
	FNCP		w_close;			/* Pointeur sur routine de fermeture */
	FNCP		w_move;				/* Pointeur sur routine fenˆtre d‚plac‚e */
	FNCP		w_size;				/* Pointeur sur routine fenˆtre redimensionn‚e */
	FNCP		w_full;				/* Pointeur sur routine fenˆtre plein pot */
	FNCP		w_hslid;			/* Pointeur sur routine slider horizontal */
	FNCP		w_vslid;			/* Pointeur sur routine slider vertical */
	FNCP		w_uppage;			/* Pointeur sur routine page vers le haut */
	FNCP		w_dnpage;			/* Pointeur sur routine page vers le bas */
	FNCP		w_lfpage;			/* Pointeur sur routine page vers la gauche */
	FNCP		w_rtpage;			/* Pointeur sur routine page vers la droite */
	FNCP		w_upline;			/* Pointeur sur routine ligne vers le haut */
	FNCP		w_dnline;			/* Pointeur sur routine ligne vers le bas */
	FNCP		w_lfline;			/* Pointeur sur routine ligne vers la gauche */
	FNCP		w_rtline;			/* Pointeur sur routine ligne vers la droite */

	char		*w_title;			/* Pointeur sur titre de la fenˆtre */
	char		*w_infos;			/* Pointeur sur infos de la fenˆtre */

	long		w_wtot;				/* Largeur maxi contenu fenˆtre (en pixels) */
	long		w_htot;				/* Hauteur maxi contenu fenˆtre (en pixels) */
	long		w_lin;				/* 1Šre ligne de la fenˆtre */
	long		w_col;				/* 1Šre colonne de la fenˆtre */
} BigWind;

extern BigWind *win;				/* Pointeur sur la zone des fenˆtres */

typedef struct t_image_dial	/* STRUCTURE POUR LES IMAGES EN FORMULAIRES */
{		/* MA 26/06/1994 */
	MFDB	mfdb_image;		/* MFDB de l'image a afficher	*/
	GRECT	cur_pos;			/* position courante de l'image	*/
} t_image_dial;

	/* Structure des Cookies */
typedef struct
{
	long ident;
	union
	{
		long l;
		int i[2];
		char c[4];
	} datack;
} COOKIE;

/* types construits pour l'identification de la machine	*/
typedef enum 
{
	ST,
	STE,
	TT,
	FALCON_030,
	AUTRE_MACHINE
}	t_machine;

typedef enum
{
	PG_ST,
	PG_STE,
	PG_TT,
	PG_FALCON_030,
	AUTRE_PG
}	t_proc_graphique;

typedef enum
{	
	PROC_68000,
	PROC_68030,
	AUTRE_PROCESSEUR
}	t_cpu;

typedef enum
{
	ST_BASSE = 0,
	ST_MOYENNE = 1,
	ST_HAUTE = 2,
	VGA_16COL = 4,					/* le VGA TT */
	VGA_256COL = 5,					/* le VGA FALCON */
	R_256C_320_480 = 7,			/* le 320*480 16 col TT */
	TRUE_COLOR_320_480 = 8,	/* le 320*480 Falcon sur VGA */
	INCONNU = -1						/* aucune r‚solution normalis‚e */
} t_resolution;

typedef enum
{
	AUCUN_GDOS	=0,
	GDOS_STANDARD	=1,
	FONT_GDOS	=2,
	FSM_GDOS	=3,
	SPEEDO_GDOS	=4
}	type_gdos;

typedef struct
{
	t_machine la_machine;
	t_proc_graphique proc_graphique;
	t_cpu le_cpu;
	int v_aes;
	int v_tos;
	char multitache;
	type_gdos le_gdos;
}	t_ident_hard_soft;

typedef struct t_display
{
	size_t				taille;					/* Taille d'un ‚cran */
	size_t				taille_ligne;		/* Longueur d'une ligne en octets */
	int						handle;					/* Num‚ro de la station de travail virtuelle associ‚e a l'‚cran */
	int						w;							/* Largeur de l'‚cran */
	int						h;							/* Hauteur de l'‚cran */
	int						hc;							/* Taille standard des caractŠres de la base line … la topline */
	t_resolution	res;						/* La r‚solution courante, au d‚marrage de l'application */
	int						mode;						/* Le mode courant si on est sur Falcon */
	char					une_palette;		/* FALSE : pas de palette = NEAR TRUE COLOR ou TRUE_COLOR) TRUE : une palette */
	long					n_color_pal;		/* Nombre de couleurs dans la palette ou dans le systŠme */
	long					n_color;				/* Nombre de feutres disponibles simultan‚ment */
	int						n_plane;				/* Profondeur de l'‚cran, en nbre de plans */
	Palette				*palette;				/* Pointeur vers la sauvegarde des feutres */
	int						nb_bit_coul[3];	/* Nombre de bit par couleur */
	int						masque_coul[3];	/* Masque pour les couleurs */
	void					*phys;					/* Adresse physique de l'‚cran */
	void					*log;						/* Adresse logique de l'‚cran */
}	t_display;

typedef struct t_desktop
{
	int xd, yd, wd, hd;
} t_desktop;

extern t_ident_hard_soft desc_hard_soft;
extern t_display work_display;
extern t_desktop bureau;

	/* Bulles d'aide : -1 : bulles toujours actives / 0 : jamais / autre : masque Kbshift */
extern int aide_en_ligne;
/* #] Variables globales :																				*/ 
/* #[ Prototypage des fonctions :																	*/
/****** Fonctions d'initialisation ********************************/
int		initial (char *rsc, int menu, int desk, int nb_tree,
							 OBJECT *rs_tree, char *rs_str, int n_wind, int acc);
void	end (void);

/****** Fonctions de gestion des formulaires **********************/
void	formm_draw (OBJECT *adr, int ed, int flmove, MFDB *img);
void	formm_undraw (OBJECT *adr, MFDB *img);
void	formf_draw (OBJECT *adr, int ed);
void	formf_undraw (OBJECT *adr);
void	create_hierm (OBJECT *adr);
int		dialog (int flags, OBJECT *address, int f,
							int fl_rb, int form_pu, MFDB *img, int fl_move);
int		match (OBJECT *adr, int button);
void	set_popup (OBJECT *adr, int button, int option);
OBJECT *create_popup (int nbre, char *liste);
int		free_popup (int posx, int posy, int form, OBJECT *adr);
void	get_bkgr (int of_x, int of_y, int of_w, int of_h, MFDB *img);
void	put_bkgr (int of_x, int of_y, int of_w, int of_h, MFDB *img);
int		big_alert (int button, int number, char *str, int fl_alrt);
char	*get_string (int number);
void	form_attach (int tree, int obj, char formular);
void	bulle_attach (int tree, int obj, char bulle);
void	set_interface (int under, int cunder, int ctxt_under,
										 int cicn_coche, int ctxt_coche,
										 int cicn_radio, int ctxt_radio,
										 int cicn_popup, int ctxt_popup,
										 int cmove, int cgrastr,
										 int clin_frame, int ctxt_frame, int cniceline);
int cdecl parent (OBJECT *adr, int object);
void	bak_rsc (OBJECT *tree, char **bak);
void	res_rsc (OBJECT *tree, char **bak);
void	dlg_mouse (void);
void set_radio (OBJECT *adr, int rb);
int get_radio (OBJECT *adr, int rb);
void set_attributes (OBJECT *adr, int ob, int attr);

/****** Fonctions pour les ‚ditables ******************************/
void objc_xedit (OBJECT *adr, int car, int ed, int *po, int action, int f, int index);
void	set_text (OBJECT *adr, int object, char *string);
char	*get_text (OBJECT *adr, int object);

/****** Fonctions diverses ****************************************/
char	*path (char *pat);
long	exist (char *name, int att);
void	set_palette (int index);
long	timer (void);
char	*trim (char *str);
void	send_mesag (int type, int wi, int x, int y, int w, int h);
int		selector (char *pat, char *ext, char *file, char *title);
void	extension (char *filename, char *ext);
int		rc_intersect (GRECT *p1, GRECT *p2);
void	clipp (int clip_flag, GRECT *area);
COOKIE *first_cookie (void);				/* Le premier cookie */
COOKIE *next_cookie (COOKIE *ck);		/* Cookie suivant */
COOKIE *vq_cookie (long id);				/* Cherche un cookie par id */
COOKIE *vq_cookie_c (char *c);			/* Cherche un cookie par chaŒne */
void fill_tab (int *pxy, int n, ...);
void empty (int evnmt);

/****** Fonctions pour les fenˆtres *******************************/
int		formw_draw (OBJECT *adr, int index, int title, int infos,
									int wed, int attr, FNCP ricn);
int		open_window (int index, int type, int attr,
									 int x, int y, int w, int h,
									 int wmi, int hmi, int wma, int hma,
									 int wu, int hu,
									 int flags, int fmouse,
									 FNCP ricn, FNCP redr, FNCP clos,
									 char *title, char *info,
									 long wt, long ht,
									 int fmenu, int ftool, int bar,
									 int pattern, int colorp);
int		find_index (int wh);
int		find_window (int mx, int my);
void	zone_work (int index, int *xw, int *yw, int *ww, int *hw);
void	height_sliders (int index, int fredraw);
void	posi_vert (int index, int fredraw);
void	posi_hori (int index, int fredraw);
void	height_vert (int index, int fredraw);
void	height_hori (int index, int fredraw);
void	draw_object (int object, int index);
void	print_page (int index);
void	next_wind (void);
void	closed (void);
void	iconise (int index);
void	uniconise (int index);
void	alliconise (int index);
void	set_work (int index, int pattern, int colorp);
void	set_windows (int mtxt_mover, int ctxt_mover, int spat_mover, int cpat_mover,
									 int ctxt_info,
									 int spat_velev, int cpat_velev, int spat_helev, int cpat_helev,
									 int spat_vslid, int cpat_vslid, int spat_hslid, int cpat_hslid,
									 int cpat_gadgt, int cicn_gadgt);
void bwind_calc (int request, int attr,
								 int tree, OBJECT *adrb,
								 int xd, int yd, int wd, int hd,
								 int *x, int *y, int *w, int *h);
void draw_title (int index);
/* #] Prototypage des fonctions :																	*/ 
#endif

