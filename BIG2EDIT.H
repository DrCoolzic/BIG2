/*	BIG2EDIT.H */

/* #[ Variables globales : */
extern int edit;
extern int object;
extern int kbd;
extern int key;
extern t_display work_display;
extern char *buffer;
/* #] Variables globales : */ 
/* #[ Prototypage des fonctions : */
int		exist_edit (OBJECT *adr);
void	empty_edit (OBJECT *adr);
int		pos_curs (OBJECT *adr, int ed, int mx, int f, int index, int po);
int		prev (OBJECT *adr, int ob);
int		next (OBJECT *adr, int ob);
void	first (OBJECT *adr, int *editable, int *position);
void	last (OBJECT *adr, int *editable, int *position);
int		next_word (OBJECT *adr, int ob, int posi);
int		prev_word (OBJECT *adr, int ob, int posi);
int		valide (char *ascii, char *valid, int p);
char	*start_edit (char *pt);
void	draw_curs (OBJECT *adr, int ed, int *po, int action);
void	copy (OBJECT *adr, int ed);
void	cut (OBJECT *adr, int ed);
void	paste (OBJECT *adr, int ed);
/* #] Prototypage des fonctions : */ 

