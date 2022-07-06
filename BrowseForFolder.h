// BrowseForFolder.h

#pragma once

#include <windows.h>
#include <shlobj.h>

#include "Common.h"

#define BROWSE_FOR_FOLDER_TITLE													"Select Folder"

BOOL BrowseForFolder( HWND hWndParent, LPTSTR lpszFolderPath );
