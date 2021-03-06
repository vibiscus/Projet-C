/*#########################################################################
#                                                                         #
#                                  hotel.c                                #
#                                                                         #
###########################################################################

* v 0.0   - 2016-01-31 squelette du main
* v 0.0.1 - 2016-02-02 ajout de l'option '6 - Modifier le catalogue des services.'
* v 0.0.2 - 2016-02-03 procédure mauvais_choix, procédure catalogue_services_menu
* v 0.0.2 - 2016-02-03 Saisie et enregistrement du catalogue des services
* v 0.1.0 - 2016-02-03 Sous-menu pour les services complet
* v 0.1.1 - 2016-02-04 On s'attaque aux chambres
* v 0.1.1 - 2016-02-04 Ebauches pour la réservation
* v 0.1.2 - 2016-02-05 Modification des chambres presque fonctionnelle
* v 0.1.3 - 2016-02-06 Modification et enregistrement fonctionnels. À optimiser
* v 0.1.4 - 2016-02-07 Chargement et mise à jour du calendrier
* v 0.1.5 - 2016-02-09 Affichage et saisie date jj/mm/aaaa
* v 0.1.6 - 2016-02-13 Début de création de réservation, cible_date marche
* v 0.1.7 - 2016-02-13 Début de création de réservation, cible_chambre marche
* v 0.1.8 - 2016-02-13 Correction de la mise à jour du calendrier dans la saisie des saisons
* v 0.1.9 - 2016-02-13 rech_periode fonctionnelle
* v 0.1.10 - 2016-02-14 tests sur cible_date, rech_periode problématique
* v 0.1.11 - 2016-02-14 cible_date OK, problèmes sur cible_chambre
* v 0.1.12 - 2016-02-15 cible_date OK avec tests. Problèmes sur cible_chambre
* v 0.1.13 - 2016-02-15 Réservation OK jusqu'au paiement
* v 0.1.14 - 2016-02-16 Séparation du code: utilitaires sortis dans fichier consacré. Makefile
* v 0.1.15 - 2016-02-16 fin paiement_cb
* v 1.0.0 - 2016-02-16 Réservation fonctionnelle
* v 1.0.1 - 2016-02-16 Création note
* v 2.1.5 - 2016-02-18 (almost) feature complete.

Reste à faire:
* Traiter des chaînes de caractères avec espaces.

#include <stdio.h>

main()
{
  char c          ;
  char phrase[50] ;
  int i           ;
  phrase[0] = '\0';
  i = 0 ;
  printf("Saisir une chaîne de caractères avec espaces : ") ;
  while((c = getchar())!='\n')
  {
    phrase[i] = c;
    i++ ;
  }
  printf("Voici la phrase : '%s'\n", phrase) ;

}

* Pour améliorer les préstations, transformer les boucles de recherche while en boucles for.
* optimiser enregistrement sur fichier et cycle de modification des chambres.
* optimiser affichage et saisie des descripteurs des chambres.
* évaluer la possibilité de déclarer les variables de choix (vue/sans vue; fumeur/non fumeur; haute saison/basse saison) comme des short int;
* Dans chaque sous-programme existent des test à ajouter, spécifiés dans l'en-tête du programme-même.

*/

#include "declarations.h" /*c'est ici que sont les #include et les #define*/

/*----------------------

Déclarations préliminaires

----------------------*/


/*----------------------

D'utilité générale

Programmes réunis dans utilitaires.c
----------------------*/

void mauvais_choix(int par_choisi)                            ; /* Mauvais choix à l'intérieur des menus */
long unsigned int jjmmaaaa_vers_aaaammjj(int j, int m, int a) ; /* Prendre une date saisie et la traduire en lonf int date */
char aaaammjj_vers_jjmmaaaa(long unsigned int date)           ; /* Retourne en chaine de caractères jj/mm/aaaa la date fournie en format aaaammjj */

/*----------------------

Calendrier
----------------------*/
void lecture_jours()       ; /* Prend les jours présents dans le fichier qui tient compte des jours (et saison) et les charge dans le tableau correspondant.*/
void test_date()           ; /* Vérifie si la date du jour d'aujourd'hui correspond au premier jour dans le tableau et dans le calendrier. Si la date a changé, déclanche la mise à jour dans les deux.*/
void maj_calendrier(int i) ; /* Mise à jour du calendrier à partir des déclarations de l'utilisateur */

/*----------------------

Chambres
----------------------*/
void chargement_chambres()                 ; /* Procédure lancée au démarrage: les chambres sont chargées dans un tableau à partir du fichier (défini dans les constantes)*/
int rech_chambre(int chambre_rech)         ; /* Trouver une chambre à partir d'un numéro de chambre */
void modification_chambre(int res_chambre) ; /* Modifier les caractéristiques d'une chambre */
void enreg_chambre()                       ; /* Enregistrer les changemens apportés aux chambres */
void affichage_chambre()                   ; /* Montre toutes les caractéristiques */
void modif_prix_chambre()                  ; /* Modification du prix des nuitées */

/*----------------------

Planning
----------------------*/
void chargement_planning()                 ; /* Procédure lancée au démarrage: le planning est chargé dans un tableau à partir du fichier (défini dans les constantes)*/
void enregistrement_planning()             ;
void maj_planning()                        ; /* La nouvelle réservation est intégrée dans le planning */
void maj_planning_travaux()                ; /* La nouvelle déclaration de travaux est intégrée dans le planning */
void annulation_resa_planning()            ; /* Met en acte l'annulation en changeant le planning */
void programme_crea(int p_hier)            ; /* Appellé par maj_calendrier, prépare le programme de la journée */
void programme_lis()                       ; /* Permet de consulter le programme de la journée */

/*----------------------

Réservations
----------------------*/
void dernier_code_resa()                   ; /* Chargement des paramètres, au démarrage */
void chargement_prix()                     ; /* Chargement des paramètres, au démarrage */

void creer_reservation()                   ; /* Création d'une réservation. Contient toutes les suivantes */
void cible_date()                          ; /* Saisie de la période demandée */
void cible_chambre()                       ; /* Saisie de tous les critères demandés pour la chambre */
void cible_chambre_simple()                ; /* Choix de la chambre simplifié */
void rech_periode(long unsigned int datearrivee, long unsigned int datedepart) ; /* Identification des cases correspondantes à la période demandée dans le tableau planning */
void calcul_nuitees()                      ; /* Calcul des nuitées selon saison pour la période demandée*/
int choix_chambre()                        ; /* Identification des chambres disponibles dans la période selon les critères demandés */
int choix_chambre_simple()                 ; /* Identification des chambres disponibles dans la période selon les critères simplifiés */
void saisie_client()                       ; /* Saisie des informations sur le client */
void nouveau_nb_resa()                     ; /* Un numéro de réservation est affecté à la demande de réservation en cours de traitement */
void paiement_resa()                       ; /* Paiement de la réservation */
void paiement_cb()                         ; /* Saisie des données de la carte bancaire en cas de paiement par cb */
void sauvegarde_resa()                     ; /* Les informations sur la réservation sont sauvegardées dans un fichier */
void depart()                              ; /* Vérification d'une éventuelle note à payer au moment du départ */
void supprime_resa(long unsigned int p_code_resa) ;/* Appelée par depart() ou bien suite à l'annulation */
void recherche_resa()                      ; /* Modification d'une réservation. Contient toutes les suivantes */
void chargement_resa(long unsigned int p_code_resa) ; /* Charge la réservation */
void affichage_resa()                      ; /* Affichage de la réservation chargée */
void choix_modif_resa()                    ; /* Choisir entre modification, annulation ou retour au menu principal */
void modif_resa()                          ; /* Modification des dates et des informations client. */
void modif_resa_cha()                      ;
void annul_origine()                       ; /* Menu de choix selon l'origine de la demande d'annulation */
void remboursement()                       ; /* En cas de remboursement integral */
void annul_client()                        ; /* Défini si il doit y avoir remboursement, à quel niveau, et puis met à jour le planning */
void remboursement_partiel(int p_poursan)  ; /* Rembourse le client en fonction du pourcentage indiqué par le paramètre p_poursan */

/*----------------------

Travaux
----------------------*/
void travaux()                            ;

/*----------------------

Note
----------------------*/
void creation_note()                       ; /* Création de la note, vide. Appellée dans creer_reservation */

void menu_recherche_note()                 ; /* Permet de choisir entre recherche_note_cha et recherche_note_num */
void recherche_note_cha()                  ; /* Prend un numéro de chambre et le passe à menu_choix_note */
void recherche_note_num()                  ; /* Prend un numéro de réservation et le passe à menu_choix_note */
void menu_choix_note(char p_entree_note[]) ; /* Offre le choix entre les trois suivantes */
void affichage_note(char p_entree_note[])  ; /* La note est affichée ensuite choix possibles (ajout, règlement)*/
void ajout_note(char p_entree_note[])      ; /* Ajout d'une entrée dans le fichier de la note */
void paiement_note(char p_entree_note[])   ; /* Paiement de la note*/
void recreation_note(char p_entree_note[]) ; /* Une fois la note payée elle est remise à zéro */
void supprime_note(long unsigned int p_code_resa) ; /* Suppression de la note */


/*----------------------

Gestion des services
complémentaires
----------------------*/
void catalogue_services_menu()                        ; /* Menu qui montre les choix possibles pour le catalogue de services */
void chargement_catalogue_services()                  ; /* Prend le fichier des services et le charge en mémoire (dans un tableau). Procédure transparente */
void affichage_catalogue()                            ; /* Montre le tableau de services */
void saisie_services()                                ; /* Pour saisir de nouveaux services, à la suite de ceux déjà listés */
void enreg_catalogue_services()                       ; /* Sauvegarde le tableau chargé en mémoire dans un fichier */
void modif_services()                                 ; /* Pour modifier les services déjà listés */
int rech_service(char nom_serv_rech[MAX_NOM_SERVICE]) ; /* Recherche pour la suppression d'un service*/
void suppression_service()                            ; /* Suppression d'un service */

/*----------------------

Nuitées
----------------------*/
void catalogue_services_menu()             ; /* Menu qui montre les choix possibles pour le catalogue de services */
void chargement_catalogue_services()       ; /* Prend le fichier des services et le charge en mémoire (dans un tableau). Procédure transparente */
void affichage_catalogue()                 ; /* Montre le tableau de services */
void saisie_services()                     ; /* Pour saisir de nouveaux services, à la suite de ceux déjà listés */
void enreg_catalogue_services()            ; /* Sauvegarde le tableau chargé en mémoire dans un fichier */
void modif_services()                      ; /* Pour modifier les services déjà listés */
void modification_catalogue()              ;
int rech_services(char nom_serv_rech[MAX_NOM_SERVICE]) ;
void sauvegarde_prix_chambres()            ; /* Sauvegarde le tableau des prix des nuitées dans le fichier PRIX_NUIT*/



/*----------------------

Structures et variables

----------------------*/

/*Utilitaires*/

char poubelle            ; /*pour vider le buffer des \n*/

/*Calendrier*/
struct jour
{
  long unsigned int date ; /*date aaaammjj*/
  short int saison       ; /*0 basse saison, 1 haute saison*/
}                        ;

struct jour calendrier[ANNEE] ;


/*Prix nuitée*/
struct prix_nuit
{
  int type_chambre        ; /* 0 simple ; 1 double ; 2 triple */
  int categorie_chambre   ; /* 0 chambre ; 1 suite */
  float prix_hs           ; /* prix haute saison */
  float prix_bs           ; /* prix basse saison */
}                         ;

struct prix_nuit tab_prix_chambres[NB_CHAMBRES_PRIX] ;
struct prix_nuit nuit                                ;


/*Réservations*/

struct resa
{
  long unsigned int code_resa    ; /* long unsigned int, cela en garantit l'unicité sur une période assez longue. */
  int chambre_resa               ; /* Le numéro de la chambre dans laquelle dormira le client */
  long unsigned int datearrivee  ; /* Date (aaaammjj) de la première nuitée */
  long unsigned int datedepart   ; /* Date (aaaammjj) de la dernière nuitée */
  int nuitees_resa[2]            ; /* Case 0 basse saison, case 1 haute saison */
  char nomclient[MAX_NOM_CLI]    ;
  char prenomclient[MAX_PNOM_CLI] ;
  char telclient[12]             ;  /* +33653332003 qui peut être affiché +33 6 53 33 20 03. Vérifier de quelle taille doit être le numéro: 12? */
  float total_resa               ; /* Montant total de la chambre en fonction des nuitées et de la saison */
  int mode_paiement              ; /* Comment est reglée la réservation: 1 espèces, 2 chèque, 3 CB, 4 virement */
}                                ;

struct resa demande              ;
long unsigned int nb_resa        ; /* Dernière réservation faite, la suivante devra prendre nb_resa+1 */
long unsigned int planning[MAX_NB_CHAMBRES][ANNEE] ; /* Les valeurs dans ce tableau sont les codes de réservation. 0 est utilisé pour signaler que la chambre est libre; 1 pour déclarer des travaux. */

int numcase_resa_chambre         ; /*Identifie la case dans le tableau planning ou dans tab_chambres qui correspond à la chambre demandée/reservée*/
int numcase_resa_date_debut, numcase_resa_date_fin ; /*identifient la position dans le planning/calendrier de la case où débute et finit la réservation*/
/* Variables globales concernant les chambres*/

struct cha
{  /* structure pour les chambres*/
   /*l'identifiant unique de la chambre est son indice dans le tableau des chambres, qui correspond également à l'indice dans le planning, cela permet de relier les deux informations*/
  int num_chambre         ; /* Numéro selon la codification de l'hôtel */
  int type_chambre        ; /* 1 simple, 2 double, 3 triple */
  int categorie_chambre   ; /* 0 chambre ; 1 suite */
  int type_lits           ; /* 1 lit simple, 2 deux lits simples, 3 lit double, 4 trois lits simples, 5 lit simple et lit double */
  int balcon              ; /* 0 pas de balcon; 1 avec balcon */
  int vue                 ; /* 0 pas de vue; 1 avec vue */
  int bain                ; /* 0 baignoire; 1 douche */
  int fumeur              ; /* 0 non fumeur, 1 fumeur */
  int animaux             ; /* 0 pas d'animaux, 1 animaux acceptés */
  /*champ remarques en chaîne de caractères*/
}                         ;

struct cha tab_chambres[MAX_NB_CHAMBRES] ; /*Tableau listant les chambres*/
struct cha chambre                       ;


int a_sauv_chambre = 0                   ; /*pour la modification des chambres*/



  /* Variables globales concernant les frais*/
struct frais
{
  long int datefrais                     ; /*autrement on en fera une string de 9, 'aaaammjj' (8) + '\0'*/
  float montantfrais                     ;
  char  nomfrais[MAX_NOM_SERVICE]        ;
}                                        ;


  /* Variables globales concernant le catalogue_services*/

int sauv_catalogue_services = 0          ; /* Test. 1 : tableau à sauvegarder; 0 non.*/
int nb_services = 0                      ; /* Pour garder trace du nombre de services déjà saisis*/

struct entree_service /* Modèle du service additionnel*/
{
  char  nom_service[MAX_NOM_SERVICE]     ;
  float prix_service                     ;
}                                        ;
struct entree_service catalogue_services[MAX_CAT_SERV] ;  /* Tableau listant les services*/



/*############################################
#                                            #
#                    main                    #
#       #############################        #
#                                            #
#                                            #
#                                            #
##############################################
*/

