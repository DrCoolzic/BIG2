/*	BIG2FOR2.H */

/* #[ Variables globales : */
extern int event;
extern int buf[];
extern int mx;
extern int my;
extern int edit;
extern int pos;
extern int object;
extern int kbd;
extern int key;
extern int clik;
extern t_display work_display;
extern t_desktop bureau;
extern OBJECT *adr_menu;
extern MFDB screen;
extern OBJECT **ir_trindex;
extern char **ir_frstr;
extern BigWind	*win;

#include "BIG2INCL.H"
extern OBJECT *bi_trindex[];
/* #] Variables globales : */ 
/* #[ Prototypage des fonctions : */
void	help (OBJECT *adr, int obj);
void	move_form (OBJECT *adr, int x, int y, int w, int h, MFDB *img, int fl_depl);
int		scan (OBJECT *adr, int f);
int		scan_wmenu (int index, char touc);
int		scan_wtool (int index, char touc);
int		scan_menu (char touc);
int		scan_form (char touc, OBJECT *adr);
int		find_touche (char *chaine, int *position);
void	find_special (char *chaine, int *spe, int j);
int		m_title (OBJECT *adresse, int option);
/* #] Prototypage des fonctions : */ 

