// ControlWindow.cpp

#include "ControlWindow.h"

HWND ControlWindowCreate( HWND hWndParent, int nID )
{
	HWND hWndControl;

	HINSTANCE hInstance;

	// Get instance handle
	hInstance = GetModuleHandle( NULL );

	// Create control window
	hWndControl = CreateWindowEx( CONTROL_WINDOW_EXTENDED_STYLE, CONTROL_WINDOW_CLASS_NAME, CONTROL_WINDOW_TEXT, CONTROL_WINDOW_STYLE, 0, 50, 100, 25, hWndParent, ( HMENU )( INT_PTR )nID, hInstance, NULL );

	// Ensure that control window was created
	if( hWndControl )
	{
		// Successfully created control window
		int nWhichColumn;
		LPCTSTR lpszColumnTities [] = CONTROL_WINDOW_COLUMN_TITLES;
		LVCOLUMN lvColumn;
		static HIMAGELIST hImageList = NULL;

		// Ensure that image list is valid
		if( hImageList == NULL )
		{
			// Image list is not valid
			SHFILEINFO shfi;

			// Get system image list
			hImageList = ( HIMAGELIST )SHGetFileInfo( NULL, 0, &shfi, sizeof( SHFILEINFO ), ( SHGFI_SYSICONINDEX | SHGFI_SMALLICON ) );

		} // End of image list is not valid

		// Assign image list to window
		SendMessage( hWndControl, LVM_SETIMAGELIST, ( WPARAM )LVSIL_NORMAL,	( LPARAM )hImageList );
		SendMessage( hWndControl, LVM_SETIMAGELIST, ( WPARAM )LVSIL_SMALL,	( LPARAM )hImageList );

		// Set exended list view window style
		SendMessage( hWndControl, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )NULL, ( LPARAM )CONTROL_WINDOW_EXTENDED_STYLE );

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask	= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx		= CONTROL_WINDOW_DEFAULT_COLUMN_WIDTH;

		// Add columns to window
		for( nWhichColumn = 0; nWhichColumn < NUMBER_OF_COLUMNS; nWhichColumn ++ )
		{
			// Update list view column structure
			lvColumn.pszText = ( LPTSTR )lpszColumnTities[ nWhichColumn ];

			// Insert column
			SendMessage( hWndControl, LVM_INSERTCOLUMN, ( WPARAM )nWhichColumn, ( LPARAM )&lvColumn );

		}; // End of loop to add columns to window

	} // End of successfully created control window

	return hWndControl;

} // End of function ControlWindowCreate

BOOL ControlWindowGetItemPath( HWND hWndControl, int nWhichItem, LPCTSTR lpszParentFolderPath, LPTSTR lpszItemPath )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Copy parent folder path into item path
	lstrcpy( lpszItemPath, lpszParentFolderPath );

	// Ensure that item path ends with a back-slash
	if( lpszItemPath[ lstrlen( lpszItemPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Item path does not end with a back-slash

		// Append a back-slash onto item path
		lstrcat( lpszItemPath, ASCII_BACK_SLASH_STRING );

	} // End of item path does not end with a back-slash

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= NAME_COLUMN_ID;
	lvItem.pszText		= ( lpszItemPath + lstrlen( lpszItemPath ) );
	lvItem.cchTextMax	= STRING_LENGTH;

	// Get item text
	if( SendMessage( hWndControl, LVM_GETITEMTEXT, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem ) )
	{
		// Successfully got item text

		// Update return value
		bResult = TRUE;

	} // End of successfully got item text

	return bResult;

} // End of function ControlWindowMove

BOOL ControlWindowMove( HWND hWndParent, int nID, int nX, int nY, int nWidth, int nHeight )
{
	BOOL bResult = FALSE;

	HWND hWndControl;

	// Get control window
	hWndControl = GetDlgItem( hWndParent, nID );

	// Move control window
	if( MoveWindow( hWndControl, nX, nY, nWidth, nHeight, TRUE ) )
	{
		// Successfully moved control window

		// Size column to fit window
		SendMessage( hWndControl, LVM_SETCOLUMNWIDTH, ( WPARAM )NAME_COLUMN_ID, ( LPARAM )nWidth );

		// Update return value
		bResult = TRUE;

	} // End of successfully moved control window

	return bResult;

} // End of function ControlWindowMove

int ControlWindowPopulate( HWND hWndControl, LPCTSTR lpszFolderPath, LPCTSTR lpszFileFilter )
{
	int nResult = 0;

	WIN32_FIND_DATA wfd;
	HANDLE hFind;

	// Allocate string memory
	LPTSTR lpszParentFolderPath		= new char[ STRING_LENGTH ];
	LPTSTR lpszFullSearchPattern	= new char[ STRING_LENGTH ];
	LPTSTR lpszFoundItemPath		= new char[ STRING_LENGTH ];

	// Store parent folder path
	lstrcpy( lpszParentFolderPath, lpszFolderPath );

	// Ensure that parent folder path ends with a back slash character
	if( lpszParentFolderPath[ lstrlen( lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Parent folder path does not end with a back slash character

		// Append a back slash character onto parent folder path
		lstrcat( lpszParentFolderPath, ASCII_BACK_SLASH_STRING );

	} // End of parent folder path does not end with a back slash character

	// Copy parent folder path into full search pattern
	lstrcpy( lpszFullSearchPattern, lpszParentFolderPath );

	// Append file filter onto full search pattern
	lstrcat( lpszFullSearchPattern, lpszFileFilter );

	// Find first item
	hFind = FindFirstFile( lpszFullSearchPattern, &wfd );

	// Ensure that first item was found
	if( hFind != INVALID_HANDLE_VALUE )
	{
		// Successfully found first item
		LVITEM lvItem;
		SHFILEINFO shfi;
		int nItem;

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask	= ( LVIF_TEXT | LVIF_IMAGE );
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.iItem		= 0;
		lvItem.iSubItem		= NAME_COLUMN_ID;

		// Loop through all items
		do
		{
			// See if found item is a file
			if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				// Found item is a file

				// Copy parent folder path into found item path
				lstrcpy( lpszFoundItemPath, lpszParentFolderPath );

				// Append found item name onto found item path
				lstrcat( lpszFoundItemPath, wfd.cFileName );

				// Get file information for item
				SHGetFileInfo( ( LPTSTR )lpszFoundItemPath, 0, &shfi, sizeof( SHFILEINFO ), ( SHGFI_SYSICONINDEX | SHGFI_SMALLICON ) );

				// Update list view item structure for item
				lvItem.pszText 	= wfd.cFileName;
				lvItem.iImage	= shfi.iIcon;

				// Insert item
				nItem = SendMessage( hWndControl, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

				// Ensure that item was inserted
				if( nItem >= 0 )
				{
					// Successfully inserted item

					// Update list view item structure for next item
					lvItem.iItem ++;

					// Update return value
					nResult ++;

				} // End of successfully inserted item
				else
				{
					// Unable to insert item

					// Force exit from loop
					break;

				} // End of unable to insert item

			} // End of found item is a file

		} while( FindNextFile( hFind, &wfd ) != 0 ); // End of loop through all items

		// Close file find
		FindClose( hFind );

	} // End of successfully found first item

	// Free string memory
	delete lpszParentFolderPath;
	delete lpszFullSearchPattern;
	delete lpszFoundItemPath;

	return nResult;

} // End of function ControlWindowPopulate