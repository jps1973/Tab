// ControlWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define CONTROL_WINDOW_CLASS_NAME												WC_LISTBOX

#define CONTROL_WINDOW_EXTENDED_STYLE											0
#define CONTROL_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | LBS_NOINTEGRALHEIGHT )
#define CONTROL_WINDOW_TEXT														NULL

HWND ControlWindowCreate( HWND hWndParent, HINSTANCE hInstance, LPCTSTR lpszData );

BOOL ControlWindowMove( HWND hWndControl, HWND hWndTabControl );

void ControlWindowSetFont( HWND hWndControl, int nWhichFont );
