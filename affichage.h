#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include"fournisseur.h"

#include <QDialog>

#include<QString>
#include<QSqlQueryModel>
namespace Ui {
class affichage;
}

class affichage : public QDialog
{
    Q_OBJECT

public:
    explicit affichage(int id, QString nom,QString prenom, QString ad_mail, QString ad, int num,QString nom_produit,QWidget *parent = nullptr);
    ~affichage();



private slots:

    void on_pushButton_enregistrer_clicked();

private:
    Ui::affichage *ui;

    int id;
};

#endif // AFFICHAGE_H