main()
{
  char choix_modif      ;
  int chambre_cible = 0 ; /*Chambre recherchée*/
  int choix = 3         ; /* Déclaration de la variable et initialisation à une valeur qui permet de rentrer dans le 'while'. */
  int res_chambre = 0   ; /*Résultat recherche chambres */
  int test, t1, t2      ;

  dernier_code_resa()   ;
  chargement_chambres() ;
  lecture_jours()       ;
  test_date()           ;
  chargement_planning() ;
  chargement_prix()     ;
  chargement_catalogue_services() ;

  printf("\n\nBienvenue dans le programme de gestion des réservations.\n\n") ;
  while(choix != 0) /* 0 est la valeur pour quitter. */
  {
    test = 0                                                   ;
    while(test == 0)
    {
      printf("__________________________________________\n\n") ;
      printf("              MENU PRINCIPAL\n\n")               ;
      printf("__________________________________________\n\n") ;
      printf("Voici les actions possibles\n\n")                ;
      printf("               RÉSERVATIONS      \n")            ;
      printf("__________________________________________\n\n") ;
      printf("-1- Effectuer une réservation\n")                ;
      printf("-2- Rechercher une réservation\n")               ;
      printf("-3- Rechercher une note\n")                      ;
      printf("-4- Exécuter un check-out\n\n\n")                ;
      printf("                CHAMBRES      \n")               ;
      printf("__________________________________________\n\n") ;
      printf("-5- Déclarer des travaux dans une chambre\n")    ;
      printf("-6- Rechercher une chambre\n\n\n")               ;
      printf("                PARAMÈTRES    \n")               ;
      printf("__________________________________________\n\n") ;
      printf("-7- Catalogue des services\n")                   ;
      printf("-8- Voir le programme du jour\n")                ;
      printf("-9- Modifier le prix des chambres\n\n\n")        ;
      printf("                 QUITTER    \n")                 ;
      printf("__________________________________________\n\n") ;
      printf("-0- Quitter l'application\n\n")                  ;
      /*printf("7 - Imprimer le programme d'aujourd'hui.\n") ; On ne l'a pas mis dans le planning, mais c'est une idée dont on avait parlé. On verra plus tard si et comment le faire.*/

      printf("Choix : ")                                       ;
      t1=scanf("%d", &choix)                                   ; /* Attention, à partir de ce moment il y a un '\n' qui traîne dans le buffer. */
      if(t1 == 0)
      {
        printf("Choix invalide.\n")                            ;
        while((poubelle=getchar()) != '\n')                    ;
      }
      else
      {
        test = 1                                               ;
      }
    }

    switch(choix)
    {
      case 0:
        printf("Vous avez choisi de quitter l'application.\nMerci et au revoir.\n") ;
        break                                                                       ;
      case 1:
        creer_reservation()                                                         ;
        break                                                                       ;
      case 2:
        recherche_resa()                                                            ;
        break                                                                       ;
      case 3:
        menu_recherche_note()                                                       ;
        break                                                                       ;
      case 4:
        depart()                                                                    ;
        break                                                                       ;
      case 5:
        travaux()                                                                   ;
        break                                                                       ;
      case 6:
        printf("Entrez le numéro de la chambre à rechercher: ") ;

        t2 = scanf("%d", &chambre_cible)                        ;
        if(t2 == 0)
        {
          printf("Erreur de saisie.\n")                         ;
          while((poubelle=getchar()) != '\n')                   ;
        }
        res_chambre=rech_chambre(chambre_cible)                 ;
        if(res_chambre == NON_TROUVE)
        {
          printf("%d n'a pas été trouvé.\n", chambre_cible)     ;
        }
        else
        {
          chambre=tab_chambres[res_chambre]                     ;
          affichage_chambre()                                   ;
          printf("Voulez-vous modifier la chambre (o/N) ? ")    ;
          while((poubelle=getchar()) != '\n')                   ;
          scanf("%c", &choix_modif)                             ;
          if (choix_modif == 'o')
          {
            modification_chambre(res_chambre)                   ;
          }
          printf("Retour au menu.\n")                           ;
          enreg_chambre()                                       ;

        }
        break                                                                       ;
      case 7:
        catalogue_services_menu()                                                   ;
        break                                                                       ;
      case 8:
        programme_lis()                                                             ;
        break                                                                       ;
      case 9:
        modif_prix_chambre()                                                        ;
        break                                                                       ;
      default:
        mauvais_choix(choix)                                                        ;
        break                                                                       ;
    }
  }
}


/*############################################
#                                            #
#                PARTIE JOURS                #
#        #############################       #
#                                            #
#               lecture_jours                #
#                                            #
##############################################

Prend les jours présents dans le fichier qui tient compte des jours (et saison) et les charge dans le tableau correspondant.

*/

void lecture_jours()
{
  FILE *f1                    ;
  int i                       ;

  f1 = fopen(CALENDRIER, "r") ;
  for(i=0 ; i < ANNEE ; i++)
  {
    fscanf(f1, "%lu %hd", &calendrier[i].date, &calendrier[i].saison) ;
  }
}

/*############################################
#                                            #
#                test_date                   #
#                                            #
##############################################
Vérifie si la date du jour d'aujourd'hui correspond au premier jour dans le tableau et dans le calendrier. Si la date a changé, déclanche la mise à jour dans les deux.
*/

void test_date()
{
  char choix_date = 'f'                ;
  char date_chaine[11], temporaire[5]  ;
  int i = 0, a, m, j                   ;

  printf("Test des paramètres\n")      ;
  while(choix_date != 'o')
  {
    date_chaine[0] = '\0'; /*cette ligne et les onze suivantes étaient dans une fonction à part entière, mais retourner une chaîne de caractères et l'affecter à une variable, je ne sais pas faire*/
    a = calendrier[i].date/10000                       ;
    m = (calendrier[i].date - (a * 10000)) / 100       ;
    j = (calendrier[i].date - (a * 10000) - (m * 100)) ;
    sprintf(temporaire, "%d", j)                       ;
    strcat(date_chaine, temporaire)                    ;
    strcat(date_chaine, "/")                           ;
    sprintf(temporaire, "%d", m)                       ;
    strcat(date_chaine, temporaire)                    ;
    strcat(date_chaine, "/")                           ;
    sprintf(temporaire, "%d", a)                       ;
    strcat(date_chaine, temporaire)                    ;
    printf("Aujourd'hui nous sommes le %s (o/n) ? ", date_chaine) ;
    scanf("%c", &choix_date)                           ;
    if(choix_date != 'o')
    {
      if(choix_date == 'n')
      {
        while((poubelle=getchar()) != '\n')            ;
        i++                                            ;
      }
      else
      {
        printf("Choix non accepté. Les choix possibles sont 'o' pour 'oui' ou 'n' pour 'non'.\n") ;
        while((poubelle=getchar()) != '\n')            ;
      }
    }
  }
  if(i != 0)
  {
    maj_calendrier(i) ;
  }
}

/*############################################
#                                            #
#             maj_calendrier                 #
#                                            #
##############################################

Mise à jour du calendrier déclenchée par le test sur la date courante.

*/
void maj_calendrier(int i)
{
  char date_chaine[11], temporaire[5] ;
  char choix_saison = 'z'             ;
  FILE *f1                            ;
  int j, a, m, d, test                ;

  programme_crea(i - 1)               ;

  for(j = i; j < ANNEE; j++) /* déplacer les valeurs des jours dans le calendrier: LE FUTUR C'EST MAINTENANT!*/
  {
    calendrier[j-i] = calendrier[j]   ;
  }
  for(j = ANNEE - i; j < ANNEE ; j++) /*créer les cases qui manquent: MORE FUTURE (slogan anti-punk)*/
  {
    date_chaine[0] = '\0'                                ; /*cette ligne et les onze suivantes étaient dans une fonction à part entière, mais retournaient une chaîne de caractères et l'affectaient à une variable, je ne sais pas faire*/
    a = calendrier[j-1].date/10000                       ;
    m = (calendrier[j-1].date - (a * 10000)) / 100       ;
    d = (calendrier[j-1].date - (a * 10000) - (m * 100)) ;
    sprintf(temporaire, "%d", d)                         ;
    strcat(date_chaine, temporaire)                      ;
    strcat(date_chaine, "/")                             ;
    sprintf(temporaire, "%d", m)                         ;
    strcat(date_chaine, temporaire)                      ;
    strcat(date_chaine, "/")                             ;
    sprintf(temporaire, "%d", a)                         ;
    strcat(date_chaine, temporaire)                      ;
    test = 0 ;
    while(test == 0)
    {
      printf("Quel jour vient après le %s (format jj/mm/aaaa) ? ", date_chaine) ;
      test = scanf("%d/%d/%d", &d, &m, &a)               ;
      if(test == 0)
      {
        printf("Erreur de saisie.\n")                    ;
        while((poubelle=getchar()) != '\n')              ;
      }
    }
    calendrier[j].date = jjmmaaaa_vers_aaaammjj(d, m, a) ;
    while((choix_saison != 'b') && (choix_saison != 'h'))
    {
      printf("Quelle est sa saison (b/h) ? ") ;
      while((poubelle=getchar()) != '\n')     ;
      scanf("%c", &choix_saison)              ;
      if((choix_saison != 'b') && (choix_saison != 'h'))
      {
        printf("Le choix n'est pas valide")   ;
      }
    }
    if(choix_saison == 'b')
    {
      calendrier[j].saison = 0                ;
    }
    else
    {
      calendrier[j].saison = 1                ;
    }
    choix_saison = 'z'                        ; /*réinitialisation de la variable*/
  }
  f1 = fopen(CALENDRIER, "w")                 ;
  for(j = 0 ; j < ANNEE ; j++) /*jusqu'à la fin du tableau*/
  {
    fprintf(f1, "%lu %d\n", calendrier[j].date, calendrier[j].saison) ;
  }
  fclose(f1)                                  ;
}


/*############################################
#                                            #
#               PARTIE PLANNING              #
#       #############################        #
#                                            #
#            chargement_planning             #
#                                            #
##############################################

Appelée au lancement du programme.
Charge en mémoire les données du planning présentes dans le fichier PLANNING.

*/

void chargement_planning()
{
  FILE *f1                               ;
  int i, j                               ;
  f1 = fopen(PLANNING, "r")              ;
  for(i = 0 ; i < MAX_NB_CHAMBRES ; i++)
  {
    for(j = 0 ; j < ANNEE ; j++)
    {
      fscanf(f1, "%lu", &planning[i][j]) ;
    }
  }
  fclose(f1)                             ;
}

/*############################################
#                                            #
#              maj_planning                  #
#                                            #
##############################################

Appelée par creer_reservation().
Appelle enregistrement_planning().
Une fois la réservation validée, son code est inséré dans le planning aux cases déjà déterminées dans rech_periode().

*/

void maj_planning()
{
  int i, j = 0                         ;


  while(demande.chambre_resa != tab_chambres[j].num_chambre ) /*Recherche, dans le tableau des chambres, de l'indice auquel correspond la case avec le bon num_chambre*/
  {
    j++                                ;
  }

  for(i = numcase_resa_date_debut ; i < numcase_resa_date_fin + 1 ; i++)
  {
    planning[j][i] = demande.code_resa ;
  }
  enregistrement_planning()            ;
  printf("La réservation %lu a bien été insérée dans le planning.\n", demande.code_resa) ;
}

/*############################################
#                                            #
#            maj_planning_travaux            #
#                                            #
##############################################

Appelée par travaux().
Appelle enregistrement_planning().

Une fois la demande de travaux validée, insertion dans le planning.

*/

void maj_planning_travaux()
{
  int i, j = 0                         ;


  while(demande.chambre_resa != tab_chambres[j].num_chambre ) /*Recherche, dans le tableau des chambres, de l'indice auquel correspond la case avec le bon num_chambre*/
  {
    j++                                ;
  }

  for(i = numcase_resa_date_debut ; i < numcase_resa_date_fin + 1 ; i++)
  {
    planning[j][i] = demande.code_resa ;
  }
  enregistrement_planning()            ;
}

/*############################################
#                                            #
#         annulation_resa_planning           #
#                                            #
##############################################

Appelé dans annul_origine(), après on revient au menu principal.
Appelle enregistrement_planning().
À partir du code_resa, on supprime la case dans le planning: on remet à 0

Le calcul pour trouver les cases du planning pourrait être extrait, car dans un des deux cas -passage par annul_client()- il a déjà eu lieu. Il s'agit de le sortir et ne le faire qu'en cas d'annulation par l'hôtel.

*/

void annulation_resa_planning()
{
 int i=0, j=0                                     ;

 while (demande.chambre_resa!=tab_chambres[i].num_chambre) /*on trouve l'indice de la chambre*/
 {
  i++                                             ;
 }
 while (demande.datearrivee!=calendrier[j].date) /*on trouve l'indice du premier jour du séjour*/
 {
  j++                                             ;
 }

 while(planning[i][j] == demande.code_resa)
 {
  planning[i][j] = 0                              ;
  j++                                             ;
 }
  enregistrement_planning()                       ;
  printf("L'annulation a bien été effectuée. \n") ;
}

/*############################################
#                                            #
#         enregistrement_planning            #
#                                            #
##############################################

Appelée par maj_planning, maj_planning_travaux et annulation_resa_planning.
Sauvegarde dans le fichier PLANNING les données du planning présentes en mémoire.

*/


void enregistrement_planning()
{
  FILE *f1                               ;
  int i, j                               ;
  f1 = fopen(PLANNING, "w")              ;
  for(i = 0 ; i < MAX_NB_CHAMBRES ; i++)
  {
    for(j = 0 ; j < ANNEE ; j++)
    {
      fprintf(f1, "%lu", planning[i][j]) ;
    }
  }
  fclose(f1)                             ;
}
/*############################################
#                                            #
#               programme_crea               #
#                                            #
##############################################

Lit le planning de hier, celui d'aujourd'hui et prépare le programme de la journée en confrontant les codes de réservation.

*/
void programme_crea(int p_hier)
{
  FILE *f1 ;
  int i, cpt_libres = 0, cpt_hiertr = 0, cpt_aujtr = 0, cpt_recouche = 0, cpt_senva = 0, cpt_arrive = 0 ;
  int hier_travaux[MAX_NB_CHAMBRES], auj_travaux[MAX_NB_CHAMBRES], recouche[MAX_NB_CHAMBRES], gdmen[MAX_NB_CHAMBRES], libres[MAX_NB_CHAMBRES] ;
  long unsigned int senva[MAX_NB_CHAMBRES], arrive[MAX_NB_CHAMBRES], chambre_hier, chambre_auj          ;

  chargement_planning()                                        ;
  for(i = 0 ; i < MAX_NB_CHAMBRES ; i++)
  {
    chambre_hier = planning[i][p_hier]                         ;
    chambre_auj = planning[i][p_hier+1]                        ;
    switch(chambre_hier)
    {
      case 0:
        break                                                  ;
      case 1:
        if(chambre_auj != 1)
        hier_travaux[cpt_hiertr] = tab_chambres[i].num_chambre ;
        cpt_hiertr++                                           ;
        break                                                  ;
      default:
        if(chambre_hier == chambre_auj)
        {
          recouche[cpt_recouche] = tab_chambres[i].num_chambre ;
          cpt_recouche++                                       ;
        }
        else
        {
          senva[cpt_senva] = chambre_hier                      ;
          gdmen[cpt_senva] = tab_chambres[i].num_chambre       ;
          cpt_senva++                                          ;
        }
    }
    switch(chambre_auj)
    {
      case 0:
        libres[cpt_libres] = tab_chambres[i].num_chambre       ;
        cpt_libres++                                           ;
        break                                                  ;
      case 1:
        auj_travaux[cpt_aujtr] = tab_chambres[i].num_chambre   ;
        cpt_aujtr++                                            ;
        break                                                  ;
      default:
        if(chambre_auj != chambre_hier)
        {
          arrive[cpt_arrive] = chambre_auj                     ;
        }
    }
  }

  f1 = fopen(PROGRAMME, "w")                                   ;
  fprintf(f1, "PROGRAMME DU JOUR\n\n")                         ;
  fprintf(f1, "Départs")                                       ;
  if(cpt_senva == 0)
  {
    fprintf(f1, " : aucun.\n")                                 ;
  }
  else
  {
    fprintf(f1, "\n")                                          ;
    for(i = 0 ; i < cpt_senva; i++)
    {
      fprintf(f1, "Chambre n.%d\n, réservation n.%lu", gdmen[i], senva[i]) ;
    }
  }
  fprintf(f1, "Arrivées")                            ;
  if(cpt_arrive == 0)
  {
    fprintf(f1, " : aucune.\n")                      ;
  }
  else
  {
    fprintf(f1, "\n")                                ;
    for(i = 0 ; i < cpt_arrive; i++)
    {
      fprintf(f1, "Réservation n.%lu\n", arrive[i])  ;
    }
  }
  fprintf(f1, "Chambres libres")                     ;
  if(cpt_libres == 0)
  {
    fprintf(f1, " : aucune.\n")                      ;
  }
  else
  {
    fprintf(f1, "\n")                                ;
    for( i = 0 ; i < cpt_libres ; i++)
    {
      fprintf(f1, "Chambre n.%d\n", libres[i])       ;
    }
  }
  fprintf(f1, "Ménage, recouches")                   ;
  if(cpt_recouche == 0)
  {
    fprintf(f1, " : aucune.\n")                      ;
  }
  else
  {
    fprintf(f1, "\n");
    for( i = 0 ; i < cpt_recouche ; i++)
    {
      fprintf(f1, "Chambre n.%d\n", recouche[i])     ;
    }
  }
  fprintf(f1, "Ménage, grand ménage")                ;
  if(cpt_senva == 0)
  {
    fprintf(f1, " : aucun.\n")                       ;
  }
  else
  {
    fprintf(f1, "\n")                                ;
    for(i = 0 ; i < cpt_senva; i++)
    {
      fprintf(f1, "Chambre n.%d\n", gdmen[i])        ;
    }
  }
  fprintf(f1, "Travaux finis hier, à vérifier")      ;
  if(cpt_hiertr == 0)
  {
    fprintf(f1, " : aucun.\n")                       ;
  }
  else
  {
    fprintf(f1, "\n")                                ;
    for(i = 0 ; i < cpt_hiertr; i++)
    {
      fprintf(f1, "Chambre n.%d\n", hier_travaux[i]) ;
    }
  }
  fprintf(f1, "Travaux prévus pour aujourd'hui")     ;
  if(cpt_aujtr == 0)
  {
    fprintf(f1, " : aucun.\n")                       ;
  }
  else
  {
    fprintf(f1, "\n")                                ;
    for(i = 0 ; i < cpt_aujtr; i++)
    {
      fprintf(f1, "Chambre n.%d\n", auj_travaux[i])  ;
    }
  }
  fclose(f1)                                         ;
  printf("Le programme de la journée est prêt.\n")   ;
}


