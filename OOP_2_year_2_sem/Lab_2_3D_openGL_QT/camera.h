#ifndef CAMERA_H
#define CAMERA_H

#include <QtOpenGL>

class Camera {
protected:
    QVector3D position;
    QVector3D target;
    QVector3D up;
    QMatrix4x4 projection;


public:
    Camera();

    QMatrix4x4 getViewProjectMatrix();

    void setPosition(const QVector3D &pos);
    void setTarget(const QVector3D &t);
    void setUpVector(const QVector3D &t);

    void setProjectionMatrix(const QMatrix4x4 &m);

    QVector3D getPos();
    QVector3D getTarget();
    QVector3D upVector();


    void moveFront(GLfloat dist);

    void rotateView(double angle, const QVector3D &vector);

    void rotatePosition(double angle, const QVector3D &vector);
    void rotatePositionY(double angle);
    void rotatePositionUp(double angle);
    void rotateViewY(double angle);

    void rotateAroundPoint(const QVector3D &vCenter, GLfloat angle, const QVector3D &v);
};

#endif // CAMERA_H
