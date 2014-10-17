/*						BIG2_DOC.C
				Module d'impression */

#define LIGHT
#include <BIG2.H>
/* #[ D‚finitions, globales et proto :															*/
#include "BIG2_DOC.H"
#include "BDOC_IMP.H"
#include "STRUCT.H"

extern Config pref;
extern int nbrel;		/* Nbre de lignes chapitre courant */

	/* Structure des codes d'impression */
typedef struct codeimp {	/* STRUCTURE DES CODES IMPRIMANTE */
	char nb;									/* Longueur du codage */
	char codage[12];					/* Codage (le 1ø octet est la longueur du codage) */
} Codeimp;
Codeimp raz;							/* RAZ imprimante */
Codeimp gras_on;					/* Active Gras */
Codeimp gras_off;					/* D‚sactive Gras */
Codeimp ital_on;					/* Active Italique */
Codeimp ital_off;					/* D‚sactive Italique */
Codeimp under_on;					/* Active Soulign‚ */
Codeimp under_off;				/* D‚sactive Soulign‚ */
Codeimp table[256];				/* tableau des caractŠres */

void read_line (char *ptr, char *line, int *len);
void analyse_line (char *line, char *buffer, char *n);
char atoh (char *s);
void imprime (void);
int para_print (void);
void imp_line (char *line);
void imp_texte (char *txt);
void effect_gras (void);
void effect_ital (void);
void effect_under (void);
void not_effects (void);
int test_imp (void);
/* #] D‚finitions, globales et proto :															*/ 

