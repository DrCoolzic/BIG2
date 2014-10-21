/*! @file big2hdoc.h
@brief Only contains doxygen documentation for big2.h

In order not to overload the big2.h file with doxygen documentation I have created
this file that only contains complementary information for big2.h
*/

/*! @def DLG_DESK
Usual dialog() event bit mask to handle desktop (with menu & windows) events */

/*! @def DLG_FORM
Usual dialog() event bit mask to handle form events */

/*! @def WATR_ALLG
All the attribute of GEM are defined */

/*! @def WATR_ALLB
All the attribute of BIG are defined i.e. really all */

/*! @def WATR_CURRG
All the attribute of GEM most usually used i.e. all but the info line */

/*! @def WATR_CURRB
All the attribute of BIG most usually used i.e. all but the info line */

/*! @def WATR_FORMG
Reserved to normal windowed form of type GEM (The form is not bigger than
the screen. Only the name of the window, the closing button, the displacement
bar, and iconification are active */

/*! @def WATR_FORMB
Same as ::WATR_FORMG but for BIG, also with the CYCLER */


/*! @var int BigWind::w_hg
To access a window, BIG uses naturally the GEM calls. It is thus essential
to preserve somewhere the GEM handle of this window. At initialization,
this element contains -1. This indicates that the window has never been
opened. As soon as it is created and opened, it contains the GEM Handle
for this window. But when it is closed, its value is not set back to -1
but to 0, which indicates that it was created, then destroyed by the GEM,
but that it always exists for BIG. The values contained in the other 
elements of the structure are therefore not anymore the default values,
but they constraints the last parameters of the window, which will be used
if the creation of this window is requested again to GEM.
*/

/*! @var int BigWind::w_type
This is the type of windows from a BIG perspective. Big defines the 
following types:
	- ::WTYP_NORM The window is "normal" contains probably text.
	- ::WTYP_PICT The window contains an image.
	- ::WTYP_FORM The window contains a form.
	- ::WTYP_TOOL The window contains a toolbar.
	- ::WTYP_MENU The window contains a menu.
	
A window cannot have at the same time a toolbar and a menu. BIG 
does not check, but gives the priority to the menu. 
::WTYP_NORM is the default type at initialization 
*/

/*! @var int BigWind::w_attr
They are the GEM attributes of the window 
(slider, mover, etc) that one indicates to the function wind_create(). 
Several sets of attributes are predefined by BIG to simplify work:
	- ::WATR_ALLG All the attributes GEM are activated.
	- ::WATR_ALLB All attributes BIG are activated, i.e. really all.
	- ::WATR_CURRG Most common (current) GEM attributes.
	- ::WATR_CURRB Most common (current) BIG attributes
	- ::WATR_FORMG Most common GEM Form attributes.
	- ::WATR_FORMB Most common BIG Form attributes.
	
ALL means all the attributes, CURR the most common attributes (all except 
the lines of information), and FORM for the windowed form (NAME, CLOSER 
and MOVER). The 'G' means that the attributes are limited to those of GEM, 
the 'B' means that they also include the extended attributes of BIG.
::WATR_CURRB is the default value at initialization. 
*/

/*! @var GRECT BigWind::w_curr
Coordinated and current dimensions of the window. This information 
is updated with each change of size or position of the window. 
At initialization the value is: {0,0,100,100}
*/

/*! @var Oldw BigWind::w_old
Recording of the coordinates and the attributes of a window before 
iconification, in order to restore them at the time of desiconification.
*/

/*! @var int BigWind::w_wmini
Minimum Width of the window working area. If one tries to dimension the width
of a window with lower values, BIG takes this minimum. Calculation takes into 
account the possible presence of a menu or of a toolbar in the window. 
@sa w_hmini w_wmini w_wmaxi w_hmaxi
*/

/*! @var int BigWind::w_hmini
Minimum Height of the window working area. If one tries to dimension the height
of a window with lower values, BIG takes this minimum. Calculation takes into 
account the possible presence of a menu or of a toolbar in the window.
@sa w_hmini w_wmini w_wmaxi w_hmaxi
*/

