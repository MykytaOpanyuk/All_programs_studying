#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QtOpenGL>
#include <QList>

class SceneObject {
protected:
    QMatrix4x4 transform;

    QVector<GLfloat> points;
    QVector<GLubyte> indices;

    virtual void addPoint(GLfloat x, GLfloat y, GLfloat z){
        points.push_back(x);
        points.push_back(y);
        points.push_back(z);
    }
    virtual void addPoint(const QVector3D &p){
        addPoint(p.x(), p.y(), p.z());
    }

public:
    SceneObject(){}
    virtual ~SceneObject(){}

    void setTransform(const QMatrix4x4 &tr){
        transform = tr;
    }

    QMatrix4x4 getTransform(){
        return transform;
    }

    virtual void translate(const QVector3D &v){
        transform.translate(v);
    }

    virtual void draw() = 0;
};

#endif // SCENEOBJECT_H
