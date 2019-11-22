#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gcodeprocess.h"

//Public Entities
GCodeProcess GCode;

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

void MainWindow::on_pbTest_released()
{
    QString Name = "ttyACM0";
    GCode.InitializeCommunication(Name,115200);
}

void MainWindow::on_pbTest2_released()
{
    GCode.Send("G1 X10");
}
