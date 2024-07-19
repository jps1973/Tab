// TabControlWindow.cpp

#include "TabControlWindow.h"

// Global variables
static HWND g_hWndTabControl;
static HINSTANCE g_hInstance;
static int g_nNumberOfTabs;

BOOL IsTabControlWindow( HWND hWnd )
{
	// See if supplied window is tab control window
	return( hWnd == g_hWndTabControl );

} // End of function IsTabControlWindow

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance )
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

		// Create tab control window
		g_hWndTabControl = ::CreateWindowEx( TAB_CONTROL_WINDOW_EXTENDED_STYLE, TAB_CONTROL_WINDOW_CLASS_NAME, TAB_CONTROL_WINDOW_TEXT, TAB_CONTROL_WINDOW_STYLE, 0, 0, nClientWidth, nClientHeight, hWndParent, ( HMENU )NULL, g_hInstance, NULL );

		// Ensure that tab control window was created
		if( g_hWndTabControl )
		{
			// Successfully created tab control window
			int nExtraBytes;

			// Calculate extra bytes required for tab data
			nExtraBytes = ( sizeof( TabData ) - sizeof( TCITEMHEADER ) );

			// Set extra bytes
			SendMessage( g_hWndTabControl, TCM_SETITEMEXTRA, ( WPARAM )nExtraBytes, ( LPARAM )0 );

			// Initialise global variables
			g_nNumberOfTabs = 0;

			// Update return value (assume success)
			bResult = TRUE;

		} // End of successfully initilised common controls

	} // End of successfully created tab control window

	return bResult;

} // End of function TabControlWindowCreate

HWND TabControlWindowGetControlWindow( int nWhichTab )
{
	HWND hWndResult = NULL;

	TabData tabData;

	// Clear tab control structure
	::ZeroMemory( &tabData, sizeof( tabData ) );

	// Initialise tab control structure
	tabData.tcItemHeader.mask = TCIF_PARAM;

	// Get tab data
	if( ::SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nWhichTab, ( LPARAM )&tabData ) )
	{
		// Successfully got tab data

		// Update return value
		hWndResult = tabData.hWndControl;

	} // End of successfully got tab data

	return hWndResult;

} // End of function TabControlWindowGetControlWindow

int TabControlWindowGetControlWindowID( int nWhichTab )
{
	int nResult = 0;

	TCITEM tcItem;

	// Clear tab control structure
	::ZeroMemory( &tcItem, sizeof( tcItem ) );

	// Initialise tab control structure
	tcItem.mask = TCIF_PARAM;

	// Get tab control item
	if( ::SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nWhichTab, ( LPARAM )&tcItem ) )
	{
		// Successfully got tab control item

		// Update return value
		nResult = tcItem.lParam;

	} // End of successfully got tab control item

	return nResult;

} // End of function TabControlWindowGetControlWindowID

BOOL TabControlWindowGetRect( LPRECT lpRect )
{
	// Get tab control window rect
	return ::GetWindowRect( g_hWndTabControl, lpRect );

} // End of function TabControlWindowGetRect

