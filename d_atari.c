/*! @file d_atari.c
@brief Dummy file used to compile under Windows.

Dummy file used to compile under Windows. Defines the GEM and TOS functions
to make the linker happy. :)

BIG_IMGS library allow to load images in the context of the BIG library.
To use this library in your program you need to include the BIG_IMGS.H 
include file. The program will also need to be linked with the BIG_IMGS.LIB
library. The library does not define any new type it just offers 6 new
functions described below.

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

*//* dummy file for windows to link correctly */
#ifdef WIN32
#define cdecl
#pragma warning(disable : 4996)					/* disable deprecated function */
#endif

#include <aes.h>

/* AES */
int  vq_aes( void ){return 1;}
void _crystal( AESPB *aespb ){return;}

int appl_init( void ) {return 1;}
int appl_read( int ap_rid, int ap_rlength, void *ap_rpbuff ) {return 1;}
int appl_write( int ap_wid, int ap_wlength, void *ap_wpbuff ) {return 1;}
int appl_find( const char *ap_fpname ) {return 1;}
int appl_tplay( void *ap_tpmem, int ap_tpnum, int ap_tpscale ) {return 1;}
int appl_trecord( void *ap_trmem, int ap_trcount ) {return 1;}
int appl_exit( void ) {return 1;}
int appl_search( int ap_smode, char *ap_sname, int *ap_stype,
                 int *ap_sid ) {return 1;}
int appl_getinfo( int ap_gtype, int *ap_gout1, int *ap_gout2,
                  int *ap_gout3, int *ap_gout4 ) {return 1;}

int evnt_keybd( void ) {return 1;}
int evnt_button( int ev_bclicks, int ev_bmask, int ev_bstate,
                 int *ev_bmx, int *ev_bmy, int *ev_bbutton,
                 int *ev_bkstate ) {return 1;}
int evnt_mouse( int ev_moflags, int ev_mox, int ev_moy,
                int ev_mowidth, int ev_moheight, int *ev_momx,
                int *ev_momy, int *ev_mobutton,
                int *ev_mokstate ) {return 1;}
int evnt_mesag( int *ev_mgpbuff ) {return 1;}
int evnt_timer( int ev_tlocount, int ev_thicount ) {return 1;}
int evnt_multi( int ev_mflags, int ev_mbclicks, int ev_mbmask,
                int ev_mbstate, int ev_mm1flags, int ev_mm1x,
                int ev_mm1y, int ev_mm1width, int ev_mm1height,
                int ev_mm2flags, int ev_mm2x, int ev_mm2y,
                int ev_mm2width, int ev_mm2height,
                int *ev_mmgpbuff, int ev_mtlocount,
                int ev_mthicount, int *ev_mmox, int *ev_mmoy,
                int *ev_mmbutton, int *ev_mmokstate,
                int *ev_mkreturn, int *ev_mbreturn ) {return 1;}
int evnt_dclick( int ev_dnew, int ev_dgetset ) {return 1;}

int menu_bar( OBJECT *me_btree, int me_bshow ) {return 1;}
int menu_icheck( OBJECT *me_ctree, int me_citem, int me_ccheck ) {return 1;}
int menu_ienable( OBJECT *me_etree, int me_eitem,
                  int me_eenable ) {return 1;}
int menu_tnormal( OBJECT *me_ntree, int me_ntitle,
                  int me_nnormal ) {return 1;}
int menu_text( OBJECT *me_ttree, int me_titem,
               const char *me_ttext ) {return 1;}
int menu_register( int me_rapid, const char *me_rpstring ) {return 1;}
int menu_popup( MENU *me_menu, int me_xpos, int me_ypos,
                MENU *me_mdata ) {return 1;}
int menu_attach( int me_flag, OBJECT *me_tree, int me_item,
                 MENU *me_mdata ) {return 1;}
int menu_istart( int me_flag, OBJECT *me_tree, int me_imenu,
                 int me_item ) {return 1;}
int menu_settings( int me_flag, MN_SET *me_values ) {return 1;}

int objc_add( OBJECT *ob_atree, int ob_aparent, int ob_achild ) {return 1;}
int objc_delete( OBJECT *ob_dltree, int ob_dlobject ) {return 1;}
int objc_draw( OBJECT *ob_drtree, int ob_drstartob,
               int ob_drdepth, int ob_drxclip, int ob_dryclip,
               int ob_drwclip, int ob_drhclip ) {return 1;}
int objc_find( OBJECT *ob_ftree, int ob_fstartob, int ob_fdepth,
               int ob_fmx, int ob_fmy ) {return 1;}
int objc_offset( OBJECT *ob_oftree, int ob_ofobject,
                 int *ob_ofxoff, int *ob_ofyoff ) {return 1;}
int objc_order( OBJECT *ob_ortree, int ob_orobject,
                int ob_ornewpos ) {return 1;}
