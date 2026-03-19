#include "fournisseur.h"

#include "fournisseur.h"
#include"affichage.h"
#include"ui_affichage.h"
#include <QSqlQuery>
#include<QDebug>
#include<QObject>
#include<QSqlError>

Fournisseur::Fournisseur()
{
    id=0;
    prenom="";
    nom="";
    ad="";
    ad_mail="";
    nom_produit="";
    num=0;
}


Fournisseur::Fournisseur(int id,QString nom,QString prenom,QString ad_mail,QString ad,int num,QString nom_produit)
{

    this->id=id;
    this->nom=nom;
    this->prenom=prenom;
    this->ad_mail=ad_mail;
    this->ad=ad;
    this->num=num;
    this->nom_produit=nom_produit;


}

void Fournisseur::set_nom(QString nom)
{
          this->nom=nom;
}
void Fournisseur::set_prenom(QString prenom)
{
          this->prenom=prenom;
}
void Fournisseur::set_id(int id)
{
          this->id=id;
}
void Fournisseur::set_num(int num)
{
          this->num=num;
}
void Fournisseur::set_ad(QString ad)
{
           this->ad=ad;
}
void Fournisseur::set_ad_mail(QString ad_mail)
{
           this->ad_mail=ad_mail;
}
void Fournisseur::set_produit(QString nom_produit)
{
            this->nom_produit=nom_produit;
}


QString Fournisseur::get_nom()
{
             return nom;
}

QString Fournisseur::get_prenom()
{
             return prenom;
}
QString Fournisseur::get_ad_mail()
{

    return ad_mail;
}
QString Fournisseur::get_produit()
{
    return nom_produit;
}
QString Fournisseur::get_ad()
{
    return ad;
}
int Fournisseur::get_num()
{
    return num;
}
int Fournisseur::get_id()
{
    return id;
}

bool Fournisseur::ajouter()
{
    QSqlQuery query;
    QString id_string=QString::number(id);
    QString num_string=QString::number(num);
    query.prepare("INSERT INTO GS_FOURNISSEUR (IDENTIFIANT, NOM,PRENOM,ADRESSE_MAIL,ADRESSE, NUMERO,NOM_PRODUIT) "
                  "VALUES (:identifiant, :nom, :prenom, :adresse_mail, :adresse, :numero ,:nom_produit)");
    query.bindValue(":identifiant", id_string);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":adresse_mail", ad_mail);
    query.bindValue(":adresse", ad);
    query.bindValue(":numero", num_string);
    query.bindValue(":nom_produit", nom_produit);


    return query.exec();
}
QSqlQueryModel* Fournisseur::afficher()
{
    QSqlQueryModel* model=new QSqlQueryModel();

    model->setQuery("SELECT * FROM GS_FOURNISSEUR");
          model->setHeaderData(0, Qt::Horizontal, QObject:: tr("identifiant"));
          model->setHeaderData(1, Qt::Horizontal, QObject:: tr("nom"));
          model->setHeaderData(2, Qt::Horizontal, QObject:: tr("prenom"));
          model->setHeaderData(3, Qt::Horizontal, QObject:: tr("adresse mail"));
          model->setHeaderData(4, Qt::Horizontal, QObject:: tr("adresse"));
          model->setHeaderData(5, Qt::Horizontal, QObject:: tr("numero"));
          model->setHeaderData(6, Qt::Horizontal, QObject:: tr("nom produit"));

          return model;
}

bool Fournisseur::rechercher(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM GS_FOURNISSEUR WHERE IDENTIFIANT = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next())
    {
        int count = query.value(0).toInt();
        return count > 0; // S'il y a au moins un fournisseur avec cet ID, il existe.
    }

    // En cas d'erreur ou d'absence de résultats, considérez que le fournisseur n'existe pas.
    return false;

}
bool Fournisseur::supprimer(int id)
{


QSqlQuery query;
query.prepare("DELETE from GS_FOURNISSEUR where IDENTIFIANT=:id ");
query.bindValue(0, id);

return query.exec();
}
bool Fournisseur::rechercher_modifier(int id)
{
QSqlQuery query;

query.prepare("SELECT * FROM GS_FOURNISSEUR WHERE IDENTIFIANT = :id");
query.bindValue(":id", id);
if (query.exec() && query.next())
{
    // Le fournisseur a été trouvé, vous pouvez récupérer ses données ici
    QString nom = query.value(1).toString();
    QString prenom= query.value(2).toString();
    QString ad_mail = query.value(3).toString();
    QString ad = query.value(4).toString();
    int num = query.value(5).toInt();
    QString nom_produit=query.value(6).toString();
    query.exec();
    return true;
}
return false; // Le fournisseur n'a pas été trouvé
}

