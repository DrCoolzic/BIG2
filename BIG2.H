/*! @file big2.h
@brief This is the main include file for the BIG library

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

@todo Description for BIG2.H file

*/

#ifndef _BIG_H_
#define _BIG_H_

#ifdef WIN32							/* allow to compile in Windows environment */
#define cdecl							/* used in AES.H */
#pragma warning(disable : 4996)			/* disable deprecated function */
#pragma warning(disable : 4018)			/* disable signed/unsigned mismatch */
#pragma warning(disable : 4129)			/* disable unrecognized character escape sequence */
#endif

#include <STRING.H>
#include <AES.H>
#include <VDI.H>
#include <TOS.H>
#include <STDIO.H>
#include <STDLIB.H>
#include <CTYPE.H>

/*! @addtogroup const_group
@{ */

#if defined(LIGHT)
#define dialog dialog_light			/*!< Dialog light processing without timer */
#endif

#define	FALSE	0					/*!< FALSE = 0 */
#define TRUE	1					/*!< TRUE = 1 */
#define NOT !						/*!< NOT='!' */
#define BLANK -1					/*!< Defined to -1. Used a lot in BIG! */
#define ZERO 0						/*!< instead of 0 */
#define MAX_ALRT 250				/*!< Maximum size for an alert string */

#ifndef max		/* not present in stdlib for Pure C */
#define max(a, b)((a)>(b)?(a):(b))	/*!< compute max value of two numbers */
#define min(a, b)((a)<(b)?(a):(b))	/*!< compute min value of two numbers */
#endif

/* Some compilers do not define color icons */
#if !defined( G_CICON )
#define G_CICON 33

typedef struct cicon_data {
	int num_planes;
	int* col_data;
	int* col_mask;
	int* sel_data;
	int* sel_mask;
	struct cicon_data* next_res;
} CICON;

typedef struct cicon_blk {
	ICONBLK monoblk;
	CICON* mainlist;
} CICONBLK;
#endif

/* in case iconify messages not defined */
#if !defined( WM_ICONIFY )
#define WM_ICONIFY 34
#define WM_UNICONIFY 35
#define WM_ALLICONIFY 36
#endif

/* in case send window to bottom not defined */
#define WM_CYCLED 38
#if !defined( WM_BOTTOM )
#define WM_BOTTOM 33
#endif

#define NGAD	23		/*!< Number of BIG object windows  */
#define BIT15	0x8000	/*!< Test bit 15 set */

/* BIG new extended types */
#define B_MOVE		17	/*!< Button used to move a form on the screen */
#define B_SELEC		18	/*!< Used to define B_CHECKB and B_ROUNDB buttons */
#define B_GRASTR	19	/*!< Character String with Graphical Attribute */
#define B_FRAME		20	/*!< Frame with text used to group objects like radio button */
#define B_HELP		21	/*!< Help button called without quiting current form */
#define B_POPUP		22	/*!< Button to access associated popup */
#define B_LIST		23	/*!< Button to access popup list with slider */
#define B_PICT		24	/*!< To place an MFDB picture in a form */
#define B_EDIT		25	/*!< extended editable Text field */
#define B_SMSTR		26	/*!< Small Character String with Graphical Attribute */
#define B_UNDER		30	/*!< Button that can have an underlined character */
#define B_UNDO		31	/*!< Normal button also activated by pressing the Undo key */
#define B_HIERM		41	/*!< Menu option that calls a hierarchical menu */
#define B_FNCP		51	/*!< Callback function associated to button or menu option */

/* Usual event bit mask */
#define DLG_DESK	(MU_KEYBD | MU_BUTTON | MU_M1 | MU_MESAG | MU_TIMER)
#define DLG_FORM	(MU_KEYBD | MU_BUTTON | MU_TIMER)

/* BIG new events */
#define BEV_HIERM	150	/*!< MU_MESAG Event option of a hier. menu selected */
#define BEV_FREEPU	151	/*!< MU_BUTTON Event option of a "free" popup selected */
#define BEV_WFORM	152	/*!< MU_BUTTON Event exit object of a form selected */
#define BEV_WMENU	153	/*!< MU_BUTTON Event menu option in a window selected */
#define	BEV_WHIER	154	/*!< MU_BUTTON Event option hier. menu in a window selected */
#define BEV_TOOL	155	/*!< MU_BUTTON Event toolbar object in a window selected */

/* definitions of a set of shortcuts for windows attributes */
#define WATR_ALLG	(NAME|CLOSER|FULLER|MOVER|INFO|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE)
#define WATR_ALLB	(CLOSER|FULLER|MOVER|INFO|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE|CYCLER|SMALLER|ALLSIZER)
#define WATR_CURRG	(NAME|CLOSER|FULLER|MOVER|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE)
#define WATR_CURRB	(CLOSER|FULLER|MOVER|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE|CYCLER|SMALLER|ALLSIZER)
#define WATR_FORMG	(NAME|CLOSER|MOVER)
#define WATR_FORMB	(CLOSER|MOVER|CYCLER|SMALLER)

/* define the BIG windows types*/
#define WTYP_NORM	1		/*!< "normal" window contain probably text. */
#define WTYP_FORM	2		/*!< form window. BIG takes care of all. */
#define WTYP_PICT	3		/*!< Picture window. BIG takes care of all. */
#define WTYP_TOOL	0x4000	/*!< Window with Toolbar */
#define WTYP_MENU	0x8000	/*!< Window with Menu  */

/* Flags used for BIG windows */
#define WFFULL	0x0001		/*!< set when window is "fulled" */
#define WFARROW	0x0002		/*!< when set allow window operation from keyboard */
#define WFCALAG	0x0004		/*!< slider & win size multiple of shift unit */
#define WFDECAL 0x0008		/*!< if set w_lin & w_col are updated on win resize */		
#define WFGROUP	0x0010		/*!< if set all slider arrows in bottom right corner */ 
#define WFLAST	0x0080		/*!< Last window indicator (like LASTOBJ) */

#if !defined( SMALLER )
#define SMALLER		0x4000	/*!< Smaller attribute in BIG window */
#endif

#define CYCLER		0x2000	/*!< Cycler attribute in BIG window */
#define ALLSIZER	0x8000	/*!< Allsizer attribute in BIG window */

#define BKGR		0x0400	/*!< used for 3D (bit 10 of ob_flags) */
#define DCDEFAULT	0x0800	/*!< activate def button on dbl click (bit 11 ob_flags) */	
#define WFCLOSE		0x1000	/*!< button close form in window (bit 12 ob_flags) */
#define BS_RIGHT	0x2000	/*!< right justification in B_SELEC (bit 13 ob_flags) */
#define AIDE_OBJET	0x4000	/*!< bubble-help on object Bit (14 ob_flags) */
#define NOTOP		0x8000	/*!< ops to window-form not in front (bit 15 ob_flags) */

/* For menus in windows */
#define WM_BKGR 0			/*!< ??? @todo */
#define WM_BOXMENU 1		/*!< ??? @todo */
#define WM_BOXARROWS 2		/*!< ??? @todo */
#define WM_LFARROW 3		/*!< ??? @todo */
#define WM_RTARROW 4		/*!< ??? @todo */
#define WM_BOXTITLES 5		/*!< ??? @todo */

/* keyboard define */
#define HELP		0x6200	/*!< HELP key */
#define UNDO		0x6100	/*!< UNDO key */
#define ENTER		0x1C0D	/*!< ENTER key */
#define RETURN		0x720D	/*!< RETURN key */
#define BACKSPC		0x0E08	/*!< BACKSPACE key */
#define DELETE		0x537F	/*!< DELETE key */
#define SH_DELETE	0x537F	/*!< SHIFT + DELETE key */
#define INSERT		0x5200	/*!< INSERT key */
#define SH_INSERT	0x5230	/*!< SHIFT + INSERT key */
#define TAB			0x0F09	/*!< TAB key */
#define ARDN		0x5000	/*!< ARROW_DOWN key */
#define ARUP		0x4800	/*!< ARROW_UP key */
#define ARLF		0x4B00	/*!< ARROW_LEFT key */
#define ARRT		0x4D00	/*!< ARROW_RIGHT key */
#define CT_ARDN		0x5000	/*!< CTRL + ARROW_DOWN key */
#define CT_ARUP		0x4800	/*!< CTRL + ARROW_UP key */
#define CT_ARLF		0x7300	/*!< CTRL + ARROW_LEFT key */
#define CT_ARRT		0x7400	/*!< CTRL + ARROW_RIGHT key */
#define SH_ARDN		0x5032	/*!< SHIFT + ARROW_DOWN key */
#define SH_ARUP		0x4838	/*!< SHIFT + ARROW_UP key */
#define SH_ARLF		0x4B34	/*!< SHIFT + ARROW_LEFT key */
#define SH_ARRT		0x4D36	/*!< SHIFT + ARROW_RIGHT key */
#define ESC			0x011B	/*!< ESC key */
#define CLR			0x4700	/*!< CLR key */
#define CT_CLR		0x7700	/*!< CTRL + CLR key */
#define SH_CLR		0x4737	/*!< SHIFT + CLR key */
#define ESPACE		0x3920	/*!< SPACE key */
#define F1			0x3B00	/*!< F1 key */
#define SH_F1		0x5400	/*!< SHIFT + F1 key */
#define F2			0x3C00	/*!< F2 key */
#define SH_F2		0x5500	/*!< SHIFT + F2 key */
#define F3			0x3D00	/*!< F3 key */
#define SH_F3		0x5600	/*!< SHIFT + F3 key */
#define F4			0x3E00	/*!< F4 key */
#define SH_F4		0x5700	/*!< SHIFT + F4 key */
#define F5			0x3F00	/*!< F5 key */
#define SH_F5		0x5800	/*!< SHIFT + F5 key */
#define F6			0x4000	/*!< F6 key */
#define SH_F6		0x5900	/*!< SHIFT + F6 key */
#define F7			0x4100	/*!< F7 key */
#define SH_F7		0x5A00	/*!< SHIFT + F7 key */
#define F8			0x4200	/*!< F8 key */
#define SH_F8		0x5B00	/*!< SHIFT + F8 key */
#define F9			0x4300	/*!< F9 key */
#define SH_F9		0x5C00	/*!< SHIFT + F9 key */
#define F10			0x4400	/*!< F10 key */
#define SH_F10		0x5D00	/*!< SHIFT + F10 key */

