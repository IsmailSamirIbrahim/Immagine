#include "gui/mainwindow.h"
#include "gui/ui_mainwindow.h"
#include "Immagine/Image.h"

#include <QImageReader>
#include <QFileDialog>
#include <QString>
#include <iostream>
#include <string>

using namespace immagine;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString tmp = QFileDialog::getOpenFileName(this, "Open File", "C://");
    string tmp1 = tmp.toStdString();
    const char* file_path = tmp1.c_str();

    Image image = image_load(file_path);

    this->mImage = QImage(image.data, image.width, image.height, QImage::Format_RGB30);
    this->ui->label->setPixmap(QPixmap::fromImage(this->mImage));
}
