/**
 *  @mainpage
 *  SGL
 *  The Simple Graphics Library: An object-oriented C++ toolkit
 *  thinly wrapping GLUT.
 *  @author Richard L. Halterman
 *
 *  Copyright (c) 2010, Richard L. Halterman
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with 
 *  or without modification, are permitted provided that the 
 *  following conditions are met:
 *
 *   - Redistributions of source code must retain the above 
 *     copyright notice, this list of conditions and the 
 *     following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the 
 *     above copyright notice, this list of conditions and 
 *     the following disclaimer in the documentation and/or 
 *     other materials provided with the distribution.
 *
 *   - Neither the name of the Southern Adventist University
 *     nor the names of its contributors may be used to endorse 
 *     or promote products derived from this software without 
 *     specific prior written permission.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 *     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 *     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 *     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *     ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER 
 *     OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 *     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
 *     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 *     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 *     OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 *     ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 *     OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef _SGL_H_
#define _SGL_H_

#ifdef _MSC_VER
    #pragma warning(disable:4251)
    #pragma warning(disable:4224)
#endif

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <ctime>
#include <fstream>

using std::list;
using std::vector;
using std::string;
using std::ifstream;

/**
 *  All client accessible classes, functions, and constants
 *  reside in the sgl namespace.
 */
namespace sgl
{

/**
 *  A Point object represents a location within
 *  a Window's viewport.  The (x,y) values are
 *  given in viewport coordinates.
 */
struct Point
{
	/**  The x coordinate of the point's location.  */
	double x;

	/**  The y coordinate of the point's location.  */
	double y;

	/**  Initializes the points coordinates.
	 *   @param _x the x coordinate of the new point's location.
	 *   @param _y the y coordinate of the new point's location.
	 */
	Point(double _x, double _y): x(_x), y(_y) {}

	/**  This default constructor makes a point at the origin
	 *   (0,0). 
	 */
	Point(): x(0.0), y(0.0) {}
};

/**
 *  A Color object represents an RGB color.
 *  Color objects are immutable.
 */
class Color
{
public:
	/**  The red component of the color  */
	double red;
	/**  The green component of the color  */
	double green;
	/**  The blue component of the color  */
	double blue;

	/**  Initializes a Color object.  The constructor
	 *   enforces clamped RGB values (0.0...1.0). 
     *   @param r the red component of the color
	 *   @param g the green component of the color
	 *   @param b the blue component of the color
	 */
	Color(double r, double g, double b): 
	           red(r < 0.0 ? 0.0 : (r > 1.0) ? 1.0 : r), 
			   green(g < 0.0 ? 0.0 : (g > 1.0) ? 1.0 : g), 
			   blue(b < 0.0 ? 0.0 : (b > 1.0) ? 1.0 : b) {};
};

//  These are duplicates of the GLUT constants.  This hack allows
//  clients to use the SGL library without exposing GLUT directly.
//  If the GLUT constants values are changed (unlikely), these
//  values would need to be changed to match.
//  The implementation code could have wrapped the GLUT constants, 
//  but then each client access of a SGL constant would require a 
//  conversion to the corresponding GLUT equivalent.  This hack 
//  avoids that processing overhead.

/*  Add MouseEvent and KeyEvent classes?  */

/**
 *  MouseButton is an enumerated type that maps directly to the
 *  GLUT equivalents for the corresponding mouse buttons.
 */
enum MouseButton 
{ 
    LEFT_BUTTON = 0, 
    RIGHT_BUTTON = 2, 
    UNKNOWN_BUTTON = -1 
};

/**
 *  SpecialKey is an enumerated type that maps directly to the
 *  GLUT equivalents for the corresponding special keys.
 */
enum SpecialKey 
{ 
    F1_KEY = 10001, 
    F2_KEY, 
    F3_KEY, 
    F4_KEY, 
    F5_KEY, 
    F6_KEY, 
    F7_KEY, 
    F8_KEY, 
    F9_KEY, 
    F10_KEY, 
    F11_KEY, 
    F12_KEY,
    LEFT_KEY = 10100, 
    UP_KEY, 
    RIGHT_KEY, 
    DOWN_KEY, 
    PAGE_UP_KEY, 
    PAGE_DOWN_KEY, 
    HOME_KEY, 
    END_KEY, 
    INSERT_KEY 
};

/**
 *  Cursor shapes map directly to the GLUT equivalents.
 */
enum CursorShape 
{ 
    CURSOR_RIGHT_ARROW = 0,
    CURSOR_LEFT_ARROW,
    /* Symbolic cursor shapes. */
    CURSOR_INFO,
    CURSOR_DESTROY,
    CURSOR_HELP,
    CURSOR_CYCLE,
    CURSOR_SPRAY,
    CURSOR_WAIT,
    CURSOR_TEXT,
    CURSOR_CROSSHAIR,
    /* Directional cursors. */
    CURSOR_UP_DOWN,
    CURSOR_LEFT_RIGHT,
    /* Sizing cursors. */
    CURSOR_TOP_SIDE,
    CURSOR_BOTTOM_SIDE,
    CURSOR_LEFT_SIDE,
    CURSOR_RIGHT_SIDE,
    CURSOR_TOP_LEFT_CORNER,
    CURSOR_TOP_RIGHT_CORNER,
    CURSOR_BOTTOM_RIGHT_CORNER,
    CURSOR_BOTTOM_LEFT_CORNER,
    /* Inherit from parent window. */
    CURSOR_INHERIT = 100,
    /* Blank cursor. */
    CURSOR_NONE,
    /* Fullscreen crosshair (if available). */
    CURSOR_FULL_CROSSHAIR
};

/**
 *  KeyModifer is an enumerated type that maps directly to the
 *  GLUT equivalents for the corresponding key modifers: SHIFT, CRTL, and ALT.
 *  These can be combined with bitwise OR to represent combinations such as
 *  CTRL-ALT.
 */
enum KeyModifier 
{ 
    NO_KEY_MODIFIER = 0, 
    SHIFT_KEY_MODIFIER = 1, 
    CTRL_KEY_MODIFIER = 2, 
    ALT_KEY_MODIFIER = 4 
};

class Window;        //  Forward reference
class ObjectWindow;  //  Forward reference


/**
 *   Represents a persistent graphical object used within a window.
 *   A graphical object is owned by a particular window.
 */
class GraphicalObject
{
private:
    /**  Used to generate a unique ID for each graphical object  */
    static unsigned id_source;

protected:
    /**  The window in which this graphical object is a component  */
    ObjectWindow *window;

    /**  Location of the mouse pointer's x coordinate relative to the graphical
      *  object's bounding box  */
    double x_hit_offset;
	
	/**  Location of the mouse pointer's y coordinate relative to the graphical
      *  object's bounding box  */
	double y_hit_offset;

	/**  x coordinate of the left-bottom corner of the graphical 
	 *   object's bounding box  */
    double left;
	
    /**  y coordinate of the left-bottom corner of the graphical 
	 *   object's bounding box  */
	double bottom;

	/**  Width of the graphical object's bounding box in
	 *   the viewport coordinate system  */
    double width;
	
	/**  Height of the graphical object's bounding box in
	 *   the viewport coordinate system  */
	double height;

    /**  Shape that the cursor should assume when the mouse
     *   hovers over this graphical object. */
    CursorShape cursor;

    /**  Indicates if this object is the one that the mouse
     *   is currently hovering over.  */
     bool mouse_over;


public:
    /**  This graphical object's unique ID  */
    const unsigned id;

	/**  Constructor specifies the bounding box extent.  
	 *   @param left the x coordinate in viewport coordinates
	 *               of the left-bottom corner of the
	 *               graphical object's bounding box
     *   @param left the y coordinate in viewport coordinates 
	 *               of the left-bottom corner of the
	 *               graphical object's bounding box
	 *   @param width the width in viewport coordinates of the graphical 
	 *                object's bounding box
	 *   @param height the height in viewport coordinates of the graphical 
	 *                object's bounding box
	 */
    GraphicalObject(double left, double bottom, double width, 
                    double height);

	/**  Copy construction creates a new graphical object with
	 *   identical location.  The new graphical object has the same
	 *   containing window but has a unique id.  The pre-existing 
	 *   graphical object is not affected by the copy.
	 *   @param other the graphical object from which this new graphical
	 *   object is being created.  	 */
    GraphicalObject(const GraphicalObject& other);

	/**  Assignment copies the location, size, and window container from 
	 *   another graphical object.  The id of the window being assigned 
	 *   is not affected.  The source graphical object is not affected 
	 *   by the copy.
	 *   @param other the source graphical object from which this graphical
	 *   object is being assigned.
	 *   @return a reference to this graphical object as per standard
	 *           assignment operator sementics
	 */
    GraphicalObject& operator=(const GraphicalObject& other);

	/**
	 *  Destroys a graphical object.
	 */
    virtual ~GraphicalObject();

	/**
	 *  Returns the x value of the left-bottom corner of the 
	 *  graphical object's bounding box.  The value is given 
	 *  in the viewport coordinate system.
	 *  @return the x coordinate of the graphical object's
	 *          left-bottom corner
	 */
	virtual double get_left() const;

	/**
	 *  Returns the y value of the left-bottom corner of the 
	 *  graphical object's bounding box.  The value is given 
	 *  in the viewport coordinate system.
	 *  @return the y coordinate of the graphical object's
	 *          left-bottom corner
	 */
	virtual double get_bottom() const;

	/**
	 *  Returns the width in viewport coordinates of the 
	 *  graphical object's bounding box.
	 *  @return the width of the graphical object
	 */
	virtual double get_width() const;

	/**
	 *  Returns the height in viewport coordinates of the 
	 *  graphical object's bounding box.
	 *  @return the height of the graphical object
	 */
	virtual double get_height() const;

	/**
	 *  Sets the location and size of the graphical object.
	 *  Values given are in the containing viewport's coordinates.
	 *  @x the x coordinate of the left-bottom corner of the 
     *  graphical object's bounding rectangle.
	 *  @y the y coordinate of the left-bottom corner of the 
     *  graphical object's bounding rectangle.
	 *  @width the graphical object's width in viewport coordinates.
	 *  @height the graphical object's height in viewport coordinates.
	 *  @return nothing
	 */
	virtual void set(double x, double y, double width, double height);


	/**
	 *  The paint method is called by the containing window's 
	 *  paint method when it renders this graphical object.
	 *  This method is abstract, so concrete derived classes must
     *  provide a concrete imppementation.
	 *  @return nothing
	 */
    virtual void paint() const = 0;


	/**
	 *  Relocates the left-bottom corner of the graphical object's
	 *  bounding box to the specified location.  The graphical object's
	 *  width and height are unaffected.
	 *  @param left the x coordinate in viewport coordinates
	 *               of the left-bottom corner of the
	 *               graphical object's bounding box
	 *  @param bottom the y coordinate in viewport coordinates
	 *               of the left-bottom corner of the
	 *               graphical object's bounding box
	 *  @return nothing
	 */  
    virtual void move_to(double left, double bottom);

	/**
	 *  Called by the containing window's mouse_pressed method
     *  when the user depresses any mouse
	 *  button when the mouse pointer is over the graphical object.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @param button the button that was depressed, left (LEFT_BUTTON) 
	 *         or right (RIGHT_BUTTON) during the current mouse event.
	 *  @return nothing
	 */
	virtual void mouse_pressed(double x, double y, MouseButton button);

	/**
	 *  Called by the containing window's mouse_released method
     *  when the user releases any mouse
	 *  button when the mouse pointer is over the graphical object.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @param button the button that was depressed, left (LEFT_BUTTON) 
	 *         or right (RIGHT_BUTTON) during the current mouse event.
	 *  @return nothing
	 */
	virtual void mouse_released(double x, double y, MouseButton button);

	/**
	 *  Called by the containing window's mouse_moved method
     *  when the user moves the mouse
	 *  when the mouse pointer is over the graphical object.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @return nothing
	 */
	virtual void mouse_moved(double x, double y);

	/**
	 *  Called by the containing window's mouse_dragged method
     *  when the user drags the mouse
	 *  By default it relocates the (left,bottom) location of 
     *  the graphical object's bounding box relative to 
     *  a mouse drag event.  
     *  The move is relative to the location of the last
	 *  mouse event on the object.  This allows the user to drag the
	 *  graphical object without it "jumping" slightly at first movement.
	 *  The graphical object's width and height are unaffected.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location during the current mouse event.
	 *  @return nothing
	 */
	virtual void mouse_dragged(double x, double y);

	/**
	 *  Called by the containing window when the user types a key
	 *  when the window has keyboard focus and the mouse pointer
     *  is over the graphical object.  The key pressed and
	 *  the location of the mouse pointer in viewport coordinates 
	 *  is reported.  
	 *  @param k the key typed by the user 
	 *  @param x the x coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window
	 *  @param y the y coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window
	 *  @return nothing
	 */
	virtual void key_pressed(int k, double x, double y);



	/**
	 *  Returns true if the coordinate provided is within the graphical
	 *  object's bounding box; otherwise, it returns false.
	 *  @param x the x coordinate in viewport coordinates of the
	 *           location to check
	 *  @param y the y coordinate in viewport coordinates of the
	 *           location to check
	 *  @return true, if (x,y) intersects the bounding box of the graphical
	 *          object; otherwise it returns false
	 */
    virtual bool hit(double x, double y);

	/**
	 *  Set the containing window.
	 *  @param win a pointer to the window to make the containing 
	 *             window for this graphical object.
	 *  @return a pointer to the previous owner of this window
	 */
	virtual Window *set_window(ObjectWindow *win);

	/**
	 *  Get the containing window of this graphical object.
	 *  @return a pointer to the window that contains this 
	 *          graphical object.
	 */
	virtual Window *get_window() const;

	/**
	 *  Activates or deactivates the graphical object
     *  @param flag if true, activates the object; otherwise,
                    deactivates the object
	 *  @return nothing
	 */
	virtual void set_mouse_over(bool flag);

    /**
     *  Sets the graphical object's cursor shape.
     *  @param cursor the object's cursor shape
     *  @return the previous cursor shape
     */
    virtual CursorShape set_cursor(CursorShape cursor);

    /**
     *  Returns the cursor shape associated with this graphical
     *  object. 
     *  @return the cursor shape associated with this graphical
     *          object
     */
    virtual CursorShape get_cursor();
};



/**
 *  Some commonly used colors
 */
extern const Color BLACK, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE,
                   DARK_RED, DARK_GREEN, DARK_BLUE, LIGHT_RED, LIGHT_GREEN,
				   LIGHT_BLUE, GRAY, LIGHT_GRAY, DARK_GRAY;

/**
 *  The class representing basic graphical window objects.
 *  This simple window class does not support contained 
 *  graphical objects, but the derived class ObjectWindow
 *  does.
 */
class Window
{
protected:
	//  Extents of window in viewport coordinates
	/**  The smallest x value representing the left-most position
	 *   within the viewport.  */
	double min_x;

	/**  The largest x value representing the right-most position
	 *   within the viewport.  */
	double max_x;

	/**  The smallest y value representing the bottom-most position
	 *   within the viewport.  */
    double min_y;

	/**  The largest y value representing the top-most position
	 *   within the viewport.  */
	double max_y;

    /**  The standard cursor for this window. */
    CursorShape normal_cursor;

	/**  Records the status of the SHIFT, ALT, and CTRL keys during the
	 *   the latest key press, mouse pressed, or mouse released event
	 *   within the window.  */
	KeyModifier key_mods;

	/**  
	 *  Initialization routine used by the window constructors.
	 *  @param title the text to appear within the window's titlebar
	 *  @param left the x coordinate in screen coordinates of the window's 
	 *         left-top corner 
	 *  @param top the y coordinate in screen coordinates of the window's 
	 *         left-top corner  
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
	 *  @param min_x the smallest x value representing the left-most position
	 *               within the viewport
	 *  @param max_x the largest x value representing the right-most position
	 *               within the viewport
	 *  @param min_y the smallest y value representing the bottom-most position
	 *               within the viewport
	 *  @param max_y the largest y value representing the top-most position
	 *               within the viewport
	 *  @return nothing
	 */
	void initialize(const char *title, int left, int top, int width, int height, 
		            double min_x, double max_x, double min_y, double max_y);

public:


	/**
	 *  Window constructor that provides the most control to the client.
	 *  Creates a window with detailed information.
	 *  After intializing instrance variables, the constructor defers 
	 *  the actual work to the initialize method.
     *  @param title the text to appear within the window's titlebar
	 *  @param left the x coordinate in screen coordinates of the window's 
	 *         left-top corner 
	 *  @param top the y coordinate in screen coordinates of the window's 
	 *         left-top corner  
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
	 *  @param min_x the smallest x value representing the left-most position
	 *               within the viewport
	 *  @param max_x the largest x value representing the right-most position
	 *               within the viewport
	 *  @param min_y the smallest y value representing the bottom-most position
	 *               within the viewport
	 *  @param max_y the largest y value representing the top-most position
	 *               within the viewport
	 */
	Window(const char *title, int left, int top, int width, int height, 
		   double min_x, double max_x, double min_y, double max_y);

	/**
	 *  Creates a window with a default position and size.
	 *  After intializing instrance variables, the constructor defers 
	 *  the actual work to the initialize method.  The virtual
     *  viewport's range is 0.0...width by 0.0...height.
	 *  @param left the x coordinate in screen coordinates of the window's 
	 *         left-top corner 
	 *  @param top the y coordinate in screen coordinates of the window's 
	 *         left-top corner  
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
	 */
	Window(const char *title, int width, int height);


	/**
	 *  Creates a window with a default position and size.
	 *  After intializing instrance variables, the constructor defers 
	 *  the actual work to the initialize method.
     *  @param title the text to appear within the window's titlebar
	 *  @param min_x the smallest x value representing the left-most position
	 *               within the viewport
	 *  @param max_x the largest x value representing the right-most position
	 *               within the viewport
	 *  @param min_y the smallest y value representing the bottom-most position
	 *               within the viewport
	 *  @param max_y the largest y value representing the top-most position
	 *               within the viewport
	 */
	Window(const char *title, double min_x, double max_x, double min_y, double max_y);

	/**
	 *  Create a default, plain window
	 */
	Window();

	/**
	 *  Destroys a graphical window object.
	 */
	virtual ~Window();

	/**
	 *  Erase all the drawing within the window.  Does not remove any 
	 *  added graphical objects.
	 *  @return nothing
	 */
	virtual void clear();


    /**
	 *  Sets the title of the window. 
	 *  @param str the text of the new title
	 *  @return nothing
	 */
    virtual void set_title(const char *str);

	/**
	 *  Sets the background color of the window's viewport.
	 *  @param color a reference to a Color object specifying the window's
	 *         background color.
	 *  @return nothing
	 */
	virtual void set_background_color(const Color &color);

	/**
	 *  Sets the location of the left-top corner of the window's
     *  client area to (x,y).
	 *  @param x the x coordinate of the window's left-top corner
	 *  @param y the y coordinate of the window's left-top corner
	 *  @return nothing
	 */
	virtual void set_position(int x, int y);

	/**
	 *  Sets the physical window's hoizontal and vertical
     *  dimensions.
	 *  @param width the new width of the window
	 *  @param height the new height of the window
	 *  @return nothing
	 */
	virtual void set_size(int width, int height);

