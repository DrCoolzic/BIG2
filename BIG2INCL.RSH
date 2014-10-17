/* GEM Resource C Source */

#include <portab.h>
#include <aes.h>
#include "BIG2INCL.H"

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
{ "T",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x11A1, 0, 1, 2, 1,
  " I",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, 1, 3, 1,
  "12345678901234567890123456789012345678901",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, 0, 42, 1,
  "___________",
  "S\202lection : ________.___",
  "FFFFFFFFFFF",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 25,
  "12345678.EXT",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, 0, 13, 1,
  "Utilisation du s\202lecteur",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 25, 1
};

WORD RSIB0MASK[] =
{ 0x0000, 0x03C0, 0x0FF0, 0x1FF8, 
  0x3FFC, 0x3FFC, 0x7FFE, 0x7FFE, 
  0x7FFE, 0x7FFE, 0x3FFC, 0x3FFC, 
  0x1FF8, 0x0FF0, 0x03C0, 0x0000
};

WORD RSIB0DATA[] =
{ 0x0000, 0x03C0, 0x0C30, 0x1008, 
  0x23CC, 0x242C, 0x4816, 0x4816, 
  0x4816, 0x4816, 0x242C, 0x23CC, 
  0x1C38, 0x0FF0, 0x03C0, 0x0000
};

WORD RSIB1MASK[] =
{ 0x0000, 0x03C0, 0x0FF0, 0x1FF8, 
  0x3FFC, 0x3FFC, 0x7FFE, 0x7FFE, 
  0x7FFE, 0x7FFE, 0x3FFC, 0x3FFC, 
  0x1FF8, 0x0FF0, 0x03C0, 0x0000
};

WORD RSIB1DATA[] =
{ 0x0000, 0x03C0, 0x0FF0, 0x1C38, 
  0x3184, 0x37E4, 0x67E2, 0x6FF2, 
  0x6FF2, 0x67E2, 0x37E4, 0x3184, 
  0x1008, 0x0C30, 0x03C0, 0x0000
};

WORD RSIB2MASK[] =
{ 0x0FF0, 0x3FFC, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0x3FFC, 0x0FF0
};

WORD RSIB2DATA[] =
{ 0x0FF0, 0x381C, 0xE3C7, 0xCC33, 
  0xCC33, 0xE3C7, 0x381C, 0x0FF0
};

WORD RSIB3MASK[] =
{ 0x0FF0, 0x3FFC, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0x3FFC, 0x0FF0
};

WORD RSIB3DATA[] =
{ 0x0FF0, 0x381C, 0xE3C7, 0xCFF3, 
  0xCFF3, 0xE3C7, 0x381C, 0x0FF0
};

WORD RSIB4MASK[] =
{ 0x0000, 0xFFFE, 0xFFFE, 0xFFFE, 
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE, 
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE, 
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE
};

WORD RSIB4DATA[] =
{ 0x0000, 0xFFFE, 0x8006, 0x8006, 
  0x8006, 0x8006, 0x8006, 0x8006, 
  0x8006, 0x8006, 0x8006, 0x8006, 
  0x8006, 0x8006, 0xFFFE, 0xFFFE
};

WORD RSIB5MASK[] =
{ 0x0000, 0xFFFE, 0xFFFE, 0xFFFE, 
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE, 
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE, 
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE
};

WORD RSIB5DATA[] =
{ 0x0000, 0xFFFE, 0xFFFE, 0xC002, 
  0xD81A, 0xDC3A, 0xCE72, 0xC7E2, 
  0xC3C2, 0xC3C2, 0xC7E2, 0xCE72, 
  0xDC3A, 0xD81A, 0xC002, 0xFFFE
};

WORD RSIB6MASK[] =
{ 0x0000, 0xFFFC, 0xFFFC, 0xFFFC, 
  0xFFFC, 0xFFFC, 0xFFFC, 0xFFFC
};

WORD RSIB6DATA[] =
{ 0x0000, 0xFFFC, 0xC00C, 0xC00C, 
  0xC00C, 0xC00C, 0xC00C, 0xFFFC
};

WORD RSIB7MASK[] =
{ 0x0000, 0xFFFC, 0xFFFC, 0xFFFC, 
  0xFFFC, 0xFFFC, 0xFFFC, 0xFFFC
};

WORD RSIB7DATA[] =
{ 0x0000, 0xFFFC, 0xF03C, 0xCCCC, 
  0xC30C, 0xCCCC, 0xF03C, 0xFFFC
};

WORD RSIB8MASK[] =
{ 0x07E0, 0x7FF8, 0x7FFC, 0x7FFE, 
  0x7E7E, 0x7E1F, 0xFE1F, 0xF00F, 
  0xF00F, 0xF87F, 0xF87E, 0x7E7E, 
  0x7FFE, 0x3FFE, 0x1FFE
};

WORD RSIB8DATA[] =
{ 0x0000, 0x03C0, 0x2FF0, 0x3C38, 
  0x380C, 0x3C0C, 0x0006, 0x6006, 
  0x6006, 0x6000, 0x303C, 0x301C, 
  0x1C3C, 0x0FF4, 0x03C0
};

WORD RSIB9MASK[] =
{ 0xFFFF, 0xFFFF, 0xFE7F, 0xFE7F, 
  0xFE7F, 0xFFFF, 0xFFFF
};

WORD RSIB9DATA[] =
{ 0x47F0, 0x7C1E, 0x7800, 0x7C3E, 
  0x001E, 0x783E, 0x0FE2
};

WORD RSIB10MASK[] =
{ 0x0000, 0x7FBC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x3FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x0000, 0x0000
};

WORD RSIB10DATA[] =
{ 0x0000, 0x0000, 0x3F18, 0x3E38, 
  0x3C78, 0x3FF8, 0x37F8, 0x27F8, 
  0x07F8, 0x0FF8, 0x1FF8, 0x3FF8, 
  0x3FF8, 0x0000, 0x0000, 0x0000
};

WORD RSIB11MASK[] =
{ 0x0000, 0x1FF0, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x1FF0, 0x0000, 0x0000
};

WORD RSIB11DATA[] =
{ 0x0000, 0x0000, 0x0FE0, 0x27C8, 
  0x3398, 0x3938, 0x3C78, 0x3EF8, 
  0x3C78, 0x3938, 0x3398, 0x27C8, 
  0x0FE0, 0x0000, 0x0000, 0x0000
};

WORD RSIB12MASK[] =
{ 0xFFFE, 0xFFFE, 0xFFFE, 0x7FFC, 
  0x3FF8, 0x1FF0, 0x0FE0, 0x07C0, 
  0x0FE0, 0x0FE0, 0x0FE0, 0x0FE0, 
  0x0FE0, 0x0FE0, 0x0FE0, 0x0000
};

WORD RSIB12DATA[] =
{ 0x0000, 0x7FFC, 0x3018, 0x1830, 
  0x0C60, 0x06C0, 0x0380, 0x0100, 
  0x0000, 0x07C0, 0x07C0, 0x0440, 
  0x0440, 0x07C0, 0x0000, 0x0000
};

WORD RSIB13MASK[] =
{ 0x0000, 0xFFC0, 0xFFE0, 0xFFF0, 
  0x7FF8, 0x3FFC, 0x1FFE, 0x0FFE, 
  0x1FFE, 0x3FFC, 0x7FF8, 0xFFF0, 
  0xFFE0, 0xFFC0, 0x0000, 0x0000
};

WORD RSIB13DATA[] =
{ 0x0000, 0x0000, 0x7380, 0x39C0, 
  0x1CE0, 0x0E70, 0x0738, 0x039C, 
  0x0738, 0x0E70, 0x1CE0, 0x39C0, 
  0x7380, 0x0000, 0x0000, 0x0000
};

