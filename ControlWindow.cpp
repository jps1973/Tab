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

		// Set control window font
		ControlWindowSetFont( hWndControl, DEFAULT_GUI_FONT );

		// Add item to control window
		SendMessage( hWndControl, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszData );

	} // End of successfully created control window

	return hWndControl;

} // End of function ControlWindowCreate

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

void ControlWindowSetFont( HWND hWndControl, int nWhichFont )
{
	HFONT hFont;

	// Get font
	hFont = ( HFONT )GetStockObject( nWhichFont );

	// Set control window font
	::SendMessage( hWndControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ControlWindowSetFont
