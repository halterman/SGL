#include <GL/sgl.hpp>
#include <cmath>
#include <sstream>
#include <string>
#include <iomanip>


const double MIN_X = -100.0,
             MAX_X =  100.0,
             MIN_Y = -100.0,
             MAX_Y =  100.0;

const int FONT_SIZE = 18;

const int TIME_INCREMENT = 200;

const double PI = 3.141592653589793,  //  Close enough!
             HALF_PI = PI/2.0;

std::ostringstream oss;

sgl::Point minutes_to_position(int minutes, double offset) {
    double x = MAX_X/2.0 - offset, 
           y = 0.0,
           fraction = -PI*minutes/30 + HALF_PI,
           sine = sin(fraction), 
           cosine = cos(fraction);
    sgl::Point result;
    result.x = x*cosine - y*sine;
    result.y = x*sine   + y*cosine;
    return result;
}

sgl::Point hours_to_position(int hours, int minutes, double offset) {
    return minutes_to_position(static_cast<int>((hours + minutes/60.0)*5), offset);
}

std::string time_to_string(int hours, int minutes) {
    oss.str("");  //  Clear the output string
    oss.width(2);
    oss.fill(' ');
    oss << ((hours == 0)? 12 : hours) << ":";
    oss.width(2);
    oss.fill('0');
    oss << minutes;
    return oss.str();
}

class ClockWindow: public sgl::Window {
    int hours;
    int minutes;
public:
    ClockWindow(): sgl::Window("Clock", 100, 100, 500, 500,
        MIN_X, MAX_X, MIN_Y, MAX_Y), hours(0), minutes(0) {
        start_timer(TIME_INCREMENT);
    }

    void paint() override {
        //  Draw frame
        sgl::set_line_width(1);
        sgl::draw_circle(0.0, 0.0, MIN_X/2.0 - 5.0);
        //  Draw center
        sgl::fill_circle(0.0, 0.0, 2.0);
        //  Draw positions
        sgl::Point loc;
        for (int i = 0; i < 12; i++) {
            loc = hours_to_position(i, 0, 1.0);
            sgl::fill_rectangle(loc.x - 2.0, loc.y - 2.0, 4.0, 4.0);
        }
        //  Draw minute hand
        sgl::set_line_width(2);
        loc = minutes_to_position(minutes, 5.0);
        sgl::draw_line(0.0, 0.0, loc.x, loc.y);
        //  Draw hour hand
        sgl::set_line_width(5);
        loc = hours_to_position(hours, minutes, 20.0);
        sgl::draw_line(0.0, 0.0, loc.x, loc.y);
        //  Print digital text
        sgl::draw_text(time_to_string(hours, minutes), -5.0, MIN_Y + 5.0, 18);
    }

    void update_time() {
        minutes++;
        if (minutes == 60) {
            minutes = 0;
            hours++;
            if (hours == 12)
                hours = 0;
        }
        repaint();
    }

    void timer_expired() override {
        update_time();
        start_timer(TIME_INCREMENT);
    }

    void mouse_pressed(double, double, sgl::MouseButton) override {
        update_time();
    }

    void key_pressed(int key, double x, double y) override {
        update_time();
        Window::key_pressed(key, x, y);
    }

    void resized(int w, int h) override {
        if (w != h) {
            if (w < h)
                h = w;
            else
                w = h;
            set_window_size(w, h);
        }
        Window::resized(w, h);
    }
};

int main() {
    sgl::run<ClockWindow>();
}
