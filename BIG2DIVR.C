/*! @file BIG2DIVR.C
@brief implementation of miscellaneous functions 

@verbatim
BIG = "BIG Is GEM" - A high level GEM library. 
Initial Development by Claude ATTARD, Maintenance by Jean LOUIS-GUERIN
Copyright (c) 1993-2014 Claude ATTARD
Copyright (c) 2010-2014 Jean LOUIS-GUERIN

website: http://info-coach.fr/atari/software/projects/big.php
forum:   http://www.atari-forum.com/viewtopic.php?f=16&t=27060

The BIG library may be used and distributed without restriction provided that 
this copyright statement is not removed from the file and that any derivative 
work contains the original copyright notice and the associated disclaimer.

The BIG library  is free software; you can redistribute it and/or modify  it 
under the terms of the GNU General Public License as published by the Free 
Software Foundation; either version 3 of the License, or (at your option) any 
later version.

The BIG library is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
HxCFloppyEmulator; if not, write to the Free Software Foundation, Inc., 51 
Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
@endverbatim

*/


#include "BIG2.H"
#include "BIG2DIVR.H"


/* #[ path () looks for and return the application path */
char *path (char *pat) {
	int drive;

	drive = Dgetdrv ();							/* Lecteur courant */
	pat[0] = (char) (65 + drive);
	pat[1] = ':';
	Dgetpath (&pat[2], drive + 1);	/* Chemin lecteur courant */
	strcat (pat, "\\");
	return pat;
}
/* #] path () looks for and return the application path */ 

/* #[ exist () tests if file exist */
long exist (char *name, int att) {
	int err, o, l;
	long length;
	char old[128];

	if (NOT (att & FA_SUBDIR))						/* Si on cherche un fichier */
	{
		err = (int)Fopen (name, ZERO);			/* On tente de l'ouvrir */
		if (err >= ZERO)										/* Si c'est bon */
		{
			length = Fseek (ZERO, err, 2);		/* On lit sa taille */
			Fclose (err);											/* On le ferme */
			return length;										/* On retourne la longueur */
		}
		else																/* Si c'est pas bon */
			return FALSE;
	}
	else																	/* Si on cherche un dossier */
	{
		o = Dgetdrv ();											/* Noter lecteur actuel */
		Dgetpath (old, ZERO);								/* Noter chemin actuel */
		l = (int)name[0] - 'A';							/* Lecteur voulu */
		err = (int)Dsetdrv (l);							/* On tente d'imposer ce lecteur */
		if (err >= ZERO)										/* Si c'est bon */
		{
			err = Dsetpath (name + 2);				/* On tente d'imposer ce chemin */
			if (err >= ZERO)
			{
				Dsetdrv (o);										/* Restaurer ancien lecteur */
				Dsetpath (old);									/* Restaurer ancien chemin */
				return TRUE;
			}
			else
				Dsetdrv (o);										/* Restaurer ancien lecteur */
		}
		return FALSE;
	}
}
/* #] exist () tests if file exist */ 

