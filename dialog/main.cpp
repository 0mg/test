#include "debug.h"
#include "main.h"
#include "kbd.h"

#define AWAKE_SYS 1
#define AWAKE_DISP 2
#define TASK_SLEEP 0
#define TASK_HIBER 1
#define TASK_DISPOFF 2
#define MS 1000
#define ATIMEOUT_DEFAULT 60
#define WTIMER_ID 1
#define WTIMER_OUT 200

static WORD langtype = C_LANG_DEFAULT;
static BOOL timeset = FALSE;
static ULONGLONG stime = 0;
static BOOL counting = FALSE;
static int task = TASK_SLEEP;
static int awaken = 0;
static BOOL debugMode = FALSE;
static BOOL atimeover = FALSE;

static struct {
  int out;
  int rest;
  int fixed;
  TCHAR text[99];
} atimer;

void __start__(void) {
  ExitProcess(WinMain(GetModuleHandle(NULL), 0, NULL, 0));
}

int parseTimeString(LPTSTR ts) {
  int a = 0, b = 0, c = 0, *p = NULL, time = 0;
  while (*ts) { // alt for swscanf(ts,"%d:%d:%d",&a,&b,&c) in msvcrt.dll
    if (*ts >= '0' && *ts <= '9') {
      if (p == NULL) p = &a;
      *p = (*p * 10) + (*ts - '0');
    } else if (*ts == ':') { // hh:mm:ss OR mm:ss OR ss
      p = p == &a ? &b : &c;
    }
    ts++;
  }
  // every `time` can be less than zero
  if (p == &c) { // hh:mm:ss
    time = (a * 3600 + b * 60 + c) * MS;
  } else if (p == &b) { // mm:ss
    time = (a * 60 + b) * MS;
  } else if (p == &a) { // ss
    time = a * MS;
  } else { // p == NULL (e.g. run `sustimer.exe abc`)
    time = -1;
  }
  return time;
}

int getATimeout() {
  int argc;
  LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc), ts = NULL;
  while (*++argv) if (**argv != '/') ts = *argv;
  int time = -1;
  if (ts) time = parseTimeString(ts);
  return time >= 0 ? time : ATIMEOUT_DEFAULT * MS;
}

void registerHotkeys() {
  Hotkey.assign(C_CMD_EXIT, VK_ESCAPE, 0);
  Hotkey.assign(C_CMD_DISPOFF, 'B', C_KBD_CTRL);
  Hotkey.assign(C_CMD_NEW, 'N', C_KBD_CTRL);
  Hotkey.assign(C_CMD_SAVEAS, 'E', C_KBD_CTRL);
  Hotkey.assign(C_CMD_AWAKEN, 'A', C_KBD_CTRL);
}

void modifyMenu(HMENU menu, WORD lang) {
  setMenuText(menu, C_STR_FILE, lang, 1);
  setMenuText(menu, C_STR_CTRL, lang, 2);
  setMenuText(menu, C_STR_TOOL, lang, 3);
  setMenuText(menu, C_CMD_EXIT, lang);
  setMenuText(menu, C_CMD_STOP, lang);
  setMenuText(menu, C_CMD_DISPOFF, lang);
  setMenuText(menu, C_CMD_AWAKEN, lang);
  setMenuText(menu, C_CMD_NEW, lang);
  setMenuText(menu, C_CMD_SAVEAS, lang);
  setMenuText(menu, C_CMD_ABOUT, lang);
  setMenuText(menu, C_CMD_MANUAL, lang);
}

