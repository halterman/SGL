#include "geopoint.h"

GeoPoint::GeoPoint(double x, double y, const sgl::Color& c):
    GraphicalObject(x, y, 4, 4), color(c) {
    set_cursor(sgl::CursorShape::Crosshair);
}

GeoPoint::~GeoPoint() {}

double GeoPoint::get_x() const {
    return left + width/2;
}

double GeoPoint::get_y() const {
    return bottom + height/2;
}


void GeoPoint::paint() const {
    sgl::set_color(color);
    sgl::draw_rectangle(left, bottom, width, height);
	if (mouse_over)
        sgl::draw_rectangle(left - 5, bottom - 5, width + 10, height + 10); 
}

