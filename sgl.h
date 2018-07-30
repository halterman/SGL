/**
 *  @mainpage
 *  SGL
 *  The Simple Graphics Library: An object-oriented C++ toolkit
 *  thinly wrapping GLUT.
 *  @author Richard L. Halterman
 *
 *  Copyright (c) 2010-2018, Richard L. Halterman
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


#ifndef SGL_H_
#define SGL_H_

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
#include <functional>


/**
 *  All client accessible classes, functions, and constants
 *  reside in the sgl namespace.
 */
namespace sgl {

/**
 *  A Point object represents a location within
 *  a Window's viewport.  The (x,y) values are
 *  given in viewport coordinates.
 */
struct Point {
	/**  The x coordinate of the point's location.  */
	double x;

	/**  The y coordinate of the point's location.  */
	double y;

	/**  Initializes the points coordinates.
	 *   @param x the x coordinate of the new point's location.
	 *   @param y the y coordinate of the new point's location.
	 */
	Point(double x, double y): x(x), y(y) {}

	/**  This default constructor makes a point at the origin
	 *   (0,0). 
	 */
	Point(): x(0.0), y(0.0) {}
};

/**
 *  A Color object represents an RGB color.
 *  Color objects are immutable.
 */
class Color {
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
enum class MouseButton { 
    Left = 0, 
    Right = 2, 
    Unknown = -1 
};

/**
 *  SpecialKey is an enumerated type that maps directly to the
 *  GLUT equivalents for the corresponding special keys.
 */
enum SpecialKey { 
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
enum class CursorShape { 
    Right_arrow = 0,
    Left_arrow,
    /* Symbolic cursor shapes. */
    Info,
    Destroy,
    Help,
    Cycle,
    Spray,
    Wait,
    Text,
    Crosshair,
    /* Directional cursors. */
    Up_down,
    Left_right,
    /* Sizing cursors. */
    Top_side,
    Bottom_side,
    Left_side,
    Right_side,
    Top_left_corner,
    Top_right_corner,
    Bottom_right_corner,
    Bottom_left_corner,
    /* Inherit from parent window. */
    Inherit = 100,
    /* Blank cursor. */
    None,
    /* Fullscreen crosshair (if available). */
    Full_crosshair
};

/**
 *  KeyModifer is an enumerated type that maps directly to the
 *  GLUT equivalents for the corresponding key modifers: SHIFT, CRTL, and ALT.
 *  These can be combined with bitwise OR to represent combinations such as
 *  CTRL-ALT.
 */
enum class KeyModifier { 
    NO_KEY = 0, 
    SHIFT_KEY = 1, 
    CTRL_KEY = 2, 
    ALT_KEY = 4 
};

class Window;        //  Forward reference
class ObjectWindow;  //  Forward reference


/**
 *   Represents a persistent graphical object used within a window.
 *   A graphical object is owned by a particular window.
 */
class GraphicalObject {
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
class Window {
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
	void initialize(const std::string& title, int left, int top, int width, int height, 
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
	Window(const std::string& title, int left, int top, int width, int height, 
		   double min_x, double max_x, double min_y, double max_y);

	/**
	 *  Creates a window with a default position.
	 *  After intializing instrance variables, the constructor defers 
	 *  the actual work to the initialize method.  The virtual
     *  viewport's range is 0.0...width by 0.0...height.
     *  @param title the text to appear within the window's titlebar
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
	 */
	Window(const std::string&  title, int width, int height);


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
	Window(const std::string& title, double min_x, double max_x, double min_y, double max_y);

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
    virtual void set_title(const std::string& str);

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
class PopupMenu {
protected:
    /**  The window associated with this popup menu */
    Window *window;

    struct MenuItem {
        //const char *name;
        std::string name;
        WindowCallback code;
        //MenuItem(const char *name, WindowCallback code):
        MenuItem(const std::string& name, WindowCallback code):
              name(name), code(code) {}
    };
	
	/**  Collection of menu items  */
	std::vector<MenuItem> items;

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
	//virtual void _add_menu_item(const char *item, WindowCallback func);
	virtual void _add_menu_item(const std::string& item, WindowCallback func);

	/**
	 *  Replaces one menu item and its associated handler with another.
	 *  The current menu item name is supplied along with the new menu 
	 *  item name and the new handler.
	 *  @param old_name a string listed for the current menu choice
	 *  @param new_name a string listed for the replacement menu choice
	 *  @param func the replacement handler for this menu item
	 *  @return nothing
	 */
	virtual void _replace_menu_item(const std::string& old_name, 
		                            const std::string& new_name, 
				  				    WindowCallback func);

	/**
	 *  Removes a menu item and its associated handler.
	 *  The current menu item name is supplied.
	 *  @param item a string listed for the current menu choice
	 *  @return nothing
	 */
	//virtual void remove_menu_item(const char *item);
	virtual void remove_menu_item(const std::string& item);

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
class ObjectWindow: public Window {
protected:
	/**  The list of graphical objects contained in this window.  */
	std::list<GraphicalObject *> object_list;
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
	ObjectWindow(const std::string& title, int left, int top, int width, 
                 int height, double min_x, double max_x, 
                 double min_y, double max_y);

	/**
	 *  Creates a window of a specified width and height
     *  @param title the text to appear within the window's titlebar
	 *  @param width the width in screen coordinates of the window
	 *  @param height the height in screen coordinates of the window
	 */
	ObjectWindow(const std::string& title, int width, int height);


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
	ObjectWindow(const std::string& title, double min_x, double max_x, 
                 double min_y, double max_y);

	/**
	 *  Create a default, plain window
	 */
	ObjectWindow();

	/**
	 *  Destroys a graphical window object.
	 */
	~ObjectWindow();

	/**  
	 *  Code executed before the call to paint
	 *  @return nothing
	 */
	void prepaint() override;

	/**
	 *  Code executed after the call to paint
	 *  @return nothing
	 */
	void postpaint() override;

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
	void mouse_pressed(double x, double y, MouseButton button) override;

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
	void mouse_released(double x, double y, MouseButton button) override;

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
	void mouse_moved(double x, double y) override;

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
	void mouse_dragged(double x, double y) override;

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
	void key_pressed(int k, double x, double y) override;

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
    std::list<GraphicalObject *>::iterator begin();

    /**
     *  Returns an iterator just past the end of the vector of 
     *  graphical objects this window contains.
     *  @return  an iterator just past the end of the vector of 
     *           graphical objects this window contains.
     */
    std::list<GraphicalObject *>::iterator end();
};


/**
 *  A composite object aggregates a collection of 
 *  graphical objects (even other composite objects
 *  into a single graphical object.
 */
class CompositeObject: public GraphicalObject {
protected:
    /**  The collection of contained graphical objects. */
    std::vector<GraphicalObject *> objects;
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
    void paint() const override;

    /**
     *  Repositions the lower-left corner of the composite object's
     *  boundng box to (x,y).  The positions of the contained
     *  graphical objects are updated accordingly.
     */
    void move_to(double x, double y) override;

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

class Pixmap {
    //  Local type represents a color triple, each with 256 
    //  possible intensities
    struct RGB {
        unsigned char red, green, blue;
    };

    /**
     *  Helper function for extracting an unsigned short
     *  value from a binary file.
     *  @param fin the file stream object providing
     *         the input.
     *  @return the value extracted from the input stream
     */
    static unsigned short get_short(std::ifstream& fin);

    /**
     *  Helper function for extracting an unsigned long
     *  value from a binary file.
     *  @param fin the file stream object providing
     *         the input.
     *  @return the value extracted from the input stream
     */
    static unsigned long get_long(std::ifstream& fin);

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

class BitmapObject: public GraphicalObject {
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
class OGLWindow: public Window {
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
	//OGLWindow(const char *title, int left, int top, int width, int height);
	OGLWindow(const std::string& title, int left, int top, int width, int height);

	/**
	 *  Creates a window with a default position and size.
     *  @param title the text to appear within the window's titlebar
	 */
	//OGLWindow(const char *title);
	OGLWindow(const std::string& title);

    // TODO add deleted copy ctor, assignment, etc.

	/**
	 *  Destroys a graphical window object.
	 */
	~OGLWindow();

	/**  
	 *  Code executed before the call to paint
	 *  @return nothing
	 */
    void prepaint() override;

	/**
	 *  Code executed after the call to paint
	 *  @return nothing
	 */
	void postpaint() override;

};


/******************************************
 *  Classes for seven segment digit displays
 ******************************************/

/**
 *  Used to draw decimal digits that look like
 *  LED/LCD seven-segment displays
 */
class SevenSegmentDigit {
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
class DisplayDigit: public GraphicalObject {
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
    void paint() const override;

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
    void mouse_dragged(double x, double y) override;
};



/**
 *  A graphical object representing a two-digit
 *  seven-segment display.
 */
class DoubleDigit: public GraphicalObject {
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
    void paint() const override;

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
    void mouse_dragged(double x, double y) override;

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
class Multidigit: public GraphicalObject {
protected:
    /**  Vector of seven segment digits  */
    std::vector<SevenSegmentDigit> digits;

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
    void paint() const override;

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
    void mouse_dragged(double x, double y) override;

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
    virtual void move_to(double left, double bottom) override;

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


class TimeDisplay: public GraphicalObject {
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
    void paint() const override;

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
    void mouse_dragged(double x, double y) override;

    /**
     *  Moves the display to a new location.  The display's 
     *  size is unaffected.
     *  @param x the new x coordinate of the display's lower-left
     *           corner.
     *  @param y the new y coordinate of the display's lower-left 
     *           corner.
     *  @return nothing
     */
    void move_to(double x, double y) override;


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
void draw_polygon(const std::vector<Point>& pts);

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
void fill_polygon(const std::vector<Point>& pts);

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
//void draw_text(const char *text, double x, double y, int font_size);
void draw_text(const std::string& text, double x, double y, int font_size);

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
//void draw_text(const char *text, double x, double y, double scale);
void draw_text(const std::string& text, double x, double y, double scale);

/**
 *  Returns the length of the graphical text in units of 
 *  the viewport coordinate system.
 *  @param text the text string to measure
 *  @param font_size the size of the font: 10, 12, or 18.
 *  @return the length of the graphical text in units of 
 *          the viewport coordinate system.
 */
//double text_width(const char *text, int font_size);
double text_width(const std::string& text, int font_size);

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


/**
 *  Creates an SGL window of type T passing the arguments 
 *  found in the Args parameter to the window's constructor.  
 *  Calls the window's run method to start the application.
 *  @tparam T the type of SGL window to create
 *  @tparam Args the argument pack to pass to the window's constructor
 *  @param args the instantiated parameters to pass to the
 *  window's constructor
 */
template <typename T, typename... Args>
inline void run(Args&&... args);

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
class Stopwatch {
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
std::string to_string(int i);

/**
 *  Converts a double to a string so it can be displayed
 *  with draw_text.
 *  @param d the double to convert
 *  @return the string representation of d
 */
std::string to_string(double d);


/**
 *  Returns the SGL version number of this library.
 */
std::string version();


//------------------------------------------------------------------------------------------
// Procedural interface to the SGL

/**
 *  Creates a graphics window for the SGL's procedural interface.  Clients
 *  need not derive a custom Window class and override methods.  Clients using
 *  the procedural interface control the application's state through global 
 *  variables rather than instance variables.  Clients must call this function
 *  before registering any callbacks calling <tt>run_window</tt>.
 *  @param title the text to appear in the window's title bar
 *  @param x the x component of the (x, y) location of the window's left-top corner
 *  @param y the y component of the (x, y) location of the window's left-top corner
 *  @param width the window's width
 *  @param height the window's height
 *  @return nothing
 */
void create_window(const std::string& title, int x, int y, int width, int height);

/**
 *  Starts the graphical event loop for a window created via 
 *  create_window.  Used for the SGL's procedural interface. 
 *  @return nothing
 */
void run_window();

/**
 *  Requests the graphical framework to repaint the window.
 *  Used for the SGL's procedural interface.  Schedules the
 *  application's painting callback for execution.
 *  @return nothing
 */
void update_window();

/**
 *  Registers the client's custom painting callback function with the graphical 
 *  window.  Used for the SGL's procedural interface. 
 *  @param f the client's callback function
 *  @return nothing
 */
void set_paint_function(const std::function<void()>& f);

/**
 *  Registers the client's custom mouse pressed callback function with the graphical 
 *  window.  Used for the SGL's procedural interface. 
 *  @param f the client's callback function
 *  @return nothing
 */
void set_mouse_pressed_function(const std::function<void(double, double, MouseButton)>& f);

/**
 *  Registers the client's custom mouse released callback function with the graphical 
 *  window.  Used for the SGL's procedural interface. 
 *  @param f the client's callback function
 *  @return nothing
 */
void set_mouse_released_function(const std::function<void(double, double, MouseButton)>& f);

/**
 *  Registers the client's custom mouse moved callback function with the graphical 
 *  window.  Used for the SGL's procedural interface. 
 *  @param f the client's callback function
 *  @return nothing
 */
void set_mouse_moved_function(const std::function<void(double, double)>& f);

/**
 *  Registers the client's custom mouse dragged callback function with the graphical 
 *  window.  Used for the SGL's procedural interface. 
 *  @param f the client's callback function
 *  @return nothing
 */
void set_mouse_dragged_function(const std::function<void(double, double)>& f);

/**
 *  Registers the client's custom keypress callback function with the graphical 
 *  window.  Used for the SGL's procedural interface. 
 *  @param f the client's callback function
 *  @return nothing
 */
void set_key_pressed_function(const std::function<void(int, double, double)>& f);

/**
 *  Changes the background color of the graphics window.  Client must call 
 *  <tt>update_window</tt> to force the repainting for it to take effect.
 *  Used for the SGL's procedural interface. 
 *  @param c the desired background color
 *  @return nothing
 */
void set_window_background(const Color& c);

/**
 *  Sets the title of the window.  Used for the SGL's procedural interface. 
 *  @param str the text of the new title
 *  @return nothing
 */
void set_window_title(const std::string& str);



}  //  End of namespace sgl

//  Some syntactic sugar for adding items to popup menus
#define POPUPCALLBACK(x) (static_cast<sgl::WindowCallback>(x))
#define add_menu_item(x, y) _add_menu_item((x), (static_cast<sgl::WindowCallback>(y)));
#define replace_menu_item(x, y, z) _replace_menu_item((x), (y), (static_cast<sgl::WindowCallback>(z)));


#endif
