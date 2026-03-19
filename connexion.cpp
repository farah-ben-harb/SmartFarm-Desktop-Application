#include "connexion.h"


connexion::connexion()
{

}
bool connexion::ouvrirConnexion()
{

    bool test=false;
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("farm");
        db.setUserName("system");//inserer nom de l'utilisateur
        db.setPassword("farah123456789");//inserer mot de passe de cet utilisateur

        if (db.open())
             test=true;

    return  test;
}
