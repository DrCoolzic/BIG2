/*	BIG2USR1.H */

	/* Nombre de gadgets USERDEFs dans les fenˆtres BIG : */
#define NU 12
	/* Nombre total de USERDEFs pour les fenˆtres BIG : */
#define NTU NU + 8
	/* Nombre d'objets USERDEFs dans les autres formulaires */
#define NFU 4
/* #[ Variables globales : */
extern t_display work_display;
extern t_desktop bureau;
extern OBJECT *adr_menu;
extern USERBLK *adr_usr;
extern char *xedit;
extern t_image_dial *xpict;
extern int nb_pict;
extern OBJECT **ir_trindex;
extern char **ir_frstr;
extern FNCP *fnc;

#include "BIG2INCL.H"
extern OBJECT *bi_trindex[];

typedef struct obj_user
{
	int		under;				/* Soulignement oui / non */
	int		cunder;				/* Couleur caractŠre soulign‚ */
	int		ctxt_under;		/* Couleur texte B_UNDER */
	int		cicn_coche;		/* Couleur ic“ne B_COCHE */
	int		ctxt_coche;		/* Couleur texte B_COCHE */
	int		cicn_radio;		/* Couleur ic“ne B_RADIO */
	int		ctxt_radio;		/* Couleur texte B_RADIO */
	int		cicn_popup;		/* Couleur ic“ne B_POPUP / B_LIST */
	int		ctxt_popup;		/* Couleur texte B_POPUP / B_LIST */
	int		cmove;				/* Couleur B_MOVE */
	int		cgrastr;			/* Couleur texte B_GRASTR */
	int		clin_frame;		/* Couleur lignes B_FRAME */
	int		ctxt_frame;		/* Couleur texte B_FRAME */
	int		cniceline;		/* Couleur B_NICELINE */
} Obj_user;
/* #] Variables globales : */ 
/* #[ Prototypage des fonctions : */
void	userdef (char *rsc, int *nb_form, int nb_tree);
int		cdecl bcoche (PARMBLK *para);
int		cdecl brond (PARMBLK *para);
int		cdecl bunder (PARMBLK *para);
int		cdecl bcadre (PARMBLK *para);
int		cdecl bmove (PARMBLK *para);
int		cdecl bgrastr (PARMBLK *para);
int		cdecl bsmstr (PARMBLK *para);
int		cdecl bpopup (PARMBLK *para);
int		cdecl bpict (PARMBLK *para);
int		cdecl bmhier (PARMBLK *para);
int		cdecl bniceline (PARMBLK *para);
/* #] Prototypage des fonctions : */ 