/*! @var int BigWind::w_wmaxi
Maximum Width of the window working area. 
If one tries to dimension it with higher value, BIG take this maximums. Calculation 
takes into account of the possible presence of a menu or of a toolbar in the window. 
Do not place objects in the toolbar on the right of the maximum width of the 
window! If the window is of type image (::WTYP_PICT), dimensions of the image become 
then the maximum dimensions of the window.
@sa w_hmini w_wmini w_wmaxi w_hmaxi
*/

/*! @var int BigWind::w_hmaxi
Maximum Height of the window working area. 
If one tries to dimension it with higher value, BIG take this maximums. Calculation 
takes into account of the possible presence of a menu or of a toolbar in the window. 
If the window is of type image (::WTYP_PICT), dimensions of the image become 
then the maximum dimensions of the window.
@sa w_hmini w_wmini w_wmaxi w_hmaxi
*/

/*! @var int BigWind::w_wunit
Horizontal shift units of the window, in pixels.
At the time of a click on one of the scroll arrows, the contents are shifted in the required direction by this value. For text, it would be respectively the width 
of a cell of character. At initialization: 1
@sa w_hunit
*/

/*! @var int BigWind::w_hunit
Vertical shift units of the window, in pixels.
At the time of a click on one of the scroll arrows, the contents are shifted in the required direction by this value. For text, it would be respectively the width 
of a cell of character. At initialization: 1
@sa w_wunit
*/

/*! @var int BigWind::w_hprot
Horizontal Protection. Gives the number of 
pixels which will be "protected" on the top of the working area. The 
events of displacement of a line toward the top or bottom will not affect this part 
of the display, which remains thus protected. That allows, for example if columns 
of information are displayed, to preserve in top of the window the titles of 
these columns even if one moves vertically. This element is fixed at 0 with 
initialization. It is the programmer who must, if necessary, to place here 
other value.
@sa w_vprot
*/

/*! @var int BigWind::w_vprot
Vertical Protection. Gives the number of pixels which will be "protected" on 
the left of the working area. The events of displacement of a line 
toward the left or right side will not affect this part of the display, which remains 
thus protected. That allows, for example if lines of information are displayed, 
to preserve in the left of the window the titles of these lines even if one 
moves horizontally. This element is fixed at 0 with initialization. It is the 
programmer who must, if necessary, to place there other value.
@sa w_hprot
*/

/*! @var Cont BigWind::w_cont
Content of window. Fixed at "0L" at init time, this element is in 
fact a ::Cont union. It depends on what is displayed in the window. The elements of this 
union are:
	- Form w_form Form. If the window contains a form, w_cont is a structure 
	::Form, defined by BIG. Here is description:
		- OBJECT* w_tree: Address of the form.
		- int w_edit If the form contains one (or several) editable field(s), 
		number of object of the field where the cursor is. -1 if not.
		- int w_pos If the form contains one (or several) editable fields 
		position of the cursor in the current field. -1 if not.
		char* w_bak: BIG is able to manage the buttons 'Confirm', 'Cancel', 
		'Apply' and 'Reset' of forms, by restoring the state of the form in 
		the event of cancellation or reset. This pointer indicates the buffer of 
		backup state for the form.
		.
	- MFDB w_img 'Image'. If the window contains an image, w_cont is MFDB. 
	The MFDB (Memory Form Block Definition) is a standardized VDI structure, 
	which contains all the necessary informations with the management of an image 
	(its address, its dimensions, etc). Thanks to this standardization, BIG can 
	deal completely with the display of the image in the window, without 
	intervention of the program itself.
	- char* w_adr: If the window contains neither an image, nor a form 
	(it then contains probably text or a calculated graphics), this element is 
	a pointer on char which indicates the beginning of the storage area where 
	the data (the text) are stored. The display routine of this window will be 
	able to then find them directly, whatever is the current window. 
	.
*/

/*! @var Pallet* BigWind::w_pal
This element is a pointer on a buffer zone of N time 3 int. 'N' being the number 
of displayable colors. Each of the 3 words defines values of red, green and blue 
of the color. Thus, it is possible to associate a palette with each window. 
That is almost essential for the window of images, but can prove to be useful 
for other windows. If this element is ZERO, BIG activates the default pallets. 
Set to ZERO at initialization.
*/

/*! @var OBJECT* BigWind::w_bar
If the window contains a menu bar or a toolbar, this is
the addresses of the tree of this menu.
*/

