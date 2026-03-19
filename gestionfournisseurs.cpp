#include "gestionfournisseurs.h"
#include "ui_gestionfournisseurs.h"
#include"affichage.h"
#include"ui_affichage.h"
#include"evenement.h"
#include"modifevent.h"
#include"suppevent.h"


#include<QIntValidator>
#include<QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include<QComboBox>
#include<QDebug>
#include<QSqlError>

#include <QSqlRecord>
#include <QTextTable>
#include <QFile>
#include <QFileDialog>

#include <QPdfWriter>
#include <QPrinter>
#include <QPainter>
#include<QStandardPaths>
#include <QPrinter>
#include <QTextDocument>
#include <QTextCursor>
#include<QDesktopServices>
#include <QStandardPaths>

#include <QTableWidgetItem>

#include<QThread>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include<QBarCategoryAxis>
#include<QTimer>

#include <QDateTime>
GestionFournisseurs::GestionFournisseurs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GestionFournisseurs)
{
    ui->setupUi(this);

    ui->le_id->setValidator(new QIntValidator(0,99999999,this));//id unique ne depasse pas 8 caraters
    ui->le_num->setValidator(new QIntValidator(0,99999999,this));
    ui->le_supp_id->setValidator(new QIntValidator(0,99999999,this));
    ui->tabFOURNISSEUR->setModel(f.afficher());

    Fournisseur f;
    QStringList nomsFournisseurs = f.chargerFournisseursDepuisBaseDeDonnees();

    // Remplissez le QComboBox avec la liste des noms
    ui->comboBox_historique->addItems(nomsFournisseurs);

    connect(ui->calendarWidget_event, &QCalendarWidget::selectionChanged, this, &GestionFournisseurs::on_calendarWidget_event_selectionChanged);
    ui->list_EVENT->setColumnCount(4); // Assurez-vous que le nombre de colonnes est correct
    ui->list_EVENT->setHorizontalHeaderLabels(QStringList() << "Titre" << "Date" << "Description" << "Fournisseur ID");
    chargerDatesEvenements();
}

GestionFournisseurs::~GestionFournisseurs()
{
    delete ui;
}
void GestionFournisseurs::on_pushButton1_clicked()//button ajouter fournisseur
{

    QString id_text=ui->le_id->text();
    QString ad_mail_text=ui->le_ad_mail->text();
    QString num_text=ui->le_num->text();

        QRegExp emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
        if (id_text.length() != 8)
        {
                QMessageBox msgbox;
                msgbox.setText("Le CIN doit avoir exactement 8 chiffres.");
                msgbox.exec();
                return;
         }
        if (num_text.length() != 8)
        {
                QMessageBox msgbox;
                msgbox.setText("Le numéro de téléphone doit avoir exactement 8 chiffres.");
                msgbox.exec();
                return;
         }
        if (!emailRegex.exactMatch(ad_mail_text))
        {
            QMessageBox msgbox;
            msgbox.setText("L'adresse email n'est pas valide. Utilisez le format 'mot@gmail.com'.");
            msgbox.exec();
            return;
        }

        int id=ui->le_id->text().toInt();
        QString nom=ui->le_nom->text();
        QString ad_mail=ui->le_ad_mail->text();
        QString ad=ui->comboBox_ad->currentText();
        int num=ui->le_num->text().toInt();
        QString nom_produit=ui->comboBox_produit->currentText();
        QString prenom=ui->le_prenom->text();

    Fournisseur f(id,nom,prenom,ad_mail,ad,num,nom_produit);

    bool test=f.ajouter();
    QMessageBox msgBox;

    if(test)
    {
        msgBox.setText("ajout avec succés ");
        ui->tabFOURNISSEUR->setModel(f.afficher());
        ui->le_ad_mail->clear();
        ui->le_id->clear();
        ui->le_nom->clear();
        ui->le_num->clear();
        ui->le_prenom->clear();
    }
    else

        msgBox.setText("Echec d'ajout ");


        msgBox.exec();
}
void GestionFournisseurs::on_pushButton2_clicked()//quitter
{
    QApplication::quit();
}

void GestionFournisseurs::on_pushButton_clicked()//button modifier fournisseur
{
    int id = ui->le_supp_id->text().toInt();
    Fournisseur f;
        if (f.rechercher_modifier(id))
        {
            QSqlQuery query;
                    query.prepare("SELECT * FROM GS_FOURNISSEUR WHERE IDENTIFIANT = :id");
                    query.bindValue(":id", id);
                    if (query.exec() && query.next())
                    {
                        QString nom = query.value(1).toString();
                        QString prenom = query.value(2).toString();
                        QString ad_mail = query.value(3).toString();
                        QString ad = query.value(4).toString();
                        int num = query.value(5).toInt();
                        QString nom_produit=query.value(6).toString();

                        affichage dialog(id, nom,prenom, ad_mail, ad,num,nom_produit, this);
                        dialog.exec();
                    }
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "Fournisseur non trouvé.");
        }
}



