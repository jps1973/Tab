// BrowseForFolder.cpp

#include "BrowseForFolder.h"

// Global variables
static LPTSTR g_lpszFolderPath;

INT CALLBACK BrowseCallbackProc( HWND hWnd, UINT uMsg, LPARAM, LPARAM )
{
	LRESULT lr = 0;

	// Select message
	switch( uMsg )
	{
		case BFFM_INITIALIZED:
		{
			// A browse for folder initialise message

			// Select folder
			SendMessage( hWnd, BFFM_SETSELECTION, ( WPARAM )TRUE, ( LPARAM )g_lpszFolderPath );

			// Break out of switch
			break;

		} // End of a browse for folder initialise message

	}; // End of selection for message

	return lr;

} // End of function BrowseCallbackProc

BOOL BrowseForFolder( HWND hWndParent, LPTSTR lpszFolderPath )
{
	BOOL bResult = FALSE;

	BROWSEINFO bi;
	LPITEMIDLIST lpItem;

	// Allocate string memory
	g_lpszFolderPath = new char[ STRING_LENGTH ];

	// Initialise global folder path
	lstrcpy( g_lpszFolderPath, lpszFolderPath );

	// Clear browse information structure
	ZeroMemory( &bi, sizeof( bi ) );

	// Initialise browse information structure
	bi.hwndOwner		= hWndParent;
	bi.pszDisplayName	= lpszFolderPath;
	bi.lpszTitle		= BROWSE_FOR_FOLDER_TITLE;
	bi.ulFlags			= ( BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON );
	bi.lpfn				= BrowseCallbackProc;

	// Browse for folder
	lpItem = SHBrowseForFolder( &bi );

	// Ensure that folder was browsed for
	if( lpItem )
	{
		// Successfully browsed for folder

		// Get folder path
		SHGetPathFromIDList( lpItem, lpszFolderPath );

		// Update return value_comp
		bResult = TRUE;

	} // End of successfully browsed for folder

	// Free string memory
	delete [] g_lpszFolderPath;

	return bResult;

} // End of function BrowseForFolder