WORD RSIB14MASK[] =
{ 0xFEFE, 0xFEFE, 0xFEFE, 0xFEFE, 
  0xFEFE, 0xFFFE, 0xFFFE, 0x07C0, 
  0xFFFE, 0xFFFE, 0xFEFE, 0xFEFE, 
  0xFEFE, 0xFEFE, 0xFEFE, 0x0000
};

WORD RSIB14DATA[] =
{ 0x0000, 0x7C7C, 0x783C, 0x783C, 
  0x7C7C, 0x4824, 0x0380, 0x0380, 
  0x0380, 0x4824, 0x7C7C, 0x783C, 
  0x783C, 0x7C7C, 0x0000, 0x0000
};

WORD RSIB15MASK[] =
{ 0x0000, 0x73FC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FF8, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x0000, 0x0000
};

WORD RSIB15DATA[] =
{ 0x0000, 0x0000, 0x31F8, 0x38F8, 
  0x3C78, 0x3FF8, 0x3FD8, 0x3FC8, 
  0x3FC0, 0x3FE0, 0x3FF0, 0x3FF8, 
  0x3FF8, 0x0000, 0x0000, 0x0000
};

WORD RSIB16MASK[] =
{ 0x0000, 0x0380, 0x07C0, 0x0FE0, 
  0x1FF0, 0x3FF8, 0x7FFC, 0xFFFE, 
  0xFFFE, 0xFFFE, 0x0FE0, 0x0FE0, 
  0x0FE0, 0x0FE0, 0x0000, 0x0000
};

WORD RSIB16DATA[] =
{ 0x0000, 0x0000, 0x0100, 0x0380, 
  0x06C0, 0x0C60, 0x1830, 0x3018, 
  0x7C7C, 0x0440, 0x0440, 0x0440, 
  0x07C0, 0x0000, 0x0000, 0x0000
};

WORD RSIB17MASK[] =
{ 0x0080, 0x01C0, 0x03E0, 0x07F0, 
  0x0FF8, 0x1FFC, 0x3FFE, 0x03E0, 
  0x3FFE, 0x1FFC, 0x0FF8, 0x07F0, 
  0x03E0, 0x01C0, 0x0080, 0x0000
};

WORD RSIB17DATA[] =
{ 0x0080, 0x01C0, 0x0360, 0x0630, 
  0x0C18, 0x180C, 0x3E3E, 0x0220, 
  0x3E3E, 0x180C, 0x0C18, 0x0630, 
  0x0360, 0x01C0, 0x0080, 0x0000
};

WORD RSIB18MASK[] =
{ 0x0000, 0x0FE0, 0x0FE0, 0x0FE0, 
  0x0FE0, 0xFFFE, 0xFFFE, 0xFFFE, 
  0x7FFC, 0x3FF8, 0x1FF0, 0x0FE0, 
  0x07C0, 0x0380, 0x0000, 0x0000
};

WORD RSIB18DATA[] =
{ 0x0000, 0x0000, 0x07C0, 0x0440, 
  0x0440, 0x0440, 0x7C7C, 0x3018, 
  0x1830, 0x0C60, 0x06C0, 0x0380, 
  0x0100, 0x0000, 0x0000, 0x0000
};

WORD RSIB19MASK[] =
{ 0x0000, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x3FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FBC, 0x0000, 0x0000
};

WORD RSIB19DATA[] =
{ 0x0000, 0x0000, 0x3FF8, 0x3FF8, 
  0x1FF8, 0x0FF8, 0x07F8, 0x27F8, 
  0x37F8, 0x3FF8, 0x3C78, 0x3E38, 
  0x3F18, 0x0000, 0x0000, 0x0000
};

WORD RSIB20MASK[] =
{ 0x01C0, 0x03C0, 0x07C0, 0x0FC0, 
  0x1FFC, 0x3FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x3FFC, 0x1FFC, 0x0FC0, 
  0x07C0, 0x03C0, 0x01C0, 0x0000
};

WORD RSIB20DATA[] =
{ 0x0000, 0x0080, 0x0180, 0x0380, 
  0x0680, 0x0CF8, 0x1808, 0x3008, 
  0x1808, 0x0CF8, 0x0680, 0x0380, 
  0x0180, 0x0080, 0x0000, 0x0000
};

WORD RSIB21MASK[] =
{ 0x0000, 0x0000, 0x0280, 0x06C0, 
  0x0EE0, 0x1EF0, 0x3FF8, 0x7FFC, 
  0xFFFE, 0x7FFC, 0x3FF8, 0x1EF0, 
  0x0EE0, 0x06C0, 0x0280, 0x0000
};

WORD RSIB21DATA[] =
{ 0x0000, 0x0000, 0x0280, 0x06C0, 
  0x0EE0, 0x1AB0, 0x3398, 0x600C, 
  0xC006, 0x600C, 0x3398, 0x1AB0, 
  0x0EE0, 0x06C0, 0x0280, 0x0000
};

WORD RSIB22MASK[] =
{ 0x0700, 0x0780, 0x07C0, 0x07E0, 
  0x7FF0, 0x7FF8, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FF8, 0x7FF0, 0x07E0, 
  0x07C0, 0x0780, 0x0700, 0x0000
};

WORD RSIB22DATA[] =
{ 0x0000, 0x0200, 0x0300, 0x0380, 
  0x02C0, 0x3E60, 0x2030, 0x2018, 
  0x2030, 0x3E60, 0x02C0, 0x0380, 
  0x0300, 0x0200, 0x0000, 0x0000
};

WORD RSIB23MASK[] =
{ 0x0000, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FF8, 0x7FF8, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7BFC, 0x0000, 0x0000
};

WORD RSIB23DATA[] =
{ 0x0000, 0x0000, 0x3FF8, 0x3FF8, 
  0x3FF0, 0x3FE0, 0x3FC0, 0x3FC8, 
  0x3FD8, 0x3FF8, 0x3C78, 0x38F8, 
  0x31F8, 0x0000, 0x0000, 0x0000
};

WORD RSIB24MASK[] =
{ 0x7FBC, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x7FFC, 0x0000
};

WORD RSIB24DATA[] =
{ 0x0000, 0x3F18, 0x3C78, 0x37F8, 
  0x07F8, 0x3FF8, 0x0000, 0x0000
};

WORD RSIB25MASK[] =
{ 0x0FE0, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x0FE0, 0x0000
};

WORD RSIB25DATA[] =
{ 0x0000, 0x07C0, 0x3118, 0x3838, 
  0x3118, 0x07C0, 0x0000, 0x0000
};

WORD RSIB26MASK[] =
{ 0x1FF8, 0x1FF8, 0x1E78, 0x07E0, 
  0x07E0, 0x07E0, 0x07E0, 0x07E0
};

WORD RSIB26DATA[] =
{ 0x0000, 0x0FF0, 0x0240, 0x0180, 
  0x0000, 0x03C0, 0x03C0, 0x0000
};

WORD RSIB27MASK[] =
{ 0x1FE0, 0x1FF0, 0x1FF8, 0x0FF8, 
  0x1FF8, 0x1FF0, 0x1FE0, 0x0000
};

WORD RSIB27DATA[] =
{ 0x0000, 0x0CC0, 0x0660, 0x0330, 
  0x0660, 0x0CC0, 0x0000, 0x0000
};

WORD RSIB28MASK[] =
{ 0x3FFC, 0x3FFC, 0x3FFC, 0x3FFC, 
  0x3FFC, 0x3FFC, 0x3FFC, 0x3FFC
};

WORD RSIB28DATA[] =
{ 0x1E78, 0x1818, 0x1428, 0x0180, 
  0x1428, 0x1818, 0x1E78, 0x0000
};

WORD RSIB29MASK[] =
{ 0x3DFE, 0x3FFE, 0x3FFE, 0x3FFE, 
  0x3FFE, 0x3FFE, 0x3FFE, 0x0000
};