/*############################################
#                                            #
#               programme_lis                #
#                                            #
##############################################

Appelée dans le menu principal.
Affiche le programme du jour via cat.
Ce n'est pas que de la fainéantise, c'est aussi pour permettre à l'utilisateur d'imprimer le fichier!
*/

void programme_lis()
{
  system("cat 1-Parametres/programme.txt") ;
}
/*############################################
#                                            #
#             PARTIE RESERVATIONS            #
#       #############################        #
#                                            #
#             dernier_code_resa              #
#                                            #
##############################################

Cherche dans le fichier qui contient le numéro de la dernière réservation faite.
Lancée au démarrage.
Le code 0 indique que la chambre est libre, le code 1 est réservé pour la déclaration de travaux.

*/

void dernier_code_resa()
{
  FILE *f1                    ;
  f1 = fopen(RESA_CODE, "r")  ;
  fscanf(f1, "%lu", &nb_resa) ;
  fclose(f1)                  ;
}

/*############################################
#                                            #
#              chargement_prix               #
#                                            #
##############################################

Au démarrage, récupère les prix des nuitées et les charge dans un tableau, variable globale.

*/

void chargement_prix()
{
  FILE *f1                 ;
  int i                    ;

  f1=fopen(PRIX_NUIT, "r") ;
  for (i=0; i<NB_CHAMBRES_PRIX; i++)
  {
    fscanf(f1, "%d %d %f %f", &tab_prix_chambres[i].type_chambre, &tab_prix_chambres[i].categorie_chambre, &tab_prix_chambres[i].prix_hs, &tab_prix_chambres[i].prix_bs) ;
  }
}

/*############################################
#                                            #
#             creer_reservation              #
#                                            #
##############################################

Procédure pour créer une réservation.


*/
void creer_reservation()
{
  int continue_resa, kell_resa, t1 = 0, t2                  ; /* continue resa permet de savoir si on poursuit ou bien on abandonne le processus. 0 pour abandonner, 1 pour continuer */

  printf("Création de réservation.\n")                      ;
  cible_date()                                              ;
  while(t1 == 0)
  {
    printf("Sélection de la chambre :\n")                   ;
    printf("-1- Spécification de tous les critères\n")      ;
    printf("-2- Par type de lits et catégorie seulement\n") ;
    printf("-3- Retour au menu principal\n")                ;
    printf("Choix : ")                                      ;
    t2 = scanf("%d", &kell_resa)                            ;
    if(t2 == 0)
    {
      printf("Erreur de saisie.\n")                         ;
      while((poubelle=getchar()) != '\n')                   ;
    }
    else
    {
      if(kell_resa > 3)
      {
        mauvais_choix(kell_resa)                            ;
      }
      else
      {
        t1 = 1                                              ;
      }
    }
  }
  if(kell_resa == 1)
  {
    cible_chambre()                                         ;
    rech_periode(demande.datearrivee, demande.datedepart)   ;
    calcul_nuitees()                                        ;
    continue_resa = choix_chambre()                         ;
    if(continue_resa == 1)
    {
      saisie_client()                                       ;
      nouveau_nb_resa()                                     ;
      paiement_resa()                                       ; /* Comme le paiement est supposé être toujours effectué avec succès, pas de test pour décider si poursuivre ou pas. */
      sauvegarde_resa()                                     ;
      creation_note()                                       ; /* On crée la note, vide. À partir de ce moment le client peut faire des commandes qui seront enregistrées.*/
      maj_planning()                                        ;
    }
  }
  else
  {
    if(kell_resa == 2)
    {
      cible_chambre_simple();
      rech_periode(demande.datearrivee, demande.datedepart) ;
      calcul_nuitees()                                      ;
      continue_resa = choix_chambre_simple()                ;
      if(continue_resa == 1)
      {
        saisie_client()                                     ;
        nouveau_nb_resa()                                   ;
        paiement_resa()                                     ; /* Comme le paiement est supposé être toujours effectué avec succès, pas de test pour décider si poursuivre ou pas. */
        sauvegarde_resa()                                   ;
        creation_note()                                     ; /* On crée la note, vide. À partir de ce moment le client peut faire des commandes qui seront enregistrées.*/
        maj_planning()                                      ;
      }
    }
    else
    {
      printf("Abandon.\nRetour au menu principal\n")        ;
    }
  }
}

/*############################################
#                                            #
#                cible_date                  #
#                                            #
##############################################

Procédure pour saisir une date.


*/
void cible_date()
{
  int jour_debut, mois_debut, annee_debut                                             ;
  int jour_fin, mois_fin, annee_fin                                                   ;
  short int  test                                                                     ; /*test 0: FAUX; test 1: VRAI*/

  test = 0                                                                            ;
  while(test == 0)
  {
    printf("Saisir la date de début (jj/mm/aaaa) : ")                                 ;
    scanf("%d/%d/%d", &jour_debut, &mois_debut, &annee_debut)                         ;
    demande.datearrivee = jjmmaaaa_vers_aaaammjj(jour_debut, mois_debut, annee_debut) ;
    if((demande.datearrivee < calendrier[0].date)|| (demande.datearrivee > calendrier[ANNEE-1].date))
    {
      printf("Date fausse.\n")                                                        ;
      while((poubelle=getchar()) != '\n')                                             ;
    }
    else
    {
      test = 1                                                                        ;
    }
  }

  test = 0                                                                            ;
  while(test == 0)
  {
    printf("Saisir la date de la dernière nuitée (jj/mm/aaaa) : ")                    ;
    scanf("%d/%d/%d", &jour_fin, &mois_fin, &annee_fin)                               ;
    demande.datedepart = jjmmaaaa_vers_aaaammjj(jour_fin, mois_fin, annee_fin)        ;
    if((demande.datedepart < demande.datearrivee)||(demande.datedepart > calendrier[ANNEE-1].date))
    {
      printf("Date fausse.\n")                                                        ;
      while((poubelle=getchar()) != '\n')                                             ;

    }
    else
    {
      test = 1                                                                        ;
    }
  }
}

/*############################################
#                                            #
#              cible_chambre                 #
#                                            #
##############################################

Procédure pour déterminer quelle chambre recherche le client.
Les données sont chargées dans la struct cha chambre.
Dans un premier temps on contraint à spécifier tout choix, on ne permet pas de déclarer un critère comme indifférent (ceci est une évolution envisageable).
*/

void cible_chambre()
{
  /*ce n'est pas nécessaire, on le déduit d cible_type_lits int cible_type_chambre dont les valeurs possibles sont 1 lit simple, 2 double, 3 triple*/
  int cible_cat_chambre  ; /* 0 chambre, 1 suite*/
  int cible_type_lits    ; /* 1 lit simple, 2 deux lits simples, 3 lit double, 4 trois lits simples, 5 lit simple et lit double */
  int cible_balcon       ; /* 0 pas, 1 balcon, 2 indifférent*/
  int cible_vue          ; /* 0 pas, 1 vue, 2 indifférent*/
  int cible_bain         ; /* 0 pas, 1 douche, 2 indifférent*/
  int cible_fumeur       ; /* 0 pas, 1 fumeur, 2 indifférent*/
  int cible_animaux      ; /* 0 pas, 1 animaux acceptés, 2 indifférent*/
  int test = 0           ;

  cible_type_lits = 999  ;
  while((cible_type_lits < 1) || (cible_type_lits > 5))
  {
    printf("Quel type de lits ?\n")                ;
    printf("-1- un lit simple\n")                  ;
    printf("-2- deux lits simples\n")              ;
    printf("-3- un lit double\n")                  ;
    printf("-4- trois lits simples\n")             ;
    printf("-5- un lit double et un lit simple\n") ;
    printf("Saisir le type de lits : ")            ;
    test = scanf("%d", &cible_type_lits)           ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                ;
      while((poubelle=getchar()) != '\n')          ;
    }
    else
    {
      if((cible_type_lits < 1) || (cible_type_lits > 5))
      {
        printf("Choix non valide.\n")              ;
      }
    }

  }
  chambre.type_lits = cible_type_lits              ;

  switch(cible_type_lits)
  {
    case 1 :
      chambre.type_chambre = 1                     ;
      break                                        ;
    case 2 :
    case 3 :
      chambre.type_chambre = 2                     ;
      break                                        ;
    case 4 :
    case 5 :
      chambre.type_chambre = 3                     ;
      break                                        ;
  }

  cible_cat_chambre = 999                          ;
  while((cible_cat_chambre < 0) || (cible_cat_chambre > 1))
  {
    printf("Quelle catégorie de chambre ?\n")      ;
    printf("-0- chambre\n")                        ;
    printf("-1- suite\n")                          ;
    printf("Saisir la catégorie : ")               ;
    test = scanf("%d", &cible_cat_chambre)         ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                ;
      while((poubelle=getchar()) != '\n')          ;
    }
    else
    {
      if((cible_cat_chambre < 0) || (cible_cat_chambre > 1))
      {
        printf("Choix non valide.\n")              ;
      }
    }
  }
  chambre.categorie_chambre = cible_cat_chambre    ;

  cible_balcon = 999                               ;
  while((cible_balcon < 0) || (cible_balcon > 1))
  {
    printf("Chambre avec balcon ?\n")              ;
    printf("-0- pas de balcon\n")                  ;
    printf("-1- balcon\n")                         ;
    printf("Choix : ")                             ;
    test = scanf("%d", &cible_balcon)              ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                ;
      while((poubelle=getchar()) != '\n')          ;
    }
    else
    {
      if((cible_balcon < 0) || (cible_balcon > 1))
      {
        printf("Choix non valide.\n")              ;
      }
    }
  }
  chambre.balcon = cible_balcon                    ;

  cible_bain = 999                                 ;
  while((cible_bain < 0) || (cible_bain > 1))
  {
    printf("Quel type de salle de bain ?\n")       ;
    printf("-0- baignoire\n")                      ;
    printf("-1- douche\n")                         ;
    printf("Choix : ")    ;
    test = scanf("%d", &cible_bain)                ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                ;
      while((poubelle=getchar()) != '\n')          ;
    }
    else
    {
      if((cible_bain < 0) || (cible_bain > 1))
      {
        printf("Choix non valide.\n")              ;
      }
    }
  }
  chambre.bain = cible_bain                        ;

  cible_vue = 999                                  ;
  while((cible_vue < 0) || (cible_vue > 1))
  {
    printf("Chambre avec vue ?\n")                 ;
    printf("-0- pas de vue\n")                     ;
    printf("-1- vue\n")                            ;
    printf("Choix : ")                             ;
    test = scanf("%d", &cible_vue)                 ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                ;
      while((poubelle=getchar()) != '\n')          ;
    }
    else
    {
      if((cible_vue < 0) || (cible_vue > 1))
      {
        printf("Choix non valide.\n")              ;
      }
    }
  }
  chambre.vue = cible_vue                          ;

  cible_fumeur = 999                               ;
  while((cible_fumeur < 0) || (cible_fumeur > 1))
  {
    printf("Chambre pour fumeur ?\n")              ;
    printf("0 - non fumeur\n")                     ;
    printf("1 - fumeur\n")                         ;
    printf("Choix : ")                             ;
    test = scanf("%d", &cible_fumeur)              ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                ;
      while((poubelle=getchar()) != '\n')          ;
    }
    else
    {
      if((cible_fumeur < 0) || (cible_fumeur > 1))
      {
        printf("Choix non valide.\n")              ;
      }
    }
  }
  chambre.fumeur = cible_fumeur                    ;


  cible_animaux = 999                              ;
  while((cible_animaux < 0) || (cible_animaux > 1))
  {
    printf("Chambre avec animaux autorisés?\n")    ;
    printf("0 - animaux non autorisés\n")          ;
    printf("1 - animaux autorisés\n")              ;
    printf("Choix : ")                             ;
    test = scanf("%d", &cible_animaux)             ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                ;
      while((poubelle=getchar()) != '\n')          ;
    }
    else
    {
      if((cible_animaux < 0) || (cible_animaux > 1))
      {
        printf("Choix non valide.\n")              ;
      }
    }
  }
  chambre.animaux = cible_animaux                  ;
}

/*############################################
#                                            #
#           cible_chambre_simple             #
#                                            #
##############################################

Procédure pour déterminer quelle chambre recherche le client sur moins de critères.
Les données sont chargées dans la struct cha chambre.
*/

void cible_chambre_simple()
{
  int cible_cat_chambre                            ; /* 0 chambre, 1 suite*/
  int cible_type_lits                              ; /* 1 lit simple, 2 deux lits simples, 3 lit double, 4 trois lits simples, 5 lit simple et lit double */
  int test = 0                                     ;

  cible_type_lits = 999                            ;
  while((cible_type_lits < 1) || (cible_type_lits > 5))
  {
    printf("Quel type de lits ?\n")                ;
    printf("-1- un lit simple\n")                  ;
    printf("-2- deux lits simples\n")              ;
    printf("-3- un lit double\n")                  ;
    printf("-4- trois lits simples\n")             ;
    printf("-5- un lit double et un lit simple\n") ;
    printf("Saisir le type de lits : ")            ;
    test = scanf("%d", &cible_type_lits)           ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                ;
      while((poubelle=getchar()) != '\n')          ;
    }
    else
    {
      if((cible_type_lits < 1) || (cible_type_lits > 5))
      {
        printf("Choix non valide.\n")              ;
      }
    }
  }
  chambre.type_lits = cible_type_lits              ;

  switch(cible_type_lits)
  {
    case 1 :
      chambre.type_chambre = 1                     ;
      break                                        ;
    case 2 :
    case 3 :
      chambre.type_chambre = 2                     ;
      break                                        ;
    case 4 :
    case 5 :
      chambre.type_chambre = 3                     ;
      break                                        ;
  }

  cible_cat_chambre = 999                          ;
  while((cible_cat_chambre < 0) || (cible_cat_chambre > 1))
  {
    printf("Quelle catégorie de chambre ?\n")      ;
    printf("-0- chambre\n")                        ;
    printf("-1- suite\n")                          ;
    printf("Saisir la catégorie : ")               ;
    test = scanf("%d", &cible_cat_chambre)         ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                ;
      while((poubelle=getchar()) != '\n')          ;
    }
    else
    {
      if((cible_cat_chambre < 0) || (cible_cat_chambre > 1))
      {
        printf("Choix non valide.\n")              ;
      }
    }
  }
  chambre.categorie_chambre = cible_cat_chambre    ;
}

/*############################################
#                                            #
#             rech_periode                   #
#                                            #
##############################################

Trouve les indices des cases dans le planning pour les dates données.

*/

void rech_periode(long unsigned int datearrivee, long unsigned int datedepart)
{
  int i=0                                   ;

  numcase_resa_date_debut=NON_TROUVE        ;
  numcase_resa_date_fin=NON_TROUVE          ;


  while((i<ANNEE)&&(numcase_resa_date_debut==NON_TROUVE))
  {
    if(demande.datearrivee==calendrier[i].date)
    {
      numcase_resa_date_debut=i             ;
    }
    else
    {
      i++                                   ;
    }
  }
  if (numcase_resa_date_debut!=NON_TROUVE)
  {
    while ((i<ANNEE)&&(numcase_resa_date_fin==NON_TROUVE))
    {
      if(demande.datedepart==calendrier[i].date)
      {
        numcase_resa_date_fin=i              ;
      }
      else
      {
        i++                                  ;
      }
    }
  }
  else
  {
      printf("Date d'arrivée non trouvée\n") ;
  }
  if (numcase_resa_date_fin==NON_TROUVE)
  {
    printf("Date de départ non trouvée\n")   ;
  }
}

/*############################################
#                                            #
#             calcul_nuitees                 #
#                                            #
##############################################

Renseigne les deux cases du tableau demande.nuitees_resa en regardant dans le calendrier[].saison
saison = 0 : basse saison
saison = 1 : haute saison

[0] = basse saison
[1] = haute saison

*/

void calcul_nuitees()
{
  int i                       ;

  i = numcase_resa_date_debut ;
  demande.nuitees_resa[0] = 0 ;
  demande.nuitees_resa[1] = 0 ;


  while(i < (numcase_resa_date_fin + 1))
  {
    if(calendrier[i].saison == 0)
    {
      demande.nuitees_resa[0]++ ;
    }
    else
    {
      demande.nuitees_resa[1]++ ;
    }
    i++                         ;
  }
  if((demande.nuitees_resa[0] == 0) && (demande.nuitees_resa[1] == 0))
  {
    printf("Erreur dans le calcul des nuitées.") ;
  }
}