QSqlQueryModel* Fournisseur::rechercher_nom(QString nom)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Exécutez une requête pour rechercher les fournisseurs par nom
    query.prepare("SELECT * FROM GS_FOURNISSEUR WHERE NOM = :nom");
    query.bindValue(":nom", nom);

    if (query.exec())
    {
        model->setQuery(query);
    }
    else
    {
        qDebug() << "Erreur lors de la recherche de fournisseurs par nom : " << query.lastError().text();
    }

    return model;
}
QSqlQueryModel* Fournisseur::trierParNomProduit()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    // Exécutez une requête pour sélectionner les fournisseurs triés par nom de produit
    query.prepare("SELECT * FROM GS_FOURNISSEUR ORDER BY NOM_PRODUIT ASC");
    if (query.exec())
    {
        model->setQuery(query);
    }
    else
    {
        qDebug() << "Erreur lors du tri des fournisseurs par nom de produit : " << query.lastError().text();
    }
    return model;
}

QStringList Fournisseur::chargerFournisseursDepuisBaseDeDonnees() //historique
{
QStringList fournisseurs;
// Connexion à la base de données
QSqlDatabase db = QSqlDatabase::database();
if (!db.isValid())
{
    // Gérez les erreurs de connexion à la base de données
    return fournisseurs;
}
QSqlQuery query;
query.prepare("SELECT NOM FROM GS_FOURNISSEUR");
if (query.exec())
{
    while (query.next())
    {
        // Récupérez le nom du fournisseur depuis la base de données
        QString fournisseur = query.value(0).toString();
        fournisseurs.append(fournisseur);
    }
}
else
{

    qDebug() << "Erreur lors de la récupération des noms des fournisseurs : " << query.lastError().text();
    // Gérez les erreurs d'exécution de la requête
}

return fournisseurs;
}


int Fournisseur::obtenirIDFournisseurParNom(const QString& nomFournisseur)
{
QSqlQuery query;
query.prepare("SELECT IDENTIFIANT FROM GS_FOURNISSEUR WHERE NOM = :nom");
query.bindValue(":nom", nomFournisseur);

if (query.exec() && query.next())
{
    return query.value(0).toInt();
}
return -1; // Retourne -1 si le fournisseur n'a pas été trouvé
}

// Obtient la liste des adresses de tous les fournisseurs
QStringList Fournisseur::obtenirAdressesFournisseurs()
{
 QStringList adresses;
 QSqlQuery query;
 query.prepare("SELECT DISTINCT ADRESSE FROM GS_FOURNISSEUR");
 if (query.exec())
    {
 while (query.next())
 {
    QString ville = query.value(0).toString();
    adresses.append(ville);

}
}
else
    {
        qDebug() << "Erreur lors de la récupération de la liste des villes : " << query.lastError().text();
    }
return adresses;
}

// Calcule le pourcentage des fournisseurs pour une adresse donnée
double Fournisseur::pourcentageFournisseursParAdresse(const QString &adresse)
{
QSqlQuery totalQuery;
    totalQuery.prepare("SELECT COUNT(*) FROM GS_FOURNISSEUR");

    QSqlQuery adresseQuery;
    adresseQuery.prepare("SELECT COUNT(*) FROM GS_FOURNISSEUR WHERE ADRESSE = :adresse");
    adresseQuery.bindValue(":adresse", adresse);

    if (totalQuery.exec() && adresseQuery.exec() && totalQuery.next() && adresseQuery.next())
    {
        int total = totalQuery.value(0).toInt();
        int dansAdresse = adresseQuery.value(0).toInt();

        if (total > 0)
        {
            return (static_cast<double>(dansAdresse) / total) * 100.0;
        }
    }

    // En cas d'erreur, retournez -1 ou une valeur qui indique une erreur
    return -1.0;
}

