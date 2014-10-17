/*	BIG2FOR1.H */

/* #[ Variables globales : */
extern int ap_id;
extern int buf[];
extern int edit;
extern int pos;
extern int object;
extern t_display work_display;
extern t_desktop bureau;
extern OBJECT *adr_menu;
extern OBJECT *scroll;
extern MFDB screen;
extern OBJECT **ir_trindex;
extern FNCP *fnc;

#include "BIG2INCL.H"
extern OBJECT *bi_trindex[];
/* #] Variables globales : */ 
/* #[ Prototypage des fonctions : */
int		menu_hierarch (int option);
void	pop_up (OBJECT *adr, int object);
void	pop_liste (OBJECT *adr, int object);
void	next_option (OBJECT *adr, int object, int form);
int		create_scroll (OBJECT *pu, int y, int *nb, int first);
/* #] Prototypage des fonctions : */ 