/*############################################
#                                            #
#             choix_chambre                  #
#                                            #
##############################################
Dans les bornes du planning données par rech_periode, recherche de chambres avec code de réservation qui indique qu'elles sont libres.
On désigne une liste des chambres possibles.
On demande d'en choisir une ou bien de renoncer.
*/
int choix_chambre()
{
  int chambres_ok[MAX_NB_CHAMBRES]         ;
  int chambres_dispo[MAX_NB_CHAMBRES]      ;
  int i, j, k, l, m, test, chambre_choisie ;
  j = 0                                    ;


  /* On trouve les chambres qui correspondent à la chambre souhaitée */

  for(i=0 ; i < MAX_NB_CHAMBRES ; i++)
  {
    if
    (
      (chambre.type_chambre == tab_chambres[i].type_chambre) &&
      (chambre.type_lits == tab_chambres[i].type_lits) &&
      (chambre.categorie_chambre == tab_chambres[i].categorie_chambre) &&
      (chambre.balcon == tab_chambres[i].balcon) &&
      (chambre.bain == tab_chambres[i].bain) &&
      (chambre.vue == tab_chambres[i].vue) &&
      (chambre.fumeur == tab_chambres[i].fumeur) &&
      (chambre.animaux == tab_chambres[i].animaux)
    )
    {
    chambres_ok[j] = i ;
    j++                ;
    }
  }

  printf("Chambres correspondant à la demande : %d\n", j) ;

  /*On teste leur disponibilité sur la période demandée*/
  l = 0 ; /*décompte des chambres disponibles*/
  if(j > 0)
  {
    for(i = 0 ; i < j ; i++)
    {
      test = 1            ;
      k = chambres_ok[i]  ;
      m = numcase_resa_date_debut ;
      while((test==1)&&(m <= numcase_resa_date_fin))
       /*disponibilité sur le premier jour*/
      {
        if(planning[k][m] != 0)
        {
          test = 0 ;
        }
        else
        {
          m++ ;
        }
      }
      if(test == 1)
      {
        chambres_dispo[l] = chambres_ok[i] ;
        l++                                ;
      }
    }
  }
  else
  {
    printf("Pas de réservation possible.\n") ;
    return(0)                                ;
    /*Offrir la possibilité de modifier les critères de chambre*/
  }
  /*Si l == 0, pas de choix disponibles dans la période, avec les critères donnés*/
  if(l==0)
  {
    printf("Il n'y a pas de chambres disponibles dans la période définie selon les critères donnés\n") ;
    return(0) ;
    /*Offrir la possibilité de modifier les critères de date*/
  }
  else
  {
    printf("Chambres disponibles : %d\n", l)                    ;
    for(i=0; i < l; i++)
    {
      j = chambres_dispo[i]                                     ;
      printf("Chambre n.%d\n", tab_chambres[j].num_chambre)     ;
    }
  }
  test = NON_TROUVE                                             ;
  while(test == NON_TROUVE)
  {
    printf("Choisir la chambre à réserver (0 pour quitter) : ") ;
    scanf("%d", &chambre_choisie)                               ;
    if(chambre_choisie == 0)
    {
      return(0)                                                 ;
    }
    else
    {

      i = 0                                                     ;
      while((test == NON_TROUVE) && (i < l)) /*test pour vérifier que le choix fait est parmi les choix possibles*/
      {
        j = chambres_dispo[i]                                   ;
        if(chambre_choisie == tab_chambres[j].num_chambre)
        {
          test = 1                                              ;
        }
        else
        {
          i++                                                   ;
        }
      }
      if(test == NON_TROUVE)
      {
        printf("Le choix n'est pas valide. Choisir une des chambres disponibles affichées.\n") ;
      }
    }
  }
  demande.chambre_resa = chambre_choisie                        ;
  return(1)                                                     ;
}

/*############################################
#                                            #
#           choix_chambre_simple             #
#                                            #
##############################################
Dans les bornes du planning données par rech_periode, recherche de chambres avec code de réservation qui indique qu'elles sont libres.
On désigne une liste des chambres possibles.
On demande d'en choisir une ou bien de renoncer.
*/


int choix_chambre_simple()
{
  int chambres_ok[MAX_NB_CHAMBRES]         ;
  int chambres_dispo[MAX_NB_CHAMBRES]      ;
  int i, j, k, l, m, test, chambre_choisie ;
  j = 0                                    ;


  /* On trouve les chambres qui correspondent à la chambre souhaitée */

  for(i=0 ; i < MAX_NB_CHAMBRES ; i++)
  {
    if
    (
      (chambre.type_chambre == tab_chambres[i].type_chambre) &&
      (chambre.type_lits == tab_chambres[i].type_lits) &&
      (chambre.categorie_chambre == tab_chambres[i].categorie_chambre)
    )
    {
    chambres_ok[j] = i ;
    j++                ;
    }
  }

  printf("Chambres correspondant à la demande : %d\n", j) ;

  /*On teste leur disponibilité sur la période demandée*/
  l = 0 ; /*décompte des chambres disponibles*/
  if(j > 0)
  {
    for(i = 0 ; i < j ; i++)
    {
      test = 1            ;
      k = chambres_ok[i]  ;
      m = numcase_resa_date_debut ;
      while((test==1)&&(m <= numcase_resa_date_fin))
       /*disponibilité sur le premier jour*/
      {
        if(planning[k][m] != 0)
        {
          test = 0 ;
        }
        else
        {
          m++ ;
        }
      }
      if(test == 1)
      {
        chambres_dispo[l] = chambres_ok[i] ;
        l++                                ;
      }
    }
  }
  else
  {
    printf("Pas de réservation possible.\n") ;
    return(0)                                ;
    /*Offrir la possibilité de modifier les critères de chambre*/
  }
  /*Si l == 0, pas de choix disponibles dans la période, avec les critères donnés*/
  if(l==0)
  {
    printf("Il n'y a pas de chambres disponibles dans la période définie selon les critères donnés\n") ;
    return(0) ;
    /*Offrir la possibilité de modifier les critères de date*/
  }
  else
  {
    printf("Chambres disponibles : %d\n", l)                    ;
    for(i=0; i < l; i++)
    {
      j = chambres_dispo[i]                                     ;
      printf("Chambre n.%d\n", tab_chambres[j].num_chambre)     ;
    }
  }
  test = NON_TROUVE                                             ;
  while(test == NON_TROUVE)
  {
    printf("Choisir la chambre à réserver (0 pour quitter) : ") ;
    scanf("%d", &chambre_choisie)                               ;
    if(chambre_choisie == 0)
    {
      return(0)                                                 ;
    }
    else
    {

      i = 0                                                     ;
      while((test == NON_TROUVE) && (i < l)) /*test pour vérifier que le choix fait est parmi les choix possibles*/
      {
        j = chambres_dispo[i]                                   ;
        if(chambre_choisie == tab_chambres[j].num_chambre)
        {
          test = 1                                              ;
        }
        else
        {
          i++                                                   ;
        }
      }
      if(test == NON_TROUVE)
      {
        printf("Le choix n'est pas valide. Choisir une des chambres disponibles affichées.\n") ;
      }
    }
  }
  demande.chambre_resa = chambre_choisie                        ;
  return(1)                                                     ;
}

/*############################################
#                                            #
#             saisie_client                  #
#                                            #
##############################################

À ajouter, un test sur la chaîne de caractères du numéro de téléphone.
Le premier doit être un +, les suivants que des chiffres.

*/
void saisie_client()
{
  char nom_client[100], prenom_client[200], numero_telephone_client[20] ;

  printf("Nom du client : ")                                            ;
  scanf("%s", demande.nomclient)                                        ;

  printf("Prénom du client : ")                                         ;
  scanf("%s", demande.prenomclient)                                     ;

  printf("Numéro de téléphone (format +3301234567) : ")                 ;
  scanf("%s", demande.telclient)                                        ;
}


/*############################################
#                                            #
#            nouveau_nb_resa                 #
#                                            #
##############################################

Un numéro de réservation est affecté à la demande de réservation en cours de traitement.

*/

void nouveau_nb_resa()
{
  FILE *f1                        ;
  demande.code_resa = nb_resa + 1 ; /* La nouvelle réservation = dernière réservation +1 */
  nb_resa = demande.code_resa     ; /* Mise à jour du dernier numéro de réservation */

  f1=fopen(RESA_CODE, "w")        ; /* Afin de garantir la symmétrie entre nb_resa et l'information sauvegardée dans le fichier, mise à jour du fichier */
  fprintf(f1, "%lu", nb_resa)     ;
  fclose(f1)                      ;
}


/*############################################
#                                            #
#             paiement_resa                  #
#                                            #
##############################################

*/
void paiement_resa()
{
  char date_1[11], date_2[11], temporaire[5]             ;
  int i, test = 0, a, m, j, t2                           ;
  struct prix_nuit nuit                                  ;
  FILE *f1;

  nuit.type_chambre = chambre.type_chambre               ; /* On retrouve les critères de type de chambre et catégorie de chambre, nécessaires pour déterminer le prix à la nuitée */
  nuit.categorie_chambre = chambre.categorie_chambre     ;

  i= -1;
  while (test == 0) /* On retrouve le prix à la nuitée en trouvant dans le tableau des prix le champ où type de chambre et catégorie sont les mêmes que dans notre cas */
  {
    i++                                                    ;
    if ((tab_prix_chambres[i].type_chambre==nuit.type_chambre) && (tab_prix_chambres[i].categorie_chambre==nuit.categorie_chambre))

    {
      nuit=tab_prix_chambres[i]                            ;
      test = 1                                             ;
    }
  }
  demande.total_resa=(demande.nuitees_resa[0]*nuit.prix_bs)+(demande.nuitees_resa[1]*nuit.prix_hs) ;

  date_1[0] = '\0'                                         ;
  temporaire[0] = '\0'                                     ;
  a = demande.datearrivee/10000                            ;
  m = (demande.datearrivee - (a * 10000)) / 100            ;
  j = (demande.datearrivee - (a * 10000) - (m * 100))      ;
  sprintf(temporaire, "%d", j)                             ;
  strcat(date_1, temporaire)                               ;
  strcat(date_1, "/")                                      ;
  sprintf(temporaire, "%d", m)                             ;
  strcat(date_1, temporaire)                               ;
  strcat(date_1, "/")                                      ;
  sprintf(temporaire, "%d", a)                             ;
  strcat(date_1, temporaire)                               ;

  date_2[0] = '\0'                                         ;
  temporaire[0] = '\0'                                     ;
  a = demande.datedepart/10000                             ;
  m = (demande.datedepart - (a * 10000)) / 100             ;
  j = (demande.datedepart - (a * 10000) - (m * 100))       ;
  sprintf(temporaire, "%d", j)                             ;
  strcat(date_2, temporaire)                               ;
  strcat(date_2, "/")                                      ;
  sprintf(temporaire, "%d", m)                             ;
  strcat(date_2, temporaire)                               ;
  strcat(date_2, "/")                                      ;
  sprintf(temporaire, "%d", a)                             ;
  strcat(date_2, temporaire)                               ;


  printf("Réservation n.%lu\n", demande.code_resa)         ;
  printf("Client : %s %s\n", demande.nomclient, demande.prenomclient)    ;
  printf("Séjour du %s au %s\n", date_1, date_2 )          ;
  printf("Chambre n.%d\n", demande.chambre_resa)           ;
  if(demande.nuitees_resa[0] > 0)
  {
    printf("Nuit en tarif basse saison : %d\n", demande.nuitees_resa[0]) ;
    printf("Tarif par nuit : %.2f\n", nuit.prix_bs)        ;
  }
  if(demande.nuitees_resa[1] > 0)
  {
    printf("Nuit en tarif haute saison : %d\n", demande.nuitees_resa[1]) ;
    printf("Tarif par nuit : %.2f\n", nuit.prix_hs)        ;
  }
  test = 0 ;
  while(test == 0)
  {
    printf("Montant à payer : %.2f\n", demande.total_resa) ;
    printf("Choisir le mode de paiement: \n")              ;
    printf("-1- Espèces\n")                                ;
    printf("-2- Chèque\n")                                 ;
    printf("-3- Carte bancaire\n")                         ;
    printf("-4- Virement\n")                               ;
    printf("Choix : ")                                     ;
    t2 = scanf("%d", &demande.mode_paiement)               ;
    if(t2 == 0)
    {
      printf("Erreur de saisie.\n")                        ;
    }
    else
    {
      if((demande.mode_paiement < 0) || (demande.mode_paiement > 4))
      {
        mauvais_choix(demande.mode_paiement)               ;
      }
      else
      {
        if(demande.mode_paiement == 3)
        {
          paiement_cb()                                    ;
        }
        printf("Le paiement a bien été effectué.\n")       ;
        test = 1                                           ;
      }
    }
  }
}


/*############################################
#                                            #
#              paiement_cb                   #
#                                            #
##############################################

Si dans paiement_resa le choix de paiement saisi (demande.mode_paiement) est 3, formulaire de saisie des données de la carte bleue.

À ajouter: tests sur les valeurs saisies pour la carte de crédit, tests sur les espaces.
*/


void paiement_cb()
{
  char numero_cb[16]                       ;
  char titulaire_cb_nom[20]                ;
  char titulaire_cb_prenom[20]             ;
  int test = 0, mois_cb, annee_cb, code_cb ;

//nom, prénom, numéro, code 3 chiffres, validité

  printf("Saisir le nom du titulaire de la carte bancaire : ")                ;
  scanf("%s", titulaire_cb_nom)                                               ;

  printf("Saisir le prénom du titulaire de la carte bancaire : ")             ;
  scanf("%s", titulaire_cb_prenom)                                            ;

  printf("Saisir les 16 chiffres de la carte bancaire, sans espaces : ")      ;
  scanf("%s", numero_cb)                                                      ;

  while(test == 0)
  {
    printf("Saisir la date de fin de validité de la carte (format mm/aa) :" ) ;
    test = scanf("%d/%d", &mois_cb, &annee_cb)                                ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                                           ;
    }
  }

  test = 0                                                                    ;
  while(test == 0)
  {
    printf("Saisir le code de sécurité de trois chiffres :" )                 ;
    test = scanf("%d", &code_cb )                                             ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                                           ;
    }
  }
}


/*############################################
#                                            #
#           sauvegarde_resa                  #
#                                            #
##############################################

Les informations sur la réservation sont sauvegardées dans le fichier DOSSIER_RESA/$code_resa.txt.

*/

void sauvegarde_resa()
{
  char entree_resa[20], temporaire[11]          ; /* Devant acceuillir un long int, temporaire doit être 10 +'\0'*/
  FILE *f1                                      ;

  temporaire[0] = '\0'                          ;
  entree_resa[0] = '\0'                         ;
  strcat(entree_resa, DOSSIER_RESA)             ;
  sprintf(temporaire, "%lu", demande.code_resa) ;
  strcat(entree_resa, temporaire)               ;
  strcat(entree_resa, ".txt")                   ;
  f1=fopen(entree_resa, "w")                    ;
  fprintf(f1, "%lu %d %lu %lu %d %d %s %s %s %.2f %d", demande.code_resa, demande.chambre_resa, demande.datearrivee, demande.datedepart, demande.nuitees_resa[0], demande.nuitees_resa[1], demande.nomclient, demande.prenomclient, demande.telclient, demande.total_resa, demande.mode_paiement);
  fclose(f1)                                    ;
  printf("La réservation n.%lu a bien été enregistrée.\n", demande.code_resa) ;
}

/*############################################
#                                            #
#                   depart                   #
#                                            #
##############################################

Appelée dans le menu principal, lui passe la main après avoir effectué la suppresssion du fichier de réservation.
Gestion du départ: si note à régler alors afficher la note.

*/
void depart()
{
  char note_nom_cli[MAX_NOM_CLI], note_pnom_cli[MAX_PNOM_CLI] ;
  char temporaire[11], entree_note[20]                        ;
  int i=0, t1 =0 , t2                                         ;
  FILE *f1                                                    ;
  float note_total = 0                                        ;
  long unsigned int id_resa                                   ;
  struct frais note[MAX_ENTREES_FRAIS]                        ;

  while(t1 == 0)
  {
    printf("Saisir le numéro de réservation : " )             ;
    t2 = scanf("%lu", &id_resa)                               ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                            ;
      while((poubelle=getchar()) != '\n')                     ;
    }
    else
    {
      t1 = 1 ;
    }
  }

  temporaire[0] = '\0'                                        ;
  entree_note[0] = '\0'                                       ;
  strcat(entree_note, DOSSIER_NOTES)                          ;
  sprintf(temporaire, "%lu", id_resa)                         ;
  strcat(entree_note, temporaire)                             ;
  strcat(entree_note, ".txt")                                 ;
  t1 = 0                                                      ;
  while(t1 == 0)
  {
    i = 0                                                     ;
    f1=fopen(entree_note, "r")                                ;
    fscanf(f1, "%s %s\n", note_nom_cli, note_pnom_cli) ; /*Pour avoir accès à toutes les informations rapidement et de manière indépendante de la réservation, on met dans la première ligne du fichier nom et prénom du client*/
    while(!feof(f1))
    {
      fscanf(f1, "%lu %s %f", &note[i].datefrais, note[i].nomfrais, &note[i].montantfrais) ;
      note_total += note[i].montantfrais                      ;
      i++                                                     ;
    }
    fclose(f1)                                                ;

    if (i>0)
    {
      printf("Il reste une note à payer.\n")                  ;
      affichage_note(entree_note)                             ;
    }
    else
    {
      t1 = 1                                                  ;
    }
  }
  supprime_note(id_resa)                                      ;
  printf("Aucune note à payer. Bon départ !\n")               ;
}