int objc_edit( OBJECT *ob_edtree, int ob_edobject,
               int ob_edchar, int *ob_edidx, int ob_edkind ) {return 1;}
int objc_change( OBJECT *ob_ctree, int ob_cobject,
                 int ob_cresvd, int ob_cxclip, int ob_cyclip,
                 int ob_cwclip, int ob_chclip,
                 int ob_cnewstate, int ob_credraw ) {return 1;}

int form_do( OBJECT *fo_dotree, int fo_dostartob ) {return 1;}
int form_dial( int fo_diflag, int fo_dilittlx,
               int fo_dilittly, int fo_dilittlw,
               int fo_dilittlh, int fo_dibigx,
               int fo_dibigy, int fo_dibigw, int fo_dibigh ) {return 1;}
int form_alert( int fo_adefbttn, const char *fo_astring ) {return 1;}
int form_error( int fo_enum ) {return 1;}
int form_center( OBJECT *fo_ctree, int *fo_cx, int *fo_cy,
                 int *fo_cw, int *fo_ch ) {return 1;}
int form_keybd( OBJECT *fo_ktree, int fo_kobject, int fo_kobnext,
                int fo_kchar, int *fo_knxtobject, int *fo_knxtchar ) {return 1;}
int form_button( OBJECT *fo_btree, int fo_bobject, int fo_bclicks,
                int *fo_bnxtobj ) {return 1;}

int graf_rubberbox( int gr_rx, int gr_ry, int gr_minwidth,
                    int gr_minheight, int *gr_rlastwidth,
                    int *gr_rlastheight ) {return 1;}
int graf_rubbox( int gr_rx, int gr_ry, int gr_minwidth,
                    int gr_minheight, int *gr_rlastwidth,
                    int *gr_rlastheight ) {return 1;}
int graf_dragbox( int gr_dwidth, int gr_dheight,
                  int gr_dstartx, int gr_dstarty,
                  int gr_dboundx, int gr_dboundy,
                  int gr_dboundw, int gr_dboundh,
                  int *gr_dfinishx, int *gr_dfinishy ) {return 1;}
int graf_movebox( int gr_mwidth, int gr_mheight,
                  int gr_msourcex, int gr_msourcey,
                  int gr_mdestx, int gr_mdesty ) {return 1;}
int graf_mbox( int gr_mwidth, int gr_mheight,
                  int gr_msourcex, int gr_msourcey,
                  int gr_mdestx, int gr_mdesty ) {return 1;}
int graf_growbox( int gr_gstx, int gr_gsty,
                  int gr_gstwidth, int gr_gstheight,
                  int gr_gfinx, int gr_gfiny,
                  int gr_gfinwidth, int gr_gfinheight ) {return 1;}
int graf_shrinkbox( int gr_sfinx, int gr_sfiny,
                    int gr_sfinwidth, int gr_sfinheight,
                    int gr_sstx, int gr_ssty,
                    int gr_sstwidth, int gr_sstheight ) {return 1;}
int graf_watchbox( OBJECT *gr_wptree, int gr_wobject,
                   int gr_winstate, int gr_woutstate) {return 1;}
int graf_slidebox( OBJECT *gr_slptree, int gr_slparent,
                   int gr_slobject, int gr_slvh ) {return 1;}
int graf_handle( int *gr_hwchar, int *gr_hhchar,
                 int *gr_hwbox, int *gr_hhbox ) {return 1;}
int graf_mouse( int gr_monumber, MFORM *gr_mofaddr ) {return 1;}
int graf_mkstate( int *gr_mkmx, int *gr_mkmy,
                  int *gr_mkmstate, int *gr_mkkstate ) {return 1;}

int scrp_read( char *sc_rpscrap ) {return 1;}
int scrp_write( char *sc_wpscrap ) {return 1;}

int fsel_input( char *fs_iinpath, char *fs_iinsel,
                int *fs_iexbutton ) {return 1;}
int fsel_exinput( char *fs_einpath, char *fs_einsel,
                int *fs_eexbutton, char *fs_elabel ) {return 1;}

int wind_create( int wi_crkind, int wi_crwx, int wi_crwy,
                 int wi_crww, int wi_crwh ) {return 1;}
int wind_open( int wi_ohandle, int wi_owx, int wi_owy,
               int wi_oww, int wi_owh ) {return 1;}
int wind_close( int wi_clhandle ) {return 1;}
int wind_delete( int wi_dhandle ) {return 1;}
int wind_get( int wi_ghandle, int wi_gfield, ... ) {return 1;}
int wind_set( int wi_shandle, int wi_sfield, ... ) {return 1;}
int wind_find( int wi_fmx, int wi_fmy ) {return 1;}
int wind_update( int wi_ubegend ) {return 1;}
int wind_calc( int wi_ctype, int wi_ckind, int wi_cinx,
               int wi_ciny, int wi_cinw, int wi_cinh,
               int *coutx, int *couty, int *coutw,
               int *couth ) {return 1;}
void wind_new( void ) {return;}

