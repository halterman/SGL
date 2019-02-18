#include <GL/sgl.hpp>

//  Signals are color independent
enum class Signal { Off, Stop, Caution, Go };

//  Trafficlight logic base class
class TrafficLightLogic {
protected: 
    Signal signal;     //  Current state of the controller

public:
    TrafficLightLogic(Signal initial_signal): signal(initial_signal) {}
    
    //  Derived classes must provide their own custom logic
    virtual void change() = 0;

    //  Changes to state to the next one in the controller's pattern
    Signal get_signal() const {
        return signal;
    }
};

//  Controller for a standard trafficlight sequence:
//     +----------------+
//     |                |
//     v                |
//    red -> green -> yellow

class StandardLogic: public TrafficLightLogic {
public:
    //  Standand trafficlights begin red
    StandardLogic(): TrafficLightLogic(Signal::Stop) {}

    void change() override {
        switch (signal) {
            case Signal::Stop:    
                signal = Signal::Go;       //  Red goes to green   
                break;
            case Signal::Go:      
                signal = Signal::Caution;  //  green goes to yellow
                break;
            case Signal::Caution: 
                signal = Signal::Stop;     //  Yellow goes to red
                break;
        }
    }
};

//  Controller for a flashing caution light sequence
//  yellow, off, yellow, off, ...
class CautionLogic: public TrafficLightLogic {
public:
    //  Flashing caution lights begin yellow
    CautionLogic(): TrafficLightLogic(Signal::Caution) {}

    void change() override {
        switch (signal) {
            case Signal::Off:    
                signal = Signal::Caution;      
                break;
            case Signal::Caution:      
                signal = Signal::Off; 
                break;
        }
    }
};

class TrafficLightWindow: public sgl::Window {
protected:
    TrafficLightLogic *controller;  //  Currently active controller
    CautionLogic caution_ctrl;      //  Controller for flashing yellow
    StandardLogic standard_ctrl;    //  Constroller for standard signal sequence

public:
    //  Trafficlights by default follow the standard sequence
    TrafficLightWindow(): 
         sgl::Window("Trafficlight", 200, 100, 300, 600, -60.0, 60.0, -120.0, 120.0),
         controller(&standard_ctrl) {}

    //  Render the trafficlight in the window
    void paint() override {
        //  Draw frame
        sgl::set_color(sgl::GRAY);
        sgl::fill_rectangle(-45.0, -110.0, 90.0, 220.0);
        //  Draw red lamp
        if (controller->get_signal() == Signal::Stop)
            sgl::set_color(sgl::RED);
        else
            sgl::set_color(sgl::DARK_GRAY);
        sgl::fill_circle(0.0, 65.0, 28.0);
        //  Draw yellow lamp
        if (controller->get_signal() == Signal::Caution)
            sgl::set_color(sgl::YELLOW);
        else
            sgl::set_color(sgl::DARK_GRAY);
        sgl::fill_circle(0.0, 0.0, 28.0);
        //  Draw green lamp
        if (controller->get_signal() == Signal::Go)
            sgl::set_color(sgl::GREEN);
        else
            sgl::set_color(sgl::DARK_GRAY);
        sgl::fill_circle(0.0, -65.0, 28.0);
        //draw_axes(10, 10);
    }

    //  Pressing and releasing the mouse button changes the light
    void mouse_released(double, double, sgl::MouseButton) override {
        controller->change();
        repaint();
    }

    //  Pressing the 'C' puts the light in flashing yellow mode; pressing
    //  the 'S' key puts the light into red-green-yellow mode
    void key_pressed(int key, double x, double y) override {
        switch (key) {
            case 'C':
            case 'c':
                controller = &caution_ctrl;
                break;
            case 'S':
            case 's':
                controller = &standard_ctrl;
                break;
        }
        sgl::Window::key_pressed(key, x, y);
        repaint();
    }

    //  User may not resize the window; it's always 300x600
    void resized(int w, int h) override {
        if (w == 300 && h == 600)
            sgl::Window::resized(w, h);
        else
            set_window_size(300, 600);
    }
};

//  Simply create a trafficlight window object and run it
int main() {
    sgl::run<TrafficLightWindow>();
}
