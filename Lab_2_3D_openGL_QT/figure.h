#ifndef FIGURE_H
#define FIGURE_H
#include "sceneobject.h"

class Figure : public SceneObject
{
protected:
    QVector3D pos;

    bool selected;
    QQuaternion rotation;
    QVector<QColor> planeColors;
    int type;

    virtual void recalcTransform(){
        transform.setToIdentity();
        transform.translate(pos);
        transform.rotate(rotation);
    }
public:
    Figure():SceneObject(),pos(QVector3D(0,0,0)),selected(false){}
    virtual ~Figure(){}
    virtual void drawGeometryOnly() = 0;
    virtual void setPosition(const QVector3D &pos)
    {
        this->pos = pos;
        recalcTransform();
    }
    virtual QVector3D getPosition() const
    {
        return pos;
    }
    virtual void translate(const QVector3D &v)
    {
        pos = pos + v;
        recalcTransform();
    }
    virtual void rotate(const QQuaternion &q)
    {
        rotation = rotation * q;
        recalcTransform();
    }
    virtual void scale(float s)
    {
        transform.scale(s);
    }
    virtual bool isSelected() const
    {
        return selected;
    }
    virtual void setSelected(bool s)
    {
        selected = s;
    }
    virtual QColor getPlaneColor(uint num) const
    {
        return planeColors[num];
    }
    virtual void rainbow(){}
    virtual uint countPlanes() const = 0;
    virtual QQuaternion getRotation()
    {
        return rotation;
    }
    virtual int getType()
    {
        return type;
    }

    virtual void setPlaneColor(uint num, const QColor &color){
        if (num >= planeColors.size()) planeColors.resize(num+1);
        planeColors[num] = color;}
};

#endif // FIGURE_H