int rsrc_load( const char *re_lpfname ) {return 1;}
int rsrc_free( void ) {return 1;}
int rsrc_gaddr( int re_gtype, int re_gindex, void *gaddr ) {return 1;}
int rsrc_saddr( int re_stype, int re_sindex, void *saddr ) {return 1;}
int rsrc_obfix( OBJECT *re_otree, int re_oobject ) {return 1;}
int rsrc_rcfix( RSHDR *rc_header ) {return 1;}

int shel_read( char *sh_rpcmd, char *sh_rptail ) {return 1;}
int shel_write( int sh_wdoex, int sh_wisgr, int sh_wiscr,
                char *sh_wpcmd, char *sh_wptail ) {return 1;}
int shel_get( char *sh_gaddr, int sh_glen ) {return 1;}
int shel_put( char *sh_paddr, int sh_plen ) {return 1;}
int shel_find( char *sh_fpbuff ) {return 1;}
int shel_envrn( char **sh_epvalue, char *sh_eparm ) {return 1;}

#include <tos.h>
/* TOS*/

long    gemdos(U32) {return 1;}
long    bios(U32) {return 1;}
long    xbios(U32) {return 1;}

void    Pterm0( void ) {return;}
long    Cconin( void ) {return 1;}
void    Cconout( int c ) {return;}
int     Cauxin( void ) {return 1;}
void    Cauxout( int c ) {return;}
int     Cprnout( int c ) {return 1;}
long    Crawio( int w ) {return 1;}
long    Crawcin( void ) {return 1;}
long    Cnecin( void ) {return 1;}
int     Cconws( const char *buf ) {return 1;}
void    Cconrs( LINE *buf ) {return;}
int     Cconis( void ) {return 1;}
long    Dsetdrv( int drv ) {return 1;}
int     Cconos( void ) {return 1;}
int     Cprnos( void ) {return 1;}
int     Cauxis( void ) {return 1;}
int     Cauxos( void ) {return 1;}
int     Dgetdrv( void ) {return 1;}
void    Fsetdta( DTA *buf ) {return;}
long    Super( void *stack ) {return 1;}
unsigned int  Tgetdate( void ) {return 1;}
unsigned int Tsetdate( unsigned int date ) {return 1;}
unsigned int  Tgettime( void ) {return 1;}
unsigned int  Tsettime( unsigned int time ) {return 1;}
DTA     *Fgetdta( void ) {return (DTA*)1;}
int     Sversion( void ) {return 1;}
void    Ptermres( long keepcnt, int retcode ) {return;}
int     Dfree( DISKINFO *buf, int driveno ) {return 1;}
int     Dcreate( const char *path ) {return 1;}
int     Ddelete( const char *path ) {return 1;}
int     Dsetpath( const char *path ) {return 1;}
long    Fcreate( const char *filename, int attr ) {return 1;}
long    Fopen( const char *filename, int mode ) {return 1;}
int     Fclose( int handle ) {return 1;}
long    Fread( int handle, long count, void *buf ) {return 1;}
long    Fwrite( int handle, long count, void *buf ) {return 1;}
int     Fdelete( const char *filename ) {return 1;}
long    Fseek( long offset, int handle, int seekmode ) {return 1;}
int     Fattrib( const char *filename, int wflag, int attrib ) {return 1;}
long    Fdup( int handle ) {return 1;}
int     Fforce( int stch, int nonstdh ) {return 1;}
int     Dgetpath( char *path, int driveno ) {return 1;}
void    *Malloc( long number ) {return (void*)1;}
int     Mfree( void *block ) {return 1;}
int     Mshrink( int zero, void *block, long newsiz ) {return 1;}
long    Pexec( int mode, char *ptr1, void *ptr2, void *ptr3 ) {return 1;}
void    Pterm( int retcode ) {return;}
int     Fsfirst( const char *filename, int attr ) {return 1;}
int     Fsnext( void ) {return 1;}
int     Frename( int zero, const char *oldname, const char *newname ) {return 1;}
int     Fdatime( DOSTIME *timeptr, int handle, int wflag ) {return 1;}

void    *Mxalloc( long number, int mode ) {return (void*)0;}
long    Maddalt( void *start, long size ) {return 1;}

long    Flock( int handle, int mode, long start, long length ) {return 1;}

void    Getmpb( MPB *ptr ) {return;}
int     Bconstat( int dev ) {return 1;}
long    Bconin( int dev ) {return 1;}
void    Bconout( int dev, int c ) {return;}
long    Rwabs( int rwflag, void *buf, int cnt, int recnr, int dev ) {return 1;}
void    (*Setexc( int number, void (*exchdlr)() )) () {return (void*)1;}
long    Tickcal( void ) {return 1;}
BPB     *Getbpb( int dev ) {return (BPB*)1;}
long    Bcostat( int dev ) {return 1;}
long    Mediach( int dev ) {return 1;}
long    Drvmap( void ) {return 1;}
long    Kbshift( int mode ) {return 1;}

