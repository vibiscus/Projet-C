#include <stdio.h>
#include <string.h>

/*Constantes*/

#define MAX_CAT_SERV 20    /* Constante qui limite le catalogue des services -- ajouter un test concernant cette limite - Utilisé pour catalogue_services */
#define MAX_NOM_SERVICE 21 /* Taille maximale de la chaîne de caractères qui décrit les services complémentaires - Utilisé pour nom_service */
#define MAX_NB_CHAMBRES 50 /*Utilisée dans tab_chambres[]*/
#define NON_TROUVE -1 /*Pour tester si un résultat a été trouvé à la recherche*/
#define ANNEE 365 /* Utilisée pour dimensionner le planning et le calendrier*/
#define NB_CHAMBRES_PRIX 6 /* nb de chambres différentes dans les prix*/

/*Fichiers*/

#define CALENDRIER "1-Parametres/calendrier.txt" /*Nom du fichier contentant le calendrier (dates et saison)*/
#define CAT_SERVICES "1-Parametres/catalogue-services.txt" /* Nom du fichier qui contient le catalogue des services*/
#define CHAMBRES  "1-Parametres/chambres.txt" /*Nom du fichier contentant les chambres*/
#define PLANNING "1-Parametres/planning.txt" /*Nom du fichier contenant le planning*/
#define PRIX_NUIT "1-Parametres/prix_nuits.txt" /*Nom du fichier contenant le prix d'une nuitée*/
#define RESA_CODE "1-Parametres/resa-code.txt" /*Nom du fichier qui contient le numéro de la dernière réservation faite. À charger au démarrage via long unsigned int dernier_code_resa() qui affecte nb_resa.
Le code 0 indique que la chambre est libre, le code 1 est réservé pour la déclaration de travaux.*/