/*############################################
#                                            #
#              supprime_resa                 #
#                                            #
##############################################

Appelée dans le annul_origine() et dans depart(), leur passe la main après avoir effectué la suppresssion du fichier de réservation.

*/

void supprime_resa(long unsigned int p_code_resa)
{
  char entree_resa[20], temporaire[11]    ;
  FILE *f1                                ;
  int test                                ;

  temporaire[0] = '\0'                    ;
  entree_resa[0] = '\0'                   ;
  strcat(entree_resa, DOSSIER_RESA)       ;
  sprintf(temporaire, "%lu", p_code_resa) ;
  strcat(entree_resa, temporaire)         ;
  strcat(entree_resa, ".txt")             ;

  test = unlink(entree_resa)              ;
  if(test == -1)
  {
    printf("Erreur dans la suppression du fichier\n")              ;
    printf("Supprimez manuellemenet le fichier %s\n", entree_resa) ;
  }
}

/*############################################
#                                            #
#           recherche_resa                   #
#                                            #
##############################################

Appelé dans le main() le programme gère la recherche et, le cas échéant, modification d'une réservation.
Appelle dans l'ordre chargement_resa(), affichage_resa() et choix_modif_resa().

*/

void recherche_resa()
{
  int test = 0, t2                                                      ;
  long unsigned int code_modif_resa                                     ;
  printf("Recherche de réservation.\n")                                 ;
  while(test == 0)
  {
    printf("Veuillez saisir le numéro de la réservation à rechercher ") ;
    printf("(0 pour revenir au menu principal) : ")                     ;
    t2 = scanf("%lu", &code_modif_resa)                                 ;
    if(t2 == 0)
    {
      printf("Choix non valide.\n")                                     ;
      while((poubelle=getchar()) != '\n')                               ;
    }
    else /* l'utilisateur a effectivement saisi un numérique */
    {
       if((code_modif_resa > nb_resa)||(code_modif_resa == 1)) /* 0 est le code pour indiquer que la réservation est possible, 1 pour signaler des travaux. On ne veut pas permettre leur modification */
       {
         printf("Code de réservation invalide.\n")                      ;
       }
       else
       {
         test = 1                                                       ; /* On sortira de toute façon de la boucle while */
         if(code_modif_resa == 0)
         {
           printf("Retour au menu principal.\n")                        ;
         }
         else
         {
           chargement_resa(code_modif_resa)                             ;
           affichage_resa()                                             ;
           choix_modif_resa()                                           ;
         }
       }
     } /* fin des tests sur le numérique */
   } /* fin du while */
  }

/*############################################
#                                            #
#           chargement_resa                  #
#                                            #
##############################################

Appelée par recherche_resa(), passe la main à affichage_resa() et choix_modif_resa().
Reçoit un code de réservation en paramètre et charge les données dans la variable globale demande.
Le int retourné indique le résultat de l'opération.

*/

void chargement_resa(long unsigned int p_code_resa)
{
  char entree_resa[20], temporaire[11]    ;
  FILE *f1                                ;

  temporaire[0] = '\0'                    ;
  entree_resa[0] = '\0'                   ;
  strcat(entree_resa, DOSSIER_RESA)       ;
  sprintf(temporaire, "%lu", p_code_resa) ;
  strcat(entree_resa, temporaire)         ;
  strcat(entree_resa, ".txt")             ;

  f1 = fopen(entree_resa, "r")            ;
  if(f1 != NULL)
  {
  fscanf(f1, "%lu %d %lu %lu %d %d %s %s %s %f %d", &demande.code_resa, &demande.chambre_resa, &demande.datearrivee, &demande.datedepart, &demande.nuitees_resa[0], &demande.nuitees_resa[1], demande.nomclient, demande.prenomclient, demande.telclient, &demande.total_resa, &demande.mode_paiement) ;
  fclose(f1)                              ;
  }
  else
  {
    printf("Il n'existe pas de fichier pour la réservation %lu.\n", p_code_resa) ;
    printf("Les informations qui suivent sont fausses.\n")                       ;
  }
}


/*############################################
#                                            #
#             affichage_resa                 #
#                                            #
##############################################

Appelée par recherche_resa() après chargement_resa(), passe la main à choix_modif_resa().
Affiche la réservation chargée dans la variable globale demande.

*/

void affichage_resa()
{
  char paiement[9], date_1[11], date_2[11], temporaire[5]                ;
  int a, m, j                                                            ;
  paiement[0] = '\0'                                                     ;


  date_1[0] = '\0'                                                       ;
  temporaire[0] = '\0'                                                   ;
  a = demande.datearrivee/10000                                          ;
  m = (demande.datearrivee - (a * 10000)) / 100                          ;
  j = (demande.datearrivee - (a * 10000) - (m * 100))                    ;
  sprintf(temporaire, "%d", j)                                           ;
  strcat(date_1, temporaire)                                             ;
  strcat(date_1, "/")                                                    ;
  sprintf(temporaire, "%d", m)                                           ;
  strcat(date_1, temporaire)                                             ;
  strcat(date_1, "/")                                                    ;
  sprintf(temporaire, "%d", a)                                           ;
  strcat(date_1, temporaire)                                             ;

  date_2[0] = '\0'                                                       ;
  temporaire[0] = '\0'                                                   ;
  a = demande.datedepart/10000                                           ;
  m = (demande.datedepart - (a * 10000)) / 100                           ;
  j = (demande.datedepart - (a * 10000) - (m * 100))                     ;
  sprintf(temporaire, "%d", j)                                           ;
  strcat(date_2, temporaire)                                             ;
  strcat(date_2, "/")                                                    ;
  sprintf(temporaire, "%d", m)                                           ;
  strcat(date_2, temporaire)                                             ;
  strcat(date_2, "/")                                                    ;
  sprintf(temporaire, "%d", a)                                           ;
  strcat(date_2, temporaire)                                             ;

  switch (demande.mode_paiement)
  {
    case 1:
      strcpy(paiement, "espèces")                                        ;
      break                                                              ;
    case 2:
      strcpy(paiement, "chèque")                                         ;
      break                                                              ;
    case 3:
      strcpy(paiement, "CB")                                             ;
      break                                                              ;
    case 4:
      strcpy(paiement, "virement")                                       ;
      break                                                              ;
    default:
      printf("Le moyen de paiement n'a pas été correctement renseigné.\n") ;
      break                                                              ;
  }
  printf("Réservation n.%lu\n", demande.code_resa)                       ;
  printf("Client : %s %s\n", demande.nomclient, demande.prenomclient)    ;
  printf("Séjour du %s au %s\n", date_1, date_2 )                        ;
  printf("Chambre n.%d\n", demande.chambre_resa)                         ;
  if(demande.nuitees_resa[0] > 0)
  {
    printf("Nuit en tarif basse saison : %d\n", demande.nuitees_resa[0]) ;
/*    printf("Tarif par nuit : %.2f\n", nuit.prix_bs)                      ; Pas de décompte des tarifs par nuit: en cas de changement dans les tarifs, nous lirions le nouveau prix, ce qui donnerait un résultat en désaccord avec le total. Faute d'historisation des prix, omission ici de cette information. */
  }
  if(demande.nuitees_resa[1] > 0)
  {
    printf("Nuit en tarif haute saison : %d\n", demande.nuitees_resa[1]) ;
/*    printf("Tarif par nuit : %.2f\n", nuit.prix_hs)                      ; Pas de décompte des tarifs par nuit: en cas de changement dans les tarifs, nous lirions le nouveau prix, ce qui donnerait un résultat en désaccord avec le total. Faute d'historisation des prix, omission ici de cette information. */
  }
  printf("Montant reglé : %.2f\n", demande.total_resa)                   ;
  printf("Moyen de paiement : %s\n\n", paiement)                         ;
}

/*############################################
#                                            #
#           choix_modif_resa                 #
#                                            #
##############################################

Appelé dans recherche_resa() après affichage_resa() et chargement_resa().
Choisir entre modification, annulation ou retour au menu principal.

*/

void choix_modif_resa()
{
  int choix_mod, t1 = 0, t2 = 0                    ;

  while(t1 == 0)
  {
    while(t2 == 0)
    {
      printf("Que voulez-vous faire? \n")          ;
      printf("-1- Modifier la réservation \n")     ;
      printf("-2- Annuler la réservation \n")      ;
      printf("-3- Retourner au menu principal \n") ;
      printf("Choix : " )                          ;
      t2 = scanf("%d", &choix_mod)                 ;
      if(t2 == 0)
      {
        printf("Erreur de saisie\n")               ;
        while((poubelle=getchar()) != '\n')        ;
      }
    }
    switch (choix_mod)
    {
      case 1:
        modif_resa()                               ;
        t1 = 1                                     ;
        break                                      ;
      case 2:
        annul_origine()                            ;
        t1 = 1                                     ;
        break                                      ;
      case 3:
        t1 = 1                                     ;
        printf("Retour au menu principal\n")       ;
        break                                      ;
      default:
        mauvais_choix(choix_mod)                   ;
        t2 = 0                                     ;
        break                                      ;
    }
  }
}

/*############################################
#                                            #
#             modif_resa                     #
#                                            #
##############################################

Appelée par choix_modif_resa(), passe la main au menu principal.
Modification d'une réservation : partie dates et client

*/
void modif_resa()
{
  char nom_client[100], prenom_client[200], numero_telephone_client[20]               ;
  int jour_debut, mois_debut, annee_debut                                             ;
  int jour_fin, mois_fin, annee_fin                                                   ;
  int a, m, j, t1, t2                                                                 ;
  char datearriveeavant[10], datedepartavant[11], temporaire[5], choix_modif_chambre  ;
  int continu_modif                                                                   ;

  datearriveeavant[0] = '\0'                              ;
  temporaire[0] = '\0'                                    ;
  a = demande.datearrivee / 10000                         ;
  m = (demande.datearrivee - (a * 10000)) / 100           ;
  j = (demande.datearrivee - (a * 10000) - (m * 100))     ;
  sprintf(temporaire, "%d", j)                            ;
  strcat(datearriveeavant, temporaire)                    ;
  strcat(datearriveeavant, "/")                           ;
  sprintf(temporaire, "%d", m)                            ;
  strcat(datearriveeavant, temporaire)                    ;
  strcat(datearriveeavant, "/")                           ;
  sprintf(temporaire, "%d", a)                            ;
  strcat(datearriveeavant, temporaire)                    ;


  t1 = 0                                                                                ;
  t2 = 0                                                                                ;
  while (t1 == 0)
  {
    printf("Date actuelle de début  : %s\n", datearriveeavant)                          ;
    printf("Saisir la nouvelle date de début (jj/mm/aaaa) : ")                          ;
    t2 = scanf("%d/%d/%d", &jour_debut, &mois_debut, &annee_debut)                      ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                                      ;
    }
    else
    {
      demande.datearrivee = jjmmaaaa_vers_aaaammjj(jour_debut, mois_debut, annee_debut) ;
      if((demande.datearrivee < calendrier[0].date) || (demande.datearrivee > calendrier[ANNEE].date))
      {
        printf("Date fausse.\n")                                                        ;
      }
      else
      {
        t1 = 1                                                                          ;
      }
    }
  }


  datedepartavant[0] = '\0'                              ;
  temporaire[0] = '\0'                                   ;
  a = demande.datedepart / 10000                         ;
  m = (demande.datedepart - (a * 10000)) / 100           ;
  j = (demande.datedepart - (a * 10000) - (m * 100))     ;
  sprintf(temporaire, "%d", j)                           ;
  strcat(datedepartavant, temporaire)                    ;
  strcat(datedepartavant, "/")                           ;
  sprintf(temporaire, "%d", m)                           ;
  strcat(datedepartavant, temporaire)                    ;
  strcat(datedepartavant, "/")                           ;
  sprintf(temporaire, "%d", a)                           ;
  strcat(datedepartavant, temporaire)                    ;

  t1 = 0                                                                         ;
  t2 = 0                                                                         ;
  while (t1 == 0)
  {
    printf("Date actuelle de fin  : %s\n", datedepartavant)                      ;
    printf("Saisir la date de la dernière nuitée (jj/mm/aaaa) : ")               ;
    t2 = scanf("%d/%d/%d", &jour_fin, &mois_fin, &annee_fin)                     ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                               ;
    }
    else
    {
      demande.datedepart = jjmmaaaa_vers_aaaammjj(jour_fin, mois_fin, annee_fin) ;
      if((demande.datedepart < demande.datearrivee)||(demande.datedepart > calendrier[ANNEE].date))
      {
        printf("Date fausse.\n")                                                 ;
      }
      else
      {
        t1 = 1                                                                   ;
      }
    }
  }


  choix_modif_chambre='a'                               ;
  while ((choix_modif_chambre!='o')||(choix_modif_chambre!='n'))
  {
    printf("Voulez-vous modifier la chambre (o/n)?\n")  ;
    scanf("%c", &choix_modif_chambre)                   ;
    if((choix_modif_chambre!='o')&&(choix_modif_chambre!='n'))
    {
      while((poubelle=getchar()) != '\n')               ;
      printf("Choix non valide\n")                      ;
    }
  }
  if (choix_modif_chambre=='o')
  {
    modif_resa_cha();
  }
  rech_periode(demande.datearrivee, demande.datedepart) ;
  calcul_nuitees()                                      ;
  if ((numcase_resa_date_debut != NON_TROUVE) && (numcase_resa_date_fin != NON_TROUVE))
  {
    continu_modif=choix_chambre()                       ;
  }

  if (continu_modif==1)
  {
    printf("Nom actuel du client : %s\n", demande.nomclient)       ;
    printf("Nouveau nom du client : ")                             ;
    scanf("%s", demande.nomclient)                                 ;
    printf("Prénom actuel du client : %s\n", demande.prenomclient) ;
    printf("Nouveau prénom du client : ")                          ;
    scanf("%s", demande.prenomclient)                              ;
    printf("Numéro de téléphone actuel : %s\n", demande.telclient) ;
    printf("Nouveau numéro de téléphone (format +3301234567): ")   ;
    scanf("%s", demande.telclient)                                 ; /* cf test préconisé dans saisie_client() */

    /* s'il y a une différence dans le montant, il faut un paiement */
  }
  else
  {
    printf("Abandon de la procédure de modification.\nRetour au menu principal\n") ;
  }
}



