#ifndef _GEOPOINT_H_
#define _GEOPOINT_H_

#include <GL/sgl.h>


class GeoPoint: public sgl::GraphicalObject {
protected:
    sgl::Color color;
public:
    GeoPoint(double x, double y, const sgl::Color &color);
    virtual ~GeoPoint();
    double get_x() const;
    double get_y() const;
    void paint() const override;
};

#endif