/* #[ select_driver () Choix d'un driver :													*/
int select_driver (void)
{
int etat;
char chemin[128], fname[13] = "", fichier[128], *selec;

	etat = NOT (win[WHELP].w_bar[IMPRIMER + 3].ob_state & DISABLED);
	strcpy (chemin, pathapp);					/* Chemin de l'application */
	selec = get_string (SELDRIVER);		/* R‚cup‚rer message s‚lecteur */
	if (selector (chemin, "\*.IMP", fname, selec, TRUE) == TRUE && strlen (fname))
	{					/* Si un fichier a ‚t‚ choisi */
		strcpy (fichier, chemin);
		strcat (fichier, fname);					/* Reconstituer son chemin d'accŠs */
		if (exist (fichier, ZERO))
		{
			if (lire_driver (fichier))
				return TRUE;
		}
		return FALSE;
	}
	return etat;
}
/* #] select_driver () Choix d'un driver :													*/ 
/* #[ lire_driver () Charger un driver :														*/
int lire_driver (char *nom)
{
int fh, i = ZERO, len;
long taille;
char ligne[256], buffer[31], nb, *zone, *ptr;

	taille = exist (nom, ZERO);
	fh = (int)Fopen (nom, FO_READ);
	if (fh > ZERO)
	{
		zone = (char *)malloc (taille);
		Fread (fh, taille, zone);
		Fclose (fh);
		ptr = zone;

		read_line (ptr, ligne, &len);					/* Ligne de titre */
		ptr+= len;

		read_line (ptr, ligne, &len);					/* Ligne RAZ */
		ptr+= len;
		analyse_line (ligne, buffer, &nb);
		raz.nb = nb;
		strncpy (raz.codage, buffer, nb);
		raz.codage[nb] = '\0';

		read_line (ptr, ligne, &len);					/* Ligne GRAS ON */
		ptr+= len;
		analyse_line (ligne, buffer, &nb);
		gras_on.nb = nb;
		strncpy (gras_on.codage, buffer, nb);
		gras_on.codage[nb] = '\0';

		read_line (ptr, ligne, &len);					/* Ligne GRAS OFF */
		ptr+= len;
		analyse_line (ligne, buffer, &nb);
		gras_off.nb = nb;
		strncpy (gras_off.codage, buffer, nb);
		gras_off.codage[nb] = '\0';

		read_line (ptr, ligne, &len);					/* Ligne ITAL ON */
		ptr+= len;
		analyse_line (ligne, buffer, &nb);
		ital_on.nb = nb;
		strncpy (ital_on.codage, buffer, nb);
		ital_on.codage[nb] = '\0';

		read_line (ptr, ligne, &len);					/* Ligne ITAL OFF */
		ptr+= len;
		analyse_line (ligne, buffer, &nb);
		ital_off.nb = nb;
		strncpy (ital_off.codage, buffer, nb);
		ital_off.codage[nb] = '\0';

		read_line (ptr, ligne, &len);					/* Ligne SOULIG ON */
		ptr+= len;
		analyse_line (ligne, buffer, &nb);
		under_on.nb = nb;
		strncpy (under_on.codage, buffer, nb);
		under_on.codage[nb] = '\0';

		read_line (ptr, ligne, &len);					/* Ligne SOULIG OFF */
		ptr+= len;
		analyse_line (ligne, buffer, &nb);
		under_off.nb = nb;
		strncpy (under_off.codage, buffer, nb);
		under_off.codage[nb] = '\0';

			/* Remplir les ‚l‚ments de la table avec les codes ASCII correspondants */
		for (i = 0 ; i < 256 ; i++)
		{
			table[i].nb = 1;
			table[i].codage[0] = (char)i;
		}

			/* Lignes de code */
		read_line (ptr, ligne, &len);
		ptr+= len;
		while (NOT ((strlen (ligne) == 1) && (ligne[0] == '0')))
		{
			if (strlen (ligne))
			{
				analyse_line (ligne, buffer, &nb);
				if (strlen (buffer))
				{
					table[buffer[0]].nb = nb - 1;
					strncpy (table[buffer[0]].codage, buffer + 1, nb - 1);
					table[buffer[0]].codage[nb - 1] = '\0';
				}
			}
			read_line (ptr, ligne, &len);
			ptr+= len;
		}

		free (zone);
		return TRUE;
	}
	else
		return FALSE;
}
/* #] lire_driver () Charger un driver :														*/ 
/* #[ read_line () Lit une ligne de driver :												*/
void read_line (char *ptr, char *line, int *len)
{
int i = ZERO;
char c, *star;

	do
	{
		c = *ptr;						/* Lire un caractŠre */
		line[i++] = c;			/* Copier le caractŠre */
		ptr++;
	} while (c != '\r');
	line[i - 1] = '\0';
	*len = (int)strlen (line) + 2;

	star = strchr (line, '*');
	if (star)
		*star = '\0';
	trim (line);
}
/* #] read_line () Lit une ligne de driver :												*/ 
/* #[ analyse_line () Analyse une ligne de driver :									*/
void analyse_line (char *line, char *buffer, char *n)
{
int i = 1, len;
char val, c[3], *ptr, *v;

	ptr = line;
	*n = ZERO;
	do
	{
		v = strchr (ptr, ',');
		if (v)
		{
			len = (int)(v - ptr);
			strncpy (c, ptr, len);
			c[len] = '\0';
			(*n)++;
		}
		else if (strlen (ptr))
		{
			strcpy (c, ptr);
			(*n)++;
		}

		if (strlen (c))
		{
			val = (char)atoh (c);
			buffer[i - 1] = val;
			i++;
		}
		ptr += (len + 1);
		trim (ptr);
	} while (ptr < (line + strlen (line)));
}
/* #] analyse_line () Analyse une ligne de driver :									*/ 
/* #[ atoh () Conversion ChaŒne Hexa vers octet :										*/
char atoh (char *s)
{
int i;
char n;

	for (i = ZERO ; i < strlen (s) ; i++)	/* V‚rifier chaque caractŠre */
	{
		if (islower ((int)s[i]))						/* Mettre en majuscules */
			s[i] &= ~0x20;
		if (NOT isdigit ((int)s[i])) 				/* Pour les lettres de 'A' … 'F' */
			s[i] -= 7;
	}

	n = ZERO;
	for (i = ZERO ; (s[i] >= '0' && s[i] <= '9') || (s[i] >= ':' && s[i] <= '?') ; ++i)
		n = 16 * n + (s[i] - '0');

	return n;
}
/* #] atoh () Conversion ChaŒne Hexa vers octet :										*/ 
/* #[ imprime () Imprime le chapitre courant :											*/
void imprime (void)
{
register int i;
char *ptr, line[100];
int ligne, count = ZERO;

	if (para_print () == FALSE)
		return;

	if (test_imp () == TRUE)
	{
		graf_mouse (HOURGLASS, 0);			/* Souris -> abeille */
		wind_update (BEG_UPDATE);
		for (i = ZERO ; i < raz.nb ; i++)	/* RAZ de l'imprimante */
			Cprnout ((int)raz.codage[i]);

		ptr = win[WHELP].w_cont.w_adr;
		for (ligne = ZERO ; ligne < nbrel ; ligne++)
		{
			Cprnout (' ');		/* Un espace … gauche */
			strcpy (line, ptr);
			if (strchr (line, (int)'`'))
				imp_line (line);
			else
				imp_texte (line);
			ptr += (strlen (line) + 1);
			Cprnout ('\n');		/* Saut de ligne */
			Cprnout ('\r');
			count++;					/* Compter les lignes */
			if (count == pref.nbrel)
			{		/* Si nbre de ligne atteint */
				Cprnout ('\f');		/* Ejecter la page */
				count = ZERO;			/* Repartir … z‚ro */
			}
		}
		Cprnout ('\f');			/* Ejecter derniŠre page */

		wind_update (END_UPDATE);
		graf_mouse (ARROW, 0);								/* Souris -> flˆche */
	}
}
/* #] imprime () Imprime le chapitre courant :											*/ 
/* #[ para_print () ParamŠtres d'impression :												*/
int para_print (void)
{
int retour = FALSE;
char nbrel[3];
MFDB img;

	itoa (pref.nbrel, nbrel, 10);
	set_text (ir_trindex[PRINT], P_NBREL, nbrel);
	formf_draw (ir_trindex[PRINT], P_NBREL);
	object = BLANK;
	do
	{
		dialog (DLG_FORM, ir_trindex[PRINT], FALSE, FALSE, BLANK, &img, FALSE);
	} while (NOT (object == P_OK || object == P_ANN));
	formf_undraw (ir_trindex[PRINT]);
	if (object == P_OK)
	{
		retour = TRUE;
		strcpy (nbrel, get_text (ir_trindex[PRINT], P_NBREL));
		pref.nbrel = atoi (nbrel);
	}
	return retour;
}
/* #] para_print () ParamŠtres d'impression :												*/ 
/* #[ imp_line () Imprime une ligne :																*/
void imp_line (char *line)
{
char ligne[100], texte[100], *trouve, att;

	strcpy (ligne, line);
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
		imp_texte (texte);
		strcpy (ligne, trouve);
		trouve = strchr (ligne, (int)'`');
		if (trouve)
		{
			imp_line (ligne);
			return;
		}
		else
		{
			imp_texte (ligne);
			return;
		}
	}

	imp_texte (texte);
	att = ligne[trouve - ligne + 1] - ' ';	/* Lire les effets */
	not_effects ();				/* Annuler les effets pr‚c‚dents */
	if (att & 0x01)				/* Activer les effets voulus */
		effect_gras ();
	if (att & 0x04)
		effect_ital ();
	if (att & 0x08)
		effect_under ();
	strcpy (ligne, trouve + 2);
	trouve = strchr (ligne, (int)'`');
	if (trouve)
		imp_line (ligne);
	else
		imp_texte (ligne);
	not_effects ();
}
/* #] imp_line () Imprime une ligne :																*/ 
/* #[ imp_texte () Imprime un bout de texte :												*/
void imp_texte (char *txt)
{
int caractere, i, l;
char code;
Codeimp car;

	for (caractere = ZERO ; caractere < strlen (txt) ; caractere++)
	{
		code = txt[caractere];
		car = table[code];
		l = (int)car.nb;
		for (i = ZERO ; i < l ; i++)
			Cprnout ((int)car.codage[i]);
	}
}
/* #] imp_texte () Imprime un bout de texte :												*/ 
/* #[ effect_gras () Active le gras :																*/
void effect_gras (void)
{
register int i;

	for (i = ZERO ; i < gras_on.nb ; i++)
		Cprnout ((int)gras_on.codage[i]);
}
/* #] effect_gras () Active le gras :																*/ 
/* #[ effect_ital () Active l'italique :														*/
void effect_ital (void)
{
register int i;

	for (i = ZERO ; i < ital_on.nb ; i++)
		Cprnout ((int)ital_on.codage[i]);
}
/* #] effect_ital () Active l'italique :														*/ 
/* #[ effect_under () Active le soulign‚ :													*/
void effect_under (void)
{
register int i;

	for (i = ZERO ; i < under_on.nb ; i++)
		Cprnout ((int)under_on.codage[i]);
}
/* #] effect_under () Active le soulign‚ :													*/ 
/* #[ not_effects () Annule les effets de texte :										*/
void not_effects (void)
{
register int i;

	for (i = ZERO ; i < gras_off.nb ; i++)	/* Annule Gras */
		Cprnout ((int)gras_off.codage[i]);
	for (i = ZERO ; i < ital_off.nb ; i++)	/* Annule Italique */
		Cprnout ((int)ital_off.codage[i]);
	for (i = ZERO ; i < under_off.nb ; i++)	/* Annule Soulign‚ */
		Cprnout ((int)under_off.codage[i]);
}
/* #] not_effects () Annule les effets de texte :										*/ 
/* #[ test_imp () Teste si imprimante prˆte :												*/
int test_imp (void)
{
int r = TRUE;

	while (Cprnos () == FALSE && r == TRUE) 		/* V‚rifier imprimante prˆte */
	{
		if (big_alert (2, NOTIMP, "", TRUE) == 1)	/* Gueuler au besoin */
			r = FALSE;
	}
	return r;
}
/* #] test_imp () Teste si imprimante prˆte :												*/ 