/*############################################
#                                            #
#              modif_resa_cha                #
#                                            #
##############################################

Appelée par modif_resa() lui passe également la main.
Modification de la réservation : partie chambres

*/
void modif_resa_cha()
{
  int cible_cat_chambre             ; /* 0 chambre, 1 suite*/
  int cible_type_lits               ; /* 1 lit simple, 2 deux lits simples, 3 lit double, 4 trois lits simples, 5 lit simple et lit double */
  int cible_balcon                  ; /* 0 pas, 1 balcon, 2 indifférent*/
  int cible_vue                     ; /* 0 pas, 1 vue, 2 indifférent*/
  int cible_bain                    ; /* 0 pas, 1 douche, 2 indifférent*/
  int cible_fumeur                  ; /* 0 pas, 1 fumeur, 2 indifférent*/
  int cible_animaux                 ; /* 0 pas, 1 animaux acceptés, 2 indifférent*/
  int test = 0                      ;

  printf("Saisir les critères de la chambre souhaitée :   ")  ;
  printf("Type actuel de chambre : %d \n", chambre.type_lits) ;
  cible_type_lits = 999                                       ;
  while((cible_type_lits < 1) || (cible_type_lits > 5))
  {
    printf("Nouveau type de chambre : ")            ;
    printf("1 - un lit simple\n")                   ;
    printf("2 - deux lits simples\n")               ;
    printf("3 - un lit double\n")                   ;
    printf("4 - trois lits simples\n")              ;
    printf("5 - un lit double et un lit simple\n")  ;
    printf("Choix : ")                              ;
    test = scanf("%d", &cible_type_lits)            ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                 ;
      while((poubelle=getchar()) != '\n')           ;
    }
    else
    {
      if((cible_type_lits < 1) || (cible_type_lits > 5))
      {
        printf("Choix non valide.\n")               ;
      }
    }
  }
  chambre.type_lits = cible_type_lits               ;

  switch(cible_type_lits)
  {
    case 1 :
      chambre.type_chambre = 1                      ;
      break                                         ;
    case 2 :
    case 3 :
      chambre.type_chambre = 2                      ;
      break                                         ;
    case 4 :
    case 5 :
      chambre.type_chambre = 3                      ;
      break                                         ;
  }

  cible_cat_chambre = 999                           ;
  while((cible_cat_chambre < 0) || (cible_cat_chambre > 1))
  {
    printf("Catégorie :")                           ;
    printf("0 - chambre\n")                         ;
    printf("1 - suite\n")                           ;
    printf("Actuellement : %d\n", chambre.categorie_chambre) ;
    printf("Choix : ")                              ;
    test = scanf("%d", &cible_cat_chambre)          ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                 ;
      while((poubelle=getchar()) != '\n')           ;
    }
    else
    {
      if((cible_cat_chambre < 0) || (cible_cat_chambre > 1))
      {
        printf("Choix non valide.\n")               ;
      }
    }
  }
  chambre.categorie_chambre = cible_cat_chambre     ;

  cible_balcon = 999                                ;
  while((cible_balcon < 0) || (cible_balcon > 1))
  {
    printf("Balcon :")                              ;
    printf("0 - pas de balcon\n")                   ;
    printf("1 - balcon\n")                          ;
    scanf("%d", &cible_balcon)                      ;
    printf("Actuellement : %d\n", chambre.balcon);
    printf("Choix : ")                              ;
    test = scanf("%d", &cible_balcon)               ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                 ;
      while((poubelle=getchar()) != '\n')           ;
    }
    else
    {
      if((cible_balcon < 0) || (cible_balcon > 1))
      {
        printf("Choix non valide.\n")               ;
      }
    }
  }
  chambre.balcon = cible_balcon                     ;

  cible_bain = 999                                  ;
  while((cible_bain < 0) || (cible_bain > 1))
  {
    printf("Salle de bain : ")                      ;
    printf("0 - baignoire\n")                       ;
    printf("1 - douche\n")                          ;
    printf("Actuellement : %d\n", chambre.bain)     ;
    printf("Choix : ")                              ;
    test = scanf("%d", &cible_bain)                 ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                 ;
      while((poubelle=getchar()) != '\n')           ;
    }
    else
    {
      if((cible_bain < 0) || (cible_bain > 1))
      {
        printf("Choix non valide.\n")               ;
      }
    }
  }
  chambre.bain = cible_bain                         ;

  cible_vue = 999                                   ;
  while((cible_vue < 0) || (cible_vue > 1))
  {
    printf("Vue :")                                 ;
    printf("0 - pas de vue\n")                      ;
    printf("1 - vue\n")                             ;
    printf("Actuellement : %d\n", chambre.vue)      ;
    printf("Choix : ")                              ;
    test = scanf("%d", &cible_vue)                  ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                 ;
      while((poubelle=getchar()) != '\n')           ;
    }
    else
    {
      if((cible_vue < 0) || (cible_vue > 1))
      {
        printf("Choix non valide.\n")               ;
      }
    }
  }
  chambre.vue = cible_vue                           ;

  cible_fumeur = 999                                ;
  while((cible_fumeur < 0) || (cible_fumeur > 1))
  {
    printf("Fumeur :")                              ;
    printf("0 - non fumeur\n")                      ;
    printf("1 - fumeur\n")                          ;
    printf("Actuellement : %d\n", chambre.fumeur)   ;
    printf("Choix : ")                              ;
    test = scanf("%d", &cible_fumeur)               ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                 ;
      while((poubelle=getchar()) != '\n')           ;
    }
    else
    {
      if((cible_fumeur < 0) || (cible_fumeur > 1))
      {
        printf("Choix non valide.\n")               ;
      }
    }
  }
  chambre.fumeur = cible_fumeur                     ;

  cible_animaux = 999                               ;
  while((cible_animaux < 0) || (cible_animaux > 1))
  {
    printf("Animaux :")                             ;
    printf("0 - animaux non autorisés\n")           ;
    printf("1 - animaux autorisés\n")               ;
    printf("Actuellement : %d \n", chambre.animaux) ;
    printf("Choix : ")                              ;
    test = scanf("%d", &cible_animaux)              ;
    if(test == 0)
    {
      printf("Erreur de saisie.\n")                 ;
      while((poubelle=getchar()) != '\n')           ;
    }
    else
    {
      if((cible_animaux < 0) || (cible_animaux > 1))
      {
        printf("Choix non valide.\n")               ;
      }
    }
  }
  chambre.animaux = cible_animaux                   ;
}

/*############################################
#                                            #
#              annul_origine                 #
#                                            #
##############################################

Appelé dans choix_modif_resa(), ensuite on est redirigé vers le menu principal.
Vérifie l'origine de l'annulation appelle le sous-programme nécessaire.
Origine de l'annulation d'une réservation
*/
void annul_origine()
{
  int raison_annul = 0, i=0, t1= 0, t2                           ;

  while(t1 == 0)
  {
    printf("Qui est à l'origine de la demande d'annulation ?\n") ;
    printf("-1- Décision de l'hôtel\n")                          ;
    printf("-2- Décision du client\n")                           ;
    printf("-9- Quitter et revenir au menu précédent\n")         ;
    printf("Choix : ")                                           ;
    t2 = scanf("%d", &raison_annul)                              ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                               ;
      while((poubelle=getchar()) != '\n')                        ;
    }
    switch (raison_annul)
    {
      case 1:
        remboursement()                                          ;
        annulation_resa_planning()                               ;
        supprime_resa(demande.code_resa)                         ;
        supprime_note(demande.code_resa)                         ;
        t1 = 1                                                   ;
        break                                                    ;
      case 2:
        annul_client()                                           ;
        annulation_resa_planning()                               ;
        supprime_resa(demande.code_resa)                         ;
        supprime_note(demande.code_resa)                         ;
        t1 = 1                                                   ;
        break;
      case 9:
        printf("Abandon.\n Retour au menu principal\n")          ;
        t1 = 1                                                   ;
        break                                                    ;
      default:
        mauvais_choix(raison_annul)                              ;
        break                                                    ;
    }
  }
}


/*############################################
#                                            #
#               annul_client                 #
#                                            #
##############################################

Appelée par annul_origine().
Appelle remboursement_partiel() en lui fournissant le pourcentage de remboursement.
Après retour dans annul_origine() et donc le menu principal.
Calcul du préavis afin de rembourser la réservation.

*/

void annul_client()
{
  char c                                                                   ;
  int t1 = 0, t2, trouve = 0, deja= 0, i = 0, j = 0                        ;
  long unsigned int nb_resa_annul                                          ;

  while((trouve == 0 ) || (i < ANNEE))
  {
    if(planning[j][i] == demande.code_resa) /* i sera l'indice du calendrier et indiquera donc les jours de préavis*/
    {
      trouve = 1                                                           ;
    }
    else
    {
      if(j < MAX_NB_CHAMBRES)
      {
        j++                                                                ;
      }
      else
      {
        j = 0                                                              ;
        i++                                                                ;
      }
    }
  }
  if(trouve == 0)
  {
    printf("La réservation est déjà passée.\n")                            ;
  }
  else
  {
    while((poubelle=getchar()) != '\n')                                    ;
    printf("L'annulation a-t-elle été demandée aujourd'hui (o/n) ?")       ;
    scanf("%c", &c)                                                        ;
    if(c != 'o')
    {
      if(c == 'n')
      {
        while((poubelle=getchar()) != '\n')                                ;
        printf("Il y a combien de jours ? ")                               ;
        t2 = scanf("%d", &deja)                                            ;
        if(t2 == 0)
        {
          printf("Erreur de saisie.\n")                                    ;
          while((poubelle=getchar()) != '\n')                              ;
        }
      }
      else
      {
        printf("Choix non accepté. Les choix possibles sont 'o' pour 'oui' ou 'n' pour 'non'.\n") ;
        while((poubelle=getchar()) != '\n')                                ;
      }
    }
    i += deja                                                              ;
    switch(i) /* y en a marre des if */
    {
      case 1: case 2: case 3: case 4: case 5: case 6:
        while((poubelle=getchar()) != '\n')                                ;
        printf("Annullation la dernière semaine : aucun remboursement.\n") ;
        printf("Êtes-vous sûr de vouloir annuler la réservation (o/N) ? ") ;
        scanf("%c", &c)                                                    ;
        if(c == 'o' )
        {
          annulation_resa_planning()                                       ;
          t1 = 1                                                           ;
        }
        break                                                              ;
      case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14:
        printf("La réservation sera remboursée à hauteur de trente pour cent.\n") ;
        remboursement_partiel(REMB_SEMAINE)                                ;
        annulation_resa_planning()                                         ;
        t1 = 1                                                             ;
        break                                                              ;
        default:
        printf("La réservation sera remboursée à hauteur de soixante-dix pour cent.\n") ;
        remboursement_partiel(REMB_QUINZE)                                 ;
        t1 = 1                                                             ;
        break                                                              ;
    }
  }
}

/*############################################
#                                            #
#               remboursement                #
#                                            #
##############################################

En cas de remboursement total.

Appelée par annul_origine(), y retourne et passe la main à annulation_resa_planning().
À partir du code_resa, on supprime la case dans le planning: on remet à 0

*/

void remboursement()
{
  char paiement[9] ;
  FILE *f1         ;
  int t1 = 0, t2   ;


  switch (demande.mode_paiement)
  {
    case 1:
      strcpy(paiement, "espèces")                                              ;
      break                                                                    ;
    case 2:
      strcpy(paiement, "chèque")                                               ;
      break                                                                    ;
    case 3:
      strcpy(paiement, "CB")                                                   ;
      break                                                                    ;
    case 4:
      strcpy(paiement, "virement")                                             ;
      break                                                                    ;
    default:
      printf("La réservation contient des informations à vérifier\n")          ;
      break                                                                    ;
  }
  printf("Le montant total payé par le client, %s %s, a été de : %.2f\n", demande.prenomclient, demande.nomclient, demande.total_resa) ;
  printf("Le paiement a été effectué par %s\n", paiement)                      ;

  while(t1 == 0)
  {
    printf("Choisir le mode de remboursement de %.2f: \n", demande.total_resa) ;
    printf("-1- Espèces\n")                                                    ;
    printf("-2- Chèque\n")                                                     ;
    printf("-3- Carte bancaire\n")                                             ;
    printf("-4- Virement\n")                                                   ;
    printf("Choix : ")                                                         ;
    t2 = scanf("%d", &demande.mode_paiement)                                   ;
    if(t2 == 0)
    {
      printf("Erreur de saisie.\n")                                            ;
    }
    else
    {
      if((demande.mode_paiement < 0) || (demande.mode_paiement > 4))
      {
        mauvais_choix(demande.mode_paiement)                                   ;
      }
      else
      {
        if(demande.mode_paiement == 3)
        {
          paiement_cb()                                                        ;
        }
        printf("Le remboursement a bien été effectué.\n")                      ;
        t1 = 1                                                                 ;
      }
    }
  }
}


/*############################################
#                                            #
#           remboursement_partiel            #
#                                            #
##############################################

En cas de remboursement partiel.

Appelée par annul_origine(), y retourne et passe la main à annulation_resa_planning().
À partir du code_resa, on supprime la case dans le planning: on remet à 0

*/

void remboursement_partiel(int p_poursan)
{
  char paiement[9]                                                             ;
  FILE *f1                                                                     ;
  float combien                                                                ;
  int t1 = 0, t2                                                               ;


  switch (demande.mode_paiement)
  {
    case 1:
      strcpy(paiement, "espèces")                                              ;
      break                                                                    ;
    case 2:
      strcpy(paiement, "chèque")                                               ;
      break                                                                    ;
    case 3:
      strcpy(paiement, "CB")                                                   ;
      break                                                                    ;
    case 4:
      strcpy(paiement, "virement")                                             ;
      break                                                                    ;
    default:
      printf("La réservation contient des informations à vérifier\n")          ;
      break                                                                    ;
  }
  printf("Le montant total payé par le client, %s %s, a été de : %.2f\n", demande.prenomclient, demande.nomclient, demande.total_resa) ;
  printf("Le paiement a été effectué par %s\n", paiement)                      ;

  combien = (demande.total_resa * p_poursan) / 100                             ;
  while(t1 == 0)
  {
    printf("Choisir le mode de remboursement :\n")                             ;
    printf("-1- Espèces\n")                                                    ;
    printf("-2- Chèque\n")                                                     ;
    printf("-3- Carte bancaire\n")                                             ;
    printf("-4- Virement\n")                                                   ;
    printf("Choix : ")                                                         ;
    t2 = scanf("%d", &demande.mode_paiement)                                   ;
    if(t2 == 0)
    {
      printf("Erreur de saisie.\n")                                            ;
    }
    else
    {
      if((demande.mode_paiement < 0) || (demande.mode_paiement > 4))
      {
        mauvais_choix(demande.mode_paiement)                                   ;
      }
      else
      {
        if(demande.mode_paiement == 3)
        {
          paiement_cb()                                                        ;
        }
        printf("Le remboursement a bien été effectué.\n")                      ;
        t1 = 1                                                                 ;
      }
    }
  }
}

/*############################################
#                                            #
#              PARTIE NOTES                  #
#       #############################        #
#                                            #
#              creation_note                 #
#                                            #
##############################################

Création de la note, vide. Appellée dans creer_reservation.

*/
void creation_note()
{
  char entree_note[20], temporaire[11]          ;
  FILE *f1                                      ;

  temporaire[0] = '\0'                          ;
  entree_note[0] = '\0'                         ;
  strcat(entree_note, DOSSIER_NOTES)            ;
  sprintf(temporaire, "%lu", demande.code_resa) ;
  strcat(entree_note, temporaire)               ;
  strcat(entree_note, ".txt")                   ;
  f1=fopen(entree_note, "w")                    ;
  fprintf(f1, "%s %s\n", demande.nomclient, demande.prenomclient) ; /*Pour avoir accès à toutes les informations rapidement et de manière indépendante de la réservation, on met dans la première ligne du fichier nom et prénom du client*/
  fclose(f1)                                    ;
}


/*############################################
#                                            #
#           menu_recherche_note              #
#                                            #
##############################################

Appelée dans le menu principal.

*/
void menu_recherche_note()
{
  int t1 = 0, t2 = 0, choix_recherche_note   ;
  printf("Menu de recherche d'une note\n")   ;
  while(t1 == 0)
  {
    printf("-1- Recherche de note par numéro de réservation\n") ;
    printf("-2- Recherche de note par numéro de chambre (uniquement pour les réservations en cours)\n") ;
    printf("-3- Retour au menu principal\n") ;
    printf("Choix : ")                       ;
    t2 = scanf("%d", &choix_recherche_note)  ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")           ;
      while((poubelle=getchar()) != '\n')    ;
    }
    switch (choix_recherche_note)
    {
      case 1:
        recherche_note_num()                 ;
        t1 = 1                               ;
        break                                ;
      case 2:
        recherche_note_cha()                 ;
        t1 = 1                               ;
        break                                ;
      case 3:
        printf("Retour au menu principal\n") ;
        t1 = 1                               ;
        break                                ;
      default:
        mauvais_choix(choix_recherche_note)  ;
        break                                ;
    }
  }
}

/*############################################
#                                            #
#           recherche_note_cha               #
#                                            #
##############################################

Appelée par menu_recherche_note().
Après son éxecution on revient au menu principal.

On demande la saisie du numéro de chambre, on récupère le numéro de réservation correspondante. Il sera passé à menu_choix_note.

*/

void recherche_note_cha()
{
  char temporaire[11], entree_note[20], client_nom[MAX_NOM_CLI], client_prenom[MAX_PNOM_CLI], confirmation;
  FILE *f1                                        ;
  int rech_note_chambre, t1 = 0, t2               ;
  long unsigned int id_resa                       ;

  while(t1 == 0)
  {
    printf("Saisir le numéro de la chambre : " )  ;
    t2 = scanf("%d", &rech_note_chambre)          ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                ;
      while((poubelle=getchar()) != '\n')         ;
    }
    else
    {
      t1 = 1                                      ;
    }
  }

  if ((planning[rech_note_chambre][0]==0) || (planning[rech_note_chambre][0]==1))
  {
    printf("Erreur: la chambre n'est pas réservée.\n");
    menu_recherche_note()                         ;
  }
  else
  {
   id_resa = planning[rech_note_chambre][0]       ;
   temporaire[0] = '\0'                           ;
   entree_note[0] = '\0'                          ;
   strcat(entree_note, DOSSIER_NOTES)             ;
   sprintf(temporaire, "%lu", id_resa) ;
   strcat(entree_note, temporaire)                ;
   strcat(entree_note, ".txt")                    ;
   f1=fopen(entree_note, "r")                     ;

   fscanf(f1, "%s %s", client_nom, client_prenom) ;
   fclose(f1)                                     ;

   printf("Réservation de %s %s\n", client_prenom, client_nom) ;
   t2 = 0                                         ;
   while(t2 == 0)
   {
     while((poubelle=getchar()) != '\n')          ;
     printf("Vous confirmez (o/n) ? ")            ;
     scanf("%c", &confirmation)                   ;
     if((confirmation != 'o') && (confirmation != 'n'))
     {
       printf("Erreur de saisie\n")               ;
     }
     else
     {
       t2 = 1                                     ;
     }
   }
   if(confirmation == 'n')
   {
     printf("Retour au menu principal\n")         ;
   }
   else
   {
     menu_choix_note(entree_note)                 ;
   }
  }
}

/*############################################
#                                            #
#           recherche_note_num               #
#                                            #
##############################################
On demande la saisie du numéro de réservation, on le passe à menu_choix_note.
*/

