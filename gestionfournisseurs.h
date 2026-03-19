#ifndef GESTIONFOURNISSEURS_H
#define GESTIONFOURNISSEURS_H

#include <QMainWindow>
#include"fournisseur.h"



#include <QSqlTableModel>
#include<QComboBox>

#include<QStandardItemModel>
namespace Ui {
class GestionFournisseurs;
}

class GestionFournisseurs : public QMainWindow
{
    Q_OBJECT

public:
    explicit GestionFournisseurs(QWidget *parent = nullptr);
    ~GestionFournisseurs();
signals:
     void transmettreDonneesEvenement(int id, const QString &titre, const QString &description, const QString &date);

private slots:
     //void update_label();   // slot permettant la mise à jour du label état de la lampe 1,
     // ce slot est lancé à chaque réception d'un message de Arduino
     //fournisseur
    void on_pushButton1_clicked();//ajouter

    void on_pushButton2_clicked();//quitter

    void on_pushButton_update_tab_clicked();//button actualiser

    void on_pushButton_rech_par_nom_clicked();//rechercher par nom

    void on_pushButton_supp_id_clicked();//supp fournisseur

    void on_pushButton_clicked();//modifier fournisseur

    void on_pushButton_4_clicked();//tri par nom de produit

    void on_pushButton_7_clicked();//stat selon les adresses

    //commande
    void on_comboBox_historique_currentIndexChanged(int index);//button historique commande

    //fichier pdf
    void on_pushButton_fichierpdf_clicked();//liste des fournisseurs
    void on_pushButton_5_clicked();//liste commandes

    //event
    void on_pushButton_ajout_event_clicked();

    void on_pushButton_consulter_event_clicked();

    void on_pushButton_modif_Event_clicked();

    void on_pushButton_supprimer_event_clicked();

    void on_calendarWidget_event_selectionChanged();


    //amelioration
    void verifierId(int id);
        void enregistrerModifications(int id, const QString &date, const QString &titre, const QString &description);
        void supprimerEvent(int id);
        void rafraichirCalendrier();


        void on_pushButton_eventpdf_clicked();

private:
    Ui::GestionFournisseurs *ui;

    Fournisseur f;

    int fournisseur_id;

    int currentFournisseurID;



    QStringList chargerFournisseursDepuisBaseDeDonnees();
    void chargerHistoriqueDesCommandes(int fournisseurID);

    void enregistrerEvenement(const QString &date, const QString &titre,const QString &description,const int &id);

    bool fournisseurExiste(const int &fournisseurId);

    //void verifierId(int id);
    /*void enregistrerModifications(int id, const QString &date, const QString &titre, const QString &description);
    void supprimerEvent(int id);*/
    void chargerEvenements(const QDate &date);
    void chargerDatesEvenements();

    void afficherGrapheStatistiques(const QStringList &adresses, const QVector<double> &pourcentages);

    QStandardItemModel *eventModel;

    bool evenementExiste(const QString&);
    void initialiserCalendrier();


};

#endif // GESTIONFOURNISSEURS_H
