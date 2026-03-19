#ifndef EVENEMENT_H
#define EVENEMENT_H

#include <QDialog>
#include"fournisseur.h"
namespace Ui {
class evenement;
}

class evenement : public QDialog
{
    Q_OBJECT

public:
    explicit evenement(QWidget *parent = nullptr);
    ~evenement();

signals:
    void evenementEnregistre(const QString &date, const QString &titre,const QString &description,const int &id);


private slots:
    void on_pushButton_enregistrer_event_clicked();

private:
    Ui::evenement *ui;
    int fournisseurID;
};

#endif // EVENEMENT_H
