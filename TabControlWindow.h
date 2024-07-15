// TabControlWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#include "ControlWindow.h"

#define TAB_CONTROL_WINDOW_CLASS_NAME											WC_TABCONTROL

#define TAB_CONTROL_WINDOW_EXTENDED_STYLE										0
#define TAB_CONTROL_WINDOW_STYLE												( WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE )
#define TAB_CONTROL_WINDOW_TEXT													NULL

#define TAB_CONTROL_WINDOW_TITLE_FORMAT_STRING									"Tab %d"

#define TAB_CONTROL_WINDOW_FILE_NAME											"Tabs.txt"

#define TAB_CONTROL_WINDOW_FIRST_CONTROL_WINDOW_ID								( WM_USER + 10 )

BOOL IsTabControlWindow( HWND hWnd );

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int TabControlWindowGetControlWindowID( int nWhichTab );

BOOL TabControlWindowGetRect( LPRECT lpRect );

BOOL TabControlWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, HWND hWndMain );

int TabControlWindowLoad( HWND hWndMain, LPCTSTR lpszFileName );

BOOL TabControlWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int TabControlWindowNewTab( HWND hWndMain, LPCTSTR lpszTitle );

BOOL TabControlWindowSelectTab( HWND hWndMain, int nWhichTab );

HWND TabControlWindowSetFocus();

void TabControlWindowSetFont( HFONT hFont );

BOOL TabControlWindowShowControlWindow( HWND hWndMain, int nSelectedTab );
