#include <cmath>
#include <GL/sgl.hpp>
#include <iostream>

//  Some constants to compute the points of a 5-point star
static const double DEGREES_TO_RADIANS = 3.14159/180.0,
                    ANGLE = 360.0/5.0,
                    SIN = sin(ANGLE*DEGREES_TO_RADIANS),
                    COS = cos(ANGLE*DEGREES_TO_RADIANS);

class VariousObjectsWindow: public sgl::ObjectWindow {
protected:
    class Shape: public sgl::GraphicalObject {
    protected:
        sgl::Color color;
    public:
        Shape(double x, double y, double width, double height, sgl::Color color):
              sgl::GraphicalObject(x - width/2.0, y - height/2.0, width, height), color(color) {
            //  Shapes by default use a crosshairs-shaped cursor
            set_cursor(sgl::CursorShape::Crosshair);
        }
        //  Shape is abstract because pure virtual paint method is not overridden
    };

    class CircularShape: public Shape {
    protected:
        //  Controls the user's ability to move the object 
        bool is_locked;
    public:
        CircularShape(double x, double y, double diameter, sgl::Color color):
          Shape(x, y, diameter, diameter, color), is_locked(false) {} 
  
        void paint() const override {
            set_color(color);
            //  Locked circles drawn as outlines
            if (is_locked)
                sgl::draw_circle(left + width/2.0, bottom + height/2.0, width/2.0);
            else
                sgl::fill_circle(left + width/2.0, bottom + height/2.0, width/2.0);
            //  When the mouse is over this circle draw a ring  around it
            if (mouse_over)
                sgl::draw_circle(left + width/2.0, bottom + height/2.0, width/2.0 + 5.0);
        }

        //  Pressing the 'L' key toggles locking and unlocking
        //  this circle; the user cannot move a locked circle
        void key_pressed(int key, double x, double y) override {
            if (key == 'L' || key == 'l') {
                is_locked = !is_locked;
                Shape::key_pressed(key, x, y);
                window->repaint();
            }
			else if (key == 'D' || key == 'd') {
				window->remove(this);
			}
        }

        //  Moves a circle object if it currently is not locked
        void move_to(double x, double y) override {
            if (!is_locked)
                Shape::move_to(x, y);
        }
    };

    class RectangularShape: public Shape {
    public:
        RectangularShape(double x, double y, double width, double height, sgl::Color color):
                            Shape(x, y, width, height, color) {
            //  Rectangle objects use the standard cursor instead of the default crosshairs cursor 
            //  inherited from Shape 
            set_cursor(sgl::CursorShape::Right_arrow);
        } 
  
        void paint() const override {
            sgl::set_color(color);
            //  When the mouse is over a rectangle the rectangle is rendered as an outline
            if (mouse_over)
                sgl::draw_rectangle(left, bottom, width, height);
            else
                sgl::fill_rectangle(left, bottom, width, height);
        }
    };

    class StarShape: public Shape {
    protected:
        //  Points locating the tips of the star points
        sgl::Point p1, p2, p3, p4, p5;
    public:
        //  The constructor computes the relative locations of the star's tips
        //  Since stars cannot be resized, the relative locations
        //  are based on the star's size.  Since the user can move
        //  a star to a new location within the viewport, the
        //  constructor cannot precompute 
        StarShape(double x, double y, double diameter, sgl::Color color):
                         Shape(x, y, diameter, diameter, color),
                         p1(0.0, diameter/2.0), 
                         p2(p1.x*COS - p1.y*SIN, p1.x*SIN + p1.y*COS),
                         p3(p2.x*COS - p2.y*SIN, p2.x*SIN + p2.y*COS),
                         p4(p3.x*COS - p3.y*SIN, p3.x*SIN + p3.y*COS),
                         p5(p4.x*COS - p4.y*SIN, p4.x*SIN + p4.y*COS) {}

        //  The paint method computes the absolute locations of the star's 
        //  tips based on the star's location
        void paint() const override {
            sgl::set_color(color);
            double x_offset = left + width/2.0,
                   y_offset = bottom + height/2.0;
            sgl::draw_line(p1.x + x_offset, p1.y + y_offset, 
                           p3.x + x_offset, p3.y + y_offset);
            sgl::draw_line(p3.x + x_offset, p3.y + y_offset, 
                           p5.x + x_offset, p5.y + y_offset);
            sgl::draw_line(p5.x + x_offset, p5.y + y_offset, 
                           p2.x + x_offset, p2.y + y_offset);
            sgl::draw_line(p2.x + x_offset, p2.y + y_offset, 
                           p4.x + x_offset, p4.y + y_offset);
            sgl::draw_line(p4.x + x_offset, p4.y + y_offset, 
                           p1.x + x_offset, p1.y + y_offset);

            //  Uncomment for accessory drawing
            //set_color(LIGHT_GRAY);
            //draw_rectangle(left, bottom, width, height);
            //draw_circle(left + width/2.0, bottom + height/2.0, width/2.0);
        }
    };

public:
    VariousObjectsWindow(): sgl::ObjectWindow("Graphical Objects", 100, 100, 800, 600,
                                              0.0, 799.0, 0.0, 599.0) {
        sgl::set_random_seed();  //  Use a seed based on system time
    }

    //  Provide a concrete paint that does nothing; the added shapes paint themselves
    void paint() override {}  
    
    void mouse_pressed(double x, double y, sgl::MouseButton button) override {
        
        if (button == sgl::MouseButton::Right) {
            int shape = sgl::random(0, 2),               //  0 = circle, 1 = rectangle, 2 = star
                obj_w = sgl::random(2, get_width()/5),   //  Random width
                obj_h = sgl::random(2, get_height()/5);  //  Random height
            double red = sgl::random(0, 1000)/1000.0,    //  Random color: red component
                   green = sgl::random(0, 1000)/1000.0,  //  green component
                   blue = sgl::random(0, 1000)/1000.0;   //  blue component
            sgl::Color obj_color(red, green, blue);
			if (shape == 0)
				add<CircularShape>(x, y, obj_w, obj_color); // add(new CircularShape(x, y, obj_w, obj_color));
            else if ( shape == 1 )
                add<RectangularShape>(x, y, obj_w, obj_h, obj_color);
            else
                add<StarShape>(x, y, obj_w, obj_color);
        }
        sgl::ObjectWindow::mouse_pressed(x, y, button);
    }

	void key_pressed(int key, double x, double y) override {
		if (key == 'r' || key == 'R') {
			remove_all();
		}
		ObjectWindow::key_pressed(key, x, y);
	}
};

int main() {
    sgl::run<VariousObjectsWindow>();
}