/* shortcuts */
#define ADR			ir_trindex	/*!< Shortcut see ir_trindex */
#define get_popup	match	/*!< Shortcut see get_popup() */

/* VDI.H complementary definitions */
#define POINTIL		0x8888	/*!< define dotted line for vsl_type() & vsl_udsty() */

/* pattern shape (vsm_type) */
#define MTPOINT		1		/*!< point */
#define MTPLUS		2		/*!< plus */
#define MTETOILE	3		/*!< star */
#define MTCARRE		4		/*!< square */
#define MTCROIX		5		/*!< cross */
#define MTLOSANGE	6		/*!< diamond */

/* effect attribute (vst_effects) */
#define EFNONE		0x00	/*!< None */
#define EFGRAS		0x01	/*!< bold */
#define EFGRIS		0x02	/*!< grayed */
#define EFITAL		0x04	/*!< italic */
#define EFSOULI		0x08	/*!< underline */
#define EFCONT		0x10	/*!< contour */

/* justification attribute (vst_alignment) */
#define HLEFT		0		/*!< left */
#define HCENT		1		/*!< center */
#define HRIGHT		2		/*!< right */
#define VDOWN		0		/*!< down */
#define VLINE		3		/*!< base */
#define VTOP		5		/*!< top */

/* size attribute (vst_height) */
#define HNORM		13		/*!< norm */
#define HPETIT		6		/*!< small */
#define HMINUS		4		/*!< tiny */

/* rotation attribute (vst_rotation) */
#define RONORM		0		/*!< 0 deg. */
#define RO90		900		/*!< 90 deg. */
#define RO180		1800	/*!< 180 deg. */
#define RO270		2700	/*!< 270 deg. */

/*! @} */

/*! @addtogroup typedef_group
@{ */

typedef int Palette[3];		/*!< used by ::bpal[16] */
typedef void (*FNCP)(void);	/*!< function pointer type */

/*! structure used for forms in windows */
typedef struct form {
	OBJECT	*w_tree;		/*!< Tree address */
	int		w_edit;			/*!< current editable */
	int		w_pos;			/*!< cursor position */
	char	*w_bak;			/*!< for saving state of a form */
} Form;

/*! structure used to define the content of a window */
typedef union cont {
	Form	w_form;			/*!< if form */
	MFDB	w_img;			/*!< if image */
	char*	w_adr;			/*!< if text or other data */
} Cont;

/*! define data before iconification */
typedef struct oldw {
	GRECT	w_coord;		/*!< old coordinate */
	int		w_att;			/*!< old attribute */
} Oldw;

/*! Definition of BIG Windows */
typedef struct bigwind {
	int		w_hg;			/*!< GEM Handle: positive=GEM, -1=init, 0=closed */
	int		w_type;			/*!< BIG type of windows */
	int		w_attr;			/*!< GEM Window attribute */
	GRECT	w_curr;			/*!< window current coordinates */
	Oldw	w_old;			/*!< coordinate before iconification */
	int		w_wmini;		/*!< minimum width */
	int		w_hmini;		/*!< minimum Height */
	int		w_wmaxi;		/*!< maximum width */
	int		w_hmaxi;		/*!< maximum Height */
	int		w_wunit;		/*!< horizontal shift unit */
	int		w_hunit;		/*!< vertical shift unit */
	int		w_hprot;		/*!< number of protected points on left */
	int		w_vprot;		/*!< number of protected points on top */
	Cont	w_cont;			/*!< vary according to window content */
	Palette	*w_pal;			/*!< Pointer to the associated pallet */
	OBJECT	*w_bar;			/*!< window with menu/toolbar => tree address */
	OBJECT	w_fen[NGAD];	/*!< windowed form */
	int		w_flags;		/*!< BIG Windows Flags  */
	int		w_mouse;		/*!< mouse pointer shape in the window */
	int		w_icon;			/*!< iconification : associated  window index */
	FNCP	w_redicn;		/*!< Function pointer redraw if win iconified */
	FNCP	w_cycle;		/*!< Function pointer display next window */
	FNCP	w_redraw;		/*!< Function pointer win redraw */
	FNCP	w_top;			/*!< Function pointer bring win to top */
	FNCP	w_close;		/*!< Function pointer close win */
	FNCP	w_move;			/*!< Function pointer move win */
	FNCP	w_size;			/*!< Function pointer resize win */
	FNCP	w_full;			/*!< Function pointer full size win */
	FNCP	w_hslid;		/*!< Function pointer slider horizontal */
	FNCP	w_vslid;		/*!< Function pointer slider vertical */
	FNCP	w_uppage;		/*!< Function pointer one page up */
	FNCP	w_dnpage;		/*!< Function pointer one page down */
	FNCP	w_lfpage;		/*!< Function pointer one page left */
	FNCP	w_rtpage;		/*!< Function pointer one page right */
	FNCP	w_upline;		/*!< Function pointer one line up */
	FNCP	w_dnline;		/*!< Function pointer one line down */
	FNCP	w_lfline;		/*!< Function pointer one line left */
	FNCP	w_rtline;		/*!< Function pointer one line right */
	char	*w_title;		/*!< window title pointer */
	char	*w_infos;		/*!< window info pointer */
	long	w_wtot;			/*!< max window content width (in pixels) */
	long	w_htot;			/*!< max window content Height (in pixels) */
	long	w_lin;			/*!< window first line */
	long	w_col;			/*!< window first column */
} BigWind;

/*! used for images in form MA 26/06/1994 */
typedef struct t_image_dial	{
	MFDB	mfdb_image;		/*!< MFDB image to display */
	GRECT	cur_pos;		/*!< position of the image */
} t_image_dial;

/*! structure for cookies */
typedef struct {
	long ident;				/*!< Cookie identifier */
	union {	
		long l;				/*!< if data is type long */
		int i[2];			/*!< if data is type int */
		char c[4];			/*!< if data is type char */
	} datack;				/*!< Cookie data */
} COOKIE;

/*! Atari model used */
typedef enum {
	ST,						/*!< Atari ST Model */
	STE,					/*!< Atari STE Model */
	TT,						/*!< Atari TT Model */
	FALCON_030,				/*!< Atari Falcon 30 Model */
	AUTRE_MACHINE			/*!< Unknown model */
} t_machine;

/*! Graphical processor used */
typedef enum {
	PG_ST,					/*!< ST graphical processor */
	PG_STE,					/*!< STE graphical processor */
	PG_TT,					/*!< TT graphical processor */
	PG_FALCON_030,			/*!< Falcon graphical processor */
	AUTRE_PG				/*!< Unknown graphical processor */
} t_proc_graphique;

/*! CPU used */
typedef enum {	
	PROC_68000,				/*!< 68000 Processor */
	PROC_68030,				/*!< 68030 Processor */
	AUTRE_PROCESSEUR		/*!< Unknown Processor */
}	t_cpu;

/*! Resolution used */
typedef enum {
	ST_BASSE = 0,			/*!< low resolution */
	ST_MOYENNE = 1,			/*!< medium resolution */
	ST_HAUTE = 2,			/*!< high resolution */
	VGA_16COL = 4,			/*!< TT VGA */
	VGA_256COL = 5,			/*!< FALCON VGA */
	R_256C_320_480 = 7,		/*!< TT 320*480 16 col */
	TRUE_COLOR_320_480 = 8,	/*!< Falcon 320*480 VGA */
	INCONNU = -1			/*!< Unknown resolution */
} t_resolution;

/*! type of GDOS used */
typedef enum {
	AUCUN_GDOS	=0,			/*!< No GDOS running */
	GDOS_STANDARD	=1,		/*!< Standard GDOS running */
	FONT_GDOS	=2,			/*!< Font GDOS running */
	FSM_GDOS	=3,			/*!< FSM GDOS running */
	SPEEDO_GDOS	=4			/*!< Speedo GDOS running */
} type_gdos;

/*! Hardware / Software information */
typedef struct {
	t_machine la_machine;	/*!< Atari model */
	t_proc_graphique proc_graphique;	/*!< Graphical processor used */
	t_cpu le_cpu;			/*!< CPU used */
	int v_aes;				/*!< AES version in hex e.g. "0x410" is V4.10 */
	int v_tos;				/*!< TOS version in hex same as above */
	char multitache;		/*!< TRUE if multitasking */
	type_gdos le_gdos;		/*!< GDOS used */
} t_ident_hard_soft;

/*! Display Information */
typedef struct t_display {
	size_t	taille;			/*!< Screen size in number of bytes */
	size_t	taille_ligne;	/*!< line size in number of bytes */
	int		handle;			/*!< VDI virtual workstation handle */
	int		w;				/*!< Screen width in pixels */
	int		h;				/*!< Screen Height in pixels */
	int		hc;				/*!< "Normal" Character Height from baseline to top-line */
	t_resolution res;		/*!< resolution at application start */
	int		mode;			/*!< Current mode if running on Falcon */
	char	une_palette;	/*!< FALSE no pallet (NEAR_TRUE_COLOR or TRUE_COLOR) */
	long	n_color_pal;	/*!< pallet or system number of color */
	long	n_color;		/*!< number of pen available simultaneously */
	int		n_plane;		/*!< screen depth in number of plans */
	Palette* palette;		/*!< Save pallet Pointer */
	int		nb_bit_coul[3];	/*!< number of bits per color */
	int		masque_coul[3];	/*!< Color mask */
	void*	phys;			/*!< Physical address of screen at startup */
	void*	log;			/*!< Logical address of screen at startup */
} t_display;

