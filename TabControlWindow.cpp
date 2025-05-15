// TabControlWindow.cpp

#include "TabControlWindow.h"

// Global variables
static HWND g_hWndTabControl;
static HWND g_hWndActiveControl;

BOOL IsTabControlWindow( HWND hWnd )
{
	// See if supplied window is tab control window
	return( hWnd == g_hWndTabControl );

} // End of function IsTabControlWindow

int TabControlWindowAddItem( HINSTANCE hInstance, LPCTSTR lpszItemText )
{
	int nResult = -1;

	HWND hWndControl;

	// Create control window
	hWndControl = ControlWindowCreate( g_hWndTabControl, hInstance );

	// Ensure that control window was created
	if( hWndControl )
	{
		// Successfully created control window
		TAB_CONTROL_WINDOW_DATA tcwData;
		int nItemCount;
		HFONT hFont;

		// Clear tab control window data structure
		ZeroMemory( &tcwData, sizeof( tcwData ) );

		// Initialise tab control window data structure
		tcwData.tcItemHeader.mask		= ( TCIF_TEXT | TCIF_PARAM );
		tcwData.hWndControl				= hWndControl;
		tcwData.tcItemHeader.cchTextMax	= STRING_LENGTH;
		tcwData.tcItemHeader.pszText	= ( LPTSTR )lpszItemText;
		wsprintf( tcwData.cData, "Data for tab %s.", lpszItemText );

		// Get font
		hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

		// Set control window font
		SendMessage( hWndControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

		// Add text to control window
		SendMessage( hWndControl, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszItemText );

		// Count items on tab
		nItemCount = SendMessage( g_hWndTabControl, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Add tab
		nResult = SendMessage( g_hWndTabControl, TCM_INSERTITEM, ( WPARAM )nItemCount, ( LPARAM )( LPTCITEMHEADER )( &tcwData ) );

	} // End of successfully created control window

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

			// Clear active control window
			g_hWndActiveControl = NULL;

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
	BOOL bResult = FALSE;

	// Move tab control window
	if( MoveWindow( g_hWndTabControl, nX, nY, nWidth, nHeight, bRepaint ) )
	{
		// Successfully moved tab control window

		// Move control window
		if( TabControlWindowMoveControlWindow() )
		{
			// Successfully moved control window

			// Update return value
			bResult = TRUE;

		} // End of successfully moved control window

	} // End of successfully moved tab control window

	return bResult;

} // End of function TabControlWindowMove

BOOL TabControlWindowMoveControlWindow()
{
	BOOL bResult = FALSE;

	RECT rc;
	int nControlWindowWidth;
	int nControlWindowHeight;

	// Get tab control window size
	GetClientRect( g_hWndTabControl, &rc );

	// Adjust rect for control window
	SendMessage( g_hWndTabControl, TCM_ADJUSTRECT, ( WPARAM )FALSE, ( LPARAM )&rc );

	// Calculate control window size
	nControlWindowWidth		= ( rc.right - rc.left );
	nControlWindowHeight	= ( rc.bottom - rc.top );

	// Move control window
	bResult = MoveWindow( g_hWndActiveControl, rc.left, rc.top, nControlWindowWidth, nControlWindowHeight, TRUE );

	return bResult;

} // End of function TabControlWindowMoveControlWindow

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

		// See if active control window is valid
		if( g_hWndActiveControl )
		{
			// Active control window is valid

			// Hide active control window
			ShowWindow( g_hWndActiveControl, SW_HIDE );

		} // End of active control window is valid

		// Update active control window
		g_hWndActiveControl = tcwData.hWndControl;

		// Move active control window
		TabControlWindowMoveControlWindow();

		// Show active control window
		ShowWindow( g_hWndActiveControl, SW_SHOW );

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
