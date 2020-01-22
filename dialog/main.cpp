#include "main.h"
#include "kbd.h"

void __start__(void) {
  ExitProcess(WinMain(GetModuleHandle(NULL), 0, NULL, 0));
}

void modifyMenu(HMENU menu, WORD lang) {
  setMenuText(menu, C_CMD_EXIT, lang);
  setMenuText(menu, C_CMD_DISPOFF, lang);
  setMenuText(menu, C_CMD_AWAKEN, lang);
}

BOOL CALLBACK winproc1(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
  case WM_INITDIALOG: {
    HWND probar = GetDlgItem(hwnd, ID_PB_MAIN);
    SendMessage(probar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    SendMessage(probar, PBM_SETSTEP, 10, 0);
    SendMessage(probar, PBM_STEPIT, 0, 0);
    return 1;
  }
  case WM_COMMAND: {
    switch (LOWORD(wp)) {
    case IDOK: MessageBox(hwnd,L"YES",0,0); EndDialog(hwnd, IDOK); break;
    case IDCANCEL: EndDialog(hwnd, IDCANCEL); break;
    }
    return 1;
  }
  case WM_CLOSE: {
    EndDialog(hwnd, IDCANCEL);
    return 1;
  }
  default:;
  }
  return 0;
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
static WORD langtype = C_LANG_DEFAULT;
static BOOL mmmm;
static HMENU menu;
static HMENU pop1;
  switch (msg) {
  case WM_CREATE: {
    // MENU & LANG & HOTKEY
    Hotkey.assign(C_CMD_EXIT, 'Q', C_KBD_CTRL);
    Hotkey.assign(C_CMD_DISPOFF, 'B', C_KBD_CTRL);
    switch (LANGIDFROMLCID(GetUserDefaultLCID())) {
    case 0x0411: langtype = C_LANG_JA; break;
    default: langtype = C_LANG_DEFAULT; break;
    }
    modifyMenu(GetMenu(hwnd), langtype);
    DrawMenuBar(hwnd);
    return 0;
  }
  case WM_SHOWWINDOW: {
    if (!mmmm) {
      HWND ctx = GetDlgItem(hwnd, ID_TXT_TIMER);
      HFONT font = CreateFont(75, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, FIXED_PITCH, L"MS Shell Dlg");
      SendMessage(ctx, WM_SETFONT, (WPARAM)font, TRUE);
      SetWindowText(ctx, L"1:30:58");
      ctx = GetDlgItem(hwnd, ID_TXT_MISSION);
      font = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, FIXED_PITCH, L"MS Shell Dlg");
      SendMessage(ctx, WM_SETFONT, (WPARAM)font, TRUE);
      SetWindowText(ctx, L"スリープ状態に移行します");
      HWND probar = GetDlgItem(hwnd, ID_PB_MAIN);
      SendMessage(probar, PBM_SETRANGE32, 0, 60000);
      SendMessage(probar, PBM_SETSTEP, -1000, 0);
      SendMessage(probar, PBM_SETPOS, SendMessage(probar, PBM_GETRANGE, FALSE, NULL), 0);
      mmmm = TRUE;
    }
    return 0;
  }
  case WM_COMMAND: {
    switch (LOWORD(wp)) {
    case IDOK: break;
    case IDCANCEL: break;
    case C_CMD_EXIT: DestroyWindow(hwnd); break;
    case C_CMD_DISPOFF: MessageBox(hwnd, L"ブラックアウト！", 0, 0); break;
    }
    HWND probar = GetDlgItem(hwnd, ID_PB_MAIN);
    SendMessage(probar, PBM_STEPIT, 0, 0);
    return 0;
  }
  case WM_LBUTTONUP: {
    return 0;
  }
  case WM_PAINT: {
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
  // Main Window: Settings
  WNDCLASSEX wc;
  SecureZeroMemory(&wc, sizeof(WNDCLASSEX));
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.cbWndExtra = DLGWINDOWEXTRA;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = mainWndProc;
  wc.hInstance = hi;
  wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
  wc.lpszClassName = C_STR_CLASS_MAIN;
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
  // finish
  return msg.wParam;
}
