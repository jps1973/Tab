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

#define TAB_CONTROL_WINDOW_UNABLE_TO_SAVE_TABS_WARNING_MESSAGE					"Unable to Save Tabs.\r\n\r\nDo you still want to close?"

typedef struct
{
	TCITEMHEADER tcItemHeader;

	HWND hWndControl;
	char cData[ STRING_LENGTH + sizeof( char ) ];

} TAB_CONTROL_WINDOW_DATA;

BOOL IsTabControlWindow( HWND hWnd );

int TabControlWindowAddItem( HINSTANCE hInstance, LPCTSTR lpszItemText );

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TabControlWindowGetItemText( int nWhichItem, LPTSTR lpszItemText );

BOOL TabControlWindowGetRect( LPRECT lpRect );

int TabControlWindowGetSelectedItem();

BOOL TabControlWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

int TabControlWindowLoad( LPCTSTR lpszFileName, HINSTANCE hInstance );

BOOL TabControlWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

BOOL TabControlWindowMoveControlWindow();

BOOL TabControlWindowOnItemSelected( int nWhichItem, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

int TabControlWindowSave( LPCTSTR lpszFileName );

HWND TabControlWindowSetFocus();

void TabControlWindowSetFont( HFONT hFont );
