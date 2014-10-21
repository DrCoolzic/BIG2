/*! @file BIG2INIT.C
@brief implementation of functions to initialize BIG

@verbatim
BIG = "BIG Is GEM" - A high level GEM library. 
Initial Development by Claude ATTARD, Maintenance by Jean LOUIS-GUERIN
Copyright (c) 1993-2014 Claude ATTARD
Copyright (c) 2010-2014 Jean LOUIS-GUERIN

website: http://info-coach.fr/atari/software/system/big.php
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

#include "big2.h"
#include "BIG2USR1.H"
#include "BIG2USR2.H"
#include "BIG2EDIT.H"
#include "BIG2WIND.H"
#include <STRING.H>
#include <AES.H>
#include <VDI.H>
#include <TOS.H>
#include <STDIO.H>
#include <STDLIB.H>
#include <CTYPE.H>
#include "BIG2INCL.HBG"

/****** Variables globales ****************************************/

int ap_id;							/* Num‚ro de l'application */
int event;							/* Ev‚nement courant */
int buf[8];							/* Buffer d'‚vŠnements */
int mx, my, mk;					/* Position et ‚tat souris */
int edit, pos, object;	/* Editable courant, position curseur, objet cliqu‚ */
int kbd, key, clik;			/* Etat touches sp‚ciales, touches, clics */
int wind;								/* Fenˆtre cliqu‚e */
int	intgr;							/* Flag de ressource int‚gr‚ ou non */
int pal_img = ZERO;			/* Flag de palette non standard */
int old_top = BLANK;
int id_top;

char pathapp[128];			/* Chemin de l'application */

t_ident_hard_soft desc_hard_soft;
t_display work_display;
t_desktop bureau;
int cliq_in;	/* MA 16/07/94 : pour savoir si le clic de fermeture
								 d'un menu liste, popup ou hi‚rarchique
								 s'est deroule dans ou dehors le menu	*/

MOBLK mouse = {ZERO, ZERO, ZERO, ZERO, ZERO};	/* Zone d'‚v‚nement zone souris */

OBJECT *adr_menu, *adr_desk;	/* Adresses formulaires r‚serv‚es */
USERBLK *adr_usr;							/* Adresse Malloc structures USERBLK */
char *xedit;									/* Ptr sur les chaŒnes d'‚ditables ‚tendus */
t_image_dial *xpict;					/* MA 26/06/94 pointeurs sur les zones d'info des images */
int nb_pict = ZERO;						/* MA 26/06/94 */

OBJECT **objets_maison = NULL;	/* MA 14/07/94 */
int nb_objets_maison = ZERO;

Palette	bpal[16];				/* Palette interne au Prg : 16 couleurs */
char *buffer = ZERO;		/* Ptr buffer Copier/Couper/Coller */
OBJECT *scroll;					/* Adresse scroll_menu courant */

MFDB screen;						/* Pour les Copies Rasters */

OBJECT **ir_trindex;		/* Pour les arbres de ressource int‚gr‚ */
char **ir_frstr;				/* Pour les chaŒnes libres ressource int‚gr‚ */

	/* Cr‚ation des types "pointeur sur fonction" */
FNCP *fnc;	/* Pointeur sur les pointeurs de fonction li‚s aux objets */

BigWind	*win;				/* Pointeur sur la zone des fenˆtres */
/* #] D‚finitions :																								*/ 
/* #[ Prototypage des fonctions : */
void	vq_screen (t_display *Xdisplay);
void	get_palette (void);
void	open_work (void);
int		identifier_tos (void);
t_ident_hard_soft	identifier_hard_soft (void);
/* #] Prototypage des fonctions : */ 



