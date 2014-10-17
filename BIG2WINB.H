/*	BIG2WINB.H */

extern OBJECT *bi_trindex[];

/* #[ Prototypage des fonctions : */
void put_fen (int index);
void no_top (int obj, int index);
int asce (int obj, int index);
void arrows (int obj, int index);
void slid (int obj, int index);
void sizebw (int obj, int index);
void movebw (int index);
void resize (int index, int deltaw, int deltah);
void draw_obj (OBJECT *adr, int obj);
void draw_wind (int index, GRECT *r);
/* #] Prototypage des fonctions : */ 