	/**
	 *  Sets the horizontal and vertical ranges of the window's virtual 
	 *  viewport.  Values given are in viewport's coordinates.
	 *  @left the left-most (lowest) x value in viewport coordinates.
	 *  @right the right-most (highest) x value in viewport coordinates.
	 *  @bottom the bottom-most (lowest) y value in viewport coordinates.
	 *  @top the top-most (highest) y value in viewport coordinates.
	 *  @return nothing
	 */
	virtual void set_viewport(double left, double right, 
                              double bottom, double top);

	/**
	 *  Shows or hides the window.
	 *  viewport.  Values given are in viewport's coordinates.
	 *  @param visible if true, the window is visible;
     *                 otherwise, the window is invisible.
	 *  @return nothing
	 */
	virtual void set_visible(bool visible);

    /**
     *  Sets the window's cursor shape
     *  @param cursor one of several CursorShape values
     *  @return the previous cursor shape that was replaced
     *          by this call
     */
    virtual CursorShape set_cursor(CursorShape cursor);

    /**
     *  Gets the window's curent cursor shape
     *  @return the window's current cursor shape
     */
    virtual CursorShape get_cursor();

    /*
     *  Start the graphics event loop.  Control is passed to the 
     *  event manager and the event manager maintains control for the
     *  rest of the program's execution.  This method should be
     *  called only once.
	 *  @return nothing
     */
	virtual void run();

	/**
	 *  Request the window to be redrawn.
	 *  @return nothing
	 */
	virtual void repaint();

	/**  
	 *  Code executed before the call to paint
	 *  @return nothing
	 */
	virtual void prepaint();

	/**
	 *  Code executed after the call to paint
	 *  @return nothing
	 */
	virtual void postpaint();

	/**
	 *  Responsible for rendering the image in the window's viewport.
     *  This method is abstract, so concrete derived classes must
     *  provide a concrete implementation.
	 *  @return nothing.
	 */
	virtual void paint() = 0;

	/**  Called automatically by the event loop.  Calls prepaint, paint, 
	 *   and postpaint.  Called when the window is repainted.
	 *   @return nothing
	 */
	virtual void paint_all();

	/**
	 *  Returns the x value in screen coordinates of the left-top 
	 *  corner of the physical window.
	 *  @return the x value in screen coordinates of the left-top 
	 *          corner of the window
	 */
	virtual int get_x() const;

	/**
	 *  Returns the y value in screen coordinates of the left-top 
	 *  corner of the physical window.
	 *  @return the y value in screen coordinates of the left-top 
	 *          corner of the window
	 */
	virtual int get_y() const;

	/**
	 *  Returns the width in screen coordinates of the physical window.
	 *  @return the width in screen coordinates of the window
	 */
	virtual int get_width() const;

	/**
	 *  Returns the height in screen coordinates of the physical window.
	 *  @return the height in screen coordinates of the window
	 */
	virtual int get_height() const;

	/**
	 *  Sets the size of the physical window.
	 *  @param w the window's new width in screen coordinates
	 *  @param h the window's new height in screen coordinates
	 *  @return nothing
	 */
	virtual void set_window_size(int w, int h);

	/**
	 *  Responds to a change in the width and/or height of the 
	 *  physical window.  The event loop calls this method when the user
	 *  resizes the window.
	 *  @param w the window's new width in screen coordinates
	 *  @param h the window's new height in screen coordinates
	 *  @return nothing
	 */
	virtual void resized(int w, int h);

	/**
	 *  Returns the viewport's left-most (lowest) x value in viewport
	 *  coordinates.
	 *  @return the viewport's left-most (lowest) x value in viewport
	 *          coordinates
	 */
	virtual double get_min_x() const;

	/**
	 *  Returns the viewport's right-most (highest) x value in viewport
	 *  coordinates.
	 *  @return the viewport's right-most (highest) x value in viewport
	 *          coordinates
	 */
	virtual double get_max_x() const;

	/**
	 *  Returns the viewport's bottom-most (lowest) y value in viewport
	 *  coordinates.
	 *  @return the viewport's bottom-most (lowest) y value in viewport
	 *          coordinates
	 */
	virtual double get_min_y() const;

	/**
	 *  Returns the viewport's top-most (highest) y value in viewport
	 *  coordinates.
	 *  @return the viewport's top-most (highest) y value in viewport
	 *          coordinates
	 */
	virtual double get_max_y() const;

	/**
	 *  Draws the x and y axes for the viewport.  Draws grid lines at
	 *  the specified intervals.  To avoid drawing grid lines, 
	 *  clients should specify x_inc and/or y_inc values greater than 
	 *  the range of viewport coordinates.
	 */
	virtual void draw_axes(double x_inc, double y_inc) const;

	/**
	 *  Called by the event loop when the user depresses any mouse
	 *  button when the mouse pointer is within the window.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param button the button that was depressed, left (LEFT_BUTTON) 
	 *         or right (RIGHT_BUTTON) during the current mouse event.
	 *  @return nothing
	 */
	virtual void mouse_pressed(double x, double y, MouseButton button);

	/**
	 *  Called by the event manager when the user releases any mouse
	 *  button when the mouse pointer is within the window.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param button the button that was released, left (LEFT_BUTTON) 
	 *         or right (RIGHT_BUTTON) during the current mouse event.
	 *  @return nothing
	 */
	virtual void mouse_released(double x, double y, MouseButton button);

	/**
	 *  Called by the event manager when the user moves the mouse
	 *  when the mouse pointer is within the window.  The location
	 *  reported is given in viewport coordinates.
	 *  @param x the x coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @param y the y coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @return nothing
	 */
	virtual void mouse_moved(double x, double y);

	/**
	 *  Called by the event manager when the user drags the mouse
	 *  (moves the mouse while holding down a mouse button)
	 *  when the mouse pointer is within the window.  The location
	 *  reported is given in viewport coordinates.
	 *  @param x the x coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @param y the y coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @return nothing
	 */
	virtual void mouse_dragged(double x, double y);

	/**
	 *  Called by the event manager when the mouse pointer
	 *  enters the window.
	 *  @return nothing
	 */
	virtual void mouse_entered();

	/**
	 *  Called by the event manager when the mouse pointer
	 *  exits the window.
	 *  @return nothing
	 */
	virtual void mouse_exited();


	/**
	 *  Called by the event manager when the user types a key
	 *  when the window has keyboard focus.  The key pressed and
	 *  the location of the mouse pointer in viewport coordinates 
	 *  is reported.  
	 *  @param k the key typed by the user 
	 *  @param x the x coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window
	 *  @param y the y coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window
	 *  @return nothing
	 */
	virtual void key_pressed(int k, double x, double y);

	/**
	 *  Returns the active key modifier: SHIFT, CTRL, and/or ALT, if
	 *  any.  The return value is the bitwise ORed combination of 
	 *  active key modifiers.
	 *  @return the active key modifier
	 */
	KeyModifier get_key_modifiers() const;

	/**
	 *  Sets the key modifiers instance variable.  This method is
	 *  ordinarily used by SGL internally for preprocessing calls from
	 *  the event manager; clients typically will not call this method
	 *  directly.
	 *  @param mod the bitwise ORed combination of key modifiers 
	 *             (SHIFT_KEY, CTRL_KEY, ALT_KEY)
     *  @return nothing
	 */
	void set_key_modifiers(KeyModifier mod);

	/**
	 *  Starts a countdown timer for this window.  The event manager
	 *  calls timer_expired when the timer expires.
	 *  @param msec the number of milliseconds to count
	 *  @return nothing
	 */
	virtual void start_timer(int msec);

	/**
	 *  Called by the event manager when the current timer expires.
	 *  The timer is not automatically reset, so clients such as 
	 *  animations that require continuous periodic notification
	 *  must call start_timer after processing each timer_expired 
	 *  message.
	 *  @return nothing
	 */
	virtual void timer_expired();
};

/** A type for menu item event handling. */
typedef void (Window::*WindowCallback)();


/**
 *  Implements a popup menu.  Each popup menu must be
 *  associated with a particular window.
 */
class PopupMenu
{
protected:
    /**  The window associated with this popup menu */
    Window *window;

    struct MenuItem
    {
        const char *name;
        WindowCallback code;
        MenuItem(const char *name, WindowCallback code):
              name(name), code(code) {}
    };
	
	/**  Collection of menu items  */
	vector<MenuItem> items;

	/** Calls the proper handler for the user's selection  */
	static void process_menu_events(int option);

public:
	
	/**
	 *  Intializes an empty popup menu.
	 */
	PopupMenu(Window *win);

	/**
	 *  Destroys a popup menu
	 */
	virtual ~PopupMenu();

	/**
	 *  Adds specifies which method to call for a given menu item.
	 *  Clients ordinarily will use the add_menu_item macro (no leading
	 *  underscore) so a messy type cast is no necessary.
	 *  @param item a string listed for the menu choice
	 *  @param func a method to call when the user selects the given
	 *              menu item
	 *  @return nothing
	 */
	virtual void _add_menu_item(const char *item, WindowCallback func);

	/**
	 *  Replaces one menu item and its associated handler with another.
	 *  The current menu item name is supplied along with the new menu 
	 *  item name and the new handler.
	 *  @param old_name a string listed for the current menu choice
	 *  @param new_name a string listed for the replacement menu choice
	 *  @param func the replacement handler for this menu item
	 *  @return nothing
	 */
	virtual void _replace_menu_item(const char *old_name, 
		                            const char *new_name, 
				  				    WindowCallback func);

	/**
	 *  Removes a menu item and its associated handler.
	 *  The current menu item name is supplied.
	 *  @param item a string listed for the current menu choice
	 *  @return nothing
	 */
	virtual void remove_menu_item(const char *item);

	/**
	 *  Invokes the proper handler when the user selects an item in 
	 *  a popup menu.  Client ordinarily do not call this method 
	 *  directly, nor do they override it.
	 *  @param n an index into the table of handlers maintained by
	 *           this popup menu
	 *  @return nothing
	 */
	virtual void execute_handler(int n);
};

/**
 *  The class representing window objects that can contain
 *  graphical objects that the user can manipulate.
 */
class ObjectWindow: public Window
{
protected:
	/**  The list of graphical objects contained in this window.  */
	list<GraphicalObject *> object_list;
    GraphicalObject *active_object;

public:
	/**
	 *  Constructor that provides the most control to the client.
	 *  Creates a window with detailed information.
     *  @param title the text to appear within the window's titlebar
	 *  @param left the x coordinate in screen coordinates of the window's 
	 *         left-top corner 
	 *  @param top the y coordinate in screen coordinates of the window's 
	 *         left-top corner  
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
	 *  @param min_x the smallest x value representing the left-most position
	 *               within the viewport
	 *  @param max_x the largest x value representing the right-most position
	 *               within the viewport
	 *  @param min_y the smallest y value representing the bottom-most position
	 *               within the viewport
	 *  @param max_y the largest y value representing the top-most position
	 *               within the viewport
	 */
	ObjectWindow(const char *title, int left, int top, int width, 
                 int height, double min_x, double max_x, 
                 double min_y, double max_y);

	/**
	 *  Creates a window of a specified width and height
     *  @param title the text to appear within the window's titlebar
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
	 */
	ObjectWindow(const char *title, int width, int height);


	/**
	 *  Creates a window with a default position and size.
     *  @param title the text to appear within the window's titlebar
	 *  @param min_x the smallest x value representing the left-most position
	 *               within the viewport
	 *  @param max_x the largest x value representing the right-most position
	 *               within the viewport
	 *  @param min_y the smallest y value representing the bottom-most position
	 *               within the viewport
	 *  @param max_y the largest y value representing the top-most position
	 *               within the viewport
	 */
	ObjectWindow(const char *title, double min_x, double max_x, 
                 double min_y, double max_y);

	/**
	 *  Create a default, plain window
	 */
	ObjectWindow();

	/**
	 *  Destroys a graphical window object.
	 */
	virtual ~ObjectWindow();

	/**  
	 *  Code executed before the call to paint
	 *  @return nothing
	 */
	virtual void prepaint();

	/**
	 *  Code executed after the call to paint
	 *  @return nothing
	 */
	virtual void postpaint();

	/**
	 *  Called by the event loop when the user depresses any mouse
	 *  button when the mouse pointer is within the window.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param button the button that was depressed, left (LEFT_BUTTON) 
	 *         or right (RIGHT_BUTTON) during the current mouse event.
	 *  @return nothing
	 */
	virtual void mouse_pressed(double x, double y, MouseButton button);

	/**
	 *  Called by the event manager when the user releases any mouse
	 *  button when the mouse pointer is within the window.
	 *  @param x the x coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param y the y coordinate in viewport coordinates of the mouse
	 *           pointer location within the window during the current
	 *           mouse event.
	 *  @param button the button that was released, left (LEFT_BUTTON) 
	 *         or right (RIGHT_BUTTON) during the current mouse event.
	 *  @return nothing
	 */
	virtual void mouse_released(double x, double y, MouseButton button);

	/**
	 *  Called by the event manager when the user moves the mouse
	 *  when the mouse pointer is within the window.  The location
	 *  reported is given in viewport coordinates.
	 *  @param x the x coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @param y the y coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @return nothing
	 */
	virtual void mouse_moved(double x, double y);

	/**
	 *  Called by the event manager when the user drags the mouse
	 *  (moves the mouse while holding down a mouse button)
	 *  when the mouse pointer is within the window.  The location
	 *  reported is given in viewport coordinates.
	 *  @param x the x coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @param y the y coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window during
	 *           the most recent mouse event.
	 *  @return nothing
	 */
	virtual void mouse_dragged(double x, double y);

	/**
	 *  Called by the event manager when the user types a key
	 *  when the window has keyboard focus.  The key pressed and
	 *  the location of the mouse pointer in viewport coordinates 
	 *  is reported.  
	 *  @param k the key typed by the user 
	 *  @param x the x coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window
	 *  @param y the y coordinate in viewport coordinates of the 
	 *           location of the mouse pointer within the window
	 *  @return nothing
	 */
	virtual void key_pressed(int k, double x, double y);

	/**
	 *  Adds a graphical object to the window.
	 *  @param obj the graphical object to add to this window
	 *  @return nothing
	 */
	void add(GraphicalObject *obj);

	/**  
	 *  Removes a graphical object from the window.
	 *  @param obj the graphical object to remove from the window.
	 *  @return nothing
	 */
    void remove(GraphicalObject *obj);

	/**
	 *  Removes all the graphical objects from the list of
	 *  contained graphical objects.  Frees up the memory 
	 *  allocated for the object.
	 *  @return nothing
	 */
	void remove_all();

	/**
	 *  Returns the first graphical object in the list of
	 *  graphical objects with a bounding box that intersects the 
	 *  given viewport coordinates.
	 *  @param x the x coordinate in viewport coordinates of a
	 *           location within the viewport
	 *  @param y the y coordinate in viewport coordinates of a
	 *           location within the viewport
	 *  @return a contained graphical object with a bounding box 
	 *          that intersects the given position within the 
	 *          viewport.  Returns null if no contained graphical
	 *          object intersects the given point
	 */
    GraphicalObject *hit(double x, double y) const;

    /**
     *  Returns an iterator to the begining of the vector of 
     *  graphical objects this window contains.
     *  @return  an iterator to the begining of the vector of 
     *           graphical objects this window contains.
     */
    list<GraphicalObject *>::iterator begin();

    /**
     *  Returns an iterator just past the end of the vector of 
     *  graphical objects this window contains.
     *  @return  an iterator just past the end of the vector of 
     *           graphical objects this window contains.
     */
    list<GraphicalObject *>::iterator end();
};


/**
 *  A composite object aggregates a collection of 
 *  graphical objects (even other composite objects
 *  into a single graphical object.
 */
class CompositeObject: public GraphicalObject
{
protected:
    /**  The collection of contained graphical objects. */
    vector<GraphicalObject *> objects;
public:
    /**
     *  The constructor makes an initially empty 
     *  container.
     */
    CompositeObject();

    /**
     *  Draws each object that makes up the composite
     *  object.
     *  @return nothing.
     */
    void paint() const;

    /**
     *  Repositions the lower-left corner of the composite object's
     *  boundng box to (x,y).  The positions of the contained
     *  graphical objects are updated accordingly.
     */
    void move_to(double x, double y);

    /**
     *  Adds a graphical object to this container.  Adjusts
     *  the container's size accordingly.
     *  @param obj a pointer to the graphical object to add.
     *  @return nothing.
     */
    void add(GraphicalObject *obj);
};


/**
 *  The Pixmap class stores the number of rows and columns 
 *  in the pixmap, as well as the address of the first 
 *  pixel in memory.
 *  The implementation of the Pixmap class is adapted from 
 *  code found in  rgbpixmap.h from Hill's textbook 
 *  "Computer Graphics Using OpenGL" attributed to
 *  J. J. Rajnovich, June 05, 2002
 */

class Pixmap 
{
    //  Local type represents a color triple, each with 256 
    //  possible intensities
    struct RGB 
    {
        unsigned char red, green, blue;
    };

    /**
     *  Helper function for extracting an unsigned short
     *  value from a binary file.
     *  @param fin the file stream object providing
     *         the input.
     *  @return the value extracted from the input stream
     */
    static unsigned short get_short(ifstream& fin);

    /**
     *  Helper function for extracting an unsigned long
     *  value from a binary file.
     *  @param fin the file stream object providing
     *         the input.
     *  @return the value extracted from the input stream
     */
    static unsigned long get_long(ifstream& fin);

    /**
     *  Helper function for determining if an integer
     *  in a power of two.
     *  @param fin the file stream object providing
     *         the input.
     *  @return true, if n is an exact binary; otherwise, false
     */
    static bool power_of_2(int n);
public : 
	// dimensions of the pixmap
	int nRows, nCols;

	// array of pixels
	RGB* pixel;

	/**
     *  Read a BMP file into this pixmap.
     *  @param fname the name of the BMP file
     *         the read in.
     *  @return true, if the operation was 
     *          successful; otherwise, false.
     */
	bool read_BMP_file(const char *fname);

	/**
     *  Creates a checkerboard pixmap.
     *  @return true if successful and false
     *          if no memory was available
     *          for the pixmap.
     */
	bool make_checkerboard();

	// 
	//void set_texture(GLuint textureName);
	void set_texture(unsigned int textureName);
};



/**
 *  The class representing graphical objects that render
 *  bitmapped images.  The image for a graphical object
 *  is loaded from a 24-bit uncompressed BMP file with
 *  dimensions that must be binary powers.
 */

class BitmapObject: public GraphicalObject
{
    Pixmap pix;
    unsigned int texture_id;
public:
    /**
     *  Constructor 
     *  @param filename the BMP file containing the image to 
     *         display.
     *  @param x the x coordinate of the bitmap image's 
     *           left-bottom corner
     *  @param y the y coordinate of the bitmap image's 
     *           left-bottom corner
     *  @param width the bitmap image's width
     *  @param height the bitmap image's height
     */
    BitmapObject(const char *filename, double x, double y, 
           double width, double height);

    /**
     *  Draws the bitmap image within the window.
     */
    void paint() const;
};





/**
 *  The class representing window objects that bypass
 *  SGL and use OpenGL and GLUT routines directly.
 */
class OGLWindow: public Window
{
public:
	/**
	 *  Constructor that provides the most control to the client.
	 *  Creates a window with detailed information.
     *  @param title the text to appear within the window's titlebar
	 *  @param left the x coordinate in screen coordinates of the window's 
	 *         left-top corner 
	 *  @param top the y coordinate in screen coordinates of the window's 
	 *         left-top corner  
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
	 */
	OGLWindow(const char *title, int left, int top, int width, int height);

