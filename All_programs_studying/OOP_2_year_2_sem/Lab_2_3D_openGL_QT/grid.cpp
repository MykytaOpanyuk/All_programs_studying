#include "grid.h"

Grid::Grid(uint count_cells, GLfloat cell_size):SceneObject()
{
    setSize(count_cells, cell_size);

    GLfloat translate = count_cells/2.f*cell_size;

    for (uint i = 0; i <= count_cells; i++) {
        addPoint(-translate, 0, i*cell_size - translate);
        addPoint(translate, 0, i*cell_size - translate);
        addPoint(i*cell_size - translate, 0, -translate);
        addPoint(i*cell_size - translate, 0, translate);

        indices.push_back(i*4);
        indices.push_back(i*4 + 1);
        indices.push_back(i*4 + 2);
        indices.push_back(i*4 + 3);

        if ((count_cells/2) == i) {
            for (int j = 0; j < 2; j++)
                addColor(255,0,0);
            for (int j = 0; j < 2; j++)
                addColor(0,255,0);
        }
        else
            for (int j = 0; j < 12; j++)
                colors.push_back(120);
    }
}

void Grid::addColor(GLbyte r, GLbyte g, GLbyte b)
{
    colors.push_back(r);
    colors.push_back(g);
    colors.push_back(b);
}

void Grid::setSize(uint count_cells, GLfloat cell_size)
{
    this->count_cells = count_cells;
    this->cell_size = cell_size;
}

void Grid::draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT , 0, points.data());
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors.data());
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_BYTE, indices.data());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
