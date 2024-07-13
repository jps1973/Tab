// TabControlWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define TAB_CONTROL_WINDOW_CLASS_NAME											WC_TABCONTROL

#define TAB_CONTROL_WINDOW_EXTENDED_STYLE										0
#define TAB_CONTROL_WINDOW_STYLE												( WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE )
#define TAB_CONTROL_WINDOW_TEXT													NULL

#define TAB_CONTROL_WINDOW_TITLE_FORMAT_STRING									"Tab %d"

#define TAB_CONTROL_WINDOW_FILE_NAME											"Tabs.txt"

BOOL IsTabControlWindow( HWND hWnd );

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TabControlWindowGetRect( LPRECT lpRect );

BOOL TabControlWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam );

int TabControlWindowLoad( LPCTSTR lpszFileName );

BOOL TabControlWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int TabControlWindowNewTab( LPCTSTR lpszTitle );

HWND TabControlWindowSetFocus();

void TabControlWindowSetFont( HFONT hFont );

BOOL TabControlWindowSize( LPARAM lParam );
