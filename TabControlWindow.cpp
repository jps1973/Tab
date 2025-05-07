// TabControlWindow.cpp

#include "TabControlWindow.h"

// Global variables
static HWND g_hWndTabControl;

BOOL IsTabControlWindow( HWND hWnd )
{
	// See if supplied window is tab control window
	return( hWnd == g_hWndTabControl );

} // End of function IsTabControlWindow

int TabControlWindowAddItem( LPCTSTR lpszItemText )
{
	int nResult = -1;

	TAB_CONTROL_WINDOW_DATA tcwData;
	int nItemCount;

	// Clear tab control window data structure
	ZeroMemory( &tcwData, sizeof( tcwData ) );

	// Initialise tab control window data structure
	tcwData.tcItemHeader.mask		= ( TCIF_TEXT | TCIF_PARAM );
	tcwData.tcItemHeader.cchTextMax	= STRING_LENGTH;
	tcwData.tcItemHeader.pszText	= ( LPTSTR )lpszItemText;
	wsprintf( tcwData.cData, "Data for tab %s.", lpszItemText );

	// Count items on tab
	nItemCount = SendMessage( g_hWndTabControl, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Add tab
	nResult = SendMessage( g_hWndTabControl, TCM_INSERTITEM, ( WPARAM )nItemCount, ( LPARAM )( LPTCITEMHEADER )( &tcwData ) );

	return nResult;

} // End of function TabControlWindowAddItem

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create tab control window
	g_hWndTabControl = CreateWindowEx( TAB_CONTROL_WINDOW_EXTENDED_STYLE, TAB_CONTROL_WINDOW_CLASS_NAME, TAB_CONTROL_WINDOW_TEXT, TAB_CONTROL_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that tab control window was created
	if( g_hWndTabControl )
	{
		// Successfully created tab control window
		int nExtraBytes;

		// Calculate extra bytes
		nExtraBytes = ( sizeof( TAB_CONTROL_WINDOW_DATA ) - sizeof( TCITEMHEADER ) );

		// Allocate extra memory for tab data
		if( SendMessage( g_hWndTabControl, TCM_SETITEMEXTRA, ( WPARAM )nExtraBytes, ( LPARAM )NULL ) )
		{
			// Successfully allocated extra memory for tab data

			// Update return value
			bResult = TRUE;

		} // End of successfully allocated extra memory for tab data

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

	TAB_CONTROL_WINDOW_DATA tcwData;

	// Allocate string memory
	LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear tab control window data structure
	ZeroMemory( &tcwData, sizeof( tcwData ) );

	// Initialise tab control window data structure
	tcwData.tcItemHeader.mask		= ( TCIF_TEXT | TCIF_PARAM );
	tcwData.tcItemHeader.cchTextMax	= STRING_LENGTH;
	tcwData.tcItemHeader.pszText	= ( LPTSTR )lpszItemText;

	// Get tab control item
	if( SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nWhichItem, ( LPARAM )( LPTCITEMHEADER )( &tcwData ) ) )
	{
		// Successfully got tab control item

		// Show item text on status bar window
		( *lpStatusFunction )( tcwData.cData );

		// Update return value
		bResult = TRUE;

	} // End of successfully got tab control item

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
