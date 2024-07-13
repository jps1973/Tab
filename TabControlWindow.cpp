// TabControlWindow.cpp

#include "TabControlWindow.h"

// Global variables
HWND g_hWndTabControl;
HINSTANCE g_hInstance;
int g_nNumberOfTabs;
int g_nNextTabNumber;


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

			// Initialise global variables
			g_nNumberOfTabs		= 0;
			g_nNextTabNumber	= 1;

			// Update return value (assume success)
			bResult = TRUE;

		} // End of successfully initilised common controls

	} // End of successfully created tab control window

	return bResult;

} // End of function TabControlWindowCreate

BOOL TabControlWindowGetRect( LPRECT lpRect )
{
	// Get tab control window rect
	return ::GetWindowRect( g_hWndTabControl, lpRect );

} // End of function TabControlWindowGetRect

BOOL TabControlWindowHandleNotifyMessage( WPARAM, LPARAM lParam )
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
				TCITEM tcItem;

				// Allocate string memory
				LPTSTR lpszTitle = new char[ STRING_LENGTH ];

				// Clear tab control item structure
				ZeroMemory( &tcItem, sizeof( tcItem ) );

				// Initialise tab control item structure
				tcItem.mask			= TCIF_TEXT;
				tcItem.pszText		= lpszTitle;
				tcItem.cchTextMax	= STRING_LENGTH;

				// Get tab control item
				if( SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nSelectedTab, ( LPARAM )&tcItem ) )
				{
					// Successfully got tab control item

					MessageBox( 0, lpszTitle, "", MB_OK );

				} // End of successfully got tab control item

				// Free string memory
				delete [] lpszTitle;

			} // End of successfully got selected tab

/*
			int iPage = TabCtrl_GetCurSel(hwndTab); 

			// Note that g_hInst is the global instance handle.
			LoadString(g_hInst, IDS_SUNDAY + iPage, achTemp,
			sizeof(achTemp) / sizeof(achTemp[0])); 
			LRESULT result = SendMessage(hwndDisplay, WM_SETTEXT, 0,
			(LPARAM) achTemp); 
*/
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

int TabControlWindowLoad( LPCTSTR lpszFileName )
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
					if( TabControlWindowNewTab( lpszTab ) >= 0 )
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

int TabControlWindowNewTab( LPCTSTR lpszTitle )
{
	int nResult = -1;

	TCITEM tabControlItem;

	// Clear tab control item structure
	ZeroMemory( &tabControlItem, sizeof( tabControlItem ) );

	// Initialise tab control item structure
	tabControlItem.mask		= TCIF_TEXT;
	tabControlItem.pszText	= ( LPTSTR )lpszTitle;

	// Insert tab control item
	nResult = SendMessage( g_hWndTabControl, TCM_INSERTITEM, ( WPARAM )g_nNumberOfTabs, ( LPARAM )&tabControlItem );

	// Ensure that tab control item was inserted
	if( nResult >= 0 )
	{
		// Successfully inserted tab control item

		// Update global variables
		g_nNextTabNumber ++;
		g_nNumberOfTabs ++;

	} // End of successfully inserted tab control item

	return nResult;

} // End of function TabControlWindowNewTab

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

BOOL TabControlWindowSize( LPARAM lParam )
{
	BOOL bResult = FALSE;

	int nClientWidth;
	int nClientHeight;

	// Store client size
	nClientWidth	= ( int )LOWORD( lParam );
	nClientHeight	= ( int )HIWORD( lParam );


	// Set tab control window position
	if( SetWindowPos( g_hWndTabControl, HWND_TOP, 0, 0, nClientWidth, nClientHeight, SWP_SHOWWINDOW ) )
	{
		// Successfully set tab control window position

		// Update return value
		bResult = TRUE;

	} // End of successfully set tab control window position

	return bResult;

} // End of function TabControlWindowSize