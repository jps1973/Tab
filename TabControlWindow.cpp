// TabControlWindow.cpp

#include "TabControlWindow.h"

// Global variables
static HWND g_hWndTabControl;
static HINSTANCE g_hInstance;
static int g_nNumberOfTabs;
static int g_nNextControlWindowID;

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
			g_nNumberOfTabs			= 0;
			g_nNextControlWindowID	= TAB_CONTROL_WINDOW_FIRST_CONTROL_WINDOW_ID;

			// Update return value (assume success)
			bResult = TRUE;

		} // End of successfully initilised common controls

	} // End of successfully created tab control window

	return bResult;

} // End of function TabControlWindowCreate

HWND TabControlWindowGetControlWindow()
{
	HWND hWndResult = NULL;

	int nSelectedTab;

	// Get selected tab
	nSelectedTab = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Ensure that selected tab was got
	if( nSelectedTab >= 0 )
	{
		// Successfully got selected tab

		// Get control window
		hWndResult = TabControlWindowGetControlWindow( nSelectedTab );

	} // End of successfully got selected tab

	return hWndResult;

} // End of function TabControlWindowGetControlWindow

HWND TabControlWindowGetControlWindow( int nWhichTab )
{
	HWND hWndResult = NULL;

	TabData tabData;

	// Clear tab data structure
	::ZeroMemory( &tabData, sizeof( tabData ) );

	// Initialise tab data structure
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

BOOL TabControlWindowGetRect( LPRECT lpRect )
{
	// Get tab control window rect
	return ::GetWindowRect( g_hWndTabControl, lpRect );

} // End of function TabControlWindowGetRect

BOOL TabControlWindowHandleControlCommandMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusText ) )
{
	BOOL bResult = FALSE;

	int nSelectedTab;

	// Get selected tab
	nSelectedTab = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Ensure that selected tab was got
	if( nSelectedTab >= 0 )
	{
		// Successfully got selected tab
		TabData tabData;

		// Clear tab data structure
		::ZeroMemory( &tabData, sizeof( tabData ) );

		// Initialise tab data structure
		tabData.tcItemHeader.mask = TCIF_PARAM;

		// Get tab data
		if( ::SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nSelectedTab, ( LPARAM )&tabData ) )
		{
			// Successfully got tab data

			// Handle command message from control window
			bResult = ControlWindowHandleCommandMessage( wParam, lParam, tabData.lpszParentFolderPath, lpStatusFunction );

		} // End of successfully got tab data

	} // End of successfully got selected tab

	return bResult;

} // End of function TabControlWindowHandleControlCommandMessage

BOOL TabControlWindowHandleNotifyMessage( WPARAM, LPARAM lParam, HWND hWndMain, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusText ) )
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
				if( TabControlWindowSelectTab( hWndMain, nSelectedTab ) )
				{
					// Successfully selected tab
					TabData tabData;

					// Clear tab data structure
					::ZeroMemory( &tabData, sizeof( tabData ) );

					// Initialise tab data structure
					tabData.tcItemHeader.mask = TCIF_PARAM;

					// Get tab data
					if( ::SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nSelectedTab, ( LPARAM )&tabData ) )
					{
						// Successfully got tab data

						// Show tab path on status bar
						( lpStatusFunction )( tabData.lpszParentFolderPath );

					} // End of successfully got tab data

				} // End of successfully selected tab

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

BOOL TabControlWindowIsControlWindow( int nID )
{
	BOOL bResult = FALSE;

	// See if supplied id refers t oa control window
	if( ( nID >= TAB_CONTROL_WINDOW_FIRST_CONTROL_WINDOW_ID ) && ( nID < g_nNextControlWindowID ) )
	{
		// Supplied id refers to a control window

		// Update return value
		bResult = TRUE;

	} // End of supplied id refers to a control window

	return bResult;

} // End of function TabControlWindowIsControlWindow

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

int TabControlWindowNewTab( HWND hWndMain )
{
	int nResult = -1;

	int nSelectedTab;

	// Get selected tab
	nSelectedTab = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Ensure that selected tab was got
	if( nSelectedTab >= 0 )
	{
		// Successfully got selected tab
		TabData tabData;

		// Clear tab data structure
		::ZeroMemory( &tabData, sizeof( tabData ) );

		// Initialise tab data structure
		tabData.tcItemHeader.mask = TCIF_PARAM;

		// Get tab data
		if( ::SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nSelectedTab, ( LPARAM )&tabData ) )
		{
			// Successfully got tab data

			// Allocate string memory
			LPTSTR lpszParentFolderPath = new char[ STRING_LENGTH ];

			// Store (current) parent folder path
			lstrcpy( lpszParentFolderPath, tabData.lpszParentFolderPath );

			// Select parent folder
			if( SelectFolder( TAB_CONTROL_WINDOW_NEW_TAB_SELECT_FOLDER_TITLE, lpszParentFolderPath ) )
			{
				// Successfully selected parent folder

				// Create new tab
				nResult = TabControlWindowNewTab( hWndMain, lpszParentFolderPath );

				// Ensure that new tab was created
				if( nResult >= 0 )
				{
					// Successfully created new tab

					// Select new tab
					TabControlWindowSelectTab( hWndMain, nResult );

				} // End of successfully created new tab

			} // End of successfully selected parent folder

			// Free string memory
			delete [] lpszParentFolderPath;

		} // End of successfully got tab data

	} // End of successfully got selected tab

	return nResult;

} // End of function TabControlWindowNewTab