WORD RSIB29DATA[] =
{ 0x0000, 0x18FC, 0x1E3C, 0x1FEC, 
  0x1FE0, 0x1FFC, 0x0000, 0x0000
};

WORD RSIB30MASK[] =
{ 0x07C0, 0x0FE0, 0x1FF0, 0x1FF0, 
  0x1FF0, 0x07C0, 0x07C0, 0x07C0
};

WORD RSIB30DATA[] =
{ 0x0100, 0x0280, 0x0440, 0x0EE0, 
  0x0280, 0x0280, 0x0380, 0x0000
};

WORD RSIB31MASK[] =
{ 0x0100, 0x0380, 0x07C0, 0x0380, 
  0x0380, 0x07C0, 0x0380, 0x0100
};

WORD RSIB31DATA[] =
{ 0x0100, 0x0280, 0x06C0, 0x0280, 
  0x0280, 0x06C0, 0x0280, 0x0100
};

WORD RSIB32MASK[] =
{ 0x07C0, 0x07C0, 0x1FF0, 0x1FF0, 
  0x1FF0, 0x0FE0, 0x07C0, 0x0380
};

WORD RSIB32DATA[] =
{ 0x0380, 0x0280, 0x0280, 0x0EE0, 
  0x0440, 0x0280, 0x0100, 0x0000
};

WORD RSIB33MASK[] =
{ 0x7FFC, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x7FBC, 0x0000
};

WORD RSIB33DATA[] =
{ 0x0000, 0x3FF8, 0x07F8, 0x37F8, 
  0x3C78, 0x3F18, 0x0000, 0x0000
};

WORD RSIB34MASK[] =
{ 0x0780, 0x0FF0, 0x1FF0, 0x1FF0, 
  0x1FF0, 0x0FF0, 0x0780, 0x0380
};

WORD RSIB34DATA[] =
{ 0x0100, 0x0300, 0x05E0, 0x0820, 
  0x05E0, 0x0300, 0x0100, 0x0000
};

WORD RSIB35MASK[] =
{ 0x0000, 0x0240, 0x07E0, 0x0FF0, 
  0x07E0, 0x0240, 0x0000, 0x0000
};

WORD RSIB35DATA[] =
{ 0x0000, 0x0240, 0x07E0, 0x0810, 
  0x07E0, 0x0240, 0x0000, 0x0000
};

WORD RSIB36MASK[] =
{ 0x01E0, 0x0FF0, 0x0FF8, 0x0FF8, 
  0x0FF8, 0x0FF0, 0x01E0, 0x01C0
};

WORD RSIB36DATA[] =
{ 0x0080, 0x00C0, 0x07A0, 0x0410, 
  0x07A0, 0x00C0, 0x0080, 0x0000
};

WORD RSIB37MASK[] =
{ 0x7FFC, 0x7FFC, 0x7FFC, 0x7FFC, 
  0x7FFC, 0x7FFC, 0x7BFC, 0x0000
};

WORD RSIB37DATA[] =
{ 0x0000, 0x3FF8, 0x3FC0, 0x3FD8, 
  0x3C78, 0x31F8, 0x0000, 0x0000
};

ICONBLK rs_iconblk[] =
{ RSIB0MASK, RSIB0DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB1MASK, RSIB1DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB2MASK, RSIB2DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB3MASK, RSIB3DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB4MASK, RSIB4DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB5MASK, RSIB5DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB6MASK, RSIB6DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB7MASK, RSIB7DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB8MASK, RSIB8DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  15,   0,   0,   0,   8,
  RSIB9MASK, RSIB9DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   7,   0,   0,   0,   8,
  RSIB10MASK, RSIB10DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB11MASK, RSIB11DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB12MASK, RSIB12DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB13MASK, RSIB13DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB14MASK, RSIB14DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB15MASK, RSIB15DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB16MASK, RSIB16DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB17MASK, RSIB17DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB18MASK, RSIB18DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB19MASK, RSIB19DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB20MASK, RSIB20DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB21MASK, RSIB21DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB22MASK, RSIB22DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB23MASK, RSIB23DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,  16,   0,   0,   0,   8,
  RSIB24MASK, RSIB24DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB25MASK, RSIB25DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB26MASK, RSIB26DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB27MASK, RSIB27DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB28MASK, RSIB28DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB29MASK, RSIB29DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB30MASK, RSIB30DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB31MASK, RSIB31DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB32MASK, RSIB32DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB33MASK, RSIB33DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB34MASK, RSIB34DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB35MASK, RSIB35DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB36MASK, RSIB36DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8,
  RSIB37MASK, RSIB37DATA, "\0", 0x1000|'\0',
    0,   0,   0,   0,  16,   8,   0,   0,   0,   8
};

WORD RSBB0DATA[] =
{ 
};

WORD RSBB1DATA[] =
{ 0x3FFF, 0xFFFC, 0x4000, 0x0002, 
  0x8000, 0x0001, 0x8007, 0xE001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x8007, 0xE001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8007, 0xE001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x8007, 0xE001, 0x8000, 0x0001, 
  0x4000, 0x0002, 0x3FFF, 0xFFFC
};

WORD RSBB2DATA[] =
{ 0x3FFF, 0xFFFC, 0x4000, 0x0002, 
  0x803F, 0xFC01, 0x807F, 0xFE01, 
  0x80FF, 0xFF01, 0x81FF, 0xFF81, 
  0x81FF, 0xFF81, 0x81FF, 0xFF81, 
  0x81FF, 0xFF81, 0x81FC, 0x7F81, 
  0x80FC, 0x7F81, 0x8000, 0xFF01, 
  0x8001, 0xFF01, 0x8003, 0xFE01, 
  0x8007, 0xFC01, 0x8007, 0xF801, 
  0x800F, 0xF801, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x8007, 0xE001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8007, 0xE001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x8007, 0xE001, 
  0x4000, 0x0002, 0x3FFF, 0xFFFC
};

WORD RSBB3DATA[] =
{ 0x007F, 0xFE00, 0x00C0, 0x0300, 
  0x01BF, 0xFD80, 0x037F, 0xFEC0, 
  0x06FF, 0xFF60, 0x0DFF, 0xFFB0, 
  0x1BFF, 0xFFD8, 0x37FF, 0xFFEC, 
  0x6FFF, 0xFFF6, 0xDFFF, 0xFFFB, 
  0xB181, 0x860D, 0xA081, 0x0205, 
  0xA4E7, 0x3265, 0xA7E7, 0x3265, 
  0xA3E7, 0x3265, 0xB1E7, 0x3205, 
  0xB8E7, 0x320D, 0xBCE7, 0x327D, 
  0xA4E7, 0x327D, 0xA0E7, 0x027D, 
  0xB1E7, 0x867D, 0xBFFF, 0xFFFD, 
  0xDFFF, 0xFFFB, 0x6FFF, 0xFFF6, 
  0x37FF, 0xFFEC, 0x1BFF, 0xFFD8, 
  0x0DFF, 0xFFB0, 0x06FF, 0xFF60, 
  0x037F, 0xFEC0, 0x01BF, 0xFD80, 
  0x00C0, 0x0300, 0x007F, 0xFE00
};

WORD RSBB4DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0007, 0xE000, 
  0x003F, 0xFC00, 0x00FF, 0xFF00, 
  0x01FF, 0xFF80, 0x03FF, 0xFFC0, 
  0x07FF, 0xFFE0, 0x07FF, 0xFFE0, 
  0x0FFF, 0xFFF0, 0x0FFF, 0xFFF0, 
  0x0FFF, 0xFFF0, 0x1C00, 0x0038, 
  0x1C00, 0x0038, 0x1C00, 0x0038, 
  0x1C00, 0x0038, 0x1C00, 0x0038, 
  0x1C00, 0x0038, 0x0FFF, 0xFFF0, 
  0x0FFF, 0xFFF0, 0x0FFF, 0xFFF0, 
  0x07FF, 0xFFE0, 0x07FF, 0xFFE0, 
  0x03FF, 0xFFC0, 0x01FF, 0xFF80, 
  0x00FF, 0xFF00, 0x003F, 0xFC00, 
  0x0007, 0xE000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