	/**
	 *  Creates a window with a default position and size.
     *  @param title the text to appear within the window's titlebar
	 */
	OGLWindow(const char *title);

	/**
	 *  Destroys a graphical window object.
	 */
	virtual ~OGLWindow();

	/**  
	 *  Code executed before the call to paint
	 *  @return nothing
	 */
	virtual void prepaint();

	/**
	 *  Code executed after the call to paint
	 *  @return nothing
	 */
	virtual void postpaint();

};


/******************************************
 *  Classes for seven segment digit displays
 ******************************************/

/**
 *  Used to draw decimal digits that look like
 *  LED/LCD seven-segment displays
 */
class SevenSegmentDigit
{
protected:
    /** The current digit's value.  */
    int current_value;

    /** The color of the displayed digit.  */
    Color color;

    /**  (x,y) coordinates of lower-left corner of the LED.  */
    double x, y;    

    /**  The height of the LED.  */
    double height;  
    
    //  Draw the segments:
    //        +--- a ---+
    //        |         |
    //        f         b
    //        |         |
    //        +--- g ---+
    //        |         |
    //        e         c
    //        |         |
    //        +--- d ---+

    Point segment_a[4],
          segment_b[4],
          segment_c[4],
          segment_d[4],
          segment_e[4],
          segment_f[4],
          segment_g[6];

   
    /**
     *  This helper method updates the vertices of the individual
     *  segments relative to the position of the digit.
     *  This method is called when the digit is repositioned.
     *  @return nothing
     */
    void update_segment_locations();
    
public:
    /**
     *  The constructor sets the digit's color, position, and size.
     *  @param color the color of the digit
     *  @param x the x coordinate of the digit's lower-left corner
     *  @param y the y coordinate of the digit's lower-left corner
     *  @param height the height of the digit
     */
    SevenSegmentDigit(Color color, double x, double y, double height);

    /**
     *  Returns the x coordinate of the digit's lower-left corner.
     *  @return the x coordinate of the digit's lower-left corner.
     */
    double get_x() const;

    /**
     *  Returns the y coordinate of the digit's lower-left corner.
     *  @return the y coordinate of the digit's lower-left corner.
     */
    double get_y() const;

    /**
     *  Returns the digit's height.
     *  @return the digit's height.
     */
    double get_height() const;

    /**
     *  Draws the digit.
     */
    void paint() const;

    /**
     *  Set's the value of the digit.
     *  The assigned value is modulo ten.
     *  @param value value to assign, modulo 10
     *  @return nothing
     */
    void set_value(int value);

    /**
     *  Returns the digit's current value.
     *  @return the digit's current value.
     */
    int get_value() const;

    /**
     *  Increases the digit's value by 1, modulo 10.
     *  @return nothing
     */
    void increment();

    /**
     *  Decreases the digit's value by 1, modulo 10.
     *  @return nothing
     */
    void decrement();

    /**
     *  Changes the height of the digit by a given amount.  The digit's 
     *  width changes proportionally.
     *  @param the change in the height of the digit
     *  @return nothing
     */
    void resize(double inc);

    /**
     *  Moves the digit to a new location.  The digit's 
     *  size is unaffected.
     *  @param x the new x coordinate of the digit's lower-left corner.
     *  @param y the new y coordinate of the digit's lower-left corner.
     *  @return nothing
     */
    void move_to(double x, double y);
};


/**
 *  A graphical object wrapper for a seven-segment digit display.
 */
class DisplayDigit: public GraphicalObject
{
protected:
    /**  The wrapper seven-segment digit display.  */
    SevenSegmentDigit led;
    
public:
    /**
     *  The constructor sets the digit's color, position, and
     *  height.
     *  @param color the color of the digit
     *  @param x the x coordinate of the digit's lower-left corner
     *  @param y the y coordinate of the digit's lower-left corner
     *  @param height the height of the digit
     */
    DisplayDigit(Color color, double x, double y, double height);

    /**  Draws the digit. */
    void paint() const;

    /**
     *  Set's the value of the digit.
     *  The assigned value is modulo ten.
     *  @param value value to assign, modulo 10
     *  @return nothing
     */
    void set_value(int value);

    /**
     *  Returns the digit's current value.
     *  @return the digit's current value.
     */
    int get_value() const;


    /**
     *  Increases the digit's value by 1, modulo 10.
     *  @return nothing
     */
    void increment();

    /**
     *  Decreases the digit's value by 1, modulo 10.
     *  @return nothing
     */
    void decrement();

    /**
     *  Changes the height of the digit by a given amount.  The digit's 
     *  width changes proportionally.
     *  @param the change in the height of the digit
     *  @return nothing
     */
    void resize(double inc);

    /**
     *  Moves the digit to a new location.  The digit's 
     *  size is unaffected.
     *  @param x the new x coordinate of the digit's lower-left corner.
     *  @param y the new y coordinate of the digit's lower-left corner.
     *  @return nothing
     */
    void mouse_dragged(double x, double y);
};



/**
 *  A graphical object representing a two-digit
 *  seven-segment display.
 */
class DoubleDigit: public GraphicalObject
{
protected:
    /**  The tens digit.  */
    SevenSegmentDigit tens;

    /**  The ones digit.  */
    SevenSegmentDigit ones;

    /** If true, a leading zero is visible; otherwise,
        a leading zero is not displayed.  */
    bool leading_zero;

    /**  If true, the display is visible; otherwise, it is 
         invisible.  */
    bool visible;
    
public:
    /**
     *  The constructor sets the digit's color, position, and
     *  height.  The digit's value is initially zero.
     *  @param color the color of the digit
     *  @param x the x coordinate of the tens digit's lower-left corner
     *  @param y the y coordinate of the tens digit's lower-left corner
     *  @param height the height of the digits
     */
    DoubleDigit(Color color, double x, double y, double height);

    /**  Draws the digits. */
    void paint() const;

    /**
     *  Set's the value of the digits.
     *  The assigned value is modulo 100.
     *  @param value value to assign, modulo 100
     *  @return nothing
     */
    void set_value(int value);

    /**
     *  Returns the current value.
     *  @return the current value.
     */
    int get_value() const;

    /**
     *  Increases the digit's value by 1, modulo 100.
     *  @return nothing
     */
    void increment();

    /**
     *  Decreases the digit's value by 1, modulo 100.
     *  @return nothing
     */
    void decrement();

    /**
     *  Changes the height of the digits by a given amount.  The digits' 
     *  width changes proportionally.
     *  @param the change in the height of the digits
     *  @return nothing
     */
    void resize(double inc);

    /**
     *  Moves the digits to a new location.  The digits' 
     *  sizes are unaffected.
     *  @param x the new x coordinate of the tens digit's lower-left
     *           corner.
     *  @param y the new y coordinate of the tens digit's lower-left 
     *           corner.
     *  @return nothing
     */
    void mouse_dragged(double x, double y);

    /**
     *  Turns on or off the display of a leading zero.
     *  @param flag if true, a leading zero is displayed;
     *              otherwise, a leading zero is not displayed.
     *  @return nothing
     */
    void set_leading_zero(bool flag);

    /**
     *  Makes the display visible or invisible.
     *  @param flag if true, the digits are visible
     *              otherwise, the digits are invisible.
     *  @return nothing
     */
    void set_visible(bool flag);
};


/**
 *  A graphical object representing a multi-digit
 *  seven-segment display.
 */
class Multidigit: public GraphicalObject
{
protected:
    /**  Vector of seven segment digits  */
    vector<SevenSegmentDigit> digits;

    // /** Dynamic array of seven segment digits  */
    // SevenSegmentDigit *digits;

    /** If true, a leading zeros are visible; otherwise,
        leading zeros are not displayed.  */
    bool leading_zeros;

    /**  If true, the display is visible; otherwise, it is 
         invisible.  */
    bool visible;
    
    void increment_helper(int n);

    void decrement_helper(int n);

public:
    /**
     *  The constructor sets the seven-segment display's 
     *  number of digits, color, position, and
     *  height.  The display's value is initially zero.
     *  @param n the number of digits
     *  @param color the color of the digits
     *  @param x the x coordinate of the leading digit's lower-left corner
     *  @param y the y coordinate of the leading digit's lower-left corner
     *  @param height the height of the digits
     */
    Multidigit(int n, Color color, double x, double y, double height);

    /**  Draws the digits. */
    void paint() const;

    /**
     *  Set's the value of the display.
     *  @param value value to assign
     *  @return nothing
     */
    void set_value(int value);

    /**
     *  Returns the display's current value.
     *  @return the current value.
     */
    int get_value() const;

    /**
     *  Increases the display's value by 1
     *  @return nothing
     */
    void increment();

    /**
     *  Decreases the display's value by 1
     *  @return nothing
     */
    void decrement();

    /**
     *  Changes the height of the display by a given amount.  The 
     *  display's width changes proportionally.
     *  @param the change in the height of the display
     *  @return nothing
     */
    void resize(double inc);

    /**
     *  Moves the display to a new location.  The display's 
     *  size is unaffected.
     *  @param x the new x coordinate of the display's lower-left
     *           corner.
     *  @param y the new y coordinate of the display's lower-left 
     *           corner.
     *  @return nothing
     */
    void mouse_dragged(double x, double y);

	/**
	 *  Relocates the left-bottom corner of the display
	 *  to the specified location.  The display's size is
	 *  unaffected.
	 *  @param left the x coordinate in viewport coordinates
	 *               of the left-bottom corner of the
	 *               display
	 *  @param bottom the y coordinate in viewport coordinates
	 *               of the left-bottom corner of the
	 *               display
	 *  @return nothing
	 */  
    virtual void move_to(double left, double bottom);

    /**
     *  Turns on or off the display of leading zeros.
     *  @param flag if true, leading zeros are displayed;
     *              otherwise, leading zeros are not displayed.
     *  @return nothing
     */
    void set_leading_zeros(bool flag);

    /**
     *  Makes the display visible or invisible.
     *  @param flag if true, the digits are visible
     *              otherwise, the digits are invisible.
     *  @return nothing
     */
    void set_visible(bool flag);
};


class TimeDisplay: public GraphicalObject
{
protected:
    /**  The hours unit tens digit.  */
    SevenSegmentDigit hours_tens;

    /**  The hours unit ones digit.  */
    SevenSegmentDigit hours_ones;

    /**  The minutes unit tens digit.  */
    SevenSegmentDigit minutes_tens;

    /**  The minutes unit ones digit.  */
    SevenSegmentDigit minutes_ones;

    /**  The seconds unit tens digit.  */
    SevenSegmentDigit seconds_tens;

    /**  The seconds unit ones digit.  */
    SevenSegmentDigit seconds_ones;

    /**  The time, in seconds.  */
    int seconds;

    /** If true, leading units are visible; otherwise,
        leading units are not displayed.  */
    bool leading_units;

    /**  If true, the display is visible; otherwise, it is 
         invisible.  */
    bool visible;

    /**  
     *  Draw the hours-minutes or minutes-seconds colon separator.  
     *  @param left_ones the ones digit of the left quantity
     *  @param right_tens the tens digit of the right quantity
     *  @return nothing
     */
    void draw_separator(const SevenSegmentDigit& left_ones, const SevenSegmentDigit& right_tens) const;

    /**
     *  Draw the display without leading units that are zero; for example, 1:03:23 instead of
     *  01:03:23, 4:10 instead of 00:04:10, and 3 instead of 00:00:03.
     *  @return nothing
     */
    void paint_without_leading_units() const;

public:
    /**
     *  The constructor sets the display's color, position, and
     *  height.  The time is initially zero.
     *  @param color the color of the digit
     *  @param x the x coordinate of the tens digit's lower-left corner
     *  @param y the y coordinate of the tens digit's lower-left corner
     *  @param height the height of the digits
     */
    TimeDisplay(Color color, double x, double y, int height);

    /**  Draws the display. 
     *  @return nothing
     */
    void paint() const;

    /**
     *  Sets the time.
     *  @param sec desired time, in seconds
     *  @return nothing
     */
    void set_value(int sec);

    /**
     *  Returns the current time, in seconds.
     *  @return the current value.
     */
    int get_value() const;

    /**
     *  Increases the time by one second.
     *  @return nothing
     */
    void increment();

    /**
     *  Decreases the time by one second.
     *  @return nothing
     */
    void decrement();

    /**
     *  Changes the height of the digits by a given amount.  The digits' 
     *  width changes proportionally.
     *  @param the change in the height of the digits
     *  @return nothing
     */
    void resize(double inc);

    /**
     *  Moves the display to a new location.  The display's 
     *  size is unaffected.
     *  @param x the new x coordinate of the display's lower-left
     *           corner.
     *  @param y the new y coordinate of the display's lower-left 
     *           corner.
     *  @return nothing
     */
    void mouse_dragged(double x, double y);

    /**
     *  Moves the display to a new location.  The display's 
     *  size is unaffected.
     *  @param x the new x coordinate of the display's lower-left
     *           corner.
     *  @param y the new y coordinate of the display's lower-left 
     *           corner.
     *  @return nothing
     */
    void move_to(double x, double y);


    /**
     *  Turns on or off the display of a leading zero.
     *  @param flag if true, a leading zero is displayed;
     *              otherwise, a leading zero is not displayed.
     *  @return nothing
     */
    void set_leading_units(bool flag);

    /**
     *  Makes the display visible or invisible.
     *  @param flag if true, the digits are visible
     *              otherwise, the digits are invisible.
     *  @return nothing
     */
    void set_visible(bool flag);
};





/******************************************
 *  Global drawing functions
 ******************************************/


/**
 *  Global inialization function, called once per program
 *  execution.  This version allows the client to specify a
 *  initial display mode.  All SGL programs should call this
 *  function or its overloaded equivalent before creating 
 *  and using a graphical window.
 *  @param mode the initial display mode
 *  @return nothing
 */  
void initialize_graphics(unsigned int mode);

/**
 *  Global inialization function, called once per program
 *  execution.  This version provides double buffering,
 *  RGBA mode, and no depth buffer.
 *  All SGL programs should call this
 *  function or its overloaded equivalent before creating 
 *  and using a graphical window.
 *  @return nothing
 */  
void initialize_graphics();


/**
 *  Determines if the bounding boxes of two graphical objects
 *  overlap.  
 *  @param obj1 one of the graphical objects
 *  @param obj2 the other graphical object
 *  @return true, if obj1's bounding box intersects 
 *          obj2's bounding box
 */
bool intersect(const GraphicalObject& obj1, 
                          const GraphicalObject& obj2);

/**
 *  Draws the point (x,y) in the current drawing color.
 *  @param x the x coordinate of the (x,y) location.
 *  @param y the y coordinate of the (x,y) location.
 *  @return nothing
 */  
void draw_point(double x, double y);

/**
 *  Draws the given Point object in the current 
 *  drawing color.
 *  @param pt the Point object to draw.
 *  @return nothing
 */
void draw_point(const Point& pt);

/**
 *  Sets the size of a rendred point
 *  @param the point size
 *  @return nothing
 */
void set_point_size(int point_size);


/**
 *  Draws a line in the current drawing color.
 *  @param x0 the x coordinate in viewport coordinates 
 *            of the first point
 *  @param y0 the y coordinate in viewport coordinates 
 *            of the first point
 *  @param x1 the x coordinate in viewport coordinates 
 *            of the second point
 *  @param y1 the y coordinate in viewport coordinates 
 *            of the second point
 *  @return nothing
 */
void draw_line(double x0, double y0, double x1, double y1);

/**
 *  Draws a strippled line in the current drawing color.
 *  @param x0 the x coordinate in viewport coordinates 
 *            of the first point
 *  @param y0 the y coordinate in viewport coordinates 
 *            of the first point
 *  @param x1 the x coordinate in viewport coordinates 
 *            of the second point
 *  @param y1 the y coordinate in viewport coordinates 
 *            of the second point
 *  @param pattern a 32-bit pattern used to render the line;
                   defaults to dashes: 0x00FF
 *  @return nothing
 */
void draw_dashed_line(double x0, double y0, double x1, double y1, 
					  int pattern=0x00FF);

/**
 *  Draws a rectangular frame in the current drawing color.
 *  @param x the x coordinate in the viewport coordinate system
 *           of the left-bottom corner of the rectangle
 *  @param y the y coordinate in the viewport coordinate system
 *           of the left-bottom corner of the rectangle
 *  @param width the width in viewport coordinates of the 
 *               rectangle
 *  @param height the height in viewport coordinates of the 
 *                rectangle
 *  @return nothing
 */
void draw_rectangle(double x, double y, double width, double height);

/**
 *  Draws a filled rectangle in the current drawing color.
 *  @param x the x coordinate in the viewport coordinate system
 *           of the left-bottom corner of the rectangle
 *  @param y the y coordinate in the viewport coordinate system
 *           of the left-bottom corner of the rectangle
 *  @param width the width in viewport coordinates of the 
 *               rectangle
 *  @param height the height in viewport coordinates of the 
 *                rectangle
 *  @return nothing
 */
void fill_rectangle(double x, double y, double width, double height);

/**
 *  Draws a circle in the current drawing color.
 *  @param x the x coordinate in the viewport coordinate system
 *           of the circle's center
 *  @param y the y coordinate in the viewport coordinate system
 *           of the circle's center
 *  @param radius the radius of the circle in viewport coordinates 
 *  @return nothing
 */
void draw_circle(double x, double y, double radius);

/**
 *  Draws a filled circle in the current drawing color.
 *  @param x the x coordinate in the viewport coordinate system
 *           of the circle's center
 *  @param y the y coordinate in the viewport coordinate system
 *           of the circle's center
 *  @param radius the radius of the circle in viewport coordinates 
 *  @return nothing
 */
void fill_circle(double x, double y, double radius);

/**
 *  Draws the outline of a polygon specified by the points
 *  in a std::vector of Points. The points adjacent in the polygon 
 *  also must be adjacent in the vector.
 *  @param pts a std::vector of Point objects that comprise the vertices
 *             of the polygon.  Adjacent vertices in the polygon must
 *             adjacent in the vector.
 *  @return nothing
 */
void draw_polygon(const vector<Point>& pts);

/**
 *  Draws the outline of a polygon specified by the points
 *  in an array of Point objects. The points adjacent in the polygon 
 *  also must be adjacent in the vector.
 *  @param pts a std::vector of Point objects that comprise the vertices
 *             of the polygon.  Adjacent vertices in the polygon must
 *             adjacent in the vector.
 *  @param n the number of points in the array.
 *  @return nothing
 */
void draw_polygon(const Point *pts, int n);

/**
 *  Draws a filled polygon specified by the points
 *  in a std::vector of Points. The points adjacent in the polygon 
 *  also must be adjacent in the vector.
 *  @param pts a std::vector of Point objects that comprise the vertices
 *             of the polygon.  Adjacent vertices in the polygon must
 *             adjacent in the vector.
 *  @return nothing
 */
void fill_polygon(const vector<Point>& pts);

/**
 *  Draws a filled polygon specified by the points
 *  in an array of Point objects. The points adjacent in the polygon 
 *  also must be adjacent in the vector.
 *  @param pts an array of Point objects that comprise the vertices
 *             of the polygon.  Adjacent vertices in the polygon must
 *             adjacent in the vector.
 *  @param n the number of points in the array.
 *  @return nothing
 */
void fill_polygon(const Point *pts, int n);

/**
 *  Draw a piece of graphical text in the window.
 *  The provided (x,y) location specifies the left-bottom portion
 *  of the text.  This version of draw_text uses fixed-size
 *  bitmap fonts.
 *  @param text the string of text to display
 *  @param x the x coordinate in the viewport coordinate system
 *           of the text's left-most extent
 *  @param y the y coordinate in the viewport coordinate system
 *           of the text's baseline
 *  @param font_size the size of the font: 10, 12, or 18.
 */
void draw_text(const char *text, double x, double y, int font_size);

/**
 *  Draw a piece of graphical text in the window.  This version of
 *  draw_text uses a scalable stroke font.
 *  The provided (x,y) location specifies the left-bottom portion
 *  of the text.
 *  @param text the string of text to display
 *  @param x the x coordinate in the viewport coordinate system
 *           of the text's left-most extent
 *  @param y the y coordinate in the viewport coordinate system
 *           of the text's baseline
 *  @param scale the scale of the font; this generally will be 
 *           value less than one (for example, 0.1 or 0.25).
 */
void draw_text(const char *text, double x, double y, double scale);

/**
 *  Returns the length of the graphical text in units of 
 *  the viewport coordinate system.
 *  @param text the text string to measure
 *  @param font_size the size of the font: 10, 12, or 18.
 *  @return the length of the graphical text in units of 
 *          the viewport coordinate system.
 */
double text_width(const char *text, int font_size);

/**
 *  Draws a 2-D mathematical function, f(x).  Plots the line 
 *  segments joining (begin_x, f(begin_x)), (begin_x + delta, f(begin_x + delta)),
 *  (begin_x + delta + delta, f(begin_x + delta + delta)), ..., 
 *  (end_x, f(end_x)).
 *  @param f a pointer to a function that accepts a single 
 *           double-precision floating-point value
 *  @param begin_x the lowest x argument to evaluate
 *  @param end_x the highest x argument to evaluate
 *  @param delta the change in x; first begin_x will be evaluated,
           then begin_x + delta, then begin_x + delta + delta, ...,
		   until end_x is reached.
 *  @return nothing
 */
void draw_function(double (*f)(double), double begin_x, double end_x,
				   double delta);

/**
 *  Sets the current drawing color.
 *  @param color the color to use for subsequent drawing operations.
 *  @return nothing
 */
void set_color(const Color &color);

/**
 *  Sets the current drawing color according to the given RGB values.
 *  @param r the intensity of red to use for subsequent drawing operations.
 *  @param g the intensity of green to use for subsequent drawing operations.
 *  @param b the intensity of blue to use for subsequent drawing operations.
 *  @return nothing
 */
void set_color(double r, double g, double b);

/**
 *  Sets the current line width.
 *  @param width the width to use for subsequent line drawing operations.
 *  @return nothing
 */
void set_line_width(double width);

/****************************************************
 *   Miscellaneous utility functions
 ****************************************************/

/**
 *  Gets the width of the screen in pixels.
 *  @return the width of the screen in pixels; returns
 *          zero if the screen's width cannot be determined
 */
int get_screen_width();

/**
 *  Gets the height of the screen in pixels.
 *  @return the height of the screen in pixels; returns
 *          zero if the screen's height cannot be determined
 */
int get_screen_height();


/*  Add a random number generator class? */

/**
 *  Sets the pseudorandom number generator seed.  
 *  @param seed the pseudorandom number seed.  If 
 *              omitted, the seed is generated
 *              pseudorandomly from the operating
 *  			system's time.
 *  @return nothing
 */
void set_random_seed(int seed=-1);

/**
 *  Returns a pseudorandom number in the specified
 *  range.  
 *  @param begin the lowest integer in the set of 
 *         potential return values.
 *  @param end the highest integer in the set of
 *         potential return values.
 *  @return a pseudorandom integer, r,  such that
 *         begin <= r <= end
 */
int random(int begin, int end);

/**
 *  Returns a pseudorandom number in the range 
 *  0...n - 1.  
 *  @param n the number of distinct potential pseudorandom
           values.
 *  @return a pseudorandom integer, r,  such that
 *         0 <= r < n
 */
int random(int n);


/**
 *  Implements stopwatch objects that can be used to 
 *  measure elapsed clock time in an executing program.  
 */
class Stopwatch
{
protected:
    /**  The starting time */
    clock_t start_time;

