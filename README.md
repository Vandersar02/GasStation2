# Station Management System

## Introduction

Le système de gestion de station-service est une application en C++ utilisant SQLite pour la gestion des stations, des employés, des fournisseurs, et des ventes. Il permet aux utilisateurs de se connecter en tant que `manager`, `pompiste`, ou `fournisseur` pour effectuer diverses opérations selon leur rôle.

## Prérequis

- **SQLite3** : Assurez-vous que la bibliothèque SQLite3 est installée sur votre machine.
- **Compiler C++** : Un compilateur C++ compatible, comme g++, pour compiler le code source.
- **SQLite3.5** : https://github.com/GaloisInc/sqlite/tree/master/sqlite3.5

## Structure du Code

### 1. Fonctionnalités Principales

- **Gestion des Stations** : Ajouter et modifier des stations.
- **Gestion des Fournisseurs** : Ajouter et modifier des fournisseurs.
- **Gestion des Employés** : Affecter un employé à une station et modifier les informations des employés.
- **Historique des Modifications** : Afficher l'historique des modifications.
- **Ventes et Approvisionnements** : Ajouter des ventes et des commandes d'approvisionnement, et afficher leurs historiques.

### 2. Fichiers Source

#### `main.cpp`

Ce fichier contient la fonction principale (`main()`) qui gère le flux principal du programme :

- **Initialisation de la Base de Données** : `CreateDataBaseAndTable()` crée les tables nécessaires dans la base de données.
- **Authentification Utilisateur** : `getLoginCredentials()` demande les informations de connexion et vérifie les identifiants dans la base de données.
- **Menu Principal** : Affiche les options disponibles selon le rôle de l'utilisateur (`manager`, `pompiste`, `fournisseur`) et gère les choix de l'utilisateur.

#### Fonctionnalités de Gestion

- **Stations** : Ajouter (`ajouterStation()`) et modifier (`modifierStation()`) les stations.
- **Fournisseurs** : Ajouter (`ajouterFournisseur()`) et modifier (`modifierFournisseurAvecLogin()`) les fournisseurs.
- **Employés** : Affecter (`ajouterEmployeeStation()`) un employé à une station et modifier (`modifierEmployee()`) les informations des employés.

### 3. Structure de la Base de Données

Les tables suivantes sont créées dans la base de données `pomp.db` :

- **Station** : Informations sur les stations.
- **Personne** : Informations sur les personnes (employés et fournisseurs).
- **Camion** : Informations sur les camions.
- **Login** : Informations sur les utilisateurs et leurs rôles.
- **Conduit** : Informations sur les chauffeurs.
- **UniteVolume** : Unités de mesure utilisées.
- **Flux** : Historique des flux de ventes et d'approvisionnements.
- **Citerne** : Informations sur les citernes.

### 4. Compilation et Exécution

1. **Compilation**
   Compilez le code source en utilisant le compilateur C++. Exemple avec g++ :

   ```bash
   g++ -o station_management main.cpp -lsqlite3
   ```

2. **Exécution**
   Exécutez le programme compilé :

   ```bash
   ./station_management
   ```
