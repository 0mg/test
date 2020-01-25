#include <windows.h>
#include <commctrl.h>
#include <powrprof.h>

// controls ID
#define ID_DLG_MAIN     101
#define ID_DLG_OPTION   102
#define ID_PB_MAIN      103
#define ID_ICO_MISSION  104
#define ID_ICO_AWAKEN   105
#define ID_ICO_DEBUG    106
#define ID_ICO_EXTRA    107
#define ID_TXT_TIMER    108
#define ID_TXT_MISSION  109
#define ID_TXT_EXTRA    110
#define ID_MNU_MAIN     111
#define ID_EDIT_TIME    112
#define ID_EDIT_TASK    113
#define ID_EDIT_AWDISP  114
#define ID_EDIT_AWSYS   115

// command ID, string ID. using with lang mask
#define C_CMD_STOP      IDOK      // 0x1, Enter
#define C_CMD_EXIT      IDCANCEL  // 0x2, Escape
#define C_CMD_DISPOFF   0x102
#define C_CMD_AWAKEN    0x103
#define C_CMD_NEW       0x104
#define C_CMD_SAVEAS    0x105
#define C_CMD_ABOUT     0x106
#define C_CMD_MANUAL    0x107
/**/
#define C_CMD_LANG_DEFAULT 0x6F0
#define C_CMD_LANG_JA   0x6F1
#define C_STR_FILE      0x701
#define C_STR_TOOL      0x702
#define C_STR_HELP      0x703
#define C_STR_CTRL      0x704

// lang mask
#define C_LANG_DEFAULT  0x0000
#define C_LANG_JA       0x1000

// const value
#define C_STR_APPNAME    TEXT("ダイアログタイマ")
