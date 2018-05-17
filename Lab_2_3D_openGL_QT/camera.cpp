#include "camera.h"

Camera::Camera():position(0.1, 10, 0.1),target(0,0,0),up(0,1,0)
{
}

void Camera::setPosition(const QVector3D &pos)
{
    position = pos;
}

void Camera::setUpVector(const QVector3D &t)
{
    up = t;
}

void Camera::setProjectionMatrix(const QMatrix4x4 &m)
{
    projection = m;
}

QMatrix4x4 Camera::getViewProjectMatrix()
{
    QMatrix4x4 res;
    res.frustum(-1, 1, -1, 1, 1, 1000);
    res.lookAt(position, target, up);
    res *= projection;
    return res;
}

void Camera::moveFront(GLfloat dist)
{
    QVector3D v = target - position;
    v.normalize();
    position += v * dist;
    target += v * dist;
}

void Camera::rotateAroundPoint(const QVector3D &vCenter, GLfloat angle, const QVector3D &v)
{
    QVector3D vPos = position - vCenter;

    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);

    QVector3D newPos;
    newPos.setX(  (cosTheta + (1 - cosTheta) * v.x() * v.x())* vPos.x() +
                    ((1 - cosTheta) * v.x() * v.y() - v.z() * sinTheta)* vPos.y() +
                    ((1 - cosTheta) * v.x() * v.z() + v.y() * sinTheta)* vPos.z());

    newPos.setY( ((1 - cosTheta) * v.x() * v.y() + v.z() * sinTheta)* vPos.x() +
                 (cosTheta + (1 - cosTheta) * v.y() * v.y())* vPos.y() +
                 ((1 - cosTheta) * v.y() * v.z() - v.x() * sinTheta)* vPos.z());

    newPos.setZ(((1 - cosTheta) * v.x() * v.z() - v.y() * sinTheta)* vPos.x() +
                ((1 - cosTheta) * v.y() * v.z() + v.x() * sinTheta)* vPos.y() +
                (cosTheta + (1 - cosTheta) * v.z() * v.z())* vPos.z());

    position = vCenter + newPos;
}

void Camera::rotateView(double angle, const QVector3D &v)
{
    QVector3D view = target - position;

    double cosa =cos(angle);
    double sina =sin(angle);

    QVector3D newView((cosa+(1-cosa)*v.x()*v.x())*view.x(),
                    ((1 - cosa)*v.x()*v.y()+v.z()*sina)*view.x(),
                    ((1 - cosa)*v.x()*v.z()-v.y()*sina)*view.x());

    newView.setX(newView.x() + ((1-cosa)*v.x()*v.y()-v.z()*sina)*view.y() +
                 ((1-cosa)*v.x()*v.z()+v.y()*sina)*view.z());


    newView.setY(newView.y() + (cosa + (1 - cosa)*v.y()*v.y())*view.y() +
                 ((1 - cosa)*v.y()*v.z()-v.z()*sina)*view.z());

    newView.setZ(newView.z() + ((1 - cosa)*v.y()*v.z()+v.x()*sina)*view.y() +
                 (cosa + (1 - cosa)*v.z()*v.z())*view.z());

    target = position + newView;
}

void Camera::rotatePosition(double angle, const QVector3D &v)
{
    rotateAroundPoint(target, angle, v);
}


void Camera::rotateViewY(double angle)
{
    rotateView(angle, up);
}

void Camera::rotatePositionY(double angle)
{
    rotatePosition(angle, up);
}

void Camera::rotatePositionUp(double angle)
{
    QVector3D v = QVector3D::crossProduct(target - position, QVector3D(1,0,1));
    v.normalize();
    rotatePosition(angle, QVector3D(1,0,0));
}

void Camera::setTarget(const QVector3D &t)
{
    target = t;
}

QVector3D Camera::getPos()
{
    return position;
}

QVector3D Camera::getTarget()
{
    return target;
}
QVector3D Camera::upVector()
{
    return up;
}
