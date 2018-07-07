#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDataStream>
#include <QVector>

struct file_element {
    int id;
    double x,y,z,qs,qx,qy,qz;
    double size;
    int type;

    QVector<int> color;
    QVector<int> id_plane;

};


QByteArray serialize(file_element *info)
{
    QByteArray byteArray;

    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_5);

    stream << info->id << info->x << info->y << info->z << info->qs << info->qx << info->qy << info->qz;
    stream << info->size << info->type << info->color << info->id_plane;

    return byteArray;
}

void deserialize(const QByteArray& byteArray, file_element *info)
{
    QDataStream stream(byteArray);
    stream.setVersion(QDataStream::Qt_4_5);

    stream >> info->id >> info->x >> info->y >> info->z >> info->qs >> info->qx >> info->qy >> info->qz;
    stream >> info->size >> info->type >> info->color >> info->id_plane;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu *menu = new QMenu(ui->addButton);
    menu->addAction(ui->actionCube);
    menu->addAction(ui->actionPrism);
    ui->addButton->setMenu(menu);

    for (int i = 0; i < 8; i++)
    {
        QPushButton *b = new QPushButton(ui->colors);
        b->setMaximumSize(24,24);
        b->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
        colors.append(b);
        ui->colors->layout()->addWidget(b);
        b->hide();
        connect(b, SIGNAL(clicked()), this, SLOT(selectColor()));
    }

    connect(ui->scene, SIGNAL(figureSelected()), this, SLOT(figureSelected()));

    loadScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::figureSelected()
{
    Figure *f = ui->scene->getFirstSelected();
    for (int i = 0; i < colors.size(); i++)
        colors[i]->hide();

    if (ui->scene->countSelected() > 1) return;
    if (f == nullptr)
        return;

    for (uint i = 0; i < f->countPlanes(); i++)
    {
        colors[i]->setStyleSheet(QString("background:rgb(%1,%2,%3)").arg(f->getPlaneColor(i).red())
                                 .arg(f->getPlaneColor(i).green()).arg(f->getPlaneColor(i).blue()));
        colors[i]->show();
    }
}


void MainWindow::selectColor()
{
    for (int i = 0; i < colors.size(); i++)
        if (colors[i] == sender())
        {
           Figure *f = ui->scene->getFirstSelected();
           QColor color = QColorDialog::getColor(f->getPlaneColor(i),this);
           f->setPlaneColor(i, color);
           figureSelected();
           return;
        }

}
void MainWindow::on_actionCube_triggered()
{
    Cube *cube = new Cube();
    ui->scene->addFigure(cube);
    ui->scene->repaint();
}

void MainWindow::on_actionPrism_triggered()
{
    Prism *p = new Prism(2);
    ui->scene->addFigure(p);
    ui->scene->repaint();
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
    ui->scene->repaint();
}

void MainWindow::on_radioButton_clicked()
{
    ui->scene->setViewType(Scene::vt3D);
    ui->scene->repaint();
}

void MainWindow::on_radioButton_2_clicked()
{
    ui->scene->setViewType(Scene::vtXY);
    ui->scene->repaint();
}

void MainWindow::on_radioButton_3_clicked()
{
    ui->scene->setViewType(Scene::vtZY);
    ui->scene->repaint();
}

void MainWindow::on_radioButton_4_clicked()
{
    ui->scene->setViewType(Scene::vtXZ);
    ui->scene->repaint();
}

void MainWindow::on_checkBox_clicked()
{
    ui->scene->setOnlySelected(ui->checkBox->isChecked());
    ui->scene->repaint();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->scene->setRotateAngle(arg1/180.0*M_PI);
}

void MainWindow::saveScene()
{
    QFile *file_scene = new QFile("data.txt");
    //QByteArray file_byte_array;
    QList<Figure*> figures = ui->scene->getFigures();

    if(file_scene->open(QIODevice::WriteOnly)) {
        QDataStream st(file_scene);

        for (int i = 0; i < figures.size(); i++) {
            file_element *info = new file_element;

            int id;

            info->x = figures[i]->getPosition().x();
            info->y = figures[i]->getPosition().y();
            info->z = figures[i]->getPosition().z();
            info->qs = figures[i]->getRotation().scalar();
            info->qx = figures[i]->getRotation().x();
            info->qy = figures[i]->getRotation().y();
            info->qz = figures[i]->getRotation().z();

            if (figures[i]->getType() == 1)
                info->size = figures[i]->getSize();
            else if (figures[i]->getType() == 0)
                info->size = figures[i]->getSize();
            else
                info->size = 0;

            info->type = figures[i]->getType();

            info->id = i;

            for (uint j = 0; j < figures[i]->countPlanes(); j++) {
                QColor c = figures[i]->getPlaneColor(j);
                int color = (((c.blue()<<8) + c.green())<<8)+c.red();

                info->color.push_back(color);
                info->id_plane.push_back(id);
            }

            st << info->id << info->x << info->y << info->z << info->qs << info->qx << info->qy << info->qz;
            st << info->size << info->type << info->color << info->id_plane;

            //file_byte_array.append(serialize(info));

        }
        //st.writeRawData(file_byte_array, figures.size()*sizeof(file_element));
        file_scene->close();
    }
}

void MainWindow::loadScene()
{
    QFile *file_scene = new QFile("../build-lab11-Desktop-Debug/data.txt");
    uint count = 0, i = 0;
    QByteArray file_byte_array;

    if(file_scene->open(QIODevice::ReadOnly | QIODevice::Append)) {
        if (file_scene->pos() == 0)
            return;
        else
            file_scene->close();
    }

    if(file_scene->open(QIODevice::ReadOnly)) {
        QDataStream stream(file_scene);
        //file_byte_array = file_scene->readAll();
        //QDataStream stream(file_byte_array);
        stream.setVersion(QDataStream::Qt_5_5);

        while (!stream.atEnd()) {
            file_element *info = new file_element;

            stream >> info->id >> info->x >> info->y >> info->z >> info->qs >> info->qx >> info->qy >> info->qz;
            stream >> info->size >> info->type >> info->color >> info->id_plane;

            Figure *f;

            if (info->type == 0)
                f = new Cube();
            else
                f = new Prism();

            f->scale(info->size - 1);

            int id = info->id;
            f->setPosition(QVector3D(info->x,
                                 info->y,
                                 info->z));
            f->rotate(QQuaternion(info->qs,
                              info->qx,
                              info->qy,
                              info->qz));

            int j = 0;
            while (j < info->color.size()) {
                int c = info->color[j];
                GLubyte r = c & 0xFF;
                GLubyte g = (c >> 8) & 0xFF;
                GLubyte b = (c >> 16) & 0xFF;
                f->setPlaneColor(j, QColor(r,g,b));
                j++;
            }
            ui->scene->addFigure(f);

            i++;
        }
        file_scene->close();
    }
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    e->accept();
}

void MainWindow::on_pushButton_clicked()
{
    saveScene();
}
