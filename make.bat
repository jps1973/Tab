cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra Tab.cpp BrowseForFolder.cpp ControlWindow.cpp Resource.o -o Tab.exe -lcomctl32