/*! desktop coordinate */
typedef struct t_desktop
{
	int xd;					/*!< X top corner of desktop */ 
	int yd;					/*!< Y top corner of desktop */ 
	int wd;					/*!< width of desktop */ 
	int hd;					/*!< height of desktop */ 
} t_desktop;

/*! @} */

/*! @addtogroup global_group
@{ */

/* Global Variables */
extern int ap_id;			/*!< AES application identifier */
extern int buf[8];			/*!< Events buffer */
extern int mx;				/*!< Mouse X Position */
extern int my;				/*!< Mouse Y Position */
extern int mk;				/*!< Mouse state */
extern int edit;			/*!< Current Editable */
extern int pos;				/*!< Position cursor in editable */
extern int object;			/*!< Object clicked for MU_BUTTON */
extern int kbd;				/*!< State control key */
extern int key;				/*!< Current KB event */
extern int clik; 			/*!< number of mouse clicks */
extern int wind;			/*!< GEM handle of clicked window in working area */
extern int intgr;			/*!< True if resource is internal */
extern char pathapp[];		/*!< Application path at startup */
extern Palette	bpal[16];	/*!< Internal 16 color palette */
extern OBJECT* adr_menu;	/*!< Reserved address for menu form */
extern OBJECT* adr_desk;	/*!< Reserved address for desktop */
extern OBJECT** ir_trindex;	/*!< Address of all forms in program */
extern int cliq_in;			/*!< Validate option selection */ 
extern GRECT zclip;			/*!< clipping zone for redraw */
extern FNCP* fnc;			/*!< Pointer to function pointer array */
extern BigWind* win;		/*!< Pointer to the windows array */
extern t_ident_hard_soft desc_hard_soft; /*!< Hard/Soft information */
extern t_display work_display;	/*!< Display information */
extern t_desktop bureau;	/*!< Desktop information */
extern int aide_en_ligne;	/*!< Bubble-help -1=active; 0=never; other=kbshift mask */

/*! @} */

/*! @addtogroup control_group
@{ */

/* Control Functions */

/*! @brief Initializes the BIG Library
@param	rsc		pointer to a string which contains the filename of the resource to 
				load, or an empty string for an integrated resource.
@param	menu	is the tree number (in the resource) which will be taken as 
				top menu of the application. In the case of a desktop accessory 
				this parameter is ignored. If one wishes to create a program 
				without menu, it is necessary to transmit "-1" in this parameter 
@param	desk	is the tree number (in the resource) which will be taken as 
				desktop of the application. In the case of a desktop accessory 
				this parameter is ignored. If one wishes to create a program 
				without desktop, it is necessary to transmit "-1" in this parameter 
@param	nb_tree	must be equal to 0 if the resource use an external file.
				If the program works with an integrated resource, nb_tree indicates 
				the number of trees (menu and forms) of its resource.
@param	rs_tree	is a special pointer. In the case of an integrated resource, 
				it make it possible to indicate to BIG where in memory begin the 
				tables "rs_trindex[]" created by file RSH. If not, it point on ZERO. 
				See in the chapter "Working with an integrated resource the way of 
				using it.
@param	rs_str	is a special pointer. In the case of an integrated resource, 
				it make it possible to indicate to BIG where in memory begin the 
				tables "rs_frstr[]" created by file RSH. If not, it point on ZERO. 
				See in the chapter "Working with an integrated resource the way of 
				using it.
@param	n_wind	total number of BIG windows used by the program.
@param	acc		Number in the resource of the free string to use for the call of 
				the function menu_register() if the application is a desktop 
				(name of the accessory in the desktop menu). If it is a program, 
				this parameter is ignored, one can transmit "-1".
@return			TRUE if everything went well, FALSE otherwise (for example, if the 
				resource file was not found).

This function declares the application, opens the workstation, initializes 
the majority of the global variables, loads the resource (if it is external, 
if not adapts the integrated resource to the current resolution), sets up the 
resource included of BIG, sets up the storage areas for window management, 
of the userdef objects, of extended editable, of the pointers on function, 
of the pallet, reserves the memory for window management by BIG, initializes 
certain elements of the BigWind structures, etc...

@note The variables "adr_menu" and "adr_desk" have fixed names to facilitate 
their management. Indeed, there are many functions which make call to these 
addresses, and to transmit them each time would be tiresome and not very 
practical. If your program works with several menu bars (or even several 
desktops - why not), the address of the current menu must be in adr_menu. 
The function centers in advance all the forms and empty all the editable 
fields. You can reach the windows structures (and their elements) as 
elements of a table, by win[index].w_adr for example 
(numbering starts obviously at zero).\n
It is strongly recommended not to save up with the quantity of windows 
necessary, and to declare a window by data element to be treated.
For example, it is not advised to have only one window to display a text or an 
image, with the excuse that both will never be displayed at the same time. 
It is, in this case, better to have two distinct windows, the parameters contained 
in their elements being different. In the same way, declare a window for each form 
in window. The management of the program will be simplified by doing so, even if 
it means to close a window before opening another one if both are not displayed at 
the same time.\n
A BigWind structure represents only 158 bytes in memory, whereas a complicated 
management of few windows sharing different contents is likely to take much 
more, without speaking of processing time.\n
This function must IMPERATIVELY be called by the main() function of the program 
at the beginning of the execution. */
int		initial(char* rsc, int menu, int desk, int nb_tree,
				OBJECT* rs_tree, char* rs_str, int n_wind, int acc);

/*! @brief Terminates and cleans BIG library

This function closes all the windows of the program, it releases the various 
storage areas which were reserved by initialization with initial() and in the 
course of program, it releases the resource, it closes the workstation, it 
restores the palette and other parameters of display and it leaves the program.

@note This function is not called by BIG. It is your program which will have to 
take care of this call when the user asks to leave the application.
If a storage area is assigned to the contents of a window, it must be released. 
If the closing function assigned to the element win[index].w_close
of this window does not take care of it, you will have to do it \b BEFORE calling 
end(). */
void	end(void);

/*! @} */

/*! @addtogroup form_group
@{ */

/****** Forms Management Functions *******/

/*! @brief Display a movable form
@param	adr		Address of the form to be displayed.
@param	ed		Object Number of the editable field in which the cursor must be 
				placed at the beginning of the management of the form. The cursor 
				is placed at the end of the field. -1 if there is no editable.
@param	flmove	Move flag: If this flag is FALSE, the displacement of the form is 
				done as a "phantom box". If it is TRUE, the function reserve a 
				memory area and saves the screen there in order to carry out 
				displacement in real-time. If it is not possible to reserve this 
				memory, displacement is done with a phantom box.
@param	img		Pointer to a MFDB used to save the background under the form.
@return			Nothing

This function displays a movable form 

@note To remove the form from the screen at the end of management, it is 
imperatively necessary to use the function formm_undraw(). */
void	formm_draw(OBJECT* adr, int ed, int flmove, MFDB* img);

/*! @brief Erase a movable form
@param	adr		Address of the tree.
@param	img		Pointer to the MFDB in which the background of the form was saved.
@return			Nothing

This function erases a movable form which was displayed by the function formm_draw()
form from the screen.

@note The movable form should have been displayed imperatively by the function 
formm_draw(). */
void	formm_undraw(OBJECT* adr, MFDB* img);

/*! @brief Display a traditional form
@param	adr		Address of the tree to be displayed.
@param	ed		Object Number of the editable field, if there is one, where to place 
				the cursor at the beginning of management or -1 if no editable. The 
				cursor is placed at the end of the field.

Display a traditional form
@note This function exists only for reasons of compatibility and to be complete, 
but the function formm_draw() should preferably be used. The form displayed with 
this function must be removed from the screen with formf_undraw() function. */
void	formf_draw(OBJECT* adr, int ed);

/*! @brief Erase a traditional form
@param	adr		Address of the tree.
@return			Nothing

This function erases the screen for a fixed form which was displayed by the 
function formf_draw().

@note The function must be used in complement of formf_draw() function. */
void	formf_undraw(OBJECT* adr);

/*! @brief Place an arrow in a menu
@param	adr		Address of the menu tree.
@return			Nothing

This function seeks the B_HIERM objects of a menu and places in the last but 
one character of the string an arrow towards the right or left according to 
where the hierarchical menu will unroll

@note With regard to the top menu of an application, the function initial() is 
given the responsibility to call this function. It is also called automatically 
with each time a click takes place on a title of of menu in windows, because the 
window could have been moved, and the sub-menus could then have to be unrolled 
on the other side. But if you wish to change the menu bar (and if the new one 
contains some B_HIERM), you must call this function during the first display 
of this bar. At the time of the later redraw, or reactivation of the first bar, 
it is not necessary. The menu options of the type B_HIERM must be filled with 
spaces on all their width. The last but one character is replaced by an arrow 
(ASCII 3 or 4) placed by this function. */
void	create_hierm(OBJECT* adr);

