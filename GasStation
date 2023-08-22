#include <iostream>
#include "sqlite3.h"
#include <string.h>
#define ERRORINT -1000
#define ERRORDOUBLE -1000.0

using namespace std;


sqlite3* openDb(string dbName)
{
    sqlite3* dataBase = NULL;
    int resultat = sqlite3_open(dbName.c_str(), &dataBase);
    return dataBase;
}


int requete(sqlite3* dataBase, string query)
{
    sqlite3_stmt* statement;
    int resultat = sqlite3_prepare_v2(dataBase, query.c_str(), -1, &statement, NULL);
    sqlite3_step(statement);
    sqlite3_finalize(statement);
    if (resultat == SQLITE_OK)
    {

        return 1;
    }
    else
    {

        return ERRORINT;
    }
}

int getInt(sqlite3* dataBase, string query, int position)
{
    sqlite3_stmt* statement;
    int resultat = ERRORINT, requete = sqlite3_prepare_v2(dataBase, query.c_str(), -1, &statement, NULL);
    if (requete == SQLITE_OK)
    {
        if (sqlite3_step(statement) == SQLITE_ROW)
        {
            resultat = sqlite3_column_int(statement, position - 1);
            sqlite3_finalize(statement);
            return resultat;
        }
        else
        {
            return ERRORINT;
        }

    }
    else
    {
        return ERRORINT;
    }

}


string getText(sqlite3* dataBase, string query, int position)
{
    sqlite3_stmt* statement;
    string resultat;
    int requete = sqlite3_prepare_v2(dataBase, query.c_str(), -1, &statement, NULL);
    if (requete == SQLITE_OK)
    {
        if (sqlite3_step(statement) == SQLITE_ROW)
        {
            std::string resultat(reinterpret_cast<const char*>(sqlite3_column_text(statement, position - 1)));
            return resultat;
            //strcpy(resultat,sqlite3_column_text(statement,0));
            //sqlite3_finalize(statement);
            //return resultat;
        }
        else
        {
            return NULL;
        }

    }
    else
    {
        return NULL;
    }
}

double getDouble(sqlite3* dataBase, string query, int position)
{
    sqlite3_stmt* statement;
    int requete = sqlite3_prepare_v2(dataBase, query.c_str(), -1, &statement, NULL);
    double resultat = ERRORDOUBLE;
    if (requete == SQLITE_OK)
    {
        if (sqlite3_step(statement) == SQLITE_ROW)
        {
            resultat = sqlite3_column_double(statement, 0);
            sqlite3_finalize(statement);
            return resultat;
        }
        else
        {
            return ERRORDOUBLE;
        }

    }
    else
    {
        return ERRORDOUBLE;
    }

}


void config()
{
    sqlite3* db;
    db = openDb("db/pomp.db");
    string query = "create table personne(idpersonnes int, nomcomplet varchar(100))";

    requete(db, query);
    query = "create table station (idStation int,nomStation varchar(50),adresseStation varchar(50))";
    requete(db, query);
    query = "create table citerne(idCiterne int,nomCiterne varchar(50),idStation int,capaciteMax int,CapaciteMin int ,niveauactue int)";
    requete(db, query);
}



int main()
{
    //config();
}