void GestionFournisseurs::on_pushButton_4_clicked()//button tri par nom produit
{
    // Utilisez la fonction trierParNomProduit de la classe Fournisseur
        Fournisseur f;
        QSqlQueryModel *model = f.trierParNomProduit();

        if (model->rowCount() > 0) {
            ui->tabFOURNISSEUR->setModel(model);
        } else {
            QMessageBox::information(this, "Aucun résultat", "Aucun fournisseur trouvé.");
        }
}
void GestionFournisseurs::on_pushButton_update_tab_clicked()//button actualiser
{
    ui->tabFOURNISSEUR->setModel(f.afficher());
}


void GestionFournisseurs::on_pushButton_rech_par_nom_clicked()//button rechercher par nom
{
    QString nomARechercher = ui->le_rech_nom->text();

        // Utilisez la fonction cherchernom de la classe Fournisseur
        QSqlQueryModel *model = f.rechercher_nom(nomARechercher);

        if (model->rowCount() > 0)
        {
            ui->tabFOURNISSEUR->setModel(model);
            ui->le_rech_nom->clear();
        }
        else
        {
            QMessageBox::information(this, "Aucun résultat", "Aucun fournisseur trouvé avec ce nom.");
        }
}

void GestionFournisseurs::on_pushButton_supp_id_clicked()//button supprimer par id
{
        QMessageBox msgbox;
        Fournisseur f;
        f.set_id(ui->le_supp_id->text().toInt());
        if(f.rechercher(f.get_id()))
        {
            bool test=f.supprimer(f.get_id());
            if(test)
            {
                msgbox.setText("Suppression avec succes.");
                ui->tabFOURNISSEUR->setModel((f.afficher()));
                ui->le_supp_id->clear();
            }
            else
            {
                msgbox.setText("Échec de suppression.");
            }
        }
        else
        {
            msgbox.setText("Le client n'existe pas.\n Echec de suppression.");
        }

}
//historique commande

QStringList GestionFournisseurs::chargerFournisseursDepuisBaseDeDonnees() //historique
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
        // Gérez les erreurs d'exécution de la requête
    }

    return fournisseurs;
}
void GestionFournisseurs::chargerHistoriqueDesCommandes(int fournisseurID)
{
    // Créez un modèle de données (QSqlQueryModel)
    QSqlQueryModel *historiqueModel = new QSqlQueryModel(this);

    // Utilisez une requête SQL pour récupérer l'historique des commandes du fournisseur
    QSqlQuery query;
    query.prepare("SELECT ID_FOURNISSEUR,CODE_PRODUIT,DATE_COMMANDE,QUANTITE,PRIX FROM GS_COMMANDE WHERE ID_FOURNISSEUR = :fournisseurID");
    query.bindValue(":fournisseurID", fournisseurID);
    if (query.exec())
    {
        historiqueModel->setQuery(query);
        ui->tab_historique->setModel(historiqueModel);

        // Masquer la colonne d'ID_FOURNISSEUR (index 0)
                ui->tab_historique->hideColumn(0);

    } else
    {
        // Gérez l'erreur si la requête échoue
        QSqlError error = query.lastError();  // Utilisez QSqlError pour obtenir des informations sur l'erreur
            qDebug() << "Erreur SQL : " << error.text();
    }
}
void GestionFournisseurs::on_comboBox_historique_currentIndexChanged(int index)
{
    if (index != -1)
    {
           QString nomFournisseur = ui->comboBox_historique->currentText();
           int fournisseurID = f.obtenirIDFournisseurParNom(nomFournisseur);

           if (fournisseurID != -1)
           {
               chargerHistoriqueDesCommandes(fournisseurID);

           }
           else
           {
               QMessageBox::information(this, "Information", "Fournisseur non trouvé.");
           }
       }
}
bool GestionFournisseurs::fournisseurExiste(const int &fournisseurId)
{

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM GS_FOURNISSEUR WHERE IDENTIFIANT = :fournisseur_id");
    query.bindValue(":fournisseur_id", fournisseurId);

    if (query.exec() && query.next())
    {
        int count = query.value(0).toInt();
        return count > 0; // S'il y a au moins un fournisseur avec cet ID, il existe.
    }

    // En cas d'erreur ou d'absence de résultats, considérez que le fournisseur n'existe pas.
    return false;
}

