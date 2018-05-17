#ifndef CUBE_H
#define CUBE_H

#include <QtOpenGL>
#include "figure.h"

using namespace std;

class Cube: public Figure
{
protected:
    GLubyte vertexColors[72];
    GLfloat size;

    virtual void recalcTransform();
public:
    Cube(GLfloat size = 1);
    QColor getPlaneColor(uint num) const {return planeColors[num];}
    virtual void setPlaneColor(uint num, const QColor &color);
    virtual uint countPlanes() const {return 6;}
    virtual void scale(float s){size += s; recalcTransform();}
    virtual void draw();
    virtual void drawGeometryOnly();
    virtual void rainbow();
    virtual double getSize(){return size;}
};

#endif // CUBE_H