void    Initmouse( int type, MOUSE *par, void (*mousevec)() ) {return;}
void    *Ssbrk( int count ) {return (void*)1;}
void    *Physbase( void ) {return (void*)1;}
void    *Logbase( void ) {return (void*)1;}
int     Getrez( void ) {return 1;}
void    Setscreen( void *laddr, void *paddr, int rez ) {return;}
void    Setpalette( void *pallptr ) {return;}
int     Setcolor( int colornum, int color ) {return 1;}
int     Floprd( void *buf, void *filler, int devno, int sectno,
               int trackno, int sideno, int count ) {return 1;}
int     Flopwr( void *buf, void *filler, int devno, int sectno,
               int trackno, int sideno, int count ) {return 1;}
int     Flopfmt( void *buf, void *filler, int devno, int spt, int trackno,
                int sideno, int interlv, long magic, int virgin ) {return 1;}
void    Midiws( int cnt, void *ptr ) {return;}
void    Mfpint( int erno, void (*vector)() ) {return;}
IOREC   *Iorec( int dev ) {return (IOREC*)1;}
long    Rsconf( int baud, int ctr, int ucr, int rsr, int tsr, int scr ) {return 1;}
KEYTAB  *Keytbl( void *unshift, void *shift, void *capslock ) {return (KEYTAB*)1;}
long    Random( void ) {return 1;}
void    Protobt( void *buf, long serialno, int disktype, int execflag ) {return;}
int     Flopver( void *buf, void *filler, int devno, int sectno,
                int trackno, int sideno, int count ) {return 1;}
void    Scrdmp( void ) {return;}
int     Cursconf( int func, int rate ) {return 1;}
void    Settime( unsigned long time ) {return;}
unsigned long  Gettime( void ) {return 1;}
void    Bioskeys( void ) {return;}
void    Ikbdws( int count, void *ptr ) {return;}
void    Jdisint( int number ) {return;}
void    Jenabint( int number ) {return;}
char    Giaccess( char data, int regno ) {return 1;}
void    Offgibit( int bitno ) {return;}
void    Ongibit( int bitno ) {return;}
void    Xbtimer( int timer, int control, int data, void (*vector)() ) {return;}
void    *Dosound( void *buf ) {return (void*)1;}
int     Setprt( int config ) {return 1;}
KBDVBASE *Kbdvbase( void ) {return (KBDVBASE*)1;}
int     Kbrate( int initial, int repeat ) {return 1;}
void    Prtblk( PBDEF *par ) {return;}
void    Vsync( void ) {return;}
long    Supexec( long (*func)() ) {return 1;}
void    Puntaes( void ) {return;}
int     Floprate( int devno, int newrate ) {return 1;}
int     Blitmode( int mode ) {return 1;}

int     DMAread( long sector, int count, void *buffer, int devno ) {return 1;}
int     DMAwrite( long sector, int count, void *buffer, int devno ) {return 1;}
int     NVMaccess( int opcode, int start, int count, void *buffer ) {return 1;}
long    Bconmap( int devno ) {return 1;}
int     Esetshift( int shftMode ) {return 1;}
int     Egetshift( void ) {return 1;}
int     EsetBank( int bankNum ) {return 1;}
int     EsetColor( int colorNum, int color ) {return 1;}
void    EsetPalette( int colorNum, int count, int *palettePtr ) {return;}
void    EgetPalette( int colorNum, int count, int *palettePtr ) {return;}
int     EsetGray( int swtch ) {return 1;}
int     EsetSmear( int swtch ) {return 1;}

