// ControlWindow.cpp

#include "ControlWindow.h"

HWND ControlWindowCreate( HWND hWndParent, HINSTANCE hInstance, int nID, LPCTSTR lpszParentFolderPath )
{
	HWND hWndControl;

	// Create control window
	hWndControl = ::CreateWindowEx( CONTROL_WINDOW_EXTENDED_STYLE, CONTROL_WINDOW_CLASS_NAME, CONTROL_WINDOW_TEXT, CONTROL_WINDOW_STYLE, 100, 100, 100, 100, hWndParent, ( HMENU )INT_PTR( nID ), hInstance, NULL );

	// Ensure that control window was created
	if( hWndControl )
	{
		// Successfully created control window

		// Set control window font
		ControlWindowSetFont( hWndControl, DEFAULT_GUI_FONT );

		// Populate control window
		ControlWindowPopulate( hWndControl,  lpszParentFolderPath );

	} // End of successfully created control window

	return hWndControl;

} // End of function ControlWindowCreate

BOOL ControlWindowGetFilePath( HWND hWndControl, int nWhichFile, LPCTSTR lpszParentFolderPath, LPTSTR lpszFilePath )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszFileName = new char[ STRING_LENGTH ];

	// Get file name
	if( SendMessage( hWndControl, LB_GETTEXT, ( WPARAM )nWhichFile, ( LPARAM )lpszFileName ) != LB_ERR )
	{
		// Successfully got file name

		// Copy parent folder path into file path
		lstrcpy( lpszFilePath, lpszParentFolderPath );

		// Ensure that file path ends with a back-slash
		if( lpszFilePath[ lstrlen( lpszFilePath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
		{
			// File path does not end with a back-slash

			// Append s back-slash onto file path
			lstrcat( lpszFilePath, ASCII_BACK_SLASH_STRING );

		} // End of file path does not end with a back-slash

		// Append file name onto file path
		lstrcat( lpszFilePath, lpszFileName );

		// Update return value
		bResult = TRUE;

	} // End of successfully got file name

	// Free string memory
	delete [] lpszFileName;

	return bResult;

} // End of function ControlWindowGetFilePath

BOOL ControlWindowGetFilePath( HWND hWndControl, LPCTSTR lpszParentFolderPath, LPTSTR lpszFilePath )
{
	BOOL bResult = FALSE;

	int nSelected;

	// Get selected item number
	nSelected = SendMessage( hWndControl, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Ensure that selected item number was got
	if( nSelected >= 0 )
	{
		// Successfully got selected item number

		// Get selected item path
		bResult = ControlWindowGetFilePath( hWndControl, nSelected, lpszParentFolderPath, lpszFilePath );

	} // End of successfully got selected item number

	return bResult;

} // End of function ControlWindowGetFilePath

BOOL ControlWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, LPCTSTR lpszParentFolderPath, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusText ) )
{
	BOOL bResult = FALSE;

	// Select command
	switch( HIWORD( wParam ) )
	{
		case LBN_SELCHANGE:
		{
			// A list box selection change command
			HWND hWndControl;

			// Get control window
			hWndControl = ( HWND )lParam;

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH ];

			// Get file path
			if( ControlWindowGetFilePath( hWndControl, lpszParentFolderPath, lpszFilePath ) )
			{
				// Successfully got file path

				// Display file path as status message
				( lpStatusFunction )( lpszFilePath );

				// Update return value
				bResult = TRUE;

			} // End of successfully got file path

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a list box selection change command
		default:
		{
			// Default command

			// We dont need to do anything here, just exit with the default value

			// Break out of switch
			break;

		} // End of default command

	}; // End of selection for command

	return bResult;

} // End of function ControlWindowHandleCommandMessage

BOOL ControlWindowMove( HWND hWndControl, HWND hWndTabControl )
{
	BOOL bResult = FALSE;

	RECT rcControlWindow;
	int nControlWindowWidth;
	int nControlWindowHeight;

	// Get tab window size
	GetClientRect( hWndTabControl, &rcControlWindow );

	// Get (available) control window rect
	SendMessage( hWndTabControl, TCM_ADJUSTRECT, ( WPARAM )FALSE, ( LPARAM )&rcControlWindow );

	// Calculate control window size
	nControlWindowWidth		= ( rcControlWindow.right - rcControlWindow.left );
	nControlWindowHeight	= ( rcControlWindow.bottom - rcControlWindow.top );

	// Move control window
	if( MoveWindow( hWndControl, rcControlWindow.left, rcControlWindow.top, nControlWindowWidth, nControlWindowHeight, TRUE ) )
	{
		// Successfully moved control window

		// Update return valu
		bResult = TRUE;

	} // End of successfully moved control window

	return bResult;

} // End of function ControlWindowMove

int ControlWindowPopulate( HWND hWndControl, LPCTSTR lpszParentFolderPath )
{
	int nResult = 0;

	// Allocate string memory
	LPTSTR lpszFullSearchPattern = new char[ STRING_LENGTH ];

	// Copy parent folder path into full search pattern
	lstrcpy( lpszFullSearchPattern, lpszParentFolderPath );

	// Append all files filter onto full search pattern
	lstrcat( lpszFullSearchPattern, ALL_FILES_FILTER );

	WIN32_FIND_DATA  wfd;
	HANDLE hFind;

	// Find first item
	hFind = FindFirstFile( lpszFullSearchPattern, &wfd );

	// Ensure that first item was found
	if( hFind != INVALID_HANDLE_VALUE )
	{
		// Successfully found first item

		// Loop through all items
		do
		{
			// See if found item is a file
			if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				// Found item is a file

				// Add found item to control window
				SendMessage( hWndControl, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )wfd.cFileName );

				// Update return value
				nResult ++;

			} // End of found item is a file

		} while( FindNextFile( hFind, &wfd ) != 0 );

		// Close file find
		FindClose( hFind );

	} // End of successfully found first item

	// Free string memory
	delete [] lpszFullSearchPattern;

	return nResult;

} // End of function ControlWindowPopulate

void ControlWindowSetFont( HWND hWndControl, int nWhichFont )
{
	HFONT hFont;

	// Get font
	hFont = ( HFONT )GetStockObject( nWhichFont );

	// Set control window font
	::SendMessage( hWndControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ControlWindowSetFont