void recherche_note_num()
{
  char temporaire[11], entree_note[20], client_nom[MAX_NOM_CLI], client_prenom[MAX_PNOM_CLI], confirmation;
  FILE *f1                                        ;
  int t1 = 0, t2                                  ;
  long unsigned int id_resa                       ;

  while(t1 == 0)
  {
    printf("Saisir le numéro de réservation : " ) ;
    t2 = scanf("%lu", &id_resa)                   ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                ;
      while((poubelle=getchar()) != '\n')         ;
    }
    else
    {
      t1 = 1                                      ;
    }
  }

  temporaire[0] = '\0'                            ;
  entree_note[0] = '\0'                           ;
  strcat(entree_note, DOSSIER_NOTES)              ;
  sprintf(temporaire, "%lu", id_resa)             ;
  strcat(entree_note, temporaire)                 ;
  strcat(entree_note, ".txt")                     ;
  f1=fopen(entree_note, "r")                      ;

  fscanf(f1, "%s %s", client_nom, client_prenom)  ;
  fclose(f1)                                      ;

  printf("Réservation de %s %s\n", client_prenom, client_nom) ;
  t2 = 0                                          ;
  while(t2 == 0)
  {
    while((poubelle=getchar()) != '\n')           ;
    printf("Vous confirmez (o/n) ? ")             ;
    scanf("%c", &confirmation)                    ;
    if((confirmation != 'o') && (confirmation != 'n'))
    {
      printf("Erreur de saisie\n")                ;
    }
    else
    {
      t2 = 1                                      ;
    }
  }
  if(confirmation == 'n')
  {
    printf("Retour au menu de recherche d'une note\n");
    menu_recherche_note()                         ;
  }
  else
  {
    menu_choix_note(entree_note)                  ;
  }
}

/*############################################
#                                            #
#              menu_choix_note               #
#                                            #
##############################################
Récupère le numéro d'une réservation, trouve le fichier correspondant, et propose des actions à l'utilisateur.
*/

void menu_choix_note(char p_entree_note[])
{
  int t1, t2, choix_note                                 ;
  FILE *f1                                               ;

  while(t1 == 0)
  {
    printf("-1- Afficher la note\n")                     ;
    printf("-2- Ajouter une entrée sur la note\n")       ;
    printf("-3- Régler la note\n")                       ;
    printf("-4- Quitter et revenir au menu principal\n") ;
    printf("Choix : ")                                   ;
    t2 = scanf("%d", &choix_note)                        ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                       ;
    }
    else
    {
      switch(choix_note)  /* Passe p_entree_note au programme suivant, selon le choix. */
      {
        case 1:
          affichage_note(p_entree_note)        ;
          break                                ;
        case 2:
          ajout_note(p_entree_note)            ;
          break                                ;
        case 3:
          paiement_note(p_entree_note)         ;
          break                                ;
        case 4:
          printf("Retour précédent\n")         ;
          t1 = 1                               ;
          break                                ;
      }
    }
  }
}

/*############################################
#                                            #
#              affichage_note                #
#                                            #
##############################################

Appelée par recherche_note, prend en argument le nom du fichier de la note (num_note).
Sortie par un menu de choix.
*/

void affichage_note(char p_entree_note[])
{
  char note_nom_cli[MAX_NOM_CLI], note_pnom_cli[MAX_PNOM_CLI] ;
  char date_chaine[11], temporaire[5]                         ;
  FILE *f1                                                    ;
  float note_total = 0                                        ;
  int i = 0, k, a, m, j                                       ;
  struct frais note[MAX_ENTREES_FRAIS]                        ;

  f1=fopen(p_entree_note, "r")                                ;
  fscanf(f1, "%s %s\n", note_nom_cli, note_pnom_cli) ; /*Pour avoir accès à toutes les informations rapidement et de manière indépendante de la réservation, on met dans la première ligne du fichier nom et prénom du client*/
  while(!feof(f1))
  {
    fscanf(f1, "%lu %s %f", &note[i].datefrais, note[i].nomfrais, &note[i].montantfrais) ;
    note_total += note[i].montantfrais                 ;
    i++                                                ;
  }
  fclose(f1)                                           ;

  for(k = 0 ; i < k ; j++)
  {
    date_chaine[0] = '\0'                              ;
    temporaire[0] = '\0'                               ;
    a = note[i].datefrais/10000                        ;
    m = (note[i].datefrais - (a * 10000)) / 100        ;
    j = (note[i].datefrais - (a * 10000) - (m * 100))  ;
    sprintf(temporaire, "%d", j)                       ;
    strcat(date_chaine, temporaire)                    ;
    strcat(date_chaine, "/")                           ;
    sprintf(temporaire, "%d", m)                       ;
    strcat(date_chaine, temporaire)                    ;
    strcat(date_chaine, "/")                           ;
    sprintf(temporaire, "%d", a)                       ;
    strcat(date_chaine, temporaire)                    ;

    printf("%s %s %.2f", date_chaine, note[i].nomfrais, note[i].montantfrais) ;
  }
  printf("Le montant total de la note est : %.2f €.\n", note_total) ;
  if(i == 1000)
  {
    printf("La note est pleine et doit être réglée.") ;
    paiement_note(p_entree_note)                      ;
  }
  menu_choix_note(p_entree_note)                      ;
}

/*############################################
#                                            #
#                ajout_note                  #
#                                            #
##############################################
Ajoute une entrée depuis le catalogue des services.
Prend en paramètre l'identifiant d'une note.
*/

void ajout_note(char p_entree_note[])
{
  char date[11], note_nom_cli[MAX_NOM_CLI], note_pnom_cli[MAX_PNOM_CLI] ;
  int t1 = 0, t2, ajout_frais, j, i=0                              ;
  FILE *f1                                                         ;
  struct entree_service ajout                                      ;
  struct frais note[MAX_ENTREES_FRAIS]                             ;

  affichage_catalogue()                                            ;

  while(t1 == 0)
  {
    printf("Saisir le numéro du service à ajouter sur la note : ") ;
    t2 = scanf("%d", &ajout_frais)                                 ;
    if((t2 == 0) || (ajout_frais > nb_services))
    {
      printf("Erreur de saisie.\n")                                ;
      while((poubelle=getchar()) != '\n')                          ;
    }
    else
    {
      t1 = 1                                                       ;
    }
  }

  ajout = catalogue_services[ajout_frais]                          ;

  f1=fopen(p_entree_note, "r")                                     ;
  fscanf(f1, "%s %s\n", note_nom_cli, note_pnom_cli)               ; /*Pour avoir accès à toutes les informations rapidement et de manière indépendante de la réservation, on met dans la première ligne du fichier nom et prénom du client*/
  while(!feof(f1))
  {
    fscanf(f1, "%lu %s %f", &note[i].datefrais, note[i].nomfrais, &note[i].montantfrais) ;
    i++                                                            ;
  }
  fclose(f1)                                                       ;

  note[i].datefrais = calendrier[0].date                           ;
  strcpy(note[i].nomfrais, ajout.nom_service)                      ;
  note[i].montantfrais = ajout.prix_service                        ;

  i++                                                              ;
  j = 0                                                            ;

  f1=fopen(p_entree_note, "w")                                     ;
  fprintf(f1, "%s %s\n", note_nom_cli, note_pnom_cli) ;
  while(j < i)
  {
    fprintf(f1, "%lu %s %f", note[j].datefrais, note[j].nomfrais, note[j].montantfrais) ;
    j++                                                            ;
  }
  fclose(f1)                                                       ;
  printf("Le service a bien été ajouté à la note. \n")             ;
}

/*############################################
#                                            #
#              paiement_note                 #
#                                            #
##############################################
Prend en paramètre le nom d'une note.
Teste si la note existe. Si non, renvoie un message d'erreur.
Autrement elle récupère le total, demande les moyens de paiement et des informations.
Le paiement est simulé et sauvegardé.
*/
void paiement_note(char p_entree_note[])
{
  char note_nom_cli[MAX_NOM_CLI], note_pnom_cli[MAX_PNOM_CLI] ;
  char date_chaine[11], temporaire[5]                         ;
  FILE *f1                                                    ;
  float note_total = 0                                        ;
  int i = 0, k, a, m, j, test=0, t2                           ;
  struct frais note[MAX_ENTREES_FRAIS]                        ;


  f1=fopen(p_entree_note, "r")                                ;
  fscanf(f1, "%s %s\n", note_nom_cli, note_pnom_cli)          ; /*Pour avoir accès à toutes les informations rapidement et de manière indépendante de la réservation, on met dans la première ligne du fichier nom et prénom du client*/
  while(!feof(f1))
  {
    fscanf(f1, "%lu %s %f", &note[i].datefrais, note[i].nomfrais, &note[i].montantfrais) ;
    note_total += note[i].montantfrais                 ;
    i++                                                ;
  }
  fclose(f1)                                           ;

  for(k = 0 ; i < k ; j++)
  {
    date_chaine[0] = '\0'                              ;
    temporaire[0] = '\0'                               ;
    a = note[i].datefrais/10000                        ;
    m = (note[i].datefrais - (a * 10000)) / 100        ;
    j = (note[i].datefrais - (a * 10000) - (m * 100))  ;
    sprintf(temporaire, "%d", j)                       ;
    strcat(date_chaine, temporaire)                    ;
    strcat(date_chaine, "/")                           ;
    sprintf(temporaire, "%d", m)                       ;
    strcat(date_chaine, temporaire)                    ;
    strcat(date_chaine, "/")                           ;
    sprintf(temporaire, "%d", a)                       ;
    strcat(date_chaine, temporaire)                    ;

    printf("%s %s %.2f", date_chaine, note[i].nomfrais, note[i].montantfrais) ;
  }
  printf("Le montant total de la note est : %.2f €.\n", note_total)           ;

  while(test == 0)
  {

    printf("Choisir le mode de paiement: \n")              ;
    printf("-1- Espèces\n")                                ;
    printf("-2- Chèque\n")                                 ;
    printf("-3- Carte bancaire\n")                         ;
    printf("-4- Virement\n")                               ;
    printf("Choix : ")                                     ;
    t2 = scanf("%d", &demande.mode_paiement)               ;
    if(t2 == 0)
    {
      printf("Erreur de saisie.\n")                        ;
    }
    else
    {
      if((demande.mode_paiement < 0) || (demande.mode_paiement > 4))
      {
        mauvais_choix(demande.mode_paiement)               ;
      }
      else
      {
        if(demande.mode_paiement == 3)
        {
          paiement_cb()                                    ;
        }
        printf("Le paiement a bien été effectué.\n")       ;
        test = 1                                           ;
       }
     }
   }
   recreation_note(p_entree_note);
}


/*############################################
#                                            #
#              recreation_note               #
#                                            #
##############################################
Ecrase ancienne note.

*/
void recreation_note(char p_entree_note[])
{
  char note_nomclient[MAX_NOM_CLI], note_prenomclient[MAX_PNOM_CLI] ;
  FILE *f1                                                          ;

  f1=fopen(p_entree_note, "r")                                      ;
  fscanf(f1, "%s %s", note_nomclient, note_prenomclient)            ;
  fclose(f1)                                                        ;

  f1=fopen(p_entree_note, "w")                                      ;
  fprintf(f1, "%s %s\n", note_nomclient, note_prenomclient)         ;
  fclose(f1)                                                        ;
}

/*############################################
#                                            #
#               supprime_note                #
#                                            #
##############################################

Appelée par depart() ou annul_origine().
Supprime la note.

*/

void supprime_note(long unsigned int p_code_resa)
{
  char entree_note[20], temporaire[11]                             ;
  int test                                                         ;

  temporaire[0] = '\0'                                             ;
  entree_note[0] = '\0'                                            ;
  strcat(entree_note, DOSSIER_NOTES)                               ;
  sprintf(temporaire, "%lu", p_code_resa)                          ;
  strcat(entree_note, temporaire)                                  ;
  strcat(entree_note, ".txt")                                      ;

  test = unlink(entree_note)                                       ;
  if(test == -1)
  {
    printf("Erreur dans la suppression du fichier\n")              ;
    printf("Supprimez manuellemenet le fichier %s\n", entree_note) ;
  }
}

/*############################################
#                                            #
#              PARTIE SERVICES               #
#       #############################        #
#                                            #
#           catalogue_services_menu          #
#                                            #
##############################################

Appelé dans le main()
Menu qui affiche les choix concernant les services de l'hôtel.


*/
void catalogue_services_menu()
{
  int choix_cat = 0                                        ;
  while(choix_cat != 9)
  {
    printf("\n")                                           ;
    printf("    CATALOGUE DES SERVICES\n\n")               ;
    printf("-1- Lire le catalogue des services\n")         ;
    printf("-2- Modifier le catalogue des services\n")     ;
    printf("-3- Ajouter un service\n")                     ;
    printf("-9- Quitter et revenir au menu principal\n\n") ;

    printf("Choisissez une action : ")                     ;
    scanf("%d", &choix_cat)                                ;
    switch(choix_cat)
    {
      case 1:
        affichage_catalogue()                ;
        break                                ;
      case 2:
        modif_services()                     ;
        break                                ;
      case 3:
        saisie_services()                    ;
        break                                ;
      case 9:
        printf("Retour au menu principal\n") ;
        break                                ;
      default:
        mauvais_choix(choix_cat)             ;
        while((poubelle=getchar()) != '\n')  ;
        break                                ;
    }
  }
}

/*############################################
#                                            #
#             modif_services                 #
#                                            #
##############################################

Modifie les services déjà saisis.

*/
void modif_services()
{
  char nom_serv[MAX_NOM_SERVICE] ;
  float prix_serv                ;
  struct entree_service service  ;
  int i = 0                      ;
  if (nb_services== 0)
  {
    printf("Il n'y a actuellement aucun service dans le catalogue pour la modification.\n") ;
  }
  else
  {
    strcpy(nom_serv, "début")                                           ;
    while(strcmp(nom_serv,"fin\0") != 0)
    {
      service = catalogue_services[i]                                   ;
      printf("Nom actuel du service : %s\n", service.nom_service)       ;
      printf("Nouveau nom ('fin' pour terminer) : ")                    ;
      scanf("%s", nom_serv)                                             ;
      printf("%s\n", nom_serv)                                          ;
      if (strcmp(nom_serv, "fin")== 0)
      {
        printf("Fin de la modification. Retour au menu précédent. \n")  ;
      }
      else
      {
        strcpy(service.nom_service, nom_serv)                           ;
        printf("Prix actuel du service : %.2f\n", service.prix_service) ;
        printf("Nouveau prix : ")                                       ;
        scanf("%f", &service.prix_service)                              ;
        catalogue_services[i]=service                                   ;
        i++                                                             ;
      }
    }
    sauv_catalogue_services=1                                           ;
  }
}


/*############################################
#                                            #
#           rech_service                     #
#                                            #
##############################################

Recherche d'un service

*/

int rech_services(char nom_serv_rech[MAX_NOM_SERVICE])
{
  struct entree_service service     ;
  int i, numserv = -1               ;
  for (i=0; i<MAX_CAT_SERV; i++)
  {
    service = catalogue_services[i] ;
    if (strcmp(service.nom_service, nom_serv_rech) ==0)
    {
      numserv = i                   ;
    }
  }
  if(numserv == -1)
  {
    return(NON_TROUVE)              ;
  }
  else
  {
    return(numserv)                 ;
  }
}

/*############################################
#                                            #
#           suppression_service              #
#                                            #
##############################################

Supprime un service


*/
void suppression_service()
{
  char nom_rech[MAX_NOM_SERVICE]                      ;
  int i = 0, numero=NON_TROUVE                        ;
  if (nb_services== 0)
  {
    printf("Il n'y a actuellement aucun service dans le catalogue pour la suppression.\n") ;
  }
  else
  {
    printf("Nom du service à supprimer : ")           ;
    scanf("%s", nom_rech)                             ;
    numero = rech_services(nom_rech)                  ;
    if (numero == NON_TROUVE)
      {
        printf("%s non trouvé \n", nom_rech)          ;
      }
    else
    {
      for (i=numero ; i<nb_services-1 ; i++)
      {
        catalogue_services[i]=catalogue_services[i+1] ;
      }
      nb_services--                                   ;
      printf("%s supprimé \n", nom_rech)              ;
      sauv_catalogue_services=1                       ;
    }
  }
}

/*############################################
#                                            #
#            saisie_services                 #
#                                            #
##############################################

Saisie par l'utilisateur de services.
Ils sont chargés dans un tableau après ceux déjà lus depuis le fichier.

*/
void saisie_services()
{
  struct entree_service service                                 ;
  int i = nb_services                                           ;

  strcpy(service.nom_service, "début")                          ;
  while(strcmp(service.nom_service,"fin") != 0)
  {
    printf("Saisir le nom du service ('fin' pour terminer) : ") ;
    scanf("%s", service.nom_service)                            ;
    if(strcmp(service.nom_service,"fin") == 0)
    {
      printf("Fin de la saisie. Retour au menu précédent.\n")   ;
    }
    else
    {
      printf("Saisir le prix : ")                               ; /*test pour éviter des prix avec virgule au lieu du point*/
      scanf("%f", &service.prix_service)                        ;
      printf("\n")                                              ;
      catalogue_services[i++] = service                         ;
    }
  }
  if(i > 0)
  {
    nb_services = i                                                  ;
    printf("%d services présents dans le catalogue.\n", nb_services) ;
    sauv_catalogue_services = 1                                      ;
    enreg_catalogue_services()                                       ;
  }
}