int     Syield( void ) {return 1;}
int     Fpipe( int *usrh ) {return 1;}
int     Fcntl( int f, long arg, int cmd ) {return 1;}
long    Finstat( int f ) {return 1;}
long    Foutstat( int f ) {return 1;}
long    Fgetchar( int f, int mode ) {return 1;}
long    Fputchar( int f, long c, int mode ) {return 1;}
long    Pwait( void ) {return 1;}
int     Pnice( int delta ) {return 1;}
int     Pgetpid( void ) {return 1;}
int     Pgetppid( void ) {return 1;}
int     Pgetpgrp( void ) {return 1;}
int     Psetpgrp( int pid, int newgrp ) {return 1;}
int     Pgetuid( void ) {return 1;}
int     Psetuid( int id ) {return 1;}
int     Pkill( int pid, int sig ) {return 1;}
void    *Psignal( int sig, void *handler ) {return (void*)1;}
int     Pvfork( void ) {return 1;}
int     Pgetgid( void ) {return 1;}
int     Psetgid( int id ) {return 1;}
long    Psigblock( long mask ) {return 1;}
long    Psigsetmask( long mask ) {return 1;}
long    Pusrval( long arg ) {return 1;}
int     Pdomain( int newdom ) {return 1;}
void    Psigreturn( void ) {return;}
int     Pfork( void ) {return 1;}
long    Pwait3( int flag, long *rusage ) {return 1;}
int     Fselect( unsigned int timeout, long *rfds, long *wfds, long *xfds ) {return 1;}
void    Prusage( long *r ) {return;}
long    Psetlimit( int lim, long value ) {return 1;}
long    Talarm( long secs ) {return 1;}
void    Pause( void ) {return;}
long    Sysconf( int n ) {return 1;}
long    Psigpending( void ) {return 1;}
long    Dpathconf( char *name, int n ) {return 1;}
long    Pmsg( int mode, long mbox, MSG *msg ) {return 1;}
long    Fmidipipe( int pid, int in, int out ) {return 1;}
int     Prenice( int pid, int delta ) {return 1;}
long    Dopendir( char *name, int flag ) {return 1;}
long    Dreaddir( int buflen, long dir, char *buf ) {return 1;}
long    Drewinddir( long dir ) {return 1;}
long    Dclosedir( long dir ) {return 1;}
long    Fxattr( int flag, char *name, XATTR *buf ) {return 1;}
long    Flink( char *oldname, char *newname ) {return 1;}
long    Fsymlink( char *oldname, char *newname ) {return 1;}
long    Freadlink( int size, char *buf, char *name ) {return 1;}
long    Dcntl( int cmd, char *name, long arg ) {return 1;}
long    Fchown( char *name, int uid, int gid ) {return 1;}
long    Fchmod( char *name, int mode ) {return 1;}
long    Pumask( unsigned int mode ) {return 1;}
long    Psemaphore( int mode, long id, long timeout ) {return 1;}
long    Dlock( int mode, int drive ) {return 1;}
void    Psigpause( unsigned long mask ) {return;}
long    Psigaction( int sig, SIGACTION *act, SIGACTION *oact ) {return 1;}
int     Pgeteuid( void ) {return 1;}
int     Pgetegid( void ) {return 1;}
long    Pwaitpid( int pid, int flag, long *rusage ) {return 1;}

long    locksnd( void ) {return 1;}
long    unlocksnd( void ) {return 1;}
long    soundcmd( int mode, int data ) {return 1;}
long    setbuffer( int reg, void *begaddr, void *endaddr ) {return 1;}
long    setmode( int mode ) {return 1;}
long    settracks( int playtracks, int rectracks ) {return 1;}
long    setmontracks( int montrack ) {return 1;}
long    setinterrupt( int src_inter, int cause ) {return 1;}
long    buffoper( int mode ) {return 1;}
long    dsptristate( int dspxmit, int dsprec ) {return 1;}
long    gpio( int mode, int data ) {return 1;}
long    devconnect( int src, int dst, int srcclk, int prescale,
                    int protocol ) {return 1;}
long    sndstatus( int reset ) {return 1;}
long    buffptr( long *ptr ) {return 1;}

int     Dsp_LoadProg( char *file, int ability, char *buffer ) {return 1;}
long    Dsp_LodToBinary( char *file, char *codeptr ) {return 1;}
void    Dsp_DoBlock( char *data_in, long size_in, char *data_out,
                     long size_out ) {return;}
void    Dsp_BlkHandShake( char *data_in, long size_in, char *data_out,
                          long size_out ) {return;}
void    Dsp_BlkUnpacked( long *data_in, long size_in, long *data_out,
                         long size_out ) {return;}
void    Dsp_InStream( char *data_in, long block_size, long num_blocks,
                      long *blocks_done ) {return;}
void    Dsp_OutStream( char *data_out, long block_size, long num_blocks,
                       long *blocks_done ) {return;}
void    Dsp_IOStream( char *data_in, char *data_out, long block_insize,
                      long block_outsize, long num_blocks,
                      long *blocks_done ) {return;}
void    Dsp_RemoveInterrupts( int mask ) {return;}
int     Dsp_GetWordSize( void ) {return 1;}
int     Dsp_Lock( void ) {return 1;}
void    Dsp_Unlock( void ) {return;}
void    Dsp_Available( long *xavailable, long *yavailable ) {return;}
int     Dsp_Reserve( long xreserve, long yreserve ) {return 1;}
void    Dsp_ExecProg( char *codeptr, long codesize, int ability ) {return;}
void    Dsp_ExecBoot( char *codeptr, long codesize, int ability ) {return;}
void    Dsp_TriggerHC( int vector ) {return;}
int     Dsp_RequestUniqueAbility( void ) {return 1;}
int     Dsp_GetProgAbility( void ) {return 1;}
void    Dsp_FlushSubroutines( void ) {return;}
int     Dsp_LoadSubroutine( char *codeptr, long codesize, int ability ) {return 1;}
int     Dsp_InqSubrAbility( int ability ) {return 1;}
int     Dsp_RunSubroutine( int handle ) {return 1;}
int     Dsp_Hf0( int flag ) {return 1;}
int     Dsp_Hf1( int flag ) {return 1;}
int     Dsp_Hf2( void ) {return 1;}
int     Dsp_Hf3( void ) {return 1;}
void    Dsp_BlkWords( void *data_in, long size_in, void *data_out,
                      long size_out ) {return;}
