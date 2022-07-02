// ControlWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#define CONTROL_WINDOW_CLASS_NAME												"Edit"
#define CONTROL_WINDOW_EXTENDED_STYLE											0
#define CONTROL_WINDOW_STYLE													( WS_CHILD | WS_TABSTOP | WS_BORDER )
#define CONTROL_WINDOW_TEXT														NULL

HWND ControlWindowCreate( HWND hWndParent, int nID );

BOOL ControlWindowMove( HWND hWndParent, int nID, int nX, int nY, int nWidth, int nHeight );