//event
bool GestionFournisseurs::evenementExiste(const QString &titre)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM GS_EVENEMENT WHERE TITRE = :titre");
    query.bindValue(":titre", titre);

    if (query.exec() && query.next())
    {
        int count = query.value(0).toInt();
        return count > 0;
    }
    else
    {
        qDebug() << "Erreur lors de la vérification de l'existence de l'événement : " << query.lastError().text();
        return false;
    }
}
void GestionFournisseurs::initialiserCalendrier()
{
    // Chargez les événements existants et mettez à jour le calendrier avec les couleurs appropriées
    rafraichirCalendrier();
}
void GestionFournisseurs::enregistrerEvenement(const QString &date, const QString &titre, const QString &description, const int &fournisseurId)
{
    // Après avoir ajouté une ligne dans enregistrerEvenement ou toute autre méthode où vous ajoutez des lignes
    for (int i = 0; i < ui->list_EVENT->columnCount(); ++i) {
        ui->list_EVENT->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }

    if (fournisseurExiste(fournisseurId))
    {
        QSqlQuery query;
        query.prepare("INSERT INTO GS_EVENEMENT (TITRE,DATE_EVENT, DESCRIPTION, FOURNISSEUR_ID) "
                      "VALUES (:titre, :date_event, :description, :fournisseur_id)");
        query.bindValue(":titre", titre);
        query.bindValue(":date_event", date);
        query.bindValue(":description", description);
        query.bindValue(":fournisseur_id", fournisseurId);

        if (query.exec())
        {
            // L'événement a été ajouté avec succès à la base de données.
            QMessageBox::information(this, "Succès", "L'événement a été ajouté avec succès.");



            // Réinitialisez le calendrier et mettez à jour les dates des événements
            QTextCharFormat format;
                        format.setForeground(Qt::green);
                        QDate selectedDate = QDate::fromString(date, "dd/MM/yyyy");
                        ui->calendarWidget_event->setDateTextFormat(selectedDate, format);
            rafraichirCalendrier();
            //chargerDatesEvenements(); // Mettez à jour le calendrier avec les nouvelles dates

            // Ajoutez l'événement à la tableWidget_EVENT
            int rowCount = ui->list_EVENT->rowCount();
            ui->list_EVENT->insertRow(rowCount);
            ui->list_EVENT->setItem(rowCount,0, new QTableWidgetItem(titre));
            ui->list_EVENT->setItem(rowCount, 1, new QTableWidgetItem(date));
            ui->list_EVENT->setItem(rowCount, 2, new QTableWidgetItem(description));
            ui->list_EVENT->setItem(rowCount, 3, new QTableWidgetItem(QString::number(fournisseurId)));

            // Ajout de messages de débogage
            qDebug() << "Événement ajouté avec succès à la tableWidget_EVENT :";
            qDebug() << "Date : " << date;
            qDebug() << "Titre : " << titre;
            qDebug() << "Description : " << description;
            qDebug() << "Fournisseur ID : " << fournisseurId;

            // Mettez à jour le modèle
                      ui->list_EVENT->update();
        }
        else
        {
            // Gérez les erreurs si l'ajout échoue.
            qDebug() << "Erreur lors de l'ajout d'un événement : " << query.lastError().text();
            QMessageBox::critical(this, "Erreur", "Impossible d'ajouter l'événement. Veuillez réessayer.");
        }
    }
    else
     {
               QMessageBox::critical(this, "Erreur", "Le fournisseur avec l'ID spécifié n'existe pas.");

       }
}

void GestionFournisseurs::on_pushButton_ajout_event_clicked()
{
    evenement evenementDialog(this);

    connect(&evenementDialog, &evenement::evenementEnregistre, this, [=](const QString &date, const QString &titre, const QString &description, int id)
    {
        int fournisseur_id = id;

        if (fournisseurExiste(fournisseur_id))
        {
            enregistrerEvenement(date, titre, description, fournisseur_id);
            rafraichirCalendrier(); // Rafraîchir le calendrier après avoir ajouté un nouvel événement
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Le fournisseur avec l'ID spécifié n'existe pas.");
        }
    });

    evenementDialog.exec();
}