int		initial (char *rsc, int menu, int desk, int nb_tree, OBJECT *rs_tree, 
				 char *rs_str, int n_wind, int acc) {
int nb_form, i, j, k, d, dummy, ecart[5], effets[3], accok;
unsigned long map, bit;
char alrt[MAX_ALRT], gohome[255], prgname[MAX_LEN], pat[128], *tacc;
OBJECT *adr;

	path (pathapp);
	strcpy (pat, pathapp);
	strcat (pat, rsc);
	if (_app)
	{
		if (vq_aes () == BLANK)
		{
			strcpy (prgname, rsc);
			extension (prgname, ".PRG");
			sprintf (gohome, bi_frstr[EXITAUTO], prgname);
			Cconws (gohome);
			Bconin (2);
			exit (1);
		}
	}
	adr_usr = (USERBLK *)ZERO;
	fnc = (FNCP *)ZERO;
	win = (BigWind *)ZERO;
	screen.fd_addr = NULL;
	scroll = (OBJECT *)ZERO;

	if (nb_tree)
	{
		ir_trindex = (OBJECT **)rs_tree;
		ir_frstr = (char **)rs_str;
	}
	else
	{
		ir_trindex = (OBJECT **)ZERO;
		ir_frstr = (char **)ZERO;
	}

	ap_id = appl_init (); /* D‚clarer l'application */

	if (NOT nb_tree)
		intgr = FALSE;
	else
		intgr = TRUE;

	if ((NOT _app) && (nb_tree))	/* Accessoire et ressource int‚gr‚ */
	{
		adr_desk = adr_menu = 0L;
		accok = menu_register (ap_id, get_string (acc));
		if (accok < ZERO)		/* Si plus d'accessoires */
			while (TRUE) ;		/* Boucle sans fin. */
	}

	/* Identification de la config HARD et SOFT	*/
	desc_hard_soft = identifier_hard_soft ();

	if (NOT nb_tree)
	{
		if (rsrc_load (pat) == ZERO)			/* Charger RSC */
		{
			sprintf (alrt, bi_frstr[BIGNOT_RSC], rsc, rsc);
			form_alert (1, alrt);
			if (_app)					/* Si programme */
			{
				appl_exit (); 			/* Larguer l'AES */
				exit (-33);
			}
			else							/* Si accessoire */
			{
				while (TRUE)				/* Boucle sans fin */
					evnt_mesag (buf);
			}
			return FALSE;
		}
	}
	else		/* Si ressource int‚gr‚ */
	{
		for (i = ZERO ; i < nb_tree ; i++) 	/* Pour chaque arbre du RSC int‚gr‚, */
		{
			j = ZERO;										/* et chaque objet, */
			do
			{			/* Tant qu'on n'est pas au bout */
				rsrc_obfix (ir_trindex[i], j);		/* Adapter coordonn‚es */
			} while (NOT (ir_trindex[i][j++].ob_flags & LASTOB));
		}
	}

	if ((NOT _app) && (NOT nb_tree))	/* Accessoire et ressource externe */
	{
		adr_desk = adr_menu = 0L;
		rsrc_gaddr (R_FRSTR, acc, &tacc);	/* Adr chaŒne pointeur sur pointeur */
		tacc = *((char **)tacc);							/* Pointeur sur le texte */
		accok = menu_register (ap_id, tacc);
		if (accok < ZERO)		/* Si plus d'accessoires */
			while (TRUE) ;		/* Boucle sans fin. */
	}

	open_work (); 				/* Ouverture station et initialisations par d‚faut */

	/* R‚cuperation de l'ensemble des infos sur les caract‚ristiques de l'‚cran */
	vq_screen (&work_display);

	work_display.palette = (Palette *)malloc (work_display.n_color * sizeof (Palette));
	get_palette ();
		
	vqt_fontinfo (work_display.handle, &dummy, &dummy, ecart, &dummy, effets);

	/* Ecriture de la taille des fontes dans la variable work_display */
	work_display.hc = ecart[4];

	for (i = ZERO ; i < 10 ; i++) 	/* Pour chaque arbre du RSC inclus, */
	{
		j = ZERO;										/* et chaque objet, */
		do
		{			/* Tant qu'on n'est pas au bout */
			rsrc_obfix (bi_trindex[i], j);		/* Adapter coordonn‚es */
		} while (NOT (bi_trindex[i][j++].ob_flags & LASTOB));
	}

	userdef (pat, &nb_form, nb_tree);		/* Traite USERDEFs et PFNTs */

	for (i = ZERO ; i < nb_form ; i++)	/* Centrer tous les formulaires */
	{
		adr = ir_trindex[i];

		if ((adr[3].ob_type & 0xff) != G_TITLE)	/* Si ce n'est pas un menu */
		{
			form_center (adr, &adr->ob_x, &adr->ob_y, &dummy, &dummy);
			empty_edit (adr);		/* Vider tous les EDITABLEs */
		}
	}
		/* Pr‚parer S‚lecteur BIG2 */
	map = Drvmap ();														/* Lecteurs dispos */
	k = 2;																			/* A et B : toujours */
	for (bit = 4 ; bit <= 65536L ; bit *= 2)		/* Pour chaque bit */
	{
		if (map & bit)														/* Si lecteur */
			bi_trindex[SELF][k + FS_LA].ob_state &= ~DISABLED;		/* Activer le bouton */
		k++;																			/* Bouton suivant */
	}
	form_center (bi_trindex[SELF], &d, &d, &d, &d);

	if (n_wind)							/* S'il y a des fenˆtres */
		dcl_windows (n_wind);	/* D‚clarer les fenˆtres */

	wind_get (ZERO, WF_WORKXYWH, &bureau.xd, &bureau.yd, &bureau.wd, &bureau.hd); /* Dimensions bureau */
	if (_app)				/* Si pas accessoire */
	{
		adr_desk = adr_menu = 0L;
		if (desk > BLANK)			/* Si un bureau est demand‚ */
		{
			adr_desk = ir_trindex[desk];
			adr_desk->ob_x = bureau.xd;					/* Fixe */
			adr_desk->ob_y = bureau.yd;					/* position */
			adr_desk->ob_width = bureau.wd;			/* et dimensions */
			adr_desk->ob_height = bureau.hd; 		/* du bureau */
			wind_set (ZERO, WF_NEWDESK, adr_desk, ZERO); 						/* Impose bureau */
			form_dial (FMD_FINISH, ZERO, ZERO, ZERO, ZERO, bureau.xd, bureau.yd, bureau.wd, bureau.hd);	/* L'affiche */
		}
		if (menu > BLANK)			/* Si un menu est demand‚ */
		{
			adr_menu = ir_trindex[menu];
			create_hierm (adr_menu);
			menu_bar (adr_menu, TRUE);								/* Incruster le menu */
		}
	}

	graf_mouse (ARROW, ZERO);				/* Souris -> flˆche */

	edit = BLANK;					/* Aucun champ ‚ditable */
	aide_en_ligne = ZERO;	/* Pas de bulles d'aide */
	id_top = BLANK;				/* Pas de fenˆtre de 1ø plan */
	return TRUE;
}
/* #] initial () Charge et initialise le ressource :							*/ 
/* #[ vq_screen () Caract‚ristiques de la station graphique :			*/
void vq_screen (t_display *Xdisplay)
{
int work_out[57];
size_t screen_size;
int nb_bit_coul;	/* et par couleur				*/
int restant;
int i;

	/* r‚cuperation d'information supplementaires	*/
	vq_extnd (Xdisplay->handle, TRUE, work_out);
	/* test de la pr‚sence ou non d'une palette	*/
	Xdisplay->une_palette = work_out[5];
	/* r‚cuperation du nombre de plans de l'‚cran	*/
	Xdisplay->n_plane = work_out[4];

	/* r‚cuperation du mode graphique courant			*/
	/* -------------------------------------------------*/
	/* si on est sur un Falcon 030 */
	if (desc_hard_soft.la_machine == FALCON_030)
	{
/* Si on a un processeur graphique de falcon 030, et que on
	 n'est pas avec une carte graphique	true color 24 bits (ou +)
	 mais dans l'un des modes d'origine du Falcon */
		if ((desc_hard_soft.proc_graphique ==	PG_FALCON_030) && (Xdisplay->n_plane <= 16))
		{
			Xdisplay->mode = Vsetmode (-1);			/* R‚cuperer le mode graphique d'origine du Falcon */
			if (Xdisplay->n_color_pal == ZERO)	/* Si on dans un mode falcon avec palette ‚tendue */
			{
				Xdisplay->n_color_pal = 262144L;	/* ca, c'est la taille de la palette du Falcon
																						 je n'ai trouve aucun moyen de le lire dans
																						 une variable systŠme ou VDI quelconque et je
																						 me vois donc contraint de la mettre en dur. */
			}
			/* Pour avoir la taille de l'ecran , rien de plus simple */
			Xdisplay->taille = VgetSize (Xdisplay->mode);
		}
		else
		{	/* Cas du Falcon avec carte graphique true color */
			Xdisplay->res = -1;
			Xdisplay->taille = (unsigned long)Xdisplay->w * Xdisplay->h * Xdisplay->n_plane / 8;
		}
		/* Trouver les r‚solutions ‚quivalentes en ST/TT */
		Xdisplay->res = INCONNU;
		switch (Xdisplay->h)
		{
		case 480 :
			switch (Xdisplay->w)
			{
			case 640 :
				if (Xdisplay->n_color == 256)
					Xdisplay->res = VGA_256COL;
				else
				{ 
					if (Xdisplay->n_color == 16)
						Xdisplay->res = VGA_16COL;
				}						
				break;
			case 320 :
				if (Xdisplay->n_color == 256)
					Xdisplay->res = R_256C_320_480;
				else
				{
					if (Xdisplay->n_color == 262144L)
						Xdisplay->res = TRUE_COLOR_320_480;
				}
				break;
			}	/* Fin du switch res horizontale */
		case 400 :
			if ((Xdisplay->w == 640) && (Xdisplay->n_color == 2))
				Xdisplay->res = ST_HAUTE;
			break;
		case 200 :
			switch (Xdisplay->w)
			{
			case 640 :
				if (Xdisplay->n_color == 4)
					Xdisplay->res = ST_MOYENNE;
				break;
			case 320 :
				if (Xdisplay->n_color == 16)
					Xdisplay->res=ST_BASSE;
				break;
			}
				break;
		}	/* Fin du switch res verticale */
	}		/* Fin du cas Falcon */
	else
	{
		/* Calcul de la taille de l'ecran en octet */
		Xdisplay->taille = (unsigned long)Xdisplay->w * Xdisplay->h * Xdisplay->n_plane / 8;

		/* Sinon, si on est sur une machine connue a ce jour (ST,STE,TT) */
		if (desc_hard_soft.la_machine != AUTRE_MACHINE)
		{
			if (Xdisplay->une_palette)		/* Si on est dans un mode avec palette */
				Xdisplay->res = Getrez ();	/* on r‚cupere la r‚solution */
			else										/* Sinon, si pas de palette on est sur carte  graphique */
				Xdisplay->res = -1;		/* et on l'indique par un -1 indiquant */
															/* que l'on n'est pas dans un mode graphique connu */
		}		
		else	/* sinon on met un -1 dans le mode pour indiquer le pbm */
			Xdisplay->res = -1;
	}
	
	screen_size = Xdisplay->taille;
	Xdisplay->taille_ligne = screen_size / Xdisplay->h;
	
	if (NOT Xdisplay->une_palette)	/* si il n'y pas de palette */
	{						/* le nombre de couleur depend du nombre de plans de bits */
		/* si le nombre de bit plane est superieur a 24, */
		/* cela signifie que les bits en trop traitent la transparence */
		/* ils n'interviennent donc pas sur le codage de couleur */
		if (Xdisplay->n_plane > 24)
			Xdisplay->n_plane = 24;

		Xdisplay->n_color_pal = 1 << Xdisplay->n_plane;	/* le nb de couleur est 2 puissance le nombre de plans */
	}
	else
		Xdisplay->n_color_pal = Xdisplay->n_color;
	/* on calcule le nombre de bit pour chaque couleur */
	nb_bit_coul = Xdisplay->n_plane / 3;
	restant = Xdisplay->n_plane - 3 * nb_bit_coul;	/* nombre de bit restant */
	Xdisplay->nb_bit_coul[0] = nb_bit_coul;
	Xdisplay->nb_bit_coul[1] = nb_bit_coul + restant;	/* les bits restants vont au vert	*/
	Xdisplay->nb_bit_coul[2] = nb_bit_coul;
	
	for (i = ZERO ; i < 3 ; i++)
	{			/* valeur maximale de la couleur vu le nombre de bits */
		Xdisplay->masque_coul[i] = (1 << Xdisplay->nb_bit_coul[i]) - 1;
	}

	Xdisplay->phys = Physbase ();
	Xdisplay->log = Logbase ();
	Xdisplay->palette = NULL;		/* Pour l'instant, les pinceaux ne sont pas sauvegard‚s */
}
/* #] vq_screen () Caract‚ristiques de la station graphique :			*/ 
/* #[ get_palette () Sauve la palette courante :									*/
void get_palette (void)
{
register int i;

	for (i = ZERO ; i < work_display.n_color ; i++)
	{
		/* Utilistion de la structure work_display et optimisation */
		vq_color (work_display.handle, i, 1, (int *)&work_display.palette[i]);
	}
	for (i = ZERO ; i < 16 ; i++)
	{			/* Copie les 16 premiŠres couleurs */
		bpal[i][0] = work_display.palette[i][0];
		bpal[i][1] = work_display.palette[i][1];
		bpal[i][2] = work_display.palette[i][2];
	}
}
/* #] get_palette () Sauve la palette courante :									*/ 
/* #[ open_work () Ouvrir station travail :												*/
void open_work (void)
{
int i, a, b;

	for (i = ZERO ; i < 10 ; _VDIParBlk.intin[i++] = 1);
	_VDIParBlk.intin[10] = 2;

	_VDIParBlk.intin[0] = Getrez ()+2;	/* Le numero de la r‚solution d'ouverture */
																			/* de la station virtuelle d‚pend de la r‚solution */

	work_display.handle = graf_handle (&a, &b, &i, &i);

	v_opnvwk (_VDIParBlk.intin, &work_display.handle, _VDIParBlk.intout);
	work_display.n_color = _VDIParBlk.intout[13];
	work_display.n_color_pal = _VDIParBlk.intout[39];
	work_display.h = _VDIParBlk.intout[1] + 1;
	work_display.w = _VDIParBlk.intout[0] + 1;
}
/* #] open_work () Ouvrir station travail :												*/ 
/* #[ identifier_tos () Identifier le TOS :												*/
int identifier_tos(void)
{
int version_tos;
char **sysbase = (char **)0x4f2;		/* pointeur vers le debut du systeme d'exploitation	*/
char *systeme;												
long old_stack;

	if (Super ((void *)1L) == 0L)	/* Si on est en mode utilisateur */
		old_stack = Super (0L);			/* on passe en mode superviseur */
	systeme = *sysbase;						/* debut du systeme d'exploitation */

	if (old_stack)							/* Si on ‚tait en utilisateur au d‚but */
		Super((void *)old_stack);	/*  on y retourne */

	version_tos = *(int *)(systeme+2);
	return (version_tos);
}
/* #] identifier_tos () Identifier le TOS :												*/ 
/* #[ identifier_hard_soft () Identifier la machine :							*/
t_ident_hard_soft identifier_hard_soft (void)
{
COOKIE *reponse;
t_ident_hard_soft	id_locale;
long le_gdos;
char *c;

	/* identification de la machine */
	reponse = vq_cookie_c ("_MCH");
	if (reponse == NULL)		/* si il n'y a pas de reponse sur */
	{												/* le cookie MCH, on est sur un ST */
													/* sans cookie JAR et ses caracteristiques */
													/* sont donc celles du ST */
		id_locale.la_machine = ST;
		id_locale.proc_graphique = PG_ST;
		id_locale.le_cpu = PROC_68000;
	}
	else							
	{
		switch (reponse->datack.i[0])	/* Seuls les 16 bits forts nous interessent */
								/* et un switch se fait toujours sur un entier */
		{	
		case	0x0 :
			id_locale.la_machine = ST;
			break;
		case	0x1 :
			id_locale.la_machine = STE;
			break;
		case 0x2 :
			id_locale.la_machine = TT;
			break;
		case 0x3 :
			id_locale.la_machine = FALCON_030;
			break;
		default :
			id_locale.la_machine = AUTRE_MACHINE;
			break;
		}

		/* identification du systeme graphique */
		reponse = vq_cookie_c ("_VDO");
		if (reponse != NULL)				/* si il y a une reponse */
		switch (reponse->datack.i[0])	/* seul les 16 bits forts nous interessent */
									/* et un switch se fait toujours sur un entier */
		{	
		case	0x0:
			id_locale.proc_graphique = PG_ST;
			break;
		case	0x1:
			id_locale.proc_graphique = PG_STE;
			break;
		case 0x2:
			id_locale.proc_graphique = PG_TT;
			break;
		case 0x3:
			id_locale.proc_graphique = PG_FALCON_030;
			break;
		default :
			id_locale.proc_graphique = AUTRE_PG;
			break;
		}	

		/* identification du CPU */
		reponse = vq_cookie_c ("_CPU");
		if (reponse != NULL)				/* si il y a une reponse */
		switch (reponse->datack.i[1])		/* seul les 16 bits faibles nous interessent */
		{
		case 0 :
			id_locale.le_cpu = PROC_68000;
			break;
		case 30 :
			id_locale.le_cpu = PROC_68030;
			break;
		default	:
			id_locale.le_cpu = AUTRE_PROCESSEUR;
			break;
		}
	}			/* fin du if sur la presence d'une cookie jar */

	id_locale.v_aes = _GemParBlk.global[0];	/* Nø de version AES */;
	if (_GemParBlk.global[1] == BLANK)
		id_locale.multitache = TRUE;	/* Nous sommes sous multitƒche */
	else
		id_locale.multitache = FALSE;
	id_locale.v_tos = identifier_tos ();

	le_gdos = vq_vgdos ();
	if (le_gdos == GDOS_FSM)	/* cas SPEEDO	*/
	{
		reponse = vq_cookie_c ("FSMC");
		c = (char *)reponse->datack.l;
		if ((c[0] == '_') &&
				(c[1] == 'F') &&
				(c[2] == 'S') &&
				(c[3] == 'M'))
		{
			id_locale.le_gdos = FSM_GDOS;
		}
		else
		{
			if ((c[0] == '_') &&
					(c[1] == 'S') &&
					(c[2] == 'P') &&
					(c[3] == 'D'))
			{
				id_locale.le_gdos = SPEEDO_GDOS;
			}
			else
			{
				id_locale.le_gdos = GDOS_STANDARD;
			}	
		}
	}
	else
	{
		if (le_gdos == GDOS_FNT)	/* cas fontgdos	*/
		{
			id_locale.le_gdos = FONT_GDOS;
		}
		else
		{
			if (le_gdos == GDOS_NONE)
			{
				id_locale.le_gdos = AUCUN_GDOS;
			}
			else
			{
				id_locale.le_gdos = GDOS_STANDARD;
			}
		}	
	}

	return (id_locale);
}
/* #] identifier_hard_soft () Identifier la machine :							*/ 

