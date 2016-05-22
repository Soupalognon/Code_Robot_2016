/* 
 * File:   autom.h
 * Author: Quentin
 *
 * Created on 1 février 2015, 17:12
 */

#ifndef AUTOM_H
#define	AUTOM_H

#ifdef	__cplusplus
extern "C" {
#endif


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "Config_robots.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/



/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/


    /**************************************************************************/
    /*************************** DEFINE Général *******************************/
    /**************************************************************************/

    //Couleurs de départs
#define VERT                0
#define VIOLET              1

    //Conditions

#define NON                 2
#define OUI                 3
    //EN_COURS = 6

    //Evitement
#define ACTIF               0
#define INACTIF             1

#define ACTION_EVITEMENT    2   //Implémenté pour pouvoir réagir 
#define EVITEMENT_NORMAL    3
#define STOP                5
#define DELAI_ACTION        4

#define DISTANCE_STOP       5
#define ANGLE_STOP          5

    
    /**************************************************************************/
    /*************************** DEFINE ID AX12 *******************************/
    /**************************************************************************/

    //Pince
     #define PINCE_D                26
     #define PINCE_G                13
     #define ASCENSEUR              12
     #define CALAGE_CONE            17
    
     //Capteurs Avant     
    #define AX_US_DROIT             14
    #define AX_US_GAUCHE            3
    
    //Filet à Poissons
    #define DEPLOIMENT_BRAS_FILET   5
    #define ROT_FILET               4
    #define OUVERTURE_FILET         15
    
    //Funny Action
    #define PARASOL                 2

    
    /**************************************************************************/
    /*************************** POSITIONS AX12 *******************************/
    /**************************************************************************/


    /**************************************************************************/
    /************************* DEFINE ETAT PINCES *****************************/
    /**************************************************************************/

#define FERMER              1
#define OUVERT              2
#define OUVERTE             2
#define RELACHE             3
#define RACLETTE            4
#define RANGEMENT           5
//#define EN_COURS            6 -> fichier asserv
#define MONTER              7
#define DESCENDRE           8
#define HAUTEUR_DEMMARAGE   9
#define EN_HAUT             10
#define ACCOMPLI            11
#define DEPOSE              12
#define EN_BAS              13
#define INTERMEDIAIRE       14

#define EN_ATTENTE          16
#define EN_DESCENTE         17
#define EN_MONTER           18
#define LIBRE               19

#define DROITE              30
#define DROIT               30
#define GAUCHE              31

#define AVANT               32
#define ARRIERE             33

    /**************************************************************************/
    /************************* DEFINE FLAG_ACTION *****************************/
    /**************************************************************************/

#define NE_RIEN_FAIRE                   0
#define FIN_DE_MATCH                    1

#ifdef  PETIT_ROBOT
    #define ATTRAPE_PIEDS               2
    #define INIT_PINCES_DEMARRAGE       3
    #define DEPOSE_PIEDS                4
    #define PREPARATION_DEPOSE_PIEDS    5
    #define ATTRAPE_GOBELET             6
    #define DEPOSE_GOBELET              7
    #define ATTRAPE_BALLE               8
    #define CLAP                        9
    #define ZONE_DEPART                 10
    #define FERMETURE_PINCE             11
    #define PIEDS_4                     12
    #define EMPILEMENT                  13
    #define DESACTIVATION_EVITEMENT     14
#endif

#ifdef GROS_ROBOT
    #define TOUR_ALLIE                  2
    #define TOUR_ADVERSAIRE             3
    #define PORTES                      10
    #define POISSONS                    4
    #define OUVERTURE_PARASOL           5
    #define COQUILLAGES                 6
    #define DEFONCER_DUNE               11
#define TRANSITION_POISSON_TOUR_ADVERSAIRE  12
#endif

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/
    void autom_10ms (void);
    void son_evitement (uint8_t melodie);
    void rotation_us_avant();
    
    // Fonctions d'init
    void init_depart();

    /**
     * Fonction qui permet d'inverser le côté en fonction de la couleur
     * @param cote : le cote (DROITE ou GAUCHE)
     * @return le côté opposé GAUCHE ou DROITE
     */
    uint8_t inversion_autom (uint8_t cote);
    
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* AUTOM_H */

