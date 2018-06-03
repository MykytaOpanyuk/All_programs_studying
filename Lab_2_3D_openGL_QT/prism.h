#ifndef PRISM_H
#define PRISM_H

#include "figure.h"

using namespace std;

class Prism: public Figure
{
protected:
    GLubyte vertexColors[108];
    GLfloat size;

    virtual void recalcTransform();
public:
    Prism(GLfloat size = 1);
    QColor getPlaneColor(uint num) const
    {
        return planeColors[num];
    }
    virtual void setPlaneColor(uint num, const QColor &color);
    virtual uint countPlanes() const
    {
        return 8;
    }
    virtual void scale(float s)
    {
        size += s; recalcTransform();
    }
    virtual void draw();
    virtual void drawGeometryOnly();
    virtual void rainbow();
    virtual double getSize()
    {
        return size;
    }
};

#endif // PRISM_H
