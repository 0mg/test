#include <windows.h>
#include <commctrl.h>
#include <powrprof.h>

// controls ID
#define ID_DLG_MAIN     0xF01
#define ID_DLG_OPTION   0xF02
#define ID_PB_MAIN      0xF03
#define ID_ICO_MISSION  0xF04
#define ID_ICO_AWAKEN   0xF05
#define ID_ICO_DEBUG    0xF06
#define ID_ICO_EXTRA    0xF07
#define ID_TXT_TIMER    0xF08
#define ID_TXT_MISSION  0xF09
#define ID_TXT_EXTRA    0xF0A
#define ID_MNU_MAIN     0xF0B
#define ID_EDIT_TIME    0xF0C
#define ID_EDIT_TASK    0xF0D
#define ID_EDIT_AWDISP  0xF0E
#define ID_EDIT_AWSYS   0xF0F

// command ID, string ID. using with lang mask
#define C_CMD_STOP      IDOK      // 0x1, Enter
#define C_CMD_EXIT      IDCANCEL  // 0x2, Escape
#define C_CMD_DISPOFF   0x102
#define C_CMD_AWAKEN    0x103
#define C_CMD_NEW       0x104
#define C_CMD_SAVEAS    0x105
#define C_CMD_ABOUT     0x106
#define C_CMD_MANUAL    0x107
#define C_CMD_START     0x108
/**/
#define C_CMD_LANG_DEFAULT 0x6F0
#define C_CMD_LANG_JA   0x6F1
#define C_STR_FILE      0x701
#define C_STR_TOOL      0x702
#define C_STR_HELP      0x703
#define C_STR_CTRL      0x704
#define C_STR_SLEEP     0x705
#define C_STR_HIBER     0x706
#define C_STR_DISPOFF   0x707
#define C_STR_AUTO      0x708
#define C_STR_SUPRESS   0x709
#define C_STR_UNDEFINED 0x70A

// lang mask
#define C_LANG_DEFAULT  0x0000
#define C_LANG_JA       0x1000

// const value
#define C_STR_APPNAME    TEXT("ダイアログタイマ")
