// ControlWindow.cpp

#include "ControlWindow.h"

HWND ControlWindowCreate( HWND hWndParent, HINSTANCE hInstance, int nID, LPCTSTR lpszData )
{
	HWND hWndControl;

	// Create control window
	hWndControl = ::CreateWindowEx( CONTROL_WINDOW_EXTENDED_STYLE, CONTROL_WINDOW_CLASS_NAME, CONTROL_WINDOW_TEXT, CONTROL_WINDOW_STYLE, 100, 100, 100, 100, hWndParent, ( HMENU )INT_PTR( nID ), hInstance, NULL );

	// Ensure that control window was created
	if( hWndControl )
	{
		// Successfully created control window

		// Add item
		SendMessage( hWndControl, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszData );

	} // End of successfully created control window

	return hWndControl;

} // End of function ControlWindowCreate