/*! @var OBJECT BigWind::w_fen[NGAD]
It is in this zone of ::NGAD OBJECT structures that the 'window-form' of BIG 
windows is copied. Indeed, it consists of NGAD objects, and each window has its 
own, with characteristics which its specific to this window (for example, 
certain objects could be hidden if the window does not have the corresponding 
attribute. Normally, the programmer does not need to modify nor even read the 
information placed in this element of BigWind. The functions of BIG take care 
of everything, some are precisely intended to modify the data in this element 
(the color of an object, for example).
*/

/*! @var int BigWind::w_flags
This element is a bit mask. Flags description:
	- Bit 0 ::WFFULL (Window flag FULL): is set if the window currently 
	has its maximum size (full opening).
	- Bit 1 ::WFARROW (Window Flag ARROW): is set if the window must be handled
	 starting from the keyboard. The arrow keys correspond to the arrows of the 
	 windows, and the <Shift + arrow> correspond to the scroller. \<Esc> opens the 
	 window with its maximum size or that of the screen, or brings back it to 
	 its preceding size. Moreover, if the window is of type ::WTYP_NORM, \<Clr> 
	 brings the display on the first line and <Shift + Clr> on the last 
	 (less the height of the window).
	- Bit 2 ::WFCALAG (Window Flag Position): When set indicates that the 
	width and the height of the working area of the window must always be multiples 
	of 	the units of shift. In the same way for the positioning of the contents of 
	the window at the time of a Slider event.
	- Bit 3 ::WFDECAL (Window Flag Shift): Necessary by the presence of new Sizers. If 
	it is not set, the part of the contents displayed from the upper-left angle of 
	the working area of the window will not be modified, even in the event of 
	re-dimensioning by Upper Left Sizer. If the bit is set, the displayed part will 
	be possibly be updated. By example, if you reduce the height of the window of 
	50 pixels by means of the Upper Right Sizer, the contents of the window will 
	remain with the same 'virtual' position, and thus the display will begin 50 
	lower pixels. If the bit is not set, the display will always start at the 
	same place.
	- Bit 4 ::WFGROUP (Window Flag Grouping arrows) If this bit is set, the four 
	arrows of the window will be grouped in the lower right angle. This flag 
	must \b never be put for a window form.
	- Bit 5: reserved
	- Bit 6: reserved
	- Bit 7: ::WFLAST (Window LAST) It is used by BIG to know if the window is the 
	last of those which were declared (NEVER modify this bit).
	.
	At initialization only ::WFARROW is set.\n
	The low-order byte of this word is entirely used or reserved for possible 
	future extensions of BIG. On the other hand, the most significant byte is 
	not used, nor not even tested. You are thus free to use it with your own 
	way if the windows of your program need other flags.
*/

/*! @var int BigWind::w_mouse
If this element is equal to zero (value at initialization), 
the mouse will always have the traditional shape of an arrow. If not, the value is
interpreted as the AES number of the shape which the mouse must take in the 
working area of the window (when the window is in front).
*/

/*! @var int BigWind::w_icon
If the window is normal, this element contains '- 1'. If it is 
iconified, it contains index of the associated BIG window. That mean that if 
the window was iconified alone, it is its own index, but if it belongs to a 
group of iconified windows in only one icon, it is the index of the main 
window, the one which was actually iconified, the others being temporarily 
invisible. At the time of desiconification, BIG will seek all the currently 
closed windows whose this element is the index of the one asked for the 
desiconification, and it will reopen them.
*/

/*! @var FNCP BigWind::w_redicn
Pointer on a routine for redrawing the contents 
of the window which is used only if the window is iconified. If it contains 0L, 
it is the normal routine which is used. Initialized with 0L.
*/

/*! @var FNCP BigWind::w_cycle
Pointer on the routine to bring in the fron the following 
window. At initialization, it points on the function next_wind().
*/

/*! @var FNCP BigWind::w_redraw
Pointer on the redraw routine of the window. Initialized with 0L. 
This pointer is necessary only if the window contains another thing that an 
image or a form, because in these case BIG takes care of the redraw. If there 
are 'protected' zones in the top or on the left of the display (or both), it 
is necessary to take this in account in the routine. It must display the contents 
of the protected zone(s) independently of the display itself.
*/