/*! @brief Manage all the events in BIG
@param	flags	It is the bits mask, the same one as for the GEM function 
				evnt_multi(), who indicates which events must be supervised. 
				BIG proposes several defines for the usual situations:
				::WATR_ALLG, ::WATR_ALLB, ::WATR_CURRG, ::WATR_CURRB, ::WATR_FORMG,
				::WATR_FORMB
@param	address	Address of the form or the desktop to be work on.
@param	f		Flag. If this flag is TRUE, the function supervises the menu, 
				the desktop, the windows, etc If it is FALSE, it is limited to 
				the form indicated by address, considering that the other events 
				are blocked by wind_update().
@param	fl_rb	Flag Right button. If TRUE, a right click of a button of the mouse 
				calls a free pop-up menu (see following parameter).
@param	form_pu	Pop-Up Form. Tree number for the form used as a free pop-up in the 
				event of click on the right button. -1 if there is not. To reserve 
				for work on the desktop.
@param	img		In the case of a movable form, it is a pointer to the MFDB which 
				contains the background of this form. This parameter is not used 
				if one work on the desktop, but nevertheless a MFDB must be transmitted.
@param	fl_move	Indicates if the form is removable (TRUE) or not (FALSE). 
				Also FALSE if the function manages the desktop.
@return			The function turns over a bit mask corresponding to the type of event 
				which occurred. This mask is the same one as for the GEM function 
				evnt_multi(). Here is a reminder the bit mask of bits used by 
				evnt_multi() and dialog():
				- Bit 0: MU_KEYBD (keyboard event)
				- Bit 1: MU_BUTTON (mouse click event)
				- Bit 2: MU_M1 (mouse area 1 event)
				- Bit 3: MU_M2 (mouse area 2 event)
				- Bit 4: MU_MESAG (menu or window message event)
				- Bit 5: MU_TIMER (timer event)

This function is the heart of BIG. It is the main loop which scans the events and 
manages them mainly. It will be always called at least once in a program under BIG.

@note The function dialog() also performs, thanks to the timer, the monitoring of 
several things: windows in foreground to activate the corresponding palette even on 
intervention of another application, bubble-help, hierarchical menu and right-click 
of the mouse. This permanent monitoring involves a certain slow down, particularly 
obvious on the old 8 MHz machines. It is possible to "shutdown" these functions by 
typing in the source of your program the line:
@code
#define LIGHT
#include "big2.h"
@endcode
The definition must be placed \b BEFORE the include. Obviously, the 
mentioned effects also disappear. No more bubbles-help, no more menus hierarchical, 
etc. (but the hierarchical menus for menus in window are always active). But in a 
program which does not use them, it is to better shut them down (it is for example 
the case of the program BIG_DOC). However, the processing of the color palette 
continue to be active since it is at the time when a window is brought to the 
foreground that BIG activates the pallet which corresponds to this window. Simply, 
this management will no more take into account the windows belonging to other 
applications. */
int		dialog(int flags, OBJECT* address, int f,
			   int fl_rb, int form_pu, MFDB* img, int fl_move);

/*! @brief Search for a text match  in pop-up pop-list
@param	adr		Address of the form in which the button is.
@param	button	Button Number in this form.
@return			Object Number in the pop-up form if an occurrence is found, 0 if not.
`
This function seeks if there is a correspondence between the text (or the pattern 
of image or box) for a pop-up button or pop-list and one of the objects of its 
associated pop-up form.

@note The root-object of the pop-up form having the number 0, it is not possible
to have an occurrence with this object number. The function thus return 0 if no 
correspondence is found. */
int		match(OBJECT* adr, int button);

/*! @brief Link Text to pop-up
@param	adr		Address of the form that contain the button
@param	button	Button Number in this form.
@param	option	Object Number of the option in the pop-up menu.
@return			Nothing

This function connects the text (or the pattern of image or box) for a pop-up 
menu option to the pop-up or pop-list button associated. */
void	set_popup(OBJECT* adr, int button, int option);

/*! @brief Create text pop-up menu
@param	nbre	It is the number of character strings intended to become 
				the options of the pop-up.
@param	list	Pointer to the beginning of the string table. They must imperatively 
				be consecutive in memory while being separated by a null byte.
@return			The address of the reserved area which is from now on the pop-up 
				menu or zero in the event of problem.

This function makes it possible to create a text pop-up menu in memory  
from a list of character string.
@note This function is extremely practical to create a pop-up menu when you do not 
know in advance (at the creation of the resource) the number of options or the text. 
For example, pop-up allowing for choice of a SpeedoGDOS font. The pop-up created can 
be used only as a free pop-up free, with the function free_popup(). Indeed,
"ordinary" pop_up would have to be related to a form button, which would contain 
into its ob_state the number of the sub-menu. But the pop-up created does not have 
a number! In addition, one should not forget to release the memory which the function 
has reserved, like this:
@code
	adr = create_popup(20, list);
	free_pop_up(x, y, BLANK, adr);
	free(adr);
@endcode
The memory reservation is made with the C function malloc() and not the one from
GEMDOS, which has lots of bugs. The release of memory must thus be also made by 
C function. If some options must be "deactivated" or "checked", the programmer 
will have to take care of it. For example: adr[3].ob_flags |= HIDETREE; 
deactivate the third option (the object numbered 0 is the root). */
OBJECT*	create_popup(int nbre, char* list);

/*! @brief Call a free pop-up
@param	posx	X Coordinate of the center of the pop-up. It will be possibly 
				shifted if it were to leave the screen.
@param	posy	Y Coordinate of the center of the pop-up. It will be possibly 
				shifted if it were to leave the screen.
@param	tree	Number of the tree serving as pop-up, or "-1" if one prefers
				to transmit the address directly.
@param	adr		If the preceding parameter is set to "-1", addresses of the pop-up 
				form. If not, this parameter is ignored.
@return			TRUE if an option was selected, FALSE if the pop-up was left by 
				clicking outside.

This function calls a "free" pop-up form, in the sense that its position is not 
related to the position of a button in a form.

@note If an object has been chosen, TRUE is returned and an event is set up by 
the function in the buffer of events. It is of type ::BEV_FREEPU.
If the clicked object is of  type function pointer, the function is 
automatically carried out before the return of the function.
Although this function is called internally by the function dialog() 
(see the parameter "fl_rb" of dialog()), you may need to call it directly in 
certain situations. */
int		free_popup(int posx, int posy, int tree, OBJECT* adr);

/*! @brief Saves the image of an area in a MFDB
@param	of_x	X offset Coordinate of the Image area to save
@param  of_y	Y offset Coordinate of the Image area to save
@param	of_w	Width offset of the Image area to save
@param	of_h	Height offset of the Image area to save
@param	img		Pointer to the MFDB which will describe the saved area.
@return			nothing

This function saves the image of an area in a MFDB, in order to restore it later. 
It makes it possible to put "aside" the bottom of a form or pop-up, for example. 
This bottom will be restored by the function put_bkgr().

@note This function is called internally by BIG, but you can use it to save area 
of screen of necessary. See also function put_bkgr(). */
void	get_bkgr(int of_x, int of_y, int of_w, int of_h, MFDB* img);

/*! @brief Restore the image of an area from a MFDB
@param	of_x	X offset Coordinate of the Image area to restore
@param  of_y	Y offset Coordinate of the Image area to restore
@param	of_w	Width offset of the Image area to restore
@param	of_h	Height offset of the Image area to restore
@param	img		Pointer to the MFDB which will be used to restore the saved area.
@return			nothing

This function restores the image of an area which was saved in a MFDB, by the 
function get_bkgr().

@note This function is called internally by BIG, but you can use it to restore 
zones of screen if necessary. See also function get_bkgr(). */
void	put_bkgr(int of_x, int of_y, int of_w, int of_h, MFDB* img);

/*! @brief Call and manage a BIG Alert
@param	button	Number of the default button: 0, 1, 2, or 3.
@param	number	Number (in the resource) of the free string containing the text 
				of the alert box, with the same format as for the GEM function 
				form_alert(). If that is more appropriate for the situation, the 
				string can be indicated directly in the following parameter. In this
				case number must be equal to -1 (BLANK).
@param	str		If one wishes to directly transmit the string, it must set the 
				preceding parameter is -1 and str is then a pointer on the alert 
				string. Otherwise the string is empty.
@param	fl_alrt	If this flag is TRUE, alert will be managed by BIG, like a movable 
				form. If it is FALSE, it will be a normal GEM alarm, called by 
				form_alert().
@return:		Number (1, 2 or 3) of the button which was clicked, as in GEM function

This function calls and manages a BIG alert box, i.e. a movable form. The call and 
the string of alert are similar to their GEM counterparts. Alert can contain up to 
5 lines of 31 characters maximum, and three buttons of 20 characters maximum.

@note BIG proposes 9 alert icons. If the string contains 0 as the icon number 
the alert will be displayed without icon. Numbers 1 to 9 correspond to the 
following icons:
	- 1: Exclamation point
	- 2: Question mark
	- 3: Stop Panel 
	- 4: prohibited Direction Panel 
	- 5: Open hand
	- 6: Letter "I", like Information
	- 7: Bomb (for the error messages)
	- 8: Printer
	- 9: Diskette
	.
If one works with 16 colors or more, the alert box is displayed in 3D, the bottom 
being gray (color number 8). If not, it is displayed with a white background, 
without 3D. */
int		big_alert(int button, int number, char* str, int fl_alrt);

/*! @brief Return a pointer to a free string
@param	number	Number of the free string in the resource (given in the file 
				header created by the resource).
@return			Pointer on the string in memory.

This function returns a pointer on a free string (or on a free alert) of an 
internal or external resource. */
char*	get_string(int number);

/*! @brief Link pop-up object and a sub-menu
@param	tree		Number of the tree containing the object.
@param	obj			Number of the object concerned.
@param	formular	Number of the sub-menu (or help form) to assign to the object.
@return				Nothing

This function creates the link between an object of the pop-up type, B_POPUP, 
B_LIST, B_HELP or B_HIERM and the sub-menu (or help form) who must be affected to it.

@note It is possible to create the link manually at the time of the creation of 
the resource. However, that is rather tedious and especially, if new tree structures 
are inserted later on, the work must be remade. It is thus more convenient to use 
a series of calls to this function at the beginning of the program in order to 
create these links by using the names of the trees and objects. */
void	form_attach(int tree, int obj, char formular);

/*! @brief Create a link between an object an a free string of a bubble-help
@param	tree	Number of the tree containing the object.
@param	obj		Number of the object concerned.
@param	bulle	Number of the free string to assign to the object as a text of 
				the bubble.
@return			Nothing

This function create the link between an object and the free string which must 
be affected to him as a bubble-help.

@note	It is possible to create the link "manually" at the time of the creation 
of the resource. However, that is rather tedious and especially, if new strings 
are inserted later on, the work must be remade. It is thus more convenient to 
make series of calls to this function at the beginning of the program in order 
to create these links by using the names of the trees, objects and strings.
The function also set the bit AIDE_OBJET of the concerned  object in order to 
announce that a bubble-help is affected to it. The function is without effect 
for the objects which cannot receive bubble-help (see the chapter on the 
bubble-help) */
void	bulle_attach(int tree, int obj, char bulle);

