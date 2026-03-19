#include "affichage.h"
#include "ui_affichage.h"


#include"fournisseur.h"
#include<QString>
#include<QMessageBox>
#include <QSqlQuery>
#include<QStandardItemModel>
#include<QSqlQueryModel>
#include<QDebug>

affichage::affichage(int id, QString nom,QString prenom, QString ad_mail, QString ad, int num,QString nom_produit,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::affichage),
    id(id)

{
    ui->setupUi(this);

        ui->le_nom_modif->setText(nom);
        ui->le_ad_mail_modif->setText(ad_mail);
        ui->comboBox_modif->setCurrentText(ad);
        ui->le_num_modif->setText(QString::number(num));
        ui->comboBox_produit_modif->setCurrentText(nom_produit);
        ui->le_prenom_modif->setText(prenom);

}
affichage::~affichage()
{
    delete ui;
}


void affichage::on_pushButton_enregistrer_clicked()
{

    Fournisseur f;
    qDebug() << "Bouton Enregistrer cliqué";
   // Récupérez les données modifiées de la fenêtre et enregistrez-les dans la base de données
       QString nouveauNom = ui->le_nom_modif->text();
       QString nouvelleAdMail = ui->le_ad_mail_modif->text();
       QString nouvelleAd = ui->comboBox_modif->currentText();
       int nouveauNumero = ui->le_num_modif->text().toInt();
       QString nouveau_produit=ui->comboBox_produit_modif->currentText();
       QString nouveauPrenom=ui->le_prenom_modif->text();

       // Mettez à jour les données du fournisseur dans la base de données en utilisant idFournisseur
       QSqlQuery query;
       query.prepare("UPDATE GS_FOURNISSEUR SET NOM = :nom, PRENOM = :prenom, ADRESSE_MAIL = :ad_mail, ADRESSE = :ad, NUMERO = :num , NOM_PRODUIT = :nom_produit WHERE IDENTIFIANT = :id");
       query.bindValue(":nom", nouveauNom);
       query.bindValue(":prenom", nouveauPrenom);
       query.bindValue(":ad_mail", nouvelleAdMail);
       query.bindValue(":ad", nouvelleAd);
       query.bindValue(":num", nouveauNumero);
       query.bindValue(":nom_produit", nouveau_produit);
       query.bindValue(":id", id);

       if (query.exec())
       {
           QMessageBox::information(this, "Enregistrement", "Les modifications ont été enregistrées avec succès.");//problem:pas de update dans consulter en meme temps

           accept();
       }
       else
       {
           QMessageBox::critical(this, "Erreur", "Erreur lors de l'enregistrement des modifications.");
       }
}
