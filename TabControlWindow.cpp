// TabControlWindow.cpp

#include "TabControlWindow.h"

// Global variables
static HWND g_hWndTabControl;

BOOL IsTabControlWindow( HWND hWnd )
{
	// See if supplied window is tab control window
	return( hWnd == g_hWndTabControl );

} // End of function IsTabControlWindow

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create tab control window
	g_hWndTabControl = CreateWindowEx( TAB_CONTROL_WINDOW_EXTENDED_STYLE, TAB_CONTROL_WINDOW_CLASS_NAME, TAB_CONTROL_WINDOW_TEXT, TAB_CONTROL_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that tab control window was created
	if( g_hWndTabControl )
	{
		// Successfully created tab control window
		TCITEM tcItem;
		int nWhichDay;
		LPCTSTR lpszTitles [] = TAB_CONTROL_WINDOW_TITLES;

		// Clear tab control item structure
		ZeroMemory( &tcItem, sizeof( tcItem ) );

		// Initialise tab control item structure
		tcItem.mask = TCIF_TEXT;

		// Loop through tabs
		for ( nWhichDay = 0; nWhichDay < TAB_CONTROL_WINDOW_NUMBER_OF_TABS; nWhichDay ++)
		{
			// Update tab control item structure for tab
			tcItem.pszText = ( LPTSTR )( lpszTitles[ nWhichDay ] );

			// Add tab
			SendMessage( g_hWndTabControl, TCM_INSERTITEM, ( WPARAM )nWhichDay, ( LPARAM )&tcItem );

		} // End of loop through tabs

		// Update return value
		bResult = TRUE;

	} // End of successfully created tab control window

	return bResult;

} // End of function TabControlWindowCreate

BOOL TabControlWindowGetItemText( int nWhichItem, LPTSTR lpszItemText )
{
	BOOL bResult = FALSE;

	TCITEM tcItem;

	// Clear tab control item structure
	ZeroMemory( &tcItem, sizeof( tcItem ) );

	// Initialise tab control item structure
	tcItem.mask			= TCIF_TEXT;
	tcItem.cchTextMax	= STRING_LENGTH;
	tcItem.pszText		= lpszItemText;

	// Get tab control item
	if( SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nWhichItem, ( LPARAM )&tcItem ) )
	{
		// Successfully got tab control item

		// Update return value
		bResult = TRUE;

	} // End of successfully got tab control item

	return bResult;

} // End of function TabControlWindowGetItemText

BOOL TabControlWindowGetRect( LPRECT lpRect )
{
	// Get tab control window rect
	return GetWindowRect( g_hWndTabControl, lpRect );

} // End of function TabControlWindowGetRect

int TabControlWindowGetSelectedItem()
{
	// Get selected item
	return SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function TabControlWindowGetSelectedItem

BOOL TabControlWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmhdr;

	// Get notify message handler
	lpNmhdr = ( ( LPNMHDR )lParam );

	// Select tab control window notification code
	switch( lpNmhdr->code )
	{
		case TCN_SELCHANGING:
		{
			// A selection changing notification code

			// Break out of switch
			break;

		}// End of a selection changing notification code
		case TCN_SELCHANGE:
		{
			// A selection change notification code
			int nSelectedItem;

			// Get selected item
			nSelectedItem = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Ensure that selected item was got
			if( nSelectedItem >= 0 )
			{
				// Successfully got selected item

				// Call item selected function
				bResult = TabControlWindowOnItemSelected( nSelectedItem, lpStatusFunction );

			} // End of successfully got selected item

			// Break out of switch
			break;

		}// End of a selection change notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for tab control window notification code

	return bResult;

} // End of function TabControlWindowHandleNotifyMessage

BOOL TabControlWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move tab control window
	return MoveWindow( g_hWndTabControl, nX, nY, nWidth, nHeight, bRepaint );

} // End of function TabControlWindowMove

BOOL TabControlWindowOnItemSelected( int nWhichItem, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

	// Get item text
	if( TabControlWindowGetItemText( nWhichItem, lpszItemText ) )
	{
		// Successfully got item text

		// Show item text on status bar window
		( *lpStatusFunction )( lpszItemText );

		// Update return value
		bResult = TRUE;

	} // End of successfully got item text

	// Free string memory
	delete [] lpszItemText;

	return bResult;

} // End of function TabControlWindowOnItemSelected

HWND TabControlWindowSetFocus()
{
	// Focus on tab control window
	return SetFocus( g_hWndTabControl );

} // End of function TabControlWindowSetFocus

void TabControlWindowSetFont( HFONT hFont )
{
	// Set tab control window font
	SendMessage( g_hWndTabControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function TabControlWindowSetFont
