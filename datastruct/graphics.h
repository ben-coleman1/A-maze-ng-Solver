#define _GRAPHICS_H_
#include <gtk/gtk.h>
// Filename: graphics.h
// Description: General macros and definitions for the graphics rendering program
// This includes DEFINES constants and common macros
//

#define UPDATE_INTERVAL 100  // how often graphics checks for map
#define WINDOW_WIDTH 600  	// Width of the graphics window
#define WINDOW_HEIGHT 600  	// Height of the graphics window

// Converts from walls extended coordinates (internal structure coords)
// to regular coordinates
#define convertBack(n)  (n+1)/2

// Prototypes
// Set the timer tic
gboolean timer_tic(gpointer data);

// Render the maze, which sets the timer as well
void render_maze();

#ifndef GDKTEST
// Sets the global data2 variable, from which the graphics update every timer tic
void set_maze(int  **walls, int width, int height);
#endif
