#ifndef SCENE_H
#define SCENE_H

#include <QtOpenGL>
#include "cube.h"
#include "grid.h"
#include "figure.h"
#include "cube.h"
#include "pyramid.h"
#include "camera.h"

class Scene : public QGLWidget
{
    Q_OBJECT
public:
    enum MouseMoveAction{MOVE, ROTATE};
    enum ViewType{vt3D, vtXY, vtZY, vtXZ};
    explicit Scene(QWidget *parent = 0);
    ~Scene();
    uint countSelected(){return selectedFigures.size();}
    Figure *getFirstSelected();
    void addFigure(Figure *f){figures.append(f);}
    void setViewType(ViewType t);
    void setOnlySelected(bool t){showOnlySelected = t;}
    void setRotateAngle(double a){rotateFigureAlpha = a;}
    QList<Figure*> getFigures(){ return figures;}

signals:
    void figureSelected();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    uint figureOnPoint(const QPoint &point);
protected slots:
    void paintColorBuf();
private:
    Grid* grid;
    QList<Figure*> figures;
    QList<Figure*> selectedFigures;

    Camera *cameras[4];
    uchar currCamera;
    QPoint mouseLastPos;

    bool renderAxis[3];
    QVector3D axisCenter;
    MouseMoveAction action;

    float moveFigureSpeed;
    float rotateFigureAlpha;
    ViewType currentView;

    bool showOnlySelected;
    void unselectAll();
    void selectFigure(uint num);
    void unselectFigure(uint num);
    void moveSelected(float dist, const QVector3D &v);
    void rotateSelected(float angle, const QVector3D &v);
    void scaleSelected(float s);
};

#endif // SCENE_H
