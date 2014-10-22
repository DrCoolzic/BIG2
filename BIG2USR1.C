/*! @file BIG2USR1.C
@brief implementation of functions for userdef objects

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

#include "big2.h"
#include "BIG2USR1.H"
#include "BIG2USR2.H"

/* #[ Look par d‚faut des objets BIG :														*/
Obj_user look = {TRUE,		/* Soulignement du caractŠre raccourci clavier */
								 RED,			/* Couleur du caractŠre raccourci clavier */
								 BLACK,		/* Couleur texte B_UNDER */
								 BLACK,		/* Couleur ic“ne B_CHECKB */
								 BLACK,		/* Couleur texte B_CHECKB */
								 BLACK,		/* Couleur ic“ne B_ROUNDB */
								 BLACK,		/* Couleur texte B_ROUNDB */
								 BLACK,		/* Couleur ic“ne B_POPUP / B_LIST */
								 BLACK,		/* Couleur texte B_POPUP / B_LIST */
								 BLACK,		/* Couleur B_MOVE */
								 BLACK,		/* Couleur texte B_GRASTR */
								 BLACK,		/* Couleur lignes B_FRAME */
								 BLACK,		/* Couleur texte B_FRAME */
								 BLACK};	/* Couleur B_NICELINE */
/* #] Look par d‚faut des objets BIG :														*/ 
/* #[ userdef () Objet -> UserDef : 															*/
void userdef (char *rsc, int *nb_form, int nb_tree)
{
OBJECT *adr;
RSHDR rsh;
int i, j, k = ZERO, xtype, nb_user = ZERO, fh, p = ZERO, ptr, nb_pfnc = ZERO,
		nb_xedit = ZERO, len_xedit = ZERO, taille, pere, coul, epais, nu_pict = ZERO,
		tab[NU] = {BW_SIZERUL, BW_CLOSER, BW_SMALLER, BW_CYCLER, BW_FULLER,
							 BW_SIZERUR, BW_UPARROW, BW_DNARROW, BW_SIZERDL, BW_LFARROW,
							 BW_RTARROW, BW_SIZERDR};

	if (NOT nb_tree)
	{
		fh = (int)Fopen (rsc, FO_READ);				/* Ouvrir RSC comme un fichier */
		Fread (fh, sizeof (rsh), &rsh);		/* Lire le header */
		Fclose (fh);														/* Fermer le fichier */
		*nb_form = rsh.rsh_ntree;					/* Prendre le nbre de formulaires */

		/* MA 18/07/94: allocation dynamique du ir_trindex dans le cas d'un ressource externe	*/
		ir_trindex = (OBJECT**)calloc (*nb_form, sizeof (OBJECT**));
		ir_frstr = (char **)calloc (rsh.rsh_nstring, sizeof (char **));
	}
	else
		*nb_form = nb_tree;

	/* MA 18/07/94: r‚cup‚ration de l'adresse des free string dŠs le d‚but
		 pour ˆtre compatible avec les ressources int‚gr‚s */
	if (NOT nb_tree)
	{
		for (i = ZERO ; i < rsh.rsh_nstring ; i++)
			rsrc_gaddr (R_STRING, i, &ir_frstr[i]);	/* Adr free string */
	}	

	for (i = ZERO ; i < *nb_form ; i++)					/* Pour chaque arbre */
	{
		if (NOT nb_tree)
		{
			rsrc_gaddr (R_TREE, i, &adr);						/* Adr formulaire */
			/* MA 18/07/94: on garde les adresses des TREEs */
			ir_trindex[i] = adr;
		}
		else
			adr = ir_trindex[i];
		j = ZERO;


		do		/* Compter les types ‚tendus (UserDef ou Pfnc) */
		{
			xtype = adr[j].ob_type >> 8;

			if ((xtype == B_MOVE) || (xtype == B_SELEC) ||
					(xtype == B_FRAME) || (xtype == B_GRASTR) ||
					(xtype == B_HIERM) || (xtype == B_UNDER) ||
					(xtype == B_UNDO) || (xtype == B_POPUP) ||
					(xtype == B_LIST) || (xtype == B_PICT) || (xtype == B_SMSTR))
			{
				/* MA 18/07/94: on garde le type de l'objet userdef dans le poids fort
					 de ob_type, pour pouvoir analyser ensuite en fonction du type de l'objet
					 on garde le type ‚tendu mais on remplace l'ob_type par USERDEF */
				adr[j].ob_type = (adr[j].ob_type & 0xFF00) + G_USERDEF;									
				nb_user++;					/* incr‚menter compteur UserDefs */
			}	
			if (adr[3].ob_type == G_TITLE)		/* Si on est dans un arbre de menu */
			{
				if (adr[j].ob_type == G_STRING && (adr[j].ob_state & DISABLED) &&
						((strstr(adr[j].ob_spec.free_string, "-----") != ZERO) ||
						(strstr(adr[j].ob_spec.free_string, "=====") != ZERO)))
					nb_user++;		/* Si s‚parateur DISABLED */
			}
			if (xtype == B_PICT)	/* MA 26/06/94 */
				nb_pict++;					/* Incr‚menter le compteur d'images */
			if (xtype == B_FNCP)
				nb_pfnc++;					/* incr‚menter compteur Pfncs */
			if (xtype == B_EDIT)
			{
				nb_xedit++;
				len_xedit += (adr[j].ob_state >> 8);
			}
		} while (NOT (adr[j++].ob_flags & LASTOB));	/* Jusqu'au dernier objet */
	}
	nb_user += (NFU + NTU);	/* Ne pas oublier le ressource inclu et les fenˆtres */
	len_xedit += (nb_xedit + 1);

	if (nb_pict)		/* s'il y a des images */
	{
		xpict = calloc (nb_pict, sizeof (t_image_dial));	/* Allouer la zone pour stocker leurs infos */
		for (nu_pict = ZERO ; nu_pict < nb_pict ; nu_pict++)
		{
			xpict[nu_pict].mfdb_image.fd_addr = NULL;	/* Le MFDB n'est pas allou‚ */
			xpict[nu_pict].cur_pos.g_x = xpict[nu_pict].cur_pos.g_y =
			xpict[nu_pict].cur_pos.g_w = xpict[nu_pict].cur_pos.g_h = ZERO;
		}
		nu_pict = ZERO;
	}
	
	if (nb_xedit)				/* S'il y a des ‚ditables ‚tendus */
	{
		xedit = (char *)malloc (len_xedit);
		*xedit = '\0';
		ptr = 1;

		for (i = ZERO ; i < *nb_form ; i++)					/* Pour chaque arbre */
		{
			adr = ir_trindex[i];
			j = ZERO;

			do
			{
				if ((adr[j].ob_type >> 8) == B_EDIT)
				{
					adr[j].ob_spec.tedinfo->te_ptext = xedit + ptr;
					taille = (adr[j].ob_state >> 8);
					ptr += (taille + 1);
					*(xedit + ptr - 1) = '\0';
					adr[j].ob_spec.tedinfo->te_txtlen = taille + 1;
					adr[j].ob_spec.tedinfo->te_tmplen +=
							taille - (int)strlen (adr[j].ob_spec.tedinfo->te_pvalid);
				}
			} while (NOT (adr[j++].ob_flags & LASTOB));	/* Jusqu'au dernier objet */
		}
	}
	else
		xedit = (char *)ZERO;

	if (nb_pfnc)				/* S'il y a des pointeurs sur fonction */
	{
		fnc = (FNCP *)malloc (nb_pfnc * sizeof (FNCP));
		ptr = ZERO;

		for (i = ZERO ; i < *nb_form ; i++)					/* Pour chaque arbre */
		{
			adr = ir_trindex[i];
			j = ZERO;

			do
			{
				if ((adr[j].ob_type >> 8) == B_FNCP)
				{
					ptr = p++;							/* Index du pointeur */
					ptr <<= 8;							/* Le transformer en ob_state ‚tendu */
					adr[j].ob_state |= ptr;	/* Le mettre dans l'ob_state ‚tendu */
				}
			} while (NOT (adr[j++].ob_flags & LASTOB));	/* Jusqu'au dernier objet */
		}
	}
	else
		fnc = (FNCP *)ZERO;

		/* Reserver tableau de structures USERBLKs */
	adr_usr = (USERBLK *)malloc (nb_user * sizeof(USERBLK));

	if (nb_user >= 1)		/* S'il y a des UserDefs dans le programme */
	{
		for (i = ZERO ; i < *nb_form ; i++)					/* Pour chaque arbre */
		{
			adr = ir_trindex[i];
			j = ZERO;

			do
			{
				xtype = adr[j].ob_type >> 8;
				switch (xtype)
				{
				case B_SELEC :
					if ((adr[j].ob_flags & SELECTABLE) && (adr[j].ob_flags & RBUTTON))
					{
						adr_usr[k].ub_code = brond;
						adr_usr[k].ub_parm = (long) adr[j].ob_spec.free_string;
						adr[j].ob_spec.userblk = adr_usr + k;
					}
					else if ((adr[j].ob_flags & SELECTABLE) &&
									 (NOT (adr[j].ob_state & SHADOWED)))
					{
						adr_usr[k].ub_code = bcoche;
						adr_usr[k].ub_parm = (long) adr[j].ob_spec.free_string;
						adr[j].ob_spec.userblk = adr_usr + k;
					}
					k++;
					break;
				case B_UNDER :
				case B_UNDO :
					adr_usr[k].ub_code = bunder;
					adr_usr[k].ub_parm = (long) adr[j].ob_spec.free_string;
					adr[j].ob_spec.userblk = adr_usr + k;
					k++;
					break;
				case B_GRASTR :
					adr_usr[k].ub_code = bgrastr;
					adr_usr[k].ub_parm = (long) adr[j].ob_spec.free_string;
					adr[j].ob_spec.userblk = adr_usr + k;
					k++;
					break;
				case B_SMSTR :
					adr_usr[k].ub_code = bsmstr;
					adr_usr[k].ub_parm = (long) adr[j].ob_spec.free_string;
					adr[j].ob_spec.userblk = adr_usr + k;
					k++;
					break;
				case B_FRAME :
					adr_usr[k].ub_code = bcadre;
					adr_usr[k].ub_parm = (long) adr[j].ob_spec.free_string;
					adr[j].ob_spec.userblk = adr_usr + k;
					k++;
					break;
				case B_HIERM :
					adr_usr[k].ub_code = bmhier;
					adr_usr[k].ub_parm = (long) adr[j].ob_spec.free_string;
					adr[j].ob_spec.userblk = adr_usr + k;
					k++;
					break;
				case B_MOVE :
					adr_usr[k].ub_code = bmove;
					adr_usr[k].ub_parm = ZERO;
					adr[j].ob_spec.userblk = adr_usr + k;
					k++;
					break;
				case B_POPUP :
				case B_LIST :
					adr_usr[k].ub_code = bpopup;
					adr_usr[k].ub_parm = (long) adr[j].ob_spec.free_string;
					adr[j].ob_spec.userblk = adr_usr + k;
					if (work_display.n_color < 16)	/* Si moins de 16 couleurs */
						adr[j].ob_flags &= ~BKGR;			/* Virer le BackGround */
					k++;
					break;
				case B_PICT :		/* MA 26/06/94 */
					adr_usr[k].ub_code = bpict;			/* Routine image en formulaire */
					adr_usr[k].ub_parm = (long)&xpict[nu_pict++];	/* Adresse de la zone de d‚finition de l'image */
					adr[j].ob_spec.userblk = adr_usr + k;
					k++;
					break;
				}

				if (adr[3].ob_type == G_TITLE)		/* Si on est dans un arbre de menu */
				{
					if ((adr[j].ob_type == G_STRING) && (adr[j].ob_state & DISABLED) &&
							((strstr(adr[j].ob_spec.free_string, "-----") != ZERO) ||
							(strstr(adr[j].ob_spec.free_string, "=====") != ZERO)))
					{		/* Si s‚parateur DISABLED */
						adr_usr[k].ub_code = bniceline;
						adr_usr[k].ub_parm = ZERO;
						adr[j].ob_spec.userblk = adr_usr + k;
						adr[j].ob_type = G_USERDEF;
						k++;
					}
				}

				if (((adr[j].ob_type & 0xFF) == G_FTEXT) || ((adr[j].ob_type & 0xFF) == G_FBOXTEXT))
				{
					if (adr[j].ob_flags & BKGR)
					{
						if ((adr[j].ob_type & 0xFF) == G_FTEXT)
						{
							adr[j].ob_type = (xtype << 8) | G_FBOXTEXT;
							epais = ZERO;
						}
						else
							epais = adr[j].ob_spec.tedinfo->te_thickness;
						coul = adr[j].ob_spec.tedinfo->te_color >> 8;
						adr[j].ob_spec.tedinfo->te_thickness = epais;
						pere = parent (adr, j);
						while (adr[pere].ob_type != G_BOX)
							pere = parent (adr, pere);
						adr[j].ob_spec.tedinfo->te_color = adr[pere].ob_spec.obspec.interiorcol;
						adr[j].ob_spec.tedinfo->te_color |= (adr[pere].ob_spec.obspec.fillpattern << 4);
						adr[j].ob_spec.tedinfo->te_color |= (coul << 8);
					}
					if (work_display.n_color < 16)		/* Si moins de 16 couleurs */
					{
						coul = adr[j].ob_spec.tedinfo->te_color >> 8;	/* Fond en blanc */
						adr[j].ob_spec.tedinfo->te_color = WHITE;
						adr[j].ob_spec.tedinfo->te_color |= (coul << 8);
					}
				}
			} while (NOT (adr[j++].ob_flags & LASTOB));	/* Jusqu'au dernier objet */
		}
	}
	else
		adr_usr = (USERBLK *)ZERO;

		/* Traiter UserDefs ressource inclu */
	adr_usr[k].ub_code = bmove;						/* Alerte int‚gr‚e */
	adr_usr[k].ub_parm = ZERO;
	bi_trindex[BIGALERTE][AL_MOVE].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIGALERTE][AL_MOVE].ob_type = G_USERDEF;
	bi_trindex[BIGALERTE][AL_MOVE].ob_type |= 0x1100;	/* Garder type ‚tendu */
	k++;
	adr_usr[k].ub_code = bunder;
	adr_usr[k].ub_parm = (long) bi_trindex[BIGALERTE][AL_BU1].ob_spec.free_string;
	bi_trindex[BIGALERTE][AL_BU1].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIGALERTE][AL_BU1].ob_type = G_USERDEF;
	k++;
	adr_usr[k].ub_code = bunder;
	adr_usr[k].ub_parm = (long) bi_trindex[BIGALERTE][AL_BU2].ob_spec.free_string;
	bi_trindex[BIGALERTE][AL_BU2].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIGALERTE][AL_BU2].ob_type = G_USERDEF;
	k++;
	adr_usr[k].ub_code = bunder;
	adr_usr[k].ub_parm = (long) bi_trindex[BIGALERTE][AL_BU3].ob_spec.free_string;
	bi_trindex[BIGALERTE][AL_BU3].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIGALERTE][AL_BU3].ob_type = G_USERDEF;
	k++;																	/* Les fenˆtres BIG */
	adr_usr[k].ub_code = fond;								/* Le fond (racine) */
	adr_usr[k].ub_parm = ZERO;
	bi_trindex[BIG_WIND][ROOT].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIG_WIND][ROOT].ob_type = G_USERDEF;
	k++;
	adr_usr[k].ub_code = titinf;							/* Barre de titre */
	adr_usr[k].ub_parm = ZERO;
	bi_trindex[BIG_WIND][BW_MOVER].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIG_WIND][BW_MOVER].ob_type = G_USERDEF;
	k++;
	adr_usr[k].ub_code = titinf;							/* Barre d'info */
	adr_usr[k].ub_parm = ZERO;
	bi_trindex[BIG_WIND][BW_INFO].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIG_WIND][BW_INFO].ob_type = G_USERDEF;
	k++;
	adr_usr[k].ub_code = elev;								/* Ascenseur V */
	adr_usr[k].ub_parm = ZERO;
	bi_trindex[BIG_WIND][BW_VELEV].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIG_WIND][BW_VELEV].ob_type = G_USERDEF;
	k++;
	adr_usr[k].ub_code = elev;								/* Ascenseur H */
	adr_usr[k].ub_parm = ZERO;
	bi_trindex[BIG_WIND][BW_HELEV].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIG_WIND][BW_HELEV].ob_type = G_USERDEF;
	k++;
	adr_usr[k].ub_code = sld;									/* Slider vertical */
	adr_usr[k].ub_parm = ZERO;
	bi_trindex[BIG_WIND][BW_VSLIDE].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIG_WIND][BW_VSLIDE].ob_type = G_USERDEF;
	k++;
	adr_usr[k].ub_code = sld;									/* Slider horizontal */
	adr_usr[k].ub_parm = ZERO;
	bi_trindex[BIG_WIND][BW_HSLIDE].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIG_WIND][BW_HSLIDE].ob_type = G_USERDEF;
	k++;
	adr_usr[k].ub_code = work;								/* Zone travail */
	adr_usr[k].ub_parm = ZERO;
	bi_trindex[BIG_WIND][BW_WORK].ob_spec.userblk = adr_usr + k;
	bi_trindex[BIG_WIND][BW_WORK].ob_type = G_USERDEF;
	k++;
	for (i = ZERO ; i < NU ; i++)					/* Pour chaque gadget de fenˆtre */
	{
		adr_usr[i + k].ub_code = gadget;
		adr_usr[i + k].ub_parm = ZERO;
		bi_trindex[BIG_WIND][tab[i]].ob_spec.userblk = adr_usr + i + k;
		bi_trindex[BIG_WIND][tab[i]].ob_type = G_USERDEF;
	}

	if (work_display.n_color >= 16)	/* Si on est en couleurs */
	{
		bi_trindex[BIGALERTE]->ob_spec.obspec.interiorcol = LWHITE;
		bi_trindex[BIGALERTE]->ob_spec.obspec.fillpattern = 7;
		bi_trindex[BIGLIST][BL_ASC].ob_spec.obspec.interiorcol = LWHITE;
		bi_trindex[BIGLIST][BL_ASC].ob_spec.obspec.fillpattern = 7;
		set_windows (BLANK, BLANK, BLANK, BLANK, BLANK,
								 8, LWHITE, 8, LWHITE,
								 BLANK, BLANK, BLANK, BLANK,
								 LWHITE, BLANK);
	}
}
/* #] userdef () Objet -> UserDef : 															*/ 
/* #[ bcoche () Routine bouton Coch‚ :														*/
int cdecl bcoche (PARMBLK *para)
{
int tab[4], pxy[8], w, h, len, pox, poy, i, sel,
		dummy, obj, color[2], pere, att[10];
char *texte, car[2];
MFDB nul = {0}, pic, msk;

	vqt_attributes (work_display.handle, att);	/* Noter attributs texte */
	vst_font (work_display.handle, 1);
	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);	/* BoŒtes caractŠres */
	fill_tab (tab, 4,			/* Zone de clip */
							para->pb_xc, para->pb_yc,
							para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);	/* Clip ON */

	sel = para->pb_currstate & SELECTED;

	if (h > 8)			/* Haute */
	{
		obj = sel ? BCHS : BCHN;
		pic.fd_h = msk.fd_h = 16;
	}
	else						/* Basse */
	{
		obj = sel ? BCMS : BCMN;
		pic.fd_h = msk.fd_h = 8;
	}

	pic.fd_addr = (void *) bi_trindex[IMG_USER][obj].ob_spec.iconblk->ib_pdata;
	msk.fd_addr = (void *) bi_trindex[IMG_USER][obj].ob_spec.iconblk->ib_pmask;
	pic.fd_w = msk.fd_w = 16;
	pic.fd_wdwidth = msk.fd_wdwidth = 1;
	pic.fd_nplanes = msk.fd_nplanes = 1;
	pic.fd_stand = msk.fd_stand = ZERO;

	if (NOT (para->pb_tree[para->pb_obj].ob_flags & BS_RIGHT))
		fill_tab (pxy, 8,
							ZERO, ZERO, pic.fd_w - 1, pic.fd_h - 1,
							para->pb_x, para->pb_y,
							para->pb_x + pic.fd_w - 1,
							para->pb_y + pic.fd_h - 1);
	else
		fill_tab (pxy, 8,
							ZERO, ZERO, pic.fd_w - 1, pic.fd_h - 1,
							para->pb_x + para->pb_w - pic.fd_w - 1, para->pb_y,
							para->pb_x + para->pb_w - pic.fd_w - 1 + pic.fd_w - 1,
							para->pb_y + pic.fd_h - 1);

	pere = parent (para->pb_tree, para->pb_obj);
	while (para->pb_tree[pere].ob_type != G_BOX)
		pere = parent (para->pb_tree, pere);
	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
		color[0] = para->pb_tree[pere].ob_spec.obspec.interiorcol;
	else
		color[0] = WHITE;
	color[1] = WHITE;
  vrt_cpyfm (work_display.handle, MD_TRANS, pxy, &msk, &nul, color);
	color[0] = look.cicn_coche;
  vrt_cpyfm (work_display.handle, MD_TRANS, pxy, &pic, &nul, color);

	texte = (char *)para->pb_parm;	/* R‚cup‚rer pointeur sur le texte */
	len = (int)strlen (texte);			/* Longueur du texte */
	vst_color (work_display.handle, look.ctxt_coche);
	vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);
	vswr_mode (work_display.handle, MD_TRANS);		/* Texte en mode transparent */
	vst_effects (work_display.handle, EFNONE);		/* D‚sactiver tous les effets */
	if (NOT (para->pb_tree[para->pb_obj].ob_flags & BS_RIGHT))
	{
		pox = para->pb_x + h + w;				/* Position en x */
		if (work_display.res == 1)
			pox += w;
	}
	else
		pox = para->pb_x;
	poy = para->pb_y;								/* et en y. */
	car[1] = '\0';
	for (i = ZERO ; i < len ; i++)	/* CaractŠres un par un */
	{
		if (texte[i] != '[')			/* Si ce n'est pas le [, */
		{
			car[0] = texte[i];								/* le mettre ds chaine temporaire, */
			v_gtext (work_display.handle, pox, poy, car);	/* et l'‚crire. */
			pox += w;													/* R‚ajuster position en x */
		}
		else											/* Si c'est le [, */
		{
			if (look.under)												/* Si soulignement */
				vst_effects (work_display.handle, EFSOULI);		/* Soulign‚ */
			vst_color (work_display.handle, look.cunder);	/* couleur caractŠre */
			i++;																/* caractŠre suivant, */
			car[0] = texte[i];									/* dans la chaine temporaire, */
			v_gtext (work_display.handle, pox, poy, car);		/* l'‚crire. */
			pox += w;														/* R‚ajuster position en x */
			vst_effects (work_display.handle, EFNONE);							/* D‚sactiver tous les effets */
			vst_color (work_display.handle, look.ctxt_coche);
		}
	}
	vs_clip (work_display.handle, FALSE, tab);	/* Clip OFF */
		/* Restaurer attributs */
	vswr_mode (work_display.handle, att[5]);
	vst_font (work_display.handle, att[0]);
	vst_color (work_display.handle, att[1]);
	vst_alignment (work_display.handle, att[3], att[4], &dummy, &dummy);
	vst_height (work_display.handle, work_display.hc,
							&att[6], &att[7], &att[8], &att[9]);
	return (para->pb_currstate & ~SELECTED);
}
/* #] bcoche () Routine bouton Coch‚ :														*/ 
/* #[ brond () Routine bouton Rond :															*/
int cdecl brond (PARMBLK *para)
{
int tab[4], pxy[8], w, h, len, pox, poy, i, sel,
		dummy, obj, color[2], pere, att[10];
char *texte, car[2];
MFDB nul = {0}, pic, msk;

	vqt_attributes (work_display.handle, att);	/* Noter attributs texte */
	vst_font (work_display.handle, 1);
	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);	/* BoŒtes caractŠres */

	fill_tab (tab, 4,			/* Zone de clip */
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);	/* Clip ON */

	sel = para->pb_currstate & SELECTED;

	if (h > 8)			/* Haute */
	{
		obj = sel ? RBHS : RBHN;
		pic.fd_h = msk.fd_h = 16;
	}
	else						/* Basse */
	{
		obj = sel ? RBMS : RBMN;
		pic.fd_h = msk.fd_h = 8;
	}

	pic.fd_addr = (void *) bi_trindex[IMG_USER][obj].ob_spec.iconblk->ib_pdata;
	msk.fd_addr = (void *) bi_trindex[IMG_USER][obj].ob_spec.iconblk->ib_pmask;
	pic.fd_w = msk.fd_w = 16;
	pic.fd_wdwidth = msk.fd_wdwidth = 1;
	pic.fd_nplanes = msk.fd_nplanes = 1;
	pic.fd_stand = msk.fd_stand = ZERO;

	if (NOT (para->pb_tree[para->pb_obj].ob_flags & BS_RIGHT))
		fill_tab (pxy, 8,
							ZERO, ZERO, pic.fd_w - 1, pic.fd_h - 1,
							para->pb_x, para->pb_y,
							para->pb_x + pic.fd_w - 1,
							para->pb_y + pic.fd_h - 1);
	else
		fill_tab (pxy, 8,
							ZERO, ZERO, pic.fd_w - 1, pic.fd_h - 1,
							para->pb_x + para->pb_w - pic.fd_w - 1, para->pb_y,
							para->pb_x + para->pb_w - pic.fd_w - 1 + pic.fd_w - 1,
							para->pb_y + pic.fd_h - 1);

	pere = parent (para->pb_tree, para->pb_obj);
	while (para->pb_tree[pere].ob_type != G_BOX)
		pere = parent (para->pb_tree, pere);
	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
		color[0] = para->pb_tree[pere].ob_spec.obspec.interiorcol;
	else
		color[0] = WHITE;
	color[1] = WHITE;
  vrt_cpyfm (work_display.handle, MD_TRANS, pxy, &msk, &nul, color);
	color[0] = look.cicn_radio;
  vrt_cpyfm (work_display.handle, MD_TRANS, pxy, &pic, &nul, color);

	texte = (char *)para->pb_parm;	/* R‚cup‚rer pointeur sur le texte */
	len = (int)strlen (texte);			/* Longueur du texte */
	vst_color (work_display.handle, look.ctxt_radio);
	vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);
	vswr_mode (work_display.handle, MD_TRANS);	/* Texte en mode Transparent */
	vst_effects (work_display.handle, EFNONE);		/* D‚sactiver tous les effets */
	if (NOT (para->pb_tree[para->pb_obj].ob_flags & BS_RIGHT))
	{
		pox = para->pb_x + h + w;				/* Position en x */
		if (work_display.res == 1)
			pox += w;
	}
	else
		pox = para->pb_x;
	poy = para->pb_y;								/* et en y. */
	car[1] = '\0';
	for (i = ZERO ; i < len ; i++)	/* CaractŠres un par un */
	{
		if (texte[i] != '[')			/* Si ce n'est pas le [, */
		{
			car[0] = texte[i];								/* dans la chaine temporaire, */
			v_gtext (work_display.handle, pox, poy, car);	/* l'‚crire. */
			pox += w;													/* R‚ajuster position en x */
		}
		else											/* Si c'est le [, */
		{
			if (look.under)																/* Si soulignement */
				vst_effects (work_display.handle, EFSOULI);		/* Soulign‚ */
			vst_color (work_display.handle, look.cunder);	/* Couleur caractŠre */
			i++;																/* caractŠre suivant, */
			car[0] = texte[i];									/* dans la chaine temporaire, */
			v_gtext (work_display.handle, pox, poy, car);		/* l'‚crire. */
			pox += w;														/* R‚ajuster position en x */
			vst_effects (work_display.handle, EFNONE);							/* D‚sactiver tous les effets */
			vst_color (work_display.handle, look.ctxt_radio);
		}
	}

	vs_clip (work_display.handle, FALSE, tab);	/* Clip OFF */
		/* Restaurer attributs */
	vswr_mode (work_display.handle, att[5]);
	vst_font (work_display.handle, att[0]);
	vst_color (work_display.handle, att[1]);
	vst_alignment (work_display.handle, att[3], att[4], &dummy, &dummy);
	vst_height (work_display.handle, work_display.hc,
							&att[6], &att[7], &att[8], &att[9]);
	return (para->pb_currstate & ~SELECTED);
}
/* #] brond () Routine bouton Rond :															*/ 
/* #[ bcadre () Routine bouton Cadre :														*/
int cdecl bcadre (PARMBLK *para)
{
int tab[4], rec[10], x, y, w, h, dummy, effets, pere, att[10], atf[5];
char *texte;

	vqt_attributes (work_display.handle, att);	/* Noter attributs texte */
	vqf_attributes (work_display.handle, atf);	/* Noter attributs remplissage */
	vst_font (work_display.handle, 1);
	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);	/* BoŒtes caractŠres */

	fill_tab (tab, 4,			/* Zone de clip */
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);	/* Clip ON */

	vswr_mode (work_display.handle, MD_REPLACE);				/* Dessin en mode Remplacement */
	vsf_color (work_display.handle, look.clin_frame);		/* Couleur cadre */
	if (NOT (para->pb_tree[para->pb_obj].ob_flags & BKGR))	/* Si pas Fond */
	{
		vsf_interior (work_display.handle, FIS_HOLLOW);	/* Fond blanc */
		vsf_perimeter (work_display.handle, TRUE);	/* Dessiner le p‚rimŠtre */
		fill_tab (rec, 4,
							para->pb_x, para->pb_y + (h / 2),
							para->pb_x + para->pb_w, para->pb_y + para->pb_h);
		v_bar (work_display.handle, rec);					/* Rectangle */
	}
	else
	{
		vsl_color (work_display.handle, look.clin_frame);
		vsl_width (work_display.handle, 1);
		vsl_ends (work_display.handle, SQUARE, SQUARE);
		vsl_type (work_display.handle, SOLID);
		fill_tab (rec, 10,
							para->pb_x, para->pb_y + (h / 2),
							para->pb_x + para->pb_w, para->pb_y + (h / 2),
							para->pb_x + para->pb_w, para->pb_y + para->pb_h,
							para->pb_x, para->pb_y + para->pb_h,
							para->pb_x, para->pb_y + (h / 2));
		v_pline (work_display.handle, 5, rec);		/* Rectangle vide */
		vsl_color (work_display.handle, WHITE);
		fill_tab (rec, 6,
							para->pb_x + 1, para->pb_y + para->pb_h,
							para->pb_x + 1, para->pb_y + (h / 2) + 1,
							para->pb_x + 1 + para->pb_w, para->pb_y + (h / 2) + 1);
		v_pline (work_display.handle, 3, rec);		/* 1ø ligne blanche */
		fill_tab (rec, 6,
							para->pb_x + para->pb_w + 1, para->pb_y + (h / 2) + 2,
							para->pb_x + para->pb_w + 1, para->pb_y + para->pb_h + 1,
							para->pb_x + 1, para->pb_y + para->pb_h + 1);
		v_pline (work_display.handle, 3, rec);		/* 2ø ligne blanche */
	}

	texte = (char *)para->pb_parm;	/* R‚cup‚rer pointeur sur le texte */
	fill_tab (rec, 4,
						para->pb_x + w, para->pb_y,
						para->pb_x + w + (w * (int)strlen (texte)),
						para->pb_y + h);
	vsf_perimeter (work_display.handle, FALSE);
	vsf_color (work_display.handle, BLACK);
	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
	{
		pere = parent (para->pb_tree, para->pb_obj);
		while (para->pb_tree[pere].ob_type != G_BOX)
			pere = parent (para->pb_tree, pere);
		if (para->pb_tree[pere].ob_spec.obspec.fillpattern == ZERO)
			vsf_interior (work_display.handle, FIS_HOLLOW);
		else if (para->pb_tree[pere].ob_spec.obspec.fillpattern == 7)
			vsf_interior (work_display.handle, FIS_SOLID);
		else
			vsf_interior (work_display.handle, FIS_PATTERN);
		vsf_color (work_display.handle, para->pb_tree[pere].ob_spec.obspec.interiorcol);
		vsf_style (work_display.handle, para->pb_tree[pere].ob_spec.obspec.fillpattern);
	}
	else
		vsf_interior (work_display.handle, FIS_HOLLOW);
	v_bar (work_display.handle, rec);

	vst_color (work_display.handle, look.ctxt_frame);
	vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);	/* Align‚ … gauche en haut */
	vswr_mode (work_display.handle, MD_TRANS);				/* Texte en mode Transparent */
	effets = para->pb_tree[para->pb_obj].ob_state >> 8;
	vst_effects (work_display.handle, effets);	/* Activer les effets */
	x = para->pb_x + w;						/* Position en x */
	y = para->pb_y;								/* et en y. */
	v_gtext (work_display.handle, x, y, texte);	/* Ecrire le texte */

	vs_clip (work_display.handle, FALSE, tab);	/* Clip OFF */
		/* Restaurer attributs */
	vswr_mode (work_display.handle, att[5]);
	vst_font (work_display.handle, att[0]);
	vst_color (work_display.handle, att[1]);
	vst_alignment (work_display.handle, att[3], att[4], &dummy, &dummy);
	vst_height (work_display.handle, work_display.hc,
							&att[6], &att[7], &att[8], &att[9]);
	vsf_interior (work_display.handle, atf[0]);
	vsf_color (work_display.handle, atf[1]);
	vsf_style (work_display.handle, atf[2]);
	vsf_perimeter (work_display.handle, atf[4]);
	return (para->pb_currstate);
}
/* #] bcadre () Routine bouton Cadre :														*/ 
/* #[ bunder () Routine bouton caractŠre soulign‚ :								*/
int cdecl bunder (PARMBLK *para)
{
int tab[4], rec[6], w, h, i, j, len, dummy, pox, poy,
		pere, borne, att[10], atf[5];
char *texte, car[2];

	vqt_attributes (work_display.handle, att);	/* Noter attributs texte */
	vqf_attributes (work_display.handle, atf);	/* Noter attributs remplissage */
	vst_font (work_display.handle, 1);
	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);

	fill_tab (tab, 4,			/* Zone de clip */
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);	/* Clip ON */

	vsf_perimeter (work_display.handle, TRUE);		/* Dessiner le p‚rimŠtre */
	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
	{
		pere = parent (para->pb_tree, para->pb_obj);
		while (para->pb_tree[pere].ob_type != G_BOX)
			pere = parent (para->pb_tree, pere);
		if (para->pb_tree[pere].ob_spec.obspec.fillpattern == ZERO)
			vsf_interior (work_display.handle, FIS_HOLLOW);
		else if (para->pb_tree[pere].ob_spec.obspec.fillpattern == 7)
			vsf_interior (work_display.handle, FIS_SOLID);
		else
			vsf_interior (work_display.handle, FIS_PATTERN);
		vsf_color (work_display.handle, para->pb_tree[pere].ob_spec.obspec.interiorcol);
		vsf_style (work_display.handle, para->pb_tree[pere].ob_spec.obspec.fillpattern);
	}
	else
	{
		vsf_color (work_display.handle, BLACK);						/* Couleur noire */
		if (para->pb_currstate & SELECTED)
			vsf_interior (work_display.handle, FIS_SOLID);	/* Fond noir */
		else
			vsf_interior (work_display.handle, FIS_HOLLOW);	/* Fond blanc */
	}
	vswr_mode (work_display.handle, MD_REPLACE);	/* Dessin en mode Remplacement */
	fill_tab (rec, 4,
						para->pb_x - 1, para->pb_y - 1,
						para->pb_x + para->pb_w, para->pb_y + para->pb_h);
	v_bar (work_display.handle, rec);				/* Rectangle int‚rieur */
	vswr_mode (work_display.handle, MD_TRANS);	/* Dessin en mode Transparent */
	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
	{
		vsf_color (work_display.handle, BLACK);
		vsf_interior (work_display.handle, FIS_HOLLOW);
		v_bar (work_display.handle, rec);				/* Contour rectangle */
	}

	if (para->pb_tree[para->pb_obj].ob_flags & EXIT)			/* Si EXIT */
	{
		vswr_mode (work_display.handle, MD_TRANS);				/* Dessin en mode Transparent */
		rec[0]--;
		rec[1]--;
		rec[2]++;
		rec[3]++;
		v_bar (work_display.handle, rec);			/* Second rectangle */
		if (para->pb_tree[para->pb_obj].ob_flags & DEFAULT)	/* Si DEFAULT */
		{
			rec[0]--;
			rec[1]--;
			rec[2]++;
			rec[3]++;
			v_bar (work_display.handle, rec);		/* TroisiŠme rectangle */
		}
	}
	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
	{
		if (NOT (para->pb_currstate & SELECTED))
		{
			fill_tab (rec, 6,
								para->pb_x, para->pb_y + para->pb_h,
								para->pb_x, para->pb_y,
								para->pb_x + para->pb_w, para->pb_y);
		}
		else
		{
			fill_tab (rec, 6,
								para->pb_x, para->pb_y + para->pb_h - 1,
								para->pb_x + para->pb_w - 1, para->pb_y + para->pb_h - 1,
								para->pb_x + para->pb_w - 1, para->pb_y);
		}
		vswr_mode (work_display.handle, MD_REPLACE);		/* Dessin en mode Remplacement */
		vsl_color (work_display.handle, WHITE);
		vsl_ends (work_display.handle, SQUARE, SQUARE);
		vsl_type (work_display.handle, SOLID);
		vsl_width (work_display.handle, 1);
		v_pline (work_display.handle, 3, rec);
	}

	texte = (char *)para->pb_parm;	/* R‚cup‚rer pointeur sur le texte */
	len = (int)strlen (texte);			/* Longueur du texte */
	
	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
		vswr_mode (work_display.handle, MD_TRANS);			/* Texte en mode Transparent */
	else																							/* Sinon */
		vswr_mode (work_display.handle, MD_XOR);				/* En Xor */
	vst_effects (work_display.handle, EFNONE);		/* D‚sactiver tous les effets */
	if (para->pb_tree[para->pb_obj].ob_state & DISABLED)	/* Si inactif, */
		vst_effects (work_display.handle, EFGRIS);													/* ‚criture claire */

	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
		borne = ZERO;
	else
		borne = 1;

	for (j = borne ; j < 2 ; j++)
	{
		if (j == ZERO)
			vst_color (work_display.handle, WHITE);
		else
			vst_color (work_display.handle, look.ctxt_under);
		if (strchr (texte, '['))	/* S'il y a un caractŠre soulign‚ */
		{
			vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);
			pox = para->pb_x + ((para->pb_w - ((len - 1) * w)) / 2) + j - 1;	/* Position x */
			poy = para->pb_y + ((para->pb_h - h) / 2) + j - 1;								/* et y. */
			if ((para->pb_currstate & SELECTED) &&
					(para->pb_tree[para->pb_obj].ob_flags & BKGR))
			{
				pox++;
				poy++;
			}
			car[1] = '\0';
			for (i = ZERO ; i < len ; i++)	/* CaractŠres un par un */
			{
				if (texte[i] != '[')			/* Si ce n'est pas le [, */
				{
					car[0] = texte[i];								/* dans la chaine temporaire, */
					v_gtext (work_display.handle, pox, poy, car);	/* l'‚crire. */
					pox += w;													/* R‚ajuster position en x */
				}
				else											/* Si c'est le [, */
				{
					if (look.under)															/* Si soulignement */
						vst_effects (work_display.handle, EFSOULI);	/* Soulign‚ */
					if (j)
						vst_color (work_display.handle, look.cunder);		/* Couleur caractŠre */
					if (para->pb_tree[para->pb_obj].ob_state & DISABLED)	/* Si inactif, */
					{
						if (look.under)																		/* Si soulignement, */
							vst_effects (work_display.handle, EFSOULI | EFGRIS);	/* en soulign‚ gris‚. */
						else																							/* Sinon, */
							vst_effects (work_display.handle, EFGRIS);				/* en gris‚. */
					}
					i++;																/* caractŠre suivant, */
					car[0] = texte[i];									/* dans la chaine temporaire, */
					v_gtext (work_display.handle, pox, poy, car);		/* l'‚crire. */
					pox += w;														/* R‚ajuster position en x */
					vst_effects (work_display.handle, EFNONE);						/* D‚sactiver tous les effets */
					if (j == ZERO)
						vst_color (work_display.handle, WHITE);
					else
						vst_color (work_display.handle, look.ctxt_under);
					if (para->pb_tree[para->pb_obj].ob_state & DISABLED)	/* Si inactif, */
						vst_effects (work_display.handle, EFGRIS);						/* en gris‚. */
				}
			}
		}
		else		/* S'il n'y a pas de caractŠre soulign‚ */
		{
			vst_alignment (work_display.handle, HCENT, VTOP, &dummy, &dummy);
			pox = para->pb_x + (para->pb_w / 2) + j - 1;					/* Position x */
			poy = para->pb_y + ((para->pb_h - h) / 2) + j - 1;		/* et y. */
			if ((para->pb_currstate & SELECTED) &&
					(para->pb_tree[para->pb_obj].ob_flags & BKGR))
			{
				pox++;
				poy++;
			}
			v_gtext (work_display.handle, pox, poy, texte);						/* Ecrire le texte */
		}
	}
	vs_clip (work_display.handle, FALSE, tab);	/* Clip OFF */
		/* Restaurer attributs */
	vswr_mode (work_display.handle, att[5]);
	vst_font (work_display.handle, att[0]);
	vst_color (work_display.handle, att[1]);
	vst_alignment (work_display.handle, att[3], att[4], &dummy, &dummy);
	vst_height (work_display.handle, work_display.hc,
							&att[6], &att[7], &att[8], &att[9]);
	vsf_interior (work_display.handle, atf[0]);
	vsf_color (work_display.handle, atf[1]);
	vsf_style (work_display.handle, atf[2]);
	vsf_perimeter (work_display.handle, atf[4]);
	return (para->pb_currstate & ~SELECTED);
}
/* #] bunder () Routine bouton caractŠre soulign‚ :								*/ 
/* #[ bmove () Routine bouton Move :															*/
int cdecl bmove (PARMBLK *para)
{
int tab[4], rec[6], atf[5];

	vqf_attributes (work_display.handle, atf);	/* Noter attributs remplissage */
	fill_tab (tab, 4,			/* Zone de clip */
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);	/* Clip ON */

	vswr_mode (work_display.handle, MD_REPLACE);		/* Dessin en mode Remplacement */
	vsf_interior (work_display.handle, FIS_HOLLOW);	/* Fond blanc */
	vsf_color (work_display.handle, look.cmove);		/* Couleur cadre */
	vsl_width (work_display.handle, 1);							/* Largeur de trait : 1 */
	vsl_ends (work_display.handle, SQUARE, SQUARE);
	vsl_type (work_display.handle, SOLID);
	vsl_color (work_display.handle, look.cmove);		/* Couleur lignes */
	vsf_perimeter (work_display.handle, TRUE);	/* Dessiner le p‚rimŠtre */
	fill_tab (rec, 6,			/* Triangle */
						para->pb_x - 2, para->pb_y - 2,
						para->pb_x + para->pb_w + 2,
						para->pb_y + para->pb_h + 2,
						para->pb_x - 2, para->pb_y + para->pb_h + 2);
	v_fillarea (work_display.handle, 3, rec);

	vs_clip (work_display.handle, FALSE, tab);	/* Clip OFF */
		/* Restaurer attributs */
	vswr_mode (work_display.handle, atf[3]);
	vsf_interior (work_display.handle, atf[0]);
	vsf_color (work_display.handle, atf[1]);
	vsf_style (work_display.handle, atf[2]);
	vsf_perimeter (work_display.handle, atf[4]);
	return ZERO;
}
/* #] bmove () Routine bouton Move :															*/ 
/* #[ bgrastr () Routine String graphique : 											*/
int cdecl bgrastr (PARMBLK *para)
{
int tab[4], rec[4], x, y, w, h, dummy, effets, att[10], atf[5];
char *texte;

	vqt_attributes (work_display.handle, att);	/* Noter attributs texte */
	vqf_attributes (work_display.handle, atf);	/* Noter attributs remplissage */
	vst_font (work_display.handle, 1);
	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);
	fill_tab (tab, 4,			/* Zone de clip */
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);	/* Clip ON */

	texte = (char *)para->pb_parm;	/* R‚cup‚rer pointeur sur le texte */
	fill_tab (rec, 4,
						para->pb_x, para->pb_y,
						para->pb_x + (w * (int)strlen (texte)), para->pb_y + h);
	vswr_mode (work_display.handle, MD_TRANS);		/* Dessin en mode Transparent */
	vsf_perimeter (work_display.handle, FALSE);	/* Ne pas dessiner le p‚rimŠtre */
	vsf_interior (work_display.handle, FIS_HOLLOW);
	vsf_color (work_display.handle, WHITE);			/* Couleur blanche */
	v_bar (work_display.handle, rec);						/* Rectangle */

	vsf_color (work_display.handle, BLACK);			/* Couleur noire */
	vst_color (work_display.handle, look.cgrastr);	/* Couleur texte */
	if (para->pb_tree[para->pb_obj].ob_flags & BS_RIGHT)
	{
		vst_alignment (work_display.handle, HRIGHT, VTOP, &dummy, &dummy);
		x = para->pb_x + para->pb_w;	/* Position en x */
	}
	else
	{
		vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);
		x = para->pb_x;								/* Position en x */
	}
	effets = para->pb_tree[para->pb_obj].ob_state >> 8;
	effets &= ~EFSOULI;							/* D‚sactiver soulign‚ */
	vst_effects (work_display.handle, effets);	/* Activer les effets */
	y = para->pb_y;								/* et en y. */
	v_gtext (work_display.handle, x, y, texte);	/* Ecrire le texte */

	if ((para->pb_tree[para->pb_obj].ob_state >> 8) & EFSOULI)
	{															/* Si soulign‚ */
		vsl_color (work_display.handle, look.cgrastr);	/* Couleur trait */
		vsl_ends (work_display.handle, SQUARE, SQUARE);
		vsl_width (work_display.handle, 1);			/* Largeur de trait : 1 */
		vsl_type (work_display.handle, SOLID);
		fill_tab (rec, 4,
							para->pb_x, para->pb_y + para->pb_h - 1,
							para->pb_x + para->pb_w - 1,
							para->pb_y + para->pb_h - 1);
		vswr_mode (work_display.handle, MD_TRANS);			/* Dessin en mode Transparent */
		v_pline (work_display.handle, 2, rec);		/* Tracer le trait */
		if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
		{
			vsl_color (work_display.handle, WHITE);
			vswr_mode (work_display.handle, MD_REPLACE);
			rec[0]++;									/* Doubler le trait d'une ligne blanche */
			rec[1]++;
			rec[2]++;
			rec[3]++;
			v_pline (work_display.handle, 2, rec);	/* Tracer la ligne */
		}
	}

	vs_clip (work_display.handle, FALSE, tab);	/* Clip OFF */
		/* Restaurer attributs */
	vswr_mode (work_display.handle, att[5]);
	vst_font (work_display.handle, att[0]);
	vst_color (work_display.handle, att[1]);
	vst_alignment (work_display.handle, att[3], att[4], &dummy, &dummy);
	vst_height (work_display.handle, work_display.hc,
							&att[6], &att[7], &att[8], &att[9]);
	vsf_interior (work_display.handle, atf[0]);
	vsf_color (work_display.handle, atf[1]);
	vsf_style (work_display.handle, atf[2]);
	vsf_perimeter (work_display.handle, atf[4]);
	return (para->pb_currstate);
}
/* #] bgrastr () Routine String graphique : 											*/ 
/* #[ bsmstr () Routine String en petit caractŠres :							*/
int cdecl bsmstr (PARMBLK *para)
{
int tab[4], rec[4], pox, poy, effets, dummy, wc, hc;
char *texte;

	vst_height (work_display.handle, HPETIT, &dummy, &dummy, &wc, &hc);	/* Petite taille */
	fill_tab (tab, 4,			/* Zone de clip */
							para->pb_xc, para->pb_yc,
							para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);	/* Clip ON */

	texte = (char *)para->pb_parm;	/* R‚cup‚rer pointeur sur le texte */
	vst_color (work_display.handle, BLACK);
	vst_effects (work_display.handle, EFNONE);			/* D‚sactiver tous les effets */
	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)
		vswr_mode (work_display.handle, MD_TRANS);		/* Texte en mode transparent */
	else
		vswr_mode (work_display.handle, MD_REPLACE);	/* Texte en mode opaque */
	if (para->pb_tree[para->pb_obj].ob_flags & BS_RIGHT)
	{
		vst_alignment (work_display.handle, HRIGHT, VTOP, &dummy, &dummy);
		pox = para->pb_x + para->pb_w;	/* Position en x */
	}
	else
	{
		vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);
		pox = para->pb_x;								/* Position en x */
	}
	effets = para->pb_tree[para->pb_obj].ob_state >> 8;
	effets &= ~EFSOULI;							/* D‚sactiver soulign‚ */
	vst_effects (work_display.handle, effets);	/* Activer les effets */
	poy = para->pb_y;									/* Position en y. */
	v_gtext (work_display.handle, pox, poy, texte);	/* Ecrire. */

	if ((para->pb_tree[para->pb_obj].ob_state >> 8) & EFSOULI)
	{															/* Si soulign‚ */
		vsl_color (work_display.handle, look.cgrastr);	/* Couleur trait */
		vsl_ends (work_display.handle, SQUARE, SQUARE);
		vsl_width (work_display.handle, 1);			/* Largeur de trait : 1 */
		vsl_type (work_display.handle, SOLID);
		if (para->pb_tree[para->pb_obj].ob_flags & BS_RIGHT)
			fill_tab (rec, 4,
								para->pb_x + para->pb_w - (wc * (int)strlen (texte)),
								para->pb_y + hc,
								para->pb_x + para->pb_w - 1,
								para->pb_y + hc);
		else
			fill_tab (rec, 4,
								para->pb_x, para->pb_y + hc,
								para->pb_x + wc * (int)strlen (texte),
								para->pb_y + hc);
		vswr_mode (work_display.handle, MD_TRANS);			/* Dessin en mode Transparent */
		v_pline (work_display.handle, 2, rec);		/* Tracer le trait */
		if (para->pb_tree[para->pb_obj].ob_flags & BKGR)	/* Si Fond */
		{
			vsl_color (work_display.handle, WHITE);
			vswr_mode (work_display.handle, MD_REPLACE);
			rec[0]++;									/* Doubler le trait d'une ligne blanche */
			rec[1]++;
			rec[2]++;
			rec[3]++;
			v_pline (work_display.handle, 2, rec);	/* Tracer la ligne */
		}
	}

	vs_clip (work_display.handle, FALSE, tab);	/* Clip OFF */
	return (para->pb_currstate & ~SELECTED);
}
/* #] bsmstr () Routine String en petit caractŠres :							*/ 
/* #[ bpopup () Routine bouton pop-up ou pop-liste :							*/
int cdecl bpopup (PARMBLK *para)
{
int tab[4], rec[8], w, h, dummy, pox, poy, obj, color[2],
		pere, i, temp, att[10], atf[5];
char *texte;
MFDB nul = {0}, pic, msk;

	vqt_attributes (work_display.handle, att);	/* Noter attributs texte */
	vqf_attributes (work_display.handle, atf);	/* Noter attributs remplissage */
	vst_font (work_display.handle, 1);
	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);
	fill_tab (tab, 4,			/* Zone de clip */
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);	/* Clip ON */

	vswr_mode (work_display.handle, MD_REPLACE);				/* Dessin en mode Remplacement */
	vsf_perimeter (work_display.handle, TRUE);
	if (para->pb_tree[para->pb_obj].ob_flags & BKGR)
	{
		fill_tab (rec, 4,
							para->pb_x - 1, para->pb_y - 1,
							para->pb_x + para->pb_w, para->pb_y + para->pb_h);
		pere = parent (para->pb_tree, para->pb_obj);
		while (para->pb_tree[pere].ob_type != G_BOX)
			pere = parent (para->pb_tree, pere);
		if (para->pb_tree[pere].ob_spec.obspec.fillpattern == ZERO)
			vsf_interior (work_display.handle, FIS_HOLLOW);
		else if (para->pb_tree[pere].ob_spec.obspec.fillpattern == 7)
			vsf_interior (work_display.handle, FIS_SOLID);
		else
			vsf_interior (work_display.handle, FIS_PATTERN);
		vsf_color (work_display.handle, para->pb_tree[pere].ob_spec.obspec.interiorcol);
		vsf_style (work_display.handle, para->pb_tree[pere].ob_spec.obspec.fillpattern);
		v_bar (work_display.handle, rec);				/* Rectangle */

		vsl_color (work_display.handle, BLACK);
		vsl_width (work_display.handle, 1);
		vsl_ends (work_display.handle, SQUARE, SQUARE);
		vsl_type (work_display.handle, SOLID);
		temp = rec[0] + 2;
		fill_tab (rec, 6,
							temp, para->pb_y + para->pb_h + 1,
							temp + para->pb_w - 1, para->pb_y + para->pb_h + 1,
							temp + para->pb_w - 1, para->pb_y + 2);
		v_pline (work_display.handle, 3, rec);		/* 1ø ligne d'ombre */
		rec[1]++;
		rec[2]++;
		rec[3] = rec[1];
		rec[4]++;
		v_pline (work_display.handle, 3, rec);		/* 2ø ligne d'ombre */

		vsl_color (work_display.handle, WHITE);
		if (para->pb_currstate & SELECTED)
		{
			fill_tab (rec, 6,
								para->pb_x + 1, para->pb_y + para->pb_h - 1,
								para->pb_x + para->pb_w , para->pb_y + para->pb_h - 1,
								para->pb_x + para->pb_w , para->pb_y);
			v_pline (work_display.handle, 3, rec);		/* Ligne blanche */
			vsl_color (work_display.handle, BLACK);
			rec[2] = rec[0];
			rec[3] = para->pb_y;
			rec[4] = rec[2] + para->pb_w - 1;
			rec[5] = rec[3];
		}
		else
		{
			fill_tab (rec, 6,
								para->pb_x, para->pb_y + para->pb_h,
								para->pb_x, para->pb_y,
								para->pb_x + para->pb_w, para->pb_y);
		}
		v_pline (work_display.handle, 3, rec);		/* Ligne */
	}
	else
	{
		vsf_color (work_display.handle, BLACK);				/* Couleur noire */
		vsf_perimeter (work_display.handle, TRUE);	/* Dessiner le p‚rimŠtre */
			/* Dessin de l'ombre */
		vsf_interior (work_display.handle, FIS_SOLID);	/* Fond noir */
		fill_tab (rec, 4,
							para->pb_x + 2, para->pb_y + 2,
							para->pb_x + 2 + para->pb_w, para->pb_y + 2 + para->pb_h);
		v_bar (work_display.handle, rec);				/* Rectangle */
			/* Dessin du bouton */
		if (para->pb_currstate & SELECTED)
			vsf_interior (work_display.handle, FIS_SOLID);	/* Fond noir */
		else
			vsf_interior (work_display.handle, FIS_HOLLOW);	/* Fond blanc */
		fill_tab (rec, 4,
							para->pb_x, para->pb_y,
							para->pb_x + para->pb_w, para->pb_y + para->pb_h);
		v_bar (work_display.handle, rec);				/* Rectangle */
	}

	texte = (char *)para->pb_parm;	/* R‚cup‚rer pointeur sur le texte */
	if (NOT (para->pb_tree[para->pb_obj].ob_flags & BKGR))	/* Si pas Fond */
	{
		if (para->pb_currstate & SELECTED)
			vswr_mode (work_display.handle, MD_XOR);				/* Texte en mode Xor */
		else
			vswr_mode (work_display.handle, MD_TRANS);			/* Texte en mode Transparent */
	}
	else																										/* Si Fond */
		vswr_mode (work_display.handle, MD_TRANS);							/* Texte Transparent */
	vst_effects (work_display.handle, EFNONE);		/* D‚sactiver tous les effets */
	if (para->pb_tree[para->pb_obj].ob_state & DISABLED)	/* Si inactif, */
		vst_effects (work_display.handle, EFGRIS);													/* ‚criture claire */
	vst_alignment (work_display.handle, HCENT, VTOP, &dummy, &dummy);
	pox = para->pb_x + (para->pb_w / 2);					/* Position x */
	if (h > 8)
		pox -= (h / 2);
	else
		pox -= (w / 2);
	poy = para->pb_y + ((para->pb_h - h) / 2);		/* et y. */
	if (NOT (para->pb_tree[para->pb_obj].ob_flags & BKGR))	/* Si pas Fond */
	{
		vst_color (work_display.handle, look.ctxt_popup);
		v_gtext (work_display.handle, pox, poy, texte);						/* Ecrire le texte */
	}
	else																										/* Si Fond */
	{
		pox--;
		poy--;
		if (para->pb_currstate & SELECTED)
		{
			pox++;
			poy++;
		}
		for (i = ZERO ; i < 2 ; i++)
		{
			if (i == ZERO)
				vst_color (work_display.handle, WHITE);
			else
				vst_color (work_display.handle, look.ctxt_popup);
			v_gtext (work_display.handle, pox, poy, texte);						/* Ecrire le texte */
			pox++;
			poy++;
		}
	}


		/* Dessin de l'ic“ne "suivant" */
	if (h > 8)			/* Haute */
	{
		obj = PUH;
		pic.fd_h = msk.fd_h = 15;
	}
	else						/* Basse */
	{
		obj = PUM;
		pic.fd_h = msk.fd_h = 7;
	}

	pic.fd_addr = (void *) bi_trindex[IMG_USER][obj].ob_spec.iconblk->ib_pdata;
	msk.fd_addr = (void *) bi_trindex[IMG_USER][obj].ob_spec.iconblk->ib_pmask;
	pic.fd_w = msk.fd_w = 16;
	pic.fd_wdwidth = msk.fd_wdwidth = 1;
	pic.fd_nplanes = msk.fd_nplanes = 1;
	pic.fd_stand = msk.fd_stand = ZERO;

	fill_tab (rec, 8,
						ZERO, ZERO, pic.fd_w - 1, pic.fd_h - 1,
						para->pb_x + para->pb_w - pic.fd_w, para->pb_y + 1,
						para->pb_x + para->pb_w - pic.fd_w + pic.fd_w - 1,
						para->pb_y + pic.fd_h);

	color[0] = WHITE;
	color[1] = WHITE;
  vrt_cpyfm (work_display.handle, MD_TRANS, rec, &msk, &nul, color);
	color[0] = look.cicn_popup;
  vrt_cpyfm (work_display.handle, MD_TRANS, rec, &pic, &nul, color);

	vs_clip (work_display.handle, FALSE, tab);	/* Clip OFF */
		/* Restaurer attributs */
	vswr_mode (work_display.handle, att[5]);
	vst_font (work_display.handle, att[0]);
	vst_color (work_display.handle, att[1]);
	vst_alignment (work_display.handle, att[3], att[4], &dummy, &dummy);
	vst_height (work_display.handle, work_display.hc,
							&att[6], &att[7], &att[8], &att[9]);
	vsf_interior (work_display.handle, atf[0]);
	vsf_color (work_display.handle, atf[1]);
	vsf_style (work_display.handle, atf[2]);
	vsf_perimeter (work_display.handle, atf[4]);
	return (para->pb_currstate & ~SELECTED);
}
/* #] bpopup () Routine bouton pop-up ou pop-liste :							*/ 
/* #[ bpict () Routine image en formulaire :											*/
	/* MA 26/06/94 : type picture */
