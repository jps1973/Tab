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

HWND ControlWindowCreate( HWND hWndParent, HINSTANCE hInstance, int nID, LPCTSTR lpszParentFolderPath );

BOOL ControlWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, LPCTSTR lpszParentFolderPath, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusText ) );

BOOL ControlWindowMove( HWND hWndControl, HWND hWndTabControl );

int ControlWindowPopulate( HWND hWndControl, LPCTSTR lpszParentFolderPath );

void ControlWindowSetFont( HWND hWndControl, int nWhichFont );
