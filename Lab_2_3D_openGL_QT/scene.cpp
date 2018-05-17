#include "scene.h"
#include <vector>
#include <QDebug>

using namespace std;

Scene::Scene(QWidget *parent) :
    QGLWidget(parent)
{
    grid = new Grid(16,1);

    currCamera = 0;
    currentView = vt3D;
    cameras[0] = new Camera();
    cameras[0]->setPosition(QVector3D(0,10,10));
    cameras[0]->setTarget(QVector3D(0, 0, 0));

    QMatrix4x4 p;

    //XY projection
    p.setColumn(2, QVector4D());
    cameras[1] = new Camera();
    cameras[1]->setPosition(QVector3D(0,0,10));
    cameras[1]->setTarget(QVector3D(0, 0, 0));
    cameras[1]->setProjectionMatrix(p);

    //ZY projection
    p.setToIdentity();
    p.setColumn(0, QVector4D());
    cameras[2] = new Camera();
    cameras[2]->setPosition(QVector3D(10,0,0));
    cameras[2]->setTarget(QVector3D(0, 0, 0));
    cameras[2]->setProjectionMatrix(p);

    //XZ projection
    p.setToIdentity();
    p.setColumn(1, QVector4D());
    cameras[3] = new Camera();
    cameras[3]->setPosition(QVector3D(0,10, 0));
    cameras[3]->setTarget(QVector3D(0, 0, 0));
    cameras[3]->setUpVector(QVector3D(0,0,1));
    cameras[3]->setProjectionMatrix(p);

    action = MOVE;
    moveFigureSpeed = 0.12;
    rotateFigureAlpha = 30/180.0*M_PI;

    renderAxis[0] = renderAxis[1] = renderAxis[2] = false;
    showOnlySelected = false;
}

Scene::~Scene()
{
    delete grid;
    for (auto i = figures.begin(); i != figures.end(); i++)
        delete (*i);
}


void Scene::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    //static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
}

void Scene::paintGL()
{
    qglClearColor(QColor(60,60,60));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glMultMatrixf(cameras[currCamera]->getViewProjectMatrix().constData());


    QVector3D center;
    if (currentView == vt3D)
    {
        grid->draw();
        center = axisCenter;
    }

    for (auto i = figures.begin(); i != figures.end(); i++)
        if (!showOnlySelected || (*i)->isSelected())
            (*i)->draw();


    glBegin(GL_LINES);
    if (renderAxis[0] || currentView == vtXY || currentView == vtXZ)
    {
        glColor3f(1, 0, 0);
        glVertex3f(-1000, center.y(), center.z());
        glVertex3f(1000, center.y(), center.z());
    }
    if (renderAxis[1] || currentView == vtXY || currentView == vtZY)
    {
        glColor3f(0, 0, 1);
        glVertex3f(center.x(), -1000, center.z());
        glVertex3f(center.x(), 1000, center.z());
    }
    if (renderAxis[2] || currentView == vtZY || currentView == vtXZ)
    {
        glColor3f(0, 1, 0);
        glVertex3f(center.x(), center.y(), -1000);
        glVertex3f(center.x(), center.y(), 1000);
    }
    glEnd();

}

void Scene::paintColorBuf()
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMultMatrixf(cameras[currCamera]->getViewProjectMatrix().constData());
    for (int i = 0; i < figures.size(); i++)
    {
        uint c = i+1;
        GLubyte r = c & 0xFF;
        GLubyte g = (c >> 8) & 0xFF;
        GLubyte b = (c >> 16) & 0xFF;
        //qDebug() << r << g << b;
        glColor3ub(r,g,b);
        figures[i]->drawGeometryOnly();
    }
}

uint Scene::figureOnPoint(const QPoint &point)
{
    paintColorBuf();
    GLubyte v[3];
    glReadPixels(point.x(), height() - point.y(), 1,1, GL_RGB, GL_UNSIGNED_BYTE, &v);
    //qDebug() << v[0] << v[1] << v[2];
    repaint();
    return (((v[2]<<8) + v[1])<<8)+v[0];
}

void Scene::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

void Scene::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        uint selected = figureOnPoint(e->pos());
        if (selected == 0 || (figures[selected-1]->isSelected() == false &&
                              !(e->modifiers() & Qt::ControlModifier)))
           unselectAll();

        if (selected > 0)
            selectFigure(selected-1);
        update();
        figureSelected();
    }
    mouseLastPos = e->pos();
}

