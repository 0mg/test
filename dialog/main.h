#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#define ID_DLG_MAIN     12
#define ID_DLG_OPTION   23
#define ID_PB_MAIN      33
#define ID_TXT_TIMER    72
#define ID_TXT_MISSION  70
#define ID_MNU_MAIN     8

#define C_CMD_EXIT      0x00E
#define C_CMD_DISPOFF   0x00B
#define C_CMD_AWAKEN    0x00A

#define C_STR_CLASS_MAIN TEXT("main")

// lang mask
#define C_LANG_DEFAULT 0x0000
#define C_LANG_JA 0x1000

// NONE DEPENDENCY
int popError(HWND hwnd = NULL, UINT mbstyle = MB_OK) {
  const SIZE_T len = 0x400;
  TCHAR str[len];
  DWORD code = GetLastError();
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, 0, str, len, NULL);
  LPTSTR title;
  if (code != NO_ERROR) {
    title = NULL;
    mbstyle |= MB_ICONERROR;
  } else {
    title = TEXT("OK");
    mbstyle |= MB_ICONINFORMATION;
  }
  return MessageBox(hwnd, str, title, mbstyle);
}

namespace MsgStr {
  LPTSTR get(UINT msg) {
    switch(msg) {
    case 0x0000: return TEXT("WM_NULL");
    case 0x0001: return TEXT("WM_CREATE");
    case 0x0002: return TEXT("WM_DESTROY");
    case 0x0003: return TEXT("WM_MOVE");
    case 0x0005: return TEXT("WM_SIZE");
    case 0x0006: return TEXT("WM_ACTIVATE");
    case 0x0007: return TEXT("WM_SETFOCUS");
    case 0x0008: return TEXT("WM_KILLFOCUS");
    case 0x000A: return TEXT("WM_ENABLE");
    case 0x000B: return TEXT("WM_SETREDRAW");
    case 0x000C: return TEXT("WM_SETTEXT");
    case 0x000D: return TEXT("WM_GETTEXT");
    case 0x000E: return TEXT("WM_GETTEXTLENGTH");
    case 0x000F: return TEXT("WM_PAINT");
    case 0x0010: return TEXT("WM_CLOSE");
    case 0x0011: return TEXT("WM_QUERYENDSESSION");
    case 0x0012: return TEXT("WM_QUIT");
    case 0x0013: return TEXT("WM_QUERYOPEN");
    case 0x0014: return TEXT("WM_ERASEBKGND");
    case 0x0015: return TEXT("WM_SYSCOLORCHANGE");
    case 0x0016: return TEXT("WM_ENDSESSION");
    case 0x0018: return TEXT("WM_SHOWWINDOW");
    case 0x001A: return TEXT("WM_WININICHANGE");
    case 0x001B: return TEXT("WM_DEVMODECHANGE");
    case 0x001C: return TEXT("WM_ACTIVATEAPP");
    case 0x001D: return TEXT("WM_FONTCHANGE");
    case 0x001E: return TEXT("WM_TIMECHANGE");
    case 0x001F: return TEXT("WM_CANCELMODE");
    case 0x0020: return TEXT("WM_SETCURSOR");
    case 0x0021: return TEXT("WM_MOUSEACTIVATE");
    case 0x0022: return TEXT("WM_CHILDACTIVATE");
    case 0x0023: return TEXT("WM_QUEUESYNC");
    case 0x0024: return TEXT("WM_GETMINMAXINFO");
    case 0x0026: return TEXT("WM_PAINTICON");
    case 0x0027: return TEXT("WM_ICONERASEBKGND");
    case 0x0028: return TEXT("WM_NEXTDLGCTL");
    case 0x002A: return TEXT("WM_SPOOLERSTATUS");
    case 0x002B: return TEXT("WM_DRAWITEM");
    case 0x002C: return TEXT("WM_MEASUREITEM");
    case 0x002D: return TEXT("WM_DELETEITEM");
    case 0x002E: return TEXT("WM_VKEYTOITEM");
    case 0x002F: return TEXT("WM_CHARTOITEM");
    case 0x0030: return TEXT("WM_SETFONT");
    case 0x0031: return TEXT("WM_GETFONT");
    case 0x0032: return TEXT("WM_SETHOTKEY");
    case 0x0033: return TEXT("WM_GETHOTKEY");
    case 0x0037: return TEXT("WM_QUERYDRAGICON");
    case 0x0039: return TEXT("WM_COMPAREITEM");
    case 0x003D: return TEXT("WM_GETOBJECT");
    case 0x0041: return TEXT("WM_COMPACTING");
    case 0x0044: return TEXT("WM_COMMNOTIFY");
    case 0x0046: return TEXT("WM_WINDOWPOSCHANGING");
    case 0x0047: return TEXT("WM_WINDOWPOSCHANGED");
    case 0x0048: return TEXT("WM_POWER");
    case 0x004A: return TEXT("WM_COPYDATA");
    case 0x004B: return TEXT("WM_CANCELJOURNAL");
    case 0x004E: return TEXT("WM_NOTIFY");
    case 0x0050: return TEXT("WM_INPUTLANGCHANGEREQUEST");
    case 0x0051: return TEXT("WM_INPUTLANGCHANGE");
    case 0x0052: return TEXT("WM_TCARD");
    case 0x0053: return TEXT("WM_HELP");
    case 0x0054: return TEXT("WM_USERCHANGED");
    case 0x0055: return TEXT("WM_NOTIFYFORMAT");
    case 0x007B: return TEXT("WM_CONTEXTMENU");
    case 0x007C: return TEXT("WM_STYLECHANGING");
    case 0x007D: return TEXT("WM_STYLECHANGED");
    case 0x007E: return TEXT("WM_DISPLAYCHANGE");
    case 0x007F: return TEXT("WM_GETICON");
    case 0x0080: return TEXT("WM_SETICON");
    case 0x0081: return TEXT("WM_NCCREATE");
    case 0x0082: return TEXT("WM_NCDESTROY");
    case 0x0083: return TEXT("WM_NCCALCSIZE");
    case 0x0084: return TEXT("WM_NCHITTEST");
    case 0x0085: return TEXT("WM_NCPAINT");
    case 0x0086: return TEXT("WM_NCACTIVATE");
    case 0x0087: return TEXT("WM_GETDLGCODE");
    case 0x0088: return TEXT("WM_SYNCPAINT");
    case 0x00A0: return TEXT("WM_NCMOUSEMOVE");
    case 0x00A1: return TEXT("WM_NCLBUTTONDOWN");
    case 0x00A2: return TEXT("WM_NCLBUTTONUP");
    case 0x00A3: return TEXT("WM_NCLBUTTONDBLCLK");
    case 0x00A4: return TEXT("WM_NCRBUTTONDOWN");
    case 0x00A5: return TEXT("WM_NCRBUTTONUP");
    case 0x00A6: return TEXT("WM_NCRBUTTONDBLCLK");
    case 0x00A7: return TEXT("WM_NCMBUTTONDOWN");
    case 0x00A8: return TEXT("WM_NCMBUTTONUP");
    case 0x00A9: return TEXT("WM_NCMBUTTONDBLCLK");
    case 0x00AB: return TEXT("WM_NCXBUTTONDOWN");
    case 0x00AC: return TEXT("WM_NCXBUTTONUP");
    case 0x00AD: return TEXT("WM_NCXBUTTONDBLCLK");
    case 0x00FE: return TEXT("WM_INPUT_DEVICE_CHANGE");
    case 0x00FF: return TEXT("WM_INPUT");
    case 0x0100: return TEXT("WM_KEYDOWN");
    //case 0x0100: return TEXT("WM_KEYFIRST");
    case 0x0101: return TEXT("WM_KEYUP");
    case 0x0102: return TEXT("WM_CHAR");
    case 0x0103: return TEXT("WM_DEADCHAR");
    case 0x0104: return TEXT("WM_SYSKEYDOWN");
    case 0x0105: return TEXT("WM_SYSKEYUP");
    case 0x0106: return TEXT("WM_SYSCHAR");
    case 0x0107: return TEXT("WM_SYSDEADCHAR");
    case 0x0108: return TEXT("WM_KEYLAST");
    //case 0x0109: return TEXT("WM_KEYLAST");
    case 0x0109: return TEXT("WM_UNICHAR");
    case 0x010D: return TEXT("WM_IME_STARTCOMPOSITION");
    case 0x010E: return TEXT("WM_IME_ENDCOMPOSITION");
    case 0x010F: return TEXT("WM_IME_COMPOSITION");
    //case 0x010F: return TEXT("WM_IME_KEYLAST");
    case 0x0110: return TEXT("WM_INITDIALOG");
    case 0x0111: return TEXT("WM_COMMAND");
    case 0x0112: return TEXT("WM_SYSCOMMAND");
    case 0x0113: return TEXT("WM_TIMER");
    case 0x0114: return TEXT("WM_HSCROLL");
    case 0x0115: return TEXT("WM_VSCROLL");
    case 0x0116: return TEXT("WM_INITMENU");
    case 0x0117: return TEXT("WM_INITMENUPOPUP");
    case 0x0119: return TEXT("WM_GESTURE");
    case 0x011A: return TEXT("WM_GESTURENOTIFY");
    case 0x011F: return TEXT("WM_MENUSELECT");
    case 0x0120: return TEXT("WM_MENUCHAR");
    case 0x0121: return TEXT("WM_ENTERIDLE");
    case 0x0122: return TEXT("WM_MENURBUTTONUP");
    case 0x0123: return TEXT("WM_MENUDRAG");
    case 0x0124: return TEXT("WM_MENUGETOBJECT");
    case 0x0125: return TEXT("WM_UNINITMENUPOPUP");
    case 0x0126: return TEXT("WM_MENUCOMMAND");
    case 0x0127: return TEXT("WM_CHANGEUISTATE");
    case 0x0128: return TEXT("WM_UPDATEUISTATE");
    case 0x0129: return TEXT("WM_QUERYUISTATE");
    case 0x0132: return TEXT("WM_CTLCOLORMSGBOX");
    case 0x0133: return TEXT("WM_CTLCOLOREDIT");
    case 0x0134: return TEXT("WM_CTLCOLORLISTBOX");
    case 0x0135: return TEXT("WM_CTLCOLORBTN");
    case 0x0136: return TEXT("WM_CTLCOLORDLG");
    case 0x0137: return TEXT("WM_CTLCOLORSCROLLBAR");
    case 0x0138: return TEXT("WM_CTLCOLORSTATIC");
    //case 0x0200: return TEXT("WM_MOUSEFIRST");
    case 0x0200: return TEXT("WM_MOUSEMOVE");
    case 0x0201: return TEXT("WM_LBUTTONDOWN");
    case 0x0202: return TEXT("WM_LBUTTONUP");
    case 0x0203: return TEXT("WM_LBUTTONDBLCLK");
    case 0x0204: return TEXT("WM_RBUTTONDOWN");
    case 0x0205: return TEXT("WM_RBUTTONUP");
    case 0x0206: return TEXT("WM_RBUTTONDBLCLK");
    case 0x0207: return TEXT("WM_MBUTTONDOWN");
    case 0x0208: return TEXT("WM_MBUTTONUP");
    case 0x0209: return TEXT("WM_MBUTTONDBLCLK");
    case 0x020A: return TEXT("WM_MOUSEWHEEL");
    case 0x020B: return TEXT("WM_XBUTTONDOWN");
    case 0x020C: return TEXT("WM_XBUTTONUP");
    case 0x020D: return TEXT("WM_XBUTTONDBLCLK");
    case 0x020E: return TEXT("WM_MOUSEHWHEEL");
    //case 0x020E: return TEXT("WM_MOUSELAST");
    case 0x0210: return TEXT("WM_PARENTNOTIFY");
    case 0x0211: return TEXT("WM_ENTERMENULOOP");
    case 0x0212: return TEXT("WM_EXITMENULOOP");
    case 0x0213: return TEXT("WM_NEXTMENU");
    case 0x0214: return TEXT("WM_SIZING");
    case 0x0215: return TEXT("WM_CAPTURECHANGED");
    case 0x0216: return TEXT("WM_MOVING");
    case 0x0218: return TEXT("WM_POWERBROADCAST");
    case 0x0219: return TEXT("WM_DEVICECHANGE");
    case 0x0220: return TEXT("WM_MDICREATE");
    case 0x0221: return TEXT("WM_MDIDESTROY");
    case 0x0222: return TEXT("WM_MDIACTIVATE");
    case 0x0223: return TEXT("WM_MDIRESTORE");
    case 0x0224: return TEXT("WM_MDINEXT");
    case 0x0225: return TEXT("WM_MDIMAXIMIZE");
    case 0x0226: return TEXT("WM_MDITILE");
    case 0x0227: return TEXT("WM_MDICASCADE");
    case 0x0228: return TEXT("WM_MDIICONARRANGE");
    case 0x0229: return TEXT("WM_MDIGETACTIVE");
    case 0x0230: return TEXT("WM_MDISETMENU");
    case 0x0231: return TEXT("WM_ENTERSIZEMOVE");
    case 0x0232: return TEXT("WM_EXITSIZEMOVE");
    case 0x0233: return TEXT("WM_DROPFILES");
    case 0x0234: return TEXT("WM_MDIREFRESHMENU");
    case 0x0238: return TEXT("WM_POINTERDEVICECHANGE");
    case 0x0239: return TEXT("WM_POINTERDEVICEINRANGE");
    case 0x023A: return TEXT("WM_POINTERDEVICEOUTOFRANGE");
    case 0x0240: return TEXT("WM_TOUCH");
    case 0x0241: return TEXT("WM_NCPOINTERUPDATE");
    case 0x0242: return TEXT("WM_NCPOINTERDOWN");
    case 0x0243: return TEXT("WM_NCPOINTERUP");
    case 0x0245: return TEXT("WM_POINTERUPDATE");
    case 0x0246: return TEXT("WM_POINTERDOWN");
    case 0x0247: return TEXT("WM_POINTERUP");
    case 0x0249: return TEXT("WM_POINTERENTER");
    case 0x024A: return TEXT("WM_POINTERLEAVE");
    case 0x024B: return TEXT("WM_POINTERACTIVATE");
    case 0x024C: return TEXT("WM_POINTERCAPTURECHANGED");
    case 0x024D: return TEXT("WM_TOUCHHITTESTING");
    case 0x024E: return TEXT("WM_POINTERWHEEL");
    case 0x024F: return TEXT("WM_POINTERHWHEEL");
    case 0x0251: return TEXT("WM_POINTERROUTEDTO");
    case 0x0252: return TEXT("WM_POINTERROUTEDAWAY");
    case 0x0253: return TEXT("WM_POINTERROUTEDRELEASED");
    case 0x0281: return TEXT("WM_IME_SETCONTEXT");
    case 0x0282: return TEXT("WM_IME_NOTIFY");
    case 0x0283: return TEXT("WM_IME_CONTROL");
    case 0x0284: return TEXT("WM_IME_COMPOSITIONFULL");
    case 0x0285: return TEXT("WM_IME_SELECT");
    case 0x0286: return TEXT("WM_IME_CHAR");
    case 0x0288: return TEXT("WM_IME_REQUEST");
    case 0x0290: return TEXT("WM_IME_KEYDOWN");
    case 0x0291: return TEXT("WM_IME_KEYUP");
    case 0x02A0: return TEXT("WM_NCMOUSEHOVER");
    case 0x02A1: return TEXT("WM_MOUSEHOVER");
    case 0x02A2: return TEXT("WM_NCMOUSELEAVE");
    case 0x02A3: return TEXT("WM_MOUSELEAVE");
    case 0x02B1: return TEXT("WM_WTSSESSION_CHANGE");
    case 0x02E0: return TEXT("WM_DPICHANGED");
    case 0x02c0: return TEXT("WM_TABLET_FIRST");
    case 0x02df: return TEXT("WM_TABLET_LAST");
    case 0x0300: return TEXT("WM_CUT");
    case 0x0301: return TEXT("WM_COPY");
    case 0x0302: return TEXT("WM_PASTE");
    case 0x0303: return TEXT("WM_CLEAR");
    case 0x0304: return TEXT("WM_UNDO");
    case 0x0305: return TEXT("WM_RENDERFORMAT");
    case 0x0306: return TEXT("WM_RENDERALLFORMATS");
    case 0x0307: return TEXT("WM_DESTROYCLIPBOARD");
    case 0x0308: return TEXT("WM_DRAWCLIPBOARD");
    case 0x0309: return TEXT("WM_PAINTCLIPBOARD");
    case 0x030A: return TEXT("WM_VSCROLLCLIPBOARD");
    case 0x030B: return TEXT("WM_SIZECLIPBOARD");
    case 0x030C: return TEXT("WM_ASKCBFORMATNAME");
    case 0x030D: return TEXT("WM_CHANGECBCHAIN");
    case 0x030E: return TEXT("WM_HSCROLLCLIPBOARD");
    case 0x030F: return TEXT("WM_QUERYNEWPALETTE");
    case 0x0310: return TEXT("WM_PALETTEISCHANGING");
    case 0x0311: return TEXT("WM_PALETTECHANGED");
    case 0x0312: return TEXT("WM_HOTKEY");
    case 0x0317: return TEXT("WM_PRINT");
    case 0x0318: return TEXT("WM_PRINTCLIENT");
    case 0x0319: return TEXT("WM_APPCOMMAND");
    case 0x031A: return TEXT("WM_THEMECHANGED");
    case 0x031D: return TEXT("WM_CLIPBOARDUPDATE");
    case 0x031E: return TEXT("WM_DWMCOMPOSITIONCHANGED");
    case 0x031F: return TEXT("WM_DWMNCRENDERINGCHANGED");
    case 0x0320: return TEXT("WM_DWMCOLORIZATIONCOLORCHANGED");
    case 0x0321: return TEXT("WM_DWMWINDOWMAXIMIZEDCHANGE");
    case 0x0323: return TEXT("WM_DWMSENDICONICTHUMBNAIL");
    case 0x0326: return TEXT("WM_DWMSENDICONICLIVEPREVIEWBITMAP");
    case 0x033F: return TEXT("WM_GETTITLEBARINFOEX");
    case 0x0358: return TEXT("WM_HANDHELDFIRST");
    case 0x035F: return TEXT("WM_HANDHELDLAST");
    case 0x0360: return TEXT("WM_AFXFIRST");
    case 0x037F: return TEXT("WM_AFXLAST");
    case 0x0380: return TEXT("WM_PENWINFIRST");
    case 0x038F: return TEXT("WM_PENWINLAST");
    case 0x0400: return TEXT("WM_USER");
    case 0x8000: return TEXT("WM_APP");
    default: return 0;
    }
  };
}
