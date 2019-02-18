#include <GL/sgl.hpp>

class Sierpinski: public sgl::Window {
protected:
    sgl::Point midpoint(const sgl::Point& pt1, const sgl::Point& pt2) const {
        return {(pt1.x + pt2.x)/2.0, (pt1.y + pt2.y)/2.0};
    }

    void draw_triangle(const sgl::Point& pt1, const sgl::Point& pt2, 
                       const sgl::Point& pt3, int depth) const {
        if (depth > 0) {
            sgl::draw_line(pt1.x, pt1.y, pt2.x, pt2.y);
            sgl::draw_line(pt2.x, pt2.y, pt3.x, pt3.y);
            sgl::draw_line(pt3.x, pt3.y, pt1.x, pt1.y);
            sgl::Point mid12 = midpoint(pt1, pt2),
                       mid13 = midpoint(pt1, pt3),
                       mid23 = midpoint(pt2, pt3);
            draw_triangle(pt1, mid12, mid13, depth - 1);
            draw_triangle(pt2, mid12, mid23, depth - 1);
            draw_triangle(pt3, mid13, mid23, depth - 1);
        }
    }

    int level;

public:
    Sierpinski(): sgl::Window("Sierpinski Triangle", 100, 100, 600, 600,
                              0.0, 599.0, 0.0, 599.0), level(1) {}

    void paint() override {
        sgl::set_color(sgl::DARK_RED);
        draw_triangle(sgl::Point(20.0, 20.0), sgl::Point(579.0, 20.), 
                      sgl::Point(299.0, 579.0), level);
        sgl::draw_text(std::to_string(level), 5.0, 5.0, 12);
    }

    void key_pressed(int key, double x, double y) override {
        switch (key) {
            case sgl::UP_KEY:
                level = (level < 10) ? level + 1 : 10;
                break;
            case sgl::DOWN_KEY:
                level = (level > 0) ? level - 1: 0;
                break;
        }
        sgl::Window::key_pressed(key, x, y);
        repaint();
    }
};

int main() {
    sgl::run<Sierpinski>();
}
