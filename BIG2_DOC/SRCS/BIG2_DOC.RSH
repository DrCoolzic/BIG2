/* GEM Resource C Source */

#include <portab.h>
#include <aes.h>
#include "BIG2_DOC.H"

#if !defined(WHITEBAK)
#define WHITEBAK    0x0040
#endif
#if !defined(DRAW3D)
#define DRAW3D      0x0080
#endif

#define FLAGS9  0x0200
#define FLAGS10 0x0400
#define FLAGS11 0x0800
#define FLAGS12 0x1000
#define FLAGS13 0x2000
#define FLAGS14 0x4000
#define FLAGS15 0x8000
#define STATE8  0x0100
#define STATE9  0x0200
#define STATE10 0x0400
#define STATE11 0x0800
#define STATE12 0x1000
#define STATE13 0x2000
#define STATE14 0x4000
#define STATE15 0x8000

TEDINFO rs_tedinfo[] =
{ "__",
  "Lignes / page : __",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 19
};

WORD RSBB0DATA[] =
{ 0x01FF, 0xFF80, 0x03FF, 0xFFC0, 
  0x0783, 0xC1E0, 0x0CC6, 0x6330, 
  0x186C, 0x3618, 0x3038, 0x1C0C, 
  0x7FFF, 0xFFFE, 0x3FFF, 0xFFFC, 
  0x1830, 0x0C18, 0x0C10, 0x0830, 
  0x0618, 0x1860, 0x0308, 0x10C0, 
  0x018C, 0x3180, 0x00C4, 0x2300, 
  0x0066, 0x6600, 0x0032, 0x4C00, 
  0x001B, 0xD800, 0x000D, 0xB000, 
  0x0007, 0xE000, 0x0003, 0xC000, 
  0x0001, 0x8000, 0x0000, 0x0000, 
  0x1FFF, 0xFFF8, 0x1FFF, 0xFFF8, 
  0x0000, 0x0000, 0x03E3, 0xC7C0, 
  0x03F3, 0xCFC0, 0x0331, 0x8C00, 
  0x0331, 0x8C00, 0x03E1, 0x8C00, 
  0x03E1, 0x8DC0, 0x0331, 0x8DC0, 
  0x0331, 0x8CC0, 0x03F3, 0xCFC0, 
  0x03E3, 0xC7C0
};

BITBLK rs_bitblk[] =
{ RSBB0DATA,   4,  35,   0,   0, 0x0001
};

BYTE *rs_frstr[] =
{ "[5][ Pas assez de m\202moire ! |  | Supprimez un ou deux| accessoires de bureau.][ Sortie ]",
  "[4][ Le fichier| \042BIG2_DOC.AID\042| est introuvable !][ Sortie ]",
  "[9][  Probl\212me !| Impossible de sauver| la configuration.][ Abandon ]",
  "[8][  |L\047imprimante n\047est| pas pr\212te !][ Annule | Encore ]",
  " BIG2 documentation ",
  "  Doc BIG2",
  "Choisissez le driver d\047imprimante"
};

BITBLK *rs_frimg[] =
{ &rs_bitblk[0]
};

