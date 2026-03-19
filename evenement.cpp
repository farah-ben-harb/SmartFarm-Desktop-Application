#include "evenement.h"
#include "ui_evenement.h"


#include"fournisseur.h"

#include<QDate>
#include<QMessageBox>


evenement::evenement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::evenement)

{
    ui->setupUi(this);
    ui->le_id_event->setValidator(new QIntValidator(0,99999999,this));
}

evenement::~evenement()
{
    delete ui;
}

void evenement::on_pushButton_enregistrer_event_clicked()
{
    // Récupérez l'ID saisi et convertissez-le en int
        int id = ui->le_id_event->text().toInt();

        // Vérifiez si l'ID est valide (différent de 0, par exemple)
        if (id != 0) {
            // Récupérez la date, le titre et la description
            QDate date = ui->dateEdit->date();
            QString dateStr = date.toString("dd/MM/yyyy");
            QString titre = ui->le_titre_event->text();
            QString description = ui->le_description->text();

            // Émettez le signal avec les données de l'événement
            emit evenementEnregistre(dateStr, titre, description, id);

            // Fermez la fenêtre d'ajout d'événements
            accept();
        } else {
            // Affichez un message d'erreur si l'ID n'est pas valide
            QMessageBox::critical(this, "Erreur", "L'ID du fournisseur n'est pas valide.");
        }
}

