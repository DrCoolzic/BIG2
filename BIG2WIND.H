/*	BIG2WIND.H */

/* #[ Variables globales : */
extern MOBLK mouse;
extern int ap_id;
extern int event;
extern int buf[];
extern int mk;
extern t_display work_display;
extern t_desktop bureau;
extern OBJECT *scroll;
extern OBJECT **ir_trindex;
extern FNCP *fnc;
extern BigWind	*win;

#include "BIG2INCL.H"
extern char *bi_frstr[];
extern OBJECT *bi_trindex[];
/* #] Variables globales : */ 
/* #[ Prototypage des fonctions : */
void	dcl_windows (int nbre);
void	create_wmenu (int tree, int index);
int		menu_window (int index, int xm, int ym);
int		wind_hierarch (OBJECT *adr, int index, int option);
void	wind_mouse (void);
void	print_line (int f, int index);
void	print_colum (int f, int index);
void	print_wind (int index);
/* #] Prototypage des fonctions : */ 

