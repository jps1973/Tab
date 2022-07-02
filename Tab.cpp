// https://bytes.com/topic/c/answers/678749-tab-control-win32-api

#include "Tab.h"

HWND g_hWndTab;
HWND g_hWndStatusBar;
int g_nNextControlWindowID;

int ShowAboutMessage( HWND hWnd )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWnd;
	mbp.hInstance	= GetModuleHandle( NULL );
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= MAKEINTRESOURCE( IDI_APPLICATION_ICON );

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

int GetControlWindowID( int nWhichTab )
{
	int nResult = 0;

	TCITEM tcItem;

	// Initialise tab control item structure
	tcItem.mask = TCIF_PARAM;

	// Get tab control item
	if( SendMessage( g_hWndTab, TCM_GETITEM, ( WPARAM )nWhichTab, ( LPARAM )&tcItem ) )
	{
		// Successfully got tab control item

		// Update return value_comp
		nResult = tcItem.lParam;

	} // End of successfully got tab control item

	return nResult;

} // End of function GetControlWindowID

void ResizeActiveControlWindow( HWND hWnd )
{
	RECT rcTab;
	int nSelected;
	int nControlWindowWidth;
	int nControlWindowHeight;
	int nControlWindowID;

	// Get tab window size
	GetClientRect( g_hWndTab, &rcTab );

	// Adjust tab window size
	SendMessage( g_hWndTab, TCM_ADJUSTRECT, ( WPARAM )FALSE, ( LPARAM )&rcTab );

	// Get selected tab
	nSelected = SendMessage( g_hWndTab, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Get control window id
	nControlWindowID = GetControlWindowID( nSelected );

	// Calculate control window size
	nControlWindowWidth		= ( rcTab.right - rcTab.left );
	nControlWindowHeight	= ( rcTab.bottom - rcTab.top );

	// Move control window
	ControlWindowMove( hWnd, nControlWindowID, rcTab.left, rcTab.top, nControlWindowWidth, nControlWindowHeight );

} // End of function ResizeActiveControlWindow

void HideControlWindows( HWND hWnd )
{
	int nTabCount;
	int nWhichTab;
	HWND hWndControl;
	int nControlWindowID;

	// Count tabs
	nTabCount = SendMessage( g_hWndTab, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Loop through all tabs
	for( nWhichTab = 0; nWhichTab < nTabCount; nWhichTab ++ )
	{
		// Get control window id
		nControlWindowID = GetControlWindowID( nWhichTab );

		// Get control window
		hWndControl = GetDlgItem( hWnd, nControlWindowID );

		// Ensure that control window was got
		if( hWndControl )
		{
			// Successfully got control window

			// Hide control window
			ShowWindow( hWndControl, SW_HIDE );

		} // End of successfully got control window

	}; // End of loop through all tabs

} // End of function HideControlWindows

void ShowControlWindow( HWND hWnd, int nWhichTab )
{
	HWND hWndControl;
	int nControlWindowID;

	// Get control window id
	nControlWindowID = GetControlWindowID( nWhichTab );

	// Get control window
	hWndControl = GetDlgItem( hWnd, nControlWindowID );

	// Ensure that control window was got
	if( hWndControl )
	{
		// Successfully got control window

		// Invalidate entire window
		InvalidateRect( hWnd, NULL, TRUE );

		// Show control window
		ShowWindow( hWndControl, SW_SHOW );

		// Resize active control window
		ResizeActiveControlWindow( hWnd );

		// Focus on control window
		SetFocus( hWndControl );

	} // End of successfully got control window

} // End of function ShowControlWindow

void OnTabSelectionChange( HWND hWnd )
{
	int nSelected;

	// Get selected tab
	nSelected = SendMessage( g_hWndTab, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Hide control windows
	HideControlWindows( hWnd );

	// Show selected control window
	ShowControlWindow( hWnd, nSelected );

} // End of function OnTabSelectionChange

int CreateTab( HWND hWnd, LPCTSTR lpszFolderPath )
{
	int nResult = -1;

	TCITEM tcItem;
	int nTabCount;

	// Clear tab control item structure
	ZeroMemory( &tcItem, sizeof( tcItem ) );

	// Initialise tab control item structure
	tcItem.mask			= ( TCIF_TEXT | TCIF_PARAM );
	tcItem.cchTextMax	= STRING_LENGTH;
	tcItem.pszText		= ( LPTSTR )lpszFolderPath;
	tcItem.lParam		= g_nNextControlWindowID;

	// Count tabs
	nTabCount = SendMessage( g_hWndTab, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Insert tab
	nResult = SendMessage( g_hWndTab, TCM_INSERTITEM, ( WPARAM )nTabCount, ( LPARAM )&tcItem );

	// Ensure that tab was inserted
	if( nResult >= 0 )
	{
		// Successfully inserted tab
		HWND hWndControl;

		// Create control window
		hWndControl = ControlWindowCreate( hWnd, g_nNextControlWindowID );

		// Select tab
		SendMessage( g_hWndTab, TCM_SETCURSEL, ( WPARAM )nResult, ( LPARAM )NULL );

		// Call the tab selection change function
		OnTabSelectionChange( hWnd );

		// Populate control window
		ControlWindowPopulate( hWndControl, lpszFolderPath, ALL_FILES_FILTER );

		// Update next control window id
		g_nNextControlWindowID ++;

	} // End of successfully inserted tab

	return nResult;

} // End of function CreateTab

BOOL CloseTab( HWND hWnd, int nWhichTab )
{
	BOOL bResult = FALSE;

	TCITEM tcItem;

	// Allocate string memory
	LPTSTR lpszTab = new char[ STRING_LENGTH ];

	// Clear tab control item structure
	ZeroMemory( &tcItem, sizeof( tcItem ) );

	// Initialise tab control item structure
	tcItem.mask			= TCIF_TEXT;
	tcItem.cchTextMax	= STRING_LENGTH;
	tcItem.pszText		= lpszTab;

	// Get tab control item
	if( SendMessage( g_hWndTab, TCM_GETITEM, ( WPARAM )nWhichTab, ( LPARAM )&tcItem ) )
	{
		// Successfully got tab control item

		// Allocate string memory
		LPTSTR lpszWarning = new char[ STRING_LENGTH ];

		// Format warning message
		wsprintf( lpszWarning, CLOSE_TAB_WARNING_MESSAGE_FORMAT_STRING, lpszTab );

		// Ensure that user wants to close tab
		if( MessageBox( hWnd, lpszWarning, WARNING_MESSAGE_CAPTION, ( MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING ) ) == IDYES )
		{
			// User wants to close tab
			HWND hWndControl;
			int nControlWindowID;
			int nTabCount;

			// Get control window id
			nControlWindowID = GetControlWindowID( nWhichTab );

			// Get control window
			hWndControl = GetDlgItem( hWnd, nControlWindowID );

			// Close control window
			SendMessage( hWndControl, WM_CLOSE, ( WPARAM )NULL, ( LPARAM )NULL );

			// Delete tab
			SendMessage( g_hWndTab, TCM_DELETEITEM, ( WPARAM )nWhichTab, ( LPARAM )NULL );

			// Count tabs
			nTabCount = SendMessage( g_hWndTab, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

			// Ensure that there are tabs
			if( nTabCount > 0 )
			{
				// There are tabs

				// See if selected tab was the first
				if( nWhichTab == 0 )
				{
					// Selected tab was the first

					// Select first tab
					SendMessage( g_hWndTab, TCM_SETCURSEL, ( WPARAM )0, ( LPARAM )NULL );

				} // End of selected tab was the first
				else
				{
					// Selected tab was not the first

					// Select previous tab
					SendMessage( g_hWndTab, TCM_SETCURSEL, ( WPARAM )( nWhichTab - 1 ), ( LPARAM )NULL );

				} // End of selected tab was not the first

				// Call the tab selection change function
				OnTabSelectionChange( hWnd );

			} // End of there are tabs

		} // End of user wants to close tab

		// Free string memory
		delete [] lpszWarning;

	} // End of successfully got tab control item

	// Free string memory
	delete [] lpszTab;

	return bResult;

} // End of function CloseTab

BOOL CloseTab( HWND hWnd )
{
	BOOL bResult;

	int nWhichTab;

	// Get selected tab
	nWhichTab = SendMessage( g_hWndTab, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Close selected tab
	bResult = CloseTab( hWnd, nWhichTab );

	return bResult;

} // End of function CloseTab

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMsg )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;
			HFONT hFont;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Get font
			hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

			// Initialise common controls
			InitCommonControls();

			// Create tab window
			g_hWndTab = CreateWindowEx( TAB_WINDOW_EXTENDED_STYLE, TAB_WINDOW_CLASS_NAME, TAB_WINDOW_TEXT, TAB_WINDOW_STYLE, 0, 0, 300, 250, hWnd, NULL, hInstance, NULL );

			// Ensure that tab window was created
			if( g_hWndTab )
			{
				// Successfully created tab window
				HFONT hFont;

				// Get font
				hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

				// Set tab window font
				SendMessage( g_hWndTab, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

				// Create tabs
				CreateTab( hWnd, "C:\\" );
				CreateTab( hWnd, "D:\\" );

				// Show tab window
				ShowWindow( g_hWndTab, SW_SHOW );

				// Update tab window
				UpdateWindow( g_hWndTab );

				// Create status bar window
				g_hWndStatusBar = CreateWindowEx( STATUS_BAR_WINDOW_EXTENDED_STYLE, STATUSCLASSNAME, STATUS_BAR_WINDOW_TEXT, STATUS_BAR_WINDOW_STYLE, 0, 0, 0, 0, hWnd, ( HMENU )NULL, hInstance, NULL );

				// Ensure that status bar window was created
				if( g_hWndStatusBar )
				{
					// Successfully created status bar window

					// Set status bar window font
					SendMessage( g_hWndStatusBar, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

				} // End of successfully created status bar window

			} // End of successfully created tab window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatus;
			int nStatusWindowHeight;
			int nTabControlWindowHeight;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			SendMessage( g_hWndStatusBar, WM_SIZE, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get status window size
			GetWindowRect( g_hWndStatusBar, &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nTabControlWindowHeight	= ( nClientHeight - nStatusWindowHeight );

			// Move tab window
			MoveWindow( g_hWndTab, 0, 0, nClientWidth, nTabControlWindowHeight, TRUE );

			// Resize active control window
			ResizeActiveControlWindow( hWnd );

			// Break out of switch
			break;

		} // End of a size message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = MAIN_WINDOW_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = MAIN_WINDOW_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case IDM_TAB_NEW:
				{
					// A tab new command

					// Create new tab
					CreateTab( hWnd, "U:\\" );
InvalidateRect( hWnd, NULL, TRUE );
SendMessage( hWnd, WM_PAINT, ( WPARAM )NULL, ( LPARAM )NULL );
				// Show tab window
				ShowWindow( g_hWndTab, SW_SHOW );

				// Update tab window
				UpdateWindow( g_hWndTab );

					// Break out of switch
					break;

				} // End of a tab new command
				case IDM_TAB_CLOSE:
				{
					// A tab close command

					// Close tab
					CloseTab( hWnd );

					// Break out of switch
					break;

				} // End of a tab close command
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
					DestroyWindow( hWnd );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWnd );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// Call default procedure
					lr = DefWindowProc( hWnd, uMsg, wParam, lParam );

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					ShowAboutMessage( hWnd );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWnd, uMsg, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message
			LPNMHDR lpNmHdr;

			// Get notify message handler
			lpNmHdr = ( ( LPNMHDR )lParam );

			// See if notify message is from tab window
			if( lpNmHdr->hwndFrom == g_hWndTab )
			{
				// Notify message is from tab window

				// Select notify message
				switch( lpNmHdr->code )
				{
					case TCN_SELCHANGE:
					{
						// A selection change notify message

						// Call the tab selection change function
						OnTabSelectionChange( hWnd );

						// Break out of switch
						break;

					} // End of a selection change notify message
					default:
					{
						// Default notify message

						// Call default procedure
						lr = DefWindowProc( hWnd, uMsg, wParam, lParam);

						// Break out of switch
						break;

					} // End of default notify message

				}; // End of selection for notify message

			} // End of notify message is from tab window
			else
			{
				// Notify message is not from tab window

				// Call default procedure
				lr = DefWindowProc( hWnd, uMsg, wParam, lParam);

			} // End of notify message is not from tab window

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CLOSE:
		{
			// A close message

			// Close wundow
			DestroyWindow( hWnd);

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default procedure
			lr = DefWindowProc( hWnd, uMsg, wParam, lParam);

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function WndProc

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wc;

	// Init global variables
	g_nNextControlWindowID = FIRST_CONTROL_WINDOW_ID;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear window class structure
	ZeroMemory( &wc, sizeof( wc ) );

	// Initialise window class structure
	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= hInstance;
	wc.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wc.style			= MAIN_WINDOW_CLASS_STYLE;
	wc.hIcon			= MAIN_WINDOW_CLASS_ICON;
	wc.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
	wc.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
	wc.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
	wc.hIconSm			= MAIN_WINDOW_CLASS_ICON_SMALL;

	// Register window class
	if( RegisterClassEx( &wc ) )
	{
		// Successfully registered window class
		HWND hWnd;

		// Create window
		hWnd = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

		// Ensure that window was created
		if( hWnd )
		{
			// Successfully created window
			HMENU hMenuSystem;

			// Get system menu
			hMenuSystem = GetSystemMenu( hWnd, FALSE );

			// Add separator item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

			// Add about item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Show window
			ShowWindow( hWnd, nCmdShow );

			// Update window
			UpdateWindow( hWnd );

			// Message loop
			while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
			{
				// Translate message
				TranslateMessage( &msg );

				// Dispatch message
				DispatchMessage( &msg );

			}; // End of message loop

		} // End of successfully created window
		else
		{
			// Unable to create window

			// Display error message
			MessageBox( NULL, UNABLE_TO_CREATE_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to create window

	} // End of successfully registered window class
	else
	{
		// Unable to register window class

		// Display error message
		MessageBox( NULL, UNABLE_TO_REGISTER_WINDOW_CLASS_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to register window class

	return msg.wParam;

} // End of function WinMain