BOOL TabControlWindowHandleNotifyMessage( WPARAM, LPARAM lParam, HWND hWndMain )
{
	BOOL bResult = FALSE;

	// Select tab control window notification code
	switch( ( ( LPNMHDR )lParam )->code )
	{
		case TCN_SELCHANGING:
		{
			// A selection changing message

			// Break out of switch
			break;

		} // End of a selection changing message
		case TCN_SELCHANGE:
		{
			// A selection change message
			int nSelectedTab;

			// Get selected tab
			nSelectedTab = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Ensure that selected tab was got
			if( nSelectedTab >= 0 )
			{
				// Successfully got selected tab

				// Select tab
				TabControlWindowSelectTab( hWndMain, nSelectedTab );

			} // End of successfully got selected tab

			// Break out of switch
			break;

		} // End of a selection change message
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

int TabControlWindowLoad( HWND hWndMain, LPCTSTR lpszFileName )
{
	int nResult = -1;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size

			// Allocate string memory
			LPSTR lpszFileText = new char[ dwFileSize + sizeof( char ) ];

			// Read file text
			if( ReadFile( hFile, lpszFileText, dwFileSize, NULL, NULL ) )
			{
				// Successfully read file text
				LPTSTR lpszTab;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first tab in file text
				lpszTab = strtok( lpszFileText, NEW_LINE_TEXT );

				// Initialise return value
				nResult = 0;

				// Loop through all tabs in file text
				while( lpszTab )
				{
					// Create new tab
					if( TabControlWindowNewTab( hWndMain, lpszTab ) >= 0 )
					{
						// Successfully created new tab

						// Update return value
						nResult ++;

						// Get next tab in file text
						lpszTab = strtok( NULL, NEW_LINE_TEXT );

					} // End of successfully created new tab
					else
					{
						// Unable to create new tab

						// Update return value
						nResult = -1;

						// Force exit from loop
						lpszTab = NULL;

					} // End of unable to create new tab

				}; // End of loop through all tabs in file text

			} // End of successfully read file text

			// Free string memory
			delete [] lpszFileText;

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return nResult;

} // End of function TabControlWindowLoad

BOOL TabControlWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move tab control window
	return ::MoveWindow( g_hWndTabControl, nX, nY, nWidth, nHeight, bRepaint );

} // End of function TabControlWindowMove

BOOL TabControlWindowMoveControlWindow()
{
	BOOL bResult = FALSE;

	int nSelectedTab;

	// Get selected tab
	nSelectedTab = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Ensure that selected tab was got
	if( nSelectedTab >= 0 )
	{
		// Successfully got selected tab
		HWND hWndControl;

		// Get control window
		hWndControl = TabControlWindowGetControlWindow( nSelectedTab );

		// Ensure that control window was got
		if( hWndControl )
		{
			// Successfully got control window

			// Move control indow
			if( ControlWindowMove( hWndControl, g_hWndTabControl ) )
			{
				// Successfully moved control window

				// Update return value
				bResult = TRUE;

			} // End of successfully moved control window

		} // End of successfully got control window

	} // End of successfully got selected tab

	return bResult;

} // End of function TabControlWindowMoveControlWindow

int TabControlWindowNewTab( HWND hWndMain, LPCTSTR lpszTitle )
{
	int nResult = -1;

	TabData tabData;

	// Clear tab data structure
	ZeroMemory( &tabData, sizeof( tabData ) );

	// Initialise tab control item structure
	tabData.tcItemHeader.mask		= ( TCIF_TEXT | TCIF_PARAM );
	tabData.tcItemHeader.cchTextMax	= STRING_LENGTH;
	tabData.tcItemHeader.pszText	= ( LPTSTR )lpszTitle;

	// Create control window
	tabData.hWndControl = ControlWindowCreate( hWndMain, g_hInstance, lpszTitle );

	// Ensure that control window was created
	if( tabData.hWndControl )
	{
		// Successfully created control window

		// Insert tab control item
		nResult = SendMessage( g_hWndTabControl, TCM_INSERTITEM, ( WPARAM )g_nNumberOfTabs, ( LPARAM )&tabData );

		// Ensure that tab control item was inserted
		if( nResult >= 0 )
		{
			// Successfully inserted tab control item

			// Move control window
			ControlWindowMove( tabData.hWndControl, g_hWndTabControl );

			// Update global variables
			g_nNumberOfTabs ++;

		} // End of successfully inserted tab control item

	} // End of successfully created control window

	return nResult;

} // End of function TabControlWindowNewTab
BOOL TabControlWindowSelectTab( HWND hWndMain, int nWhichTab )
{
	BOOL bResult = FALSE;

	// Select tab
	if( ::SendMessage( g_hWndTabControl, TCM_SETCURSEL, ( WPARAM )nWhichTab, ( LPARAM )NULL ) >= 0 )
	{
		// Successfully selected tab

		// Show control window for tab
		bResult = TabControlWindowShowControlWindow( hWndMain, nWhichTab );

	} // End of successfully selected tab

	return bResult;

} // End of function TabControlWindowSelectTab

HWND TabControlWindowSetFocus()
{
	// Focus on tab control window
	return ::SetFocus( g_hWndTabControl );

} // End of function TabControlWindowSetFocus

void TabControlWindowSetFont( HFONT hFont )
{
	// Set tab control window font
	::SendMessage( g_hWndTabControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function TabControlWindowSetFont

BOOL TabControlWindowShowControlWindow( HWND hWndMain, int nSelectedTab )
{
	BOOL bResult = FALSE;

	int nTabCount;
	int nWhichTab;
	HWND hWndControl;

	// Count tabs
	nTabCount = ::SendMessage( g_hWndTabControl, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Loop through all tabs
	for( nWhichTab = 0; nWhichTab < nTabCount; nWhichTab ++ )
	{
		// Get control window
		hWndControl = TabControlWindowGetControlWindow( nWhichTab );

		// Ensure that control window was got
		if( hWndControl )
		{
			// Successfully got control window

			// See if this is the selected tab
			if( nWhichTab == nSelectedTab )
			{
				// This is the selected tab

				// Move control window
				ControlWindowMove( hWndControl, g_hWndTabControl );

				// Show control window
				ShowWindow( hWndControl, SW_SHOW );

				// Focus on control window
				SetFocus( hWndControl );

				// Invalidate entire main window
				InvalidateRect( hWndMain, NULL, TRUE );

				// Update main window
				UpdateWindow( hWndMain );

				// Update return value_comp
				bResult = TRUE;

			} // End of this is the selected tab
			else
			{
				// This is not the selected tab

				// Hide control window
				ShowWindow( hWndControl, SW_HIDE );

			} // End of this is not the selected tab

		} // End of successfully got control window

	}; // End of loop through all tabs

	return bResult;

} // End of function TabControlWindowShowControlWindow