/*! @brief Modify aspect parameters of BIG object
@param	under		Flag. If it is TRUE, the character will be underlined in 
					objects ::B_SELEC, ::B_UNDO and ::B_UNDER. If it is FALSE, 
					the character will not be underlined.
@param	cunder		Color Index of the underlined character.
@param	ctxt_under	Color index of the remainder of the text for the objects 
					already mentioned above.
@param	cicn_coche	Color Index of the icon for B_CHECKB buttons.
@param	ctxt_coche	Color index of the text for B_CHECKB buttons.
@param	cicn_radio	Color Index of the icon for B_RADIOB buttons.
@param	ctxt_radio	Color index of the text for B_RADIOB buttons.
@param	cicn_popup	Color Index of the icon for B_POPUP & B_LIST buttons.
@param	ctxt_popup	Color Index of the text for B_POPUP & B_LIST buttons.
@param	cmove		Color index of the lines for B_MOVE objects.
@param	cgrastr		Color index of the text for B_GRASTR objects.
@param	clin_frame	Color index of the frame for B_FRAME objects.
@param	ctxt_frame	Color index of the text for B_FRAME objects.
@param	cniceline	Color index of the lines for B_NICELINE objects.

This function makes it possible to modify certain parameters concerning the aspect 
of BIG objects.
@note If  "-1" is transmitted to an element, the corresponding adjustment is 
unchanged. By default, the character is underlined (under == TRUE), it is in 
red (cunder == RED), and all the other parameters are set to "black" (BLACK).
The numbers of the patterns correspond to those used by VDI function 
vsf_style(). The design for the pattern is thus not limited to the 7 models of 
the AES, but can take one of the 24 patterns provided by the VDI. The adjustments 
affect all the corresponding objects of all the forms of the application.\n
The function tests if there is at the time of the call some window-forms or
windows with toolbar opened. If it finds some, these windows are entirely redrawn 
to bring up to date the new adjustments. Therefore, this function should never be 
called when the GEM is "freezed" by a blocking form. */
void	set_interface(int under, int cunder, int ctxt_under, int cicn_coche, 
					  int ctxt_coche, int cicn_radio, int ctxt_radio, 
					  int cicn_popup, int ctxt_popup, int cmove, int cgrastr,
					  int clin_frame, int ctxt_frame, int cniceline);

/*! @brief Return the number of the parent object
@param	adr		Address of the form.
@param	object	number of the object which one seeks the parent.
@return			Parent object Number

This function return the parent object number of an object. */
int cdecl parent(OBJECT* adr, int object);

/*! @brief Save the state of a Form
@param	tree	Address of the form.
@param	bak		Pointer on a pointer of string which is used as a backup buffer 
				by the function. This buffer is an storage area reserved with the 
				size necessary by the function. It will be released by res_rsc()
				if called, or will be the responsibility of programmer if not.
@return			Nothing

This function backs up in a buffer the state of a form, i.e. the buttons selected 
or not, the text of editable, and the pop-up, etc, in order to restore them by 
the function res_rsc() if the button 'Cancel' is clicked.

@note The call of this function must be made by the programmer before drawing the 
form (by formm_draw() for example). In a form in window, it is necessary to call 
this function in the event of click on the button 'Apply' in order to record the 
new state. Then do not forget to precede this call with Mfree(adr_bakrsc) 
(where adr_bakrsc is the address of the buffer), if not, an equivalent size would 
be reserved for each time, without being released. See also the function res_rsc(). */
void	bak_rsc(OBJECT* tree, char** bak);

/*! @brief Restore the state of a Form
@param	tree	Address of the form.
@param	bak		Pointer on a pointer of string which is used as a restore buffer 
				by the function. This buffer is an storage area that has been 
				reserved with the size necessary by the function bak_rsc(). The
				memory is released by res_rsc().
@return			Nothing

This function restores the state of a form saved by the function bak_rsc().

@note Warning: If the management of the form is left by 'Cancel', the programmer 
must call this function to restore the form state. The storage area is then 
released. If management is left by 'Confirm', it is the programmer who must 
undertake the release of the storage area by: Mfree(adr_bakrsc)
If not, the storage area would remain allocated. See also the function bak_rsc(). */
void	res_rsc(OBJECT* tree, char** bak);

/*! @brief Change mouse pointer in working area
@return			Nothing

This function tests the position of the mouse compared to the working area of 
the window in the foreground and changes possibly its shape.

@note It is useful to call this function in the following situation: a window 
is opened, and the mouse must take a shape determined in its working area. The 
window size is changed by the program, without the user directly changing its
dimensions. For example, the window displays a series of tables. By a keyboard 
shortcut, the user requires to pass in the following table, the window size 
is automatically adjusted by the program to the dimensions of the new table. 
At the end of the process, if the mouse is in the working area, it is necessary 
to activate the shape envisaged. If not, it is necessary to activate the default
arrow shape. This operation is carried out by the call to this function. */
void	dlg_mouse(void);

/*! @brief Select a radio button in a group
@param	adr		Address of the form.
@param	rb		Object Number of the radio-button to be selected.
@return			Nothing

This function selects a radio-button in a group of radio-buttons and deselect 
all the others.

@note The function only select the specified radio-button it does not redraw 
anything. It must thus be called before the display of the form or the program 
must take care of redrawing the parent-object of the group, which cause to also
redraw all the radio-button. */
void	set_radio(OBJECT* adr, int rb);

/*! @brief Find the selected radio-button in a group
@param	adr		Address of the form.
@param	rb		Object Number of any of the radio-buttons in the group
@return			The object number of selected radio-button

This function seeks which radio-button is selected in a group. */
int		get_radio(OBJECT* adr, int rb);

/*! @brief Set graphical attributes of B_GRASTR or B_SMSTR
@param	adr		Address of the form containing the object.
@param	ob		Object Number.
@param	attr	Bit Mask of the wanted attributes, the same bit mask as in 
				the vst_effects() function.
@return			Nothing

This function is used to change the graphic attributes of objects of type 
B_GRASTR and B_SMSTR.

@note The object is not redrawn. The function must thus be called before the 
display of the form or a redrawn must be executed. */
void	set_attributes(OBJECT* adr, int ob, int attr);

/*! @} */

/*! @addtogroup editable_group
@{ */

/***** BIG Extended Editable Functions *****/

/*! @brief BIG equivalent to GEM objc_edit() function.
@param	adr		Address of the form where the editable is located.
@param	car		Character to be registered in the field (such as transmited 
				by evnt_multi()). 0 (ZERO) for removing or initialize the cursor.
@param	ed		Object number of the editable field.
@param	po		pointer on the position of the cursor in the field (will be 
				possibly modified).
@param	action	Action to be realized, as for objc_edit():
				- ED_INIT the cursor is placed at the end of the field.
				- ED_END the cursor is removed.
				- ED_CHAR the character is processed.
@param	f		Flag. TRUE if working on a form in window, FALSE if not.
@param	index	BIG Index of the form in window, if it there one. -1 (BLANK) otherwise.
@return			Nothing

This function replaces the call to the GEM objc_edit() function. This is necessary 
because of the existence in BIG of the small characters editable fields as, that GEM 
does not handle correctly. But it can also be used also with the ordinary editable. 
It is therefore recommended to systematically use it in the place of the GEM function. 
It is used in a similar way.

@note As with the GEM function, the cursor is drawn in Xor mode. 
The use of value ED_END action parameter is thus made same manner, the cursor is alternatively put or removed at each call. With ED_INIT, it is always put, but 
always at the end of the field. BIG treats the ' te_pvalid' string filter correctly, 
and even adds 3 new filter characters:
- 'x' All the characters are authorized, but they are automatically converted in 
capital letter. This filter existed already in GEM, but was not nowhere documented. 
BIG formalizes it and ensures its operation on all the systems.
- 'H' All hex figures (0-9) + (a-f) + (A-F).
- 'S' Number (0-9) or '+' or '-' signs. Practical for the editable where a positive 
or negative value can be entered. The first character of the "te_pvalid" filter must 
be a 'S' in this case.
.
The editable fields can comprise one or more fixed character(s) in the middle of 
the string "te_ptmplt". For example, For example if the string is specified as 
"Price: ____.__" the point is a fixed character (referred as "mark" in the following 
explanation). If the data entry cursor is before the mark and that the user types a 
point ("."), the cursor jumps to the position located just after the mark, and the 
interval is filled by spaces. But that is made only if the mark is an isolated 
character. Thus, let us consider the field "Hour: __h__m__s". If the cursor is in 
the first part of the data entry and that the user types a 'h', the jump of the 
cursor is carried out. But if the field is "Hour: __Hr__Mn__Se", the jump will 
not be made. Note that the GEM function objc_edit() works in the same way.
Note also that there is a bug in the GEM function objc_draw(): If the first 
character of the string "te_ptext" is an "@", the field remains empty when 
display. However, the cursor is correctly placed, and the "te_ptext" string is 
normal in the memory. */
void	objc_xedit(OBJECT* adr, int car, int ed, int* po, int action, int f, int index);

/*! @brief Place text in an object
@param	adr		Address of the form.
@param	object	Object number.
@param	string	Pointer on the string to be placed in the object.
@return			Nothing

This function places a text in an object of the type G_STRING, G_BUTTON, 
G_TEXT, G_BOXTEXT, G_FTEXT, G_FBOXTEXT or G_USERDEF (in this case, 
BIG objects that contain a text).

@sa get_text() */
void	set_text(OBJECT* adr, int object, char* string);

/*! @brief Get text in an object
@param	adr		Address of the form.
@param	object	Object number.
@return			Pointer on the string in the object.

This function returns a pointer on the text of an object of the type 
G_STRING, G_BUTTON, G_TEXT, G_BOXTEXT, G_FTEXT, G_FBOXTEXT or G_USERDEF 
(in this case, BIG objects having a text).

@note In the case of a B_EDIT object, it is a pointer on the beginning 
of the string of data entry which is return. See also the function set_text(). */
char*	get_text(OBJECT* adr, int object);