void    Dsp_BlkBytes( void *data_in, long size_in, void *data_out,
                      long size_out ) {return;}
char    Dsp_HStat( void ) {return 1;}
void    Dsp_SetVectors( void (*receiver)( ), long (*transmitter)( )) {return;}
void    Dsp_MultBlocks( long numsend, long numreceive,
                        DSPBLOCK *sendblocks, DSPBLOCK *receiveblocks ) {return;}
int     Vsetmode( int modecode ) {return 1;}
int     mon_type( void ) {return 1;}
long    VgetSize( int mode ) {return 1;}
void    VsetSync( int flag ) {return;}
void    VsetRGB( int index, int count, long *array ) {return;}
void    VgetRGB( int index, int count, long *array ) {return;}
void    VsetMask( int ormask, int andmask, int overlay ) {return;}

#include <vdi.h>

void v_opnwk( int *work_in, int *handle, int *work_out) {return;}
void v_clswk( int handle ) {return;}
void v_opnvwk( int *work_in, int *handle, int *work_out) {return;}
void v_clsvwk( int handle ) {return;}
void v_clrwk( int handle ) {return;}
void v_updwk( int handle ) {return;}
int vst_load_fonts( int handle, int select ) {return 1;}
void vst_unload_fonts( int handle, int select ) {return;}
void vs_clip( int handle, int clip_flag, int *pxyarray ) {return;}
void    v_pline( int handle, int count, int *pxyarray ) {return;}
void    v_pmarker( int handle, int count, int *pxyarray ) {return;}
void    v_gtext( int handle, int x, int y, char *string ) {return;}
void    v_fillarea( int handle, int count, int *pxyarray ) {return;}
void    v_cellarray( int handle, int *pxyarray, int row_length,
                     int el_used, int num_rows, int wrt_mode,
                     int *colarray ) {return;}
void    v_contourfill( int handle, int x, int y, int index ) {return;}
void    vr_recfl( int handle, int *pxyarray ) {return;}
void    v_bar( int handle, int *pxyarray ) {return;}
void    v_arc( int handle, int x, int y, int radius,
               int begang, int endang ) {return;}
void    v_pieslice( int handle, int x, int y, int radius,
                    int begang, int endang ) {return;}
void    v_circle( int handle, int x, int y, int radius ) {return;}
void    v_ellarc( int handle, int x, int y, int xradius,
                  int yradius, int begang, int endang ) {return;}
void    v_ellpie( int handle, int x, int y, int xradius,
                  int yradius, int begang, int endang ) {return;}
void    v_ellipse( int handle, int x, int y, int xradius,
                   int yradius  ) {return;}
void    v_rbox  ( int handle, int *pxyarray ) {return;}
void    v_rfbox ( int handle, int *pxyarray ) {return;}
void    v_justified( int handle,int x, int y, char *string,
                     int length, int word_space,
					 int char_space ) {return;}

int     vswr_mode( int handle, int mode ) {return 1;}
void    vs_color( int handle, int index, int *rgb_in ) {return;}
int     vsl_type( int handle, int style ) {return 1;}
void    vsl_udsty( int handle, int pattern ) {return;}
int     vsl_width( int handle, int width ) {return 1;}
int     vsl_color( int handle, int color_index ) {return 1;}
void    vsl_ends( int handle, int beg_style, int end_style ) {return;}
int     vsm_type( int handle, int symbol ) {return 1;}
int     vsm_height( int handle, int height ) {return 1;}
int     vsm_color( int handle, int color_index ) {return 1;}
void    vst_height( int handle, int height, int *char_width,
                    int *char_height, int *cell_width,
                    int *cell_height ) {return;}
int     vst_point( int handle, int point, int *char_width,
                    int *char_height, int *cell_width,
                    int *cell_height ) {return 1;}
int     vst_rotation( int handle, int angle ) {return 1;}
int     vst_font( int handle, int font ) {return 1;}
int     vst_color( int handle, int color_index ) {return 1;}
int     vst_effects( int handle, int effect ) {return 1;}
void    vst_alignment( int handle, int hor_in, int vert_in,
                       int *hor_out, int *vert_out ) {return;}
int     vsf_interior( int handle, int style ) {return 1;}
int     vsf_style( int handle, int style_index ) {return 1;}
int     vsf_color( int handle, int color_index ) {return 1;}
int     vsf_perimeter( int handle, int per_vis ) {return 1;}
void    vsf_udpat( int handle, int *pfill_pat, int planes ) {return;}

void    vro_cpyfm( int handle, int vr_mode, int *pxyarray,
                   MFDB *psrcMFDB, MFDB *pdesMFDB ) {return;}
void    vrt_cpyfm( int handle, int vr_mode, int *pxyarray,
                   MFDB *psrcMFDB, MFDB *pdesMFDB,
                   int *color_index ) {return;}
