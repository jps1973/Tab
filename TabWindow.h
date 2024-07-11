// TabWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define TAB_WINDOW_CLASS_NAME													WC_TABCONTROL

#define TAB_WINDOW_EXTENDED_STYLE												0
#define TAB_WINDOW_STYLE														( WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE )
#define TAB_WINDOW_TEXT															NULL

#define TAB_WINDOW_TITLE_FORMAT_STRING											"Tab %d"

BOOL IsTabWindow( HWND hWnd );

BOOL TabWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TabWindowGetRect( LPRECT lpRect );

BOOL TabWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) );

BOOL TabWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int TabWindowNewTab( LPCTSTR lpszTitle );

HWND TabWindowSetFocus();

void TabWindowSetFont( HFONT hFont );
