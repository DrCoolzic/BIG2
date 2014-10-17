/*						BIG2_DOC.C
		Petit programme de consultation
			de la documentation de BIG2
						Juillet 1994
*/

#define LIGHT
#include <BIG2.H>
#include "BIG2_DOC.RSH"
#include "BDOC_IMP.H"
#include "LZW.H"
#include "STRUCT.H"

/* #[ D‚finitions :																									*/
#define NB_FENETRES 1

#define NUM_TREE 11
#define TRINDEX (OBJECT *)rs_trindex
#define FRSTR (char *)rs_frstr

#define PREFERENCES "BIG2_DOC.PRF"
#define DRIVER "BIG2_DOC.IMP"
/* #] D‚finitions :																									*/ 
/* #[ Variables globales :																					*/
char *comp, *uncomp;	/* Zones m‚moires d‚compression */
int nchap;						/* Nbre de chapitres */
int nbrel;						/* Nbre de lignes chapitre courant */

typedef struct txtchap {	/* D‚finition du texte d'un chapitre dans le fichier */
	long debut;		/* Offset de d‚but dans le fichier */
} Txtchap;
Txtchap *tabh;				/* Pointeur sur le tableau des lignes fichiers d'aide */

Config pref;
/* #] Variables globales :																					*/ 
/* #[ Prototypage :																									*/
void main (void);
int reservations (void);
int compte_option (int option);
void acc_loop (void);
void lire_pref (void);
void sauve_pref (void);
void new_chap (void);
int quelle_option (void);
void avant (void);
void apres (void);
int load_help (void);
int open_help (void);
void aff_help (void);
void aff_line (char *ptr, int x, int y, int wcell);
void redicn (void);
void imprime (void);
/* #] Prototypage :																									*/ 
/* #[ main () Initialisations et gestion :													*/
void main (void)
{
int evnt, quit = FALSE, dummy, hcell,
		ok = FALSE, flacc = FALSE, tit;
MFDB img;

			/* Initialiser */
	if (initial ("BIG2_DOC.RSC", BLANK, BLANK, NUM_TREE,
							 TRINDEX, FRSTR, NB_FENETRES, TITACC))
	{
		form_attach (MENU, M_COBJETS, C_OBJETS);
		form_attach (MENU, M_CFENETRES, C_FENE);
		form_attach (MENU, M_CFONCTIONS, C_FONC);
		form_attach (MENU, M_FGENE, F_GENERALES);
		form_attach (MENU, M_FFORM, F_FORMULAIRES);
		form_attach (MENU, M_FFENE, F_FENETRES);
		form_attach (MENU, M_FDIVER, F_DIVERSES);
		form_attach (MENU, M_SOURCE, SOURCE);
		form_attach (MENU, M_CONSEIL, CONSEILS);

		if (work_display.n_color >= 16)
			set_windows (MD_REPLACE, RED, 3, BLUE, BLANK,	/* Mover et Info */
									 5, BLUE, 5, BLUE,								/* Ascenseurs */
									 8, YELLOW, 8, YELLOW,						/* Sliders */
									 WHITE, MAGENTA);									/* Boutons */

		lire_pref ();
		if (NOT reservations ())
		{
			if (_app)
				end ();
			else
			{
				while (TRUE)
					evnt_mesag (buf);
			}
		}

		if ((pref.opt - 3) == FIRSTOB)
			ir_trindex[MENU][PRECEDENT].ob_state |= DISABLED;
		if (pref.chap == nchap - 1)
			ir_trindex[MENU][SUIVANT].ob_state |= DISABLED;

		if (NOT lire_driver (DRIVER))		/* Charger le driver */
			ir_trindex[MENU][IMPRIMER].ob_state |= DISABLED;

		if (_app)								/* Si on est en programme */
		{
			if (load_help ())			/* On charge et */
				if (open_help ())		/* on ouvre de suite */
					ok = TRUE;
		}
		else										/* Si on est en accessoire */
			ok = TRUE;						/* on attend un peu... */
		if (ok)
		{
			/* BOUCLE PRINCIPALE */
			do
			{
				if ((_app == ZERO) && (NOT flacc))	/* Si on tourne en accessoire */
				{
					acc_loop ();		/* Boucle d'attente */
					flacc = TRUE;
				}
				evnt = dialog (DLG_DESK, adr_desk, TRUE, FALSE, BLANK, &img, FALSE);
				if (evnt & MU_MESAG)						/* Ev‚nement message */
				{
					if (buf[0] == AC_CLOSE)				/* Message AC_CLOSE */
					{
						win[WHELP].w_hg = ZERO;			/* La fenˆtre est ferm‚e par le GEM */
						win[WHELP].w_lin = ZERO;		/* D‚but du chapitre */
						flacc = FALSE;							/* ACC inactif */
					}
					else if (buf[0] == AC_OPEN)		/* Message AC_OPEN (c'est que la fenˆtre est d‚j… ouverte) */
						open_help ();								/* Fenˆtre en premier plan */
					else if ((buf[0] == WM_CLOSED) && (buf[3] == win[WHELP].w_hg))	/* Fermeture de fenˆtre */
					{
						if (_app)										/* Si programme */
							quit = TRUE;							/* Quitter si on ferme la fenˆtre */
						else												/* Si accessoire */
							flacc = FALSE;						/* ACC inactif */
					}
				}
				else if ((evnt & MU_BUTTON) && (buf[0] == BEV_WMENU))	/* Message Menu fenˆtre */
				{
					if (buf[5] == WHELP)			/* Fenˆtre d'aide en ligne */
					{
						tit = buf[3];						/* Enregistrer le titre */
						switch (buf[4] - 3)
						{
						case FONTE :					/* Si demande changement taille */
							if (pref.sizeh == 13)
							{
								pref.sizeh = 6;
								win[WHELP].w_bar[FONTE + 3].ob_state |= CHECKED;
							}
							else
							{
								pref.sizeh = 13;
								win[WHELP].w_bar[FONTE + 3].ob_state &= ~CHECKED;
							}
							vst_height (work_display.handle, pref.sizeh, &dummy, &dummy, &dummy, &hcell);
							win[WHELP].w_htot = (long) ((long) nbrel * (long) hcell);	/* Modif hauteur totale fenˆtre */
							win[WHELP].w_hunit = hcell;			/* Nouvelle unit‚ en hauteur */
							win[WHELP].w_lin = ZERO;				/* Repartir premiŠre ligne */
							height_sliders (WHELP, TRUE);		/* Ajuster sliders */
							print_page (WHELP);							/* R‚afficher fenˆtre */
							break;
						case SAUVER :					/* Si sauvegarde config */
							sauve_pref ();
							break;
						case PRECEDENT :			/* Si paragraphe pr‚c‚dent */
							avant ();
							break;
						case SUIVANT :				/* Si paragraphe suivant */
							apres ();
							break;
						case SDRIVER :
							if (select_driver ())
								win[WHELP].w_bar[IMPRIMER + 3].ob_state &= ~DISABLED;
							else
								win[WHELP].w_bar[IMPRIMER + 3].ob_state |= DISABLED;
							break;
						case IMPRIMER :				/* Si impression */
							imprime ();
							break;
						case QUITTER :				/* Si quitter */
							if (_app)							/* En programme */
								quit = TRUE;				/* On quitte */
							else									/* En accessoire */
							{
								buf[3] = win[WHELP].w_hg;	/* On ferme */
								closed ();
								flacc = FALSE;						/* ACC inactif */
							}
							break;
						default :
							new_chap ();
							break;
						}
						win[WHELP].w_bar[tit].ob_state &= ~SELECTED;
						draw_object (tit, WHELP);	/* D‚s‚lectionner le titre */
					}
				}
				else if ((evnt & MU_BUTTON) && (buf[0] == BEV_WHIER))	/* Message Menu hi‚rarchique fenˆtre */
				{
					if (buf[7] == WHELP)			/* Fenˆtre d'aide en ligne */
					{
						tit = buf[3];						/* Enregistrer le titre */
						new_chap ();
						win[WHELP].w_bar[tit].ob_state &= ~SELECTED;
						draw_object (tit, WHELP);	/* D‚s‚lectionner le titre */
					}
				}
			} while (NOT quit);
		}
		free (win[WHELP].w_cont.w_adr);
		free (uncomp);
		free (comp);
		free (tabh);
		end ();			/* on peut quitter */
	}
}
/* #] main () Initialisations et gestion :													*/ 
/* #[ reservations () Pr‚parations en m‚moire :											*/
int reservations (void)
{
int fh, tmaxc, tmaxu;
char fichier[128];

	strcpy (fichier, pathapp);					/* On copie le chemin par d‚faut */
	strcat (fichier, "BIG2_DOC.DCM");		/* On y ajoute le nom du fichier */
	fh = (int)Fopen (fichier, FO_READ);	/* Ouverture fichier */
	if (fh > ZERO)
	{
		Fread (fh, sizeof (int), &nchap);		/* Lecture nbre de chapitres */
		Fread (fh, sizeof (int), &tmaxc);		/* Lecture taille + grd bloc compress‚ */
		Fread (fh, sizeof (int), &tmaxu);		/* Lecture taille + grd bloc non compress‚ */
		tabh = (Txtchap *) malloc ((nchap + 1) * sizeof (Txtchap));	/* R‚servation m‚moire du tableau */
		comp = (char *)malloc (tmaxc);			/* R‚servation m‚moire zone compress‚e */
		uncomp = (char *)malloc (tmaxu);		/* R‚servation m‚moire zone d‚compression */
		win[WHELP].w_cont.w_adr = (char *)malloc (tmaxu);		/* R‚servation m‚moire zone d‚compression */
		Fread (fh, sizeof (Txtchap) * (nchap + 1), tabh);	/* Lecture du tableau */
		Fclose (fh);												/* Fermeture du fichier */
		return TRUE;
	}
	return FALSE;
}
/* #] reservations () Pr‚parations en m‚moire :											*/ 
/* #[ compte_option () Compte les options d'un hi‚rarchique :				*/
int compte_option (int option)
{
int tree, n = ZERO;

	tree = (win[WHELP].w_bar[option].ob_state >> 8);
	do
	{
		n++;
	} while (NOT (ir_trindex[tree][n].ob_flags & LASTOB));
	return n;
}
/* #] compte_option () Compte les options d'un hi‚rarchique :				*/ 
/* #[ acc_loop () Boucle d'attente appel accessoire :								*/
void acc_loop (void)
{
int accrun = FALSE;

	do
	{
		evnt_mesag (buf);
		if (buf[0] == AC_OPEN)
		{
			if (load_help ())			/* On charge */
				if (open_help ())		/* On ouvre */
					accrun = TRUE;
		}
	} while (NOT accrun);
}
/* #] acc_loop () Boucle d'attente appel accessoire :								*/ 
/* #[ lire_pref () Lire les pr‚f‚rences :														*/
void lire_pref (void)
{
int fh;
char fichier[128];

	strcpy (fichier, pathapp);					/* On copie le chemin par d‚faut */
	strcat (fichier, PREFERENCES);			/* On y ajoute le nom du ficheir */
	if (exist (fichier, ZERO))					/* S'il existe */
	{
		fh = (int)Fopen (fichier, FO_READ);
		if (fh >= ZERO)
		{
			Fread (fh, sizeof (pref), &pref);
			Fclose (fh);
			if (strcmp (pref.ident, "GIB") != ZERO)
			{
				strcpy (pref.ident, "GIB");
				pref.sizeh = 13;
				pref.chap = 0;
				pref.opt = FIRSTOB + 3;
				if ((ir_trindex[MENU][FIRSTOB].ob_type >> 8) == B_HIERM)
					pref.opth = 1;
				else
					pref.opth = ZERO;
				pref.wpos.g_x = ZERO;
				pref.wpos.g_y = 25;
				pref.wpos.g_w = 640;
				pref.wpos.g_h = 200;
			}
		}

			/* Position fenˆtre */
		if (pref.wpos.g_x > bureau.xd + bureau.wd - 50)	/* Correction de position */
			pref.wpos.g_x = bureau.xd + bureau.wd - 50;		/* pour que la fenˆtre */
		if (pref.wpos.g_y > bureau.yd + bureau.hd - 50)	/* reste sur le */
			pref.wpos.g_y = bureau.yd + bureau.hd - 50;		/* bureau */
	}
	else
	{
		strcpy (pref.ident, "GIB");
		pref.sizeh = 13;
		pref.chap = 0;
		pref.opt = FIRSTOB + 3;
		if ((ir_trindex[MENU][FIRSTOB].ob_type >> 8) == B_HIERM)
			pref.opth = 1;
		else
			pref.opth = ZERO;
		pref.nbrel = 70;
		pref.wpos.g_x = ZERO;
		pref.wpos.g_y = 25;
		pref.wpos.g_w = 640;
		pref.wpos.g_h = 200;
	}
}
/* #] lire_pref () Lire les pr‚f‚rences :														*/ 
/* #[ sauve_pref () Sauver les pr‚f‚rences :												*/
void sauve_pref (void)
{
int fh;
char fichier[128];

	strcpy (fichier, pathapp);					/* On copie le chemin par d‚faut */
	strcat (fichier, PREFERENCES);			/* On y ajoute le nom du fichier */
	fh = (int)Fcreate (fichier, ZERO);
	if (fh >= ZERO)
	{
		pref.wpos.g_x = win[WHELP].w_curr.g_x;
		pref.wpos.g_y = win[WHELP].w_curr.g_y;
		pref.wpos.g_w = win[WHELP].w_curr.g_w;
		pref.wpos.g_h = win[WHELP].w_curr.g_h;
		Fwrite (fh, sizeof (pref), &pref);
		Fclose (fh);
	}
	else
	big_alert (1, NOTSPREF, "", TRUE);
}
/* #] sauve_pref () Sauver les pr‚f‚rences :												*/ 
/* #[ new_chap () Nouveau chapitre :																*/
void new_chap ()
{
int new, hcell, tree, dummy;

	new = quelle_option ();
	if (new != pref.opt)
	{
		if (pref.opth)
		{
			tree = (win[WHELP].w_bar[pref.opt].ob_state >> 8);
			ir_trindex[tree][pref.opth].ob_state &= ~CHECKED;
			pref.opth = ZERO;
		}
		win[WHELP].w_bar[pref.opt].ob_state &= ~CHECKED;
		win[WHELP].w_bar[buf[4]].ob_state |= CHECKED;
		pref.opt = buf[4];
		if (buf[0] == BEV_WHIER)
		{
			pref.opth = buf[6];
			tree = (win[WHELP].w_bar[pref.opt].ob_state >> 8);
			ir_trindex[tree][buf[6]].ob_state |= CHECKED;
		}
		pref.chap = new;
		load_help ();													/* Charger chapitre */
		vst_height (work_display.handle, pref.sizeh, &dummy, &dummy, &dummy, &hcell);
		win[WHELP].w_htot = nbrel * hcell;		/* Hauteur totale fenˆtre */
		win[WHELP].w_lin = ZERO;							/* Retour premiŠre ligne */
		print_page (WHELP);										/* R‚afficher fenˆtre */
		height_sliders (WHELP, TRUE);					/* Ajuster sliders */

		if (pref.opt == (FIRSTOB + 3))
			win[WHELP].w_bar[PRECEDENT + 3].ob_state |= DISABLED;
		else
			win[WHELP].w_bar[PRECEDENT + 3].ob_state &= ~DISABLED;
		if (win[WHELP].w_bar[pref.opt].ob_flags & LASTOB)
			win[WHELP].w_bar[SUIVANT + 3].ob_state |= DISABLED;
		else
			win[WHELP].w_bar[SUIVANT + 3].ob_state &= ~DISABLED;
	}
}
/* #] new_chap () Nouveau chapitre :																*/ 
/* #[ quelle_option () Quelle option … ‚t‚ cliqu‚e ? :							*/
int quelle_option (void)
{
int i = FIRSTOB + 3, option = BLANK;

	while (i <= buf[4])
	{
		if ((((win[WHELP].w_bar[i].ob_type & 0xFF) == G_STRING) ||		/* Si chaŒne */
				 ((win[WHELP].w_bar[i].ob_type & 0xFF) == G_USERDEF)) &&	/* ou UserDef */
				 (NOT (win[WHELP].w_bar[i].ob_state & DISABLED)))					/* et actif */
		{
			if (((win[WHELP].w_bar[i].ob_type & 0xFF00) >> 8) == B_HIERM)
			{
				if (i < buf[4])
					option += compte_option (i);
			}
			else
				option++;												/* Compter les options */
		}
		i++;
	}

	if (buf[0] == BEV_WHIER)
	{
		i = ZERO;
		do
		{
			option++;
		} while (++i < buf[6]);
	}

	return option;
}
/* #] quelle_option () Quelle option … ‚t‚ cliqu‚e ? :							*/ 
/* #[ avant () Paragraphe pr‚c‚dent :																*/
void avant (void)
{
int hcell, dummy;

	if (pref.chap > ZERO)
	{
		if (pref.opth)
			ir_trindex[win[WHELP].w_bar[pref.opt].ob_state >> 8][pref.opth].ob_state &= ~CHECKED;
		win[WHELP].w_bar[pref.opt].ob_state &= ~CHECKED;

		pref.chap--;
		if (pref.opth)
		{
			pref.opth--;
			if (NOT pref.opth)
			{
				pref.opt--;
				while (((win[WHELP].w_bar[pref.opt].ob_type & 0xFF) != G_STRING) ||
							 (win[WHELP].w_bar[pref.opt].ob_state & DISABLED))
					pref.opt--;
				if ((win[WHELP].w_bar[pref.opt].ob_type >> 8) == B_HIERM)
					pref.opth = compte_option (pref.opt);
			}
		}
		else
		{
			pref.opt--;
			while (((win[WHELP].w_bar[pref.opt].ob_type & 0xFF) != G_STRING) ||
						 (win[WHELP].w_bar[pref.opt].ob_state & DISABLED))
				pref.opt--;
			if ((win[WHELP].w_bar[pref.opt].ob_type >> 8) == B_HIERM)
				pref.opth = compte_option (pref.opt);
		}

		if (pref.opth)
			ir_trindex[win[WHELP].w_bar[pref.opt].ob_state >> 8][pref.opth].ob_state |= CHECKED;
		win[WHELP].w_bar[pref.opt].ob_state |= CHECKED;

		load_help ();													/* Charger chapitre */
		vst_height (work_display.handle, pref.sizeh, &dummy, &dummy, &dummy, &hcell);
		win[WHELP].w_htot = nbrel * hcell;		/* Hauteur totale fenˆtre */
		win[WHELP].w_lin = ZERO;							/* Retour premiŠre ligne */
		print_page (WHELP);										/* R‚afficher fenˆtre */
		height_sliders (WHELP, TRUE);					/* Ajuster sliders */

		win[WHELP].w_bar[SUIVANT + 3].ob_state &= ~DISABLED;
		if (pref.chap == ZERO)
			win[WHELP].w_bar[PRECEDENT + 3].ob_state |= DISABLED;
	}
}
/* #] avant () Paragraphe pr‚c‚dent :																*/ 
/* #[ apres () Paragraphe suivant :																	*/
void apres (void)
{
int hcell, dummy;

	if (pref.chap < (nchap - 1))
	{
		if (pref.opth)
			ir_trindex[win[WHELP].w_bar[pref.opt].ob_state >> 8][pref.opth].ob_state &= ~CHECKED;
		win[WHELP].w_bar[pref.opt].ob_state &= ~CHECKED;

		pref.chap++;
		if (pref.opth)
		{
			if (NOT (ir_trindex[win[WHELP].w_bar[pref.opt].ob_state >> 8][pref.opth].ob_flags & LASTOB))
				pref.opth++;
			else
			{
				pref.opt++;
				while (((win[WHELP].w_bar[pref.opt].ob_type & 0xFF) != G_STRING) ||
							 (win[WHELP].w_bar[pref.opt].ob_state & DISABLED))
					pref.opt++;
				if ((win[WHELP].w_bar[pref.opt].ob_type >> 8) == B_HIERM)
					pref.opth = 1;
			}
		}
		else
		{
			pref.opt++;
			while (((win[WHELP].w_bar[pref.opt].ob_type & 0xFF) != G_STRING) ||
						 (win[WHELP].w_bar[pref.opt].ob_state & DISABLED))
				pref.opt++;
			if ((win[WHELP].w_bar[pref.opt].ob_type >> 8) == B_HIERM)
				pref.opth = 1;
		}

		if (pref.opth)
			ir_trindex[win[WHELP].w_bar[pref.opt].ob_state >> 8][pref.opth].ob_state |= CHECKED;
		win[WHELP].w_bar[pref.opt].ob_state |= CHECKED;

		load_help ();													/* Charger chapitre */
		vst_height (work_display.handle, pref.sizeh, &dummy, &dummy, &dummy, &hcell);
		win[WHELP].w_htot = nbrel * hcell;		/* Hauteur totale fenˆtre */
		win[WHELP].w_lin = ZERO;							/* Retour premiŠre ligne */
		print_page (WHELP);										/* R‚afficher fenˆtre */
		height_sliders (WHELP, TRUE);					/* Ajuster sliders */

		win[WHELP].w_bar[PRECEDENT + 3].ob_state &= ~DISABLED;
		if (pref.chap == (nchap - 1))
			win[WHELP].w_bar[SUIVANT + 3].ob_state |= DISABLED;
	}
}
/* #] apres () Paragraphe suivant :																	*/ 
/* #[ load_help () Charger fichier aide :														*/
int load_help (void)
{
int fh, ok = TRUE, noctets = ZERO, nbre = ZERO, cumul = ZERO;
unsigned int size;
char *ptr, fichier[128];

	wind_update (BEG_UPDATE);
	strcpy (fichier, pathapp);				/* On copie le chemin par d‚faut */
	strcat (fichier, "BIG2_DOC.DCM");	/* On y ajoute le nom du fichier */

	graf_mouse (BUSYBEE, ZERO);
	if (exist (fichier, ZERO))			/* Si le file d'aide existe */
	{
		fh = (int)Fopen (fichier, FO_READ);
		if (fh >= ZERO)		/* Si fichier bien ouvert */
		{
			Fseek (tabh[pref.chap].debut, fh, 0);	/* Mettre le pointeur au d‚but du chapitre */
			Fread (fh, sizeof (unsigned int), &size);
			Fread (fh, size, comp);
			noctets = Decompression (comp, uncomp, size);
			cumul += noctets;
			nbre += (int)Fread (fh, sizeof (unsigned int), &size);
			nbre += (int)Fread (fh, size, comp);
			Fclose (fh);													/* Fermer le fichier */
			ptr = uncomp;
			nbrel = ZERO;
			do
			{				/* Compter les lignes du chapitres */
				ptr += strlen (ptr)+1;
				nbrel++;
			} while (ptr - uncomp < cumul);
			win[WHELP].w_cont.w_adr = uncomp;			/* Pointer vers la m‚moire */
		}
	}
	else
		ok = FALSE;
	graf_mouse (ARROW, ZERO);
	wind_update (END_UPDATE);
	return ok;
}
/* #] load_help () Charger fichier aide :														*/ 
/* #[ open_help () Ouvrir fenˆtre aide :														*/
int open_help (void)
{
int wcell, hcell, dummy;

	vst_height (work_display.handle, pref.sizeh, &dummy, &dummy, &wcell, &hcell);
	if (open_window (WHELP, WTYP_NORM,		/* Ouverture fenˆtre aide */
									 (NAME|CLOSER|FULLER|MOVER|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE|ALLSIZER|SMALLER),
									 pref.wpos.g_x, pref.wpos.g_y, pref.wpos.g_w, pref.wpos.g_h,
									 100, 100, BLANK, BLANK,
									 wcell, hcell, (WFARROW|WFGROUP), ZERO,
									 redicn, aff_help, closed, get_string (TITRE), "",
									 (long) ((long) 80 * (long) wcell),
									 (long) (nbrel * hcell),
									 TRUE, FALSE, MENU, ZERO, WHITE))
	{
		if (pref.sizeh == 6)
			win[WHELP].w_bar[FONTE + 3].ob_state |= CHECKED;
		win[WHELP].w_bar[pref.opt].ob_state |= CHECKED;
		if (pref.opth)
			ir_trindex[win[WHELP].w_bar[pref.opt].ob_state >> 8][pref.opth].ob_state |= CHECKED;
		return TRUE;
	}
	else
		return FALSE;
}
/* #] open_help () Ouvrir fenˆtre aide :														*/ 
/* #[ aff_help () Affichage de l'aide :															*/
void aff_help (void)
{
char *ptr;
int i, x, y, xw, yw, ww, hw, dummy, wcell, hcell, count = ZERO;

	if (win[WHELP].w_cont.w_adr)
	{
		zone_work (WHELP, &xw, &yw, &ww, &hw);

			/* Ecrire le texte */
		vst_height (work_display.handle, pref.sizeh, &dummy, &dummy, &wcell, &hcell);
		vst_alignment (work_display.handle, 0, 5, &dummy, &dummy);	/* En haut … gauche */
		vst_effects (work_display.handle, 0);			/* D‚sactiver tous les effets */
		vst_color (work_display.handle, BLACK);		/* Couleur du texte */
		x = xw - (int)win[WHELP].w_col + wcell;
		y = yw + (((zclip.g_y - yw) / hcell) * hcell);	/* Positionner l'affichage des lignes */
		vswr_mode (work_display.handle, MD_TRANS);		/* Dessin en mode Transparent */
		ptr = win[WHELP].w_cont.w_adr;								/* D‚but du texte */
		for (i = 0 ; i < ((win[WHELP].w_lin + (buf[5] - yw)) / win[WHELP].w_hunit) ; i++)
		{
			ptr += (strlen (ptr) + 1);		/* Sauter au d‚but de l'affichage */
			count++;
		}
		while ((y < yw + hw) && (count < nbrel))			/* Pour chaque ligne de la fenˆtre */
		{
			if (strchr (ptr, (int)'`'))
				aff_line (ptr, x, y, wcell);
			else																				/* Si aucun caractŠre d'attribut */
				v_gtext (work_display.handle, x, y, ptr);	/* Affichage ligne */
			ptr += strlen (ptr) + 1;										/* Texte suivant */
			y += hcell;																	/* Ligne suivante */
			count++;																		/* Compter les lignes */
		}
	}
}
/* #] aff_help () Affichage de l'aide :															*/ 
/* #[ aff_line () Afficher une ligne avec attributs :								*/
void aff_line (char *ptr, int x, int y, int wcell)
{
char *trouve, texte[128], ligne[128], att;

	strcpy (ligne, ptr);
	trouve = strchr (ligne, (int)'`');
	if (*(trouve - 1) != '\\')
	{
		strncpy (texte, ligne, (int)(trouve - ligne));
		texte[trouve - ligne] = '\0';
	}
	else
	{
		strcpy (trouve - 1, trouve);
		strncpy (texte, ligne, (int)(trouve - ligne));
		texte[trouve - ligne] = '\0';
		v_gtext (work_display.handle, x, y, texte);
		x += ((int)strlen (texte) * wcell);
		strcpy (ligne, trouve);
		trouve = strchr (ligne, (int)'`');
		if (trouve)
		{
			aff_line (ligne, x, y, wcell);
			return;
		}
		else
		{
			v_gtext (work_display.handle, x, y, ligne);
			return;
		}
	}

	v_gtext (work_display.handle, x, y, texte);
	x += ((int)strlen (texte) * wcell);
	att = ligne[trouve - ligne + 1] - ' ';
	vst_effects (work_display.handle, (int)att);
	strcpy (ligne, trouve + 2);
	trouve = strchr (ligne, (int)'`');
	if (trouve)
		aff_line (ligne, x, y, wcell);
	else
		v_gtext (work_display.handle, x, y, ligne);
	vst_effects (work_display.handle, ZERO);
}
/* #] aff_line () Afficher une ligne avec attributs :								*/ 
/* #[ redicn () Redraw si fenˆtre ic“nifi‚e :												*/
void redicn (void)
{
int xw, yw, ww, hw, pxy[8], w, h, color[2];
BITBLK *img;
MFDB nul = {0}, pic;

	img = rs_frimg[WICN];
	zone_work (WHELP, &xw, &yw, &ww, &hw);
	w = (img->bi_wb * 8) - 1;	/* Pr‚parer affichage de l'image */
	h = img->bi_hl - 1;
	fill_tab (pxy, 8,
						ZERO, ZERO, w, h,
						xw + ((ww - w) / 2), yw + ((hw - h) / 2),
						xw + ((ww - w) / 2) + w, yw + ((hw - h) / 2) + h);
	pic.fd_addr = (void *) img->bi_pdata;
	pic.fd_w = w;
	pic.fd_h = h;
  pic.fd_wdwidth = img->bi_wb / 2;
	pic.fd_stand = 0;
	pic.fd_nplanes = 1;

	color[0] = BLUE;
	color[1] = WHITE;
  vrt_cpyfm (work_display.handle, MD_REPLACE, pxy, &pic, &nul, color);
}
/* #] redicn () Redraw si fenˆtre ic“nifi‚e :												*/ 

