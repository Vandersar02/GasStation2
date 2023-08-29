/// Nom: ST Cyr Lee J Vandersar

#pragma region Mes classes et bibliothèques
#include <iostream>
#include "sqlite3.h"
#include <Windows.h>
#include <thread>
#include <vector>
#include <conio.h> //Inclure la bibliothèque conio.h pour getch()
#include <cstdlib> // pour avoir accès au 'system'
#include <ctime> // afin d' avoir la date et l' heure au moment donné
#include <string>

#pragma endregion

using namespace std;

#pragma region Code color for text in console
// Codes d'échappement ANSI pour les couleurs du texte
const string ANSI_COLOR_RED = "\033[31m";
const string ANSI_COLOR_GREEN = "\033[32m";
const string ANSI_COLOR_YELLOW = "\033[33m";
const string ANSI_COLOR_BLUE = "\033[34m";
const string ANSI_COLOR_BROWN = "\033[33m";
const string ANSI_COLOR_DARK_GRAY = "\033[90m";
const string ANSI_COLOR_RESET = "\033[0m";
#pragma endregion

// Structure pour stocker l'information d'historique
struct ModificationHistory {
    int idPersonne;
    string modificationType;
    string date;
};

vector<ModificationHistory> historyList;

// fonction qui va donner la date et l'heure 
void DateTime(string& Date, string& Time) {

    // Obtenir le temps actuel
    time_t currentTime; time(&currentTime);

    // Convertir le temps en une structure tm (contenant la date et l'heure)
    tm time; localtime_s(&time, &currentTime);

    Date = to_string(time.tm_year + 1900) + "-" + to_string(time.tm_mon + 1) + "-" + to_string(time.tm_mday);
    Time = to_string(time.tm_hour) + ":" + to_string(time.tm_min) + ":" + to_string(time.tm_sec);
}

#pragma region Fonction pour Pompiste
void SaisirDetailsVente(int idPompiste, sqlite3* db) {
    string type, devise = "gdes", unite = "gallon(s)"; double quantite, prixPompe;
    string modificationType = "Nouvelle vente";
    string date, time;
    DateTime(date, time); // fonction qui donne la date et l' heure du jour
    
    while (true) {
        system("cls"); // Efface l'écran pour une meilleure lisibilité
        cout << ANSI_COLOR_GREEN << "Saisie des détails de la vente : " << endl;
        cout << "---------------------------------------" << ANSI_COLOR_RESET << endl;

        // Saisie du type de carburant
        cout << "Type (gazoline, kerozene, diesel, propane): ";
        cin >> type;

        // Saisie de la quantité de carburant
        cout << "Quantité de (" << unite << "): ";
        cin >> quantite;

        // Saisie du prix à la pompe
        cout << "Prix à la pompe ("<< devise << "/" << unite << ") : ";
        cin >> prixPompe;

        // Effacer l'écran
        system("cls"); // Efface l'écran pour une meilleure lisibilité
        cout << ANSI_COLOR_DARK_GRAY << "Récapitulatif de la vente : " << endl;
        cout << "---------------------------------------" << endl;
        cout << "Identifiant du pompiste : " << idPompiste << endl;
        cout << "Date et heure de vente : " << date << " " << time << endl;
        cout << "Type : " << type << endl;
        cout << "Quantité de "<<type <<": " << quantite << " gallon(s)" << endl;
        cout << "Prix à la pompe : " << prixPompe << devise <<"/" << unite << endl;
        cout << "---------------------------------------" << ANSI_COLOR_RESET << endl;

        // Demander à l'utilisateur de confirmer ou revenir en arrière
        char choix;
        cout << "Confirmer la vente (C) ou revenir en arrière (R) ? ";
        cin >> choix;

        if (choix == 'C' || choix == 'c') {
            // Effectuer l'enregistrement de la vente dans la base de données
            const char* insertQuery = "INSERT INTO Flux (date, idEmployee, type, quantite, prixunitaire, referenceExterne, typeFlux, devise)"
                "VALUES ( ?, ?, ?, ?, ?, 0, 'vente', 'gourdes')";
            sqlite3_stmt* stmt;

            int rc = sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr);
            if (rc == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 2, idPompiste);
                sqlite3_bind_text(stmt, 3, type.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_double(stmt, 4, quantite);
                sqlite3_bind_double(stmt, 5, prixPompe);

                rc = sqlite3_step(stmt);
                if (rc != SQLITE_DONE) {
                    cerr << "Erreur lors de l'insertion de données : " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_finalize(stmt);
            }
            else {
                cerr << "Erreur lors de la préparation de la requête : " << sqlite3_errmsg(db) << endl;
            }

            cout << ANSI_COLOR_GREEN << "Vente enregistrée avec succès !" << ANSI_COLOR_RESET << endl;
            // Enregistrez les informations dans la structure
            ModificationHistory historyEntry;
            historyEntry.idPersonne = idPompiste;
            historyEntry.modificationType = modificationType;
            historyEntry.date = date + " " + time;

            // Ajoutez l'entrée d'historique à la liste
            historyList.push_back(historyEntry);

            break; // Sortir de la boucle
        }

        else if (choix == 'R' || choix == 'r') {
            // Revenir en arrière pour refaire la saisie
            system("cls"); // Efface l'écran pour une meilleure lisibilité
            continue; // Passer à l'itération suivante de la boucle
        }
        else {
            cout << "Choix invalide. Vente annulée" << endl;
            system("pause"); // faire une pause avnat de redonner la possibilte de placer une vente
            continue; // Passer à l'itération suivante de la boucle
        }
    }
}
 