void GestionFournisseurs::on_pushButton_consulter_event_clicked()
{ QSqlQuery query;
    query.prepare("SELECT * FROM GS_EVENEMENT");

    if (query.exec())
    {
        ui->list_EVENT->clearContents();
        ui->list_EVENT->setRowCount(0);

        int row = 0;

        while (query.next())
        {
            QString dateEvent = query.value(0).toString();
            QString titre = query.value(1).toString();
            QString description = query.value(2).toString();
            int fournisseurId = query.value(3).toInt();

            ui->list_EVENT->insertRow(row);
            ui->list_EVENT->setItem(row, 0, new QTableWidgetItem(dateEvent));
            ui->list_EVENT->setItem(row, 1, new QTableWidgetItem(titre));
            ui->list_EVENT->setItem(row, 2, new QTableWidgetItem(description));
            ui->list_EVENT->setItem(row, 3, new QTableWidgetItem(QString::number(fournisseurId)));

            ++row;

        }


    }
    else
    {
        qDebug() << "Erreur lors de la récupération des événements : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les événements. Veuillez réessayer.");
    }
}
void GestionFournisseurs::on_pushButton_modif_Event_clicked()
{
    ModifEvent modifeventDialog(this);

        connect(&modifeventDialog, &ModifEvent::verifierId, this, &GestionFournisseurs::verifierId);

        modifeventDialog.exec();
}
void GestionFournisseurs::verifierId(int id)
{
    ModifEvent modifeventDialog(this);
    if (fournisseurExiste(id)) {
            // Récupérez les données de l'événement à partir de la base de données.
            QSqlQuery query;
            query.prepare("SELECT TITRE, DESCRIPTION, DATE_EVENT FROM GS_EVENEMENT WHERE FOURNISSEUR_ID = :fournisseur_id");
            query.bindValue(":fournisseur_id", id);

            if (query.exec() && query.next()) {
                QString titre = query.value(0).toString();
                QString description = query.value(1).toString();
                QString date = query.value(2).toString();

                // Émettez le signal pour transmettre les données à ModifEvent
                emit transmettreDonneesEvenement(id, titre, description, date);

                // Appelez le slot afficherDonnees pour afficher les données dans ModifEvent
                modifeventDialog.afficherDonnees(id, titre, description, date);

                // Ouvrez la fenêtre ModifEvent
                if (modifeventDialog.exec() == QDialog::Accepted) {
                    // Connectez le signal de ModifEvent pour enregistrer les modifications
                    connect(&modifeventDialog, &ModifEvent::enregistrerModifications, this, &GestionFournisseurs::enregistrerModifications);
                        modifeventDialog.exec();
                }
            }
        } else {
            modifeventDialog.afficherMessageErreur("L'ID du fournisseur n'existe pas.");
        }
}
void GestionFournisseurs::enregistrerModifications(int id, const QString &date, const QString &titre, const QString &description)
{
    QSqlQuery query;
        query.prepare("UPDATE GS_EVENEMENT SET DATE_EVENT = :date, TITRE = :titre, DESCRIPTION = :description WHERE FOURNISSEUR_ID = :fournisseur_id");

        query.bindValue(":date", date);
        query.bindValue(":titre", titre);
        query.bindValue(":description", description);
        query.bindValue(":fournisseur_id", id);

        if (query.exec()) {
            QTableWidgetItem *item = ui->list_EVENT->item(ui->list_EVENT->currentRow(), 0);
                    if (item)
                    {
                        item->setText(date);
                    }

                    item = ui->list_EVENT->item(ui->list_EVENT->currentRow(), 1);
                    if (item)
                    {
                        item->setText(titre);
                    }

                    item = ui->list_EVENT->item(ui->list_EVENT->currentRow(), 2);
                    if (item)
                    {
                        item->setText(description);
                    }

                    QMessageBox::information(this, "Succès", "Les modifications ont été enregistrées avec succès.");
                    // Mettez à jour le calendrier
                            rafraichirCalendrier();
                }
                else
                {
                    qDebug() << "Erreur lors de l'enregistrement des modifications : " << query.lastError().text();
                    QMessageBox::critical(this, "Erreur", "Impossible d'enregistrer les modifications. Veuillez réessayer.");
                }
}


