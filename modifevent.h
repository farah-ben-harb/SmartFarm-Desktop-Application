#ifndef MODIFEVENT_H
#define MODIFEVENT_H

#include <QDialog>

namespace Ui {
class modifevent;
}

class ModifEvent : public QDialog
{
    Q_OBJECT

public:
    explicit ModifEvent(QWidget *parent = nullptr);
    ~ModifEvent();
signals:
    void verifierId(int id);
    void enregistrerModifications(int id, const QString &titre, const QString &description, const QString &date);

public slots:
    void afficherDonnees(int id, const QString &titre, const QString &description, const QString &date);
    void afficherMessageErreur(const QString &message);

private slots:
    void on_pushButton_enregistrert_modif_event_clicked();

    void on_pushButton_verif_id_event_clicked();

private:
    Ui::modifevent *ui;
};
#endif // MODIFEVENT_H