void AfficherHistoriqueVente(int idPompiste, sqlite3* db) {
    int nHistorique = 1;
    const char* selectHistoriqueQuery = "SELECT date, type, quantite, prixunitaire FROM Flux WHERE idEmployee = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, selectHistoriqueQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << ANSI_COLOR_RED << "Erreur lors de la préparation de la requête : " << sqlite3_errmsg(db) << ANSI_COLOR_RESET << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, idPompiste);

    system("cls"); // Efface l'écran pour une meilleure lisibilité
    cout << ANSI_COLOR_DARK_GRAY << "Historique de vente pour le pompiste (idPompiste = " << idPompiste << ")" << ANSI_COLOR_RESET << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string dateHeure = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string typeCarburant = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double quantite = sqlite3_column_double(stmt, 2);
        double prixPompe = sqlite3_column_double(stmt, 3);

        cout << ANSI_COLOR_DARK_GRAY << "--------------------" << nHistorique << "--------------------" << endl;
        cout << "Date et heure : " << dateHeure << endl;
        cout << "Type de carburant : " << typeCarburant << endl;
        cout << "Quantité de carburant : " << quantite << " gallon(s)" << endl;
        cout << "Prix à la pompe : " << prixPompe << " $/gallon" << ANSI_COLOR_RESET << endl; nHistorique++;
    }

    sqlite3_finalize(stmt);
}
#pragma endregion