BOOL CALLBACK optProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  static HWND times, tasks, awdisp, awsys;
  switch (msg) {
  case WM_INITDIALOG: {
    TCHAR s[C_MAX_MSGTEXT];
    times = GetDlgItem(hwnd, ID_EDIT_TIME);
    SendMessage(times, CB_ADDSTRING, 0, (LPARAM)L"3:00");
    SendMessage(times, CB_ADDSTRING, 0, (LPARAM)L"5:00");
    SendMessage(times, CB_ADDSTRING, 0, (LPARAM)L"2:00:00");
    SendMessage(times, CB_ADDSTRING, 0, (LPARAM)L"2:30:00");
    SendMessage(times, CB_ADDSTRING, 0, (LPARAM)L"3:00:00");
    SetDlgItemText(hwnd, ID_EDIT_TIME, atimer.text[0] ? atimer.text : L"5");
    tasks = GetDlgItem(hwnd, ID_EDIT_TASK);
    SendMessage(tasks, CB_ADDSTRING, 0, (LPARAM)getLocaleString(s, C_STR_SLEEP, langtype));
    SendMessage(tasks, CB_ADDSTRING, 0, (LPARAM)getLocaleString(s, C_STR_HIBER, langtype));
    SendMessage(tasks, CB_ADDSTRING, 0, (LPARAM)getLocaleString(s, C_STR_DISPOFF, langtype));
    SendMessage(tasks, CB_SETCURSEL, 0, 0);
    awdisp = GetDlgItem(hwnd, ID_EDIT_AWDISP);
    SendMessage(awdisp, CB_ADDSTRING, 0, (LPARAM)getLocaleString(s, C_STR_AUTO, langtype));
    SendMessage(awdisp, CB_ADDSTRING, 0, (LPARAM)getLocaleString(s, C_STR_SUPRESS, langtype));
    SendMessage(awdisp, CB_SETCURSEL, 0, 0);
    awsys = GetDlgItem(hwnd, ID_EDIT_AWSYS);
    SendMessage(awsys, CB_ADDSTRING, 0, (LPARAM)getLocaleString(s, C_STR_AUTO, langtype));
    SendMessage(awsys, CB_ADDSTRING, 0, (LPARAM)getLocaleString(s, C_STR_SUPRESS, langtype));
    SendMessage(awsys, CB_SETCURSEL, 1, 0);
    return 1;
  }
  case WM_COMMAND: {
    switch (LOWORD(wp)) {
    case IDOK: {
      TCHAR timestr[20];
      GetDlgItemText(hwnd, ID_EDIT_TIME, timestr, 20);
      int time = parseTimeString(timestr);
      atimer.out = time >= 0 ? time : ATIMEOUT_DEFAULT * MS;
      atimer.rest = atimer.out;
      timeset = TRUE;
      switch (SendMessage(tasks, CB_GETCURSEL, 0, 0)) {
        case 0: task = TASK_SLEEP; break;
        case 1: task = TASK_HIBER; break;
        case 2: task = TASK_DISPOFF; break;
        default: task = TASK_SLEEP; break;
      }
      awaken |= AWAKE_DISP * (SendMessage(awdisp, CB_GETCURSEL, 0, 0) == 1);
      awaken |= AWAKE_SYS * (SendMessage(awsys, CB_GETCURSEL, 0, 0) == 1);
      EndDialog(hwnd, IDOK);
      break;
    }
    case IDCANCEL: EndDialog(hwnd, IDCANCEL); break;
    }
    return 1;
  }
  case WM_DESTROY: {
    EndDialog(hwnd, IDCANCEL);
    return 1;
  }
  default:;
  }
  return 0;
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  static HWND probar;
  // # count down
  if (counting) {
    atimer.rest = atimer.out - (GetTickCount64() - stime);
    // # on time over
    if (atimer.rest <= 0) {
      if (debugMode) {
        TCHAR s[99];
        wsprintf(s, TEXT("sustimer rest: %d;\n"), atimer.rest);
        OutputDebugString(s);
      }
      if (atimer.rest < -3 * MS) {
        // if timeout after [suspend PC by other app or sys] or freeze
        atimer.out = 60 * MS;
        counting = FALSE;
        SendMessage(hwnd, WM_COMMAND, C_CMD_START, 0);
      } else {
        atimeover = TRUE;
        atimer.rest = 0;
        counting = FALSE;
        KillTimer(hwnd, WTIMER_ID);
        PostMessage(hwnd, WM_CLOSE, 0, 0);
      }
    }
  }
  // # message handler
  switch (msg) {
  case WM_CREATE: {
    // MENU & LANG & HOTKEY
    registerHotkeys();
    switch (LANGIDFROMLCID(GetUserDefaultLCID())) {
    case 0x0411: langtype = C_LANG_JA; break;
    default: langtype = C_LANG_DEFAULT; break;
    }
    modifyMenu(GetMenu(hwnd), langtype);
    DrawMenuBar(hwnd);
    return 0;
  }
  case WM_SHOWWINDOW: {
    // # Init dialog's layout
    HWND ctx = GetDlgItem(hwnd, ID_TXT_TIMER);
    HFONT font = CreateFont(75, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, FIXED_PITCH, L"MS Shell Dlg");
    SendMessage(ctx, WM_SETFONT, (WPARAM)font, TRUE);
    // mission
    ctx = GetDlgItem(hwnd, ID_TXT_MISSION);
    font = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, FIXED_PITCH, L"MS Shell Dlg");
    SendMessage(ctx, WM_SETFONT, (WPARAM)font, TRUE);
    // icons
    font = CreateFont(29, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, FIXED_PITCH, L"MS Shell Dlg");
    ctx = GetDlgItem(hwnd, ID_ICO_MISSION);
    SendMessage(ctx, WM_SETFONT, (WPARAM)font, TRUE);
    ctx = GetDlgItem(hwnd, ID_ICO_AWAKEN);
    SendMessage(ctx, WM_SETFONT, (WPARAM)font, TRUE);
    ctx = GetDlgItem(hwnd, ID_ICO_DEBUG);
    SendMessage(ctx, WM_SETFONT, (WPARAM)font, TRUE);
    ctx = GetDlgItem(hwnd, ID_ICO_EXTRA);
    SendMessage(ctx, WM_SETFONT, (WPARAM)font, TRUE);
    // # Init vars
    probar = GetDlgItem(hwnd, ID_PB_MAIN);
    return 0;
  }
  case WM_COMMAND: {
    switch (LOWORD(wp)) {
    case C_CMD_NEW: {
      counting = FALSE;
      timeset = FALSE;
      if (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DLG_OPTION), hwnd, optProc) == IDOK) {
        PostMessage(hwnd, WM_COMMAND, C_CMD_START, 0);
      }
      break;
    }
    case C_CMD_START: {
      stime = GetTickCount64();
      KillTimer(hwnd, WTIMER_ID);
      SetTimer(hwnd, WTIMER_ID, WTIMER_OUT, NULL);
      SendMessage(probar, PBM_SETRANGE32, 0, atimer.out);
      SendMessage(hwnd, WM_TIMER, 0, 0);
      TCHAR s[C_MAX_MSGTEXT];
      SetDlgItemText(hwnd, ID_TXT_MISSION,
        getLocaleString(s,
          task == TASK_SLEEP ? C_STR_SLEEP :
          task == TASK_HIBER ? C_STR_HIBER :
          task == TASK_DISPOFF ? C_STR_DISPOFF : C_STR_UNDEFINED, langtype));
      SetFocus((HWND)GetDlgItem(hwnd, C_CMD_STOP));
      counting = TRUE;
      break;
    }
    case C_CMD_STOP: {
      KillTimer(hwnd, WTIMER_ID);
      counting = FALSE;
      break;
    }
    case C_CMD_AWAKEN: MessageBox(hwnd, L"目覚め", 0, 0); break;
    case C_CMD_EXIT: MessageBox(hwnd,L"終了?",0,1)==1&&DestroyWindow(hwnd); break;
    case C_CMD_DISPOFF: {
      SendNotifyMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
      break;
    }
    }
    return 0;
  }
  case WM_TIMER: {
    // # format time string
    atimer.fixed = atimer.rest / MS + counting;
    if (atimer.fixed >= 3600) {
      wsprintf(atimer.text, TEXT("%d:%02d:%02d"), atimer.fixed / 3600, (atimer.fixed % 3600) / 60, atimer.fixed % 60);
    } else if (atimer.fixed >= 60) {
      wsprintf(atimer.text, TEXT("%d:%02d"), (atimer.fixed % 3600) / 60, atimer.fixed % 60);
    } else {
      wsprintf(atimer.text, TEXT("%d"), atimer.fixed);
    }
    // # draw timer
    SendMessage(probar, PBM_SETPOS, atimer.rest, 0);
    SetDlgItemText(hwnd, ID_TXT_TIMER, atimer.text);
    break;
  }
  case WM_KEYDOWN: {
    if (lp & 0x40000000) return 0; // ignore repeated press
    BYTE alt = C_KBD_ALT * !!(GetKeyState(VK_MENU) & 0x8000);
    BYTE shift = C_KBD_SHIFT * !!(GetKeyState(VK_SHIFT) & 0x8000);
    BYTE ctrl = C_KBD_CTRL * !!(GetKeyState(VK_CONTROL) & 0x8000);
    char key = wp & 0xFF;
    WORD id = Hotkey.getCmdIdByKeyCombo(key, alt | shift | ctrl);
    if (id) {
      PostMessage(hwnd, WM_COMMAND, id, 0);
    }
    TCHAR s[99];
    strifyKeyCombo(s, key, alt | shift | ctrl);
    SetWindowText(hwnd, s);
    return 0;
  }
  case WM_DESTROY: {
    PostQuitMessage(0);
    return 0;
  }
  }
  return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE hp, LPSTR cl, int cs) {
  SecureZeroMemory(&atimer, sizeof(atimer));
  // Main Window: Settings
  WNDCLASSEX wc;
  SecureZeroMemory(&wc, sizeof(WNDCLASSEX));
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.cbWndExtra = DLGWINDOWEXTRA;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = mainWndProc;
  wc.hInstance = hi;
  wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
  wc.lpszClassName = C_STR_APPNAME;
  wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
  wc.hbrBackground = (HBRUSH)(COLOR_MENU + 1);
  wc.lpszMenuName = MAKEINTRESOURCE(ID_MNU_MAIN);
  RegisterClassEx(&wc);

  // Main Window: Create, Show
  HWND hwnd = CreateDialog(hi, MAKEINTRESOURCE(ID_DLG_MAIN), 0, NULL);
  // WinMain() must return 0 before msg loop
  if (hwnd == NULL) { popError(); return 0; }

  // main
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    if (IsDialogMessage(hwnd, &msg)) {
      ///* Bug: It be sent WM_KEYDOWN twice if GetFocus()==hwnd, why??
      if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) {
        if (msg.message == WM_SYSCHAR || (msg.message == WM_KEYDOWN && GetFocus() != hwnd)) {
          SendNotifyMessage(hwnd, msg.message, msg.wParam, msg.lParam);
        }
      }
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  if (debugMode) {
    TCHAR s[99];
    wsprintf(s, TEXT("exit code: %d\nsuspend: %d\ntask: %d"), msg.wParam, atimeover, task);
    MessageBox(NULL, s, 0, 0);
    return msg.wParam;
  }
  if (atimeover) {
    switch (task) {
      case TASK_SLEEP: MessageBox(0,L"スリープしました",0,0); break;
      case TASK_HIBER: MessageBox(0,L"休止状態になった",0,0); break;
      case TASK_DISPOFF: {
        SendNotifyMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
        break;
      }
      default: MessageBox(0,L"実行内容がありません",0,0); break;
    }
  }
  // finish
  return msg.wParam;
}