/*! @} */

/*! @addtogroup utility_group
@{ */

/****** Utility Functions *******/

/*! @brief Get the default path of the application
@param	pat	is a pointer on a string which will contain in return the default path. 
			It must be of sufficient size.
@return		A pointer on the string. 


This function returns the path by default of the application (the path where to 
seek the resource and the setup file, by example).


@note the function places the path in the variable of return "pat", and returns 
a pointer on this string. This function exists for reasons of compatibilities with 
the old versions of BIG. From now on, the path of the application is in the 
variable ::pathapp. */
char*	path(char* pat);

/*! @brief Return size of a file if it exist
@param	name	is a pointer on the string of the filename to be search, with 
				possibly the indication of the path. Jokers * and ? can be used.
@param	att		is a bit mask representing the File attributes of the field to seek,
				according to GEMDOS rule.
@return			The function returns the size of the file (in bytes) of the first 
				found occurrence, FALSE if it does not find of it (or if the file 
				as a null size).

If a particular file exists, turns over its size, if not turns over FALSE (0).

@note If the file is a folder, the function turns over TRUE or FALSE. */
long	exist(char* name, int att);

/*! @brief New value for the pallet
@param	index	is BIG Index of the window which has the address of the new
				pallets in its structure. If "-1" is transmitted for the index, 
				the default pallet is restored.

This function fixes new values for the palette.

@note You can call this function directly, for example to restore the default pallet
at the request of the user. */
void	set_palette(int index);

/*! @brief New value for the pallet
@return		Value of the timer.

This function returns the value of the 200Hz timer at the time when it is called.

@note This function is very useful to be used for example as countdown or to 
measure time intervals.*/
long	timer(void);

/*! @brief Trim tab/spaces at begin/end of a string
@param	str		Pointer on the string to be processed.
@return			Pointer on the position of the string in memory.

This function eliminates spaces and the tabulations from beginning and end of a 
string. The transmitted string is modified in memory, and a pointer indicating 
location is returned by the function. */
char*	trim(char* str);

/*! @brief Send message to itself
@param	type	Type of event (GEM or BIG). It will be placed in word 0 of the buffer 
				of event.
@param	wi		Will be placed in word 3 of the buffer. Often, it is the GEM handle 
				of a window.
@param	x		will be placed in word 4 of the buffer. Often, it is a X coordinate.
@param	y		Will be placed in word 5 of the buffer. Often, it is a Y coordinate. 
@param	w		will be placed in word 6 of the buffer. Often, it is a width of area.
@param	h		will be placed in word 7 of the buffer. Often, it is a height of area.

This function makes it possible the application to be sent to itself a message, so 
as to simulate an event.

@note It is sometimes very useful to be sent to oneself messages, to generate a 
redraw for example. */
void	send_mesag(int type, int wi, int x, int y, int w, int h);

/*! @brief Call GEM File selector
@param	pat		Pointer on a string indicating the access path to be opened in the 
				selector, without the extension to be filtered. After the function, 
				points on the path chosen in the selector (the string can be rather 
				long).
@param	ext		Extension (or file names) to filter in selector. Jokers * and ? are 
				authorized. For example "\*.DOC" or "\BIG*.*".
@param	file	Pointer on a string containing the filename to preselect in the 
				selector, or empty string if there is no default file. After the 
				function, the string (which must be enough long) contains the 
				selected filename.
@param	title	Pointer on the string to be displayed in the selector of object, 
				starting from the TOS 1.4.
@return			TRUE if the selector is left with button OK, FALSE if it is left 
				with the cancel button.

This function calls and manages the selector of GEM objects.

@note The function tests the model number of the TOS and carries out a call to
fsel_input() (TOS former to 1.4) or fsel_exinput() (TOS 1.4 or more). Like the 
GEM function, selector() returns 0 if Cancels were selected.
- If the selector is called to choose a file, it is necessary to test:
@code
	if (selector(path, ext, name, title) == TRUE && strlen(name))
@endcode
In order to check that the button OK was clicked and that a file name indeed 
was selected (the user could confirm without choosing file).
- If the selector is only called to require of the user a path (the filename 
does not have importance in this case), it is enough to test:
@code
	if(selector(path, "\*.*", name, title) == TRUE)
@endcode
.
The main difference between this function and the direct call of the GEM 
function is that the access path returned by this function after the call 
is stripped from the extension filter. For example, to reach the file chosen 
by the user, it is enough to concatenate the two strings by strcat(path, name); 
to obtain its full path in path. */
int		selector(char* pat, char* ext, char* file, char* title);

/*! @brief Forces a specific extension on file or path name
@param	filename	Pointer on the filename or the full path.
@param	ext			Extension. Pointer on the extension to be imposed. The 
					extension must be transmitted with the point, for example .DOC. 
					It is possible to transmit an empty string ("") to remove 
					the extension.

This function imposes an extension to a file name or an access path

@note This function is extremely practical for example to be certain that a file 
will be saved with a precise extension. After data entry of the file name in a 
selector by the user, this function can be systematically invited so as to force 
file extension.\n
It can also be useful in the case of a program managing a series of files placed 
in the same path and all of the same name, of which only the extension changes 
(typical case of the databases). Before each access to a file, the desired extension 
is imposed. */
void	extension (char* filename, char* ext);

/*! @brief Find common zone between two areas of screen
@param	p1	Structure GRECT describing the coordinates of the first of both zones.
@param	p2	Structure GRECT describing the coordinates of the second of both zones. 
		The data of this structure will be modified by the function.
@return		Zero if there is no common part, non null value if there is one of them.

Find if there is a common part between two zones of the screen.

@note the common part between the two areas is described after the call in the 
parameter p2 which is used as variable of return. Values which were there are lost.
This function is used for the processing of the redraw messages, where it is necessary 
to determine if the working area of a given window is in the zone to be redrawn.
Under Lattice C, the function becomes brc_intersect(). */
int		rc_intersect (GRECT* p1, GRECT* p2);

/*! @brief Turn on/off clipping of a screen area
@param	clip_flag	True if the clipping must be activated, FALSE if it must be 
					deactivated.
@param	area		Pointer on a structure GRECT describing the zone of screen 
					concerned.

This function activates or decontaminates the clipping of a zone of the screen.

@note This function is used for the processing of the messages of redraw, to be 
sure that the drawing in progress will not overflow of the zone on which one works. */
void	clipp (int clip_flag, GRECT* area);

/*! @brief First Cookie of the cookie jar
@param	pxy		Start address of the table.
@param	n		number of elements of the table.
The following parameters, of number N, are the values to be placed successively 
in the elements of the table. They are imperatively of int type.

This function fills a table with integers. */
void	fill_tab (int* pxy, int n, ...);

/*! @brief Empty keyboard or message buffer
@param	evnmt	This parameter is MU_KEYBD (1) to empty the keyboard buffer, 
				or MU_MESAG (16) to empty the message buffer. All the other 
				values are ignored.

This function waits until either the keyboard buffer, or the message buffer 
(in global variable buf) is empty and relinquishes control only at this time. */
void	empty (int evnmt);

/*! @} */

/*! @addtogroup cookie_group
@{ */

/***** Cookie Functions *****/

/*! @brief First Cookie of the cookie jar
@return		Pointer on the Cookie

This function returns a pointer on first Cookie of the cookie jar, or a null pointer 
if there is not. */
COOKIE*	first_cookie (void);

/*! @brief Next Cookie of the cookie jar
@return		Pointer on the Cookie

This function returns a pointer on next Cookie of the cookie jar, or a null pointer 
if there is no more.

@note This function is used in collaboration with first_cookie(). */
COOKIE*	next_cookie (COOKIE* ck);

/*! @brief Query for a Cookie by id
@param	id	Pointer on the preceding cookie
@return		Pointer on Cookie.

This function seeks a Cookie with a given identifier id, and returns a pointer on this 
Cookie, or a null pointer if it is not found.

@note This function uses first_cookie() and next_cookie(). */
COOKIE*	vq_cookie (long id);

/*! @brief Query for a Cookie by name
@param	c		Pointer on the character string who definite the cookie.
@return			Pointer on Cookie.

This function seeks Cookie with a given identifier name, and returns a pointer 
on this Cookie, or a null pointer if it is not found.

@note This function uses vq_cookie(). */
COOKIE*	vq_cookie_c (char* c);

/*! @} */

/*! @addtogroup window_group
@{ */

/***** BIG Windows Functions *****/

/*! @brief Open a Form in Window
@param	adr		Address of the form.
@param	index	Index BIG of the window to be opened.
@param	title	Number (in the resource) of the free string containing the window 
				title (- 1 if there is not).
@param	infos	Number (in the resource) of the free string containing the text of 
				the line of information of the window (- 1 if there is not).
@param	wed		Object number of the editable field where the cursor must be placed 
				at the beginning of management (- 1 if there is not).
@param	attr	Attributes of the window.
@param	ricn	Routine of drawing of the contents of the window to be used if it 
				is iconified. If equal to 0, it will be the normal routine which 
				will be always used.
@return:		TRUE if the window was correctly creates and open, FALSE in the event 
				of problem.

This function opens a form in window, and places the form there.

@note The function search to see if a button of displacement of form (B_MOVE), 
and hides it (HIDETREE) if it finds one of them. This for the case where the same 
form would be used by the program in in window or not. The programmer should not 
forget to make visible again this button if the form must then be displayed normally. 
In addition, the root box is equipped with an edge of a pixel towards the outside, 
and a possible attribute OUTLINED is canceled. As all the forms are centered by 
default under BIG (by the function initial()), it is with this position that the 
working area of window will be by default. But it is possible to make it open 
elsewhere by modifying the position of the form before calling the function, for 
example with:
@code
	adr->ob_x = new_x;
	adr->ob_y = new_y;
@endcode
The management of the window by the keyboard is always activated if there is no 
editable field, and always canceled if not. Let us recall that under BIG a 
form in window some with editable fields should not be placed in a window with 
sliders or box of re-dimensioning (see chapter Another form in window some in 
the guided visit).\n
If the window were already opened, then closed again, its old coordinates are 
used. The state of the form is backed up by bak_rsc() in the zone indicated by 
the element win[index].w_cont.w_form.w_bak.\n
If there are already too many opened windows, the function is given the responsibility 
to display the corresponding alert box of the resource included in BIG. The minimum 
and maximum size limits of the working area of the window are obviously those of 
the form. */
int		formw_draw(OBJECT* adr, int index, int title, int infos,
				   int wed, int attr, FNCP ricn);

