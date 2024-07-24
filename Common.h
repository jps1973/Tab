// Common.h

#pragma once

#include <windows.h>

#define STRING_LENGTH															2048

#define ALL_FILES_FILTER														"*.*"
#define NEW_LINE_TEXT															"\r\n"

#define WINDOW_BORDER_WIDTH														::GetSystemMetrics( SM_CXBORDER )
#define WINDOW_BORDER_HEIGHT													::GetSystemMetrics( SM_CYBORDER )

#define SHELL_EXECUTE_OPEN_COMMAND												"open"
#define SHELL_EXECUTE_MAXIMUM_ERROR_VALUE										32
#define SHELL_EXECUTE_UNABLE_TO_OPEN_ERROR_MESSAGE_FORMAT_STRING				"Unable to open %s"

#define ABOUT_MESSAGE_CAPTION													"About"
#define ERROR_MESSAGE_CAPTION													"Error"
#define INFORMATION_MESSAGE_CAPTION												"Information"
#define WARNING_MESSAGE_CAPTION													"Warning"