void    vr_trnfm( int handle, MFDB *psrcMFDB, MFDB *pdesMFDB ) {return;}
void    v_get_pixel( int handle, int x, int y, int *pel,
                     int *index ) {return;}

void    vsin_mode( int handle, int dev_type, int mode ) {return;}
void    vrq_locator( int handle, int x, int y, int *xout,
                     int *yout, int *term ) {return;}
int     vsm_locator( int handle, int x, int y, int *xout,
                     int *yout, int *term ) {return 1;}
void    vrq_valuator( int handle, int valuator_in,
                      int *valuator_out, int *terminator ) {return;}
void    vsm_valuator( int handle, int val_in, int *val_out,
                      int *term, int *status ) {return;}
void    vrq_choice( int handle, int ch_in, int *ch_out ) {return;}
int     vsm_choice( int handle, int *choice ) {return 1;}
void    vrq_string( int handle, int max_length, int echo_mode,
                    int *echo_xy, char *string ) {return;}
int     vsm_string( int handle, int max_length, int echo_mode,
                    int *echo_xy, char *string ) {return 1;}
void    vsc_form( int handle, int *pcur_form ) {return;}
void    vex_timv( int handle, int (*tim_addr)(), int (**otim_addr)(),
                  int *tim_conv ) {return;}
void    v_show_c( int handle, int reset ) {return;}
void    v_hide_c( int handle ) {return;}
void    vq_mouse( int handle, int *pstatus, int *x, int *y ) {return;}
void    vex_butv( int handle, int (*pusrcode)(), int (**psavcode)() ) {return;}
void    vex_motv( int handle, int (*pusrcode)(), int (**psavcode)() ) {return;}
void    vex_curv( int handle, int (*pusrcode)(), int (**psavcode)() ) {return;}
void    vq_key_s( int handle, int *pstatus ) {return;}

void    vq_extnd( int handle, int owflag, int *work_out ) {return;}
int     vq_color( int handle, int color_index,
                  int set_flag, int *rgb ) {return 1;}
void    vql_attributes( int handle, int *attrib ) {return;}
void    vqm_attributes( int handle, int *attrib ) {return;}
void    vqf_attributes( int handle, int *attrib ) {return;}
void    vqt_attributes( int handle, int *attrib ) {return;}
void    vqt_extent( int handle, char *string, int *extent ) {return;}
int     vqt_width( int handle, int character,
                   int *cell_width, int *left_delta,
                   int *right_delta ) {return 1;}
int     vqt_name( int handle, int element_num, char *name ) {return 1;}
void    vq_cellarray( int handle, int *pxyarray,
                      int row_length, int num_rows,
                      int *el_used, int *rows_used,
                      int *status, int *colarray ) {return;}
void    vqin_mode( int handle, int dew_type, int *input_mode ) {return;}
void    vqt_fontinfo( int handle, int *minADE, int *maxADE,
                      int *distances, int *maxwidth,
                      int *effects ) {return;}

void    vq_chcells( int handle, int *rows, int *columns ) {return;}
void    v_exit_cur( int handle ) {return;}
void    v_enter_cur( int handle ) {return;}
void    v_curup( int handle ) {return;}
void    v_curdown( int handle ) {return;}
void    v_curright( int handle ) {return;}
void    v_curleft( int handle ) {return;}
void    v_curhome( int handle ) {return;}
void    v_eeos( int handle ) {return;}
void    v_eeol( int handle ) {return;}
void    vs_curaddress( int handle, int row, int column ) {return;}
void    v_curaddress( int handle, int row, int column ) {return;}
void    v_curtext( int handle, char *string ) {return;}
void    v_rvon( int handle ) {return;}
void    v_rvoff( int handle ) {return;}
void    vq_curaddress( int handle, int *row, int *column ) {return;}
int     vq_tabstatus( int handle ) {return 1;}
void    v_hardcopy( int handle ) {return;}
void    v_dspcur( int handle, int x, int y ) {return;}
void    v_rmcur( int handle ) {return;}
void    v_form_adv( int handle ) {return;}
void    v_output_window( int handle, int *xyarray ) {return;}
void    v_clear_disp_list( int handle ) {return;}
void    v_bit_image( int handle, const char *filename,
                     int aspect, int x_scale, int y_scale,
                     int h_align, int v_align, int *xyarray ) {return;}
void    vq_scan( int handle, int *g_slice, int *g_page,
                 int *a_slice, int *a_page, int *div_fac) {return;}
void    v_alpha_text( int handle, char *string ) {return;}
void    vs_palette( int handle, int palette ) {return;}
void	v_sound( int handle, int frequency, int duration ) {return;}
int		vs_mute( int handle, int action ) {return 1;}
void    vqp_films( int handle, char *film_names ) {return;}
void    vqp_state( int handle, int *port, char *film_name,
                   int *lightness, int *interlace,
                   int *planes, int *indexes ) {return;}