/* #[ end () Tout lib‚rer et quitter :														*/


void end (void)
{
int i = ZERO, j, rgb[3];

	if (win)
	{
		do			/* Pour chaque fenˆtre */
		{
			if (win[i].w_hg > ZERO)		/* Appeler les fonctions de fermeture de */
			{
				buf[3] = win[i].w_hg;	/* chaque fenˆtre pour lib‚rer l'‚ventuelle */
				(*win[i].w_close)();	/* m‚moire r‚serv‚e. */
			}
		} while (NOT (win[i++].w_flags & WFLAST));
	}

	if (_app)				/* Si pas accessoire */
	{
		if (adr_menu)
			menu_bar (adr_menu, FALSE);		/* Virer le menu */
		if (adr_desk)
			wind_set (ZERO, WF_NEWDESK, ZERO, ZERO);	/* Rendre le bureau */
	}

	if (screen.fd_addr)					/* Si ‚cran sauvegard‚ */
		free (screen.fd_addr);			/* Lib‚rer m‚moire */
	if (win)										/* Si m‚moire fenˆtres */
		free (win);									/* Lib‚rer m‚moire */
	if (adr_usr)								/* Si m‚moire UserDef */
		free (adr_usr);							/* Lib‚rer m‚moire */
	if (xedit)									/* Si m‚moire ‚ditables ‚tendus */
		free (xedit);								/* Lib‚rer m‚moire */
	if (fnc)										/* Si m‚moire pointeurs de fonction */
		free (fnc);									/* Lib‚rer m‚moire */
	if (buffer)									/* Si m‚moire buffer Copier/Couper/Coller */
		free (buffer);							/* Lib‚rer m‚moire */

	if (xpict)			/* MA 26/06/94 : S'il y a des images */
	{
		for (j = ZERO ; j < nb_pict ; j++)
		{
			if (xpict[j].mfdb_image.fd_addr)
			{
				free (xpict[j].mfdb_image.fd_addr);
				xpict[j].mfdb_image.fd_addr = ZERO;
			}
		}
		free (xpict);	/* Lib‚rer les images */
	}

	if (NOT intgr)		/* MA 18/07/94 : si le ressource n'est pas int‚gr‚ */
										/* lib‚ration du pointeur des adresses */
	{
		if (ir_trindex != NULL)
			free (ir_trindex);
		if (ir_frstr != NULL)
			free (ir_frstr);
	}
	if (NOT intgr)										/* Si ressource externe */
		rsrc_free ();										/* Le lib‚rer */

	for (i = ZERO ; i < work_display.n_color ; i++)
	{
		rgb[0] = work_display.palette[i][0];
		rgb[1] = work_display.palette[i][1];
		rgb[2] = work_display.palette[i][2];
		vs_color (work_display.handle, i, rgb);
	}
	if (work_display.palette)					/* Si m‚moire palette de couleurs */
		free (work_display.palette);		/* Lib‚rer m‚moire */

	/* Retour au mode d'origine si on est sur Falcon	et qu'on a chang‚ de mode */
	if (desc_hard_soft.proc_graphique == PG_FALCON_030)
	{
		if (work_display.mode != Vsetmode (-1))
			Vsetmode (work_display.mode);
	}
	v_clsvwk (work_display.handle);
	appl_exit ();
	exit (ZERO);
}
/* #] end () Tout lib‚rer et quitter :														*/ 