/* #[ set_palette () Impose une palette de couleurs :							*/
void set_palette (int index)
{
register int i;
int rgb[3];
Palette *pal;

	if (index > BLANK)
	{
		if ((win[index].w_icon == BLANK) && (win[index].w_pal))
		{
			pal = (Palette *) win[index].w_pal;
			for (i = ZERO ; i < work_display.n_color ; i++)
			{
				rgb[0] = pal[i][0];
				rgb[1] = pal[i][1];
				rgb[2] = pal[i][2];
				vs_color (work_display.handle, i, rgb);
			}
			pal_img = TRUE;
		}
	}
	else
	{
		for (i = ZERO ; i < work_display.n_color ; i++)
		{
			rgb[0] = bpal[i][0];
			rgb[1] = bpal[i][1];
			rgb[2] = bpal[i][2];
			vs_color (work_display.handle, i, rgb);
		}
		pal_img = FALSE;
	}
}
/* #] set_palette () Impose une palette de couleurs :							*/ 
/* #[ timer () Lit le timer 200 Hz :															*/
long timer (void)
{
long save_ssp;
long time;

	save_ssp = Super (0L);			/* Passer en mode superviseur */
	time = *(long *)0x4BA;			/* Prendre le timer en passant par son pointeur */
	Super ((void *)save_ssp);		/* Revenir en mode utilisateur */

	return time;								/* Retourner la valeur lue */
}
/* #] timer () Lit le timer 200 Hz :															*/ 
/* #[ stdkey () Test tous claviers :															*/
void stdkey (unsigned char *k)
{
KEYTAB *kt;
	if ((char)(key >> 8) != ZERO)
	{
		kt = Keytbl((void *)BLANK, (void *)BLANK, (void *)BLANK);
		*k = toupper (kt->shift[(char)(key >> 8)]);
	}
	else
		*k = (char)(key);
}
/* #] stdkey () Test tous claviers :															*/ 
/* #[ trim () Vire espaces d‚but et fin de chaŒne :								*/
char *trim (char *str)
{
register char	*s;
register int i = ZERO;

	while ((*(str + i) == ' ') || (*(str + i) == '\t'))
		i++;
	strcpy (str, (str + i));

	s = str + strlen (str) - 1;
	for( ; ((*s == ' ') || (*s == '\t')) && (s >= str) ; *s-- = ZERO);
	return str;
}
/* #] trim () Vire espaces d‚but et fin de chaŒne :								*/ 
/* #[ rc_intersect () Intersection :															*/
int rc_intersect (GRECT *p1, GRECT *p2)
	/* Calcule l'intersection de 2 rectangles */
{
int tx, ty, tw, th;

	tw = min(p2->g_x + p2->g_w, p1->g_x + p1->g_w);
	th = min(p2->g_y + p2->g_h, p1->g_y + p1->g_h);
	tx = max(p2->g_x, p1->g_x);
	ty = max(p2->g_y, p1->g_y);
	p2->g_x = tx;
	p2->g_y = ty;
	p2->g_w = tw - tx;
	p2->g_h = th - ty;
	return ((tw > tx) && (th > ty));
}
/* #] rc_intersect () Intersection :															*/ 
/* #[ clipp () Clipping :																					*/
void clipp (int clip_flag, GRECT *area)
{
int pxy[4];

	fill_tab (pxy, 4,
						area->g_x, area->g_y,
						area->g_w + area->g_x - 1, area->g_h + area->g_y - 1);
	vs_clip (work_display.handle, clip_flag, pxy);
}
/* #] clipp () Clipping :																					*/ 
/* #[ send_mesag () Envoi de message … soi-mˆme :									*/
void send_mesag (int type, int wi, int x, int y, int w, int h)
{
	fill_tab (buf, 8,
						type, ap_id,	/* Num‚ro du message et notre indentificateur (l'exp‚diteur du message) */
						ZERO, wi, x, y, w, h);
	appl_write (ap_id, 16, buf);	/* Envoi du message */
}
/* #] send_mesag () Envoi de message … soi-mˆme :									*/ 
/* #[ selector () S‚lecteur : 																		*/
int selector (char *pat, char *ext, char *file, char *title)
{
char ch[120], fi[13];
int retour, i;

	strcpy (ch, pat);
	strcat (ch, ext); 						/* Chemin et s‚lection */
	strcpy (fi, file);
	if ((Sversion () >> 8) <= 20)
		fsel_input (ch, fi, &retour); 					/* Appel du s‚lecteur */
	else
		fsel_exinput (ch, fi, &retour, title);	/* Appel du s‚lecteur */
	if (retour)										/* Si pas ANNULER... */
	{
		for (i = (int)strlen (ch) ; ch[i] != '\\' ; ch[i--] = '\0') ;
		strcpy (pat, ch);
		strcpy (file, fi);
	}
	return retour;
}
/* #] selector () S‚lecteur : 																		*/ 
/* #[ extension () Ajuste extension :															*/
void extension (char *filename, char *ext)
{ 							/* L'EXTENSION DOIT ETRE TRANSMISE AVEC LE POINT */
int t;

	t = (int)strlen (filename);								/* Longueur du nom de fichier */
	while (filename[t] != '.' && t > ZERO) 	/* Chercher le '.' … partir de la droite */
		t--;
	if (t > ZERO)										/* Si trouv‚, */
	{
		memcpy (filename + t, ext, 4); 			/* ajouter l'extention … partir de l… */
		filename[t + 4] = '\0';
	}
	else														/* Sinon, */
		strcat (filename, ext);							/* l'ajouter … la fin */
}
/* #] extension () Ajuste extension :															*/ 
/* #[ first_cookie () :																						*/
COOKIE *first_cookie (void)
{
COOKIE *cook;
long save_ssp;
	
	/* MA_30_10. Si on est en superviseur on ne doit pas repasser en User */
	if (Super((void *)1L) == 0L)	/* si on est en mode utilisateur */
		save_ssp = Super (ZERO);
	else
		save_ssp = ZERO;
	cook = *(COOKIE **) 0x5a0;		/* Le cookie jar */

	/* MA_30_10. Si on etait en en superviseur on ne doit pas repasser en User	*/	
	if (save_ssp) 
		Super ((void *)save_ssp);
	if (!cook)										/* Pas de cookie jar */
		return (COOKIE *) ZERO;
	return cook;
}
/* #] first_cookie () :																						*/ 
/* #[ next_cookie () :																						*/
COOKIE *next_cookie (COOKIE *ck)
{
	if (!ck->ident)						/* C'‚tait le dernier cookie */
		return ZERO;
	return ++ck;
}
/* #] next_cookie () :																						*/ 
/* #[ vq_cookie () :																							*/
COOKIE *vq_cookie (long id)
{
COOKIE *cook;
	
	cook = first_cookie ();
	while (cook)
	{
		if (cook->ident == id)			/* Trouv‚ ! */
			return cook;
		cook = next_cookie (cook);
	}
	return (COOKIE *)ZERO;				/* Pas trouv‚ */
}
/* #] vq_cookie () :																							*/ 
/* #[ vq_cookie_c () :																						*/
COOKIE *vq_cookie_c (char *c)
{
/* MA_30_10: Recherche d'un cookie avec une chaine, comme il se doit */
long l = ZERO;

	l = ((long)c[0] << 24);
	l |= ((long)c[1] << 16);
	l |= ((long)c[2] << 8);
	l |= (long)c[3];
	return (vq_cookie (l));
}
/* #] vq_cookie_c () :																						*/ 
/* #[ fill_tab () Remplissage d'un tableau :											*/
void fill_tab (int *pxy, int n, ...)
{
int i, val;
va_list pa;

	va_start (pa, n);
	for (i = ZERO ; i < n ; i++)
	{
		val = va_arg (pa, int);
		*(pxy + i) = val;
	}
	va_end (pa);
}
/* #] fill_tab () Remplissage d'un tableau :											*/ 
/* #[ empty () Attente vider buffer clavier ou message :					*/
void empty (int evnmt)
{
int quit = FALSE, ev, dummy;

	if ((evnmt == MU_MESAG) || (evnmt == MU_KEYBD))
	{
		do
		{
			ev = evnt_multi ((evnmt | MU_TIMER),
											 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
											 buf, 10, 0, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy);
			if (ev == MU_TIMER)
				quit = TRUE;
		} while (! quit);
	}
}
/* #] empty () Attente vider buffer clavier ou message :					*/ 

