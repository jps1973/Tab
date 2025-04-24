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

#define TAB_CONTROL_WINDOW_TITLES												{ "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" }

typedef enum TabIDs
{
	TAB_CONTROL_WINDOW_MONDAY_TAB_ID,
	TAB_CONTROL_WINDOW_TUESDAY_TAB_ID,
	TAB_CONTROL_WINDOW_WEDNESDAY_TAB_ID,
	TAB_CONTROL_WINDOW_THURSDAY_TAB_ID,
	TAB_CONTROL_WINDOW_FRIDAY_TAB_ID,
	TAB_CONTROL_WINDOW_SATURDAY_TAB_ID,
	TAB_CONTROL_WINDOW_SUNDAY_TAB_ID,

	TAB_CONTROL_WINDOW_NUMBER_OF_TABS

} TAB_IDS;

BOOL IsTabControlWindow( HWND hWnd );

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TabControlWindowGetRect( LPRECT lpRect );

BOOL TabControlWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL TabControlWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL TabControlWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND TabControlWindowSetFocus();

void TabControlWindowSetFont( HFONT hFont );
