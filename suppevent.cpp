#include "suppevent.h"
#include "ui_suppevent.h"

SuppEvent::SuppEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SuppEvent)
{
    ui->setupUi(this);
    ui->le_id_event_supp->setValidator(new QIntValidator(0,99999999,this));
}

SuppEvent::~SuppEvent()
{
    delete ui;
}

void SuppEvent::on_pushButton_event_supp_clicked()
{
    int id = ui->le_id_event_supp->text().toInt();
        emit suppressionDemandee(id);
        accept();
}