int cdecl bpict (PARMBLK *para)
{
int tab[4], rec[8], color[2], mode;
t_image_dial *image;
MFDB nul = {0};
	
	fill_tab (tab, 4,			/* Zone de clip */
						para->pb_xc, para->pb_yc,
						para->pb_wc + para->pb_xc - 1, para->pb_hc + para->pb_yc - 1);
	vs_clip (work_display.handle, TRUE, tab);	/* Clip ON */

	image = (t_image_dial *)para->pb_parm;	/* R‚cup‚rer pointeur sur l'image */
	if (image->mfdb_image.fd_addr != ZERO)	/* si on a un pointeur sur une image	*/
	{
		fill_tab (rec, 8,
							image->cur_pos.g_x, image->cur_pos.g_y,
							image->cur_pos.g_x + para->pb_w - 1,	/* Taille a copier en fonction */
							image->cur_pos.g_y + para->pb_h - 1,	/* de la case correspondante dans le formulaire */
							para->pb_x, para->pb_y,
							para->pb_x + para->pb_w - 1, para->pb_y + para->pb_h - 1);
		if (rec[2] > image->mfdb_image.fd_w)
		{
			rec[2] = para->pb_w;
			image->cur_pos.g_x = rec[0] = rec[2] - image->mfdb_image.fd_w;
		}
		if (rec[3] > image->mfdb_image.fd_h)
		{
			rec[3] = para->pb_h;
			image->cur_pos.g_y = rec[1] = rec[3] - image->mfdb_image.fd_h;
		}
		if (image->mfdb_image.fd_nplanes == 1)
		{
			color[0] = ((para->pb_tree[para->pb_obj].ob_state) & 0x0F00) >> 8;
			color[1] = WHITE;
			mode = ((para->pb_tree[para->pb_obj].ob_state) & 0xF000) >> 12;
			vrt_cpyfm (work_display.handle, mode, rec, &image->mfdb_image, &nul, color);
		}
		else if (image->mfdb_image.fd_nplanes == work_display.n_plane)
			vro_cpyfm (work_display.handle, S_ONLY, rec, &image->mfdb_image, &nul);
	}
	vs_clip (work_display.handle, FALSE, tab);	/* Clip OFF */
	return (para->pb_currstate);
}
/* #] bpict () Routine image en formulaire :											*/ 
/* #[ bmhier () Routine menu hi‚rarchique :												*/
int cdecl bmhier (PARMBLK *para)
{
int tab[4], x, y, w, h, mx, my, ox, oy, dummy,
		posx1, posx2, posy1, posy2, att[10], atf[5];
char *text;

	vqt_attributes (work_display.handle, att);	/* Noter attributs texte */
	vqf_attributes (work_display.handle, atf);	/* Noter attributs remplissage */
	vst_font (work_display.handle, 1);
	text = (char *)para->pb_parm;
	ox = para->pb_tree[parent (para->pb_tree, para->pb_obj)].ob_x;
	oy = para->pb_tree[para->pb_obj].ob_y +
			 para->pb_tree[parent (para->pb_tree, para->pb_obj)].ob_y + bureau.yd;
	vq_mouse (work_display.handle, &dummy, &mx, &my);

	if (para->pb_tree == adr_menu)
		posy1 = (my < oy);
	else
		posy1 = FALSE;
	posy2 = (my >= oy + para->pb_h);

	if (text[strlen (text) - 2] == 3)
	{
		posx1 = (mx < ox + para->pb_w);
		posx2 = (mx < ox);
	}
	else if (text[strlen (text) - 2] == 4)
	{
		posx1 = (mx > ox);
		posx2 = (mx > ox + para->pb_w);
	}


	if ((para->pb_currstate & SELECTED) || (posx1) ||
			(posy1) || (posy2) || (posx2))
	{
		vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &w, &h);

		fill_tab (tab, 4,
							para->pb_x, para->pb_y,
							para->pb_x + para->pb_w - 1,
							para->pb_y + para->pb_h - 1);
		vswr_mode (work_display.handle, MD_REPLACE);			/* Dessin en mode Remplacement */
		vsf_perimeter (work_display.handle, FALSE);
		if (para->pb_currstate & SELECTED)	/* Si option s‚lectionn‚e */
		{
			vsf_color (work_display.handle, BLACK);						/* Couleur noire */
			vsf_interior (work_display.handle, FIS_SOLID);
			vsf_style (work_display.handle, 8);
		}
		else
		{
			vsf_color (work_display.handle, WHITE);							/* Couleur blanche */
			vsf_interior (work_display.handle, FIS_HOLLOW);
		}
		v_bar (work_display.handle, tab);

		vst_color (work_display.handle, BLACK);
		vst_alignment (work_display.handle, HLEFT, VTOP, &dummy, &dummy);	/* Align‚ … gauche en haut */
		vst_effects (work_display.handle, EFNONE);			/* D‚sactiver tous les effets */
		vswr_mode (work_display.handle, MD_XOR);		/* Texte en mode Xor */
		x = para->pb_x;								/* Position en x */
		y = para->pb_y;								/* et en y. */
		v_gtext (work_display.handle, x, y, (char *)para->pb_parm);	/* Ecrire le texte */
	}
	else
		para->pb_tree[para->pb_obj].ob_state |= SELECTED;

		/* Restaurer attributs */
	vswr_mode (work_display.handle, att[5]);
	vst_font (work_display.handle, att[0]);
	vst_color (work_display.handle, att[1]);
	vst_alignment (work_display.handle, att[3], att[4], &dummy, &dummy);
	vst_height (work_display.handle, work_display.hc,
							&att[6], &att[7], &att[8], &att[9]);
	vsf_interior (work_display.handle, atf[0]);
	vsf_color (work_display.handle, atf[1]);
	vsf_style (work_display.handle, atf[2]);
	vsf_perimeter (work_display.handle, atf[4]);
	return ZERO;
}
/* #] bmhier () Routine menu hi‚rarchique :												*/ 
/* #[ bniceline () Routine niceline : 														*/
int cdecl bniceline (PARMBLK *para)
{
int x = para->pb_x + 1, y = para->pb_y,
		w = para->pb_w - 1, h = para->pb_h - 1,
		tab[4], ypos = 3, hb, dummy, att[10];

	vqt_attributes (work_display.handle, att);	/* Noter attributs texte */
	vst_font (work_display.handle, 1);
	vst_height (work_display.handle, work_display.hc, &dummy, &dummy, &dummy, &hb); 	/* BoŒtes caractŠres */
	if (hb > 8)
		ypos = 7;

	vsl_width (work_display.handle, 1);
	vsl_ends (work_display.handle, SQUARE, SQUARE);
	vsl_type (work_display.handle, SOLID);
	vsl_color (work_display.handle, look.cniceline);
	vswr_mode (work_display.handle, MD_REPLACE);			/* Dessin en mode Remplacement */
	fill_tab (tab, 4,
						x - 2, y - 2,
						x + w - 1,y + h - 1);
	vs_clip (work_display.handle, TRUE, tab);						/* Clip ON */
	fill_tab (tab, 4,
						x - 1, y + ypos,
						x + w + 2, y + ypos);
	v_pline (work_display.handle, 2, tab);
	fill_tab (tab, 4,
						x - 2, y + ypos + 1,
						x + w + 1, y + ypos + 1);
	v_pline (work_display.handle, 2, tab);
	fill_tab (tab, 4,
						x - 2, y - 2,
						x + w - 1, y + h - 1);
	vs_clip (work_display.handle, FALSE, tab); 					/* Clip OFF */
		/* Restaurer attributs */
	vswr_mode (work_display.handle, att[5]);
	vst_font (work_display.handle, att[0]);
	vst_color (work_display.handle, att[1]);
	vst_alignment (work_display.handle, att[3], att[4], &dummy, &dummy);
	vst_height (work_display.handle, work_display.hc,
							&att[6], &att[7], &att[8], &att[9]);
	return (para->pb_currstate & ~SELECTED);
}
/* #] bniceline () Routine niceline : 														*/ 