WORD RSBB5DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0001, 0xC000, 0x0073, 0xE000, 
  0x00FB, 0xE700, 0x00FB, 0xEF80, 
  0x1CFB, 0xEF80, 0x3EFB, 0xEF80, 
  0x3EFB, 0xEF80, 0x3EFB, 0xEF80, 
  0x3EFB, 0xEF80, 0x3EFB, 0xEF80, 
  0x3EFB, 0xEF80, 0x3EFB, 0xEF9C, 
  0x3EFB, 0xEFBC, 0x3EFB, 0xEFBC, 
  0x3FFF, 0xFF7C, 0x3FFF, 0xFF7C, 
  0x3FFF, 0xFF7C, 0x3FFF, 0xFEFC, 
  0x3FFF, 0xF7F8, 0x3FFF, 0xDFF8, 
  0x3FFF, 0x7FF0, 0x3FFF, 0xFFF0, 
  0x3FFD, 0xFFE0, 0x1FFD, 0xFFC0, 
  0x0FFF, 0xFF80, 0x07FF, 0xFF00, 
  0x03FF, 0xFE00, 0x03FF, 0xFE00, 
  0x0000, 0x0000, 0x0000, 0x0000
};

WORD RSBB6DATA[] =
{ 0x3FFF, 0xFFFC, 0x4000, 0x0002, 
  0x8000, 0x0001, 0x8007, 0xE001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x8007, 0xE001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x803F, 0xE001, 
  0x803F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x800F, 0xF001, 0x800F, 0xF001, 
  0x801F, 0xF801, 0x807F, 0xFE01, 
  0x807F, 0xFE01, 0x8000, 0x0001, 
  0x4000, 0x0002, 0x3FFF, 0xFFFC
};

WORD RSBB7DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x63C8, 0x0000, 0x7C24, 
  0x001E, 0xFC18, 0x00FF, 0xFE5A, 
  0x03FF, 0xFE00, 0x07FF, 0xFC24, 
  0x0F9F, 0xFC00, 0x0F33, 0xFC00, 
  0x1FE7, 0xFE00, 0x1E7F, 0xFE00, 
  0x1E4F, 0xFE00, 0x3FCF, 0xFF00, 
  0x3FFF, 0xFF00, 0x3FFF, 0xFF00, 
  0x3FFF, 0xFF00, 0x1FFF, 0xFE00, 
  0x1FFF, 0xFE00, 0x1FFF, 0xFE00, 
  0x0FFF, 0xFC00, 0x0FFF, 0xFC00, 
  0x07FF, 0xF800, 0x03FF, 0xF000, 
  0x00FF, 0xC000, 0x001E, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

WORD RSBB8DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x8000, 0x0001, 0xC000, 
  0x0003, 0xE000, 0x0003, 0xF000, 
  0x0007, 0xF800, 0x0007, 0xFC00, 
  0x0017, 0xFE00, 0x0037, 0xFF00, 
  0x006F, 0xFF00, 0x00DF, 0xFE00, 
  0x01BF, 0xFE00, 0x037F, 0xFC00, 
  0x01BF, 0xFD00, 0x02DF, 0xFD80, 
  0x036F, 0xFB00, 0x03B7, 0xF680, 
  0x03DB, 0xED80, 0x03ED, 0xDB80, 
  0x03F6, 0xB780, 0x03FB, 0x6F80, 
  0x01FD, 0xDF00, 0x00FE, 0xBEC0, 
  0x007F, 0x7DC0, 0x003F, 0x7DC0, 
  0x001F, 0x7D80, 0x000F, 0x7800, 
  0x0007, 0x7000, 0x0003, 0x6000, 
  0x0001, 0x4000, 0x0000, 0x0000
};

WORD RSBB9DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0FFF, 0xFDE0, 
  0x1F80, 0x07F0, 0x1F80, 0xE7F8, 
  0x1F80, 0xE7F8, 0x1F80, 0xE7F8, 
  0x1F80, 0xE7F8, 0x1F80, 0xE7F8, 
  0x1F80, 0xE7F8, 0x1FC0, 0x0FF8, 
  0x1FFF, 0xFFF8, 0x1FFF, 0xFFF8, 
  0x1FFF, 0xFFF8, 0x1C00, 0x0038, 
  0x1C00, 0x0038, 0x1CF0, 0x0038, 
  0x1C00, 0x0038, 0x1CFF, 0x6038, 
  0x1C00, 0x0038, 0x1CE4, 0x0038, 
  0x1C00, 0x0038, 0x1CFF, 0x9838, 
  0x1C00, 0x0038, 0x14DF, 0x3038, 
  0x1400, 0x0038, 0x1C00, 0x0038, 
  0x0FFF, 0xFFF0, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

BITBLK rs_bitblk[] =
{ RSBB0DATA,   0,   1,   0,   0, 0x0001,
  RSBB1DATA,   4,  32,   0,   0, 0x0001,
  RSBB2DATA,   4,  32,   0,   0, 0x0001,
  RSBB3DATA,   4,  32,   0,   0, 0x0001,
  RSBB4DATA,   4,  32,   0,   0, 0x0001,
  RSBB5DATA,   4,  32,   0,   0, 0x0001,
  RSBB6DATA,   4,  32,   0,   0, 0x0001,
  RSBB7DATA,   4,  32,   0,   0, 0x0001,
  RSBB8DATA,   4,  32,   0,   0, 0x0001,
  RSBB9DATA,   4,  32,   0,   0, 0x0001
};

BYTE *rs_frstr[] =
{ "[3][ Fichier %s| introuvable.| | File %s not found.][ Quit ]",
  "[3][ Trop de fen\210tres ouvertes !| Too many windows !][ Ok ]",
  " \13433p%s\13433q\134n\134r \13433pNe peut \210tre lanc\202 depuis le dossier AUTO / Don\047t run from AUTO folder\13433q\134n\134r \13433pPressez une touche / Press a key\13433q\134n\134r"
};

