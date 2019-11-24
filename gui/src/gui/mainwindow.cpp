#include "gui/mainwindow.h"
#include "gui/ui_mainwindow.h"
#include "Immagine/Image.h"
#include "Immagine/Utilities.h"

#include <QFileDialog>
#include <QString>

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
    QString tmp = QFileDialog::getOpenFileName(this, "Open File", "E://Work//immagine/resources/images");
    string tmp1 = tmp.toStdString();
    const char* file_path = tmp1.c_str();

    if(tmp.size() == 0)
        return;

    this->mImage = image_load(file_path);
    Image self = image_data_join(this->mImage);

    QImage qmImage = QImage(self.data, self.width, self.height, QImage::Format::Format_RGB888);
    this->ui->label->setPixmap(QPixmap::fromImage(qmImage).scaled(this->ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    image_free(self);
}

void MainWindow::on_actionNew_triggered()
{
    this->mImage = image_new(500, 700, 3);

    QImage qmImage = QImage(this->mImage.data, this->mImage.width, this->mImage.height, QImage::Format::Format_RGB888);
    this->ui->label->setPixmap(QPixmap::fromImage(qmImage));
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionSave_triggered()
{
    QString tmp = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                    QString(),
                                                    tr("Images (*.bmp)"));
    string tmp1 = tmp.toStdString();
    const char* file_path = tmp1.c_str();

    if (!tmp.isEmpty())
    {
        image_save(file_path, this->mImage, IMAGE_FORMAT::BMP);

    }
}

void MainWindow::on_actionFlip_Horizental_triggered()
{
    Image tmp = image_flip_horizontally(this->mImage);

    Image self = image_data_join(tmp);

	QImage qmImage = QImage(self.data, self.width, self.height, QImage::Format::Format_RGB888);
	this->ui->label->setPixmap(QPixmap::fromImage(qmImage).scaled(this->ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));


    image_free(this->mImage);
    image_free(self);
    this->mImage = tmp;
}

void MainWindow::on_actionFlip_Vertical_triggered()
{
    Image tmp = image_flip_vertically(this->mImage);

    Image self = image_data_join(tmp);

	QImage qmImage = QImage(self.data, self.width, self.height, QImage::Format::Format_RGB888);
	this->ui->label->setPixmap(QPixmap::fromImage(qmImage).scaled(this->ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    image_free(this->mImage);
    image_free(self);
    this->mImage = tmp;
}

void MainWindow::on_actionRotate_Left_triggered()
{
    Image tmp = image_rotate_left(this->mImage);

    Image self = image_data_join(tmp);

	QImage qmImage = QImage(self.data, self.width, self.height, QImage::Format::Format_RGB888);
	this->ui->label->setPixmap(QPixmap::fromImage(qmImage).scaled(this->ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    image_free(this->mImage);
    image_free(self);
    this->mImage = tmp;
}

void MainWindow::on_actionRotate_Rigth_triggered()
{
    Image tmp = image_rotate_right(this->mImage);

    Image self = image_data_join(tmp);

	QImage qmImage = QImage(self.data, self.width, self.height, QImage::Format::Format_RGB888);
	this->ui->label->setPixmap(QPixmap::fromImage(qmImage).scaled(this->ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    image_free(this->mImage);
    image_free(self);
    this->mImage = tmp;
}