#pragma region Fonction pour Fournisseur
void PasserCommandeApprovisionnement(int idFournisseur, sqlite3* db) {
    // Demander les détails de la commande à l'utilisateur
    string type, dateLivraison, devise = "gde(s)", unite = "gallon(s)"; double quantite, prixUnitaire;
    string modificationType = "Commande approvisionnement";

    string date, time;
    DateTime(date, time); // fonction qui donne la date et l' heure du jour

    while (true) {
        system("cls");
        cout << ANSI_COLOR_GREEN << "Passer une commande d'approvisionnement" << endl;
        cout << "---------------------------------------" << ANSI_COLOR_RESET << endl;
        cout << "Type (gazoline, kerozene, diesel, propane): ";
        cin >> type;
        cout << "Quantité de (" << unite << "): ";
        cin >> quantite;
        cout << "Prix à la pompe (" << devise << "/" << unite << ") : ";
        cin >> prixUnitaire;
        cout << "Date prévue de livraison (yyyy-mm-dd) : ";
        cin >> dateLivraison;

        system("cls"); // Efface l'écran pour une meilleure lisibilité

        cout << ANSI_COLOR_DARK_GRAY << "Récapitulatif de la commande d'approvisionnement :" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "Date de la commande : " <<date << " " << time << endl;
        cout << "Date de livraison prévue : " << dateLivraison << endl;
        cout << "Type : " << type << endl;
        cout << "Quantité de " << type << ": " << quantite << " gallon(s)" << endl;
        cout << "Prix unitaire : " << prixUnitaire << " $/gallon" << endl;
        cout << "-------------------------------------------" << ANSI_COLOR_RESET << endl;

        // Demander à l'utilisateur de confirmer ou revenir en arrière
        char choix;
        cout << "Confirmer la commande (C) ou revenir en arrière (R) ? ";
        cin >> choix;

        if (choix == 'C' || choix == 'c') {
            // Effectuer l'enregistrement de la commande dans la base de données
            const char* insertCommandeQuery = "INSERT INTO Flux (date, idEmployee, type, quantite, prixunitaire, referenceExterne, typeFlux, devise)"
                                              "VALUES (?, ?, ?, ?, ?, 0, 'approvisionnement', 'gourdes')";

            sqlite3_stmt* insertCommandeStmt;
            int rc = sqlite3_prepare_v2(db, insertCommandeQuery, -1, &insertCommandeStmt, nullptr);
            if (rc == SQLITE_OK) {
                sqlite3_bind_text(insertCommandeStmt, 1, dateLivraison.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(insertCommandeStmt, 2, idFournisseur);
                sqlite3_bind_text(insertCommandeStmt, 3, type.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_double(insertCommandeStmt, 4, quantite);
                sqlite3_bind_double(insertCommandeStmt, 5, prixUnitaire);

                rc = sqlite3_step(insertCommandeStmt);
                if (rc != SQLITE_DONE) {
                    cerr << "Erreur lors de l'insertion de données : " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_finalize(insertCommandeStmt);
            }
            else {
                cerr << "Erreur lors de la préparation de la requête : " << sqlite3_errmsg(db) << endl;
            }

            cout << ANSI_COLOR_GREEN << "Commande d'approvisionnement passée avec succès !" << ANSI_COLOR_RESET << endl;
            // Enregistrez les informations dans la structure
            ModificationHistory historyEntry;
            historyEntry.idPersonne = idFournisseur;
            historyEntry.modificationType = modificationType;
            historyEntry.date = date + " " + time;

            // Ajoutez l'entrée d'historique à la liste
            historyList.push_back(historyEntry);

            break; // Sortir de la boucle
        }
        else if (choix == 'R' || choix == 'r') {
            // Revenir en arrière pour refaire la saisie
            system("cls"); // Efface l'écran pour une meilleure lisibilité
            continue; // Passer à l'itération suivante de la boucle
        }
        else {
            cout << "Choix invalide. Commande annulée" << endl;
            system("pause"); // faire une pause avnat de redonner la possibilte de placer commande
            continue; // Passer à l'itération suivante de la boucle
        }

    }
}

void AfficherHistoriqueApprovisionnements(int idFournisseur, sqlite3* db) {

    int nHistorique = 1;

    // Préparez la requête SQL pour récupérer l'historique des approvisionnements
    string query = "SELECT date, type, quantite, prixunitaire FROM Flux WHERE idEmployee = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Erreur lors de la préparation de la requête : " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, idFournisseur);

    system("cls"); // Efface l'écran pour une meilleure lisibilité
    cout << ANSI_COLOR_DARK_GRAY << "Historique des approvisionnements pour le fournisseur (idFournisseur = " << idFournisseur << ")" << endl;

    // Parcourez les résultats de la requête et affichez-les
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        //string dateLivraison = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double quantite = sqlite3_column_double(stmt, 2);
        double prixUnitaire = sqlite3_column_double(stmt, 3);

        cout << ANSI_COLOR_DARK_GRAY << "--------------------" << nHistorique << "--------------------" << endl;
        cout << "Date : " << date << endl;   
        //cout << "Date de livraison prévue : " << dateLivraison << endl;
        cout << "Type : " << type << endl;
        cout << "Quantité : " << quantite << " gallon(s)" << endl;
        cout << "Prix unitaire : " << prixUnitaire << " gourdes" << ANSI_COLOR_RESET << endl; nHistorique++;
    }

    sqlite3_finalize(stmt);
}
#pragma endregion


#pragma region Fonction pour manager

void AfficherHistoriqueModification(vector<ModificationHistory> historyList) {
    // Parcourez la liste d'historique pour afficher les entrées
    cout << ANSI_COLOR_BROWN << "Historique des modifications :" << ANSI_COLOR_RESET << endl;
    for (const ModificationHistory& entry : historyList) {
        cout << "Utilisateur : " << entry.idPersonne << endl;
        cout << "Type de modification : " << entry.modificationType << endl;
        cout << "Date : " << entry.date << endl;
        cout << ANSI_COLOR_BROWN << "-----------------------------------" << ANSI_COLOR_RESET << endl;
    }
}


#pragma region Fonction pour Station
void ajouterStation(sqlite3* db, int idManager) {
    system("cls");
    int rc;
    string modificationType = "Ajout station";

    string date, time;
    DateTime(date, time); // fonction qui donne la date et l' heure du jour

    cout << "Ajout d'une nouvelle station" << endl;

    string nomStation;
    cout << "Nom de la station : ";
    cin.ignore(); // Ignorer le saut de ligne précédent
    getline(cin, nomStation);

    string adresseStation;
    cout << "Adresse de la station : ";
    getline(cin, adresseStation);

    int capaciteMax, capaciteMin, niveauActuel;
    cout << "Capacité maximale de la citerne : ";
    cin >> capaciteMax;

    cout << "Capacité minimale de la citerne : ";
    cin >> capaciteMin;

    cout << "Niveau actuel de la citerne : ";
    cin >> niveauActuel;

    sqlite3_stmt* stmt;
    const char* query = "PRAGMA table_info(station);"; // Requête pour obtenir des informations sur les colonnes de la table "station"
    int result = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    int numColumns = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        numColumns++;
    }
    string idStation = "station" + to_string(numColumns);

    // Insérer la nouvelle station avec les informations
    const char* insertStationQuery = "INSERT INTO Station (idStation, nomStation, adresseStation) VALUES (?, ?, ?);";
    sqlite3_stmt* insertStationStmt;
    rc = sqlite3_prepare_v2(db, insertStationQuery, -1, &insertStationStmt, nullptr);
    sqlite3_bind_text(insertStationStmt, 1, idStation.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertStationStmt, 2, nomStation.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertStationStmt, 3, adresseStation.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(insertStationStmt);

    if (rc != SQLITE_DONE) {
        cout << "Erreur lors de l'ajout de la station dans la table Station : " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(insertStationStmt);
        return;
    }

    sqlite3_finalize(insertStationStmt);

    // Insérer une nouvelle citerne avec les informations
    const char* insertCiterneQuery = "INSERT INTO Citerne (nomCiterne, idStation, type, capaciteMax, capaciteMin, niveauActuel) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* insertCiterneStmt;
    rc = sqlite3_prepare_v2(db, insertCiterneQuery, -1, &insertCiterneStmt, nullptr);
    sqlite3_bind_text(insertCiterneStmt, 1, "NomCiterneParDefaut", -1, SQLITE_STATIC); // Modifier avec le nom approprié
    sqlite3_bind_text(insertCiterneStmt, 2, idStation.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertCiterneStmt, 3, "TypeParDefaut", -1, SQLITE_STATIC); // Modifier avec le type approprié
    sqlite3_bind_int(insertCiterneStmt, 4, capaciteMax); // Utiliser la capacité maximale saisie
    sqlite3_bind_int(insertCiterneStmt, 5, capaciteMin); // Utiliser la capacité minimale saisie
    sqlite3_bind_int(insertCiterneStmt, 6, niveauActuel); // Utiliser le niveau actuel saisi

    rc = sqlite3_step(insertCiterneStmt);

    if (rc != SQLITE_DONE) {
        cout << "Erreur lors de l'ajout de la citerne dans la table Citerne : " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(insertCiterneStmt);

    cout << "Nouvelle station ajoutée avec succès !" << endl;
    // Enregistrez les informations dans la structure
    ModificationHistory historyEntry;
    historyEntry.idPersonne = idManager;
    historyEntry.modificationType = modificationType;
    historyEntry.date = date + " " + time;

    // Ajoutez l'entrée d'historique à la liste
    historyList.push_back(historyEntry);

}


void modifierStation(sqlite3* db, string& nomStationChoisie,int idManager) {
    system("cls");
    int rc;
    string date, time;
    DateTime(date, time); // fonction qui donne la date et l' heure du jour

    const char* query1 = "SELECT s.nomStation, s.adresseStation, s.idStation, SUM(c.capaciteMax), COUNT(c.nomCiterne) FROM Station s LEFT JOIN Citerne c ON s.idStation = c.idStation WHERE s.nomStation = ? GROUP BY s.idStation;";
    const char* query2 = "SELECT c.nomCiterne, c.niveauActuel FROM Station s LEFT JOIN Citerne c ON s.idStation = c.idStation WHERE c.idCiterne IS NOT NULL AND s.nomStation = ?;";

    sqlite3_stmt* stmt1;
    rc = sqlite3_prepare_v2(db, query1, -1, &stmt1, nullptr);
    sqlite3_bind_text(stmt1, 1, nomStationChoisie.c_str(), -1, SQLITE_STATIC);

    sqlite3_stmt* stmt2;
    rc = sqlite3_prepare_v2(db, query2, -1, &stmt2, nullptr);
    sqlite3_bind_text(stmt2, 1, nomStationChoisie.c_str(), -1, SQLITE_STATIC);


    // Exécutez les requêtes et traitez les résultats
    if (sqlite3_step(stmt1) == SQLITE_ROW) {
        const char* nomStation = reinterpret_cast<const char*>(sqlite3_column_text(stmt1, 0));
        const char* adresseStation = reinterpret_cast<const char*>(sqlite3_column_text(stmt1, 1));
        int idStation = sqlite3_column_int(stmt1, 2);
        int capaciteMax = sqlite3_column_int(stmt1, 3);
        int nombreCiternes = sqlite3_column_int(stmt1, 4);

        cout << ANSI_COLOR_DARK_GRAY << "Nom de la station : " << nomStation << endl;
        cout << "Adresse de la station : " << adresseStation << endl;
        cout << "ID de la station : " << idStation << endl;
        cout << "Capacité maximale : " << capaciteMax << " gallons" << endl;
        cout << "Nombre de citernes : " << nombreCiternes << endl;

        while (sqlite3_step(stmt2) == SQLITE_ROW) {
            cout << "====================================" << endl;
            const char* nomCiterne = reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 0));
            const char* niveauActuel = reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 1));
            cout << nomCiterne << "-> Niveau actuel : " << niveauActuel << endl;
        }
        cout << "******************************************" << ANSI_COLOR_RESET << endl;

        string nom, adresse, capaciteCiterne;
        // Demander les nouvelles informations au gestionnaire
        cout << ANSI_COLOR_GREEN << "\NEntrez les nouvelles informations du fournisseur : " << ANSI_COLOR_RESET << endl;
        cout << "Nouveau nom  : "; cin >> nom;
        cout << "Nouveau adresse : "; cin >> adresse;

        const char* updateQuery = "UPDATE Station SET nomStation = ? adresseStation = ? WHERE idStation = ?";
        sqlite3_stmt* updateStmt;
        rc = sqlite3_prepare_v2(db, updateQuery, -1, &updateStmt, nullptr);
        sqlite3_bind_text(updateStmt, 1, nom.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(updateStmt, 2, adresse.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(updateStmt, 3, idStation);

        if (sqlite3_step(updateStmt) == SQLITE_DONE) {
            cout << "Station mis à jour avec succès." << endl;
            // Enregistrez les informations dans la structure
            ModificationHistory historyEntry;
            historyEntry.idPersonne = idManager;
            historyEntry.modificationType = "Modification station";
            historyEntry.date = date + " " + time;

            // Ajoutez l'entrée d'historique à la liste
            historyList.push_back(historyEntry);
        }
        else {
            cout << "Erreur lors de la mise à jour de la station." << endl;
        }

        sqlite3_finalize(updateStmt);
    }
    else {
        cout << "Aucune station correspondante trouvée." << endl;
    }

    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
}
#pragma endregion


