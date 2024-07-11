// TabWindow.cpp

#include "TabWindow.h"

// Global variables
HWND g_hWndTab;
HINSTANCE g_hInstance;
int g_nNumberOfTabs;
int g_nNextTabNumber;


BOOL IsTabWindow( HWND hWnd )
{
	// See if supplied window is tab window
	return( hWnd == g_hWndTab );

} // End of function IsTabWindow

BOOL TabWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	INITCOMMONCONTROLSEX iccex;

	// Clear init common control structure
	ZeroMemory( &iccex, sizeof( iccex ) );

	// Initialise init common control structure
	iccex.dwSize	= sizeof( INITCOMMONCONTROLSEX );
	iccex.dwICC		= ICC_TAB_CLASSES;

	// Initialise common controls
	if( InitCommonControlsEx( &iccex ) )
	{
		// Successfully initilised common controls
		RECT rcClient;
		int nClientWidth;
		int nClientHeight;

		// Update global instance handle
		g_hInstance = hInstance;

		// Get parent window client size
		GetClientRect( hWndParent, &rcClient );

		// Calculate client width and height
		nClientWidth	= ( rcClient.right - rcClient.left );
		nClientHeight	= ( rcClient.bottom - rcClient.top );

		// Create tab window
		g_hWndTab = ::CreateWindowEx( TAB_WINDOW_EXTENDED_STYLE, TAB_WINDOW_CLASS_NAME, TAB_WINDOW_TEXT, TAB_WINDOW_STYLE, 0, 0, nClientWidth, nClientHeight, hWndParent, ( HMENU )NULL, g_hInstance, NULL );

		// Ensure that tab window was created
		if( g_hWndTab )
		{
			// Successfully created tab window

			// Initialise global variables
			g_nNumberOfTabs		= 0;
			g_nNextTabNumber	= 1;

			// Update return value (assume success)
			bResult = TRUE;

		} // End of successfully initilised common controls

	} // End of successfully created tab window

	return bResult;

} // End of function TabWindowCreate

BOOL TabWindowGetRect( LPRECT lpRect )
{
	// Get tab window rect
	return ::GetWindowRect( g_hWndTab, lpRect );

} // End of function TabWindowGetRect

BOOL TabWindowHandleCommandMessage( WPARAM wParam, LPARAM, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select tab window notification code
	switch( HIWORD( wParam ) )
	{
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for tab window notification code

	return bResult;

} // End of function TabWindowHandleCommandMessage

BOOL TabWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move tab window
	return ::MoveWindow( g_hWndTab, nX, nY, nWidth, nHeight, bRepaint );

} // End of function TabWindowMove

int TabWindowNewTab( LPCTSTR lpszTitle )
{
	int nResult = -1;

	TCITEM tabControlItem;

	// Clear tab control item structure
	ZeroMemory( &tabControlItem, sizeof( tabControlItem ) );

	// Initialise tab control item structure
	tabControlItem.mask		= TCIF_TEXT;
	tabControlItem.pszText	= ( LPTSTR )lpszTitle;

	// Insert tab control item
	nResult = SendMessage( g_hWndTab, TCM_INSERTITEM, ( WPARAM )g_nNumberOfTabs, ( LPARAM )&tabControlItem );

	// Ensure that tab control item was inserted
	if( nResult >= 0 )
	{
		// Successfully inserted tab control item

		// Update global variables
		g_nNextTabNumber ++;
		g_nNumberOfTabs ++;

	} // End of successfully inserted tab control item

	return nResult;

} // End of function TabWindowNewTab

HWND TabWindowSetFocus()
{
	// Focus on tab window
	return ::SetFocus( g_hWndTab );

} // End of function TabWindowSetFocus

void TabWindowSetFont( HFONT hFont )
{
	// Set tab window font
	::SendMessage( g_hWndTab, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function TabWindowSetFont
