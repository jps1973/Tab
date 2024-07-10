// TabWindow.cpp

#include "TabWindow.h"

// Global variables
HWND g_hWndTab;
HINSTANCE g_hInstance;

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
			TCITEM tabControlItem;
			int nWhichTabItem;

			// Allocate string memory
			LPTSTR lpszTitle = new char[ STRING_LENGTH ];

			// Clear tab control item structure
			ZeroMemory( &tabControlItem, sizeof( tabControlItem ) );

			// Initialise tab control item structure
			tabControlItem.mask		= TCIF_TEXT;
			tabControlItem.pszText	= lpszTitle;

			// Update return value (assume success)
			bResult = TRUE;

			// Loop through tab control items
			for ( nWhichTabItem = 0; nWhichTabItem < TAB_WINDOW_DAYS_IN_WEEK; nWhichTabItem ++ ) 
			{
				// Format title
				wsprintf( lpszTitle, TAB_WINDOW_TITLE_FORMAT_STRING, nWhichTabItem );

				// Insert tab control item
				if( SendMessage( g_hWndTab, TCM_INSERTITEM, ( WPARAM )nWhichTabItem, ( LPARAM )&tabControlItem ) < 0 )
				{
					// Unable to insert tab control item

					// Update return value
					bResult = FALSE;

					// Force exit from loop
					nWhichTabItem = TAB_WINDOW_DAYS_IN_WEEK;

				} // End of unable to insert tab control item

			}; // End of loop through tab control items

			// Free string memory
			delete[] lpszTitle;

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
