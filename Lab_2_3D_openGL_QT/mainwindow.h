#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<QPushButton*> colors;
    QString DB_USER;
    QString DB_PASS;
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);

    void saveScene();
    void loadScene();
private slots:
    void figureSelected();
    void selectColor();
    void on_actionCube_triggered();
    void on_actionPrism_triggered();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_checkBox_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