/*! @brief Create and open a Window
@param	index	Index BIG of the window to be opened.
@param	type	type BIG of the window to be opened.
@param	attr	GEM Attributes of the window to be opened.
@param	x		Position in X of the upper-left angle of the window to be opened. 
				It is about the window itself, not  the working area.
@param	y		Position in of the upper-left angle of the window to be opened there. 
				It is about the window itself, not only the working area.
@param	w		Width of the window to be opened. It is about the window itself, 
				not only the working area.
@param	h		Height of the window to be opened. It is about the window itself, 
				not only the working area.
@param	wmi		Width minimum which the window during its management will be able 
				to take. It is about the window itself, not only the zone of work.
@param	hmi		Height minimum which the window during its management will be able 
				to take. It is about the window itself, not only the zone of work.
@param	wma		Maximum width which the window during its management will be able 
				to take. It is about the window itself, not only the zone of work. 
				If -1 (BLANK) is transmitted, the maximum width will be the double 
				of the width of the desktop.
@param	hma		Maximum height which the window during its management will be able 
				to take. It is about the window itself, not only the zone of work. 
				If -1 (BLANK) is transmitted, the maximum height will be the double 
				height of the desktop.
@param	wu		Horizontal unit of shift of the window.
@param	hu		Vertical unit of shift of the window.
@param	flags	This parameter is a bit mask. See in the chapter of definitions 
				what is said about ::WFARROW, ::WFCALAG, ::WFDECAL and ::WFGROUP. 
				The other bits must not be modified, BIG uses them internally.
@param	fmouse	It is the form number which the mouse pointer will take when it is 
				brought in the working area of the window (see GEM function 
				graf_mouse()). 0 to keep the traditional form of an arrow.
@param	ricn	Drawing routine of the contents of the window to be used if it is 
				iconified. If equal to 0, it will be the normal routine (the 
				following parameter) which will be always used.
@param	redr	Pointer on the function to execute in the event of message of redraw 
				on this window.
@param	clos	Pointer on the function to execute in the event of message of closing 
				of the window.
@param	title	Pointer on the string of the window title, or empty string if there 
				is not.
@param	info	Pointer on the string of the line of infos of the window, or empty 
				string if there is not.
@param	wt		Total width of the contents of the window, in pixels.
@param	ht		Total height of the contents of the window, in pixels.
@param	fmenu	TRUE if the window must comprise a bar of menu, FALSE if not.
@param	ftool	TRUE if the window must comprise a toolbar, FALSE if not.
@param	bar		Tree Number which is the menu or the toolbar window, -1 if there 
				is not. A window cannot contain at the same time a menu and a 
				toolbar. If the two parameters fmenu and ftool are TRUE, BIG gives 
				priority with the menu.
@param	pattern Pattern of the working area at the opening of the window.
@param	colorp	Color Index of the frame of the working area at the opening of the 
				window.
@return			TRUE if the window were correctly created and opened, FALSE in the 
				event of problem (too many open windows).

This function creates and opens a window.

@note It is the "heaviest" function in BIG, with its 25 parameters!\n 
If the window had never be opened, the elements of its structure are initialized 
with default values or values which are transmitted in the parameter list. If it 
were already opened, then closed again, the values of certain elements are 
"refreshed" with the transmitted parameters, in particular maximum and minimum limits 
of size. If it is already opened, "refresh" is made, then it is brought to the 
foreground. \n
The parameter redr must be 0 if the window is of type ::WTYP_PICT, because 
in this case the routines of BIG take care of the redraw automatically (see the 
function print_page()). In the case of other contents (text, calculated graph, etc.), 
the programmer must write the drawing routine and to indicate it by this parameter.
The closed parameter indicates the function to be carried out at the time of closing
of the window. This function cannot be always only close and to destroy the window, 
it must sometimes also release the reserved storage areas if there is (see the 
function closed()).\n
The pattern number of the working area correspond to those used by VDI function 
vsf_style(). The pattern is thus not limited with the 7 models of the AES, but can 
take one of the 24 pattern provided by the VDI. If colorp is null (white color), 
the value of pattern is ignored. On this subject, see the function set_work().
If there are already too many opened windows, the function is given the responsibility 
to display the corresponding alert box of the resource integrated in BIG. */
int		open_window(int index, int type, int attr, int x, int y, int w, int h,
					int wmi, int hmi, int wma, int hma, int wu, int hu,
					int flags, int fmouse, FNCP ricn, FNCP redr, FNCP clos,
					char* title, char* info, long wt, long ht, int fmenu, 
					int ftool, int bar, int pattern, int colorp);

/*! @brief Find the BIG index of a window by its GEM Handle
@param	wh	Window GEM Handle of the window to search. If it is -1, the function 
			seeks first index BIG available.
@return		The function returns the BIG index of the window, or -1 if no
			correspondence is not found.

This function search the BIG index of a window from its GEM handle.

@note By transmitting "-1" (BLANK) as a parameter, one obtains in return the
first free BIG index, i.e. the first window having never yet been opened, or
-1 if it they it have all be. This functionality is useless in practice. */
int		find_index(int wh);

/*! @brief Find if the working area of a window is at a given location
@param	mx		X Coordinate of the position given.
@param	my		Y Coordinate of the position given.
@return:		GEM Handle of the window whose working area include the given point
				FALSE if there is not.

This function seeks if the working area of a window is at a given position.

@note WARNING the function turns over the GEM handle, not  BIGindex, and only 
if the coordinates correspond to the working area. */
int		find_window(int mx, int my);

/*! @brief Get work area position
@param	index	BIG Index of the window.
@param	xw		Returned value of the x coordinate of the window working area 
@param	yw		Returned value of the y coordinate of the window working area 
@param	ww		Returned value of the width of the window working area 
@param	hw		Returned value of the height of the window working area

This function indicates the coordinates of the working area of a window, whatever 
its attributes and its type.

@note If the window has a pull-down menu or a toolbar, the returned coordinates 
represents only the working area itself, without the menu or the toolbar. Under 
BIG, it is obligatory to use this function systematically, and not the GEM function 
wind_get(WF_WORKXYWH), which would give the same values as the total coordinates 
of the window. */
void	zone_work(int index, int* xw, int* yw, int* ww, int* hw);

/*! @brief Compute size and position of a window sliders
@param	index	BIG Index of the window to be treated.
@param	fredraw If TRUE, the sliders are redrawn, if FALSE, they are simply recomputed
@return			Nothing

This function calculates and fixes the positions and the sizes of the sliders of a 
window.

@note This function carries out a simple call of the four following functions
posi_vert(), posi_hori(), height_vert(), height_hori(), which can however be 
called separately if the needed. If the window must be entirely redrawn (by a 
message of redraw for example, not only the working area), it is useless to ask 
to redraw sliders, which would be redrawn twice. If not, it is necessary to put 
at TRUE the parameter fredraw, if not the new values would be done without 
consequence for the display. */
void	height_sliders(int index, int fredraw);

/*! @brief Compute the position of a window vertical slider
@param	index	BIG Index of the window to be treated.
@param	fredraw If TRUE, the slider is redrawn, if FALSE, they are simply recomputed
@return			Nothing

This function calculates and fixes the position of the vertical slider.

@note It is sometimes preferable to directly call this function instead of 
passing by height_sliders(), for example if the window does not have an 
horizontal slider, or if its size is unchanged but its position yes.
See with the function height_sliders() for explanation about the parameter fredraw */
void	posi_vert(int index, int fredraw);

/*! @brief Compute the position of a window horizontal slider
@param	index	BIG Index of the window to be treated.
@param	fredraw If TRUE, the slider is redrawn, if FALSE, they are simply recomputed
@return			Nothing

This function calculates and fixes the position of the horizontal slider.

@note It is sometimes preferable to directly call this function instead of 
passing by height_sliders(), for example if the window does not have an 
vertical slider, or if its size is unchanged but its position yes.
See with the function height_sliders() for explanation about the parameter fredraw */
void	posi_hori(int index, int fredraw);

/*! @brief Compute the size of a window vertical slider
@param	index	BIG Index of the window to be treated.
@param	fredraw If TRUE, the slider is redrawn, if FALSE, they are simply recomputed
@return			Nothing

This function calculates and fixes the size of the vertical slider.

@note It is sometimes preferable to directly call this function instead of 
passing by height_sliders(), for example if the window does not have an 
horizontal slider, or if its position is unchanged but its size yes.
See with the function height_sliders() for explanation about the parameter fredraw */
void	height_vert(int index, int fredraw);

/*! @brief Compute the size of a window horizontal slider
@param	index	BIG Index of the window to be treated.
@param	fredraw If TRUE, the slider is redrawn, if FALSE, they are simply recomputed
@return			Nothing

This function calculates and fixes the size of the horizontal slider.

@note It is sometimes preferable to directly call this function instead of 
passing by height_sliders(), for example if the window does not have an 
vertical slider, or if its position is unchanged but its size yes.
See with the function height_sliders() for explanation about the parameter fredraw */
void	height_hori(int index, int fredraw);

/*! @brief Redraw the visible part of an object belonging to a form
@param	object	Object number to be drawn.
@param	index	BIG Index of the window which contains the form, the toolbar or the menu
@return			Nothing

Draws an object belonging to a form, a toolbar or a menu in window, by drawing 
only the visible part.

@note This function is used like the GEM function objc_draw(), but only for the 
forms, toolbars or menus in window. It makes it possible to draw an object which 
could be partially out of the window or the screen, or which is covered by another 
window. */
void	draw_object(int object, int index);

