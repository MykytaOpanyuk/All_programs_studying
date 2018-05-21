#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu *menu = new QMenu(ui->addButton);
    menu->addAction(ui->actionCube);
    menu->addAction(ui->actionTetrahedron);
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
    DB_USER = "root";

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

void MainWindow::on_actionTetrahedron_triggered()
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
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("maxdb");
    db.open();

    QSqlQuery q;

    q.exec("DELETE FROM figures WHERE 1=1");
    q.exec("DELETE FROM colors WHERE 1=1");

    QList<Figure*> figures = ui->scene->getFigures();

    for (int i = 0; i < figures.size(); i++) {
        int id;
        q.prepare("INSERT INTO figures (posx, posy, posz, quaterionScalar, quaterionX, quaterionY, quaterionZ, size, type) "
                  "VALUES (:x, :y, :z, :qs, :qx, :qy, :qz, :size, :type)");
        q.bindValue(":x", figures[i]->getPosition().x());
        q.bindValue(":y", figures[i]->getPosition().y());
        q.bindValue(":z", figures[i]->getPosition().z());
        q.bindValue(":qs", figures[i]->getRotation().scalar());
        q.bindValue(":qx", figures[i]->getRotation().x());
        q.bindValue(":qy", figures[i]->getRotation().y());
        q.bindValue(":qz", figures[i]->getRotation().z());

        if (figures[i]->getType() == 0)
            q.bindValue(":size", dynamic_cast<Cube*>(figures[i])->getSize());
        else if (figures[i]->getType() == 1)
            q.bindValue(":size", dynamic_cast<Prism*>(figures[i])->getSize());
        else q.bindValue(":size", 1);

        q.bindValue(":type", figures[i]->getType());
        q.exec();

        id = q.lastInsertId().toInt();

        for (uint j = 0; j < figures[i]->countPlanes(); j++)
        {
            QColor c = figures[i]->getPlaneColor(j);
            int color = (((c.blue()<<8) + c.green())<<8)+c.red();

            q.prepare("INSERT INTO colors(color, figure) VALUES(:color, :figure)");
            q.bindValue(":color", color);
            q.bindValue(":figure", id);
            q.exec();
        }
    }

    db.close();
}

void MainWindow::loadScene()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("maxdb");
    db.open();

    QSqlQuery q;
    QSqlQuery q1;

    q.exec("SELECT * FROM figures");
    while (q.next())
    {
        Figure *f;
        if (q.value(9).toInt() == 0)
            f = new Cube(q.value(8).toDouble());
        else f = new Prism(q.value(8).toDouble());

        int id = q.value(0).toInt();
        f->setPosition(QVector3D(q.value(1).toDouble(),
                                 q.value(2).toDouble(),
                                 q.value(3).toDouble()));
        f->rotate(QQuaternion(q.value(4).toDouble(),
                              q.value(5).toDouble(),
                              q.value(6).toDouble(),
                              q.value(7).toDouble()));


        q1.exec(QString("SELECT * FROM colors WHERE figure = %1").arg(id));
        int j = 0;
        while (q1.next())
        {
            int c = q1.value(1).toInt();
            GLubyte r = c & 0xFF;
            GLubyte g = (c >> 8) & 0xFF;
            GLubyte b = (c >> 16) & 0xFF;
            f->setPlaneColor(j, QColor(r,g,b));
            j++;
        }

        ui->scene->addFigure(f);
    }

    db.close();
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    saveScene();
    e->accept();
}
