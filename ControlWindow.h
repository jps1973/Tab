// ControlWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Common.h"

#ifndef LVM_SETEXTENDEDLISTVIEWSTYLE
#define LVM_SETEXTENDEDLISTVIEWSTYLE											( LVM_FIRST + 54 )
#endif
 
#ifndef LVS_EX_FULLROWSELECT
#define LVS_EX_FULLROWSELECT													0x00000020
#endif

#define CONTROL_WINDOW_CLASS_NAME												WC_LISTVIEW
#define CONTROL_WINDOW_EXTENDED_STYLE											LVS_EX_FULLROWSELECT
#define CONTROL_WINDOW_STYLE													( WS_BORDER | WS_CHILD | WS_VISIBLE | LVS_NOCOLUMNHEADER | LVS_REPORT | LVS_SORTASCENDING )
#define CONTROL_WINDOW_TEXT														NULL

#define CONTROL_WINDOW_COLUMN_TITLES											{ "Name" }
#define CONTROL_WINDOW_DEFAULT_COLUMN_WIDTH										100

HWND ControlWindowCreate( HWND hWndParent, int nID );

BOOL ControlWindowGetItemPath( HWND hWndControl, int nWhichItem, LPCTSTR lpszParentFolderPath, LPTSTR lpszItemPath );

BOOL ControlWindowMove( HWND hWndParent, int nID, int nX, int nY, int nWidth, int nHeight );

int ControlWindowPopulate( HWND hWndControl, LPCTSTR lpszFolderPath, LPCTSTR lpszFileFilter );

enum ControlWindowColumns
{
	NAME_COLUMN_ID = 0,

	NUMBER_OF_COLUMNS

}; // End of enum ControlWindowColumns