#pragma region Fonction pour Fournisseur
void ajouterFournisseur(sqlite3* db, int idPersonne, int idManager) {
    system("cls");

    string nom, prenom, numeroSocial, sexe, adresse, DDN;
    string date, time; 
    DateTime(date, time); // fonction qui donne la date et l' heure du jour 

    cout << "Entrez le nom du fournisseur : ";
    cin >> nom;

    cout << "Entrez le prénom du fournisseur : ";
    cin >> prenom;

    cout << "Entrez le numéro social du fournisseur : ";
    cin >> numeroSocial;

    cout << "Entrez le sexe du fournisseur : ";
    cin >> sexe;

    cout << "Entrez l'adresse du fournisseur : ";
    cin >> adresse;

    cin.ignore();
    cout << "Entrez la date de naissance (DDN) du fournisseur : ";
    cin >> DDN;

    const char* insertQuery = "INSERT INTO Personne (idPersonne, nom, prenom, numeroSocial, sexe, adresse, DDN, fonction) VALUES (?, ?, ?, ?, ?, ?, ?, 'fournisseur');";
    sqlite3_stmt* insertStmt;
    int rc = sqlite3_prepare_v2(db, insertQuery, -1, &insertStmt, nullptr);

    if (rc == SQLITE_OK) {
        sqlite3_bind_int(insertStmt, 1, idPersonne);
        sqlite3_bind_text(insertStmt, 2, nom.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(insertStmt, 3, prenom.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(insertStmt, 4, numeroSocial.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(insertStmt, 5, sexe.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(insertStmt, 6, adresse.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(insertStmt, 7, DDN.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(insertStmt);

        if (rc == SQLITE_DONE) {
            cout << ANSI_COLOR_GREEN << "\nFournisseur ajouté avec succès." << ANSI_COLOR_RESET << endl;
            // Enregistrez les informations dans la structure
            ModificationHistory historyEntry;
            historyEntry.idPersonne = idManager;
            historyEntry.modificationType = "Ajout fournisseur";
            historyEntry.date = date + " " + time;

            // Ajoutez l'entrée d'historique à la liste
            historyList.push_back(historyEntry);
        }
        else {
            cout << ANSI_COLOR_RED << "\nErreur lors de l'ajout du fournisseur : " << sqlite3_errmsg(db) << ANSI_COLOR_RESET << endl;
        }
    }
    else {
        cout << ANSI_COLOR_RED << "\nErreur lors de la préparation de la requête : " << sqlite3_errmsg(db) << ANSI_COLOR_RESET << endl;
    }

    sqlite3_finalize(insertStmt);
}


void modifierFournisseurAvecLogin(sqlite3* db, string& nomFournisseurChoisie,int idManager) {
    system("cls");
    int rc;
    string date, time;
    DateTime(date, time); // fonction qui donne la date et l' heure du jour 

    const char* selectQuery = "SELECT * FROM Personne WHERE nom = ? AND fonction = 'fournisseur';";
    sqlite3_stmt* selectStmt;
    rc = sqlite3_prepare_v2(db, selectQuery, -1, &selectStmt, nullptr);
    sqlite3_bind_text(selectStmt, 1, nomFournisseurChoisie.c_str(), -1, SQLITE_STATIC);

    // Exécutez la requête SELECT et traitez les résultats
    if (sqlite3_step(selectStmt) == SQLITE_ROW) {
        int idPersonne = sqlite3_column_int(selectStmt, 0);
        string ancienNom = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 1));
        string ancienPrenom = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 2));
        string ancienNumeroSocial = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 3));
        string ancienSexe = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 4));
        string ancienAdresse = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 5));
        string ancienDDN = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 6));

        // Afficher les informations actuelles du fournisseur
        cout << ANSI_COLOR_GREEN << "Informations actuelles du fournisseur :" << ANSI_COLOR_RESET<< endl;
        cout << "Nom : " << ancienNom << endl;
        cout << "Prénom : " << ancienPrenom << endl;
        cout << "Numéro social : " << ancienNumeroSocial << endl;
        cout << "Sexe : " << ancienSexe << endl;
        cout << "Adresse : " << ancienAdresse << endl;
        cout << "Date de naissance : " << ancienDDN << endl;

        string nom, prenom, numeroSocial, sexe, adresse, DDN;
        // Demander les nouvelles informations au gestionnaire
        cout << ANSI_COLOR_GREEN << "\NEntrez les nouvelles informations du fournisseur : " << ANSI_COLOR_RESET << endl;
        cout << "Nouveau nom : ";
        cin >> nom;
        cout << "Nouveau prénom : ";
        cin >> prenom;
        cout << "Nouveau numéro social : ";
        cin >> numeroSocial;
        cout << "Nouveau sexe : ";
        cin >> sexe;
        cout << "Nouvelle adresse : ";
        cin >> adresse;
        cout << "Nouvelle date de naissance (DDN) : ";
        cin >> DDN;

        // Mettre à jour les informations du fournisseur dans la base de données
        // Préparer la requête UPDATE pour mettre à jour les informations du fournisseur
        const char* updateQuery = "UPDATE Personne SET nom = ?, prenom = ?, numeroSocial = ?, sexe = ?, adresse = ?, DDN = ? WHERE idPersonne = ?";
        sqlite3_stmt* updateStmt;
        rc = sqlite3_prepare_v2(db, updateQuery, -1, &updateStmt, nullptr);
        sqlite3_bind_text(updateStmt, 1, nom.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(updateStmt, 2, prenom.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(updateStmt, 3, numeroSocial.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(updateStmt, 4, sexe.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(updateStmt, 5, adresse.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(updateStmt, 6, DDN.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(updateStmt, 7, idPersonne);

        // Exécuter la requête UPDATE et afficher le résultat
        rc = sqlite3_step(updateStmt);

        if (rc == SQLITE_DONE) {
            cout << ANSI_COLOR_GREEN << "Informations du fournisseur mises à jour avec succès." << ANSI_COLOR_RESET << endl;
            // Enregistrez les informations dans la structure
            ModificationHistory historyEntry;
            historyEntry.idPersonne = idManager;
            historyEntry.modificationType = "Modification information fournisseur";
            historyEntry.date = date + " " + time;

            // Ajoutez l'entrée d'historique à la liste
            historyList.push_back(historyEntry);


            // Mettre à jour le champ username dans la table Login si le prénom a été modifié
            if (ancienPrenom != prenom) {
                const char* updateUsernameQuery = "UPDATE Login SET username = ? WHERE username = ?";
                sqlite3_stmt* updateUsernameStmt;
                rc = sqlite3_prepare_v2(db, updateUsernameQuery, -1, &updateUsernameStmt, nullptr);
                sqlite3_bind_text(updateUsernameStmt, 1, prenom.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(updateUsernameStmt, 2, ancienPrenom.c_str(), -1, SQLITE_STATIC);

                rc = sqlite3_step(updateUsernameStmt);

                if (rc == SQLITE_DONE) {
                    cout << ANSI_COLOR_GREEN << "Username mis à jour avec succès dans la table Login." << endl;
                }
                else {
                    cout << ANSI_COLOR_RED << "Erreur lors de la mise à jour de l'username dans la table Login : " << sqlite3_errmsg(db) << ANSI_COLOR_RESET << endl;
                }

                sqlite3_finalize(updateUsernameStmt);
            }
        }
        else {
            cout << ANSI_COLOR_RED << "Erreur lors de la mise à jour des informations du fournisseur : " << sqlite3_errmsg(db) << ANSI_COLOR_RESET << endl;
        }

        sqlite3_finalize(updateStmt);
    }
}
#pragma endregion


#pragma region Fonction pour employee
void ajouterEmployeeStation() {

}

void modifierEmployee() {

}
#pragma endregion


#pragma endregion


// Pour créer notre base de données, les tables ainsi que les champs
void CreateDataBaseAndTable(sqlite3* db, int rc) {

    const char* createStationTableQuery = "CREATE TABLE Station ("
        "idStation TEXT NOT NULL PRIMARY KEY,"
        "nomStation TEXT NOT NULL,"
        "adresseStation TEXT NOT NULL"
        ");";

    const char* createPersonneTableQuery = "CREATE TABLE Personne ("
        "idPersonne INTEGER NOT NULL PRIMARY KEY,"
        "nom TEXT NOT NULL,"
        "prenom TEXT NOT NULL,"
        "numeroSocial INTEGER NOT NULL UNIQUE,"
        "sexe TEXT NOT NULL,"
        "adresse TEXT NOT NULL,"
        "DDN TEXT NOT NULL,"
        "fonction TEXT NOT NULL"
        ");";

    const char* createCamionTableQuery = "CREATE TABLE Camion ("
        "plaque TEXT NOT NULL UNIQUE,"
        "type TEXT NOT NULL,"
        "capaciteMax INTEGER NOT NULL,"
        "niveauActuel INTEGER NOT NULL"
        ");";

    const char* createLoginTableQuery = "CREATE TABLE Login ("
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL UNIQUE,"
        "idPersonne INTEGER NOT NULL UNIQUE,"
        "categorie TEXT NOT NULL"
        ");";

    const char* createConduitTableQuery = "CREATE TABLE Conduit ("
        "idChauffeur INTEGER NOT NULL PRIMARY KEY,"
        "plaque TEXT NOT NULL UNIQUE,"
        "idFlux INTEGER NOT NULL"
        ");";

    const char* createUniteVolumeQuery = "CREATE TABLE uniteVolume ("
        "type TEXT NOT NULL,"
        "unite TEXT NOT NULL,"
        "reference TEXT NOT NULL"
        ");";

    const char* createFluxTableQuery = "CREATE TABLE Flux ("
        "idFlux INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "date TEXT NOT NULL,"
        "idEmployee INTEGER NOT NULL,"
        "type TEXT NOT NULL,"
        "quantite NUMERIC NOT NULL,"
        "prixunitaire NUMERIC NOT NULL,"
        "referenceExterne TEXT NOT NULL,"
        "typeFlux TEXT NOT NULL,"
        "devise TEXT NOT NULL"
        ");";

    const char* createCiterneTableQuery = "CREATE TABLE Citerne ("
        "idCiterne INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "nomCiterne TEXT NOT NULL,"
        "idStation TEXT NOT NULL,"
        "type TEXT NOT NULL,"
        "capaciteMax NUMERIC NOT NULL,"
        "capaciteMin NUMERIC NOT NULL,"
        "niveauActuel NUMERIC NOT NULL"
        ");";

    rc = sqlite3_exec(db, createStationTableQuery, nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, createCamionTableQuery, nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, createCiterneTableQuery, nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, createConduitTableQuery, nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, createFluxTableQuery, nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, createLoginTableQuery, nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, createPersonneTableQuery, nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, createUniteVolumeQuery, nullptr, nullptr, nullptr);

    //sqlite3_close(db);
}

// Fonction pour saisir le login afin de le vérifier et l' afficher
bool getLoginCredentials(sqlite3* db, int& rc, string& username, string& password, string& fonction, int& idPersonne, int& tentative, int maxTentatives) {
    system("cls"); // Efface l'écran pour une meilleure lisibilité
    password = ""; // 

    cout << ANSI_COLOR_GREEN << "Bienvenue dans l'application de gestion de station-service !" << ANSI_COLOR_RESET << endl;

    cout << "Tentative " << tentative << "/" << maxTentatives << endl;
    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
        
    // pour éviter que quiconque voit notre mot de passe, on le remplace par (*) sur l' écran
    char c;
    while ((c = _getch()) != '\r') {
        if (c == '\b') {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            cout << '*';
            password.push_back(c);
        }
    }
    cout << endl;

    // ... Insérons ici le code de vérification du login ...

    const char* query = "SELECT categorie, idPersonne FROM Login WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        system("cls");// Efface l'écran pour une meilleure lisibilité
        cout << ANSI_COLOR_RED << "Erreur lors de la préparation de la requête : " << sqlite3_errmsg(db) << ANSI_COLOR_RESET << endl;
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        system("cls");// Efface l'écran pour une meilleure lisibilité
        fonction = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        idPersonne = sqlite3_column_int(stmt, 1);

        return true; // Login réussi
    }
    else if (rc == SQLITE_DONE) {
        return false;
    }

    sqlite3_finalize(stmt);
}

int main() {
    // Changer l'encodage de la console en UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc = sqlite3_open("db/pomp.db", &db);

    // fonction qui crée le db et les tables
    CreateDataBaseAndTable(db, rc);
 
    string username, password, fonction, nomStationChoisie, nomFournisseurChoisie;  int idPersonne = 0, tentative = 1, previousColumns = 0; const int maxTentatives = 3;
    vector<string> stationNames, fournisseurNames; const char* idStation = {};
    vector<ModificationHistory> historyList2;

    while (true) {
        // Appel de la fonction pour saisir le login
        bool authenticated = getLoginCredentials(db, rc, username, password, fonction, idPersonne, tentative, maxTentatives);

        if (authenticated) {
            if (fonction == "manager") {
                cout << ANSI_COLOR_GREEN << "Bienvenue @" << username << " { " << fonction << " }" << ANSI_COLOR_RESET << endl;
                
                // déclaration de chaîne de caractères qui contient nos requêtes SQLite
                const char* query = "SELECT idStation, nomStation, adresseStation FROM Station";
                const char* queryFournisseur = "SELECT * FROM Personne WHERE fonction = \"fournisseur\"; ";

                while (true) {
                    cout << ANSI_COLOR_GREEN << "Options disponibles:" << ANSI_COLOR_RESET << endl;
                    cout << ANSI_COLOR_BROWN << "1. Gérer les station\n";
                    cout << "2. Gérer les fournisseurs\n";
                    cout << "3. Gérer les employés\n";
                    cout << "4. Afficher l' historique des modifications\n";
                    cout << "5. Quitter" << ANSI_COLOR_RESET << endl;

                    // Attendre la sélection de l'option par le manager
                    int choix;
                    cout << "\nChoix option:"; cin >> choix;
                    switch (choix) {
                    case 1:
                        system("cls"); // Efface l'écran pour une meilleure lisibilité

                        // Exécution de la requête query1 pour afficher le nom de chaque station
                        cout << ANSI_COLOR_BROWN << "Nom des stations" << ANSI_COLOR_RESET << endl;
                        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

                        while (sqlite3_step(stmt) == SQLITE_ROW) {
                            idStation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                            const char* nomStation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                            stationNames.push_back(nomStation);
                            cout << "* " << nomStation << "\t";
                        }

                        cout << ANSI_COLOR_GREEN << "\nOptions pour gérer les stations :" << ANSI_COLOR_RESET << endl;
                        cout << ANSI_COLOR_BROWN << "1. Ajouter une station\t";
                        cout << "2. Modifier une station" << ANSI_COLOR_RESET << endl;

                        int choixStations;
                        cout << "\nChoix option: "; cin >> choixStations;

                        switch (choixStations) {
                        case 1: // ajouter station
                            ajouterStation(db, idPersonne);
                            break;
                        case 2: // modifier station
                            int mChoix;
                            cout << ANSI_COLOR_DARK_GRAY << "Quelle station voulez-vous modifier ? #" << ANSI_COLOR_RESET;  cin >> mChoix;
                            if (mChoix >= 1 && mChoix <= stationNames.size()) {
                                nomStationChoisie = stationNames[mChoix - 1]; // Récupérer le nom de la station choisie
                                modifierStation( db,nomStationChoisie, idPersonne); // Passer le nom de la station à la fonction modifier Station
                            }
                            else {
                                cout << ANSI_COLOR_RED << "Option invalide." << ANSI_COLOR_RESET << endl;
                            }
                            break;
                        default:
                            cout << ANSI_COLOR_RED << "Option invalide." << ANSI_COLOR_RESET << endl;
                            break;
                        }
                        system("pause"); // Attendre que l'utilisateur appuie sur une touche pour continuer
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        break;

                    case 2:
                        system("cls"); // Efface l'écran pour une meilleure lisibilité
                        // Exécution de la requête query1 pour afficher le nom de chaque fournisseur
                        cout << ANSI_COLOR_BROWN << "Nom des fournisseurs" << ANSI_COLOR_RESET << endl;
                        rc = sqlite3_prepare_v2(db, queryFournisseur, -1, &stmt, nullptr);

                        while (sqlite3_step(stmt) == SQLITE_ROW) {
                            previousColumns = sqlite3_column_int(stmt, 0); // Récupération de l'id
                            const char* nomFournisseur = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                            const char* prenomFournisseur = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

                            fournisseurNames.push_back(nomFournisseur);
                            cout << "* " << nomFournisseur <<  ", " << prenomFournisseur << "\t";
                        }

                        cout << ANSI_COLOR_GREEN << "\nOptions pour gérer les fournisseurs :" << ANSI_COLOR_RESET << endl;
                        cout << ANSI_COLOR_BROWN << "1. Ajouter un fournisseur\t";
                        cout << "2. Modifier un fournisseur" << ANSI_COLOR_RESET << endl;

                        cout << "\nChoix option: ";
                        int choixFournisseur, currentID; cin >> choixFournisseur;

                        switch (choixFournisseur) {
                        case 1:
                            currentID = previousColumns + 1;
                            ajouterFournisseur(db, currentID, idPersonne);
                            break;
                        case 2:
                            int mChoix;
                            cout << ANSI_COLOR_DARK_GRAY << "Quelle Fournisseur voulez-vous modifier ? #" << ANSI_COLOR_RESET;  cin >> mChoix;
                            if (mChoix >= 1 && mChoix <= fournisseurNames.size()) {
                                nomFournisseurChoisie = fournisseurNames[mChoix - 1]; // Récupérer le nom du forunissseur choisie
                                modifierFournisseurAvecLogin(db, nomFournisseurChoisie, idPersonne); // Passer le nom du fournisseur à la fonction modifier le fournisseur
                            }
                            else {
                                cout << ANSI_COLOR_RED << "Option invalide." << ANSI_COLOR_RESET << endl;
                            }
                            break;
                        default:
                            cout << ANSI_COLOR_RED << "Option invalide.\n" << ANSI_COLOR_RESET;
                            break;
                        }

                        system("pause"); // Attendre que l'utilisateur appuie sur une touche pour continuer
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        break;

                    case 3:
                        system("cls"); // Efface l'écran pour une meilleure lisibilité
                        cout << ANSI_COLOR_GREEN << "Options pour gérer les employés :" << ANSI_COLOR_RESET << endl;
                        cout << ANSI_COLOR_BROWN << "1. Affecter un employé à une station \t";
                        cout << "2. Modifier information sur un employé" << ANSI_COLOR_RESET << endl;

                        cout << "\nChoix option: ";
                        int choixEmployee; cin >> choixEmployee;


                        // aucune table disponible pour l' instant
                        switch (choixEmployee) {
                        case 1:
                            ajouterEmployeeStation();
                            break;
                        case 2:
                            modifierEmployee();
                            break;
                        default:
                            cout << ANSI_COLOR_RED << "Option invalide.\n" << ANSI_COLOR_RESET;
                            break;
                        }

                        system("pause"); // Attendre que l'utilisateur appuie sur une touche pour continuer
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        break;

                    case 4:
                        system("cls"); // Efface l'écran pour une meilleure lisibilité
                        // fonction pour afficher l'historique des modifications
                        AfficherHistoriqueModification(historyList2);
                        break;

                    case 5:
                        cout << ANSI_COLOR_GREEN << "Déconnexion en cours..." << ANSI_COLOR_RESET << endl;
                        return 0;

                    default:
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        cout << ANSI_COLOR_RED << "Option invalide.\n" << ANSI_COLOR_RESET;
                        break;
                    }

                }

            }

            else if (fonction == "pompiste") {

                while (true) {
                    cout << ANSI_COLOR_GREEN << "Bienvenue @" << username << " fonction { " << fonction << " }" << ANSI_COLOR_RESET << endl;
                    cout << ANSI_COLOR_BROWN << "Options disponibles :"  << endl;
                    cout << "1. Ajouter une vente\n";
                    cout << "2. Afficher l'historique des ventes\n";
                    cout << "3. Quitter" << ANSI_COLOR_RESET << endl ;

                    // Attendre la sélection de l'option par le pompiste
                    int choix;
                    cout << "Choix:";cin >> choix;

                    switch (choix) {
                    case 1:
                        // Appeler la fonction pour ajouter une vente

                        SaisirDetailsVente(idPersonne, db);
                        system("pause"); // Attendre que l'utilisateur appuie sur une touche pour continuer
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        break;

                    case 2:
                        // Appeler la fonction pour afficher l'historique des ventes

                        AfficherHistoriqueVente(idPersonne, db);
                        system("pause"); // Attendre que l'utilisateur appuie sur une touche pour continuer
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        break;

                    case 3:
                        cout << ANSI_COLOR_GREEN << "Déconnexion en cours..." << ANSI_COLOR_RESET << endl;
                        return 0;

                    default:
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        cout << ANSI_COLOR_RED << "Option invalide.\n" << ANSI_COLOR_RESET;
                        break;
                    }
                }

            }

            else if (fonction == "fournisseur") {

                while (true) {
                    cout << ANSI_COLOR_GREEN << "Bienvenue @" << username << " fonction { " << fonction << " }" << ANSI_COLOR_RESET << endl;
                    cout << ANSI_COLOR_BROWN << "Options disponibles :" << endl;
                    cout << "1. Passer une commande d'approvisionnement\n";
                    cout << "2. Afficher l'historique des approvisionnements\n";
                    cout << "3. Quitter" << ANSI_COLOR_RESET << endl;

                    // Attendre la sélection de l'option par le pompiste
                    int choix;
                    cout << "Choix:"; cin >> choix;

                    switch (choix) {
                    case 1:
                        // Appeler la fonction pour ajouter une Commande

                        PasserCommandeApprovisionnement(idPersonne, db);
                        system("pause"); // Attendre que l'utilisateur appuie sur une touche pour continuer
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        break;

                    case 2:
                        // Appeler la fonction pour afficher l' historique de commande

                        AfficherHistoriqueApprovisionnements(idPersonne, db);
                        system("pause"); // Attendre que l'utilisateur appuie sur une touche pour continuer
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        break;

                    case 3:
                        cout << ANSI_COLOR_GREEN << "Déconnexion en cours..." << ANSI_COLOR_RESET << endl;
                        return 0;

                    default:
                        system("cls");  // Efface l'écran pour une meilleure lisibilité
                        cout << ANSI_COLOR_RED << "Option invalide.\n" << ANSI_COLOR_RESET;
                        break;
                    }
                }

            }

        }
        else {
            if (tentative == maxTentatives) {
                system("cls"); // Efface l'écran pour une meilleure lisibilité
                cout << ANSI_COLOR_RED << "Tentatives de login épuisées. Veuillez réessayer plus tard." << ANSI_COLOR_RESET << endl;
                break;
            }
            cout << ANSI_COLOR_RED << "Aucun utilisateur correspondant trouvé." << ANSI_COLOR_RESET << endl;
            system("pause"); // Attendre que l'utilisateur appuie sur une touche pour continuer
        }
        
        tentative++;
    }

    // Fermons la base de données après avoir terminé
    sqlite3_close(db);
    return 0;
}
