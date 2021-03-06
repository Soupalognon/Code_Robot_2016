/* 
 * File:   autom.h
 * Author: Quentin
 *
 * Created on 1 f�vrier 2015, 17:12
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
    /*************************** DEFINE G�n�ral *******************************/
    /**************************************************************************/

    //Couleurs de d�parts
#define VERT                0
#define JAUNE               1

    //Conditions

#define NON                 2
#define OUI                 3
    //EN_COURS = 6

    //Evitement
#define ACTIF               0
#define INACTIF             1

#define EVITEMENT_BACKUP    2
#define STOP                3

#define DISTANCE_STOP       3
#define ANGLE_STOP          3
    
#define CONFIG_1            1
#define CONFIG_2            2
#define CONFIG_3            3
#define CONFIG_4            4
#define CONFIG_5            5

    /**************************************************************************/
    /*************************** DEFINE ID AX12 *******************************/
    /**************************************************************************/

#ifdef PETIT_ROBOT
    #define PORTE_D                 11
    #define PORTE_G                 8
#endif

#ifdef  GROS_ROBOT
     
#endif

    /**************************************************************************/
    /*************************** POSITIONS AX12 *******************************/
    /**************************************************************************/

#ifdef  PETIT_ROBOT
    #define PINCE_D_OUVERTE     370
#endif

#ifdef GROS_ROBOT

#endif

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


#define DROITE              30
#define DROIT               30
#define GAUCHE              31

    /**************************************************************************/
    /************************* DEFINE FLAG_ACTION *****************************/
    /**************************************************************************/

 #define NE_RIEN_FAIRE                   0
#define FIN_DE_MATCH                    1

#ifdef  PETIT_ROBOT
    #define FERMER_PORTES               2
    #define OUVRIR_PORTES               3
    #define LEVER_DRAPEAU               4
    #define POUSSER_TOUR                5
    #define COQUILLAGE_ALLIE            6
    #define COQUILLAGE_ADVERSAIRE       7

#endif

#ifdef GROS_ROBOT
    #define INIT_ASCENSEUR              2
    #define INIT_DEPART                 3
    #define INIT_JACK                   4
    #define ATTRAPE_GOBELET             5
    #define ATTRAPE_GOBELET2            6
    #define MONTEE_MARCHE               7
    #define PREPARATION_MARCHE          8
    #define EN_ROUTE_MONTEE_MARCHE      9   
    #define ARRIVEE_MARCHE              10
    #define MONTEE_EVITEMENT_EN_COURS   11


#endif

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void jack();
    void allumer_pompes ();
    void eteindre_pompe();
    void autom_10ms (void);
    void son_evitement (uint8_t melodie);

#ifdef  PETIT_ROBOT

    
    void init_pinces_demarage();
    void init_pinces_jack();
    void depose_pieds();
    void preparation_descente_pieds ();
    void depose_banc ();
    void attrape_gobelet();
    void depose_gobelet();
    void attrape_balle();
    void ouvrir_bras (uint8_t);
    void fermer_bras (uint8_t);
    void faire_les_claps();
    void empilement(int taille_max);

    void desactivation_evitement_reculer ();

    void ejecter_balle();
    void init_balle();

    //Fonctions autom ax12
    void pinces (uint8_t ID, uint8_t etat);
    void ascenseur (uint8_t direction);
#endif

#ifdef  GROS_ROBOT

    void rotation_us(void);

    // Fonctions d'init
    void init_jack();
    void init_depart();

    void marche();
    void rotation_us_avant();;
    
    //Fonction AX12

    uint8_t check_capteur (uint8_t cote);

    /**
     * Fonction qui permet d'inverser le c�t� en fonction de la couleur
     * @param cote : le cote (DROITE ou GAUCHE)
     * @return le c�t� oppos� GAUCHE ou DROITE
     */
    uint8_t inversion_autom (uint8_t cote);

    /**
     * Fonction qui d�ploie les chenilles
     * @param action : MONTER pour ranger les chenilles, DESCENDRE pour les descendre
     */
    void chenilles(uint8_t action);

    /**
     * Fonction pour ouvrir les bras du robot
     * @param cote : DROIT / GAUCHE
     * @param action : OUVERT / FERMER
     */
    void bras(uint8_t cote, uint8_t action);

    /**
     * Fonction qui ouvre les bras des tapis
     * @param cote : DROIT / GAUCHE
     * @param action : RANGEMENT / OUVERT / DEPOSE
     */
    void tapis(uint8_t cote, uint8_t action);

    /**
     * Fonction qui ouvre les pinces avant
     * @param cote DROIT / GAUCHE
     * @param action : OUVERTE / FERMER / RANGEMENT
     */
    void pince(uint8_t cote,uint8_t action);

    /**
     * Foncion qui range les pinces � l'interieur du robot
     * @param action : AVANT / ARRIERE
     */
    void ascenseur(uint8_t action);

    void monter_balise ();

    void descendre_balise();



    /**************************************************************************/
    /**************************************************************************/
    /**************************************************************************/


    void attrape_gobelet (uint8_t reinit);
    void arrive_marche ();
    void montee_des_marches ();
    
#endif


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* AUTOM_H */