void    vsp_state( int handle, int port, int film_num,
                   int lightness, int interlace, int planes,
                   int *indexes ) {return;}
void    vsp_save( int handle ) {return;}
void    vsp_message( int handle ) {return;}
int     vqp_error( int handle ) {return 1;}
void    v_meta_extents( int handle, int min_x, int min_y,
                        int max_x, int max_y ) {return;}
void    v_write_meta( int handle,
                      int num_intin, int *intin,
                      int num_ptsin, int *ptsin ) {return;}
void    vm_coords( int handle, int llx, int lly, int urx, int ury ) {return;}
void    vm_filename( int handle, const char *filename ) {return;}
void    vm_pagesize( int handle, int pgwidth, int pdheight ) {return;}
void    v_offset( int handle, int offset ) {return;}
void    v_fontinit( int handle, int fh_high, int fh_low ) {return;}
void    v_escape2000( int handle, int times ) {return;}

void    vt_resolution( int handle, int xres, int yres,
                       int *xset, int *yset ) {return;}
void    vt_axis( int handle, int xres, int yres,
                 int *xset, int *yset ) {return;}
void    vt_origin( int handle, int xorigin, int yorigin ) {return;}
void    vq_tdimensions( int handle, int *xdimension, int *ydimension ) {return;}
void    vt_alignment( int handle, int dx, int dy ) {return;}
void    vsp_film( int handle, int index, int lightness ) {return;}
void    vsc_expose( int handle, int state ) {return;}

int     vq_gdos( void ) {return 1;}
long    vq_vgdos( void ) {return 1;}

int     v_bez_on( int handle ) {return 1;}
void    v_bez_off( int handle ) {return;}
void    v_set_app_buff( int handle, void *address, int nparagraphs ) {return;}
void    v_bez( int handle, int count, int *xyarr,
                char *bezarr, int *extent, int *totpts, int *totmoves ) {return;}
void    v_bez_fill( int handle, int count, int *xyarr,
                     char *bezarr, int *extent, int *totpts,
                     int *totmoves ) {return;}
int     v_bez_qual( int handle, int prcnt, int *actual ) {return 1;}

void    vqt_f_extent( int handle, char *string, int *extent ) {return;}
void    v_ftext( int handle, int x, int y, char *string ) {return;}
void    v_ftext_offset( int handle, int x, int y, char *string, int *offset ) {return;}
void    v_killoutline( int handle, void *component ) {return;}
void    v_getoutline( int handle, int ch, int *xyarray,
                      char *bezarray, int maxverts, int *numverts ) {return;}
void    vst_scratch( int handle, int mode ) {return;}
void    vst_error( int handle, int mode, int *errorvar ) {return;}
void    vqt_advance( int handle, int ch, int *advx, int *advy,
                     int *remx, int *remy ) {return;}
void    vqt_advance32( int handle, int ch, fix31 *advx, fix31 *advy ) {return;}
int     vst_arbpt( int handle, int point, int *chwd, int *chht,
                   int *cellwd, int *cellht ) {return 1;}
fix31   vst_arbpt32( int handle, fix31 point, int *chwd, int *chht,
                     int *cellwd, int *cellht ) {return (fix31)1;}
void    vst_charmap( int handle, int mode ) {return;}
void    v_getbitmap_info( int handle, int ch, fix31 *advx, fix31 *advy,
                          fix31 *xoff, fix31 *yoff, fix31 *width,
                          fix31 *height ) {return;}
void    vqt_pairkern( int handle, int ch1, int ch2, fix31 *x, fix31 *y ) {return;}
void    vqt_trackkern( int handle, fix31 *x, fix31 *y ) {return;}
void    vqt_fontheader( int handle, char *buffer, char *pathname ) {return;}
void    vst_kern( int handle, int tmode, int pmode, int *tracks,
                  int *pairs ) {return;}
fix31   vst_setsize32( int handle, fix31 point, int *chwd, int *chht,
                       int *cellwd, int *cellht ) {return (fix31)1;}
void    vqt_devinfo( int handle, int devnum, int *devexits,
                     char *devstr ) {return;}
int     v_flushcache( int handle ) {return 1;}
void    vqt_cachesize( int handle, int which_cache, long *size ) {return;}
void    vqt_get_table( int handle, int **map ) {return;}
int     v_loadcache( int handle, char *filename, int mode ) {return 1;}
int     v_savecache( int handle, char *filename ) {return 1;}
int     vst_setsize( int handle, int point, int *chwd, int *chht,
                     int *cellwd, int *cellht ) {return 1;}
int     vst_skew( int handle, int skew ) {return 1;}

/* GLOBALS */
GEMPARBLK _GemParBlk;
VDIPARBLK _VDIParBlk;
int       _app;

#include "big2.h"
int img_attach (t_image_dial *bimg, char *fichier, int tree, int obj, int fcenterw, int fcenterh) {return 1; }