/*############################################
#                                            #
#         enreg_catalogue_services           #
#                                            #
##############################################

Enregistre le tableau dans le fichier CAT_SERVICES.
Affiche un message de confirmation.

*/

void enreg_catalogue_services(){
  FILE *f1                                                              ;
  f1 = fopen(CAT_SERVICES, "w")                                         ;
  int i                                                                 ;
  struct entree_service service                                         ;


  for(i = 0; i < nb_services ; i++)
  {
    service = catalogue_services[i]                                     ;
    fprintf(f1, "%s %.2f\n", service.nom_service, service.prix_service) ;
  }
  fclose(f1)                                                            ;
  sauv_catalogue_services = 0                                           ;
  printf("Le catalogue a bien été sauvegardé.\n")                       ;
}

/*############################################
#                                            #
#       chargement_catalogue_services        #
#                                            #
##############################################

Lit le fichier CAT_SERVICES et charge son contenu dans le tableau de services.

*/
void chargement_catalogue_services()
{
  FILE *f1                      ;
  struct entree_service service ;
  char test_sauv                ;
  int i                         ;

  f1 = fopen(CAT_SERVICES, "r") ;
  while(!feof(f1))
  {
    fscanf(f1, "%s %f\n", service.nom_service, &service.prix_service) ;
    catalogue_services[i] = service                                   ;
    i++                                                               ;
  }
  fclose(f1)                                                          ;
  nb_services = i                                                     ;
}


/*############################################
#                                            #
#           affichage_catalogue              #
#                                            #
##############################################

Affiche les données déjà chargées dans le tableau de services.


*/
void affichage_catalogue()
{
  struct entree_service service                                                 ;
  int i                                                                         ;
  if(nb_services == 0)
  {
    printf("Le catalogue des services est vide.\n")                             ;
  }
  else
  {
    printf("\n")                                                                ;
    for(i=0; i < nb_services; i++)
    {
      service = catalogue_services[i]                                           ;
      printf("%d %s : %.2f € \n", i, service.nom_service, service.prix_service) ;
    }
  }
}




/*############################################
#                                            #
#              PARTIE CHAMBRES               #
#       #############################        #
#                                            #
#             chargement_chambres            #
#                                            #
##############################################

Lit le fichier CHAMBRES et charge son contenu dans le tableau de chambres.

*/
void chargement_chambres()
{
  FILE *f1                  ;
  int i                     ;

  f1 = fopen(CHAMBRES, "r") ;
  for(i = 0; i < 50 ; i++)
  {
    fscanf(f1, "%d %d %d %d %d %d %d %d %d", &tab_chambres[i].num_chambre, &tab_chambres[i].type_chambre, &tab_chambres[i].categorie_chambre, &tab_chambres[i].type_lits, &tab_chambres[i].balcon, &tab_chambres[i].vue, &tab_chambres[i].bain, &tab_chambres[i].fumeur, &tab_chambres[i].animaux) ;
  }
  fclose(f1)                ;
}


/*############################################
#                                            #
#           rech_chambre                     #
#                                            #
##############################################

Rechercher les chambres par leur numéro.


*/
int rech_chambre(int par_rech)
{
  int i =0, numcase_chambre=NON_TROUVE ;
  while((i<MAX_NB_CHAMBRES)&&(numcase_chambre==NON_TROUVE))
  {
    chambre=tab_chambres[i]            ;
    if(chambre.num_chambre==par_rech)
    {
      numcase_chambre=i                ;
    }
    else
    {
      i++                              ;
    }
  }
  return numcase_chambre               ;
}

/*############################################
#                                            #
#           modification_chambre             #
#                                            #
##############################################

Modifier les chambres par la recherche de leur numéro.


*/
void modification_chambre(int res_chambre)
{
  int chambre_rech = 0                                    ;
  /*Résultat recherche chambres */
  /*printf("Entrez le numéro de la chambre à modifier : ");
  scanf("%d",&chambre_rech )                            ;
  res_chambre=rech_chambre(chambre_rech)                ;
  if (res_chambre == NON_TROUVE)
  {
    printf("Aucune modification: %d n'a pas été trouvé\n", chambre_rech);
  }
  else
  {
    chambre=tab_chambres[res_chambre];*/
    printf("Numéro actuel de la chambre : %d\n", chambre.num_chambre) ;
    printf("Nouveau numéro de la chambre : ")                         ;
    scanf("%d", &chambre.num_chambre)                                 ;

    printf("Type actuel de la chambre : %d\n", chambre.type_chambre)  ;
    printf("Nouveau type de chambre : ")                              ;
    scanf("%d", &chambre.type_chambre)                                ;

    printf("Type actuel de lits : %d\n", chambre.type_lits)           ;
    printf("Nouveau type de lits : ")                                 ;
    scanf("%d", &chambre.type_lits)                                   ;

    printf("Catégorie actuelle de la chambre : %d\n", chambre.categorie_chambre);
    printf("Nouvelle catégorie de chambre : ")                        ;
    scanf("%d", &chambre.categorie_chambre)                           ;

    printf("Type actuel de lits : %d\n", chambre.type_lits)           ;
    printf("Nouveau type de lits : ")                                 ;
    scanf("%d", &chambre.type_lits)                                   ;

    printf("Chambre avec balcon : %d\n", chambre.balcon)              ;
    printf("Chambre avec vue : ")                                     ;
    scanf("%d", &chambre.balcon)                                      ;

    printf("Chambre avec vue : %d\n", chambre.vue)                    ;
    printf("Chambre avec vue : ")                                     ;
    scanf("%d", &chambre.vue)                                         ;

    printf("Douche ou baignoire : %d\n", chambre.bain)                ;
    printf("Douche ou baignoire : ")                                  ;
    scanf("%d", &chambre.bain)                                        ;

    printf("Fumeur ou non : %d\n", chambre.fumeur)                    ;
    printf("Fumeur ou non : ")                                        ;
    scanf("%d", &chambre.fumeur)                                      ;

    printf("Animaux acceptés ou non : %d\n", chambre.animaux)         ;
    printf("Animaux ou non : ")                                       ;
    scanf("%d", &chambre.animaux)                                     ;
    tab_chambres[res_chambre] = chambre                               ;
}

/*############################################
#                                            #
#           enreg_chambre                    #
#                                            #
##############################################

Appelée dans un choix du menu principal.
Sauvegarde automatique lors de la modification des chambres.


*/
void enreg_chambre()
{
  FILE *f1                                                  ;
  f1 = fopen("CHAMBRES", "w")                               ;
  int i                                                     ;

  for(i = 0; i < MAX_NB_CHAMBRES ; i++)
  {
    chambre = tab_chambres[i]                               ;
    fprintf(f1, "%d %d %d %d %d %d %d %d %d %d\n", chambre.num_chambre, chambre.type_chambre, chambre.type_lits, chambre.categorie_chambre, chambre.type_lits, chambre.balcon, chambre.vue, chambre.bain, chambre.fumeur, chambre.animaux) ;
  }
  fclose(f1)                                                ;
  printf("La liste des chambres a bien été sauvegardée.\n") ;
}

/*############################################
#                                            #
#           affichage_chambre                #
#                                            #
##############################################

Appelée dans un choix du menu principal.
Affiche les données déjà chargées dans la liste des chambres pour les modifier ensuite.

*/
void affichage_chambre()
{
    printf("Numéro de la chambre : %d\n", chambre.num_chambre)       ;
    printf("Type de chambre : %d\n", chambre.type_chambre)           ;
    printf("Type de lits : %d\n", chambre.type_lits)                 ;
    printf("Catégorie de chambre: %d\n", chambre.categorie_chambre)  ;
    printf("Balcon: %d\n", chambre.vue)                              ;
    printf("Vue: %d\n", chambre.balcon)                              ;
    printf("Douche ou baignoire : %d\n", chambre.bain)               ;
    printf("Fumeurs ou non : %d\n", chambre.fumeur)                  ;
    printf("Animaux autorisés : %d\n\n", chambre.animaux)            ;
}

/*############################################
#                                            #
#                  travaux                   #
#                                            #
##############################################

Procédure pour déclarer des travaux

*/
void travaux()
{
  char temporaire[5], date_1[11]                                    ;
  int cible_num_chambre, t1=0, t2, i, libre, a, m, j, k             ;
  long unsigned int date_num                         ;

  printf("Déclaration de travaux.\n")                               ;
  cible_date()                                                      ;
  while (t1 == 0)
  {
    printf("Saisir le numéro de la chambre (0 pour quitter) : ")    ;
    t2 = scanf("%d", &cible_num_chambre)                            ;
    if(t2 == 0)  /* test de la saisie */
    {
      printf("Erreur de saisie.\n")                                 ;
      while((poubelle=getchar()) != '\n')                           ;
    }
    else
    {
      if(cible_num_chambre == 0) /* abandon */
      {
        printf("Abandon.\n")                                        ;
        printf("Retour au menu principal.\n")                       ;
        t1 = 1                                                      ;
      }
      else /* la saisie correspond-t-elle à une chambre?*/
      {
        i = 0                                                       ;
        t2 = 0                                                      ;
        while((t2 == 0) && (i < MAX_NB_CHAMBRES))
        {
          if(cible_num_chambre == tab_chambres[i].num_chambre)
          {
            t2 = 1                                                  ;
          }
          else
          {
            i++                                                     ;
          }
        }
        if(t2 == 0)
        {
          printf("Le choix fait ne correspond pas à une chambre existante.\n") ;
        }
        else
        {
          t1 = 2                                                    ; /* pour sortir de la boucle de saisie */
          demande.chambre_resa = cible_num_chambre                  ; /* Nous savons maintenant que la valeur saisie est celle d'une chambre, on la stocke dans la structure demande */
        }
      } /* sortie du test de chambre */
    }
  } /* sortie de la boucle de saisie */

  if(t1 == 2)
  {
  rech_periode(demande.datearrivee, demande.datedepart) ;
  k = numcase_resa_date_debut                           ;
  libre = 0                                             ;
  while(k < numcase_resa_date_fin + 1)
  {
    if(planning[i][k] != 0)
    {
      libre = 1                                         ;
      date_num = calendrier[k].date                     ;
      date_1[0] = '\0'                                  ;
      temporaire[0] = '\0'                              ;
      a = date_num/10000                                ;
      m = (date_num - (a * 10000)) / 100                ;
      j = (date_num - (a * 10000) - (m * 100))          ;
      sprintf(temporaire, "%d", j)                      ;
      strcat(date_1, temporaire)                        ;
      strcat(date_1, "/")                               ;
      sprintf(temporaire, "%d", m)                      ;
      strcat(date_1, temporaire)                        ;
      strcat(date_1, "/")                               ;
      sprintf(temporaire, "%d", a)                      ;
      strcat(date_1, temporaire)                        ;
      if(planning[i][k] == 1)
      {
        printf("Le %s la chambre est déjà en travaux\n", date_1);
      }
      else
      {
        printf("La chambre est occupée le %s par la réservation %lu\n", date_1, planning[i][k]) ;
      }
    }
    k++                                                                                         ;
  }
  if(libre != 0)
  {
    printf("Impossible de déclarer des travaux, veuillez modifier les réservations avant.\n")   ;
  }
  else
  {
    demande.code_resa = 1                                      ;
    maj_planning_travaux()                                     ;
    printf("La déclaration de travaux a bien été effectuée\n") ;
  }
}
}

/*############################################
#                                            #
#             modif_prix_chambre             #
#                                            #
##############################################

Procédure modifier le prix d'un chambre


*/
void modif_prix_chambre()
{

  int t2=0                                                                     ;
  float prix=0                                                                 ;

  while(t2 == 0)
  {
    printf("Modification du prix d'une chambre.\n")                            ;
    printf("Type de la chambre: chambre simple.\n")                            ;
    printf("Catégorie de la chambre: chambre.\n")                              ;
    printf("Prix actuel de la chambre en haute saison : %.2f\n", tab_prix_chambres[1].prix_hs) ;
    printf("Nouveau prix de la chambre en haute saison : ")                    ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[1].prix_hs = prix                                        ;
  }

  t2 = 0                                                                       ;
  while(t2 == 0)
  {
    printf("Prix actuel de la chambre en basse saison : %.2f\n", tab_prix_chambres[1].prix_bs) ;
    printf("Nouveau prix de la chambre en basse saison : ")                    ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[1].prix_bs = prix                                        ;
  }

  t2 = 0                                                                       ;
  while(t2 == 0)
  {
    printf("Type de la chambre: chambre simple.\n")                            ;
    printf("Catégorie de la chambre: suite.\n")                                ;
    printf("Prix actuel de la chambre en haute saison : %.2f\n", tab_prix_chambres[2].prix_hs) ;
    printf("Nouveau prix de la chambre en haute saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[2].prix_hs = prix                                        ;
  }

  t2 = 0                                                                       ;
  while(t2 == 0)
  {
    printf("Prix actuel de la chambre en basse saison : %.2f\n", tab_prix_chambres[2].prix_bs) ;
    printf("Nouveau prix de la chambre en basse saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[2].prix_bs = prix;
  }

  t2 = 0 ;
  while(t2 == 0)
  {
    printf("Type de la chambre: chambre double.\n")                            ;
    printf("Catégorie de la chambre: chambre.\n")                              ;
    printf("Prix actuel de la chambre en haute saison : %.2f\n", tab_prix_chambres[3].prix_hs) ;
    printf("Nouveau prix de la chambre en haute saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[3].prix_hs = prix                                        ;
  }

  t2 = 0                                                                       ;
  while(t2 == 0)
  {
    printf("Prix actuel de la chambre en basse saison : %.2f\n", tab_prix_chambres[3].prix_bs) ;
    printf("Nouveau prix de la chambre en basse saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[3].prix_bs = prix;
  }

  t2 = 0                                                                       ;
  while(t2 == 0)
  {
    printf("Type de la chambre: chambre double.\n")                            ;
    printf("Catégorie de la chambre: suite.\n")                                ;
    printf("Prix actuel de la chambre en haute saison : %.2f\n", tab_prix_chambres[4].prix_hs) ;
    printf("Nouveau prix de la chambre en haute saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[4].prix_hs = prix;
  }
  t2 = 0                                                                       ;
  while(t2 == 0)
  {
    printf("Prix actuel de la chambre en basse saison : %.2f\n", tab_prix_chambres[4].prix_bs) ;
    printf("Nouveau prix de la chambre en basse saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[4].prix_bs = prix                                        ;
  }


  t2 = 0                                                                       ;
  while(t2 == 0)
  {
    printf("Type de la chambre: chambre triple.\n")                            ;
    printf("Catégorie de la chambre: chambre.\n")                              ;
    printf("Prix actuel de la chambre en haute saison : %.2f\n", tab_prix_chambres[5].prix_hs) ;
    printf("Nouveau prix de la chambre en haute saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[5].prix_hs = prix                                        ;
  }

  t2 = 0                                                                       ;
  while(t2 == 0)
  {
    printf("Prix actuel de la chambre en basse saison : %.2f\n", tab_prix_chambres[5].prix_bs) ;
    printf("Nouveau prix de la chambre en basse saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[5].prix_bs = prix                                        ;
  }

  t2 = 0 ;
  while(t2 == 0)
  {
    printf("Type de la chambre: chambre triple.\n")                            ;
    printf("Catégorie de la chambre: suite.\n")                                ;
    printf("Prix actuel de la chambre en haute saison : %.2f\n", tab_prix_chambres[6].prix_hs) ;
    printf("Nouveau prix de la chambre en haute saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[6].prix_hs = prix                                        ;
  }

  t2 = 0 ;
  while(t2 == 0)
  {
    printf("Prix actuel de la chambre en basse saison : %.2f\n", tab_prix_chambres[6].prix_bs) ;
    printf("Nouveau prix de la chambre en basse saison : ")                    ;
    scanf("%f", &prix)                                                         ;
    t2 = scanf("%f", &prix)                                                    ;
    if(t2 == 0)
    {
      printf("Erreur de saisie\n")                                             ;
      while((poubelle=getchar()) != '\n')                                      ;
    }
    tab_prix_chambres[6].prix_bs = prix                                        ;
  }
    sauvegarde_prix_chambres()                                                 ;
}

/*############################################
#                                            #
#         sauvegarde_prix_chambres           #
#                                            #
##############################################

Enregistre le tableau dans le fichier PRIX_NUIT.
Appelée par modif_prix_chambre().
Affiche un message de confirmation.

*/

void sauvegarde_prix_chambres()
{
  FILE *f1                                                                     ;
  f1 = fopen(PRIX_NUIT, "w")                                                   ;
  int i                                                                        ;
  struct prix_nuit nuit                                                        ;


  for(i = 0; i < NB_CHAMBRES_PRIX ; i++)
  {
    nuit = tab_prix_chambres[i]                                                ;
    fprintf(f1, "%d %d %.2f %.2f\n", nuit.type_chambre, nuit.categorie_chambre, nuit.prix_hs, nuit.prix_bs) ;
  }
  fclose(f1)                                                                   ;
  printf("Le tableau des prix des nuitées a bien été sauvegardé.\n")           ;
}