/*! @var FNCP BigWind::w_close
Pointer on the routine for closing a window. At initialization, 
it points on the function closed() which closes the window and requires of GEM 
to forget its existence (but BIG has more memory). However, it will be necessary 
in certain cases that the programmer writes this function in order to release if 
necessary the occupied storage area for example by the text of the window.
*/

/*! @var FNCP BigWind::w_move
- FNCP w_move Pointeur on a routine which informs GEM that the window was moved, 
and which records its new coordinates. At initialization point to the function 
moved().
*/

/*! @var FNCP BigWind::w_size
- FNCP w_size Pointer on a routine which informs GEM that the window was 
resized, and which records its new coordinates. Point at initialization on 
the function sized()or size_form() in the case of a window form. See 
in the chapter of the guided visit what was said as in connection with the 
histograms window.
*/

/*! @var FNCP BigWind::w_full
- FNCP w_full Pointer on the function which gives to the window its maximum 
size, or brings back it to the preceding ones if it is already to the maximum. 
This maximum size is possibly limited in the case of a window image or form to 
the dimensions of this image or this form. The new coordinates are transmitted 
to GEM. At initialization, points on the function fulled() or  full_form() in 
the case of a window form.
*/

/*! @var FNCP BigWind::w_hslid
Pointers on the function to be carried out in the event of displacement of a 
horizontal slider box. The function carries out the tests necessary, transmits 
information to GEM and records it. At initialization, point respectively on hslid()
@sa w_vslid
*/

/*! @var FNCP BigWind::w_vslid
Pointers on the function to be carried out in the event of displacement of a 
vertical slider box. The function carries out the tests necessary, transmits 
information to GEM and records it. At initialization, point respectively on vslid()
@sa w_hslid
*/

/*! @var FNCP BigWind::w_uppage
Pointers on the functions to be carried out in case of event of slider, i.e. 
a request for shifting per page toward the top side. The function carries out 
the tests necessary, transmits information to GEM and records it. 
At initialization it points to uppage().
This functions move the contents of the window by the height of the window
working area.
@sa w_uppage, w_dnpage, w_lfpage, w_rtpage
*/

/*! @var FNCP BigWind::w_dnpage
Pointers on the functions to be carried out in case of event of slider, i.e. 
a request for shifting per page toward the bottom side. The function carries out 
the tests necessary, transmits information to GEM and records it. 
At initialization it points to dnpage().
This functions move the contents of the window by the height of the window
working area.
@sa w_uppage, w_dnpage, w_lfpage, w_rtpage
*/

/*! @var FNCP BigWind::w_lfpage
Pointers on the functions to be carried out in case of event of slider, i.e. 
a request for shifting per page toward the left side. The function carries out 
the tests necessary, transmits information to GEM and records it. 
At initialization, points to lfpage().
This functions move the contents of the window by the width of the window
working area.
@sa w_uppage, w_dnpage, w_lfpage, w_rtpage
*/

/*! @var FNCP BigWind::w_rtpage
Pointers on the functions to be carried out in case of event of slider, i.e. 
a request for shifting per page toward the right side. The function carries out 
the tests necessary, transmits information to GEM and records it. 
At initialization it points to rtpage().
This functions move the contents of the window by the width of the window
working area.
@sa w_uppage, w_dnpage, w_lfpage, w_rtpage
*/

/*! @var FNCP BigWind::w_upline
Pointers on the functions to be carried out in case of event of slider, i.e. 
a request for shifting per line toward the top side. The function carries out 
the tests necessary, transmits information to GEM and records it. 
At initialization it points to upline().
This functions move the contents of the window by the step size noted in 
the element  and 'w_hunit'.
@sa w_upline, w_dnline, w_lfline, w_rtline
*/

/*! @var FNCP BigWind::w_dnline
Pointers on the functions to be carried out in case of event of slider, i.e. 
a request for shifting per line toward the bottom side. The function carries out 
the tests necessary, transmits information to GEM and records it. 
At initialization it points to dnline().
This functions move the contents of the window by the step size noted in 
the element  and 'w_hunit'.
@sa w_upline, w_dnline, w_lfline, w_rtline
*/

