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

int TabWindowLoad( LPCTSTR lpszFileName )
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
					if( TabWindowNewTab( lpszTab ) >= 0 )
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

} // End of function TabWindowLoad


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

BOOL TabWindowSize( LPARAM lParam )
{
	BOOL bResult = FALSE;

	int nClientWidth;
	int nClientHeight;

	// Store client size
	nClientWidth	= ( int )LOWORD( lParam );
	nClientHeight	= ( int )HIWORD( lParam );


	// Set tab window position
	if( SetWindowPos( g_hWndTab, HWND_TOP, 0, 0, nClientWidth, nClientHeight, SWP_SHOWWINDOW ) )
	{
		// Successfully set tab window position

		// Update return value
		bResult = TRUE;

	} // End of successfully set tab window position

	return bResult;

} // End of function TabWindowSize