void GestionFournisseurs::on_pushButton_supprimer_event_clicked()
{
    // Ouvrez la fenêtre SuppEvent pour obtenir l'ID à supprimer
        SuppEvent suppEventDialog(this);

        // Connectez le signal de SuppEvent pour transmettre l'ID à supprimer
        connect(&suppEventDialog, &SuppEvent::suppressionDemandee, this, &GestionFournisseurs::supprimerEvent);

        // Ouvrez la fenêtre SuppEvent
        if (suppEventDialog.exec() == QDialog::Accepted) {
            // La fenêtre s'est fermée avec accept()
            // Vous pouvez ajouter un code ici si nécessaire
        }
}
void GestionFournisseurs::supprimerEvent(int id)
{
    if (fournisseurExiste(id))
        {
            QSqlQuery query;
            query.prepare("DELETE FROM GS_EVENEMENT WHERE FOURNISSEUR_ID = :fournisseur_id");
            query.bindValue(":fournisseur_id", id);

            if (query.exec())
            {
                QMessageBox::information(this, "Succès", "L'événement a été supprimé avec succès.");

                // Mettez à jour l'affichage dans le QTableWidget
                int selectedRow = ui->list_EVENT->currentRow();
                if (selectedRow != -1)
                {
                    ui->list_EVENT->removeRow(selectedRow);
                }

                rafraichirCalendrier(); // Rafraîchir le calendrier après la suppression d'un événement
            }
            else
            {
                qDebug() << "Erreur lors de la suppression de l'événement : " << query.lastError().text();
                QMessageBox::critical(this, "Erreur", "Impossible de supprimer l'événement. Veuillez réessayer.");
            }

    }
       else
       {
           QMessageBox::critical(this, "Erreur", "L'ID du fournisseur n'existe pas.");
       }
}
void GestionFournisseurs::rafraichirCalendrier()
{
    QSqlQuery query;
       query.prepare("SELECT TITRE, DATE_EVENT FROM GS_EVENEMENT");

       if (query.exec())
       {
           while (query.next())
           {
               QString titre = query.value(0).toString();
               QString dateString = query.value(1).toString();
               QDate selectedDate = QDate::fromString(dateString, "dd/MM/yyyy");

               QTextCharFormat format;

               // Appliquez le format à la date dans le calendrier en fonction du type d'événement
               if (titre == "livraison")
               {
                   format.setForeground(Qt::magenta); // Couleur rose pour livraison
               }
               else if (titre == "reunion")
               {
                   format.setForeground(Qt::green); // Couleur verte pour réunion
               }
               else if (titre == "promotion")
               {
                   format.setForeground(Qt::darkYellow); // Couleur jaune pour promotion
               }
               else if (titre == "nouvelle arrivage")
               {
                   format.setForeground(Qt::blue); // Couleur bleue pour nouvelle arrivage
               }
               else if (titre == "irrigation")
               {
                   format.setForeground(Qt::darkRed); // Couleur bleue pour nouvelle arrivage
               }
               else
               {
                   // Couleur par défaut pour les autres types d'événements
                   format.setForeground(Qt::black);
               }

               // Obtenez le format actuel pour la date
               QTextCharFormat existingFormat = ui->calendarWidget_event->dateTextFormat(selectedDate);

               // Fusionnez les formats existants avec le nouveau format
               existingFormat.merge(format);

               // Appliquez le format fusionné à la date dans le calendrier
               ui->calendarWidget_event->setDateTextFormat(selectedDate, existingFormat);
           }
       }
       else
       {
           qDebug() << "Erreur lors de la récupération des dates d'événements : " << query.lastError().text();
       }
}
void GestionFournisseurs::chargerDatesEvenements()
{
    QSqlQuery query;
            query.prepare("SELECT DATE_EVENT FROM GS_EVENEMENT");

            if (query.exec())
            {
                QDate selectedDate;
                QTextCharFormat format;

                while (query.next())
                {
                    QString dateString = query.value(0).toString();
                    selectedDate = QDate::fromString(dateString, "dd/MM/yyyy");
                    format.setForeground(Qt::green);
                    ui->calendarWidget_event->setDateTextFormat(selectedDate,format);
                }
            }
            else
            {
                qDebug() << "Erreur lors de la récupération des dates d'événements : " << query.lastError().text();
}
}
void GestionFournisseurs::on_calendarWidget_event_selectionChanged()
{
    QDate selectedDate = ui->calendarWidget_event->selectedDate();
       chargerEvenements(selectedDate);
}

void GestionFournisseurs::chargerEvenements(const QDate &date)
{
    ui->list_EVENT->clearContents();
        ui->list_EVENT->setRowCount(0);

        QSqlQuery query;
        query.prepare("SELECT TITRE, DESCRIPTION, FOURNISSEUR_ID FROM GS_EVENEMENT WHERE DATE_EVENT = :date");
        QString dateString = date.toString("dd/MM/yyyy");
        query.bindValue(":date", dateString);

        if (query.exec())
        {
            int row = 0;
            while (query.next())
            {
                QString titre = query.value(0).toString();
                QString description = query.value(1).toString();
                int fournisseurId = query.value(2).toInt();

                ui->list_EVENT->insertRow(row);

                ui->list_EVENT->setItem(row, 0, new QTableWidgetItem(titre));
                ui->list_EVENT->setItem(row, 1, new QTableWidgetItem(dateString));
                ui->list_EVENT->setItem(row, 2, new QTableWidgetItem(description));
                ui->list_EVENT->setItem(row, 3, new QTableWidgetItem(QString::number(fournisseurId)));

                ++row;
            }
        }
        else
        {
            qDebug() << "Erreur lors de la récupération des événements : " << query.lastError().text();
            QMessageBox::critical(this, "Erreur", "Impossible de récupérer les événements. Veuillez réessayer.");
        }
}

