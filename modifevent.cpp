#include "modifevent.h"
#include "ui_modifevent.h"

#include<QMessageBox>
#include<QDebug>

ModifEvent::ModifEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifevent)
{
    ui->setupUi(this);

    ui->le_id_event_modif->setValidator(new QIntValidator(0,99999999,this));

}

ModifEvent::~ModifEvent()
{
    delete ui;
}

void ModifEvent::on_pushButton_enregistrert_modif_event_clicked()
{
    int id = ui->le_id_event_modif->text().toInt();
     if (id != 0) {
       QString titre = ui->le_titre_modif->text();
       QString description = ui->le_description_modif->text();
       QString date = ui->dateEdit_modif->text();

       qDebug() << "ID: " << id;
           qDebug() << "Titre: " << titre;
           qDebug() << "Description: " << description;
           qDebug() << "Date: " << date;
       emit enregistrerModifications(id, date,titre, description);
            accept();
} else {

    QMessageBox::critical(this, "Erreur", "impossible de faire l enregistrement .");
}
}

void ModifEvent::on_pushButton_verif_id_event_clicked()
{
    int id = ui->le_id_event_modif->text().toInt();
        emit verifierId(id);


}

void ModifEvent::afficherDonnees(int id, const QString &titre, const QString &description, const QString &date)
{
    ui->le_id_event_modif->setText(QString::number(id));
    ui->le_titre_modif->setText(titre);
    ui->le_description_modif->setText(description);
    ui->dateEdit_modif->setDate(QDate::fromString(date, "yy-MM-dddd")); // Assurez-vous que le format correspond à votre base de données
}

void ModifEvent::afficherMessageErreur(const QString &message)
{
    // Vous pouvez afficher le message d'erreur dans une boîte de dialogue ou un QLabel ici.
    // Par exemple, utilisez QMessageBox pour afficher une boîte de dialogue d'erreur.
    QMessageBox::critical(this, "Erreur", message);
}