/*! @brief Redraw the visible part of an object belonging to a form
@param	index	BIG Index of the window concerned.
@return			Nothing

This function redraw the whole contents of a window by taking account of the 
parts covered by other windows, etc.

@note If the window is of form type, it is redrawn by means of the functions GEM.
If the contents are an image, the function vro_cpyfm() (if the image has more than 
one plan of colors) or vrt_cpyfm() (if the image has only one plan of color) is used 
if an image is actually stored with the address indicated by 
win[index].w_cont.w_img.w_blk. If not, nothing is done, it is the bottom of the 
window which appears.\n
In the other cases, the function calls the display routine indicated by the pointer 
on function win[index].w_redraw.\n
This function does not generate an artificial redraw message, it directly requires 
the list of the rectangles of this window (which is permanently available, it is 
enough to require it from GEM). It should be called only if a message of redraw is 
not sent, if not it window would be drawn twice.\n
This function redraws only the working area of the window, from the point of view 
of BIG, i.e. the menu or the toolbar is ignored if it there is one. If you wish to 
redraw ALL the window, it is necessary to use the function send_mesag() while 
asking it to send a message of redraw (WM_REDRAW) for this window 
(win[index].w_hg), with the coordinates given by win[index].w_curr. */
void	print_page(int index);

/*! @brief Bring to front the next window

This function bring to foreground the window following the currently open one.

@note the order of the windows is that of their indexes. This function seeks in 
word 3 of the buffer of event (buf[3]) the GEM handle of the window which CYCLER
was clicked. The first open window following to this one is brought to the 
foreground. The starting window is not inevitably that in the foreground. */
void	next_wind(void);

/*! @brief Close and destroy window

This function closes a window and destroys it from the point of view of GEM.

@note the element win[index].w_hg of the ::BigWind structure is put at 0 to 
indicate that the window has already been opened, but that it is currently closed.
If the window contained a form, the storage area reserved for the backup of its 
state is released. If it had a menu, the storage area reserved for the construction 
of this menu is released. If it had a toolbar, the pointer w_bar is canceled.
This function seeks in word 3 of the buffer of events (global variable buf) the 
GEM handle of the window to be closed, which explains  that no parameter is 
needed. However, If you have a window containing some text for example, you 
will have to undoubtedly create a function for closing of window and to affect 
it as pointer on function with the element w_close of the ::BigWind structure of 
this window, so that your function releases the storage area reserved for the text. 
It is strongly recommended that your function calls itself the function closed(), 
but you will have before this call to put the GEM handle of the window in word 3 of 
the buffer of events:
@code
	buf[3] = win[index].w_hg;
	closed();
@endcode
*/
void	closed(void);

/*! @brief Iconify a window
@param	index	BIG Index of the window concerned.
@return			Nothing

This function iconify a window.

@note This function is automatically called by BIG at the time of a click on the 
SMALLER. However, it can happen that one wishes iconify one window at another time. 
In this case, this function must be called by the program.
See the chapter the iconisation of the windows for more details. */
void	iconise(int index);

/*! @brief Deiconify a Window
@param	index	BIG Index of the window concerned.
@return			Nothing

This function Deiconify a window and restore its former size, position, and attributes.

@note This function is automatically called by BIG when one requests to deiconify a 
window (double-click on the iconified window). However, it can happen that one wishes 
to deiconify a window at another time. In this case, this function must be called by 
the program. See the chapter the iconization of the windows for more details. */
void	uniconise(int index);

/*! @brief Iconyfy all open windows
@param	index	BIG Index of the main window, that must be iconified and represent 
				also the others.

This function iconify all the open windows in only one iconified window.

@note This function is automatically called by BIG at the time of a Control-click on 
the SMALLER. However, it can happen that one wishes to iconify all open window at 
another time. In this case, this function must be called by the program.
See the chapter the iconisation of the windows for more details. */
void	alliconise(int index);

/*! @brief Set pattern and color of window working area
@param	index	BIG Index of the window.
@param	pattern Number of the pattern. 
@param	colorp	Index of color.

This function is used to set the pattern and the color of the working area of a window.

@note the numbers of pattern corresponds to those used by function VDI vsf_style(). 
The pattern is thus not limited to the 7 models of the AES, but can take one of the 
24 pattern provided by the VDI. If colorp is null (white color), the value of pattern 
is ignored. The working area of windows BIG is always opaque. That exempts to have 
each time to clear it as one must do it systematically with the traditional GEM 
windows, and thus allows to choose directly a pattern and a color in the situations 
where that is desirable. This function acts only on one already open window. Its 
working area then is automatically redrawn. To fix the parameters of one window at 
the time of its opening, see the function open_window(). */
void	set_work(int index, int pattern, int colorp);

/*! @brief Modify window display parameters
@param	mtxt_mover	Writing Mode of the title bar when the window is in the 
					foreground: 
					- Opaque (MD_REPLACE, 1), 
					- Transparency (MD_TRANS, 2), 
					- Xor (MD_XOR, 3) or 
					- Opposite transparency (MD_ERASE, 4).
@param	ctxt_mover	Color Index of the displacement bar.
@param	spat_mover	Pattern Number of the displacement bar when the window is in 
					the foreground (no pattern when it is in background).
@param	cpat_mover	Color index of the displacement bar when the window is in 
					the foreground (always white when it is in background).
@param	ctxt_info	Color of the text of the infos line.
@param	spat_velev	Pattern number of the vertical slider.
@param	cpat_velev	Color index of the frame of the vertical slider.
@param	spat_helev	Pattern number of the horizontal slider.
@param	cpat_helev	Color index of the frame of the horizontal slider.
@param	spat_hslid	Pattern number of the vertical slider.
@param	cpat_hslid	Color index of the vertical slider.
@param	spat_vslid	Pattern number of the horizontal slider.
@param	cpat_vslid	Color index of the horizontal slider.
@param	cpat_gadgt	Pattern Number of button frame.
@param	cicn_gadgt	Color index of the icon of the buttons.

This function makes it possible to modify certain parameters concerning the aspect 
of BIG windows.

@note If "-1" is transmitted to an element, the corresponding adjustment is unchanged. 
By default, 
- if the system has less than 16 colors, the adjustments are: 
	- for the bar of displacement, the mode of writing is Opaque, 
	- the colors of text and frame are black, the pattern is number 2. 
	- The frame of the two sliders is the number 1, its black color. 
	- Same for the frame of the sliders, but the color is white. 
	- The buttons frame is white, the icon is black.
	.
- If the system has 16 colors or more, the adjustments are modified as:
	- the two sliders and the buttons frame are in 100% pattern and in gray (color 
	number 8), in order to to give an impression of relief.
	.
- Whatever the number of colors, the pattern of the buttons is ALWAYS 100%.
- If the color of a frame is white, the parameter of pattern for frame is ignored 
at the time of the drawing of the corresponding object.
.
The numbers of pattern correspond to those used by function VDI vsf_style(). 
The reason for pattern is that we are  not limited to the 7 patterns of the AES, 
but we can use one of the 24 reasons proposed by the VDI.
The adjustments affect all the windows of the application.\n
The function tests if there is at the time of the call some the window-forms, or 
windows with toolbar, open. If it finds some, these windows are entirely redrawn 
to bring up to date the new adjustments. Therefore, this function should never be 
called when the GEM is "freezed" by one blocking form of some sort. */
void	set_windows(int mtxt_mover, int ctxt_mover, int spat_mover, int cpat_mover,
					int ctxt_info, int spat_velev, int cpat_velev, int spat_helev, 
					int cpat_helev, int spat_vslid, int cpat_vslid, int spat_hslid, 
					int cpat_hslid, int cpat_gadgt, int cicn_gadgt);

/*! @brief Equivalent to GEM wind_calc()
@param	request	If 0 (WC_BORDER), the function receive the coordinates of the working 
				area of a window of type "kind" and returns that of the window frame. 
				If 1 (WC_WORK), it is the opposite.
@param	attr	Attributes of the window (bit mask).
@param	tree	If the window contains a menu bar or a toolbar, tree number 
				of the menu or toolbar. It is also possible to transmit directly 
				the address of the tree structure (see following parameter). In 
				this case, the parameter tree must be equal to -1 (BLANK).
@param	adrb	Address of the menu bar or the toolbar if there is one and if the 
				preceding parameter (tree) is equal to "-1". If tree is significant, 
				adrb must be null. If the window has neither menu, nor toolbar, the 
				parameter tree must imperatively be with "-1" and adrb imperatively 
				with zero.
@param	xd		x coordinate of the area to "convert"
@param	yd		y coordinate of the area to "convert", 
@param	wd		width of the area to "convert"
@param	hd		height of the area to "convert"
@param	x		x coordinate of the "converted" area
@param	y		y coordinate of the "converted" area
@param	w		width of the "converted" area
@param	h		height of the "converted" area

This function is the equivalent of the GEM function wind_calc(), adapted to BIG 
windows. It receives in parameters either the total coordinates of the window, or 
the coordinates of the working area, and provides the others in return.

@note Under BIG, it is formally forbidden to use the GEM function wind_calc(). 
This one should absolutely be used. By working area, it is necessary to include
understand zone of display, i.e. put aside the element of the window 
(sliders, buttons, etc), but also to put aside the menu or the toolbar if it there one. */
void	bwind_calc(int request, int attr, int tree, OBJECT* adrb,
				   int xd, int yd, int wd, int hd, int* x, int* y, int* w, int* h);

/*! @brief Redraw the title bar
@param	index	BIG Index of the window.
@return			Nothing

This function redraws the bar of title of a window, and only the bar of title.

@note If one wishes to change the title of a window, it is necessary to make 
the element w_title of a window point towards the new text (or modify the old one), 
then to call this function so that the display is updated. */
void draw_title(int index);

/*! @} */

/* translation of definitions from French to English */

#define	B_COCHE		B_CHECKB
#define B_BUTTON	B_ROUNDB

#endif
