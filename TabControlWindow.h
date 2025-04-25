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

BOOL IsTabControlWindow( HWND hWnd );

int TabControlWindowAddItem( LPCTSTR lpszItemText );

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TabControlWindowGetItemText( int nWhichItem, LPTSTR lpszItemText );

BOOL TabControlWindowGetRect( LPRECT lpRect );

int TabControlWindowGetSelectedItem();

BOOL TabControlWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL TabControlWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

BOOL TabControlWindowOnItemSelected( int nWhichItem, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

HWND TabControlWindowSetFocus();

void TabControlWindowSetFont( HFONT hFont );