OBJECT rs_object[] =
{ 
  /******** Tree 0 BIGALERTE ****************************************************/
        -1, AL_MOVE         , AL_IC           , G_BOX             ,   /* Object 0 AL_BKGR */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0044, 0x0009,
  AL_ST1          ,       -1,       -1, G_IBOX            |0x1100,   /* Object 1 AL_MOVE */
  TOUCHEXIT, CROSSED|OUTLINED, (LONG)0x43FF1100L,
  0x0042, 0x0000, 0x0002, 0x0001,
  AL_ST2          ,       -1,       -1, G_STRING          ,   /* Object 2 AL_ST1 */
  NONE, NORMAL, (LONG)"Texte de remplissage pre. ligne",
  0x0008, 0x0001, 0x001F, 0x0001,
  AL_ST3          ,       -1,       -1, G_STRING          ,   /* Object 3 AL_ST2 */
  NONE, NORMAL, (LONG)"Texte de remplissage deu. ligne",
  0x0008, 0x0002, 0x001F, 0x0001,
  AL_ST4          ,       -1,       -1, G_STRING          ,   /* Object 4 AL_ST3 */
  NONE, NORMAL, (LONG)"Texte de remplissage tro. ligne",
  0x0008, 0x0003, 0x001F, 0x0001,
  AL_ST5          ,       -1,       -1, G_STRING          ,   /* Object 5 AL_ST4 */
  NONE, NORMAL, (LONG)"Texte de remplissage qua. ligne",
  0x0008, 0x0004, 0x001F, 0x0001,
  AL_BU1          ,       -1,       -1, G_STRING          ,   /* Object 6 AL_ST5 */
  NONE, NORMAL, (LONG)"Texte de remplissage cin. ligne",
  0x0008, 0x0005, 0x001F, 0x0001,
  AL_BU2          ,       -1,       -1, G_BUTTON          |0x1E00,   /* Object 7 AL_BU1 */
  SELECTABLE|EXIT|FLAGS10, NORMAL, (LONG)"Texte premier bouton",
  0x0002, 0x0007, 0x0014, 0x0001,
  AL_BU3          ,       -1,       -1, G_BUTTON          |0x1E00,   /* Object 8 AL_BU2 */
  SELECTABLE|EXIT|FLAGS10, NORMAL, (LONG)"Texte deuxi\212m bouton",
  0x0018, 0x0007, 0x0014, 0x0001,
  AL_IC           ,       -1,       -1, G_BUTTON          |0x1E00,   /* Object 9 AL_BU3 */
  SELECTABLE|EXIT|FLAGS10, NORMAL, (LONG)"Texte troisi\212 bouton",
  0x002E, 0x0007, 0x0014, 0x0001,
  AL_BKGR         ,       -1,       -1, G_IMAGE           ,   /* Object 10 AL_IC */
  LASTOB, NORMAL, (LONG)&rs_bitblk[0],
  0x0002, 0x0001, 0x0000, 0x0100,
  
  /******** Tree 1 BIGICN ****************************************************/
        -1,        1,        9, G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x041C, 0x0006,
         2,       -1,       -1, G_IMAGE           ,   /* Object 1  */
  NONE, NORMAL, (LONG)&rs_bitblk[1],
  0x0401, 0x0001, 0x0004, 0x0002,
         3,       -1,       -1, G_IMAGE           ,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_bitblk[2],
  0x0007, 0x0001, 0x0004, 0x0002,
         4,       -1,       -1, G_IMAGE           ,   /* Object 3  */
  NONE, NORMAL, (LONG)&rs_bitblk[3],
  0x040C, 0x0001, 0x0004, 0x0002,
         5,       -1,       -1, G_IMAGE           ,   /* Object 4  */
  NONE, NORMAL, (LONG)&rs_bitblk[4],
  0x0012, 0x0001, 0x0004, 0x0002,
         6,       -1,       -1, G_IMAGE           ,   /* Object 5  */
  NONE, NORMAL, (LONG)&rs_bitblk[5],
  0x0417, 0x0001, 0x0004, 0x0002,
         7,       -1,       -1, G_IMAGE           ,   /* Object 6  */
  NONE, NORMAL, (LONG)&rs_bitblk[6],
  0x0401, 0x0003, 0x0004, 0x0002,
         8,       -1,       -1, G_IMAGE           ,   /* Object 7  */
  NONE, NORMAL, (LONG)&rs_bitblk[7],
  0x0007, 0x0003, 0x0004, 0x0002,
         9,       -1,       -1, G_IMAGE           ,   /* Object 8  */
  NONE, NORMAL, (LONG)&rs_bitblk[8],
  0x040C, 0x0003, 0x0004, 0x0002,
         0,       -1,       -1, G_IMAGE           ,   /* Object 9  */
  LASTOB, NORMAL, (LONG)&rs_bitblk[9],
  0x0012, 0x0003, 0x0004, 0x0002,
  
  /******** Tree 2 BIGARRM ****************************************************/
        -1, BA_LF           , BA_RT           , G_BOX             ,   /* Object 0 BA_BKGR */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0000, 0x0000, 0x0004, 0x0001,
  BA_RT           ,       -1,       -1, G_BOXCHAR         ,   /* Object 1 BA_LF */
  EXIT, DISABLED, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0001,
  BA_BKGR         ,       -1,       -1, G_BOXCHAR         ,   /* Object 2 BA_RT */
  EXIT|LASTOB, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0002, 0x0000, 0x0002, 0x0001,
  
  /******** Tree 3 IMG_USER ****************************************************/
        -1, RBHN            , PUM             , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021141L,
  0x0000, 0x0000, 0x0009, 0x000B,
  RBHS            ,       -1,       -1, G_ICON            ,   /* Object 1 RBHN */
  NONE, NORMAL, (LONG)&rs_iconblk[0],
  0x0002, 0x0001, 0x0002, 0x0001,
  RBMN            ,       -1,       -1, G_ICON            ,   /* Object 2 RBHS */
  NONE, NORMAL, (LONG)&rs_iconblk[1],
  0x0005, 0x0001, 0x0002, 0x0001,
  RBMS            ,       -1,       -1, G_ICON            ,   /* Object 3 RBMN */
  NONE, NORMAL, (LONG)&rs_iconblk[2],
  0x0002, 0x0003, 0x0002, 0x0800,
  BCHN            ,       -1,       -1, G_ICON            ,   /* Object 4 RBMS */
  NONE, NORMAL, (LONG)&rs_iconblk[3],
  0x0005, 0x0003, 0x0002, 0x0800,
  BCHS            ,       -1,       -1, G_ICON            ,   /* Object 5 BCHN */
  NONE, NORMAL, (LONG)&rs_iconblk[4],
  0x0002, 0x0005, 0x0002, 0x0001,
  BCMN            ,       -1,       -1, G_ICON            ,   /* Object 6 BCHS */
  NONE, NORMAL, (LONG)&rs_iconblk[5],
  0x0005, 0x0005, 0x0002, 0x0001,
  BCMS            ,       -1,       -1, G_ICON            ,   /* Object 7 BCMN */
  NONE, NORMAL, (LONG)&rs_iconblk[6],
  0x0002, 0x0007, 0x0002, 0x0800,
  PUH             ,       -1,       -1, G_ICON            ,   /* Object 8 BCMS */
  NONE, NORMAL, (LONG)&rs_iconblk[7],
  0x0005, 0x0007, 0x0002, 0x0800,
  PUM             ,       -1,       -1, G_ICON            ,   /* Object 9 PUH */
  NONE, NORMAL, (LONG)&rs_iconblk[8],
  0x0002, 0x0009, 0x0002, 0x0F00,
         0,       -1,       -1, G_ICON            ,   /* Object 10 PUM */
  LASTOB, NORMAL, (LONG)&rs_iconblk[9],
  0x0005, 0x0009, 0x0002, 0x0800,
  
  /******** Tree 4 BIGARRS ****************************************************/
        -1, SC_UP           , SC_DN           , G_BOX             ,   /* Object 0 SC_BKGR */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0008, 0x0005,
  SC_DN           ,       -1,       -1, G_BOXCHAR         ,   /* Object 1 SC_UP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\001' << 24)|0x00001100L,
  0x0000, 0x0000, 0x0008, 0x0001,
  SC_BKGR         ,       -1,       -1, G_BOXCHAR         ,   /* Object 2 SC_DN */
  LASTOB|TOUCHEXIT, NORMAL, (LONG) ((LONG)'\002' << 24)|0x00001100L,
  0x0000, 0x0004, 0x0008, 0x0001,
  
  /******** Tree 5 BIGLIST ****************************************************/
        -1, BL_BKGR         , BL_BA           , G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0202, 0x0008,
  BL_BA           , BL_S1           , BL_S8           , G_BOX             ,   /* Object 1 BL_BKGR */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0000, 0x0000, 0x0001, 0x0008,
  BL_S2           ,       -1,       -1, G_STRING          ,   /* Object 2 BL_S1 */
  NONE, NORMAL, (LONG)"\0",
  0x0000, 0x0000, 0x0100, 0x0001,
  BL_S3           ,       -1,       -1, G_STRING          ,   /* Object 3 BL_S2 */
  NONE, NORMAL, (LONG)"\0",
  0x0000, 0x0001, 0x0100, 0x0001,
  BL_S4           ,       -1,       -1, G_STRING          ,   /* Object 4 BL_S3 */
  NONE, NORMAL, (LONG)"\0",
  0x0000, 0x0002, 0x0100, 0x0001,
  BL_S5           ,       -1,       -1, G_STRING          ,   /* Object 5 BL_S4 */
  NONE, NORMAL, (LONG)"\0",
  0x0000, 0x0003, 0x0100, 0x0001,
  BL_S6           ,       -1,       -1, G_STRING          ,   /* Object 6 BL_S5 */
  NONE, NORMAL, (LONG)"\0",
  0x0000, 0x0004, 0x0100, 0x0001,
  BL_S7           ,       -1,       -1, G_STRING          ,   /* Object 7 BL_S6 */
  NONE, NORMAL, (LONG)"\0",
  0x0000, 0x0005, 0x0100, 0x0001,
  BL_S8           ,       -1,       -1, G_STRING          ,   /* Object 8 BL_S7 */
  NONE, NORMAL, (LONG)"\0",
  0x0000, 0x0006, 0x0100, 0x0001,
  BL_BKGR         ,       -1,       -1, G_STRING          ,   /* Object 9 BL_S8 */
  NONE, NORMAL, (LONG)"\0",
  0x0000, 0x0007, 0x0100, 0x0001,
         0, BL_UP           , BL_ASC          , G_IBOX            ,   /* Object 10 BL_BA */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0000, 0x0000, 0x0202, 0x0008,
  BL_DN           ,       -1,       -1, G_BOXCHAR         ,   /* Object 11 BL_UP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\001' << 24)|0x00FF1100L,
  0x0000, 0x0000, 0x0202, 0x0001,
  BL_ASC          ,       -1,       -1, G_BOXCHAR         ,   /* Object 12 BL_DN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\002' << 24)|0x00FF1100L,
  0x0000, 0x0007, 0x0202, 0x0001,
  BL_BA           , BL_SL           , BL_SL           , G_BOX             ,   /* Object 13 BL_ASC */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1041L,
  0x0000, 0x0001, 0x0202, 0x0006,
  BL_ASC          ,       -1,       -1, G_BOX             ,   /* Object 14 BL_SL */
  LASTOB|TOUCHEXIT, NORMAL, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x0202, 0x0002,
  
  /******** Tree 6 BIG_WIND ****************************************************/
        -1, BW_SIZERUL      , BW_WORK         , G_BOX             ,   /* Object 0 BW_BKGR */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x002A, 0x060E,
  BW_CLOSER       ,       -1,       -1, G_BOXCHAR         ,   /* Object 1 BW_SIZERUL */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\006' << 24)|0x00011180L,
  0x0000, 0x0000, 0x0302, 0x0301,
  BW_MOVER        ,       -1,       -1, G_BOXCHAR         ,   /* Object 2 BW_CLOSER */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\005' << 24)|0x00011180L,
  0x0202, 0x0000, 0x0302, 0x0301,
  BW_SMALLER      ,       -1,       -1, G_BOXTEXT         ,   /* Object 3 BW_MOVER */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[0],
  0x0404, 0x0000, 0x041C, 0x0301,
  BW_CYCLER       ,       -1,       -1, G_BOXCHAR         ,   /* Object 4 BW_SMALLER */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\177' << 24)|0x00011180L,
  0x0720, 0x0000, 0x0302, 0x0301,
  BW_FULLER       ,       -1,       -1, G_BOXCHAR         ,   /* Object 5 BW_CYCLER */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\257' << 24)|0x00011180L,
  0x0123, 0x0000, 0x0302, 0x0301,
  BW_SIZERUR      ,       -1,       -1, G_BOXCHAR         ,   /* Object 6 BW_FULLER */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\007' << 24)|0x00011180L,
  0x0325, 0x0000, 0x0302, 0x0301,
  BW_INFO         ,       -1,       -1, G_BOXCHAR         ,   /* Object 7 BW_SIZERUR */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\006' << 24)|0x00011180L,
  0x0527, 0x0000, 0x0302, 0x0301,
  BW_BARRE        ,       -1,       -1, G_BOXTEXT         ,   /* Object 8 BW_INFO */
  NONE, NORMAL, (LONG)&rs_tedinfo[1],
  0x0000, 0x0201, 0x002A, 0x0301,
  BW_VBAR         ,       -1,       -1, G_BOX             ,   /* Object 9 BW_BARRE */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0000, 0x0502, 0x002A, 0x0001,
  BW_SIZERDL      , BW_UPARROW      , BW_DNARROW      , G_BOX             ,   /* Object 10 BW_VBAR */
  NONE, NORMAL, (LONG)0x00011000L,
  0x0527, 0x0402, 0x0302, 0x000B,
  BW_VELEV        ,       -1,       -1, G_BOXCHAR         ,   /* Object 11 BW_UPARROW */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\001' << 24)|0x00011180L,
  0x0000, 0x0000, 0x0302, 0x0301,
  BW_DNARROW      , BW_VSLIDE       , BW_VSLIDE       , G_BOX             ,   /* Object 12 BW_VELEV */
  TOUCHEXIT, NORMAL, (LONG)0x00011191L,
  0x0000, 0x0201, 0x0302, 0x0C08,
  BW_VELEV        ,       -1,       -1, G_BOX             ,   /* Object 13 BW_VSLIDE */
  TOUCHEXIT, NORMAL, (LONG)0x00011100L,
  0x0000, 0x0000, 0x0302, 0x0002,
  BW_VBAR         ,       -1,       -1, G_BOXCHAR         ,   /* Object 14 BW_DNARROW */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\002' << 24)|0x00011180L,
  0x0000, 0x0D09, 0x0302, 0x0301,
  BW_HBAR         ,       -1,       -1, G_BOXCHAR         ,   /* Object 15 BW_SIZERDL */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\006' << 24)|0x00011180L,
  0x0000, 0x030D, 0x0302, 0x0301,
  BW_SIZERDR      , BW_LFARROW      , BW_RTARROW      , G_BOX             ,   /* Object 16 BW_HBAR */
  NONE, NORMAL, (LONG)0x00011000L,
  0x0202, 0x030D, 0x0425, 0x0301,
  BW_HELEV        ,       -1,       -1, G_BOXCHAR         ,   /* Object 17 BW_LFARROW */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00011180L,
  0x0000, 0x0000, 0x0302, 0x0301,
  BW_RTARROW      , BW_HSLIDE       , BW_HSLIDE       , G_BOX             ,   /* Object 18 BW_HELEV */
  TOUCHEXIT, NORMAL, (LONG)0x00011191L,
  0x0202, 0x0000, 0x0021, 0x0301,
  BW_HELEV        ,       -1,       -1, G_BOX             ,   /* Object 19 BW_HSLIDE */
  TOUCHEXIT, NORMAL, (LONG)0x00011100L,
  0x0000, 0x0000, 0x0005, 0x0301,
  BW_HBAR         ,       -1,       -1, G_BOXCHAR         ,   /* Object 20 BW_RTARROW */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00011180L,
  0x0123, 0x0000, 0x0302, 0x0301,
  BW_WORK         ,       -1,       -1, G_BOXCHAR         ,   /* Object 21 BW_SIZERDR */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\006' << 24)|0x00011180L,
  0x0527, 0x030D, 0x0302, 0x0301,
  BW_BKGR         ,       -1,       -1, G_IBOX            ,   /* Object 22 BW_WORK */
  LASTOB, NORMAL, (LONG)0x00011080L,
  0x0000, 0x0402, 0x0627, 0x000B,
  
  /******** Tree 7 GADGETS ****************************************************/
        -1, GA_SULH         , GA_SDRM         , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021041L,
  0x0000, 0x0000, 0x0012, 0x000D,
  GA_CLOH         ,       -1,       -1, G_ICON            ,   /* Object 1 GA_SULH */
  NONE, NORMAL, (LONG)&rs_iconblk[10],
  0x0001, 0x0001, 0x0002, 0x0001,
  GA_SMAH         ,       -1,       -1, G_ICON            ,   /* Object 2 GA_CLOH */
  NONE, NORMAL, (LONG)&rs_iconblk[11],
  0x0003, 0x0001, 0x0002, 0x0001,
  GA_CYCH         ,       -1,       -1, G_ICON            ,   /* Object 3 GA_SMAH */
  NONE, NORMAL, (LONG)&rs_iconblk[12],
  0x0009, 0x0001, 0x0002, 0x0001,
  GA_FULH         ,       -1,       -1, G_ICON            ,   /* Object 4 GA_CYCH */
  NONE, NORMAL, (LONG)&rs_iconblk[13],
  0x000B, 0x0001, 0x0002, 0x0001,
  GA_SURH         ,       -1,       -1, G_ICON            ,   /* Object 5 GA_FULH */
  NONE, NORMAL, (LONG)&rs_iconblk[14],
  0x000D, 0x0001, 0x0002, 0x0001,
  GA_AUPH         ,       -1,       -1, G_ICON            ,   /* Object 6 GA_SURH */
  NONE, NORMAL, (LONG)&rs_iconblk[15],
  0x000F, 0x0001, 0x0002, 0x0001,
  GA_SLVH         ,       -1,       -1, G_ICON            ,   /* Object 7 GA_AUPH */
  NONE, NORMAL, (LONG)&rs_iconblk[16],
  0x000F, 0x0002, 0x0002, 0x0001,
  GA_ADNH         ,       -1,       -1, G_ICON            ,   /* Object 8 GA_SLVH */
  NONE, NORMAL, (LONG)&rs_iconblk[17],
  0x000F, 0x0003, 0x0002, 0x0001,
  GA_SDLH         ,       -1,       -1, G_ICON            ,   /* Object 9 GA_ADNH */
  NONE, NORMAL, (LONG)&rs_iconblk[18],
  0x000F, 0x0004, 0x0002, 0x0001,
  GA_ALFH         ,       -1,       -1, G_ICON            ,   /* Object 10 GA_SDLH */
  NONE, NORMAL, (LONG)&rs_iconblk[19],
  0x0001, 0x0005, 0x0002, 0x0001,
  GA_SLHH         ,       -1,       -1, G_ICON            ,   /* Object 11 GA_ALFH */
  NONE, NORMAL, (LONG)&rs_iconblk[20],
  0x0003, 0x0005, 0x0002, 0x0001,
  GA_ARTH         ,       -1,       -1, G_ICON            ,   /* Object 12 GA_SLHH */
  NONE, NORMAL, (LONG)&rs_iconblk[21],
  0x0008, 0x0005, 0x0002, 0x0001,
  GA_SDRH         ,       -1,       -1, G_ICON            ,   /* Object 13 GA_ARTH */
  NONE, NORMAL, (LONG)&rs_iconblk[22],
  0x000D, 0x0005, 0x0002, 0x0001,
  GA_SULM         ,       -1,       -1, G_ICON            ,   /* Object 14 GA_SDRH */
  NONE, NORMAL, (LONG)&rs_iconblk[23],
  0x000F, 0x0005, 0x0002, 0x0001,
  GA_CLOM         ,       -1,       -1, G_ICON            ,   /* Object 15 GA_SULM */
  NONE, NORMAL, (LONG)&rs_iconblk[24],
  0x0001, 0x0008, 0x0002, 0x0800,
  GA_SMAM         ,       -1,       -1, G_ICON            ,   /* Object 16 GA_CLOM */
  NONE, NORMAL, (LONG)&rs_iconblk[25],
  0x0003, 0x0008, 0x0002, 0x0800,
  GA_CYCM         ,       -1,       -1, G_ICON            ,   /* Object 17 GA_SMAM */
  NONE, NORMAL, (LONG)&rs_iconblk[26],
  0x0009, 0x0008, 0x0002, 0x0800,
  GA_FULM         ,       -1,       -1, G_ICON            ,   /* Object 18 GA_CYCM */
  NONE, NORMAL, (LONG)&rs_iconblk[27],
  0x000B, 0x0008, 0x0002, 0x0800,
  GA_SURM         ,       -1,       -1, G_ICON            ,   /* Object 19 GA_FULM */
  NONE, NORMAL, (LONG)&rs_iconblk[28],
  0x000D, 0x0008, 0x0002, 0x0800,
  GA_AUPM         ,       -1,       -1, G_ICON            ,   /* Object 20 GA_SURM */
  NONE, NORMAL, (LONG)&rs_iconblk[29],
  0x000F, 0x0008, 0x0002, 0x0800,
  GA_SLVM         ,       -1,       -1, G_ICON            ,   /* Object 21 GA_AUPM */
  NONE, NORMAL, (LONG)&rs_iconblk[30],
  0x000F, 0x0009, 0x0002, 0x0800,
  GA_ADNM         ,       -1,       -1, G_ICON            ,   /* Object 22 GA_SLVM */
  NONE, NORMAL, (LONG)&rs_iconblk[31],
  0x000F, 0x000A, 0x0002, 0x0800,
  GA_SDLM         ,       -1,       -1, G_ICON            ,   /* Object 23 GA_ADNM */
  NONE, NORMAL, (LONG)&rs_iconblk[32],
  0x000F, 0x000B, 0x0002, 0x0800,
  GA_ALFM         ,       -1,       -1, G_ICON            ,   /* Object 24 GA_SDLM */
  NONE, NORMAL, (LONG)&rs_iconblk[33],
  0x0001, 0x000C, 0x0002, 0x0800,
  GA_SLHM         ,       -1,       -1, G_ICON            ,   /* Object 25 GA_ALFM */
  NONE, NORMAL, (LONG)&rs_iconblk[34],
  0x0003, 0x000C, 0x0002, 0x0800,
  GA_ARTM         ,       -1,       -1, G_ICON            ,   /* Object 26 GA_SLHM */
  NONE, NORMAL, (LONG)&rs_iconblk[35],
  0x0008, 0x000C, 0x0002, 0x0800,
  GA_SDRM         ,       -1,       -1, G_ICON            ,   /* Object 27 GA_ARTM */
  NONE, NORMAL, (LONG)&rs_iconblk[36],
  0x000D, 0x000C, 0x0002, 0x0800,
         0,       -1,       -1, G_ICON            ,   /* Object 28 GA_SDRM */
  LASTOB, NORMAL, (LONG)&rs_iconblk[37],
  0x000F, 0x000C, 0x0002, 0x0800,
  
  /******** Tree 8 SELF ****************************************************/
        -1, FS_MOVE         , FS_ANN          , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002D, 0x0014,
  FS_TITRE        ,       -1,       -1, G_IBOX            |0x1100,   /* Object 1 FS_MOVE */
  TOUCHEXIT, CROSSED|OUTLINED, (LONG)0x00FF1100L,
  0x002B, 0x0000, 0x0002, 0x0001,
  FS_PATH         ,       -1,       -1, G_STRING          ,   /* Object 2 FS_TITRE */
  NONE, NORMAL, (LONG)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNO",
  0x0001, 0x0800, 0x0029, 0x0001,
  FS_SAISIE       ,       -1,       -1, G_BOXTEXT         ,   /* Object 3 FS_PATH */
  NONE, NORMAL, (LONG)&rs_tedinfo[2],
  0x0001, 0x0801, 0x0029, 0x0001,
  FS_BOXL         ,       -1,       -1, G_FTEXT           ,   /* Object 4 FS_SAISIE */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[3],
  0x000A, 0x0802, 0x0018, 0x0001,
  FS_BOX          , FS_LA           , FS_LP           , G_IBOX            ,   /* Object 5 FS_BOXL */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0029, 0x0003, 0x0002, 0x0010,
         7,       -1,       -1, G_BOXCHAR         ,   /* Object 6 FS_LA */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG) ((LONG)'A' << 24)|0x00001100L,
  0x0000, 0x0000, 0x0002, 0x0001,
         8,       -1,       -1, G_BOXCHAR         ,   /* Object 7  */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG) ((LONG)'B' << 24)|0x00001100L,
  0x0000, 0x0001, 0x0002, 0x0001,
         9,       -1,       -1, G_BOXCHAR         ,   /* Object 8  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'C' << 24)|0x00001100L,
  0x0000, 0x0002, 0x0002, 0x0001,
        10,       -1,       -1, G_BOXCHAR         ,   /* Object 9  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'D' << 24)|0x00001100L,
  0x0000, 0x0003, 0x0002, 0x0001,
        11,       -1,       -1, G_BOXCHAR         ,   /* Object 10  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'E' << 24)|0x00001100L,
  0x0000, 0x0004, 0x0002, 0x0001,
        12,       -1,       -1, G_BOXCHAR         ,   /* Object 11  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'F' << 24)|0x00001100L,
  0x0000, 0x0005, 0x0002, 0x0001,
        13,       -1,       -1, G_BOXCHAR         ,   /* Object 12  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'G' << 24)|0x00001100L,
  0x0000, 0x0006, 0x0002, 0x0001,
        14,       -1,       -1, G_BOXCHAR         ,   /* Object 13  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'H' << 24)|0x00001100L,
  0x0000, 0x0007, 0x0002, 0x0001,
        15,       -1,       -1, G_BOXCHAR         ,   /* Object 14  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'I' << 24)|0x00001100L,
  0x0000, 0x0008, 0x0002, 0x0001,
        16,       -1,       -1, G_BOXCHAR         ,   /* Object 15  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'J' << 24)|0x00001100L,
  0x0000, 0x0009, 0x0002, 0x0001,
        17,       -1,       -1, G_BOXCHAR         ,   /* Object 16  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'K' << 24)|0x00001100L,
  0x0000, 0x000A, 0x0002, 0x0001,
        18,       -1,       -1, G_BOXCHAR         ,   /* Object 17  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'L' << 24)|0x00001100L,
  0x0000, 0x000B, 0x0002, 0x0001,
        19,       -1,       -1, G_BOXCHAR         ,   /* Object 18  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'M' << 24)|0x00001100L,
  0x0000, 0x000C, 0x0002, 0x0001,
        20,       -1,       -1, G_BOXCHAR         ,   /* Object 19  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'N' << 24)|0x00001100L,
  0x0000, 0x000D, 0x0002, 0x0001,
  FS_LP           ,       -1,       -1, G_BOXCHAR         ,   /* Object 20  */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'O' << 24)|0x00001100L,
  0x0000, 0x000E, 0x0002, 0x0001,
  FS_BOXL         ,       -1,       -1, G_BOXCHAR         ,   /* Object 21 FS_LP */
  SELECTABLE|EXIT|RBUTTON, DISABLED, (LONG) ((LONG)'P' << 24)|0x00001100L,
  0x0000, 0x000F, 0x0002, 0x0001,
  FS_OK           , FS_RETOUR       , FS_BF           , G_BOX             ,   /* Object 22 FS_BOX */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0001, 0x0C03, 0x0026, 0x080D,
  FS_BOXAR        ,       -1,       -1, G_BOXCHAR         ,   /* Object 23 FS_RETOUR */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\005' << 24)|0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0001,
  FS_BOXF         , FS_ARUP         , FS_ASC          , G_IBOX            ,   /* Object 24 FS_BOXAR */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0024, 0x0001, 0x0002, 0x080C,
  FS_ARDN         ,       -1,       -1, G_BOXCHAR         ,   /* Object 25 FS_ARUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\001' << 24)|0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0001,
  FS_ASC          ,       -1,       -1, G_BOXCHAR         ,   /* Object 26 FS_ARDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\002' << 24)|0x00FF1100L,
  0x0000, 0x080B, 0x0002, 0x0001,
  FS_BOXAR        , FS_SLD          , FS_SLD          , G_BOX             ,   /* Object 27 FS_ASC */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1013L,
  0x0000, 0x0001, 0x0002, 0x080A,
  FS_ASC          ,       -1,       -1, G_BOX             ,   /* Object 28 FS_SLD */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1000L,
  0x0000, 0x0000, 0x0002, 0x0802,
  FS_BF           ,       -1,       -1, G_IBOX            ,   /* Object 29 FS_BOXF */
  NONE, NORMAL, (LONG)0x00011000L,
  0x0001, 0x0001, 0x0022, 0x080C,
  FS_BOX          , FS_FILTRE       , FS_FILTRE       , G_BOX             ,   /* Object 30 FS_BF */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1013L,
  0x0002, 0x0000, 0x0024, 0x0001,
  FS_BF           ,       -1,       -1, G_TEXT            ,   /* Object 31 FS_FILTRE */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[4],
  0x000C, 0x0000, 0x000C, 0x0001,
  FS_ANN          ,       -1,       -1, G_BUTTON          |0x1E00,   /* Object 32 FS_OK */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)"Confirme",
  0x0003, 0x0012, 0x0009, 0x0001,
         0,       -1,       -1, G_BUTTON          |0x1F00,   /* Object 33 FS_ANN */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)"Annule",
  0x001C, 0x0012, 0x0009, 0x0001,
  
  /******** Tree 9 AIDE ****************************************************/
        -1,        1,       11, G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0022, 0x080C,
         2,       -1,       -1, G_TEXT            ,   /* Object 1  */
  NONE, SELECTED, (LONG)&rs_tedinfo[5],
  0x0000, 0x0000, 0x0022, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"\001 / \002 : S\202lection haut / bas",
  0x0001, 0x0801, 0x001C, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Ctrl + \001 / \002 : Ligne haut / bas",
  0x0001, 0x0802, 0x001F, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"Shift + \001 / \002 : Page haut / bas",
  0x0001, 0x0803, 0x001F, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"Clr : D\202but liste",
  0x0001, 0x0804, 0x0011, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"Shift + Clr : Fin liste",
  0x0001, 0x0805, 0x0017, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"Tab : Bascule filtre et \042*.*\042",
  0x0001, 0x0806, 0x001D, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"Insert : Remonter d\047un niveau",
  0x0001, 0x0807, 0x001D, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"Shift Insert : Retour racine",
  0x0001, 0x0808, 0x001C, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"Alt + A \205 P : Lecteur",
  0x0001, 0x0809, 0x0015, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 11  */
  LASTOB, NORMAL, (LONG)"Saisie : S\202lection",
  0x0001, 0x080A, 0x0012, 0x0001
};

OBJECT *rs_trindex[] =
{ &rs_object[0],   /* Tree  0 BIGALERTE        */
  &rs_object[11],   /* Tree  1 BIGICN           */
  &rs_object[21],   /* Tree  2 BIGARRM          */
  &rs_object[24],   /* Tree  3 IMG_USER         */
  &rs_object[35],   /* Tree  4 BIGARRS          */
  &rs_object[38],   /* Tree  5 BIGLIST          */
  &rs_object[53],   /* Tree  6 BIG_WIND         */
  &rs_object[76],   /* Tree  7 GADGETS          */
  &rs_object[105],   /* Tree  8 SELF             */
  &rs_object[139]    /* Tree  9 AIDE             */
};
