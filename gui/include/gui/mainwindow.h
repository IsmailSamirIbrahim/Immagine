#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <Immagine/Image.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionExit_triggered();

    void on_actionSave_triggered();

    void on_actionFlip_Horizental_triggered();

    void on_actionFlip_Vertical_triggered();

    void on_actionRotate_Left_triggered();

    void on_actionRotate_Rigth_triggered();

private:
    Ui::MainWindow *ui;
    immagine::Image mImage;
};

#endif // MAINWINDOW_H
