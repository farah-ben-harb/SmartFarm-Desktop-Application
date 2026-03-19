#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"gestionfournisseurs.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_gestion_fournisseurs_clicked()
{
    GestionFournisseurs *f;
    f=new GestionFournisseurs();
    f->show();
}