    /**  The time when the stopwatch was stopped */
    clock_t end_time;

    /**  A flag indicating if the stopwatch is running or stopped */
    bool running;

public:
    /**  
     *  Creates a Stopwatch object. A newly minted object 
     *  is not running and is in a "reset" state.
     */
    Stopwatch();

    /**  
     *  Starts the stopwatch.  If there is no current
     *  start time, the stopwatch has not been used or 
     *  has just been reset, so we record the current
     *  system time to begin a new timing.  If there is
     *  a valid start time, we merely resume the current timing.
     *  Puts the stopwatch object in the 'running' state.
     *  @return nothing
     */
    void start(); 

    /**  
     *  Stops the stopwatch.  Records the current
     *  system time and puts the stopwatch object in
     *  the 'not running' state. 
     *  If the stopwatch is already stopped, a diagnostic
     *  message is issued, but the stored elapsed time is
     *  unaffected.
     *  @return nothing
     */
    void stop();

    /**  
     *  Resets the stopwatch so a subsequent start begins recording 
     *  a new time.  An attempt to  reset a running stopwatch 
     *  produces an error message.
     *  @return nothing
     */
    void reset(); 

    /**  
     *  If the stopwatch is stopped, the elapsed
     *  method reports the time elapsed between
     *  the starting time and the stop time.  If
     *  the stopwatch is running, the elapsed
     *  method reports the time elapsed since
     *  the stopwatch was started.
     *  The time is reported in seconds.
     *  @return the time elapsed in seconds
     */
    double elapsed() const;  //  Reveal the elapsed time
};

/**
 *  Pauses the program's execution for a period of 
 *  milliseconds.
 *  @param msec the number of milliseconds to pause
 *  @return nothing
 */

void pause(int msec);



/**
 *  Determines if two floating-point numbers are
 *  close enough in value to be considered "equal."
 *  The call equals(a, b, c) returns true if 
 *  |a - b| < c; otherwise, it returns true.
 *  @param d1 the first value to compare
 *  @param d2 the second value to compare
 *  @param delta the tolerance factor: the distance
 *         between d1 and d2 must be less than  delta
 *         for d1 and d2 to be considered "equal"
 *  @return true, if d1 and d2 are within delta of 
 *          each other; otherwise, false
 */
bool equals(double d1, double d2, double delta);

/**
 *  Converts an integer to a string so it can be displayed
 *  with draw_text.
 *  @param i the integer to convert
 *  @return the string representation of i
 */
//string to_string(int i);
const char *to_string(int i);

/**
 *  Converts a double to a string so it can be displayed
 *  with draw_text.
 *  @param d the double to convert
 *  @return the string representation of d
 */
//string to_string(double d);
const char *to_string(double d);



/**
 *  Returns the SGL version number of this library.
 */
const char *version();


}  //  End of namespace sgl

//  Some syntactic sugar for adding items to popup menus
#define POPUPCALLBACK(x) (static_cast<WindowCallback>(x))
#define add_menu_item(x, y) _add_menu_item((x), (static_cast<WindowCallback>(y)));
#define replace_menu_item(x, y, z) _replace_menu_item((x), (y), (static_cast<WindowCallback>(z)));


//===============================================================
//  Originally in sgl.cpp

#include <sstream>
#include <cmath>
#include <cstdlib>  //  Linux requires cstdlib for exit and rand functions
#include <cstring>
#include <GL/glut.h>

using std::cout;
using std::endl;
using std::ios;

static const char *SGL_VERSION_NUMBER = "0.7.5";


static bool glut_active = false;
static bool event_loop_running = false;



//static sgl::Window *current_window = 0;  //  Current window intially null

//  Maximum number of windows managed by SGL
const int MAX_WINDOWS = 10;
//  List of windows managed by SGL
static sgl::Window *window_list[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

//  Some coordinate conversion helper functions
static void GetOGLPos(int x, int y, double *vec)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = static_cast<float>(x);
	winY = static_cast<float>(viewport[3] - y);
	glReadPixels(x, static_cast<int>(winY), 1, 1, 
                 GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, 
                 &posX, &posY, &posZ);

    vec[0] = posX;  vec[1] = posY;  vec[2] = posZ;
}

static void convert(int mx, int my, double &vx, double &vy)
{
	 //  Determine viewport coordinates
    double mouse_vector[3];
    GetOGLPos(mx, my, mouse_vector);
    vx = mouse_vector[0];
	vy = mouse_vector[1];

}

/**
 *  Global inialization function, called once per program
 *  execution.  This version allows the client to specify a
 *  initial display mode.  All SGL programs should call this
 *  function or its overloaded equivalent before creating 
 *  and using a graphical window.
 *  @param mode the initial display mode
 *  @return nothing
 */  
void sgl::initialize_graphics(unsigned int mode)
{
	if ( !glut_active )
	{
		//  Make up a dummy program name and pass no additional arguments
		static int argc = 1;

		//static char *argv[] = { "sgl_program" };  // Warning in Linux

        //  This works for both Visual C++ and Linux
        /*
        char s[20];
        strncpy(s, "sgl_program", 15);
        static char *argv[] = { s };
        */

        static char *argv[] = { const_cast<char *>("sgl_program") };



		glutInit(&argc, argv);
		glutInitDisplayMode(mode);
		glut_active = true;
	}
	else
	{
		cout << "Graphics environment already initialized" << endl;
		exit(1);
	}
}

/**
 *  Global inialization function, called once per program
 *  execution.  This version provides double buffering,
 *  RGBA mode, and no depth buffer.
 *  All SGL programs should call this
 *  function or its overloaded equivalent before creating 
 *  and using a graphical window.
 *  @return nothing
 */  
void sgl::initialize_graphics()
{
    sgl::initialize_graphics(GLUT_DOUBLE | GLUT_RGB);
}


/* ***********
//  Global inialization function, called once per program execution
void initialize_graphics()
{
	if ( !glut_active )
	{
		//  Make up a dummy program name and pass no additional arguments
		static int argc = 1;
		static char *argv[] = { "sgl_program" };
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
		glut_active = true;
	}
	else
	{
		cout << "Graphics environment already initialized" << endl;
		exit(1);
	}
}
******************** */


//  Prints the version number of SGL
const char *sgl::version()
{
    return SGL_VERSION_NUMBER;
	//cout << "0.7.0" << endl;
}


/*****************************************************
 *  Global GLUT callback functions
 *****************************************************/

static void sgl_display()
{
	/*if ( current_window )
		current_window->paint_all();*/
	if ( window_list[glutGetWindow()] )
		window_list[glutGetWindow()]->paint_all();
}

static void sgl_reshape(int w, int h)
{
	//current_window->resized(w, h);
	window_list[glutGetWindow()]->resized(w, h);
}

static void sgl_mouse_dragged(int x, int y)
{
	double vx, vy;
	convert(x, y, vx, vy);
	//current_window->mouse_dragged(vx, vy);
	window_list[glutGetWindow()]->mouse_dragged(vx, vy);
}


static void sgl_mouse_moved(int x, int y)
{
	double vx, vy;
	convert(x, y, vx, vy);
	//current_window->mouse_moved(vx, vy);
	//window_list[glutGetWindow()]->mouse_moved(vx, vy);
	window_list[glutGetWindow()]->mouse_moved(vx, vy);
}

static void sgl_mouse_entry(int state)
{
    if ( state == GLUT_LEFT )
        window_list[glutGetWindow()]->mouse_exited();
    else
        window_list[glutGetWindow()]->mouse_entered();
}


static void sgl_timer_expired(int win_number)
{
	//current_window->timer_expired(msec);
	//cout << "Sending a timer message to window #" 
	//	 << glutGetWindow() << " (" << win_number << ")" << endl;
	//window_list[glutGetWindow()]->timer_expired(msec);
	window_list[win_number]->timer_expired();
}


static void sgl_mouse_button(int button, int state, int x, int y)
{
	double vx, vy;
	convert(x, y, vx, vy);
	//cout << "(" << vx << "," << vy << ")" << endl;

	sgl::Window *win = window_list[glutGetWindow()];

    win->set_key_modifiers(static_cast<sgl::KeyModifier>(glutGetModifiers()));

	if ( state == GLUT_DOWN )
		win->mouse_pressed(vx, vy, static_cast<sgl::MouseButton>(button));
	else if ( state == GLUT_UP )
		win->mouse_released(vx, vy, static_cast<sgl::MouseButton>(button));
}



static void sgl_key_pressed(unsigned char k, int x, int y)
{
	double vx, vy;
	convert(x, y, vx, vy);
	sgl::Window *win = window_list[glutGetWindow()];
	win->set_key_modifiers(static_cast<sgl::KeyModifier>(glutGetModifiers()));
	win->key_pressed(k, vx, vy);
}

static void sgl_special_key_pressed(int k, int x, int y)
{
	double vx, vy;
	convert(x, y, vx, vy);
	sgl::Window *win = window_list[glutGetWindow()];
	win->set_key_modifiers(static_cast<sgl::KeyModifier>(glutGetModifiers()));
	win->key_pressed(k + 10000, vx, vy);
	//current_window->special_key_pressed(k, vx, vy);
}


/*****************************************************
 *  Global constant Color objects
 *****************************************************/

const sgl::Color sgl::BLACK(0.0, 0.0, 0.0);
const sgl::Color sgl::RED(1.0, 0.0, 0.0);
const sgl::Color sgl::GREEN(0.0, 1.0, 0.0);
const sgl::Color sgl::BLUE(0.0, 0.0, 1.0);
const sgl::Color sgl::YELLOW(1.0, 1.0, 0.0);
const sgl::Color sgl::MAGENTA(1.0, 0.0, 1.0);
const sgl::Color sgl::CYAN(0.0, 1.0, 1.0);
const sgl::Color sgl::DARK_RED(0.75, 0.0, 0.0);
const sgl::Color sgl::DARK_GREEN(0.0, 0.75, 0.0);
const sgl::Color sgl::DARK_BLUE(0.0, 0.0, 0.75);
const sgl::Color sgl::LIGHT_RED(1.0, 0.75, 0.75);
const sgl::Color sgl::LIGHT_GREEN(0.75, 1.0, 0.75);
const sgl::Color sgl::LIGHT_BLUE(0.75, 0.75, 1.0);
const sgl::Color sgl::GRAY(0.5, 0.5, 0.5);
const sgl::Color sgl::LIGHT_GRAY(0.8, 0.8, 0.8);
const sgl::Color sgl::DARK_GRAY(0.2, 0.2, 0.2);
const sgl::Color sgl::WHITE(1.0, 1.0, 1.0);


/*****************************************************
 *  Graphical object code
 *****************************************************/


void sgl::Window::initialize(const char *title, int left, int top, int width, int height, 
				          double min_x, double max_x, double min_y, double max_y)
{
    //  Check for proper window dimensions
    if ( max_x - min_x <= 0 || max_y - min_y <= 0 )
        cout << "WARNING: Horizontal and/or vertical window dimensions "
             << "zero or negative (check constructor arguments)"
             << endl;
	//  Must initialize graphics for the first window created if
    //  it has not been done already
	if ( !glut_active )
		initialize_graphics();

	glutInitWindowSize(width, height);
	glutInitWindowPosition(left, top);
	//cout << "Creating window #" << glutCreateWindow(title) << endl;
	glutCreateWindow(title);
	glClearColor(1.0, 1.0, 1.0, 0.0);  //  White background
	glShadeModel(GL_FLAT);

	//  Set window dimension globals
	this->min_x = min_x;
	this->min_y = min_y;
	this->max_x = max_x;
	this->max_y = max_y;

	set_viewport(min_x, min_y, max_x, max_y);


	glutDisplayFunc(sgl_display);
	glutReshapeFunc(sgl_reshape);
	glutMotionFunc(sgl_mouse_dragged);
	glutPassiveMotionFunc(sgl_mouse_moved);
	glutMouseFunc(sgl_mouse_button);
	glutEntryFunc(sgl_mouse_entry);
	glutKeyboardFunc(sgl_key_pressed);
	glutSpecialFunc(sgl_special_key_pressed);

    normal_cursor = CURSOR_RIGHT_ARROW;
    set_cursor(normal_cursor);

	//current_window = this;
	window_list[glutGetWindow()] = this;
}


/*
 *  Constructor
 *      Creates a graphics window with a its left-top corner at (left, top) in 
 *      screen coordinates.  Its width is width, and its height is height.  The viewport
 *      it represents extends from min_x to max_y horizontally and from min_y to max_y vertically.
 */
sgl::Window::Window(const char *title, int left, int top, int width, int height, 
				   double min_x, double max_x, double min_y, double max_y)
{
	initialize(title, left, top, width, height, min_x, max_x, min_y, max_y);
}

sgl::Window::Window(const char *title, int width, int height) 
{
	initialize(title, 100, 100, width, height, 0.0, width, 0.0, height);
}

sgl::Window::Window(const char *title, double min_x, double max_x, double min_y, double max_y) 
{
    //  Compute the window's size
    double width = max_x - min_x,
           height = max_y - min_y;
    int window_width, window_height;
    //  Prevent the creation of a window with zero or negative
    //  dimensions
    if ( width <= 0 || height <= 0 )
    {
        cout << "Window vertical or horizontal dimension zero "
             << "or negative; check order of constructor "
             << "arguments" << endl;
        exit(1);
    }
    //  Scale physical window size so the longest side is 800 pixels
    if ( width > height )  //  window is shorter and wider
    {
        window_width = 800;  
        //  Adjust aspect ratio
        window_height = static_cast<int>(800*height/width + 0.5);

    }
    else  //  Window is longer and taller
    {
        window_height = 800;
        //  Adjust aspect ratio
        window_width = static_cast<int>(800*width/height + 0.5);
    }

	initialize(title, 100, 100, window_width, window_height, 
               min_x, max_x, min_y, max_y);
}

sgl::Window::Window()
{
	initialize("", 100, 100, 800, 600, 0.0, 0.0, 800.0, 600.0);
}

sgl::Window::~Window()
{
	//current_window = 0;
	window_list[glutGetWindow()] = 0;
	//object_list.clear();
}

/**
 *  Sets the location of the left-top corner of the window's
 *  client area to (x,y).
 *  @param x the x coordinate of the window's left-top corner
 *  @param y the y coordinate of the window's left-top corner
 *  @return nothing
 */
void sgl::Window::set_position(int x, int y)
{
    glutPositionWindow(x, y);
}

/**
 *  Sets the physical window's hoizontal and vertical
 *  dimensions.
 *  @param width the new width of the window
 *  @param height the new height of the window
 *  @return nothing
 */
void sgl::Window::set_size(int width, int height)
{
    glutReshapeWindow(width, height);
}

/**
 *  Shows or hides the window.
 *  viewport.  Values given are in viewport's coordinates.
 *  @param visible if true, the window is visible;
 *                 otherwise, the window is invisible.
 *  @return nothing
 */
void sgl::Window::set_visible(bool visible)
{
    if ( visible )
        glutShowWindow();
    else
        glutHideWindow();
}

/**
 *  Sets the window's cursor shape
 *  @param cursor one of several CursorShape values
 *  @return nothing
 */
sgl::CursorShape sgl::Window::set_cursor(sgl::CursorShape cursor)
{

    sgl::CursorShape previous_cursor =
           static_cast<sgl::CursorShape>(glutGet(GLUT_WINDOW_CURSOR));
    glutSetCursor(cursor);
    return previous_cursor;
}

