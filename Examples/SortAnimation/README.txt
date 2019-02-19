Keyboard commands:
   < or ,    Speed up the sorting speed
   > or .    Slow down the sorting speed
   Space     Pause (or unpause) the sorting
   Esc       Terminate the sort
   Q or q    Quit the program

This program has been tested on Windows and
minimally on Linux.  It uses threads, so for Linux 
add the statement

    XInitThreads();

as the first line in the main function, and
add the header

    #include <X11/Xlib.h>

at the top of the source file (and set up 
include and linker flags apppropriately).
