// Tab.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Common.h"
#include "Resource.h"

#include "BrowseForFolder.h"
#include "ControlWindow.h"

#define MAIN_WINDOW_CLASS_NAME													"Main Tab Window Class"

#define MAIN_WINDOW_CLASS_STYLE													0
#define MAIN_WINDOW_CLASS_ICON													LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_APPLICATION_ICON ) )
#define MAIN_WINDOW_CLASS_CURSOR												LoadCursor( NULL, IDC_ARROW )
#define MAIN_WINDOW_CLASS_BACKGROUND											( HBRUSH )( COLOR_APPWORKSPACE + 1 )
#define MAIN_WINDOW_CLASS_MENU_NAME												MAKEINTRESOURCE( IDR_MAIN_MENU )
#define MAIN_WINDOW_CLASS_ICON_SMALL											LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_APPLICATION_ICON ) )

#define MAIN_WINDOW_EXTENDED_STYLE												( WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES )
#define MAIN_WINDOW_STYLE														WS_OVERLAPPEDWINDOW
#define MAIN_WINDOW_TEXT														"Tab"

#define MAIN_WINDOW_MINIMUM_WIDTH												320
#define MAIN_WINDOW_MINIMUM_HEIGHT												200

#define TAB_WINDOW_CLASS_NAME													"SysTabControl32"
#define TAB_WINDOW_EXTENDED_STYLE												0
#define TAB_WINDOW_STYLE														( WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE )
#define TAB_WINDOW_TEXT															NULL

#define STATUS_BAR_WINDOW_EXTENDED_STYLE										0
#define STATUS_BAR_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP )
#define STATUS_BAR_WINDOW_TEXT													NULL

#define SYSTEM_MENU_SEPARATOR_ITEM_POSITION										5
#define SYSTEM_MENU_ABOUT_ITEM_POSITION											6
#define SYSTEM_MENU_ABOUT_ITEM_TEXT												"About"

#define FIRST_CONTROL_WINDOW_ID													WM_USER

#define ABOUT_MESSAGE_TEXT														"Tab\r\n"							\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"May 2022"

#define ABOUT_MESSAGE_CAPTION													"About"
#define ERROR_MESSAGE_CAPTION													"Error"
#define INFORMATION_MESSAGE_CAPTION												"Information"
#define WARNING_MESSAGE_CAPTION													"Warning"

#define CLOSE_TAB_WARNING_MESSAGE_FORMAT_STRING									"Are you sure that you want to close tab '%s'?"

#define UNABLE_TO_REGISTER_WINDOW_CLASS_ERROR_MESSAGE							"Unable to Register Window Class"
#define UNABLE_TO_CREATE_WINDOW_ERROR_MESSAGE									"Unable to Create Window"