int TabControlWindowNewTab( HWND hWndMain, LPCTSTR lpszParentFolderPath )
{
	int nResult = -1;

	TabData tabData;
	LPTSTR lpszLastBackslash;

	// Allocate string memory
	LPTSTR lpszTitle				= new char[ STRING_LENGTH ];
	LPTSTR lpszParentFolderPathCopy	= new char[ STRING_LENGTH ];

	// Copy parent folder path to a non-constant string
	lstrcpy( lpszParentFolderPathCopy, lpszParentFolderPath );

	// Remove back-slash characters from end of parent folder path copy
	while( lpszParentFolderPathCopy[ lstrlen( lpszParentFolderPathCopy ) - sizeof( char ) ] == ASCII_BACK_SLASH_CHARACTER )
	{
		// Remove back-slash character from end of parent folder path copy
		lpszParentFolderPathCopy[ lstrlen( lpszParentFolderPathCopy ) - sizeof( char ) ] = ( char )NULL;

	}; // End of loop to remove back-slash characters from end of parent folder path copy

	// Find last back-slash in parent folder path copy
	lpszLastBackslash = strrchr( lpszParentFolderPathCopy, ASCII_BACK_SLASH_CHARACTER );

	// See if last back-slash was found in parent folder path
	if( lpszLastBackslash )
	{
		// Successfully found last back-slash in parent folder path

		// See if there is text after the last back-slash
		if( lpszLastBackslash[ sizeof( char ) ] )
		{
			// There is some text after the last back-slash

			// Use text after the last back-slash for title
			lstrcpy( lpszTitle, ( lpszLastBackslash + sizeof( char ) ) );

		} // End of there is some text after the last back-slash
		else
		{
			// There is no text after the last back-slash

			// Use entire parent folder path for title
			lstrcpy( lpszTitle, lpszParentFolderPath );

		} // End of there is no text after the last back-slash

	} // End of successfully found last back-slash in parent folder path
	else
	{
		// Unable to find last back-slash in parent folder path

		// Use entire parent folder path for title
		lstrcpy( lpszTitle, lpszParentFolderPath );

	} // End of unable to find last back-slash in parent folder path

	// Clear tab data structure
	ZeroMemory( &tabData, sizeof( tabData ) );

	// Initialise tab control item structure
	tabData.tcItemHeader.mask		= ( TCIF_TEXT | TCIF_PARAM );
	tabData.tcItemHeader.cchTextMax	= STRING_LENGTH;
	tabData.tcItemHeader.pszText	= ( LPTSTR )lpszTitle;

	// Allocate string memory
	tabData.lpszParentFolderPath = new char[ STRING_LENGTH ];

	// Update tab data parent folder path
	lstrcpy( tabData.lpszParentFolderPath, lpszParentFolderPath );

	// Ensure that tab data parent folder path ends with a back-slash character
	if( tabData.lpszParentFolderPath[ lstrlen( tabData.lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Tab data parent folder path does not end with a back-slash character

		// Append a back-slash onto tab data parent folder path
		lstrcat( tabData.lpszParentFolderPath, ASCII_BACK_SLASH_STRING );

	} // End of tab data parent folder path does not end with a back-slash character

	// Create control window
	tabData.hWndControl = ControlWindowCreate( hWndMain, g_hInstance, g_nNextControlWindowID, tabData.lpszParentFolderPath );

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
			g_nNextControlWindowID ++;

		} // End of successfully inserted tab control item

	} // End of successfully created control window

	// Free string memory
	delete [] lpszTitle;
	delete [] lpszParentFolderPathCopy;

	return nResult;

} // End of function TabControlWindowNewTab

int TabControlWindowSave( LPCTSTR lpszFileName )
{
	int nResult = 0;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Eesure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		int nTabCount;
		int nWhichTab;
		TabData tabData;

		// Clear tab data structure
		::ZeroMemory( &tabData, sizeof( tabData ) );

		// Initialise tab data structure
		tabData.tcItemHeader.mask		= TCIF_PARAM;


		// Count tabs
		nTabCount = ::SendMessage( g_hWndTabControl, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through all tabs
		for( nWhichTab = 0; nWhichTab < nTabCount; nWhichTab ++ )
		{
			// Get tab data
			if( ::SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nWhichTab, ( LPARAM )&tabData ) )
			{
				// Successfully got tab data

				// Write parent folder path to file
				if( WriteFile( hFile, tabData.lpszParentFolderPath, lstrlen( tabData.lpszParentFolderPath ), NULL, NULL ) )
				{
					// Successfully wrote parent folder path to file

					// Write new line text to file
					WriteFile( hFile, NEW_LINE_TEXT, lstrlen( NEW_LINE_TEXT ), NULL, NULL );

					// Update return value
					nResult ++;

				} // End of successfully wrote parent folder path to file

			} // End of successfully got tab data

		} // End of loop through all tabs

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return nResult;

} // End of function TabControlWindowSave

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
