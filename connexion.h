#ifndef CONNEXION_H
#define CONNEXION_H

#include <QSqlDatabase>//connexion
#include <QSqlError>
#include <QSqlQuery>
#include<QDebug>


class connexion
{

private:
    QSqlDatabase db;
public:
    connexion();
    bool ouvrirConnexion();
    void fermerConnexion();
};

#endif // CONNEXION_H