void Scene::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_X) renderAxis[0] = true;
    if (e->key() == Qt::Key_Y) renderAxis[1] = true;
    if (e->key() == Qt::Key_Z) renderAxis[2] = true;
    update();
}

void Scene::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_X) renderAxis[0] = false;
    if (e->key() == Qt::Key_Y) renderAxis[1] = false;
    if (e->key() == Qt::Key_Z) renderAxis[2] = false;
    if (e->key() == Qt::Key_1) action = MOVE;
    if (e->key() == Qt::Key_2) action = ROTATE;

    update();
}

void Scene::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->x() - mouseLastPos.x();
    int dy = e->y() - mouseLastPos.y();

    if (e->buttons() & Qt::MidButton && currentView == vt3D) {
        //camera.rotatePositionUp(dy/50.f);
        cameras[0]->rotatePositionY(dx/50.f);
    }

    if (e->buttons() & Qt::RightButton)
    {
        if (action == MOVE)
        {
            if (renderAxis[0]) moveSelected(dx*moveFigureSpeed, QVector3D(1,0,0));
            if (renderAxis[1]) moveSelected(-dy*moveFigureSpeed, QVector3D(0,1,0));
            if (renderAxis[2]) moveSelected(dx*moveFigureSpeed, QVector3D(0,0,1));
        }
        else if (action == ROTATE)
        {
            if (renderAxis[0]) rotateSelected(dy*rotateFigureAlpha, QVector3D(1,0,0));
            if (renderAxis[1]) rotateSelected(dx*rotateFigureAlpha, QVector3D(0,1,0));
            if (renderAxis[2]) rotateSelected(dy*rotateFigureAlpha, QVector3D(0,0,1));
        }
    }
    mouseLastPos = e->pos();
    update();
}

void Scene::wheelEvent(QWheelEvent *e)
{
    QGLWidget::wheelEvent(e);
    if (!e->isAccepted())
    {
        if (e->modifiers() & Qt::ShiftModifier)
            scaleSelected(e->delta()/480.f);
        else
            cameras[currCamera]->setPosition(cameras[currCamera]->getPos() +
                               (cameras[currCamera]->getTarget() - cameras[currCamera]->getPos()).normalized()
                                             *(e->delta()/120));
        e->accept();
        update();
    }
}

void Scene::selectFigure(uint num)
{
    if (figures[num]->isSelected()) return;
    axisCenter *= selectedFigures.count();
    selectedFigures.append(figures[num]);
    figures[num]->setSelected(true);
    axisCenter += figures[num]->getPosition();
    axisCenter /= selectedFigures.count();
}

void Scene::unselectFigure(uint num)
{
    if (!figures[num]->isSelected()) return;
   axisCenter *= selectedFigures.count();
   selectedFigures.erase(selectedFigures.begin()+selectedFigures.indexOf(figures[num]));
   figures[num]->setSelected(false);
   axisCenter -= figures[num]->getPosition();
   axisCenter /= selectedFigures.count();
}

void Scene::unselectAll()
{
    for (auto i = selectedFigures.begin(); i != selectedFigures.end(); i++)
        (*i)->setSelected(false);
    selectedFigures.clear();
    axisCenter = QVector3D(0,0,0);
}

void Scene::moveSelected(float dist, const QVector3D &v)
{
    if (selectedFigures.size() == 0) return;

    axisCenter = QVector3D(0,0,0);
    for (auto i = selectedFigures.begin(); i != selectedFigures.end(); i++)
    {
        (*i)->translate(v*dist);
        axisCenter += (*i)->getPosition();
    }
    axisCenter /= selectedFigures.size();
}

void Scene::rotateSelected(float angle, const QVector3D &v)
{
    if (selectedFigures.size() == 0) return;

    QQuaternion q = QQuaternion::fromAxisAndAngle(v,angle);
    for (auto i = selectedFigures.begin(); i != selectedFigures.end(); i++)
    {
        (*i)->rotate(q);

    }
}

void Scene::scaleSelected(float s)
{
    for (auto i = selectedFigures.begin(); i != selectedFigures.end(); i++)
    {
        (*i)->scale(s);
    }
}

Figure* Scene::getFirstSelected()
{
    if (selectedFigures.size() == 0)
        return nullptr;
    return selectedFigures[0];
}

void Scene::setViewType(ViewType t)
{
    currentView = t;
    currCamera = (int)t;
}