/**
 *  Gets the window's curent cursor shape
 *  @return the window's current cursor shape
 */
sgl::CursorShape sgl::Window::get_cursor()
{
    return static_cast<CursorShape>(glutGet(GLUT_WINDOW_CURSOR));
}


void sgl::Window::set_viewport(double left, double right, 
                               double bottom, double top)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);
}

void sgl::Window::set_background_color(const Color& color)
{
    glClearColor(static_cast<GLclampf>(color.red), 
		         static_cast<GLclampf>(color.green), 
				 static_cast<GLclampf>(color.blue), 
				 0.0);
}

/*
 *  Start the graphics event loop.  Control is passed to the 
 *  event manager and the event manager maintains control for the
 *  rest of the program's execution.  This method should be called
 *  only once.
 */
void sgl::Window::run()
{
	if ( !event_loop_running )
	{
		event_loop_running = true;
		glutMainLoop();
	}
	else
		cout << "Event loop already running, action ignored" << endl;
}


/*
 *  draw_axes
 *      Draws the x- and y-axes within the viewport using the current drawing color.
 */
void sgl::Window::draw_axes(double x_inc, double y_inc) const
{
    //  Draw grid lines at 10 unit increments
    glColor3d(0.9, 0.9, 1.0);  // Light blue
    for ( double x = 0 + x_inc; x < max_x; x += x_inc )
        draw_line(x, min_y, x, max_y);
    for ( double y = 0 + y_inc; y < max_y; y += y_inc )
        draw_line(min_x, y, max_x, y);
    for ( double x = -x_inc; x > min_x; x -= x_inc )
        draw_line(x, min_y, x, max_y);
    for ( double y = -y_inc; y > min_y; y -= y_inc )
        draw_line(min_x, y, max_x, y);
    //  Draw axes
    glColor3d(0.0, 0.0, 0.0);   //  Black
    draw_line(min_x, 0.0, max_x, 0.0);  //  x-axis
    draw_line(0.0, min_y, 0.0, max_y);  //  y-axis
    draw_line(min_x, 0.0, min_x + 5.0, -2.0);  //  x-axis left arrow head
    draw_line(min_x, 0.0, min_x + 5.0, 2.0);
    draw_line(max_x, 0.0, max_x - 5.0, -2.0);  //  x-axis right arrow head
    draw_line(max_x, 0.0, max_x - 5.0, 2.0);
    draw_line(0.0, min_y, -2.0, min_y + 5.0);   //  y-axis bottom arrow head
    draw_line(0.0, min_y, 2.0, min_y + 5.0);
    draw_line(0.0, max_y, -2.0, max_y - 5.0);   //  y-axis top arrow head
    draw_line(0.0, max_y, 2.0, max_y - 5.0);
}



/*
 *  clear_viewport
 *      Sets the viewport to its background color, erasing all drawings within it.
 */
void sgl::Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void sgl::Window::set_title(const char *str)
{
    glutSetWindowTitle(str);
}

void sgl::Window::repaint()
{
	glutPostRedisplay();
}

void sgl::Window::paint_all()
{
	prepaint();

	paint();

	postpaint();
}

void sgl::Window::prepaint()
{
	glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
	glColor3d(0.0, 0.2, 0.0);
}

void sgl::Window::postpaint()
{
	//list<GraphicalObject *>::const_iterator p;
    //for ( p = object_list.begin(); p != object_list.end(); p++ )
    //    (*p)->paint();
	glPopMatrix();
	//glFlush();
	glutSwapBuffers();
}


//  Returns the x value of the left, top corner of the window
int sgl::Window::get_x() const
{
	return glutGet(GLUT_WINDOW_X);
}

//  Returns the y value of the left, top corner of the window
int sgl::Window::get_y() const
{
	return glutGet(GLUT_WINDOW_Y);
}

//  Returns the width of the window
int sgl::Window::get_width() const
{
	return glutGet(GLUT_WINDOW_WIDTH);
}

//  Returns the height of the window
int sgl::Window::get_height() const
{
	return glutGet(GLUT_WINDOW_HEIGHT);
}

double sgl::Window::get_min_x() const
{
	return min_x;
}

double sgl::Window::get_max_x() const
{
	return max_x;
}

double sgl::Window::get_min_y() const
{
	return min_y;
}

double sgl::Window::get_max_y() const
{
	return max_y;
}

void sgl::Window::set_window_size(int w, int h)
{
	glutReshapeWindow(w, h);
}

