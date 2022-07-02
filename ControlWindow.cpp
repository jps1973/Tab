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

	} // End of successfully created control window

	return hWndControl;

} // End of function ControlWindowCreate

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

		// Update return value
		bResult = TRUE;

	} // End of successfully moved control window

	return bResult;
	
} // End of function ControlWindowMove