/*! @var FNCP BigWind::w_lfline
Pointers on the functions to be carried out in case of event of slider, i.e. 
a request for shifting per line toward the left side. The function carries out 
the tests necessary, transmits information to GEM and records it. 
At initialization it points to lfline().
This functions move the contents of the window by the step size noted in 
the element  and 'w_wunit'.
@sa w_upline, w_dnline, w_lfline, w_rtline
*/

/*! @var FNCP BigWind::w_rtline
Pointers on the functions to be carried out in case of event of slider, i.e. 
a request for shifting per line toward the right side. The function carries out 
the tests necessary, transmits information to GEM and records it. 
At initialization it points to rtline().
This functions move the contents of the window by the step size noted in 
the element  and 'w_wunit'.
@sa w_upline, w_dnline, w_lfline, w_rtline
*/

/*! @var char* BigWind::w_title
0 at initialization.
*/

/*! @var char* BigWind::w_infos
0 at initialization.
*/

/*! @var long BigWind::w_wtot
This element contain the width of the window, in 
pixels. For an image, that corresponds to the width of this image. For 
text, the width represents the maximum number characters of a line 
multiplied by the width of a cell of character. Initialized to 0.
@sa w_htot
*/

/*! @var long BigWind::w_htot
This element contain the height of the window, in pixels. 
For an image, that corresponds to dimensions of this image. 
For text, the height would represent the maximum number of a line 
multiplied by the height of a cell of character. Initialized to 0.
@sa w_wtot
*/

/*! @var long BigWind::w_lin
Represents (in pixels) the first column to be displayed in the upper left 
corner of the working area of the window, compared to the overall
contents w_wtot
@sa w_col
*/

/*! @var long BigWind::w_col
Represents (in pixels) the first line to be displayed in the upper left 
corner of the working area of the window, compared to the overall
contents w_htot
@sa w_lin
*/

/*! @var OBJECT** ::ir_trindex
Pointer on pointer of structures OBJECT. It be thus used like a table of structures. 
This pseudo-table contains all addresses of all the forms of the resource, whether 
this one is integrated or external. The address of form 'FORM' is obtained for 
example ir_trindex[FORM]. The use of the function GEM rsrc_gaddr() is 
thus useless.
*/

/*! @var int ::cliq_in
This variable is used jointly with the pop_ups, the pop-lists and the hierarchical 
menus. If an option were indeed chosen, this variable is TRUE, if not FALSE (clicked
outside).
*/

/*! @var GRECT ::zclip
At the time of a redraw message, the rectangle of the list are drawn one by one. 
If the window is of "normal" type, the function pointed by the element w_redraw 
is called each time. Before this call, the coordinates of the clipped zone (thus to redraw) are placed in this variables. If that proves to be necessary to optimize 
the speed of display, the developer can redraw only what is indeed in the zone 
(particularly with a vectorial graphics software.
*/

/*! @var int t_display::mode 
Running mode if one works on Falcon. It is a mask of bits describing the video 
mode. Here details:
	- Bits 0 to 2: Bits by pixels. 0 = 1 bps; 1 = 2 bps; 2 = 4 bps; 
	3 = 8 bps; 4 = 16 bps.
	- Bit 3: Flag 80 columns (40 if it is not put).
	- Bit 4: Flag VGA (TV mode if it is not put).
	- Bit 5: Flag PAL or NTSC if it is not put.
	- Bit 6: Overscan flag. Multiply X and there by 1.2 (unused in VGA).
	- Bit 7: Flag of compatibility ST
	- Bit 8: Vertical flag. Mode interlaced on color screens, double line in VGA.
	- Bits 9 to 15: Reserved.
	.
*/

/*! @var Palette* t_display::palette 
Pointer on the backup of the default pallet (the one which is active when the 
program start). The type "Pallette" is defined like this: 
typedef int Pallet [3], it is thus adapted to the VDI pallet 
description format. BIG use a storage area indicated by "pallet" with  
sufficient size defined by: 
pallets = (Pallet* ) malloc (n_color*  sizeof (Pallet));
*/

/*! @typedef BLANK
Very much used by BIG, This value is often transmitted to a function to 
indicate that it must be unaware of or not to treat them things relative to 
this parameter. For example, if there is no editable field  in a form, the 
parameter transmitted for the variable "editable" is "BLANK"
*/