void GestionFournisseurs::on_pushButton_fichierpdf_clicked()
{Fournisseur F;

    QString nom_fichier_pdf = "Liste des fournisseurs.pdf"; // Nom du fichier PDF
    QString nomfichier = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", nom_fichier_pdf, "Fichiers PDF (*.pdf)");

    // Créez un objet QPrinter pour le PDF
    QPrinter pdfPrinter(QPrinter::PrinterResolution);
    pdfPrinter.setOutputFormat(QPrinter::PdfFormat);
    pdfPrinter.setOutputFileName(nomfichier);

    // Créez un objet QPainter pour dessiner le contenu du PDF
    QPainter painter(&pdfPrinter);

    if (!painter.isActive()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de générer le fichier PDF.");
        return;
    }
    // Création d'un objet QImage avec le chemin de l'image

        QImage image("C:/Users/Farah/OneDrive/Bureau/Documents/2 eme annee/projet c++/gestion des fournisseurs design/gestion_des_fournisseurs/logo.png");
        // Vérifier si l'image a été chargée correctement
         if (image.isNull())
         {
                    QMessageBox::critical(nullptr, "Erreur", "Impossible de charger l'image.");
                    return;
          }

        // Dessiner l'image en haut à gauche du PDF
        painter.drawImage(QRect(8, 2, 80, 80), image);

        if (!painter.isActive())
        {
            QMessageBox::critical(nullptr, "Erreur", "Impossible de générer le fichier PDF.");
            return;
        }
    // Récupération du modèle de tab_Fournisseur
    QSqlQueryModel *model = F.afficher();
    int nb_colonnes = model->columnCount(); // Nombre de colonnes dans tab_clent
    int espac_entre_Colonnes = 3;           // 2 pixels entre chaque colonne
    int case_Width = (pdfPrinter.width() - (nb_colonnes - 1) * espac_entre_Colonnes) / nb_colonnes;
    int case_Height = 30;
    int x = 2;
    int y = 90;


    // Définir la couleur du texte en vert
        painter.setPen(QColor(Qt::darkGreen));
    // Police pour le titre
    QFont titleFont;
    titleFont.setBold(true); // setBold pour gras
    titleFont.setPointSize(16);
    painter.setFont(titleFont);
    QString titre = "Liste des fournisseurs";
    int titreWidth = pdfPrinter.width();
    int titreHeight = 40;
    painter.drawText(0, 0, titreWidth, titreHeight, Qt::AlignCenter, titre);

    // Réinitialiser la couleur du texte à la valeur par défaut
        painter.setPen(Qt::black);

    // Police pour les données
    QFont texte_case;
    texte_case.setPointSize(10);
    painter.setFont(texte_case);

    // Titres des colonnes
    for (int col = 0; col < nb_colonnes; col++) {
        // Définir la couleur du texte en orange pour les titres de colonnes
               painter.setPen(Qt::green);
        painter.drawText(x, y, case_Width, case_Height, Qt::AlignCenter, model->headerData(col, Qt::Horizontal).toString());
        // Réinitialiser la couleur du texte à la valeur par défaut
            painter.setPen(Qt::black);
        x += case_Width;
    }

    x = 20; // Réinitialisation
    y += case_Height;

    // Dessinez les données des fournisseurs avec des lignes de séparation
    for (int i = 0; i < model->rowCount(); i++) {
        for (int col = 0; col < nb_colonnes; col++) {
            painter.drawText(x, y, case_Width, case_Height, Qt::AlignLeft | Qt::AlignVCenter, model->data(model->index(i, col)).toString());
            x += case_Width;
        }

        // Dessinez une ligne de séparation
        painter.drawLine(20, y, pdfPrinter.width() - 20, y);

        // Réinitialisez la position horizontale et passez à la ligne suivante
        x = 20;
        y += case_Height;
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
            QString dateTimeString = currentDateTime.toString("dd/MM/yyyy HH:mm:ss");
            QFont dateTimeFont;
            dateTimeFont.setPointSize(8);
            painter.setFont(dateTimeFont);
            painter.drawText(pdfPrinter.width() - 150, 2, 150, 20, Qt::AlignRight, dateTimeString);
    painter.end();

    QMessageBox::information(nullptr, "Succès", "Le fichier PDF a été généré avec succès : " + nomfichier);

    // Ouvrez le fichier PDF avec le lecteur PDF par défaut
    QDesktopServices::openUrl(QUrl::fromLocalFile(nomfichier));
}



void GestionFournisseurs::on_pushButton_5_clicked()//fichier commande button
{
    QString nom_fichier_pdf = "Liste des Commandes.pdf"; // Nom du fichier PDF
       QString nomfichier = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", nom_fichier_pdf, "Fichiers PDF (*.pdf)");

       // Créez un objet QPrinter pour le PDF
       QPrinter pdfPrinter(QPrinter::PrinterResolution);
       pdfPrinter.setOutputFormat(QPrinter::PdfFormat);
       pdfPrinter.setOutputFileName(nomfichier);

       // Créez un objet QPainter pour dessiner le contenu du PDF
       QPainter painter(&pdfPrinter);

       if (!painter.isActive()) {
           QMessageBox::critical(this, "Erreur", "Impossible de générer le fichier PDF.");
           return;
       }
       // Création d'un objet QImage avec le chemin de l'image

           QImage image("C:/Users/Farah/OneDrive/Bureau/Documents/2 eme annee/projet c++/gestion des fournisseurs design/gestion_des_fournisseurs/logo.png");
           // Vérifier si l'image a été chargée correctement
            if (image.isNull())
            {
                       QMessageBox::critical(nullptr, "Erreur", "Impossible de charger l'image.");
                       return;
             }

           // Dessiner l'image en haut à gauche du PDF
           painter.drawImage(QRect(8, 2, 80, 80), image);
       // Créez un modèle de données pour obtenir les données des commandes
       QSqlQueryModel model;
       model.setQuery("SELECT * FROM GS_COMMANDE ");

       int numCols = model.columnCount(); // Nombre de colonnes dans le modèle
       int espac_entre_Colonnes = 3;      // 2 pixels entre chaque colonne
       int cellWidth = (pdfPrinter.width() - (numCols - 1) * espac_entre_Colonnes) / numCols;
       int cellHeight = 30;
       int x =2;
       int y = 90;

       // Définir la couleur du texte en vert
           painter.setPen(QColor(Qt::darkGreen));
       // Police pour le titre
       QFont titleFont;
       titleFont.setBold(true);
       titleFont.setPointSize(16);
       painter.setFont(titleFont);
       QString titre = "Liste des commandes";
       int titreWidth = pdfPrinter.width();
       int titreHeight = 40;
       painter.drawText(0, 0, titreWidth, titreHeight, Qt::AlignCenter, titre);

       // Réinitialiser la couleur du texte à la valeur par défaut
           painter.setPen(Qt::black);

       // Police pour les données
       QFont texte_case;
       texte_case.setPointSize(8);
       painter.setFont(texte_case);

       // Titres des colonnes
       for (int col = 0; col < numCols; col++) {
           // Définir la couleur du texte en orange pour les titres de colonnes
                  painter.setPen(Qt::green);
           painter.drawText(x, y, cellWidth, cellHeight, Qt::AlignCenter, model.headerData(col, Qt::Horizontal).toString());
           // Réinitialiser la couleur du texte à la valeur par défaut
                   painter.setPen(Qt::black);

           x += cellWidth;
       }

       x = 20; // Réinitialisation
       y += cellHeight;

       // Dessinez les données des commandes
       for (int i = 0; i < model.rowCount(); i++)
       {
           for (int col = 0; col < numCols; col++)
           {
               painter.drawText(x, y, cellWidth, cellHeight, Qt::AlignLeft | Qt::AlignVCenter, model.data(model.index(i, col)).toString());
               x += cellWidth;
           }
           // Dessinez une ligne de séparation

                   painter.drawLine(20, y, pdfPrinter.width() - 20, y);
           x = 20;
           y += cellHeight;

       }
       QDateTime currentDateTime = QDateTime::currentDateTime();
               QString dateTimeString = currentDateTime.toString("dd/MM/yyyy HH:mm:ss");
               QFont dateTimeFont;
               dateTimeFont.setPointSize(8);
               painter.setFont(dateTimeFont);
               painter.drawText(pdfPrinter.width() - 150, 2, 150, 20, Qt::AlignRight, dateTimeString);
       painter.end();
       painter.end();

       QMessageBox::information(this, "Succès", "Le fichier PDF a été généré avec succès : " + nomfichier);

       // Ouvrez le fichier PDF avec le lecteur PDF par défaut
       QDesktopServices::openUrl(QUrl::fromLocalFile(nomfichier));
}

void GestionFournisseurs::afficherGrapheStatistiques(const QStringList &adresses, const QVector<double> &pourcentages)
{
    // Création d'un objet avec des barres de stat
        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();

        series->setBarWidth(1);

        for (int i = 0; i < adresses.size(); ++i)
        {
            qDebug() << "Adresse : " << adresses[i] << " Pourcentage : " << pourcentages[i];
            QtCharts::QBarSet *set = new QtCharts::QBarSet(adresses[i]);
            *set << pourcentages[i];
            series->append(set);
        }

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Statistiques des Villes");

        //Position des labels des barres
        series->setLabelsVisible(true);
        series->setLabelsPosition(QtCharts::QAbstractBarSeries::LabelsOutsideEnd);

        // Création d'un axe  Y pour les pourcentages
        QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
        axisY->setRange(0, 100); // de 0 à 100 pourcentage
        axisY->setLabelFormat("%.0f %"); // Format des étiquettes sans décimales
        axisY->setTickCount(8); // Nombre de graduations
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *graphWindow = new QMainWindow(this);
        graphWindow->setCentralWidget(chartView);
        graphWindow->resize(800, 600);
        graphWindow->show();
}
void GestionFournisseurs::on_pushButton_7_clicked()//stat par adresse
{

    Fournisseur f;
    // Récupérez la liste des adresses des fournisseurs
      QStringList adresses = f.obtenirAdressesFournisseurs();

      QVector<double> tab_pourcentages;

      for (const QString &adresse : adresses)
      {
          double pourcentage = f.pourcentageFournisseursParAdresse(adresse);
          if (pourcentage != -1.0)
          {
              tab_pourcentages.append(pourcentage);
          }
          else
          {
              qDebug() << "Erreur lors de la récupération des statistiques pour " << adresse;
          }
      }

      // Triez les adresses et les pourcentages en fonction des pourcentages (de haut en bas)
      QVector<int> indices(adresses.size());
      std::iota(indices.begin(), indices.end(), 0);
      std::sort(indices.begin(), indices.end(), [&tab_pourcentages](int a, int b)
      {
          return tab_pourcentages[a] > tab_pourcentages[b];
      });

      QStringList adressesTriees;
      QVector<double> pourcentagesTriees;
      for (int index : indices)
      {
          adressesTriees.append(adresses[index]);
          pourcentagesTriees.append(tab_pourcentages[index]);
      }

      afficherGrapheStatistiques(adressesTriees, pourcentagesTriees);

      QMessageBox msgbox;
      msgbox.setText("Statistiques mises à jour avec succès.");
      msgbox.exec();
}




void GestionFournisseurs::on_pushButton_eventpdf_clicked()
{
    QString nom_fichier_pdf = "Liste des Evenements.pdf"; // Nom du fichier PDF
       QString nomfichier = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", nom_fichier_pdf, "Fichiers PDF (*.pdf)");

       // Créez un objet QPrinter pour le PDF
       QPrinter pdfPrinter(QPrinter::PrinterResolution);
       pdfPrinter.setOutputFormat(QPrinter::PdfFormat);
       pdfPrinter.setOutputFileName(nomfichier);

       // Créez un objet QPainter pour dessiner le contenu du PDF
       QPainter painter(&pdfPrinter);

       if (!painter.isActive()) {
           QMessageBox::critical(this, "Erreur", "Impossible de générer le fichier PDF.");
           return;
       }
       // Création d'un objet QImage avec le chemin de l'image

           QImage image("C:/Users/Farah/OneDrive/Bureau/Documents/2 eme annee/projet c++/gestion des fournisseurs design/gestion_des_fournisseurs/logo.png");
           // Vérifier si l'image a été chargée correctement
            if (image.isNull())
            {
                       QMessageBox::critical(nullptr, "Erreur", "Impossible de charger l'image.");
                       return;
             }

           // Dessiner l'image en haut à gauche du PDF
           painter.drawImage(QRect(8, 2, 80, 80), image);
       // Créez un modèle de données pour obtenir les données des commandes
       QSqlQueryModel model;
       model.setQuery("SELECT * FROM GS_EVENEMENT ");

       int numCols = model.columnCount(); // Nombre de colonnes dans le modèle
       int espac_entre_Colonnes = 3;      // 2 pixels entre chaque colonne
       int cellWidth = (pdfPrinter.width() - (numCols - 1) * espac_entre_Colonnes) / numCols;
       int cellHeight = 30;
       int x =2;
       int y = 90;

       // Définir la couleur du texte en vert
           painter.setPen(QColor(Qt::darkGreen));
       // Police pour le titre
       QFont titleFont;
       titleFont.setBold(true);
       titleFont.setPointSize(16);
       painter.setFont(titleFont);
       QString titre = "Liste des evenements";
       int titreWidth = pdfPrinter.width();
       int titreHeight = 40;
       painter.drawText(0, 0, titreWidth, titreHeight, Qt::AlignCenter, titre);

       // Réinitialiser la couleur du texte à la valeur par défaut
           painter.setPen(Qt::black);

       // Police pour les données
       QFont texte_case;
       texte_case.setPointSize(8);
       painter.setFont(texte_case);

       // Titres des colonnes
       for (int col = 0; col < numCols; col++) {
           // Définir la couleur du texte en orange pour les titres de colonnes
                  painter.setPen(Qt::green);
           painter.drawText(x, y, cellWidth, cellHeight, Qt::AlignCenter, model.headerData(col, Qt::Horizontal).toString());
           // Réinitialiser la couleur du texte à la valeur par défaut
                   painter.setPen(Qt::black);

           x += cellWidth;
       }

       x = 20; // Réinitialisation
       y += cellHeight;

       // Dessinez les données des commandes
       for (int i = 0; i < model.rowCount(); i++)
       {
           for (int col = 0; col < numCols; col++)
           {
               painter.drawText(x, y, cellWidth, cellHeight, Qt::AlignLeft | Qt::AlignVCenter, model.data(model.index(i, col)).toString());
               x += cellWidth;
           }
           // Dessinez une ligne de séparation

                   painter.drawLine(20, y, pdfPrinter.width() - 20, y);
           x = 20;
           y += cellHeight;

       }
       QDateTime currentDateTime = QDateTime::currentDateTime();
               QString dateTimeString = currentDateTime.toString("dd/MM/yyyy HH:mm:ss");
               QFont dateTimeFont;
               dateTimeFont.setPointSize(8);
               painter.setFont(dateTimeFont);
               painter.drawText(pdfPrinter.width() - 150, 2, 150, 20, Qt::AlignRight, dateTimeString);
       painter.end();
       painter.end();

       QMessageBox::information(this, "Succès", "Le fichier PDF a été généré avec succès : " + nomfichier);

       // Ouvrez le fichier PDF avec le lecteur PDF par défaut
       QDesktopServices::openUrl(QUrl::fromLocalFile(nomfichier));
}
