/*						>>> Big Is Gem <<<
				MODULE FONCTIONS DE GESTION DE L'AIDE EN LIGNE
*/
/* Gestion de l'aide en ligne */
/* par Bulles d'aides */
/* d'aprŠs une id‚e et un source de Mathias Agopian	*/
/* adapt‚ … BIG par Marc Abramson le 18/07/94 */

/****** En-tˆte BIG ***********************************************/
#include <BIG2.H>
#include "BIG2HELP.H"
#define CharProLine 34

int find_zone (int mx, int my);	/* Proto */
int aide_en_ligne = FALSE;			/* Variable globale */

/* #[ affiche_bulle () Affiche une bulle d'aide :									*/
void affiche_bulle(int mode,int x,int y,
									 int w, int h, char *t,
									 int obj, int top,
									 OBJECT *form)
{
int j, k, wh = ZERO, x1, y1, w1, h1, xf, yf, wf, hf, xc, yc,
		ox1, ox2, oy1, oy2, mx, my, mk, wcar, hcar, depx, depy,
		cpl, lin, pts[8], extend, i1, i2, i,
		CntLine, sortie, pxy[8], mb, dummy;
char t2[256];
size_t len;
char BulleTxt[960];			/* Maximum 960 caractŠres */
char *ptr_line[16];			/* maximum 16 lignes */
char *ptr_char = BulleTxt;
MFDB fond;

	if (mode != ZERO)
	{
		xc = x + w / 2;								/* Coordonn‚es du centre de l'objet */
		yc = y + h / 2;
	}
	else
		graf_mkstate (&xc, &yc, &dummy, &dummy);
	vst_alignment (work_display.handle, HLEFT, VDOWN, &dummy, &dummy);
	vst_effects (work_display.handle, EFNONE);
	vst_font (work_display.handle, 1);
	vst_height (work_display.handle, HMINUS, &dummy, &dummy, &wcar, &hcar);
	hcar += 2;

	depx = 15;
	depy = 15;
	len = strlen (t);
	cpl = (int)min (len, CharProLine);		/* Taille max d'une ligne, 64 ou la taille du texte */
	w1 = (cpl+2) * wcar;	/* Taille de la ligne avec un espace devant et un espace derriere */

	CntLine = ZERO;					/* On calcule la hauteur de la bulle et on */
	i = ZERO;								/* stoke le texte format‚ au passage */
	i1 = w1 - 4 * wcar;
	do
	{
		j = ZERO;
		do
		{
			do
			{
				t2[j++] = t[i++];		/* Copie d'un mot de la source vers le tampon */
			} while ((t2[j - 1] != ' ') && (t[i] != '\0'));
			t2[j] = '\0';

			/* Mesure de la taille du mot */
			if (desc_hard_soft.le_gdos == SPEEDO_GDOS)
			{
				vqt_f_extent (work_display.handle, t2, &pts[0]);
			}
			else
			{
				vqt_extent (work_display.handle, t2, &pts[0]);
			}
			extend = pts[2] - pts[0];
		} while ((extend < i1) && (t[i] != '\0')); /* Mesure de l'ensemble
												des mots tant qu'ils tiennent dans la ligne */
		if ((extend >= i1) || (t[i] != '\0'))
		{
			for (k = 1 ; ((k < j - 1) && (t2[k] != ' ')) ; k++);		/* V‚rifie qu'on a au moins 2 mots par ligne */

			if (k < j - 1)								/* Si c'est le cas, on prend les n-1 premiers mots */
			{
				do
				{
					j--;
					i--;
				} while (t2[j - 1] != ' ' );
			}
			t2[j - 1] = '\0';
		}
		strcpy (ptr_char, t2);
		ptr_line[CntLine] = ptr_char;	/* Stoke adresse de la ligne */
		ptr_char += (int)strlen (t2);	/* On d‚cale le pointeur pour stocker le texte suivant */
		ptr_char++;					/* On laisse la place pour le 0 */
		CntLine++;
	} while (t[i] != '\0');
	h1 = (CntLine + 1) * hcar + hcar / 2;

	if (xc < work_display.w / 2)							/* Si on est du cot‚ gauche */
	{
		x1 = xc + depx;
		ox1 = depx + 12;
		ox2 = depx + 2;
	}
	else
	{
		x1 = xc - w1 - depx;
		ox1 =- depx - 12;
		ox2 =- depx - 2;
	}

	if (yc < work_display.h / 2)							/* Si on est en haut */
	{
		y1 = yc + depy;
		oy1 = depy + 2;
		oy2 = depy + 12;
	}
	else
	{
		y1 = yc - h1 - depy;
		oy1 =- depy - 2;
		oy2 =- depy - 12;
	}

	if ((x1 + w1) >= work_display.w)
	{
		x1 = work_display.w - w1;
	}
	if ((y1 + h1) >= work_display.h)
	{
		y1 = work_display.h - h1;
	}

	xf = min (xc, x1) ; yf = min (yc, y1) ; wf = w1 + depx + 1 ; hf = h1 + depy + 1;

	get_bkgr (xf, yf, wf, hf, &fond);
	wind_update (BEG_UPDATE);
	vswr_mode (work_display.handle, MD_REPLACE);				/* mode remplissage */
	v_hide_c (work_display.handle);
	vsf_interior (work_display.handle, FIS_SOLID);
	vsf_color (work_display.handle, WHITE);
	vsf_perimeter (work_display.handle, FALSE);
	vsl_width (work_display.handle, 1);
	vsl_color (work_display.handle, BLACK);
	vst_color (work_display.handle, BLACK);

	fill_tab (pxy, 4, x1, y1, x1 + w1, y1 + h1);
	v_rfbox (work_display.handle, pxy);
	v_rbox (work_display.handle, pxy);

	fill_tab (pxy, 6, xc + ox1, yc + oy1, xc, yc, xc + ox2, yc + oy2);
	v_fillarea (work_display.handle, 3, pxy);
	v_pline (work_display.handle, 3, pxy);

	vswr_mode (work_display.handle, MD_TRANS);				/* mode transparent */

	lin = y1 + hcar + hcar / 2;					/* On affiche le texte */
	i1 = w1 - 4 * wcar;
	i2 = x1 + 2 * wcar;
	for (i = ZERO ; i < CntLine - 1 ; i++)
	{
		v_justified (work_display.handle, i2, lin, ptr_line[i], i1, 1, 1);
		lin += hcar;
	}
	v_gtext (work_display.handle, i2, lin, ptr_line[i]);
	vswr_mode (work_display.handle, MD_REPLACE);
	v_show_c (work_display.handle, TRUE);

	if (mode == ZERO)
	{
		do
		{
			graf_mkstate (&mx, &my, &mb, &mk);
			if (top)
				wh = find_zone (mx, my);
			/* On sort si l'aide en ligne n'est pas active tout le temps mais sur
				 un masque de touche et que ce masque de touche n'est plus actif */
			sortie = (aide_en_ligne != -1) && ((mk & aide_en_ligne) != aide_en_ligne);
		} while (((mb & 0x1) == ZERO) && 		/* Aucune touche appuy‚e */
				(NOT sortie) &&
				((wh == top) &&									/* On est dans la mˆme fenˆtre */
				(objc_find (form, ROOT, MAX_DEPTH, mx, my) == obj)));	/* et sur l'objet */
	}
	else
	{
		evnt_keybd ();
	}
	wind_update (END_UPDATE);
	put_bkgr (xf, yf, wf, hf, &fond);
}
/* #] affiche_bulle () Affiche une bulle d'aide :									*/ 
/* #[ inline_help () Gestion des bulles d'aide :									*/
void inline_help (int index, int mx, int my, int top, OBJECT *adr)
{
int xtype, help_index, obstate;
OBJECT *form;
int ob, xo, yo, wo, ho;
char *texte;

	if (index > BLANK)
	{
		if ((win[index].w_hg != ZERO) && (win[index].w_icon == BLANK) && (find_zone (mx, my)))
		{		/* Si la fenˆtre est ouverte et pas ic“nifi‚e et qu'on est dedans */
			if (win[index].w_type & WTYP_TOOL)
			{
					form = win[index].w_bar;
			}
			else
			{
				if (win[index].w_type & WTYP_FORM)	/* Fenˆtre formulaire */
				{
					form = win[index].w_cont.w_form.w_tree;
				}
				else
				{
					return;
				}
			}
		}
	}
	else
	{
		form = adr;
	}

	ob = objc_find (form, ROOT, MAX_DEPTH, mx, my);
	if (ob > BLANK)
	{
		/* l'objet trouv‚ a t'il une aide associee */
		if (form[ob].ob_flags & AIDE_OBJET)
		{
			xtype = form[ob].ob_type >> 8;
			obstate = form[ob].ob_state;
			if (obstate & DISABLED)
				return;	/* Si objet inactif, on sort */
				/* R‚cup‚ration du nø de la free string : */
			if ((xtype == B_EDIT) || (xtype == B_FRAME) || (xtype == B_HIERM) ||
					(xtype == B_GRASTR) || (xtype == B_FNCP))
			{	/* Certains types d'objet ne peuvent avoir d'aide */
				return;
			}
			else if ((xtype == B_HELP) || (xtype == B_POPUP) ||
							 (xtype == B_LIST) ||
							 ((obstate & SHADOWED) && NOT (obstate & OUTLINED)))
			{	/* D'autres objets pointent deja vers un autre objet: il faut alors
					 r‚cup‚rer la free string associ‚e … la racine de l'arbre d‚sign‚ */
				help_index = form[ob].ob_state >> 8;
				help_index = ir_trindex[help_index]->ob_state >> 8;
			}
			else
			{	/* Enfin, d'autres portent directement le num‚ro de la free string associ‚e */
				help_index = form[ob].ob_state >> 8;
			}
		}
		else
		{
			return;
		}
		texte = ir_frstr[help_index];
		if (texte != ZERO)
		{
			objc_offset (form, ob, &xo, &yo);
			wo = form[ob].ob_width;
			ho = form[ob].ob_height;
			affiche_bulle (ZERO, xo - 4, yo - 4, wo + 8, ho + 8, texte, ob, top, form);
		}
	}
}
/* #] inline_help () Gestion des bulles d'aide :									*/ 
/* #[ find_zone () Teste si on est dans une fenˆtre :							*/
int find_zone (int mx, int my)
{
int xw, yw, ww, hw, fenetre, index;

	fenetre = wind_find (mx, my);	/* On a cliqu‚ sur une fenˆtre ? */
	if (fenetre)										/* Si oui */
	{
		index = find_index (fenetre);							/* Prendre l'index */
		if (index > BLANK)
		{		/* Zone de travail de GEM */
			wind_get (win[index].w_hg, WF_WORKXYWH, &xw, &yw, &ww, &hw);
			if ((mx > xw) && (mx < (xw + ww)))				/* Si on a cliqu‚ */
				if ((my > yw) && (my < (yw + hw)))			/* dans la zone */
					return fenetre;												/* retourner Handle GEM */
		}
	}
	return FALSE;																/* Sinon FALSE */
}
/* #] find_zone () Teste si on est dans une fenˆtre :							*/ 

