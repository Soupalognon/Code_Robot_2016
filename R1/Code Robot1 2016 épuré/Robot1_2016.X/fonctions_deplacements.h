/******************************************************************************/
/************** Carte polyvalente Botik  : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: fonctions_deplacements.h
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 14 d�cembre 2014, 15:00
 *******************************************************************************
 *
 *
 ******************************************************************************/

#ifndef FONCTIONS_DEPLACEMENTS_H
#define	FONCTIONS_DEPLACEMENTS_H

#ifdef	__cplusplus
extern "C" {
#endif

 
/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "Config_robots.h"

/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/
    #define MOYEN 80
    #define rej 9
    #define ATTENTE_EVITEMENT 150
    #define EFFACER 0
    #define AJOUTER 2

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    
/******************************************************************************/
/**************************** FONCTIONS COULEURS ******************************/
/******************************************************************************/

    void _fdt (double angle, char last);
    void faire_des_tours (int nb_tour);
/**
 * Fonction qui retourne la valeur du bouton de couleur pour savoir de quel c�t� du terrain on d�mare
 * @return JAUNE ou VERT
 */
uint8_t couleur_depart();

/**
 * Cette fonction permet d'inverser n'importe quelle param�tre selon la couleur de d�part
 * (Si on d�mare du c�t� oppos�, alors y est invers� et l'angle aussi ..)
 *
 * @param   param_inversable : le param�tre qui peut changer de signe en fonction de la couleur
 * @return  renvoit le param�tre sign� selon la couleur de d�part
 */
double inversion_couleur (double param_inversable);


/******************************************************************************/
/********************* FONCTIONS USUELLES (TEST ASSERV) ***********************/
/******************************************************************************/

void trapeze (int8_t sens_marche);

void petit_demi_cercle (int8_t sens_marche);

void grand_demi_cercle (int8_t sens_marche);

void carre (int8_t sens_marche);

/******************************************************************************/
/******************* FONCTIONS ASSERV HAUT NIVEAU (EVITEMENT) *****************/
/******************************************************************************/

//Fonction qui g�re les cas d'�vitement
void action_evitement (void);

//Fonction qui g�re les cas d'�vitement
void delai_action(void);

/**
 * Fonction qui sert � cibler un point de coordonn�es ( X, Y ) en mm !!!
 * @param x : Coordon�e X en mm
 * @param y : Coordon�e Y en mm
 * @param pourcentage_vitesse : Vitesse d�sir�e de 0 � 100 %
 */
void cibler (double x, double y, double pourcentage_vitesse);


/**
 * Fonction qui sert � orienter le robot par rapport � son rep�re de d�part
 * @param angle : valeur en degr�
 * @param pourcentage_vitesse : Vitesse d�sir� de 0 � 100%
 */
void orienter (double angle, double pourcentage_vitesse);


/**
 * Fonction qui oriente le robot et se d�place vers les coordonn�es (X, Y) en mm !!!
 * @param x : Coordonn�e X en mm
 * @param y : Coordonn�e Y en mm
 * @param sens_marche : MARCHE_AVANT ou MARCHE_ARRIERE
 * @param pourcentage_vitesse : Vitesse d�sir�e de 0 � 100 %
 */
void rejoindre (double x, double y, int8_t sens_marche, double pourcentage_vitesse);


/**
 * Fonction qui fait avancer le robot de x mm (ATTENTION : PAS D'ASSERVISSEMENT EN ANGLE !!!)
 * @param distance : distance en mm (distance n�gative pour reculer)
 * @param pourcentage_vitesse : Vitesse d�sir�e de 0 � 100%
 */
void avancer_reculer (double distance, double pourcentage_vitesse);


/*
 * Fonction qui fait avancer le robot de x mm (ATTENTION : PAS D'ASSERVISSEMENT EN ANGLE !!!)
 * @param distance : distance en mm (distance n�gative pour reculer)
 * @param pourcentage_vitesse : Vitesse d�sir�e de 0 � 100%
 * 
 * La particularit� de cette fonction est qu'elle a une detection de bloquage 100 fois plus
 * �lev� que les autres fonctions
 */
void calage (double distance, double pourcentage_vitesse);


/**
 * Fonction qui permet de passer par plusieurs points sans s'arr�ter.
 * Il faut d�finir des points par lesquels on veut que le robot passe. En pratique il passe � 15 cm de ces points !
 * \n\n
 * Attention � bien d�finir le d�placement, chaque trajectoire � un d�but, des d�placements de milieu et une fin.
 * Si on ne commence pas la trajectoire par un d�but, le robot ne d�marera pas, si on ne termine pas par une fin de trajectoire, le robot ne freinera pas, et continuera � avancer � pleine vitesse ...
 * \n
 * @param x : Coordonn�e X du point par lequel passer (en mm)
 * @param y : Coordonn�e Y du point par lequel passer (en mm)
 * @param sens_marche : MARCHE_AVANT ou MARCHE_ARRIERE
 * @param pourcentage_vitesse : Vitesse d�sir� de 0 � 100 % (voire plus .. 100% = vitesse max calcul�e par le robot, mais on peut parfois aller plus vite)
 * @param last : DEBUT_TRAJECTOIRE, MILIEU_TRAJECTOIRE, FIN_TRAJECTOIRE (NE PAS SE TROMPER !!!!!!!)
 */
void passe_part (double x, double y, int8_t sens_marche, double pourcentage_vitesse, char last);



/******************************************************************************/
/***************** FONCTIONS ASSERV BAS NIVEAU (config asserv) ****************/
/******************************************************************************/

uint8_t _cibler (double x, double y, double pourcentage_vitesse);

uint8_t _orienter (double angle, double pourcentage_vitesse);

uint8_t _rejoindre (double x, double y, int8_t sens_marche, double pourcentage_vitesse);

uint8_t _avancer_reculer (double distance, double pourcentage_vitesse);

uint8_t _calage (double distance, double pourcentage_vitesse);

uint8_t _passe_part (double x, double y, int8_t sens_marche, double pourcentage_vitesse, char last);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
#ifdef	__cplusplus
}
#endif

#endif	/* FONCTIONS_DEPLACEMENTS_H */

