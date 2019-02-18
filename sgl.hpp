/**
 *  @mainpage
 *  SGL
 *  The Simple Graphics Library: An object-oriented C++ toolkit
 *  thinly wrapping GLUT.
 *  @author Richard L. Halterman
 *
 *  Last modified 2019-02-17
 *
 *  Copyright (c) 2010-2019, Richard L. Halterman
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


#ifndef SGL_HPP_
#define SGL_HPP_

//#ifdef _MSC_VER
//    #pragma warning(disable:4251)
//    #pragma warning(disable:4224)
//    #pragma warning(disable:4458)
//#endif

#ifdef _MSC_VER
    #pragma warning(push, 3)
#endif

//#include <sstream>
#include <cmath>
#include <cstdlib>  //  Linux requires cstdlib for exit and rand functions
//#include <cstring>
//#include <memory>
#include <algorithm>
#include <GL/sgl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
 *  All client accessible classes, functions, and constants
 *  reside in the sgl namespace.
 */
namespace sgl {

static const std::string SGL_VERSION_NUMBER = "0.9.5 (February 17, 2019)";


static bool glut_active = false;
static bool event_loop_running = false;



//static Window *current_window = nullptr;  //  Current window intially null

//  Maximum number of windows managed by SGL
const int MAX_WINDOWS = 10;
//  List of windows managed by SGL
static Window *window_list[] = { 
	nullptr, // Index 0 is unused 
	nullptr, 
	nullptr, 
	nullptr, 
	nullptr, 
	nullptr, 
	nullptr, 
	nullptr, 
	nullptr, 
	nullptr, 
	nullptr 
};

//  Some coordinate conversion helper functions
static void GetOGLPos(int x, int y, double *vec) {
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

static void convert(int mx, int my, double &vx, double &vy) {
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
void initialize_graphics(unsigned int mode) {
	if (!glut_active) {
		//  Make up a dummy program name and pass no additional arguments
		static int argc = 1;
        static char *argv[] = { const_cast<char *>("sgl_program") };
		glutInit(&argc, argv);
		glutInitDisplayMode(mode);
		glut_active = true;
	}
	else {
		std::cout << "Graphics environment already initialized\n";
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
void initialize_graphics() {
    initialize_graphics(GLUT_DOUBLE | GLUT_RGB);
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
		std::cout << "Graphics environment already initialized\n";
		exit(1);
	}
}
******************** */


//  Returns the version number of SGL
std::string version() {
    return SGL_VERSION_NUMBER;
}


/*****************************************************
 *  Global GLUT callback functions
 *****************************************************/

static void sgl_display() {
	if (window_list[glutGetWindow()])
		window_list[glutGetWindow()]->paint_all();
}

static void sgl_reshape(int w, int h) {
	window_list[glutGetWindow()]->resized(w, h);
}

static void sgl_mouse_dragged(int x, int y) {
	double vx, vy;
	convert(x, y, vx, vy);
	window_list[glutGetWindow()]->mouse_dragged(vx, vy);
}


static void sgl_mouse_moved(int x, int y) {
	double vx, vy;
	convert(x, y, vx, vy);
	window_list[glutGetWindow()]->mouse_moved(vx, vy);
}

static void sgl_mouse_entry(int state) {
    if (state == GLUT_LEFT)
        window_list[glutGetWindow()]->mouse_exited();
    else
        window_list[glutGetWindow()]->mouse_entered();
}


static void sgl_timer_expired(int win_number) {
	//current_window->timer_expired(msec);
	//std::cout << "Sending a timer message to window #" 
	//	 << glutGetWindow() << " (" << win_number << ")\n";
	//window_list[glutGetWindow()]->timer_expired(msec);
	window_list[win_number]->timer_expired();
}


static void sgl_mouse_button(int button, int state, int x, int y) {
	double vx, vy;
	convert(x, y, vx, vy);
	//std::cout << "(" << vx << "," << vy << ")\n";

	Window *win = window_list[glutGetWindow()];

    win->set_key_modifiers(static_cast<KeyModifier>(glutGetModifiers()));

	if (state == GLUT_DOWN)
		win->mouse_pressed(vx, vy, static_cast<MouseButton>(button));
	else if (state == GLUT_UP)
		win->mouse_released(vx, vy, static_cast<MouseButton>(button));
}



static void sgl_key_pressed(unsigned char k, int x, int y) {
	double vx, vy;
	convert(x, y, vx, vy);
	Window *win = window_list[glutGetWindow()];
	win->set_key_modifiers(static_cast<KeyModifier>(glutGetModifiers()));
	win->key_pressed(k, vx, vy);
}

static void sgl_special_key_pressed(int k, int x, int y) {
	double vx, vy;
	convert(x, y, vx, vy);
	Window *win = window_list[glutGetWindow()];
	win->set_key_modifiers(static_cast<KeyModifier>(glutGetModifiers()));
	win->key_pressed(k + 10000, vx, vy);
	//current_window->special_key_pressed(k, vx, vy);
}


/*****************************************************
 *  Global constant Color objects
 *****************************************************/

const Color BLACK(0.0, 0.0, 0.0);
const Color RED(1.0, 0.0, 0.0);
const Color GREEN(0.0, 1.0, 0.0);
const Color BLUE(0.0, 0.0, 1.0);
const Color YELLOW(1.0, 1.0, 0.0);
const Color MAGENTA(1.0, 0.0, 1.0);
const Color CYAN(0.0, 1.0, 1.0);
const Color DARK_RED(0.75, 0.0, 0.0);
const Color DARK_GREEN(0.0, 0.75, 0.0);
const Color DARK_BLUE(0.0, 0.0, 0.75);
const Color LIGHT_RED(1.0, 0.75, 0.75);
const Color LIGHT_GREEN(0.75, 1.0, 0.75);
const Color LIGHT_BLUE(0.75, 0.75, 1.0);
const Color GRAY(0.5, 0.5, 0.5);
const Color LIGHT_GRAY(0.8, 0.8, 0.8);
const Color DARK_GRAY(0.2, 0.2, 0.2);
const Color WHITE(1.0, 1.0, 1.0);


/*****************************************************
 *  Graphical object code
 *****************************************************/


void Window::initialize(const std::string& title, int left, int top, int width, int height, 
			   	             double min_x, double max_x, double min_y, double max_y) {
    //  Check for proper window dimensions
    if (max_x - min_x <= 0 || max_y - min_y <= 0)
        std::cout << "WARNING: Horizontal and/or vertical window dimensions "
                  << "zero or negative (check constructor arguments)\n";
	//  Must initialize graphics for the first window created if
    //  it has not been done already
	if (!glut_active)
		initialize_graphics();

	glutInitWindowSize(width, height);
	glutInitWindowPosition(left, top);
	//std::cout << "Creating window #" << glutCreateWindow(title) << '\n';
	glutCreateWindow(title.c_str());
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

    normal_cursor = CursorShape::Right_arrow;
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
Window::Window(const std::string& title, int left, int top, int width, int height, 
				   double min_x, double max_x, double min_y, double max_y) {
	initialize(title, left, top, width, height, min_x, max_x, min_y, max_y);
}

Window::Window(const std::string& title, int width, int height) {
	initialize(title, 100, 100, width, height, 0.0, width, 0.0, height);
}

Window::Window(const std::string& title, double min_x, double max_x, double min_y, double max_y) 
{
    //  Compute the window's size
    double width = max_x - min_x,
           height = max_y - min_y;
    int window_width, window_height;
    //  Prevent the creation of a window with zero or negative
    //  dimensions
    if (width <= 0 || height <= 0) {
        std::cout << "Window vertical or horizontal dimension zero "
                  << "or negative; check order of constructor arguments\n";
        exit(1);
    }
    //  Scale physical window size so the longest side is 600 pixels
    if (width > height) {  //  window is shorter and wider
        window_width = 600;  
        //  Adjust aspect ratio
        window_height = static_cast<int>(600*height/width + 0.5);
    }
    else {  //  Window is longer and taller
        window_height = 600;
        //  Adjust aspect ratio
        window_width = static_cast<int>(600*width/height + 0.5);
    }

	initialize(title, 100, 100, window_width, window_height, 
               min_x, max_x, min_y, max_y);
}

Window::Window() {
	initialize("", 100, 100, 600, 600, 0.0, 0.0, 600.0, 600.0);
}

Window::~Window() {
	//current_window = nullptr;
	window_list[glutGetWindow()] = nullptr;
	//object_list.clear();
}

/**
 *  Sets the location of the left-top corner of the window's
 *  client area to (x,y).
 *  @param x the x coordinate of the window's left-top corner
 *  @param y the y coordinate of the window's left-top corner
 *  @return nothing
 */
void Window::set_position(int x, int y) {
    glutPositionWindow(x, y);
}

/**
 *  Sets the physical window's hoizontal and vertical
 *  dimensions.
 *  @param width the new width of the window
 *  @param height the new height of the window
 *  @return nothing
 */
void Window::set_size(int width, int height) {
    glutReshapeWindow(width, height);
}

/**
 *  Shows or hides the window.
 *  viewport.  Values given are in viewport's coordinates.
 *  @param visible if true, the window is visible;
 *                 otherwise, the window is invisible.
 *  @return nothing
 */
void Window::set_visible(bool visible) {
    if (visible)
        glutShowWindow();
    else
        glutHideWindow();
}

/**
 *  Sets the window's cursor shape
 *  @param cursor one of several CursorShape values
 *  @return nothing
 */
CursorShape Window::set_cursor(CursorShape cursor) {
    CursorShape previous_cursor =
           static_cast<CursorShape>(glutGet(GLUT_WINDOW_CURSOR));
    glutSetCursor(static_cast<int>(cursor));
    return previous_cursor;
}

/**
 *  Gets the window's curent cursor shape
 *  @return the window's current cursor shape
 */
CursorShape Window::get_cursor() {
    return static_cast<CursorShape>(glutGet(GLUT_WINDOW_CURSOR));
}


void Window::set_viewport(double left, double right, 
                               double bottom, double top) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);
}

void Window::set_background_color(const Color& color) {
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
void Window::run() {
	if (!event_loop_running) {
		event_loop_running = true;
		glutMainLoop();
	}
	else
		std::cout << "Event loop already running, action ignored\n";
}


/*
 *  draw_axes
 *      Draws the x- and y-axes within the viewport using the current drawing color.
 */
void Window::draw_axes(double x_inc, double y_inc) const {
    //  Draw grid lines at 10 unit increments
    glColor3d(0.9, 0.9, 1.0);  // Light blue
    for (double x = 0 + x_inc; x < max_x; x += x_inc)
        draw_line(x, min_y, x, max_y);
    for (double y = 0 + y_inc; y < max_y; y += y_inc)
        draw_line(min_x, y, max_x, y);
    for (double x = -x_inc; x > min_x; x -= x_inc)
        draw_line(x, min_y, x, max_y);
    for (double y = -y_inc; y > min_y; y -= y_inc)
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
void Window::clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::set_title(const std::string& str) {
    glutSetWindowTitle(str.c_str());
}

void Window::repaint() {
	glutPostRedisplay();
}

void Window::paint_all() {
	prepaint();
	paint();
	postpaint();
}

void Window::prepaint() {
	glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
	glColor3d(0.0, 0.2, 0.0);
}

void Window::postpaint() {
	glPopMatrix();
	//glFlush();
	glutSwapBuffers();
}


//  Returns the x value of the left, top corner of the window
int Window::get_x() const {
	return glutGet(GLUT_WINDOW_X);
}

//  Returns the y value of the left, top corner of the window
int Window::get_y() const {
	return glutGet(GLUT_WINDOW_Y);
}

//  Returns the width of the window
int Window::get_width() const {
	return glutGet(GLUT_WINDOW_WIDTH);
}

//  Returns the height of the window
int Window::get_height() const {
	return glutGet(GLUT_WINDOW_HEIGHT);
}

double Window::get_min_x() const {
	return min_x;
}

double Window::get_max_x() const {
	return max_x;
}

double Window::get_min_y() const {
	return min_y;
}

double Window::get_max_y() const {
	return max_y;
}

void Window::set_window_size(int w, int h) {
	glutReshapeWindow(w, h);
}

void Window::resized(int w, int h) {
    glutInitWindowSize(w, h);
    glViewport (0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(min_x, max_x, min_y, max_y, -1.0, 1.0);
	gluOrtho2D(min_x, max_x, min_y, max_y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void Window::mouse_moved(double, double) {}

void Window::mouse_dragged(double, double) {}

void Window::mouse_pressed(double, double, MouseButton) {}

void Window::mouse_released(double, double, MouseButton) {}

/**
 *  Called by the event manager when the mouse pointer
 *  enters the window.
 *  @return nothing
 */
void Window::mouse_entered() {}

/**
 *  Called by the event manager when the mouse pointer
 *  exits the window.
 *  @return nothing
 */
void Window::mouse_exited() {}

/*
 *  Alt-F4 closes the window and terminates the program.  Derived classes
 *  should call this version to ensure consistent behavior.
 */
void Window::key_pressed(int key, double, double) { 
	if (key == F4_KEY && glutGetModifiers() == GLUT_ACTIVE_ALT)
		exit(0);
		//if ( glutGetWindow() == 1 )
		//	exit(0);   //  Alt-F4 terminates the program
		//else
		//{
		//	int window_to_close = glutGetWindow();
		//	window_list[window_to_close] = nullptr;  //  Nullify window
		//	std::cout << "Window to close: " << window_to_close << '\n';
		//	glutSetWindow(1);  //  Reset current window to main window
		//	std::cout << "Current window is now: " << glutGetWindow() << '\n';
		//	glutDestroyWindow(window_to_close);
		//}
    //std::cout << "Pressed a key" << '\n';
    //glutPostRedisplay();
}

///*
// *  Alt-F4 closes the window and terminates the program.
// */
//void Window::special_key_pressed(int key, double, double)
//{
//    if ( key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT )
//        exit(0);   //  Alt-F4 terminates the program
//}

KeyModifier Window::get_key_modifiers() const {
	return key_mods;
}

void Window::set_key_modifiers(KeyModifier mods) {
	key_mods = mods;
}

void Window::start_timer(int msec) {
	glutTimerFunc(msec, sgl_timer_expired, glutGetWindow());
}

//  Derived classes add custom behavior
void Window::timer_expired() {}



/*****************************************************
 *  ObjectWindow code
 *****************************************************/

/*
 *  Constructor
 *      Creates a graphics window with a its left-top corner at (left, top) in 
 *      screen coordinates.  Its width is width, and its height is height.  The viewport
 *      it represents extends from min_x to max_y horizontally and from min_y to max_y vertically.
 */
ObjectWindow::ObjectWindow(const std::string& title, 
                                int left, int top, 
                                int width, int height, 
				                double min_x, double max_x, 
                                double min_y, double max_y):
                Window(title, left, top, width, height,
                       min_x, max_x, min_y, max_y),
                active_object(nullptr) {}

ObjectWindow::ObjectWindow(const std::string& title, 
                                int width, int height):
                Window(title, width, height),
                active_object(nullptr) {}

ObjectWindow::ObjectWindow(const std::string& title, 
                                double min_x, double max_x, 
                                double min_y, double max_y):
                Window(title, min_x, max_x, min_y, max_y),
                active_object(nullptr) {}

ObjectWindow::ObjectWindow(): Window() {}


ObjectWindow::~ObjectWindow() {
	window_list[glutGetWindow()] = nullptr;
	remove_all();  // Remove and delete all contained graphical objects
	//object_list.clear();
}

void ObjectWindow::prepaint() {
    Window::prepaint();
}

void ObjectWindow::postpaint() {
    for (auto& p : object_list)
        p->paint();
    Window::postpaint();
}

void ObjectWindow::mouse_pressed(double x, double y, MouseButton button) {
    if (active_object) {
        active_object->mouse_pressed(x, y, button);
        repaint();
    }
    Window::mouse_pressed(x, y, button);
}

void ObjectWindow::mouse_released(double x, double y, 
                                       MouseButton button) {
    if (active_object) {
        active_object->mouse_released(x, y, button);
        repaint();
    }
    Window::mouse_released(x, y, button);
}

void ObjectWindow::mouse_moved(double x, double y) {
    GraphicalObject *prev_active = active_object;
    active_object = hit(x, y);
    if (prev_active != active_object) {
        if (prev_active) {
            prev_active->set_mouse_over(false);
            set_cursor(normal_cursor);
        }
        if (active_object) {
            active_object->set_mouse_over(true);
            active_object->mouse_moved(x, y);
            set_cursor(active_object->get_cursor());
        }
        repaint();
    }
}

void ObjectWindow::mouse_dragged(double x, double y) {
    if (active_object) {
        active_object->mouse_dragged(x, y);
        repaint();
    }
}

void ObjectWindow::key_pressed(int key, double x, double y) {
    if (active_object) {
        active_object->key_pressed(key, x, y);
        repaint();
    }
    Window::key_pressed(key, x, y);
}


// Add the given graphical object obj to the display list
void ObjectWindow::internal_add(GraphicalObject *obj) {
    //std::cout << "In add" << '\n';
    //std::cout << static_cast<void *>(obj) << '\n';
    //std::cout << static_cast<void *>(this) << '\n';
    object_list.push_back(obj);
	obj->set_window(this);
}


// Remove the given graphical object obj from the display list and 
// deallocate the removed graphical object.
void ObjectWindow::remove(GraphicalObject *obj) {
	object_list.erase(std::remove(object_list.begin(), object_list.end(), obj), object_list.end());
    //object_list.remove(obj);
	if (active_object == obj)
		active_object = nullptr;
	delete obj;
	repaint();
}

//  Remove all the graphical objects from the display list
//  Frees up the memory allocated for the object
void ObjectWindow::remove_all() {
	for (auto& p : object_list)
        delete p;
    object_list.clear();
	active_object = nullptr;
	repaint();
}

//  Return the first graphical object in the display list that is hit
//  by the given point (x,y).  If no objects in the list intersect the 
//  given point, return null.
GraphicalObject *ObjectWindow::hit(double x, double y) const {
    for (auto p = object_list.rbegin(); p != object_list.rend(); p++)
        if ((*p)->hit(x, y))
            return *p;
    return nullptr;   //  No object hit
}

/**
 *  Returns an iterator to the begining of the vector of 
 *  graphical objects this window contains.
 *  @return  an iterator to the begining of the vector of 
 *           graphical objects this window contains.
 */
std::vector<GraphicalObject *>::iterator ObjectWindow::begin() {
    return object_list.begin();
}

/**
 *  Returns an iterator just past the end of the vector of 
 *  graphical objects this window contains.
 *  @return  an iterator just past the end of the vector of 
 *           graphical objects this window contains.
 */
std::vector<GraphicalObject *>::iterator ObjectWindow::end() {
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
unsigned short Pixmap::get_short(std::ifstream& fin) { 
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
unsigned long Pixmap::get_long(std::ifstream& fin) {
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
bool Pixmap::power_of_2(int n) {
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
bool Pixmap::read_BMP_file(const char *fname) {  
    std::ifstream fin;

    bool result = false;  //  Unsuccessful by default

	// Read into memory an mRGB image from an uncompressed BMP file.
	// return false on failure and true on success

	// open input file to read binary char's
	fin.open(fname, std::ios::in|std::ios::binary);

    if ( !fin.good() )  //  File opened successfully?
        std::cout << " can't open file: " << fname << '\n';
    else {   //  Continue if file was opened sucessfully
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

        if (bitsPerPixel != 24)   // Error - must be a 24 bit uncompressed image
            std::cout << "Not a 24 bit/pixelimage, or is compressed!\n";
        else {   //  Bits per pixel OK
            //  Check for length of sides--must be a power of two
            if (!power_of_2(numRows) || !power_of_2(numCols)) 
                std::cout << "Length of sides must be powers of two\n";
            else {   // add bytes at end of each row so total # is a multiple of 4
                // round up 3*numCols to next multiple of 4
                int nBytesInRow = ((3 * numCols + 3)/4) * 4,
                    numPadBytes = nBytesInRow - 3 * numCols;	// need this many

                // set class's data members
                nRows = numRows;
                nCols = numCols;

                //make space for array
                pixel = new RGB[nRows * nCols];

                if ( !pixel )  // Check for proper memory allocation
                    std::cout << "Out of memory!\n";
                else {
                    long count = 0;
                    for (int row = 0; row < nRows; row++) { // read pixel values
                        for (int col = 0; col < nCols; col++) {
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
                        for(int k = 0 ; k < numPadBytes ; k++)
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
bool Pixmap::make_checkerboard() {  
    bool result = false;  //  Unsuccessful by default
	// make a checkerboard patten
	nRows = nCols = 64;
	pixel = new RGB[3 * nRows * nCols];
 
    if (!pixel) {   //  Unable to allocate memory
        std::cout << "out of memory!\n";
    }
    else {
        long count = 0;
        for (int i = 0; i < nRows; i++) {
            for (int j = 0; j < nCols; j++) {
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

void Pixmap::set_texture(GLuint textureName) {
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
BitmapObject::BitmapObject(const char *filename, double x, double y, 
       double width, double height): GraphicalObject(x, y, width, height) {
    pix.read_BMP_file(filename);
    glGenTextures(1, &texture_id);
    pix.set_texture(texture_id);    // create texture
}

/**
 *  Draws the bitmap image within the window.
 */
void BitmapObject::paint() const {
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
CompositeObject::CompositeObject(): 
     GraphicalObject(0.0, 0.0, 0.0, 0.0), 
     objects(0) {}

/*
	 * The destructor frees up all contained objects.
	 */
CompositeObject::~CompositeObject() {
	for (auto& obj : objects)
		delete obj;
}


/**
 *  Draws each object that makes up the composite
 *  object.
 *  @return nothing.
 */
void CompositeObject::paint() const {
    //  Debugging: Draw container's bounding box
    set_color(BLUE);
    draw_rectangle(left, bottom, width, height);
    //  Draw each object in this container
    for (auto& obj : objects)
        obj->paint();
}

/**
 *  Repositions the lower-left corner of the composite object's
 *  boundng box to (x,y).  The positions of the contained
 *  graphical objects are updated accordingly.
 */
void CompositeObject::move_to(double x, double y) {
    double old_left = left, old_bottom = bottom;
    GraphicalObject::move_to(x, y);
    double dx = left - old_left, dy = bottom - old_bottom;
    for (auto& obj : objects) {
        obj->move_to(obj->get_left() + dx, obj->get_bottom() + dy);
    }
}

/**
 *  Adds a graphical object to this container.  Adjusts
 *  the container's size accordingly.  Clients ordinarily call 
 *  add method instead; the add method allocates space for the added
 *  object and relieves the client of memory management.
 *  @param obj a pointer to the graphical object to add.
 *  @return nothing.
 */
void CompositeObject::internal_add(GraphicalObject *obj) {
    //  Adjust this container's size, if necessary
    //  to accommodate the new element.
    if (objects.size() == 0) {  //  Add first element
        left = obj->get_left();
        bottom = obj->get_bottom();
        width = obj->get_width();
        height = obj->get_height();
    }
    else { //  Contains at least one element
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
OGLWindow::OGLWindow(const std::string&  title, int left, int top, 
                          int width, int height): 
                   Window(title, left, top, width, height) {}

/**
 *  Creates a window with a default position and size.
 *  @param title the text to appear within the window's titlebar
 */
OGLWindow::OGLWindow(const std::string&  title): 
                 Window(title, 0.0, 0.0, 0.0, 0.0) {}
/**
 *  Destroys a graphical window object.
 */
OGLWindow::~OGLWindow() {}

/**  
 *  Code executed before the call to paint
 *  @return nothing
 */
void OGLWindow::prepaint() {}

/**
 *  Code executed after the call to paint
 *  @return nothing
 */
void OGLWindow::postpaint() {}


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



void SevenSegmentDigit::update_segment_locations() {
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
SevenSegmentDigit::SevenSegmentDigit(Color color, double x, double y, double height): 
          current_value(0), color(color), x(x), y(y), height(height) {
    update_segment_locations();
}

/**
 *  Returns the x coordinate of the digit's lower-left corner.
 *  @return the x coordinate of the digit's lower-left corner.
 */
double SevenSegmentDigit::get_x() const {
    return x;
}

/**
 *  Returns the y coordinate of the digit's lower-left corner.
 *  @return the y coordinate of the digit's lower-left corner.
 */
double SevenSegmentDigit::get_y() const {
    return y;
}

/**
 *  Returns the digit's height.
 *  @return the digit's height.
 */
double SevenSegmentDigit::get_height() const {
    return height;
}

/**
 *  Draws the digit.
 */
void SevenSegmentDigit::paint() const {
    set_color(color);

    switch (current_value) {
    case 0:
        fill_polygon(segment_a, 4);
        fill_polygon(segment_b, 4);
        fill_polygon(segment_c, 4);
        fill_polygon(segment_d, 4);
        fill_polygon(segment_e, 4);
        fill_polygon(segment_f, 4);
        break;
    case 1:
        fill_polygon(segment_b, 4);
        fill_polygon(segment_c, 4);
        break;
    case 2:
        fill_polygon(segment_a, 4);
        fill_polygon(segment_b, 4);
        fill_polygon(segment_d, 4);
        fill_polygon(segment_e, 4);
        fill_polygon(segment_g, 6);
        break;
    case 3:
        fill_polygon(segment_a, 4);
        fill_polygon(segment_b, 4);
        fill_polygon(segment_c, 4);
        fill_polygon(segment_d, 4);
        fill_polygon(segment_g, 6);
        break;
    case 4:
        fill_polygon(segment_b, 4);
        fill_polygon(segment_c, 4);
        fill_polygon(segment_f, 4);
        fill_polygon(segment_g, 6);
        break;
    case 5:
        fill_polygon(segment_a, 4);
        fill_polygon(segment_c, 4);
        fill_polygon(segment_d, 4);
        fill_polygon(segment_f, 4);
        fill_polygon(segment_g, 6);
        break;
    case 6:
        fill_polygon(segment_a, 4);
        fill_polygon(segment_c, 4);
        fill_polygon(segment_d, 4);
        fill_polygon(segment_e, 4);
        fill_polygon(segment_f, 4);
        fill_polygon(segment_g, 6);
        break;
    case 7:
        fill_polygon(segment_a, 4);
        fill_polygon(segment_b, 4);
        fill_polygon(segment_c, 4);
        break;
    case 8:
        fill_polygon(segment_a, 4);
        fill_polygon(segment_b, 4);
        fill_polygon(segment_c, 4);
        fill_polygon(segment_d, 4);
        fill_polygon(segment_e, 4);
        fill_polygon(segment_f, 4);
        fill_polygon(segment_g, 6);
        break;
    case 9:
        fill_polygon(segment_a, 4);
        fill_polygon(segment_b, 4);
        fill_polygon(segment_c, 4);
        fill_polygon(segment_d, 4);
        fill_polygon(segment_f, 4);
        fill_polygon(segment_g, 6);
        break;
    }
}

/**
 *  Set's the value of the digit.
 *  The assigned value is modulo ten.
 *  @param value value to assign, modulo 10
 *  @return nothing
 */
void SevenSegmentDigit::set_value(int value) {
    current_value = value % 10;
}

/**
 *  Returns the digit's current value.
 *  @return the digit's current value.
 */
int SevenSegmentDigit::get_value() const {
    return current_value;
}

/**
 *  Increases the digit's value by 1, modulo 10.
 *  @return nothing
 */
void SevenSegmentDigit::increment() {
    current_value = (current_value + 1) % 10;
}

/**
 *  Decreases the digit's value by 1, modulo 10.
 *  @return nothing
 */
void SevenSegmentDigit::decrement() {
    current_value = (current_value == 0)? 9 : current_value - 1;
}

/**
 *  Changes the height of the digit by a given amount.  The digit's 
 *  width changes proportionally.
 *  @param the change in the height of the digit
 *  @return nothing
 */
void SevenSegmentDigit::resize(double inc) {
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
void SevenSegmentDigit::move_to(double x, double y) {
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
DisplayDigit::DisplayDigit(Color color, double x, double y, double height):
          GraphicalObject(x, y, height/2.0, height), led(color, x, y, height) {
    //  Nothing to do here
}

/**  Draws the digit. */
void DisplayDigit::paint() const {
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
void DisplayDigit::set_value(int value) {
    led.set_value(value);
}

/**
 *  Returns the digit's current value.
 *  @return the digit's current value.
 */
int DisplayDigit::get_value() const {
    return led.get_value();
}


/**
 *  Increases the digit's value by 1, modulo 10.
 *  @return nothing
 */
void DisplayDigit::increment() {
    led.increment();
}

/**
 *  Decreases the digit's value by 1, modulo 10.
 *  @return nothing
 */
void DisplayDigit::decrement() {
    led.decrement();
}

/**
 *  Changes the height of the digit by a given amount.  The digit's 
 *  width changes proportionally.
 *  @param the change in the height of the digit
 *  @return nothing
 */
void DisplayDigit::resize(double inc) {
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
void DisplayDigit::mouse_dragged(double x, double y) {
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
DoubleDigit::DoubleDigit(Color color, double x, double y, double height):
            GraphicalObject(x, y, height + 2, height), 
            tens(color, x, y, height), 
            ones(color, x + height/2.0 + 2, y, height),
            leading_zero(true), visible(true) {
    //  Nothing to do here
}

/**  Draws the digits. */
void DoubleDigit::paint() const {
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
void DoubleDigit::set_value(int value) {
    tens.set_value(value / 10);
    ones.set_value(value % 10);
}

/**
 *  Returns the current value.
 *  @return the current value.
 */
int DoubleDigit::get_value() const {
    return 10*tens.get_value() + ones.get_value();
}

/**
 *  Increases the digit's value by 1, modulo 100.
 *  @return nothing
 */
void DoubleDigit::increment() {
    if (ones.get_value() == 9)
        tens.increment();
    ones.increment();
}

/**
 *  Decreases the digit's value by 1, modulo 100.
 *  @return nothing
 */
void DoubleDigit::decrement() {
    if (ones.get_value() == 0)
        tens.decrement();
    ones.decrement();
}

/**
 *  Changes the height of the digits by a given amount.  The digits' 
 *  width changes proportionally.
 *  @param the change in the height of the digits
 *  @return nothing
 */
void DoubleDigit::resize(double inc) {
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
void DoubleDigit::mouse_dragged(double x, double y) {
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
void DoubleDigit::set_leading_zero(bool flag) {
    leading_zero = flag;
}

/**
 *  Makes the display visible or invisible.
 *  @param flag if true, the digits are visible
 *              otherwise, the digits are invisible.
 *  @return nothing
 */
void DoubleDigit::set_visible(bool flag) {
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
Multidigit::Multidigit(int n, Color color, double x, double y, double height):
            GraphicalObject(x, y, n*height/2.0, height),
            digits(n, SevenSegmentDigit(color, x, y, height)),
            //tens(color, x, y, height), 
            //ones(color, x + height/2.0 + 2, y, height),
            leading_zeros(true), visible(true) {
    double offset = height/2.0;
    for (int i = 0; i < n; i++)
        digits[i].move_to(x + i*offset, y);
}

/**  Draws the digits. */
void Multidigit::paint() const {
    /*
    set_color(BLACK);
    draw_rectangle(left, bottom, width, height);
    */
    if (visible) {
        int n = digits.size();
        int i = 0;
        if (!leading_zeros)  //  Skip leading zeros
            while (i < n - 1 && digits[i].get_value() == 0)
                i++;
        while (i < n) {
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
void Multidigit::set_value(int value) {
    const int divisor = 10;
    int n = digits.size();
    for (int i = 0; i < n; i++) {
        digits[n - i - 1].set_value(value % divisor);
        value /= divisor;
    }
}

/**
 *  Returns the current value.
 *  @return the current value.
 */
int Multidigit::get_value() const {
    int multiplier = 1;
    int result = 0;
    int n = digits.size();
    for (int i = n - 1; i >= 0; i--) {
        result += digits[i].get_value()*multiplier;
        multiplier *= 10;
    }
    return result;
}


void Multidigit::increment_helper(int n) {
    if (n >= 0) {
        digits[n].increment();
        if (digits[n].get_value() == 0)
            increment_helper(n - 1);
    }
}

void Multidigit::decrement_helper(int n) {
    if (n >= 0) {
        digits[n].decrement();
        if (digits[n].get_value() == 9)
            decrement_helper(n - 1);
    }
}

/**
 *  Increases the display's value by 1
 *  @return nothing
 */
void Multidigit::increment() {
    increment_helper(digits.size() - 1);
}

/**
 *  Decreases the display's value by 1
 *  @return nothing
 */
void Multidigit::decrement() {
    decrement_helper(digits.size() - 1);
}

/**
 *  Changes the height of the display by a given amount.  The
 *  display's width changes proportionally.
 *  @param the change in the height of the digits
 *  @return nothing
 */
void Multidigit::resize(double inc) {
    int n = digits.size();
    digits[0].resize(inc);
    double offset = digits[0].get_height()/2.0;    
    for (int i = 1; i < n; i++) {
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
void Multidigit::mouse_dragged(double x, double y) {
    GraphicalObject::mouse_dragged(x, y);
    int n = digits.size();
    double offset = height/2.0;
    for (int i = 0; i < n; i++)
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
void Multidigit::move_to(double x, double y) {
    double old_left = left, 
           old_bottom = bottom;
    GraphicalObject::move_to(x, y);
    double dx = left - old_left, 
           dy = bottom - old_bottom;
    int n = digits.size();
    for (int i = 0; i < n; i++)
        digits[i].move_to(digits[i].get_x() + dx,
                       digits[i].get_y() + dy);
}

/**
 *  Turns on or off the display of a leading zero.
 *  @param flag if true, a leading zero is displayed;
 *              otherwise, a leading zero is not displayed.
 *  @return nothing
 */
void Multidigit::set_leading_zeros(bool flag) {
    leading_zeros = flag;
}

/**
 *  Makes the display visible or invisible.
 *  @param flag if true, the digits are visible
 *              otherwise, the digits are invisible.
 *  @return nothing
 */
void Multidigit::set_visible(bool flag) {
    visible = flag;
}



/**  
 *  Draw the hours-minutes or minutes-seconds colon separator.  
 *  @param left_ones the ones digit of the left quantity
 *  @param right_tens the tens digit of the right quantity
 *  @return nothing
 */
void TimeDisplay::draw_separator(const SevenSegmentDigit& left_ones, 
                                      const SevenSegmentDigit& right_tens) const {
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
void TimeDisplay::paint_without_leading_units() const {
    if ( hours_tens.get_value() > 0 )  //  Need to display tens of hours?
        hours_tens.paint(); //  Draw the hours 10s digit
    if (seconds >= 3600) {  //  Need to display hours?
        hours_ones.paint();
        draw_separator(hours_ones, minutes_tens);
    }

    if (seconds >= 3600 || minutes_tens.get_value() != 0)  //  Need to display minutes?
        minutes_tens.paint();    //  Draw the minutes 10s digit
    if (seconds >= 3600 || minutes_ones.get_value() != 0 || minutes_tens.get_value() != 0) {
        minutes_ones.paint();  //  Draw the minutes 1s digit
        draw_separator(minutes_ones, seconds_tens);
    }

    //  Draw the seconds digits
    if (seconds >= 60 || seconds_tens.get_value() != 0)  //  Need to display 10s of seconds?
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
TimeDisplay::TimeDisplay(Color color, double x, double y, int height): 
      GraphicalObject(x, y, 6*height/2.0 + 2*height/4.0, height),
      hours_tens(color, x, y, height), 
      hours_ones(color, x + height/2.0, y, height),
      minutes_tens(color, x + 2*height/2.0 + height/4.0, y, height), 
      minutes_ones(color, x + 3*height/2.0 + height/4.0, y, height),
      seconds_tens(color, x + 4*height/2.0 + 2*height/4.0, y, height), 
      seconds_ones(color, x + 5*height/2.0 + 2*height/4.0, y, height),
      seconds(0), leading_units(true), visible(true) {}



/**  Draws the display. 
 *  @return nothing
 */
void TimeDisplay::paint() const {
    set_color(BLUE);
    draw_rectangle(left, bottom, width, height);
    if (visible) {
        if (leading_units) {
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
void TimeDisplay::set_value(int sec)
{
    sec %= 360000;             //  Upper limit is 99:59:59 (359999 secs)
    sec = (sec < 0)? 0 : sec;
    if (sec != seconds) {
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
int TimeDisplay::get_value() const {
    return seconds;
}

/**
 *  Increases the time by one second.
 *  @return nothing
 */
void TimeDisplay::increment() {
    set_value(get_value() + 1);
}

/**
 *  Decreases the time by one second.
 *  @return nothing
 */
void TimeDisplay::decrement() {
    set_value(get_value() - 1);
}

/**
 *  Changes the height of the digits by a given amount.  The digits' 
 *  width changes proportionally.
 *  @param the change in the height of the digits
 *  @return nothing
 */
void TimeDisplay::resize(double inc) {
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
void TimeDisplay::mouse_dragged(double x, double y) {
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
void TimeDisplay::move_to(double x, double y) {
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
void TimeDisplay::set_leading_units(bool flag) {
    leading_units = flag;
}

/**
 *  Makes the display visible or invisible.
 *  @param flag if true, the digits are visible
 *              otherwise, the digits are invisible.
 *  @return nothing
 */
void TimeDisplay::set_visible(bool flag) {
    visible = flag;
}



/*****************************************************
 *  Graphical object code
 *****************************************************/


//  Constructor initializes the bounding box of the graphical object and
//  assigns a unique ID number to the object.
GraphicalObject::GraphicalObject(double lf, double bm, 
                                          double wd, double ht):
                           window(0), left(lf), bottom(bm), 
						   width(wd), height(ht), 
                           cursor(CursorShape::Crosshair),
                           mouse_over(false), id(id_source++) {
	std::cout << "Making a graphical object #" << id << '\n';;
	//std::cout << "left = " << left << ", bottom = " << bottom << '\n';
}

//  Copy constructor makes a new graphical object with the same
//  characteristics except it has a unique ID
GraphicalObject::GraphicalObject(const GraphicalObject &go):
                 left(go.left), bottom(go.bottom),
                 width(go.width), height(go.height),
                 cursor(CursorShape::Crosshair),
                 mouse_over(false), id(id_source++) {}

//  Assignment operator copies all the graphical object
//  characteristics but does not affect the object's ID
GraphicalObject& GraphicalObject::operator=
                  (const GraphicalObject &go) {
    left = go.left;
    bottom = go.bottom;
    width = go.width;
    height = go.height;
    cursor = go.cursor;
    return *this;
}

//  Destructor currently does nothing
GraphicalObject::~GraphicalObject() {
	std::cout << "Destroying graphical object #" << id << '\n';
}



double GraphicalObject::get_left() const {
	return left;
}

double GraphicalObject::get_bottom() const {
	return bottom;
}

double GraphicalObject::get_width() const {
	return width;
}

double GraphicalObject::get_height() const {
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
void GraphicalObject::set(double x, double y, 
                               double width, double height) {
    left = x;
    bottom = y;
    this->width = width;
    this->height = height;
}



//  Determines if the point (x,y) falls within the bounding box of this 
//  graphical object?
bool GraphicalObject::hit(double x, double y) {
    if (x >= left && x <= left + width 
         && y >= bottom && y <= bottom + height) {
        x_hit_offset = x - left;  
        y_hit_offset = y - bottom;
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

void GraphicalObject::mouse_dragged(double x, double y) {
    //left = x - x_hit_offset;   
    //bottom = y - y_hit_offset;
    move_to(x - x_hit_offset, y - y_hit_offset);
}

void GraphicalObject::move_to(double x, double y) {
    left = x;   
    bottom = y;
}

void GraphicalObject::set_mouse_over(bool flag) {
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
void GraphicalObject::mouse_pressed(double, double, MouseButton)
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
void GraphicalObject::mouse_released(double, double, MouseButton)
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
void GraphicalObject::mouse_moved(double, double)
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
void GraphicalObject::key_pressed(int, double, double)
{}



Window *GraphicalObject::set_window(ObjectWindow *win) {
	Window *previous = window;
    window = win;
	return previous;
}

Window *GraphicalObject::get_window() const {
	return window;
}

 /**
  *  Sets the graphical object's cursor shape.
  *  @param cursor the object's cursor shape
  *  @return the previous cursor shape
  */
CursorShape GraphicalObject::set_cursor(CursorShape cursor) {
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
CursorShape GraphicalObject::get_cursor() {
    return cursor;
}



unsigned GraphicalObject::id_source = 0;


/*****************************************************
 *  Popup menu code
 *****************************************************/

static PopupMenu *current_popup_menu = nullptr;

void PopupMenu::execute_handler(int n) {
	(items[n].code)();
}

void PopupMenu::process_menu_events(int option) {
	current_popup_menu->execute_handler(option);
}

PopupMenu::PopupMenu() {
	index = glutCreateMenu(PopupMenu::process_menu_events);
	current_popup_menu = this;
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

PopupMenu::~PopupMenu() {}

void PopupMenu::add_menu_item(const std::string& item, MenuItemFunction f) {
	glutAddMenuEntry(item.c_str(), items.size());
	items.push_back(MenuItem(item, f));
}

void PopupMenu::replace_menu_item(const std::string& old_name, 
		                                const std::string& new_name, 
								        //WindowCallback func) {
								        MenuItemFunction func) {
    int num_items = items.size();
    MenuItem new_item(new_name, func);
    for (int i = 0; i < num_items; i++)
        if (old_name == items[i].name) {
            items[i] = new_item;
            glutChangeToMenuEntry(i + 1, new_name.c_str(), i);
            return;
        }
	//std::cout << "replace_menu_item unimplemented at this time" << '\n';
}


void PopupMenu::remove_menu_item(const std::string&  /* item */) {
	std::cout << "remove_menu_item unimplemented at this time\n";
    //  Find index of current . . . 
}

/**
	 *  Make this popup menu the active popup for its window.
	 *  @return nothing
	 */
void PopupMenu::activate() {
	current_popup_menu = this;
	glutSetMenu(index);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}




/********************************************************
 *  Global drawing functions
 ********************************************************/

/*
 *  set_color
 *      Sets the current drawing color to the RGB values specified 
 *      by red, green, and blue.
 */
void set_color(const Color& color) {
	glColor3d(color.red, color.green, color.blue);
}

void set_color(double r, double g, double b) {
	glColor3d(r, g, b);
}

void set_line_width(double width) {
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
bool intersect(const GraphicalObject& obj1, 
                    const GraphicalObject& obj2) {
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
void draw_point(double x, double y) {
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
void draw_point(const Point& pt) {
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
void set_point_size(int point_size) {
    glPointSize(static_cast<GLfloat>(point_size));
}


/*
 *  draw_line
 *      Draws a line from (x0,y0) to (x1,y1) in the current drawing color.
 */
void draw_line(double x0, double y0, double x1, double y1) {
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();
}

void draw_dashed_line(double x0, double y0, double x1, double y1, 
						   int pattern) {
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
void draw_rectangle(double x, double y, double width, double height) {
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
void fill_rectangle(double x, double y, double width, double height) {
	glBegin(GL_POLYGON);
    glVertex2d(x, y);
	glVertex2d(x + width, y);
	glVertex2d(x + width, y + height);
	glVertex2d(x, y + height);
	glEnd();
}

static const double DEG_TO_RAD = 3.1415926/180.0;

void draw_circle(double x, double y, double radius) {
	const int NUM_SEGMENTS = 360;
	glBegin(GL_LINE_LOOP); 
	for (int deg = 0; deg < NUM_SEGMENTS; deg++) { 
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
void fill_circle(double x, double y, double radius) {
	const int NUM_SEGMENTS = 360;
    double y1 = y;
    double x1 = x;
    glBegin(GL_TRIANGLES);  
    for(int deg = 0; deg <= NUM_SEGMENTS; deg++) {
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
void draw_polygon(const std::vector<Point>& pts) {
	glBegin(GL_LINE_LOOP);
	for (auto& p : pts)
		glVertex2d(p.x, p.y);
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
void draw_polygon(const Point *pts, int n) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++)
		glVertex2d(pts[i].x, pts[i].y);
	glEnd();

}


//  Draw a filled polygon
void fill_polygon(const std::vector<Point>& pts) {
	glBegin(GL_POLYGON);
	for (auto& p : pts)
		glVertex2d(p.x, p.y);
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
void fill_polygon(const Point *pts, int n) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
		glVertex2d(pts[i].x, pts[i].y);
	glEnd();
}

void draw_text(const std::string& text, double x, double y, int font_size) {
	void *font;
	switch (font_size) {
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
	const char *str = text.c_str();
	while (*str != '\0')
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
void draw_text(const std::string& text, double x, double y, double scale) {
    glPushMatrix();
    glTranslated(x, y, 0);
    glScaled(scale, scale, scale);
	const char *str = text.c_str();
	while (*str != '\0')
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *str++);
    glPopMatrix();
 
};



void draw_function(double (*f)(double), 
						double begin_x, double end_x,
						double increment) {
	glBegin(GL_LINE_STRIP);
	for (double x = begin_x; x <= end_x; x += increment)
		glVertex2d(x, f(x));
	glEnd();
}







/**
 *  Gets the width of the screen in pixels.
 *  @return the width of the screen in pixels; returns zero
            if the screen's width cannot be determined
 */
int get_screen_width() {
	return glutGet(GLUT_SCREEN_WIDTH);
}

/**
 *  Gets the height of the screen in pixels.
 *  @return the height of the screen in pixels; returns zero
            if the screen's height cannot be determined
 */
int get_screen_height() {
	return glutGet(GLUT_SCREEN_HEIGHT);
}

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
inline void run(Args&&... args) {
    //std::make_shared<T>(args...)->run();
    T{args...}.run();
}

/**
 *  Creates an SGL window of type T passing the arguments
 *  found in the Args parameter to the window's constructor.
 *  Does NOT call the window's run method to start the application.
 *  @tparam T the type of SGL window to create
 *  @tparam Args the argument pack to pass to the window's constructor
 *  @param args the instantiated parameters to pass to the
 *  window's constructor
 *  @return a pointer to the newly created window object.
 */
template <typename T, typename... Args>
inline T *make_window(Args&&... args) {
	return new T(args...);
}

/****************************************
 *  Utility functions
 ****************************************/


//  Creates a Stopwatch object
//  A newly minted object is not running and is in a "reset" state
Stopwatch::Stopwatch(): start_time(0), end_time(0), running(false) {}

//  Starts the stopwatch.  If there is no current
//  start time, the stopwatch has not been used or 
//  has just been reset, so we record the current
//  system time to begin a new timing.  If there is
//  a valid start time, we merely resume the current timing.
//  Puts the stopwatch object in the 'running' state.
void Stopwatch::start() {
    if (start_time == 0)     //  Need to start with a fresh timing?
        start_time = clock();  //  If so, record start time
    running = true;            //  The clock is running
}

//  Stops the stopwatch.  Records the current
//  system time and puts the stopwatch object in
//  the 'not running' state. 
//  If the stopwatch is already stopped, a diagnostic
//  message is issued, but the stored elapsed time is
//  unaffected.
void Stopwatch::stop() {
    if (running) {
        end_time = clock();
        running = false;
    }
    else
        std::cout << "Stopwatch is not running\n";
}

//  Reports the time elapsed between the
//  stopwatch's starting and stopping times.
//  If elapsed is called while the stopwatch
//  is running, the time elapsed since the start
//  time is reported,
double Stopwatch::elapsed() const {
    if (running)
        return clock() - start_time;
    else
        return static_cast<double>(end_time - start_time)/CLOCKS_PER_SEC;
}

//  Resets the stopwatch so a subsequent start begins recording 
//  a new time.  An attempt to  reset a running stopwatch 
//  produces an error message.
void Stopwatch::reset() {
    if (!running)
        start_time = end_time = 0;  //  Reset times
    else
        std::cout << "Error: Cannot reset a stopwatch that is running\n";
}

//  Pauses the program's execution for a period of 
//  milliseconds.
void pause(int msec) {
    if (msec > 0) {
        clock_t start_time = clock();
        while (clock() - start_time < msec)
            continue;
    }
}



/*  Pseudorandom number routines  */


void set_random_seed(int seed) {
	if (seed < 0) {
		seed = seed;  //  Fix this
	}
	srand(static_cast<unsigned>(seed));
}

//  Returns a pseudorandom number in the 
//  range begin...end
int random(int begin, int end) {
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
int random(int n) {
    return rand() % n;
}


//  Determines if |d1 - d2| < delta; in other words,
//  d1 and d2 are close enough to be considered "equal"
bool equals(double d1, double d2, double delta) {
    return d1 == d2 || fabs(d1 - d2) < delta;
}

// Modern C++ provides standard functions for these:
//static std::ostringstream internal_to_string_stream;
//static std::string internal_to_string_buffer;
//
///**
// *  Converts an integer to a string so it can be displayed
// *  with draw_text.
// *  @param i the integer to convert
// *  @return the string representation of i
// */
//std::string to_string(int i) {
//    internal_to_string_stream.str("");
//    internal_to_string_stream << i;
//    internal_to_string_buffer = internal_to_string_stream.str();
//    return internal_to_string_buffer; // .c_str();
//
//}
//
///**
// *  Converts a double to a string so it can be displayed
// *  with draw_text.
// *  @param d the double to convert
// *  @return the string representation of d
// */
//std::string to_string(double d) {
//    internal_to_string_stream.str("");
//    internal_to_string_stream << d;
//    internal_to_string_buffer = internal_to_string_stream.str();
//    return internal_to_string_buffer; // .c_str();
//}


//-----------------------------------------------------------------------------------------
// Procedural interface to the SGL

// The default event handlers do nothing
void procedural_default_paint_function() {}
void procedural_default_mouse_pressed_function(double, double, MouseButton) {}
void procedural_default_mouse_released_function(double, double, MouseButton) {}
void procedural_default_mouse_moved_function(double, double) {}
void procedural_default_mouse_dragged_function(double, double) {}
void procedural_default_key_pressed_function(int, double, double) {}

class ProceduralWindow : public ObjectWindow {
public:
    std::function<void()> paint_function;
    std::function<void(double, double, MouseButton)> mouse_pressed_function;
    std::function<void(double, double, MouseButton)> mouse_released_function;
    std::function<void(double, double)> mouse_moved_function;
    std::function<void(double, double)> mouse_dragged_function;
    std::function<void(int, double, double)> key_pressed_function;

    ProceduralWindow(const std::string& title, double x, double y, double width, double height) :
        ObjectWindow(title, x, y, width, height),
        mouse_pressed_function(procedural_default_mouse_pressed_function),
        mouse_released_function(procedural_default_mouse_released_function),
        mouse_moved_function(procedural_default_mouse_moved_function),
        mouse_dragged_function(procedural_default_mouse_dragged_function),
        key_pressed_function(procedural_default_key_pressed_function) {}

    void paint() override {
        paint_function();
    }

    void mouse_pressed(double x, double y, MouseButton b) override {
        mouse_pressed_function(x, y, b);
    }

    void mouse_released(double x, double y, MouseButton b) override {
        mouse_released_function(x, y, b);
    }

    void mouse_moved(double x, double y) override {
        mouse_moved_function(x, y);
    }

    void mouse_dragged(double x, double y) override {
        mouse_dragged_function(x, y);
    }

    void key_pressed(int k, double x, double y) override {
        key_pressed_function(k, x, y);
    }
};


// Global window used by the procedural code
//static std::shared_ptr<ProceduralWindow> global_procedural_window;
static ProceduralWindow *global_procedural_window;

static void exit_error(const std::string& message) {
    std::cout << message << '\n';
    exit(1);
}

void create_window(const std::string& title, int x, int y, int width, int height) {
    if (global_procedural_window)
        exit_error("An application can create only one graphics window");
    //global_procedural_window = std::make_shared<ProceduralWindow>(title, x, y, width, height);
    global_procedural_window = make_window<ProceduralWindow>(title, x, y, width, height);
}

void run_window() {
    if (!global_procedural_window)
        exit_error("Cannot run_window: No graphics window exists (use create_window)");
    global_procedural_window->run();
}

void update_window() {
    if (!global_procedural_window)
        exit_error("Cannot update_window: No graphics window exists (use create_window)");
    global_procedural_window->repaint();
}

void set_paint_function(const std::function<void()>& f) {
    if (!global_procedural_window)
        exit_error("Cannot set_paint_function: No graphics window exists (use create_window)");
    global_procedural_window->paint_function = f;
}

void set_mouse_pressed_function(const std::function<void(double, double, MouseButton)>& f) {
    if (!global_procedural_window)
        exit_error("Cannot set_mouse_pressed_function: No graphics window exists (use create_window)");
    global_procedural_window->mouse_pressed_function = f;

}

void set_mouse_released_function(const std::function<void(double, double, MouseButton)>& f) {
    if (!global_procedural_window)
        exit_error("Cannot set_mouse_released_function: No graphics window exists (use create_window)");
    global_procedural_window->mouse_released_function = f;
}

void set_mouse_moved_function(const std::function<void(double, double)>& f) {
    if (!global_procedural_window)
        exit_error("Cannot set_mouse_moved_function: No graphics window exists (use create_window)");
    global_procedural_window->mouse_moved_function = f;
}

void set_mouse_dragged_function(const std::function<void(double, double)>& f) {
    if (!global_procedural_window)
        exit_error("Cannot set_mouse_dragged_function: No graphics window exists (use create_window)");
    global_procedural_window->mouse_dragged_function = f;
}

void set_key_pressed_function(const std::function<void(int, double, double)>& f) {
    if (!global_procedural_window)
        exit_error("Cannot set_key_pressed_function: No graphics window exists (use create_window)");
    global_procedural_window->key_pressed_function = f;
}

void set_window_background(const Color& c) {
    if (!global_procedural_window)
        exit_error("Cannot set_window_background: No graphics window exists (use create_window)");
    global_procedural_window->set_background_color(c);
}

void set_window_title(const std::string& str) {
    if (!global_procedural_window)
        exit_error("Cannot set_window_title: No graphics window exists (use create_window)");
    global_procedural_window->set_title(str);
}



}   // End of namespace sgl

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif
