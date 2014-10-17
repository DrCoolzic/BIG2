/*	BIG2EVTW.H */

/* #[ Variables globales : */
extern int buf[];
extern t_ident_hard_soft desc_hard_soft;
extern t_display work_display;
extern t_desktop bureau;
extern BigWind	*win;
/* #] Variables globales : */ 
/* #[ Prototypage des fonctions : */
void wm_arrow (void);
void	redraw (void);
void	hslid (void);
void	vslid (void);
void	uppage (void);
void	dnpage (void);
void	lfpage (void);
void	rtpage (void);
void	upline (void);
void	dnline (void);
void	lfline (void);
void	rtline (void);
void	topped (void);
void	fulled (void);
void	sized (void);
void	moved (void);
void	full_wform (void);
void	size_wform (void);
/* #] Prototypage des fonctions : */ 

