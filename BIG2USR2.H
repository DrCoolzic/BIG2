/*	BIG2USR2.H */

/* #[ Variables globales : */
extern t_display work_display;
extern int id_top;

#include "BIG2INCL.H"
extern OBJECT *bi_trindex[];

typedef struct win_user
{
	int		mtxt_mover;		/* Mode texte MOVER */
	int		ctxt_mover;		/* Couleur texte MOVER */
	int		spat_mover;		/* Style trame MOVER */
	int		cpat_mover;		/* Couleur trame MOVER */
	int		ctxt_info;		/* Couleur texte INFO */
	int		spat_velev;		/* Style trame VELEV */
	int		cpat_velev;		/* Couleur trame VELEV */
	int		spat_helev;		/* Style trame HELEV */
	int		cpat_helev;		/* Couleur trame HELEV */
	int		spat_vslid;		/* Style trame VSLIDE */
	int		cpat_vslid;		/* Couleur trame VSLIDE */
	int		spat_hslid;		/* Style trame HSLIDE */
	int		cpat_hslid;		/* Couleur trame HSLIDE */
	int		cpat_gadgt;		/* Couleur fond gadgets */
	int		cicn_gadgt;		/* Couleur ic“ne gadgets */
} Win_user;
/* #] Variables globales : */ 
/* #[ Prototypage des fonctions : */
int		cdecl fond (PARMBLK *para);
int		cdecl gadget (PARMBLK *para);
int		cdecl titinf (PARMBLK *para);
int		cdecl elev (PARMBLK *para);
int		cdecl sld (PARMBLK *para);
int		cdecl work (PARMBLK *para);
/* #] Prototypage des fonctions : */ 

