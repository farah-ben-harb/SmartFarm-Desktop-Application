#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H


#include<QString>
#include<QSqlQueryModel>
#include <QFile>
#include <QFileDialog>



class Fournisseur
{

private:
    QString nom,ad_mail,ad,nom_produit,prenom;
    int id,num;
public:


    void set_nom(QString );
    void set_prenom(QString);
    void set_id(int );
    void set_num(int );
    void set_ad_mail(QString );
    void set_ad(QString );
    void set_produit(QString );



    QString get_nom();
    QString get_prenom();
    int get_id();
    int get_num();
    QString get_ad_mail();
    QString get_ad();
    QString get_produit();



    Fournisseur();
    Fournisseur(int,QString,QString,QString,QString,int,QString);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool rechercher(int);
    //modifier
    bool rechercher_modifier(int);//pour modifier
    QSqlQueryModel* rechercher_nom(QString);
    //tri
    QSqlQueryModel* trierParNomProduit();
    //historique
    QStringList chargerFournisseursDepuisBaseDeDonnees();
    int obtenirIDFournisseurParNom(const QString &nomFournisseur);
    //statistiques
    double pourcentageFournisseursParAdresse(const QString &adresse);
    QStringList obtenirAdressesFournisseurs();




};

#endif // FOURNISSEUR_H
