#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QColor col = QColor::fromCmyk(0.39, 0.39, 0.0, 0.0);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
