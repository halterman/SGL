#include <GL/sgl.hpp>
#include "geopoint.h"

static const int MAX_POINTS = 1048576,
                 MIN_POINTS = 256;

class SierpinskiChaotic: public sgl::ObjectWindow {
protected:
    sgl::Point midpoint(const sgl::Point& pt1, const GeoPoint *pt2) const {
        return sgl::Point((pt1.x + pt2->get_x())/2.0, (pt1.y + pt2->get_y())/2.0);
    }

    int points;

    GeoPoint *v0, *v1, *v2, *seed;
   
public:
    SierpinskiChaotic(): sgl::ObjectWindow("Sierpinski Triangle Chaotic", 100, 100, 600, 600,
                         0.0, 599.0, 0.0, 599.0), points(MIN_POINTS),
                         v0(nullptr), v1(nullptr), v2(nullptr), seed(nullptr) {
        set_background_color(sgl::BLACK);
    }

    void paint() override {
        sgl::set_color(sgl::WHITE);
        if (seed) {
            sgl::Point current(seed->get_x(), seed->get_y());
            sgl::CursorShape prev_cursor = set_cursor(sgl::CursorShape::Wait);
            for (int step = points; step > 0; step--) {
                sgl::Point next;
                switch (sgl::random(3)) {
                    case 0:
                        next = midpoint(current, v0);
                        break;
                    case 1:
                        next = midpoint(current, v1);
                        break;
                    case 2:
                        next = midpoint(current, v2);
                        break;
                }
                draw_point(next);
                current = next;
            }
            sgl::draw_text(std::to_string(points), 10.0, 10.0, 12);
            set_cursor(prev_cursor);
        }
    }

    void key_pressed(int key, double x, double y) override {
        switch (key) {
            case sgl::UP_KEY:
                points = (points >= MAX_POINTS) ? MAX_POINTS: 2*points;
                break;
            case sgl::DOWN_KEY:
                points = (points <= MIN_POINTS) ? MIN_POINTS : points/2;
                break;
            case 'C':
            case 'c':
                v0 = v1 = v2 = seed = nullptr;
                remove_all();
                break;
            default:
                sgl::Window::key_pressed(key, x, y);
        }
        repaint();
    }

    void mouse_released(double x, double y, sgl::MouseButton) override {
        if (!seed) {
            auto temp = add<GeoPoint>(x, y, sgl::YELLOW);
			if (!v0)
				v0 = temp;
			else if (!v1)
				v1 = temp;
			else if (!v2)
				v2 = temp;
            else if (!seed)
                seed = temp;
            repaint();
        }
    }
};

int main() {
    //  Set random number seed
    sgl::set_random_seed();
    //  Start event manager
    sgl::run<SierpinskiChaotic>();
}
