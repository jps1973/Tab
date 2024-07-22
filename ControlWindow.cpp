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

BOOL ControlWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, LPCTSTR lpszParentFolderPath, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusText ) )
{
	BOOL bResult = FALSE;

	return bResult;

} // End of function 

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