void sgl::Window::resized(int w, int h)
{
    glutInitWindowSize(w, h);
    glViewport (0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(min_x, max_x, min_y, max_y, -1.0, 1.0);
	gluOrtho2D(min_x, max_x, min_y, max_y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void sgl::Window::mouse_moved(double, double) {}

void sgl::Window::mouse_dragged(double, double) {}

void sgl::Window::mouse_pressed(double, double, sgl::MouseButton) {}

void sgl::Window::mouse_released(double, double, sgl::MouseButton) {}

/**
 *  Called by the event manager when the mouse pointer
 *  enters the window.
 *  @return nothing
 */
void sgl::Window::mouse_entered() {}

/**
 *  Called by the event manager when the mouse pointer
 *  exits the window.
 *  @return nothing
 */
void sgl::Window::mouse_exited() {}



/*
 *  Alt-F4 closes the window and terminates the program.  Derived classes
 *  should call this version to ensure consistent behavior.
 */
void sgl::Window::key_pressed(int key, double, double) //(unsigned char key, int, int)
{
	if ( key == F4_KEY && glutGetModifiers() == GLUT_ACTIVE_ALT )
		exit(0);
		//if ( glutGetWindow() == 1 )
		//	exit(0);   //  Alt-F4 terminates the program
		//else
		//{
		//	int window_to_close = glutGetWindow();
		//	window_list[window_to_close] = 0;  //  Nullify window
		//	cout << "Window to close: " << window_to_close << endl;
		//	glutSetWindow(1);  //  Reset current window to main window
		//	cout << "Current window is now: " << glutGetWindow() << endl;
		//	glutDestroyWindow(window_to_close);
		//}
    //cout << "Pressed a key" << endl;
    //glutPostRedisplay();
}

///*
// *  Alt-F4 closes the window and terminates the program.
// */
//void sgl::Window::special_key_pressed(int key, double, double)
//{
//    if ( key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT )
//        exit(0);   //  Alt-F4 terminates the program
//}

sgl::KeyModifier sgl::Window::get_key_modifiers() const
{
	return key_mods;
}

void sgl::Window::set_key_modifiers(sgl::KeyModifier mods)
{
	key_mods = mods;
}

void sgl::Window::start_timer(int msec)
{
	glutTimerFunc(msec, sgl_timer_expired, glutGetWindow());
}

//  Derived classes add custom behavior
void sgl::Window::timer_expired() {}



/*****************************************************
 *  ObjectWindow code
 *****************************************************/

/*
 *  Constructor
 *      Creates a graphics window with a its left-top corner at (left, top) in 
 *      screen coordinates.  Its width is width, and its height is height.  The viewport
 *      it represents extends from min_x to max_y horizontally and from min_y to max_y vertically.
 */
sgl::ObjectWindow::ObjectWindow(const char *title, 
                                int left, int top, 
                                int width, int height, 
				                double min_x, double max_x, 
                                double min_y, double max_y):
                sgl::Window(title, left, top, width, height,
                       min_x, max_x, min_y, max_y),
                active_object(0) {}

sgl::ObjectWindow::ObjectWindow(const char *title, 
                                int width, int height):
                sgl::Window(title, width, height),
                active_object(0) {}

sgl::ObjectWindow::ObjectWindow(const char *title, 
                                double min_x, double max_x, 
                                double min_y, double max_y):
                sgl::Window(title, min_x, max_x, min_y, max_y),
                active_object(0) {}

sgl::ObjectWindow::ObjectWindow(): sgl::Window() {}


sgl::ObjectWindow::~ObjectWindow()
{
	window_list[glutGetWindow()] = 0;
	object_list.clear();
}

void sgl::ObjectWindow::prepaint()
{
    Window::prepaint();
}

void sgl::ObjectWindow::postpaint()
{
	list<GraphicalObject *>::const_iterator p;
    for ( p = object_list.begin(); p != object_list.end(); p++ )
        (*p)->paint();
    Window::postpaint();
}

void sgl::ObjectWindow::mouse_pressed(double x, double y, 
                                      sgl::MouseButton button)
{
    if ( active_object )
    {
        active_object->mouse_pressed(x, y, button);
        repaint();
    }
    sgl::Window::mouse_pressed(x, y, button);
}

void sgl::ObjectWindow::mouse_released(double x, double y, 
                                       sgl::MouseButton button)
{
    if ( active_object )
    {
        active_object->mouse_released(x, y, button);
        repaint();
    }
    sgl::Window::mouse_released(x, y, button);
}

void sgl::ObjectWindow::mouse_moved(double x, double y)
{
    GraphicalObject *prev_active = active_object;
    active_object = hit(x, y);
    if ( prev_active != active_object )
    {
        if ( prev_active )
        {
            prev_active->set_mouse_over(false);
            set_cursor(normal_cursor);
        }
        if ( active_object )
        {
            active_object->set_mouse_over(true);
            active_object->mouse_moved(x, y);
            set_cursor(active_object->get_cursor());
        }
        repaint();
    }
}

void sgl::ObjectWindow::mouse_dragged(double x, double y)
{
    if ( active_object )
    {
        active_object->mouse_dragged(x, y);
        repaint();
    }
}

void sgl::ObjectWindow::key_pressed(int key, double x, double y)
{
    if ( active_object )
    {
        active_object->key_pressed(key, x, y);
        repaint();
    }
    sgl::Window::key_pressed(key, x, y);
}


// Add the given graphical object obj to the display list
void sgl::ObjectWindow::add(GraphicalObject *obj)
{
    //cout << "In add" << endl;
    //cout << static_cast<void *>(obj) << endl;
    //cout << static_cast<void *>(this) << endl;
    object_list.push_back(obj);
	obj->set_window(this);
}

//  Remove the given graphical object obj from the display list
void sgl::ObjectWindow::remove(GraphicalObject *obj)
{
    object_list.remove(obj);
	repaint();
}

//  Remove all the graphical objects from the display list
//  Frees up the memory allocated for the object
void sgl::ObjectWindow::remove_all()
{
	for ( list<sgl::GraphicalObject *>::iterator p = object_list.begin();
          p != object_list.end(); p++ )
        delete *p;
    object_list.clear();
	repaint();
}

//  Return the first graphical object in the display list that is hit
//  by the given point (x,y).  If no objects in the list intersect the 
//  given point, return null.
sgl::GraphicalObject *sgl::ObjectWindow::hit(double x, double y) const
{
    for ( list<sgl::GraphicalObject *>::const_reverse_iterator p = object_list.rbegin();
          p != object_list.rend(); p++ )
        if ( (*p)->hit(x, y) )
            return *p;
    return 0;   //  No object hit
}

/**
 *  Returns an iterator to the begining of the vector of 
 *  graphical objects this window contains.
 *  @return  an iterator to the begining of the vector of 
 *           graphical objects this window contains.
 */
list<sgl::GraphicalObject *>::iterator sgl::ObjectWindow::begin()
{
    return object_list.begin();
}

/**
 *  Returns an iterator just past the end of the vector of 
 *  graphical objects this window contains.
 *  @return  an iterator just past the end of the vector of 
 *           graphical objects this window contains.
 */
list<sgl::GraphicalObject *>::iterator sgl::ObjectWindow::end()
{
    return object_list.end();
}


/*****************************************************
 *  Pixmap code
 *****************************************************/

/**
 *  The Pixmap class stores the number of rows and columns 
 *  in the pixmap, as well as the address of the first 
 *  pixel in memory.
 *  The implementation of the Pixmap class is adapted from 
 *  code found in  rgbpixmap.h from Hill's textbook 
 *  "Computer Graphics Using OpenGL" attributed to
 *  J. J. Rajnovich, June 05, 2002
 */

/**
 *  Helper function for extracting an unsigned short
 *  value from a binary file.
 *  @param fin the file stream object providing
 *         the input.
 *  @return the value extracted from the input stream
 */
unsigned short sgl::Pixmap::get_short(ifstream& fin)
{ 
	// BMP format uses little-endian integer types
	// get a 2-byte integer stored in little-endian form
	char ic;
	unsigned short ip;

	fin.get(ic);
	
	//first byte is little one 
	ip = ic;

	fin.get(ic);

	// or in high order byte
	ip |= ((unsigned short)ic << 8);

	return ip;
}

/**
 *  Helper function for extracting an unsigned long
 *  value from a binary file.
 *  @param fin the file stream object providing
 *         the input.
 *  @return the value extracted from the input stream
 */
unsigned long sgl::Pixmap::get_long(ifstream& fin)
{
	//BMP format uses little-endian integer types
	// get a 4-byte integer stored in little-endian form
	unsigned long ip = 0;
	char ic = 0;
	unsigned char uc = ic;

	fin.get(ic); 
	ip = uc = ic;

	fin.get(ic);
	uc = ic; 
	ip |=((unsigned long)uc << 8);

	fin.get(ic);
	uc = ic;
	ip |=((unsigned long)uc << 16);

	fin.get(ic); 
    uc = ic;
	ip |=((unsigned long)uc << 24);

	return ip;
}

/**
 *  Helper function for determining if an integer
 *  in a power of two.
 *  @param fin the file stream object providing
 *         the input.
 *  @return true, if n is an exact binary; otherwise, false
 */
bool sgl::Pixmap::power_of_2(int n)
{
    return n == 1 || n == 2 || n == 4 || n == 8 || n == 16
           || n == 32 || n ==64 || n == 128 || n == 256
           || n == 512 || n == 1024 || n == 2048 || n == 4096
           || n == 8192 || n == 16384 || n == 32768 
           || n == 65536 || n == 131072 || n == 262144
           || n == 524288 || n == 1048576 || n == 2097152
           || n == 4194304 || n == 8388608 || n == 16777216;
}

/**
 *  Read a BMP file into this pixmap.
 *  @param fname the name of the BMP file
 *         the read in.
 *  @return true, if the operation was 
 *          successful; otherwise, false.
 */
bool sgl::Pixmap::read_BMP_file(const char *fname) 
{  
    ifstream fin;

    bool result = false;  //  Unsuccessful by default

	// Read into memory an mRGB image from an uncompressed BMP file.
	// return false on failure and true on success

	// open input file to read binary char's
	fin.open( fname, ios::in|ios::binary );

    if ( !fin.good() )  //  File opened successfully?
        cout << " can't open file: " << fname << endl;
    else
    {   //  Continue if file was opened sucessfully
        //  First, load all the preliminary file information
        fin.get();  // Read the file header information
        fin.get();  //type: always 'BM'
        get_long(fin);                                //  file size
        get_short(fin);	                              // reserved, always 0
        get_short(fin);                               // reserved, always 0 
        get_long(fin);	                              // offset to image - unreliable
        get_long(fin);                                // header size, always 40
        unsigned long numCols = get_long(fin);	      // number of columns in image
        unsigned long numRows = get_long(fin);	      // number of rows in image
        get_short(fin);                               // number of planes, always 1 
        unsigned short bitsPerPixel = get_short(fin); // 8 or 24; allow 24 here
        get_long(fin);                                // compression, must be 0 for uncompressed 
        get_long(fin);                                // total bytes in image 
        get_long(fin);                                // x pels, always 0 
        get_long(fin);                                // y pels, always 0 
        get_long(fin);                                // Number of LUT entries, 256 for 8 bit, otherwise 0 
        get_long(fin);                                // impColors, always 0 

        if ( bitsPerPixel != 24 )   // Error - must be a 24 bit uncompressed image
            cout << "Not a 24 bit/pixelimage, or is compressed!" << endl;
        else
        {   //  Bits per pixel OK
            //  Check for length of sides--must be a power of two
            if ( !power_of_2(numRows) || !power_of_2(numCols) ) 
                cout << "Length of sides must be powers of two" << endl;
            else
            {   // add bytes at end of each row so total # is a multiple of 4
                // round up 3*numCols to next multiple of 4
                int nBytesInRow = ((3 * numCols + 3)/4) * 4,
                    numPadBytes = nBytesInRow - 3 * numCols;	// need this many

                // set class's data members
                nRows = numRows;
                nCols = numCols;

                //make space for array
                pixel = new RGB[nRows * nCols];

                if ( !pixel )  // Check for proper memory allocation
                    cout << "Out of memory!" << endl;
                else
                {
                    long count = 0;
                    for ( int row = 0; row < nRows; row++ ) // read pixel values
                    {
                        for ( int col = 0; col < nCols; col++ )
                        {
                            char r, g, b;
                            // Read three bytes ...
                            fin.get(b);
                            fin.get(g); 
                            fin.get(r);
                            // ... and place the RGB values
                            pixel[count].red   = r;
                            pixel[count].green = g;
                            pixel[count].blue  = b;
                            count++;
                        }
                        // Skip pad bytes at row's end
                        for( int k = 0 ; k < numPadBytes ; k++ )
                            fin.get();
                    }
                    //  All went OK; can register success
                    result = true;
                }
            }
        }
        fin.close();    
    }
    return result;
}

/**
 *  Creates a checkerboard pixmap.
 *  @return true if successful and false
 *          if no memory was available
 *          for the pixmap.
 */
bool sgl::Pixmap::make_checkerboard()
{  
    bool result = false;  //  Unsuccessful by default
	// make a checkerboard patten
	nRows = nCols = 64;
	pixel = new RGB[3 * nRows * nCols];
 
    if ( !pixel )
    {   //  Unable to allocate memory
        cout << "out of memory!" << endl;
    }
    else
    {
        long count = 0;
        for ( int i = 0; i < nRows; i++ )
        {
            for ( int j = 0; j < nCols; j++ )
            {
                unsigned char c = static_cast<unsigned char>((((i/8) + (j/8)) %2) * 255);  
                pixel[count].red   = c; 	// red
                pixel[count].green = c; 	// green
                pixel[count].blue  = 0; 	// blue
                count++;
            }
        }
        result = true;
    }
    return result;
}

void sgl::Pixmap::set_texture(GLuint textureName)
{
	glBindTexture(GL_TEXTURE_2D, textureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nCols, nRows, 0, GL_RGB, 
                  GL_UNSIGNED_BYTE, pixel);
}




/*****************************************************
 *  BitmapObject code
 *****************************************************/

/**
 *  Constructor 
 *  @param filename the BMP file containing the image to 
 *         display.
 *  @param x the x coordinate of the bitmap image's 
 *           left-bottom corner
 *  @param y the y coordinate of the bitmap image's 
 *           left-bottom corner
 *  @param width the bitmap image's width
 *  @param height the bitmap image's height
 */
sgl::BitmapObject::BitmapObject(const char *filename, double x, double y, 
       double width, double height): sgl::GraphicalObject(x, y, width, height)
{
    pix.read_BMP_file(filename);
    glGenTextures(1, &texture_id);
    pix.set_texture(texture_id);    // create texture
}

/**
 *  Draws the bitmap image within the window.
 */
void sgl::BitmapObject::paint() const
{
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id); // top face: Ennis 
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex2d(left, bottom);
    glTexCoord2f(1.0, 0.0); glVertex2d(left + width, bottom);
    glTexCoord2f(1.0, 1.0); glVertex2d(left + width, bottom + height);
    glTexCoord2f(0.0, 1.0); glVertex2d(left, bottom + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

/*****************************************************
 *  CompositeObject code
 *****************************************************/

/**
 *  The constructor makes an initially empty 
 *  container.
 */
sgl::CompositeObject::CompositeObject(): 
     GraphicalObject(0.0, 0.0, 0.0, 0.0), 
     objects(0)
{}

/**
 *  Draws each object that makes up the composite
 *  object.
 *  @return nothing.
 */
void sgl::CompositeObject::paint() const
{
    //  Debugging: Draw container's bounding box
    set_color(BLUE);
    draw_rectangle(left, bottom, width, height);
    //  Draw each object in this container
    vector<GraphicalObject *>::const_iterator iter;
    for ( iter = objects.begin(); iter != objects.end(); ++iter )
        (*iter)->paint();
}

/**
 *  Repositions the lower-left corner of the composite object's
 *  boundng box to (x,y).  The positions of the contained
 *  graphical objects are updated accordingly.
 */
void sgl::CompositeObject::move_to(double x, double y)
{
    double old_left = left, old_bottom = bottom;
    GraphicalObject::move_to(x, y);
    double dx = left - old_left, dy = bottom - old_bottom;
    vector<GraphicalObject *>::const_iterator iter;
    for ( iter = objects.begin(); iter != objects.end(); ++iter )
    {
        GraphicalObject *obj = *iter; 
        obj->move_to(obj->get_left() + dx, obj->get_bottom() + dy);
    }
}

/**
 *  Adds a graphical object to this container.  Adjusts
 *  the container's size accordingly.
 *  @param obj a pointer to the graphical object to add.
 *  @return nothing.
 */
void sgl::CompositeObject::add(sgl::GraphicalObject *obj)
{
    //  Adjust this container's size, if necessary
    //  to accommodate the new element.
    if ( objects.size() == 0 )  //  Add first element
    {
        left = obj->get_left();
        bottom = obj->get_bottom();
        width = obj->get_width();
        height = obj->get_height();
    }
    else //  Contains at least one element
    {
        double new_left = (obj->get_left() < left)? obj->get_left() : left,
               new_bottom = (obj->get_bottom() < bottom)? obj->get_bottom() : bottom,
               right = left + width, 
               obj_right = obj->get_left() + obj->get_width(),
               top = bottom + height,
               obj_top = obj->get_bottom() + obj->get_height();
        left = new_left;
        bottom = new_bottom;
        width = (obj_right > right)? obj_right - left : right - left;
        height = (obj_top > top)? obj_top - bottom : top - bottom;
    }

    //  Place object in this container
    objects.push_back(obj);
}



/*****************************************************
 *  OGLWindow code
 *****************************************************/

/**
 *  Constructor that provides the most control to the client.
 *  Creates a window with detailed information.
 *  @param title the text to appear within the window's titlebar
 *  @param left the x coordinate in screen coordinates of the window's 
 *         left-top corner 
 *  @param top the y coordinate in screen coordinates of the window's 
 *         left-top corner  
 *  @param width the width in screen coordinates of the window
 *  @param height the height in screen coordinates of the window
 */
sgl::OGLWindow::OGLWindow(const char *title, int left, int top, 
                          int width, int height): 
                   sgl::Window(title, left, top, width, height) {}

/**
 *  Creates a window with a default position and size.
 *  @param title the text to appear within the window's titlebar
 */
sgl::OGLWindow::OGLWindow(const char *title): 
                 sgl::Window(title, 0.0, 0.0, 0.0, 0.0) {}
/**
 *  Destroys a graphical window object.
 */
sgl::OGLWindow::~OGLWindow() {}

/**  
 *  Code executed before the call to paint
 *  @return nothing
 */
void sgl::OGLWindow::prepaint() {}

/**
 *  Code executed after the call to paint
 *  @return nothing
 */
void sgl::OGLWindow::postpaint() {}


/*****************************************************
 *  Seven segment display code
 *****************************************************/

//  Older, vector-based version

//class SevenSegmentDigit
//{
//protected:
//    int current_value;
//
//    Color color;
//    double x, y;    //  (x,y) coordinates of lower-left corner of the LED
//    double height;  //  The height of the LED
//    
//    //  Draw the segments:
//    //        +--- a ---+
//    //        |         |
//    //        f         b
//    //        |         |
//    //        +--- g ---+
//    //        |         |
//    //        e         c
//    //        |         |
//    //        +--- d ---+
//
//    vector<Point> segment_a,
//                  segment_b,
//                  segment_c,
//                  segment_d,
//                  segment_e,
//                  segment_f,
//                  segment_g;
//
//   
//    void update_segment_locations()
//    {
//        double width = height/2.0,
//               max_x = x + width, 
//               max_y = y + height,
//               mid_y = y + width,
//               mid_left_x = x + 0.1*width,
//               mid_right_x = x + 0.9 * width,
//               top_left_x = x + 0.2 * width,
//               bottom_right_x = x + 0.8 * width,
//               offset = 0.12 * width,
//               half_offset = 0.5 * offset,
//               inc = 0.3 * offset;
//              
//
//        //  Clear all the segments
//        segment_a.clear();
//        segment_b.clear();
//        segment_c.clear();
//        segment_d.clear();
//        segment_e.clear();
//        segment_f.clear();
//        segment_g.clear();
//
//        //  Make segment a
//        segment_a.push_back(Point(top_left_x, max_y));
//        segment_a.push_back(Point(max_x, max_y));
//        segment_a.push_back(Point(max_x - offset, max_y - offset));
//        segment_a.push_back(Point(top_left_x + offset, max_y - offset));
//
//        //  Make segment b
//        segment_b.push_back(Point(max_x, max_y - inc));
//        segment_b.push_back(Point(mid_right_x, mid_y + inc));
//        segment_b.push_back(Point(mid_right_x - offset, mid_y + offset - inc));
//        segment_b.push_back(Point(max_x - offset, max_y - offset - inc));
//
//        //  Make segment c
//        segment_c.push_back(Point(mid_right_x, mid_y - inc));
//        segment_c.push_back(Point(bottom_right_x, y + inc));
//        segment_c.push_back(Point(bottom_right_x - offset, y + offset + inc));
//        segment_c.push_back(Point(mid_right_x - offset, mid_y - offset + inc));
//
//        //  Make segment d
//        segment_d.push_back(Point(x, y));
//        segment_d.push_back(Point(bottom_right_x, y));
//        segment_d.push_back(Point(bottom_right_x - offset, y + offset));
//        segment_d.push_back(Point(x + offset, y + offset));
//
//        //  Make segment e
//        segment_e.push_back(Point(x, y + inc));
//        segment_e.push_back(Point(mid_left_x, mid_y - inc));
//        segment_e.push_back(Point(mid_left_x + offset, mid_y - offset + inc));
//        segment_e.push_back(Point(x + offset, y + offset + inc));
//
//        //  Make segment f
//        segment_f.push_back(Point(mid_left_x, mid_y + inc));
//        segment_f.push_back(Point(top_left_x, max_y - inc));
//        segment_f.push_back(Point(top_left_x + offset, max_y - offset - inc));
//        segment_f.push_back(Point(mid_left_x + offset, mid_y + offset - inc));
//
//        //  Make segment g
//        segment_g.push_back(Point(mid_left_x, mid_y));
//
//        segment_g.push_back(Point(mid_left_x + offset, mid_y + half_offset));
//        segment_g.push_back(Point(mid_right_x - offset, mid_y + half_offset));
//
//        segment_g.push_back(Point(mid_right_x, mid_y));
//
//        segment_g.push_back(Point(mid_right_x - offset, mid_y - half_offset));
//        segment_g.push_back(Point(mid_left_x + offset, mid_y - half_offset));
//    }
//    
//public:
//    SevenSegmentDigit(Color color, double x, double y, double height): 
//              current_value(0), color(color), x(x), y(y), height(height)
//    {
//        update_segment_locations();
//    }
//
//    double get_x() const
//    {
//        return x;
//    }
//
//    double get_y() const
//    {
//        return y;
//    }
//
//    double get_height() const
//    {
//        return height;
//    }
//
//    void paint() const
//    {
//        set_color(color);
//
//        switch ( current_value )
//        {
//        case 0:
//            fill_polygon(segment_a);
//            fill_polygon(segment_b);
//            fill_polygon(segment_c);
//            fill_polygon(segment_d);
//            fill_polygon(segment_e);
//            fill_polygon(segment_f);
//            break;
//        case 1:
//            fill_polygon(segment_b);
//            fill_polygon(segment_c);
//            break;
//        case 2:
//            fill_polygon(segment_a);
//            fill_polygon(segment_b);
//            fill_polygon(segment_d);
//            fill_polygon(segment_e);
//            fill_polygon(segment_g);
//            break;
//        case 3:
//            fill_polygon(segment_a);
//            fill_polygon(segment_b);
//            fill_polygon(segment_c);
//            fill_polygon(segment_d);
//            fill_polygon(segment_g);
//            break;
//        case 4:
//            fill_polygon(segment_b);
//            fill_polygon(segment_c);
//            fill_polygon(segment_f);
//            fill_polygon(segment_g);
//            break;
//        case 5:
//            fill_polygon(segment_a);
//            fill_polygon(segment_c);
//            fill_polygon(segment_d);
//            fill_polygon(segment_f);
//            fill_polygon(segment_g);
//            break;
//        case 6:
//            fill_polygon(segment_a);
//            fill_polygon(segment_c);
//            fill_polygon(segment_d);
//            fill_polygon(segment_e);
//            fill_polygon(segment_f);
//            fill_polygon(segment_g);
//            break;
//        case 7:
//            fill_polygon(segment_a);
//            fill_polygon(segment_b);
//            fill_polygon(segment_c);
//            break;
//        case 8:
//            fill_polygon(segment_a);
//            fill_polygon(segment_b);
//            fill_polygon(segment_c);
//            fill_polygon(segment_d);
//            fill_polygon(segment_e);
//            fill_polygon(segment_f);
//            fill_polygon(segment_g);
//            break;
//        case 9:
//            fill_polygon(segment_a);
//            fill_polygon(segment_b);
//            fill_polygon(segment_c);
//            fill_polygon(segment_d);
//            fill_polygon(segment_f);
//            fill_polygon(segment_g);
//            break;
//        }
//    }
//
//    void set_value(int value)
//    {
//        current_value = value % 10;
//    }
//
//    int get_value() const
//    {
//        return current_value;
//    }
//
//    void increment()
//    {
//        current_value = (current_value + 1) % 10;
//    }
//
//    void decrement()
//    {
//        current_value = (current_value == 0)? 9 : current_value - 1;
//    }
//
//    void resize(double inc)
//    {
//        height += inc;
//        update_segment_locations();
//    }
//
//    void move(double x, double y)
//    {
//        this->x = x;
//        this->y = y;
//        update_segment_locations();
//    }
//};



void sgl::SevenSegmentDigit::update_segment_locations()
{
    double width = height/2.0,
           max_x = x + width, 
           max_y = y + height,
           mid_y = y + width,
           mid_left_x = x + 0.1*width,
           mid_right_x = x + 0.9 * width,
           top_left_x = x + 0.2 * width,
           bottom_right_x = x + 0.8 * width,
           offset = 0.14 * width,
           half_offset = 0.5 * offset,
           inc = 0.3 * offset;
          
    //  Make segment a
    segment_a[0].x = top_left_x;              segment_a[0].y = max_y;
    segment_a[1].x = max_x;                   segment_a[1].y = max_y;
    segment_a[2].x = max_x - offset;          segment_a[2].y = max_y - offset;
    segment_a[3].x = top_left_x + offset;     segment_a[3].y = max_y - offset;

    //  Make segment b
    segment_b[0].x = max_x;                   segment_b[0].y = max_y - inc;
    segment_b[1].x = mid_right_x;             segment_b[1].y = mid_y + inc;
    segment_b[2].x = mid_right_x - offset;    segment_b[2].y =  mid_y + offset - inc;
    segment_b[3].x = max_x - offset;          segment_b[3].y = max_y - offset - inc;

    //  Make segment c
    segment_c[0].x = mid_right_x;             segment_c[0].y = mid_y - inc;
    segment_c[1].x = bottom_right_x;          segment_c[1].y = y + inc;
    segment_c[2].x = bottom_right_x - offset; segment_c[2].y = y + offset + inc;
    segment_c[3].x = mid_right_x - offset;    segment_c[3].y = mid_y - offset + inc;

    //  Make segment d
    segment_d[0].x = x;                       segment_d[0].y = y;
    segment_d[1].x = bottom_right_x;          segment_d[1].y = y;
    segment_d[2].x = bottom_right_x - offset; segment_d[2].y = y + offset;
    segment_d[3].x = x + offset;              segment_d[3].y = y + offset;

    //  Make segment e
    segment_e[0].x = x;                       segment_e[0].y = y + inc;
    segment_e[1].x = mid_left_x;              segment_e[1].y = mid_y - inc;
    segment_e[2].x = mid_left_x + offset;     segment_e[2].y = mid_y - offset + inc;
    segment_e[3].x = x + offset;              segment_e[3].y = y + offset + inc;

    //  Make segment f
    segment_f[0].x = mid_left_x;              segment_f[0].y = mid_y + inc;
    segment_f[1].x = top_left_x;              segment_f[1].y = max_y - inc;
    segment_f[2].x = top_left_x + offset;     segment_f[2].y = max_y - offset - inc;
    segment_f[3].x = mid_left_x + offset;     segment_f[3].y = mid_y + offset - inc;

    //  Make segment g
    segment_g[0].x = mid_left_x;              segment_g[0].y = mid_y;

    segment_g[1].x = mid_left_x + offset;     segment_g[1].y = mid_y + half_offset;
    segment_g[2].x = mid_right_x - offset;    segment_g[2].y = mid_y + half_offset;

    segment_g[3].x = mid_right_x;             segment_g[3].y = mid_y;

    segment_g[4].x = mid_right_x - offset;    segment_g[4].y = mid_y - half_offset;
    segment_g[5].x = mid_left_x + offset;     segment_g[5].y = mid_y - half_offset;
}

/**
 *  The constructor sets the digit's color, position, and size.
 *  @param color the color of the digit
 *  @param x the x coordinate of the digit's lower-left corner
 *  @param y the y coordinate of the digit's lower-left corner
 *  @param height the height of the digit
 */
sgl::SevenSegmentDigit::SevenSegmentDigit(sgl::Color color, double x, double y, double height): 
          current_value(0), color(color), x(x), y(y), height(height)
{
    update_segment_locations();
}

/**
 *  Returns the x coordinate of the digit's lower-left corner.
 *  @return the x coordinate of the digit's lower-left corner.
 */
double sgl::SevenSegmentDigit::get_x() const
{
    return x;
}

/**
 *  Returns the y coordinate of the digit's lower-left corner.
 *  @return the y coordinate of the digit's lower-left corner.
 */
double sgl::SevenSegmentDigit::get_y() const
{
    return y;
}

/**
 *  Returns the digit's height.
 *  @return the digit's height.
 */
double sgl::SevenSegmentDigit::get_height() const
{
    return height;
}

/**
 *  Draws the digit.
 */
void sgl::SevenSegmentDigit::paint() const
{
    set_color(color);

    switch ( current_value )
    {
    case 0:
        sgl::fill_polygon(segment_a, 4);
        sgl::fill_polygon(segment_b, 4);
        sgl::fill_polygon(segment_c, 4);
        sgl::fill_polygon(segment_d, 4);
        sgl::fill_polygon(segment_e, 4);
        sgl::fill_polygon(segment_f, 4);
        break;
    case 1:
        sgl::fill_polygon(segment_b, 4);
        sgl::fill_polygon(segment_c, 4);
        break;
    case 2:
        sgl::fill_polygon(segment_a, 4);
        sgl::fill_polygon(segment_b, 4);
        sgl::fill_polygon(segment_d, 4);
        sgl::fill_polygon(segment_e, 4);
        sgl::fill_polygon(segment_g, 6);
        break;
    case 3:
        sgl::fill_polygon(segment_a, 4);
        sgl::fill_polygon(segment_b, 4);
        sgl::fill_polygon(segment_c, 4);
        sgl::fill_polygon(segment_d, 4);
        sgl::fill_polygon(segment_g, 6);
        break;
    case 4:
        sgl::fill_polygon(segment_b, 4);
        sgl::fill_polygon(segment_c, 4);
        sgl::fill_polygon(segment_f, 4);
        sgl::fill_polygon(segment_g, 6);
        break;
    case 5:
        sgl::fill_polygon(segment_a, 4);
        sgl::fill_polygon(segment_c, 4);
        sgl::fill_polygon(segment_d, 4);
        sgl::fill_polygon(segment_f, 4);
        sgl::fill_polygon(segment_g, 6);
        break;
    case 6:
        sgl::fill_polygon(segment_a, 4);
        sgl::fill_polygon(segment_c, 4);
        sgl::fill_polygon(segment_d, 4);
        sgl::fill_polygon(segment_e, 4);
        sgl::fill_polygon(segment_f, 4);
        sgl::fill_polygon(segment_g, 6);
        break;
    case 7:
        sgl::fill_polygon(segment_a, 4);
        sgl::fill_polygon(segment_b, 4);
        sgl::fill_polygon(segment_c, 4);
        break;
    case 8:
        sgl::fill_polygon(segment_a, 4);
        sgl::fill_polygon(segment_b, 4);
        sgl::fill_polygon(segment_c, 4);
        sgl::fill_polygon(segment_d, 4);
        sgl::fill_polygon(segment_e, 4);
        sgl::fill_polygon(segment_f, 4);
        sgl::fill_polygon(segment_g, 6);
        break;
    case 9:
        sgl::fill_polygon(segment_a, 4);
        sgl::fill_polygon(segment_b, 4);
        sgl::fill_polygon(segment_c, 4);
        sgl::fill_polygon(segment_d, 4);
        sgl::fill_polygon(segment_f, 4);
        sgl::fill_polygon(segment_g, 6);
        break;
    }
}

/**
 *  Set's the value of the digit.
 *  The assigned value is modulo ten.
 *  @param value value to assign, modulo 10
 *  @return nothing
 */
void sgl::SevenSegmentDigit::set_value(int value)
{
    current_value = value % 10;
}

/**
 *  Returns the digit's current value.
 *  @return the digit's current value.
 */
int sgl::SevenSegmentDigit::get_value() const
{
    return current_value;
}

/**
 *  Increases the digit's value by 1, modulo 10.
 *  @return nothing
 */
void sgl::SevenSegmentDigit::increment()
{
    current_value = (current_value + 1) % 10;
}

/**
 *  Decreases the digit's value by 1, modulo 10.
 *  @return nothing
 */
void sgl::SevenSegmentDigit::decrement()
{
    current_value = (current_value == 0)? 9 : current_value - 1;
}

/**
 *  Changes the height of the digit by a given amount.  The digit's 
 *  width changes proportionally.
 *  @param the change in the height of the digit
 *  @return nothing
 */
void sgl::SevenSegmentDigit::resize(double inc)
{
    height += inc;
    update_segment_locations();
}

/**
 *  Moves the digit to a new location.  The digit's 
 *  size is unaffected.
 *  @param x the new x coordinate of the digit's lower-left corner.
 *  @param y the new y coordinate of the digit's lower-left corner.
 *  @return nothing
 */
void sgl::SevenSegmentDigit::move_to(double x, double y)
{
    this->x = x;
    this->y = y;
    update_segment_locations();
}



/**
 *  The constructor sets the digit's color, position, and
 *  height.
 *  @param color the color of the digit
 *  @param x the x coordinate of the digit's lower-left corner
 *  @param y the y coordinate of the digit's lower-left corner
 *  @param height the height of the digit
 */
sgl::DisplayDigit::DisplayDigit(sgl::Color color, double x, double y, double height):
          GraphicalObject(x, y, height/2.0, height), led(color, x, y, height)
{
    //  Nothing to do here
}

/**  Draws the digit. */
void sgl::DisplayDigit::paint() const
{
    /*set_color(WHITE);
    draw_rectangle(left, bottom, width, height);*/
    led.paint();
}


/**
 *  Set's the value of the digit.
 *  The assigned value is modulo ten.
 *  @param value value to assign, modulo 10
 *  @return nothing
 */
void sgl::DisplayDigit::set_value(int value)
{
    led.set_value(value);
}

/**
 *  Returns the digit's current value.
 *  @return the digit's current value.
 */
int sgl::DisplayDigit::get_value() const
{
    return led.get_value();
}


/**
 *  Increases the digit's value by 1, modulo 10.
 *  @return nothing
 */
void sgl::DisplayDigit::increment()
{
    led.increment();
}

/**
 *  Decreases the digit's value by 1, modulo 10.
 *  @return nothing
 */
void sgl::DisplayDigit::decrement()
{
    led.decrement();
}

/**
 *  Changes the height of the digit by a given amount.  The digit's 
 *  width changes proportionally.
 *  @param the change in the height of the digit
 *  @return nothing
 */
void sgl::DisplayDigit::resize(double inc)
{
    led.resize(inc);
    width += inc/2.0;
    height += inc;
}

/**
 *  Moves the digit to a new location.  The digit's 
 *  size is unaffected.
 *  @param x the new x coordinate of the digit's lower-left corner.
 *  @param y the new y coordinate of the digit's lower-left corner.
 *  @return nothing
 */
void sgl::DisplayDigit::mouse_dragged(double x, double y)
{
    GraphicalObject::mouse_dragged(x, y);
    led.move_to(left, bottom);
}


/**
 *  The constructor sets the digit's color, position, and
 *  height.  The digit's value is initially zero.
 *  @param color the color of the digit
 *  @param x the x coordinate of the tens digit's lower-left corner
 *  @param y the y coordinate of the tens digit's lower-left corner
 *  @param height the height of the digits
 */
sgl::DoubleDigit::DoubleDigit(sgl::Color color, double x, double y, double height):
            GraphicalObject(x, y, height + 2, height), 
            tens(color, x, y, height), 
            ones(color, x + height/2.0 + 2, y, height),
            leading_zero(true), visible(true)
{
    //  Nothing to do here
}

/**  Draws the digits. */
void sgl::DoubleDigit::paint() const
{
    /*set_color(WHITE);
    draw_rectangle(left, bottom, width, height);*/
    if ( visible )
    {
        if ( leading_zero || tens.get_value() != 0 )
            tens.paint();
        ones.paint();
    }
}

/**
 *  Set's the value of the digits.
 *  The assigned value is modulo 100.
 *  @param value value to assign, modulo 100
 *  @return nothing
 */
void sgl::DoubleDigit::set_value(int value)
{
    tens.set_value(value / 10);
    ones.set_value(value % 10);
}

/**
 *  Returns the current value.
 *  @return the current value.
 */
int sgl::DoubleDigit::get_value() const
{
    return 10*tens.get_value() + ones.get_value();
}

/**
 *  Increases the digit's value by 1, modulo 100.
 *  @return nothing
 */
void sgl::DoubleDigit::increment()
{
    if ( ones.get_value() == 9 )
        tens.increment();
    ones.increment();
}

/**
 *  Decreases the digit's value by 1, modulo 100.
 *  @return nothing
 */
void sgl::DoubleDigit::decrement()
{
    if ( ones.get_value() == 0 )
        tens.decrement();
    ones.decrement();
}

/**
 *  Changes the height of the digits by a given amount.  The digits' 
 *  width changes proportionally.
 *  @param the change in the height of the digits
 *  @return nothing
 */
void sgl::DoubleDigit::resize(double inc)
{
    tens.resize(inc);
    ones.resize(inc);
    ones.move_to(tens.get_x() + tens.get_height()/2.0 + 2, ones.get_y());
    width += inc/2.0;
    height += inc;
}

/**
 *  Moves the digits to a new location.  The digits' 
 *  sizes are unaffected.
 *  @param x the new x coordinate of the tens digit's lower-left
 *           corner.
 *  @param y the new y coordinate of the tens digit's lower-left 
 *           corner.
 *  @return nothing
 */
void sgl::DoubleDigit::mouse_dragged(double x, double y)
{
    GraphicalObject::mouse_dragged(x, y);
    tens.move_to(left, bottom);
    ones.move_to(left + height/2.0 + 2.0, bottom);
}

/**
 *  Turns on or off the display of a leading zero.
 *  @param flag if true, a leading zero is displayed;
 *              otherwise, a leading zero is not displayed.
 *  @return nothing
 */
void sgl::DoubleDigit::set_leading_zero(bool flag)
{
    leading_zero = flag;
}

/**
 *  Makes the display visible or invisible.
 *  @param flag if true, the digits are visible
 *              otherwise, the digits are invisible.
 *  @return nothing
 */
void sgl::DoubleDigit::set_visible(bool flag)
{
    visible = flag;
}



/**
  *  The constructor sets the display's number of digits,
  *  color, position, and
  *  height.  The digit's value is initially zero.
  *  @param n the number of digits
  *  @param color the color of the digits
  *  @param x the x coordinate of the leading digit's lower-left corner
  *  @param y the y coordinate of the leading digit's lower-left corner
  *  @param height the height of the digits
  */
sgl::Multidigit::Multidigit(int n, sgl::Color color, double x, double y, double height):
            sgl::GraphicalObject(x, y, n*height/2.0, height),
            digits(n, sgl::SevenSegmentDigit(color, x, y, height)),
            //tens(color, x, y, height), 
            //ones(color, x + height/2.0 + 2, y, height),
            leading_zeros(true), visible(true)
{
    double offset = height/2.0;
    for ( int i = 0; i < n; i++ )
        digits[i].move_to(x + i*offset, y);
}

/**  Draws the digits. */
void sgl::Multidigit::paint() const
{
    /*
    set_color(BLACK);
    draw_rectangle(left, bottom, width, height);
    */
    if ( visible )
    {
        int n = digits.size();
        int i = 0;
        if ( !leading_zeros )  //  Skip leading zeros
            while ( i < n - 1 && digits[i].get_value() == 0 )
                i++;
        while ( i < n )
        {
            digits[i].paint();
            i++;
        }
    }
}

/**
 *  Set's the value of the display.
 *  @param value value to assign
 *  @return nothing
 */
void sgl::Multidigit::set_value(int value)
{
    const int divisor = 10;
    int n = digits.size();
    for ( int i = 0; i < n; i++ )
    {
        digits[n - i - 1].set_value(value % divisor);
        value /= divisor;
    }
}

/**
 *  Returns the current value.
 *  @return the current value.
 */
int sgl::Multidigit::get_value() const
{
    int multiplier = 1;
    int result = 0;
    int n = digits.size();
    for ( int i = n - 1; i >= 0; i-- )
    {
        result += digits[i].get_value()*multiplier;
        multiplier *= 10;
    }
    return result;
}


void sgl::Multidigit::increment_helper(int n)
{
    if ( n >= 0 )
    {
        digits[n].increment();
        if ( digits[n].get_value() == 0 )
            increment_helper(n - 1);
    }
}

void sgl::Multidigit::decrement_helper(int n)
{
    if ( n >= 0 )
    {
        digits[n].decrement();
        if ( digits[n].get_value() == 9 )
            decrement_helper(n - 1);
    }
}

/**
 *  Increases the display's value by 1
 *  @return nothing
 */
void sgl::Multidigit::increment()
{
    increment_helper(digits.size() - 1);
}

/**
 *  Decreases the display's value by 1
 *  @return nothing
 */
void sgl::Multidigit::decrement()
{
    decrement_helper(digits.size() - 1);
}

/**
 *  Changes the height of the display by a given amount.  The
 *  display's width changes proportionally.
 *  @param the change in the height of the digits
 *  @return nothing
 */
void sgl::Multidigit::resize(double inc)
{
    int n = digits.size();
    digits[0].resize(inc);
    double offset = digits[0].get_height()/2.0;    
    for ( int i = 1; i < n; i++ )
    {
        digits[i].resize(inc);
        digits[i].move_to(digits[0].get_x() + i*offset, digits[0].get_y());
    }
    height += inc;
    width = n*offset;
}

/**
 *  Moves the display to a new location.  The digits' 
 *  sizes are unaffected.
 *  @param x the new x coordinate of the display's lower-left
 *           corner.
 *  @param y the new y coordinate of the display's lower-left 
 *           corner.
 *  @return nothing
 */
void sgl::Multidigit::mouse_dragged(double x, double y)
{
    GraphicalObject::mouse_dragged(x, y);
    int n = digits.size();
    double offset = height/2.0;
    for ( int i = 0; i < n; i++ )
        digits[i].move_to(left + i*offset, bottom);
}

/**
 *  Relocates the left-bottom corner of the display
 *  to the specified location.  The display's size is
 *  unaffected.
 *  @param x the x coordinate in viewport coordinates
 *               of the left-bottom corner of the
 *               display
 *  @param y the y coordinate in viewport coordinates
 *               of the left-bottom corner of the
 *               display
 *  @return nothing
 */  
void sgl::Multidigit::move_to(double x, double y)
{
    double old_left = left, 
           old_bottom = bottom;
    GraphicalObject::move_to(x, y);
    double dx = left - old_left, 
           dy = bottom - old_bottom;
    int n = digits.size();
    for ( int i = 0; i < n; i++ )
        digits[i].move_to(digits[i].get_x() + dx,
                       digits[i].get_y() + dy);
}

/**
 *  Turns on or off the display of a leading zero.
 *  @param flag if true, a leading zero is displayed;
 *              otherwise, a leading zero is not displayed.
 *  @return nothing
 */
void sgl::Multidigit::set_leading_zeros(bool flag)
{
    leading_zeros = flag;
}

/**
 *  Makes the display visible or invisible.
 *  @param flag if true, the digits are visible
 *              otherwise, the digits are invisible.
 *  @return nothing
 */
void sgl::Multidigit::set_visible(bool flag)
{
    visible = flag;
}



/**  
 *  Draw the hours-minutes or minutes-seconds colon separator.  
 *  @param left_ones the ones digit of the left quantity
 *  @param right_tens the tens digit of the right quantity
 *  @return nothing
 */
void sgl::TimeDisplay::draw_separator(const sgl::SevenSegmentDigit& left_ones, 
                                      const sgl::SevenSegmentDigit& right_tens) const
{
    double width = height/2.0,
           rect_width = width/8.0, 
           x = (left_ones.get_x() + width + right_tens.get_x())/2.0 - rect_width/2.0;
    fill_rectangle(x, bottom + height/3.0, rect_width, rect_width); 
    fill_rectangle(x, bottom + 2.0*height/3.0, rect_width, rect_width); 
}


/**
 *  Draw the display without leading units that are zero; for example, 1:03:23 instead of
 *  01:03:23, 4:10 instead of 00:04:10, and 3 instead of 00:00:03.
 *  @return nothing
 */
void sgl::TimeDisplay::paint_without_leading_units() const
{
    if ( hours_tens.get_value() > 0 )  //  Need to display tens of hours?
        hours_tens.paint(); //  Draw the hours 10s digit
    if ( seconds >= 3600 )  //  Need to display hours?
    {
        hours_ones.paint();
        draw_separator(hours_ones, minutes_tens);
    }

    if ( seconds >= 3600 || minutes_tens.get_value() != 0 )  //  Need to display minutes?
        minutes_tens.paint();    //  Draw the minutes 10s digit
    if ( seconds >= 3600 || minutes_ones.get_value() != 0 || minutes_tens.get_value() != 0 )
    {
        minutes_ones.paint();  //  Draw the minutes 1s digit
        draw_separator(minutes_ones, seconds_tens);
    }

    //  Draw the seconds digits
    if ( seconds >= 60 || seconds_tens.get_value() != 0 )  //  Need to display 10s of seconds?
        seconds_tens.paint();
    seconds_ones.paint();    //  Always display ones of seconds

}

/**
 *  The constructor sets the display's color, position, and
 *  height.  The time is initially zero.
 *  @param color the color of the digit
 *  @param x the x coordinate of the tens digit's lower-left corner
 *  @param y the y coordinate of the tens digit's lower-left corner
 *  @param height the height of the digits
 */
sgl::TimeDisplay::TimeDisplay(sgl::Color color, double x, double y, int height): 
      GraphicalObject(x, y, 6*height/2.0 + 2*height/4.0, height),
      hours_tens(color, x, y, height), 
      hours_ones(color, x + height/2.0, y, height),
      minutes_tens(color, x + 2*height/2.0 + height/4.0, y, height), 
      minutes_ones(color, x + 3*height/2.0 + height/4.0, y, height),
      seconds_tens(color, x + 4*height/2.0 + 2*height/4.0, y, height), 
      seconds_ones(color, x + 5*height/2.0 + 2*height/4.0, y, height),
      seconds(0), leading_units(true), visible(true)
{
    //  Nothing to do here.
}



/**  Draws the display. 
 *  @return nothing
 */
void sgl::TimeDisplay::paint() const
{
    set_color(BLUE);
    draw_rectangle(left, bottom, width, height);
    if ( visible )
    {
        if ( leading_units )
        {
            //  Draw the hours digits
            hours_tens.paint();
            hours_ones.paint();
          
            //  Draw the hours-minutes colon separator
            draw_separator(hours_ones, minutes_tens);

            //  Draw the minutes digits
            minutes_tens.paint();
            minutes_ones.paint();

            //  Draw the minutes-seconds colon separator
            draw_separator(minutes_ones, seconds_tens);

            //  Draw the seconds digits
            seconds_tens.paint();
            seconds_ones.paint();
        }
        else
            paint_without_leading_units();
    }
}

/**
 *  Sets the time.
 *  @param sec desired time, in seconds
 *  @return nothing
 */
void sgl::TimeDisplay::set_value(int sec)
{
    sec %= 360000;             //  Upper limit is 99:59:59 (359999 secs)
    sec = (sec < 0)? 0 : sec;
    if ( sec != seconds )
    {
        //  Compute hours, minutes, and seconds
        seconds = sec;
        int hours, minutes;
        hours = sec/3600;
        sec %= 3600;
        minutes = sec/60;
        sec %= 60;

        //  Compute digit values
        hours_tens.set_value(hours/10);
        hours_ones.set_value(hours%10);
        minutes_tens.set_value(minutes/10);
        minutes_ones.set_value(minutes%10);
        seconds_tens.set_value(sec/10);
        seconds_ones.set_value(sec%10);
    }
}

/**
 *  Returns the current time, in seconds.
 *  @return the current value.
 */
int sgl::TimeDisplay::get_value() const
{
    return seconds;
}

/**
 *  Increases the time by one second.
 *  @return nothing
 */
void sgl::TimeDisplay::increment()
{
    set_value(get_value() + 1);
}

/**
 *  Decreases the time by one second.
 *  @return nothing
 */
void sgl::TimeDisplay::decrement()
{
    set_value(get_value() - 1);
}

/**
 *  Changes the height of the digits by a given amount.  The digits' 
 *  width changes proportionally.
 *  @param the change in the height of the digits
 *  @return nothing
 */
void sgl::TimeDisplay::resize(double inc)
{
    height += inc;
    width = 6*height/2.0 + height/2.0;

    hours_tens.resize(inc);
    hours_ones.resize(inc);
    minutes_tens.resize(inc); 
    minutes_ones.resize(inc);
    seconds_tens.resize(inc); 
    seconds_ones.resize(inc);

    double width = height/2.0,
           offset = height/4.0;
    hours_tens.move_to(left, bottom);
    hours_ones.move_to(left + width, bottom);
    minutes_tens.move_to(left + 2*width + offset, bottom);
    minutes_ones.move_to(left + 3*width + offset, bottom);
    seconds_tens.move_to(left + 4*width + 2.0*offset, bottom);
    seconds_ones.move_to(left + 5*width + 2.0*offset, bottom);
}

/**
 *  Moves the display to a new location.  The display's 
 *  size is unaffected.
 *  @param x the new x coordinate of the display's lower-left
 *           corner.
 *  @param y the new y coordinate of the display's lower-left 
 *           corner.
 *  @return nothing
 */
void sgl::TimeDisplay::mouse_dragged(double x, double y)
{
    GraphicalObject::mouse_dragged(x, y);
    double width = height/2.0,
           offset = height/4.0;
    hours_tens.move_to(left, bottom);
    hours_ones.move_to(left + width, bottom);
    minutes_tens.move_to(left + 2*width + offset, bottom);
    minutes_ones.move_to(left + 3*width + offset, bottom);
    seconds_tens.move_to(left + 4*width + 2.0*offset, bottom);
    seconds_ones.move_to(left + 5*width + 2.0*offset, bottom);
}

/**
 *  Moves the display to a new location.  The display's 
 *  size is unaffected.
 *  @param x the new x coordinate of the display's lower-left
 *           corner.
 *  @param y the new y coordinate of the display's lower-left 
 *           corner.
 *  @return nothing
 */
void sgl::TimeDisplay::move_to(double x, double y)
{
    GraphicalObject::move_to(x, y);
    double width = height/2.0,
           offset = height/4.0;
    hours_tens.move_to(left, bottom);
    hours_ones.move_to(left + width, bottom);
    minutes_tens.move_to(left + 2*width + offset, bottom);
    minutes_ones.move_to(left + 3*width + offset, bottom);
    seconds_tens.move_to(left + 4*width + 2.0*offset, bottom);
    seconds_ones.move_to(left + 5*width + 2.0*offset, bottom);
}


/**
 *  Turns on or off the display of a leading zero.
 *  @param flag if true, a leading zero is displayed;
 *              otherwise, a leading zero is not displayed.
 *  @return nothing
 */
void sgl::TimeDisplay::set_leading_units(bool flag)
{
    leading_units = flag;
}

/**
 *  Makes the display visible or invisible.
 *  @param flag if true, the digits are visible
 *              otherwise, the digits are invisible.
 *  @return nothing
 */
void sgl::TimeDisplay::set_visible(bool flag)
{
    visible = flag;
}



/*****************************************************
 *  Graphical object code
 *****************************************************/


//  Constructor initializes the bounding box of the graphical object and
//  assigns a unique ID number to the object.
sgl::GraphicalObject::GraphicalObject(double lf, double bm, 
                                          double wd, double ht):
                           window(0), left(lf), bottom(bm), 
						   width(wd), height(ht), 
                           cursor(sgl::CURSOR_CROSSHAIR),
                           mouse_over(false), id(id_source++)
{
	//cout << "Making a graphical object: ";
	//cout << "left = " << left << ", bottom = " << bottom << endl;
}

//  Copy constructor makes a new graphical object with the same
//  characteristics except it has a unique ID
sgl::GraphicalObject::GraphicalObject(const GraphicalObject &go):
                 left(go.left), bottom(go.bottom),
                 width(go.width), height(go.height),
                 cursor(sgl::CURSOR_CROSSHAIR),
                 mouse_over(false), id(id_source++) {}

//  Assignment operator copies all the graphical object
//  characteristics but does not affect the object's ID
sgl::GraphicalObject& sgl::GraphicalObject::operator=
                  (const sgl::GraphicalObject &go)
{
    left = go.left;
    bottom = go.bottom;
    width = go.width;
    height = go.height;
    cursor = go.cursor;
    return *this;
}

//  Destructor currently does nothing
sgl::GraphicalObject::~GraphicalObject() 
{
	//cout << "Destroying graphical object #" << id << endl;
}



double sgl::GraphicalObject::get_left() const
{
	return left;
}

double sgl::GraphicalObject::get_bottom() const
{
	return bottom;
}

double sgl::GraphicalObject::get_width() const
{
	return width;
}

double sgl::GraphicalObject::get_height() const
{
	return height;
}

/**
 *  Sets the location and size of the graphical object.
 *  Values given are in the containing viewport's coordinates.
 *  @x the x coordinate of the left-bottom corner of the 
 *  graphical object's bounding rectangle.
 *  @y the y coordinate of the left-bottom corner of the 
 *  graphical object's bounding rectangle.
 *  @width the graphical object's width in viewport coordinates.
 *  @height the graphical object's height in viewport coordinates.
 *  @return nothing
 */
void sgl::GraphicalObject::set(double x, double y, 
                               double width, double height)
{
    left = x;
    bottom = y;
    this->width = width;
    this->height = height;
}



//  Determines if the point (x,y) falls within the bounding box of this 
//  graphical object?
bool sgl::GraphicalObject::hit(double x, double y)
{
    if ( x >= left && x <= left + width 
         && y >= bottom && y <= bottom + height )
    {
        x_hit_offset = x - left;  y_hit_offset = y - bottom;
        return true;
    }
    return false;
}

/**
 *  Called by the containing window's mouse_dragged method
 *  when the user drags the mouse
 *  when the mouse pointer is over the graphical object.
 *  @param x the x coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param y the y coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @return nothing
 */

void sgl::GraphicalObject::mouse_dragged(double x, double y)
{
    //left = x - x_hit_offset;   
    //bottom = y - y_hit_offset;
    move_to(x - x_hit_offset, y - y_hit_offset);
}

void sgl::GraphicalObject::move_to(double x, double y)
{
    left = x;   
    bottom = y;
}

void sgl::GraphicalObject::set_mouse_over(bool flag)
{
    mouse_over = flag;
}


/**
 *  Called by the containing window's mouse_pressed method
 *  when the user depresses any mouse
 *  button when the mouse pointer is over the graphical object.
 *  @param x the x coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param y the y coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param button the button that was depressed, left (LEFT_BUTTON) 
 *         or right (RIGHT_BUTTON) during the current mouse event.
 *  @return nothing
 */
void sgl::GraphicalObject::mouse_pressed(double, double, sgl::MouseButton)
{}

/**
 *  Called by the containing window's mouse_released method
 *  when the user releases any mouse
 *  button when the mouse pointer is over the graphical object.
 *  @param x the x coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param y the y coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param button the button that was depressed, left (LEFT_BUTTON) 
 *         or right (RIGHT_BUTTON) during the current mouse event.
 *  @return nothing
 */
void sgl::GraphicalObject::mouse_released(double, double, sgl::MouseButton)
{}

/**
 *  Called by the containing window's mouse_moved method
 *  when the user moves the mouse
 *  when the mouse pointer is over the graphical object.
 *  @param x the x coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @param y the y coordinate in viewport coordinates of the mouse
 *           pointer location during the current mouse event.
 *  @return nothing
 */
void sgl::GraphicalObject::mouse_moved(double, double)
{}


/**
 *  Called by the containing window when the user types a key
 *  when the window has keyboard focus and the mouse pointer
 *  is over the graphical object.  The key pressed and
 *  the location of the mouse pointer in viewport coordinates 
 *  is reported.  
 *  @param k the key typed by the user 
 *  @param x the x coordinate in viewport coordinates of the 
 *           location of the mouse pointer within the window
 *  @param y the y coordinate in viewport coordinates of the 
 *           location of the mouse pointer within the window
 *  @return nothing
 */
void sgl::GraphicalObject::key_pressed(int, double, double)
{}



sgl::Window *sgl::GraphicalObject::set_window(sgl::ObjectWindow *win)
{
	Window *previous = window;
    window = win;
	return previous;
}

sgl::Window *sgl::GraphicalObject::get_window() const
{
	return window;
}

 /**
  *  Sets the graphical object's cursor shape.
  *  @param cursor the object's cursor shape
  *  @return the previous cursor shape
  */
sgl::CursorShape sgl::GraphicalObject::set_cursor(sgl::CursorShape cursor)
{
    CursorShape prev_cursor = this->cursor;
    this->cursor = cursor;
    return prev_cursor;
}

/**
 *  Returns the cursor shape associated with this graphical
 *  object. 
 *  @return the cursor shape associated with this graphical
 *          object
 */
sgl::CursorShape sgl::GraphicalObject::get_cursor()
{
    return cursor;
}



unsigned sgl::GraphicalObject::id_source = 0;


/*****************************************************
 *  Popup menu code
 *****************************************************/

static sgl::PopupMenu *current_popup_menu = 0;

void sgl::PopupMenu::execute_handler(int n)
{
	(window->*(items[n].code))();
}

void sgl::PopupMenu::process_menu_events(int option)
{
	current_popup_menu->execute_handler(option);
}

sgl::PopupMenu::PopupMenu(Window *win): window(win) 
{
	glutCreateMenu(PopupMenu::process_menu_events);
	current_popup_menu = this;
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

sgl::PopupMenu::~PopupMenu() {}

void sgl::PopupMenu::_add_menu_item(const char *item, WindowCallback f)
{
	glutAddMenuEntry(item, items.size());
	items.push_back(MenuItem(item, f));
}

void sgl::PopupMenu::_replace_menu_item(const char * old_name, 
		                               const char * new_name, 
								       WindowCallback func)
{
    int num_items = items.size();
    MenuItem new_item(new_name, func);
    for ( int i = 0; i < num_items; i++ )
        if ( !strncmp(old_name, items[i].name, 50) )
        {
            items[i] = new_item;
            glutChangeToMenuEntry(i + 1, new_name, i);
            return;
        }
	//cout << "replace_menu_item unimplemented at this time" << endl;
}


void sgl::PopupMenu::remove_menu_item(const char * /* item */)
{
	cout << "remove_menu_item unimplemented at this time" << endl;
    //  Find index of current . . . 
}




/********************************************************
 *  Global drawing functions
 ********************************************************/

/*
 *  set_color
 *      Sets the current drawing color to the RGB values specified 
 *      by red, green, and blue.
 */
void sgl::set_color(const sgl::Color& color)
{
	glColor3d(color.red, color.green, color.blue);
}

void sgl::set_color(double r, double g, double b)
{
	glColor3d(r, g, b);
}

void sgl::set_line_width(double width)
{
	glLineWidth(static_cast<GLfloat>(width));
}

/**
 *  Determines if the bounding boxes of two graphical objects
 *  overlap.  
 *  @param obj1 one of the graphical objects
 *  @param obj2 the other graphical object
 *  @return true, if obj1's bounding box intersects 
 *          obj2's bounding box
 */
bool sgl::intersect(const sgl::GraphicalObject& obj1, 
                    const sgl::GraphicalObject& obj2)
{
    double min_x_1 = obj1.get_left(),
           max_x_1 = min_x_1 + obj1.get_width(),
           min_y_1 = obj1.get_bottom(),
           max_y_1 = min_y_1 + obj1.get_height(),

           min_x_2 = obj2.get_left(),
           max_x_2 = min_x_2 + obj2.get_width(),
           min_y_2 = obj2.get_bottom(),
           max_y_2 = min_y_2 + obj2.get_height();

    return min_x_2 < max_x_1
           && max_x_2 > min_x_1
           && max_y_2 > min_y_1
           && min_y_2 < max_y_1;
}


/*
 *  draw_point
 *      Draws a point in the current drawing color at location (x,y) in viewpoint coordinates.
 */
void sgl::draw_point(double x, double y)
{
	//  4x4 point
    //glRectd(x - 2.0, y - 2.0, x + 2.0, y + 2.0);
	glBegin(GL_POINTS);
	glVertex2d(x, y);
	glEnd();
}

/*
 *  draw_point
 *      Draws a point in the current drawing color at location (x,y) in viewpoint coordinates.
 */
void sgl::draw_point(const sgl::Point& pt)
{
	//  4x4 point
    //glRectd(pt.x - 2.0, pt.y - 2.0, pt.x + 2.0, pt.y + 2.0);
	glBegin(GL_POINTS);
	glVertex2d(pt.x, pt.y);
	glEnd();
}
/**
 *  Sets the size of a rendred point
 *  @param the point size
 *  @return nothing
 */
void sgl::set_point_size(int point_size)
{
    glPointSize(static_cast<GLfloat>(point_size));
}


/*
 *  draw_line
 *      Draws a line from (x0,y0) to (x1,y1) in the current drawing color.
 */
void sgl::draw_line(double x0, double y0, double x1, double y1)
{
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();
}

void sgl::draw_dashed_line(double x0, double y0, double x1, double y1, 
						   int pattern)
{
	glLineStipple(1, static_cast<GLushort>(pattern));
	glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();
	glDisable(GL_LINE_STIPPLE);
}

/*
 *  draw_rectangle
 *      Draws a rectangle frame in the current drawing color at location (x,y) in viewpoint coordinates.
 */
void sgl::draw_rectangle(double x, double y, double width, double height)
{
    //glRectd(x, y, x + width, y + height);
	glBegin(GL_LINE_LOOP);
    glVertex2d(x, y);
	glVertex2d(x + width, y);
	glVertex2d(x + width, y + height);
	glVertex2d(x, y + height);
    //glVertex2d(x, y);
	glEnd();
}

/*
 *  fill_rectangle
 *      Draws a filled rectangle in the current drawing color at location (x,y) in viewpoint coordinates.
 */
void sgl::fill_rectangle(double x, double y, double width, double height)
{
	glBegin(GL_POLYGON);
    glVertex2d(x, y);
	glVertex2d(x + width, y);
	glVertex2d(x + width, y + height);
	glVertex2d(x, y + height);
	glEnd();
}

static const double DEG_TO_RAD = 3.1415926/180.0;

void sgl::draw_circle(double x, double y, double radius)
{
	const int NUM_SEGMENTS = 360;
	glBegin(GL_LINE_LOOP); 
	for(int deg = 0; deg < NUM_SEGMENTS; deg++) 
	{ 
        //  Get the current angle 
		//double theta = 2.0 * 3.1415926 * i/NUM_SEGMENTS;
        double theta = deg*DEG_TO_RAD;
		double edge_x = radius*cos(theta);   //  Calculate the x coordinate 
		double edge_y = radius*sin(theta);   //  Calculate the y coordinate 

		glVertex2d(x + edge_x, y + edge_y);  //  Output vertex 

	} 
	glEnd(); 
}

// Draw Filled Circle  Adapted from code 
//  on http://www.allegro.cc/forums/thread/588625 
void sgl::fill_circle(double x, double y, double radius)
{
	const int NUM_SEGMENTS = 360;
    double y1 = y;
    double x1 = x;
    glBegin(GL_TRIANGLES);  
    for( int deg = 0; deg <= NUM_SEGMENTS; deg++ )
    {
        double angle = deg*DEG_TO_RAD;
        double x2 = x + radius*sin(angle);
        double y2 = y + radius*cos(angle);             
        glVertex2d(x,y);
        glVertex2d(x1,y1);
        glVertex2d(x2,y2);
        y1 = y2;
        x1 = x2;
    }
    glEnd();
}

//  Draws a polygon
void sgl::draw_polygon(const vector<sgl::Point>& pts)
{
	vector<sgl::Point>::const_iterator iter;
	glBegin(GL_LINE_LOOP);
	for ( iter = pts.begin(); iter != pts.end(); iter++ )
		glVertex2d(iter->x, iter->y);
	glEnd();
}

/**
 *  Draws the outline of a polygon specified by the points
 *  in an array of Point objects. The points adjacent in the polygon 
 *  also must be adjacent in the vector.
 *  @param pts a std::vector of Point objects that comprise the vertices
 *             of the polygon.  Adjacent vertices in the polygon must
 *             adjacent in the vector.
 *  @param n the number of points in the array.
 *  @return nothing
 */
void sgl::draw_polygon(const sgl::Point *pts, int n)
{
	glBegin(GL_LINE_LOOP);
	for ( int i = 0; i < n; i++ )
		glVertex2d(pts[i].x, pts[i].y);
	glEnd();

}


//  Draw a filled polygon
void sgl::fill_polygon(const vector<sgl::Point>& pts)
{
	vector<sgl::Point>::const_iterator iter;
	glBegin(GL_POLYGON);
	for ( iter = pts.begin(); iter != pts.end(); iter++ )
		glVertex2d(iter->x, iter->y);
	glEnd();
}

/**
 *  Draws a filled polygon specified by the points
 *  in an array of Point objects. The points adjacent in the polygon 
 *  also must be adjacent in the vector.
 *  @param pts an array of Point objects that comprise the vertices
 *             of the polygon.  Adjacent vertices in the polygon must
 *             adjacent in the vector.
 *  @param n the number of points in the array.
 *  @return nothing
 */
void sgl::fill_polygon(const sgl::Point *pts, int n)
{
	glBegin(GL_POLYGON);
	for ( int i = 0; i < n; i++ )
		glVertex2d(pts[i].x, pts[i].y);
	glEnd();
}

void sgl::draw_text(const char *text, double x, double y, int font_size)
{
	void *font;
	switch ( font_size )
	{
	case 12:
		font = GLUT_BITMAP_HELVETICA_12;
		break;
	case 18:
		font = GLUT_BITMAP_HELVETICA_18;
		break;
	case 915:
		font = GLUT_BITMAP_9_BY_15;
		break;
	case 813:
		font = GLUT_BITMAP_8_BY_13;
		break;
	default:
		font = GLUT_BITMAP_HELVETICA_10;
	}
	glRasterPos2d(x, y);
	const char *str = text;
	while ( *str != '\0' )
        glutBitmapCharacter(font, *str++);
};


/**
 *  Draw a piece of graphical text in the window.  This version of
 *  draw_text uses a scalable stroke font.
 *  The provided (x,y) location specifies the left-bottom portion
 *  of the text.
 *  @param text the string of text to display
 *  @param x the x coordinate in the viewport coordinate system
 *           of the text's left-most extent
 *  @param y the y coordinate in the viewport coordinate system
 *           of the text's baseline
 *  @param scale the scale of the font; this generally will be 
 *           value less than one (for example, 0.1 or 0.25).
 */
void sgl::draw_text(const char *text, double x, double y, double scale)
{
    glPushMatrix();
    glTranslated(x, y, 0);
    glScaled(scale, scale, scale);
	const char *str = text;
	while ( *str != '\0' )
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *str++);
    glPopMatrix();
 
};



void sgl::draw_function(double (*f)(double), 
						double begin_x, double end_x,
						double increment)
{
	glBegin(GL_LINE_STRIP);
	for ( double x = begin_x; x <= end_x; x += increment )
		glVertex2d(x, f(x));
	glEnd();
}







/**
 *  Gets the width of the screen in pixels.
 *  @return the width of the screen in pixels; returns zero
            if the screen's width cannot be determined
 */
int sgl::get_screen_width()
{
	return glutGet(GLUT_SCREEN_WIDTH);
}

/**
 *  Gets the height of the screen in pixels.
 *  @return the height of the screen in pixels; returns zero
            if the screen's height cannot be determined
 */
int sgl::get_screen_height()
{
	return glutGet(GLUT_SCREEN_HEIGHT);
}


/****************************************
 *  Utility functions
 ****************************************/


//  Creates a Stopwatch object
//  A newly minted object is not running and is in a "reset" state
sgl::Stopwatch::Stopwatch(): start_time(0), end_time(0), running(false) {}

//  Starts the stopwatch.  If there is no current
//  start time, the stopwatch has not been used or 
//  has just been reset, so we record the current
//  system time to begin a new timing.  If there is
//  a valid start time, we merely resume the current timing.
//  Puts the stopwatch object in the 'running' state.
void sgl::Stopwatch::start()
{
    if ( start_time == 0 )     //  Need to start with a fresh timing?
        start_time = clock();  //  If so, record start time
    running = true;            //  The clock is running
}

//  Stops the stopwatch.  Records the current
//  system time and puts the stopwatch object in
//  the 'not running' state. 
//  If the stopwatch is already stopped, a diagnostic
//  message is issued, but the stored elapsed time is
//  unaffected.
void sgl::Stopwatch::stop()
{
    if ( running )
    {
        end_time = clock();
        running = false;
    }
    else
        cout << "Stopwatch is not running" << endl;
}

//  Reports the time elapsed between the
//  stopwatch's starting and stopping times.
//  If elapsed is called while the stopwatch
//  is running, the time elapsed since the start
//  time is reported,
double sgl::Stopwatch::elapsed() const
{
    if ( running )
        return clock() - start_time;
    else
        return static_cast<double>(end_time - start_time)/CLOCKS_PER_SEC;
}

//  Resets the stopwatch so a subsequent start begins recording 
//  a new time.  An attempt to  reset a running stopwatch 
//  produces an error message.
void sgl::Stopwatch::reset()
{
    if ( !running )
        start_time = end_time = 0;  //  Reset times
    else
        cout << "Error: Cannot reset a stopwatch that is running"
             << endl;
}

//  Pauses the program's execution for a period of 
//  milliseconds.
void sgl::pause(int msec)
{
    if ( msec > 0 )
    {
        clock_t start_time = clock();
        while ( clock() - start_time < msec )
            continue;
    }
}



/*  Pseudorandom number routines  */


void sgl::set_random_seed(int seed)
{
	if ( seed < 0 )
	{
		seed = seed;  //  Fix this
	}
	srand(static_cast<unsigned>(seed));
}

//  Returns a pseudorandom number in the 
//  range begin...end
int sgl::random(int begin, int end)
{
    int range = end - begin + 1;
    return rand() % range + begin;
}

/**
 *  Returns a pseudorandom number in the range 
 *  0...n - 1.  
 *  @param n the number of distinct potential pseudorandom
           values.
 *  @return a pseudorandom integer, r,  such that
 *         0 <= r < n
 */
int sgl::random(int n)
{
    return rand() % n;
}


//  Determines if |d1 - d2| < delta; in other words,
//  d1 and d2 are close enough to be considered "equal"
bool sgl::equals(double d1, double d2, double delta)
{
    return d1 == d2 || fabs(d1 - d2) < delta;
}

static std::ostringstream __to_string_stream;
static string __to_string_buffer;

/**
 *  Converts an integer to a string so it can be displayed
 *  with draw_text.
 *  @param i the integer to convert
 *  @return the string representation of i
 */
//string sgl::to_string(int i)
const char *sgl::to_string(int i)
{
    __to_string_stream.str("");
    __to_string_stream << i;
    __to_string_buffer = __to_string_stream.str();
    return __to_string_buffer.c_str();

}

/**
 *  Converts a double to a string so it can be displayed
 *  with draw_text.
 *  @param d the double to convert
 *  @return the string representation of d
 */
//string sgl::to_string(double d)
const char *sgl::to_string(double d)
{
    __to_string_stream.str("");
    __to_string_stream << d;
    __to_string_buffer = __to_string_stream.str();
    return __to_string_buffer.c_str();
}


#endif