OBJECT rs_object[] =
{ 
  /******** Tree 0 MENU ****************************************************/
        -1,        1,        9, G_IBOX            ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0000, 0x0000, 0x0050, 0x0019,
         9,        2,        2, G_BOX             ,   /* Object 1  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0000, 0x0000, 0x0050, 0x0201,
         1, T_BIG           , T_METHODE       , G_IBOX            ,   /* Object 2  */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0002, 0x0000, 0x0039, 0x0301,
  T_GENE          ,       -1,       -1, G_TITLE           ,   /* Object 3 T_BIG */
  NONE, NORMAL, (LONG)" BIG",
  0x0000, 0x0000, 0x0005, 0x0301,
  T_DEMO          ,       -1,       -1, G_TITLE           ,   /* Object 4 T_GENE */
  NONE, NORMAL, (LONG)" G\202n\202ralit\202s",
  0x0005, 0x0000, 0x000D, 0x0301,
  T_CONCEPTS      ,       -1,       -1, G_TITLE           ,   /* Object 5 T_DEMO */
  NONE, NORMAL, (LONG)" BigDemo",
  0x0012, 0x0000, 0x0009, 0x0301,
  T_FONCTIONS     ,       -1,       -1, G_TITLE           ,   /* Object 6 T_CONCEPTS */
  NONE, NORMAL, (LONG)" Concepts",
  0x001B, 0x0000, 0x000A, 0x0301,
  T_METHODE       ,       -1,       -1, G_TITLE           ,   /* Object 7 T_FONCTIONS */
  NONE, NORMAL, (LONG)" Fonctions",
  0x0025, 0x0000, 0x000B, 0x0301,
         2,       -1,       -1, G_TITLE           ,   /* Object 8 T_METHODE */
  NONE, NORMAL, (LONG)" M\202thode",
  0x0030, 0x0000, 0x0009, 0x0301,
         0,       10,       62, G_IBOX            ,   /* Object 9  */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0000, 0x0301, 0x0050, 0x0017,
        20, FONTE           , QUITTER         , G_BOX             ,   /* Object 10  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0002, 0x0000, 0x0017, 0x0009,
  PRECEDENT       ,       -1,       -1, G_STRING          ,   /* Object 11 FONTE */
  NONE, NORMAL, (LONG)"  Petite fonte      ^F ",
  0x0000, 0x0000, 0x0017, 0x0001,
  SUIVANT         ,       -1,       -1, G_STRING          ,   /* Object 12 PRECEDENT */
  NONE, NORMAL, (LONG)"  Pr\202c\202dent         ^A ",
  0x0000, 0x0001, 0x0017, 0x0001,
  SAUVER          ,       -1,       -1, G_STRING          ,   /* Object 13 SUIVANT */
  NONE, NORMAL, (LONG)"  Suivant           ^Z ",
  0x0000, 0x0002, 0x0017, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14 SAUVER */
  NONE, NORMAL, (LONG)"  Sauver r\202glages   ^S ",
  0x0000, 0x0003, 0x0017, 0x0001,
  SDRIVER         ,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, DISABLED, (LONG)"-----------------------",
  0x0000, 0x0004, 0x0017, 0x0001,
  IMPRIMER        ,       -1,       -1, G_STRING          ,   /* Object 16 SDRIVER */
  NONE, NORMAL, (LONG)"  Choix driver...   ^D ",
  0x0000, 0x0005, 0x0017, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17 IMPRIMER */
  NONE, NORMAL, (LONG)"  Imprimer...       ^I ",
  0x0000, 0x0006, 0x0017, 0x0001,
  QUITTER         ,       -1,       -1, G_STRING          ,   /* Object 18  */
  NONE, DISABLED, (LONG)"-----------------------",
  0x0000, 0x0007, 0x0017, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 19 QUITTER */
  NONE, NORMAL, (LONG)"  Quitter / Fermer  ^Q ",
  0x0000, 0x0008, 0x0017, 0x0001,
        35, FIRSTOB         ,       34, G_BOX             ,   /* Object 20  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0007, 0x0000, 0x0023, 0x000E,
        22,       -1,       -1, G_STRING          ,   /* Object 21 FIRSTOB */
  NONE, NORMAL, (LONG)"  Pourquoi BIG ?                   ",
  0x0000, 0x0000, 0x0023, 0x0001,
        23,       -1,       -1, G_STRING          ,   /* Object 22  */
  NONE, NORMAL, (LONG)"  Que font ces routines ?          ",
  0x0000, 0x0001, 0x0023, 0x0001,
        24,       -1,       -1, G_STRING          ,   /* Object 23  */
  NONE, NORMAL, (LONG)"  Travail sur formulaires et menus ",
  0x0000, 0x0002, 0x0023, 0x0001,
        25,       -1,       -1, G_STRING          ,   /* Object 24  */
  NONE, NORMAL, (LONG)"  Travail sur fen\210tres             ",
  0x0000, 0x0003, 0x0023, 0x0001,
        26,       -1,       -1, G_STRING          ,   /* Object 25  */
  NONE, NORMAL, (LONG)"  Les outils n\202cessaires           ",
  0x0000, 0x0004, 0x0023, 0x0001,
        27,       -1,       -1, G_STRING          ,   /* Object 26  */
  NONE, NORMAL, (LONG)"  Les explications                 ",
  0x0000, 0x0005, 0x0023, 0x0001,
        28,       -1,       -1, G_STRING          ,   /* Object 27  */
  NONE, DISABLED, (LONG)"-----------------------------------",
  0x0000, 0x0006, 0x0023, 0x0001,
        29,       -1,       -1, G_STRING          ,   /* Object 28  */
  NONE, NORMAL, (LONG)"  L\047aide                           ",
  0x0000, 0x0007, 0x0023, 0x0001,
        30,       -1,       -1, G_STRING          ,   /* Object 29  */
  NONE, NORMAL, (LONG)"  Le fichier d\047aide                ",
  0x0000, 0x0008, 0x0023, 0x0001,
        31,       -1,       -1, G_STRING          ,   /* Object 30  */
  NONE, NORMAL, (LONG)"  L\047impression                     ",
  0x0000, 0x0009, 0x0023, 0x0001,
        32,       -1,       -1, G_STRING          ,   /* Object 31  */
  NONE, DISABLED, (LONG)"-----------------------------------",
  0x0000, 0x000A, 0x0023, 0x0001,
        33,       -1,       -1, G_STRING          ,   /* Object 32  */
  NONE, NORMAL, (LONG)"  Historique                       ",
  0x0000, 0x000B, 0x0023, 0x0001,
        34,       -1,       -1, G_STRING          ,   /* Object 33  */
  NONE, NORMAL, (LONG)"  Salut les copains                ",
  0x0000, 0x000C, 0x0023, 0x0001,
        20,       -1,       -1, G_STRING          ,   /* Object 34  */
  NONE, NORMAL, (LONG)"  Pour me contacter                ",
  0x0000, 0x000D, 0x0023, 0x0001,
        53,       36,       52, G_BOX             ,   /* Object 35  */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0014, 0x0000, 0x0019, 0x0011,
        37,       -1,       -1, G_STRING          ,   /* Object 36  */
  NONE, NORMAL, (LONG)"  Vue g\202n\202rale           ",
  0x0000, 0x0000, 0x0019, 0x0001,
        38,       -1,       -1, G_STRING          ,   /* Object 37  */
  NONE, NORMAL, (LONG)"  Premier formulaire     ",
  0x0000, 0x0001, 0x0019, 0x0001,
        39,       -1,       -1, G_STRING          ,   /* Object 38  */
  NONE, NORMAL, (LONG)"  Formulaire complet     ",
  0x0000, 0x0002, 0x0019, 0x0001,
        40,       -1,       -1, G_STRING          ,   /* Object 39  */
  NONE, NORMAL, (LONG)"  Une bo\214te d\047alerte     ",
  0x0000, 0x0003, 0x0019, 0x0001,
        41,       -1,       -1, G_STRING          ,   /* Object 40  */
  NONE, NORMAL, (LONG)"  Formulaire en fen\210tre  ",
  0x0000, 0x0004, 0x0019, 0x0001,
        42,       -1,       -1, G_STRING          ,   /* Object 41  */
  NONE, NORMAL, (LONG)"  Une fen\210tre sp\202ciale   ",
  0x0000, 0x0005, 0x0019, 0x0001,
        43,       -1,       -1, G_STRING          ,   /* Object 42  */
  NONE, NORMAL, (LONG)"  Autre form. en fen\210tre ",
  0x0000, 0x0006, 0x0019, 0x0001,
        44,       -1,       -1, G_STRING          ,   /* Object 43  */
  NONE, NORMAL, (LONG)"  Menu hi\202rarchique      ",
  0x0000, 0x0007, 0x0019, 0x0001,
        45,       -1,       -1, G_STRING          ,   /* Object 44  */
  NONE, NORMAL, (LONG)"  Fen\210tre de texte       ",
  0x0000, 0x0008, 0x0019, 0x0001,
        46,       -1,       -1, G_STRING          ,   /* Object 45  */
  NONE, NORMAL, (LONG)"  Fen\210tre d\047image        ",
  0x0000, 0x0009, 0x0019, 0x0001,
        47,       -1,       -1, G_STRING          ,   /* Object 46  */
  NONE, NORMAL, (LONG)"  Fen\210tre graphique      ",
  0x0000, 0x000A, 0x0019, 0x0001,
        48,       -1,       -1, G_STRING          ,   /* Object 47  */
  NONE, NORMAL, (LONG)"  Fen\210tre avec menu      ",
  0x0000, 0x000B, 0x0019, 0x0001,
        49,       -1,       -1, G_STRING          ,   /* Object 48  */
  NONE, NORMAL, (LONG)"  Pop-up libre           ",
  0x0000, 0x000C, 0x0019, 0x0001,
        50,       -1,       -1, G_STRING          ,   /* Object 49  */
  NONE, NORMAL, (LONG)"  Gestion du timer       ",
  0x0000, 0x000D, 0x0019, 0x0001,
        51,       -1,       -1, G_STRING          ,   /* Object 50  */
  NONE, NORMAL, (LONG)"  Ic\223nification          ",
  0x0000, 0x000E, 0x0019, 0x0001,
        52,       -1,       -1, G_STRING          ,   /* Object 51  */
  NONE, NORMAL, (LONG)"  Bulles d\047aide          ",
  0x0000, 0x000F, 0x0019, 0x0001,
        35,       -1,       -1, G_STRING          ,   /* Object 52  */
  NONE, NORMAL, (LONG)"  Fin de la visite       ",
  0x0000, 0x0010, 0x0019, 0x0001,
        57, M_COBJETS       , M_CFONCTIONS    , G_BOX             ,   /* Object 53  */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x001D, 0x0000, 0x000E, 0x0003,
  M_CFENETRES     ,       -1,       -1, G_STRING          |0x2900,   /* Object 54 M_COBJETS */
  NONE, NORMAL, (LONG)"  Objets    \003 ",
  0x0000, 0x0000, 0x000E, 0x0001,
  M_CFONCTIONS    ,       -1,       -1, G_STRING          |0x2900,   /* Object 55 M_CFENETRES */
  NONE, NORMAL, (LONG)"  Fen\210tres  \003 ",
  0x0000, 0x0001, 0x000E, 0x0001,
        53,       -1,       -1, G_STRING          |0x2900,   /* Object 56 M_CFONCTIONS */
  NONE, NORMAL, (LONG)"  Fonctions \003 ",
  0x0000, 0x0002, 0x000E, 0x0001,
        62, M_FGENE         , M_FDIVER        , G_BOX             ,   /* Object 57  */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0027, 0x0000, 0x000F, 0x0004,
  M_FFORM         ,       -1,       -1, G_STRING          |0x2900,   /* Object 58 M_FGENE */
  NONE, NORMAL, (LONG)"  G\202n\202rales  \003 ",
  0x0000, 0x0000, 0x000F, 0x0001,
  M_FFENE         ,       -1,       -1, G_STRING          |0x2900,   /* Object 59 M_FFORM */
  NONE, NORMAL, (LONG)"  Formulaire \003 ",
  0x0000, 0x0001, 0x000F, 0x0001,
  M_FDIVER        ,       -1,       -1, G_STRING          |0x2900,   /* Object 60 M_FFENE */
  NONE, NORMAL, (LONG)"  Fen\210tres   \003 ",
  0x0000, 0x0002, 0x000F, 0x0001,
        57,       -1,       -1, G_STRING          |0x2900,   /* Object 61 M_FDIVER */
  NONE, NORMAL, (LONG)"  Diverses   \003 ",
  0x0000, 0x0003, 0x000F, 0x0001,
         9, M_SOURCE        , M_CONSEIL       , G_BOX             ,   /* Object 62  */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0032, 0x0000, 0x000E, 0x0002,
  M_CONSEIL       ,       -1,       -1, G_STRING          |0x2900,   /* Object 63 M_SOURCE */
  NONE, NORMAL, (LONG)"  Le source \003 ",
  0x0000, 0x0000, 0x000E, 0x0001,
        62,       -1,       -1, G_STRING          |0x2900,   /* Object 64 M_CONSEIL */
  LASTOB, NORMAL, (LONG)"  Conseils  \003 ",
  0x0000, 0x0001, 0x000E, 0x0001,
  
  /******** Tree 1 C_OBJETS ****************************************************/
        -1,        1,       14, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x001C, 0x000E,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"  Nouveaux types",
  0x0000, 0x0000, 0x001C, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"  Les attributs",
  0x0000, 0x0001, 0x001C, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"  Dessine-moi un bouton",
  0x0000, 0x0002, 0x001C, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"  Les menus pop-up",
  0x0000, 0x0003, 0x001C, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"  Les boutons s\202lectables",
  0x0000, 0x0004, 0x001C, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"  Les \202ditables \202tendus",
  0x0000, 0x0005, 0x001C, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"  D\202placer le formulaire",
  0x0000, 0x0006, 0x001C, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"  Du texte graphique",
  0x0000, 0x0007, 0x001C, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"  Le relief",
  0x0000, 0x0008, 0x001C, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"  Les images en formulaires",
  0x0000, 0x0009, 0x001C, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"  Les menus hi\202rarchiques",
  0x0000, 0x000A, 0x001C, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  NONE, NORMAL, (LONG)"  Les formulaires ToolBar",
  0x0000, 0x000B, 0x001C, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"  Les raccourcis clavier",
  0x0000, 0x000C, 0x001C, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 14  */
  LASTOB, NORMAL, (LONG)"  Les bulles d\047aide",
  0x0000, 0x000D, 0x001C, 0x0001,
  
  /******** Tree 2 C_FENE ****************************************************/
        -1,        1,        4, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x001B, 0x0004,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"  Pr\202sentation g\202n\202rale",
  0x0000, 0x0000, 0x001B, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"  Concept des fen\210tres BIG",
  0x0000, 0x0001, 0x001B, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"  La structure BigWind",
  0x0000, 0x0002, 0x001B, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 4  */
  LASTOB, NORMAL, (LONG)"  Le formulaire-fen\210tre",
  0x0000, 0x0003, 0x001B, 0x0001,
  
  /******** Tree 3 C_FONC ****************************************************/
        -1,        1,        7, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x001C, 0x0007,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"  Coup d\047\264il g\202n\202ral",
  0x0000, 0x0000, 0x001C, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"  Les \202v\202nements",
  0x0000, 0x0001, 0x001C, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"  Le ressource inclus",
  0x0000, 0x0002, 0x001C, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"  Les variables globales",
  0x0000, 0x0003, 0x001C, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"  Les d\202finitions",
  0x0000, 0x0004, 0x001C, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"  Les Cookies",
  0x0000, 0x0005, 0x001C, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 7  */
  LASTOB, NORMAL, (LONG)"  Les fonctions programmeur",
  0x0000, 0x0006, 0x001C, 0x0001,
  
  /******** Tree 4 F_GENERALES ****************************************************/
        -1,        1,       10, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x0012, 0x000A,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"  initial()",
  0x0000, 0x0000, 0x0012, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"  end()",
  0x0000, 0x0001, 0x0012, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"  create_hierm()",
  0x0000, 0x0002, 0x0012, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"  dialog()",
  0x0000, 0x0003, 0x0012, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"  get_bkgr()",
  0x0000, 0x0004, 0x0012, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"  put_bkgr()",
  0x0000, 0x0005, 0x0012, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"  get_string()",
  0x0000, 0x0006, 0x0012, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"  bulle_attach()",
  0x0000, 0x0007, 0x0012, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"  set_interface()",
  0x0000, 0x0008, 0x0012, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 10  */
  LASTOB, NORMAL, (LONG)"  dlg_mouse()",
  0x0000, 0x0009, 0x0012, 0x0001,
  
  /******** Tree 5 F_FORMULAIRES ****************************************************/
        -1,        1,       19, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x0014, 0x0013,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"  formm_draw()",
  0x0000, 0x0000, 0x0014, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"  formm_undraw()",
  0x0000, 0x0001, 0x0014, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"  formf_draw()",
  0x0000, 0x0002, 0x0014, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"  formf_undraw()",
  0x0000, 0x0003, 0x0014, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"  match()",
  0x0000, 0x0004, 0x0014, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"  set_popup()",
  0x0000, 0x0005, 0x0014, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"  create_popup()",
  0x0000, 0x0006, 0x0014, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"  free_popup()",
  0x0000, 0x0007, 0x0014, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"  big_alert()",
  0x0000, 0x0008, 0x0014, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"  form_attach()",
  0x0000, 0x0009, 0x0014, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"  parent()",
  0x0000, 0x000A, 0x0014, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  NONE, NORMAL, (LONG)"  bak_rsc()",
  0x0000, 0x000B, 0x0014, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"  res_rsc()",
  0x0000, 0x000C, 0x0014, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14  */
  NONE, NORMAL, (LONG)"  set_radio ()",
  0x0000, 0x000D, 0x0014, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"  get_radio ()",
  0x0000, 0x000E, 0x0014, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16  */
  NONE, NORMAL, (LONG)"  set_attributes ()",
  0x0000, 0x000F, 0x0014, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"  objc_xedit()",
  0x0000, 0x0010, 0x0014, 0x0001,
        19,       -1,       -1, G_STRING          ,   /* Object 18  */
  NONE, NORMAL, (LONG)"  set_text()",
  0x0000, 0x0011, 0x0014, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 19  */
  LASTOB, NORMAL, (LONG)"  get_text()",
  0x0000, 0x0012, 0x0014, 0x0001,
  
  /******** Tree 6 F_FENETRES ****************************************************/
        -1,        1,       19, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x0013, 0x0013,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"  formw_draw()",
  0x0000, 0x0000, 0x0013, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"  open_window()",
  0x0000, 0x0001, 0x0013, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"  find_index()",
  0x0000, 0x0002, 0x0013, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"  find_window()",
  0x0000, 0x0003, 0x0013, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"  zone_work()",
  0x0000, 0x0004, 0x0013, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"  height_sliders()",
  0x0000, 0x0005, 0x0013, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"  posi_xxxx()",
  0x0000, 0x0006, 0x0013, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"  height_xxxx()",
  0x0000, 0x0007, 0x0013, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"  draw_object()",
  0x0000, 0x0008, 0x0013, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"  print_page()",
  0x0000, 0x0009, 0x0013, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"  next_wind()",
  0x0000, 0x000A, 0x0013, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  NONE, NORMAL, (LONG)"  closed()",
  0x0000, 0x000B, 0x0013, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"  iconise()",
  0x0000, 0x000C, 0x0013, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14  */
  NONE, NORMAL, (LONG)"  uniconise()",
  0x0000, 0x000D, 0x0013, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"  alliconise()",
  0x0000, 0x000E, 0x0013, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16  */
  NONE, NORMAL, (LONG)"  set_work()",
  0x0000, 0x000F, 0x0013, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"  set_windows()",
  0x0000, 0x0010, 0x0013, 0x0001,
        19,       -1,       -1, G_STRING          ,   /* Object 18  */
  NONE, NORMAL, (LONG)"  bwind_calc()",
  0x0000, 0x0011, 0x0013, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 19  */
  LASTOB, NORMAL, (LONG)"  draw_title ()",
  0x0000, 0x0012, 0x0013, 0x0001,
  
  /******** Tree 7 F_DIVERSES ****************************************************/
        -1,        1,       16, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x0011, 0x0010,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"  path()",
  0x0000, 0x0000, 0x0011, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"  exist()",
  0x0000, 0x0001, 0x0011, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"  set_palette()",
  0x0000, 0x0002, 0x0011, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"  timer()",
  0x0000, 0x0003, 0x0011, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"  trim()",
  0x0000, 0x0004, 0x0011, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"  send_mesag()",
  0x0000, 0x0005, 0x0011, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"  selector()",
  0x0000, 0x0006, 0x0011, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"  extension()",
  0x0000, 0x0007, 0x0011, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"  rc_intersect()",
  0x0000, 0x0008, 0x0011, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"  clipp()",
  0x0000, 0x0009, 0x0011, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"  first_cookie()",
  0x0000, 0x000A, 0x0011, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  NONE, NORMAL, (LONG)"  next_cookie()",
  0x0000, 0x000B, 0x0011, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"  vq_cookie()",
  0x0000, 0x000C, 0x0011, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14  */
  NONE, NORMAL, (LONG)"  vq_cookie_c()",
  0x0000, 0x000D, 0x0011, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"  fill_tab()",
  0x0000, 0x000E, 0x0011, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 16  */
  LASTOB, NORMAL, (LONG)"  empty()",
  0x0000, 0x000F, 0x0011, 0x0001,
  
  /******** Tree 8 SOURCE ****************************************************/
        -1,        1,       10, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x0019, 0x000A,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"  Introduction",
  0x0000, 0x0000, 0x0019, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"  Le ressource",
  0x0000, 0x0001, 0x0019, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"  Le source",
  0x0000, 0x0002, 0x0019, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"  Fonction main()",
  0x0000, 0x0003, 0x0019, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"  Image de bureau",
  0x0000, 0x0004, 0x0019, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"  Gestion formulaires",
  0x0000, 0x0005, 0x0019, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"  Formulaires en fen\210tre",
  0x0000, 0x0006, 0x0019, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"  La fen\210tre texte",
  0x0000, 0x0007, 0x0019, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"  La fen\210tre image",
  0x0000, 0x0008, 0x0019, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 10  */
  LASTOB, NORMAL, (LONG)"  La fen\210tre graphique",
  0x0000, 0x0009, 0x0019, 0x0001,
  
  /******** Tree 9 CONSEILS ****************************************************/
        -1,        1,        6, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x001C, 0x0006,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"  BIG en multit\203che",
  0x0000, 0x0000, 0x001C, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"  Pointeurs sur fonctions",
  0x0000, 0x0001, 0x001C, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"  Ressource int\202gr\202",
  0x0000, 0x0002, 0x001C, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"  Accessoires de bureau",
  0x0000, 0x0003, 0x001C, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"  Ic\223nification de fen\210tres",
  0x0000, 0x0004, 0x001C, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 6  */
  LASTOB, NORMAL, (LONG)"  Copier, Couper, Coller",
  0x0000, 0x0005, 0x001C, 0x0001,
  
  /******** Tree 10 PRINT ****************************************************/
        -1,        1, P_ANN           , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021000L,
  0x0000, 0x0000, 0x0018, 0x0007,
  P_NBREL         ,       -1,       -1, G_BUTTON          |0x1300,   /* Object 1  */
  FLAGS10, STATE8|STATE10|STATE11, (LONG)"Impression",
  0x0002, 0x0001, 0x0014, 0x0001,
  P_OK            ,       -1,       -1, G_FTEXT           ,   /* Object 2 P_NBREL */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[0],
  0x0003, 0x0003, 0x0012, 0x0001,
  P_ANN           ,       -1,       -1, G_BUTTON          |0x1E00,   /* Object 3 P_OK */
  SELECTABLE|DEFAULT|EXIT|FLAGS10, NORMAL, (LONG)"[Imprime",
  0x0002, 0x0005, 0x0009, 0x0001,
         0,       -1,       -1, G_BUTTON          |0x1F00,   /* Object 4 P_ANN */
  SELECTABLE|EXIT|LASTOB|FLAGS10, NORMAL, (LONG)"[Annule",
  0x000D, 0x0005, 0x0009, 0x0001
};

OBJECT *rs_trindex[] =
{ &rs_object[0],   /* Tree  0 MENU             */
  &rs_object[65],   /* Tree  1 C_OBJETS         */
  &rs_object[80],   /* Tree  2 C_FENE           */
  &rs_object[85],   /* Tree  3 C_FONC           */
  &rs_object[93],   /* Tree  4 F_GENERALES      */
  &rs_object[104],   /* Tree  5 F_FORMULAIRES    */
  &rs_object[124],   /* Tree  6 F_FENETRES       */
  &rs_object[144],   /* Tree  7 F_DIVERSES       */
  &rs_object[161],   /* Tree  8 SOURCE           */
  &rs_object[172],   /* Tree  9 CONSEILS         */
  &rs_object[179]    /* Tree 10 PRINT            */
};
