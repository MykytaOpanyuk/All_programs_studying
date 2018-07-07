#ifndef GRID_H
#define GRID_H

#include "sceneobject.h"

class Grid : public SceneObject
{
protected:
   GLfloat count_cells;
   GLfloat cell_size;
   QVector<GLbyte> colors;
   virtual void addColor(GLbyte r, GLbyte g, GLbyte b);
public:
    Grid(uint size = 10, GLfloat cell_size = 1);
    void setSize(uint size, GLfloat cell_size);
    uint getCellsCount() const
    {
        return count_cells;
    }
    GLfloat getCellSize() const
    {
        return cell_size;
    }
    virtual void draw();
};

#endif // GRID_H
