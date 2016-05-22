/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: autom.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 08 fevrier 2016, 19:00
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************
 ******************************** INCLUDES ************************************
 ******************************************************************************/

#include "system.h"


/******************************************************************************
 ***************************** FONCTIONS DIVERSES *****************************
 ******************************************************************************/

void son_evitement(uint8_t melodie)
{
    Nop();
    commande_AX12(100, _4PARAM, WRITE_DATA, 0x29, 10, NC, NC, NC);
    commande_AX12(100, _4PARAM, WRITE_DATA, 0x28, melodie, NC, NC, NC);
}


void rotation_us_avant()
{
    static char sens_D = 0, sens_G = 0;
    static uint16_t position_D = 0, position_G = 0;

    if(sens_D == 0)
    {
        position_D += 10;
        if(position_D > 500)
            sens_D = 1;
    }
    else
    {
        position_D -= 10;
        if(position_D < 400)
            sens_D = 0;
    }

    if(sens_G == 0)
    {
        position_G += 10;
        if(position_G > 200)
            sens_G = 1;
    }
    else
    {
        position_G -= 10;
        if(position_G < 100)
            sens_G = 0;
    }

    angle_AX12(AX_US_DROIT, position_D, 1023, SANS_ATTENTE);
    angle_AX12(AX_US_GAUCHE, position_G, 1023, SANS_ATTENTE);
}


uint8_t inversion_autom(uint8_t cote)
{
    if(COULEUR == VIOLET)
        return cote;
    else
    {
        if(cote == DROIT)
            return GAUCHE;
        else if(cote == GAUCHE)
            return DROIT;
        else
            return cote;
    }
}


/******************************************************************************
 ********************************  FONCTION AX12  *****************************
 ******************************************************************************/


/******************************************************************************
 **************************** FONCTIONS D'INITS *******************************
 ******************************************************************************/

void init_depart()
{
    ROTATION_US = ACTIVE; //Active la rotation des US avant pour mieux voir devant soi
}

/******************************************************************************
 ******************************** FONCTIONS AUTOM *****************************
 ******************************************************************************/


/******************************************************************************
 ******************************** FONCTION BOUCLE *****************************
 ******************************************************************************/

void autom_10ms(void)
{
    static uint16_t compteur = 0;
    static uint8_t evitement_en_cours = OFF;
    static uint8_t compteur_moyenne_evitement = 0;
    static uint16_t timer_delai_evitement = 0;

    /**********************************************************************/
    /******************************* Autom ********************************/
    /**********************************************************************/

    /*
     Mettre les cas où le robot doit changer de strat au bout d'un certain du COMPTEUR_TEMPS_MATCH
     */

    //fonction qui definit les actions
    switch(FLAG_ACTION)
    {//Mettre toutes les fonctions de déplacement des AX12
    }



    /* POUR CONNAITRE LES DIFFERENTS TYPES D'EVITEMENT QUI EXISTE ALORS ALLER DANS "autom.h" section Evitement*/
    /**********************************************************************
     **************************** Gestion Evitement ***********************
     **********************************************************************/
    //Permettant de lancer la fonction d'évitement
    if(EVITEMENT_ADV_AVANT == ON)
    {
        if((CAPT_US_AV_DROIT == 0 || CAPT_US_AV_GAUCHE == 0) && DETECTION == OFF)
        {//Si les capteurs avant detecte quelque chose
            compteur = 0;
            compteur_moyenne_evitement = 0;
            timer_delai_evitement = COMPTEUR_PERIODE_10MS;
            evitement_en_cours = OFF;

            if(STRATEGIE_EVITEMENT != DELAI_ACTION) //On ne veut pas le lancer maintenant pour ce type d'évitement
                FLAG_ASSERV.erreur = EVITEMENT;

            DETECTION = ON;
            brake();
        }
        else if(DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
        {
            compteur++;
            if(compteur > 20)
            {
                if(CAPT_US_AV_DROIT == 1 && CAPT_US_AV_GAUCHE == 1) //Si les 2 ne voient rien
                    compteur_moyenne_evitement++;
                else
                    compteur_moyenne_evitement = 0;

                if(compteur_moyenne_evitement > 100)
                {
                    compteur_moyenne_evitement = 0;
                    DETECTION = OFF;
                    unbrake();
                }
            }
        }
        else if(DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL))
        {
            if(evitement_en_cours == OFF)
            {
                compteur++;
                if(compteur > 40)
                {
                    evitement_en_cours = ON;
                    compteur = 0;
                    fin_deplacement();
                }
            }
        }

        else if(DETECTION == ON && STRATEGIE_EVITEMENT == DELAI_ACTION)
        {
            if(CAPT_US_AV_DROIT == 1 && CAPT_US_AV_GAUCHE == 1)
                compteur_moyenne_evitement++; //Si les capteurs ne voient rien après 10ms alors on incrémente
            else
                compteur_moyenne_evitement = 0;

            if(compteur_moyenne_evitement > 50) //Attend 500ms
            { //Si on ne voit rien durant 1s complete alors on relance
                DETECTION = OFF;
                unbrake();
            }

            if((COMPTEUR_PERIODE_10MS - timer_delai_evitement) > 200) //Attend 2 sec 
            {//Si on dépasse le temps d'attente et qu'il y a toujours l'adversaire en face on change de strat
                FLAG_ASSERV.erreur = EVITEMENT;
                fin_deplacement();
            }
        }

        if(ROTATION_US == ACTIVE)
            rotation_us_avant();
    }



        //Evitement arrière
    else if(EVITEMENT_ADV_ARRIERE == ON)
    {
        if((CAPT_US_AR_GAUCHE == 1 || CAPT_US_AR_DROITE == 1) && DETECTION == OFF)
        {
            compteur = 0;
            compteur_moyenne_evitement = 0;
            timer_delai_evitement = COMPTEUR_TEMPS_MATCH;
            DETECTION = ON;
            evitement_en_cours = OFF;


            if(STRATEGIE_EVITEMENT != DELAI_ACTION)
                FLAG_ASSERV.erreur = EVITEMENT;
            brake();
        }
        else if(DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
        {
            compteur++;
            if(compteur > 20)
            {
                if((CAPT_US_AR_GAUCHE == 0 && CAPT_US_AR_DROITE == 0))
                    compteur_moyenne_evitement++;
                else
                    compteur_moyenne_evitement = 0;
                if(compteur_moyenne_evitement > 100)
                {
                    DETECTION = OFF;
                    unbrake();
                }
            }
        }
        else if(DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL))
        {
            if(evitement_en_cours == OFF)
            {
                compteur++;
                if(compteur > 40)
                {
                    evitement_en_cours = ON;
                    compteur = 0;
                    fin_deplacement();
                    son_evitement(30);
                }
                else
                {
                    son_evitement(2);
                }
            }
        }

        else if(DETECTION == ON && STRATEGIE_EVITEMENT == DELAI_ACTION)
        {
            if(CAPT_US_AR_GAUCHE == 0 && CAPT_US_AR_DROITE == 0)
                compteur_moyenne_evitement++;
            else
                compteur_moyenne_evitement = 0;

            if(compteur_moyenne_evitement > 50)
            {
                DETECTION = OFF;
                unbrake();
            }

            if((COMPTEUR_TEMPS_MATCH - timer_delai_evitement) > 2)
            {//Si on dépasse le temps d'attente et qu'il y a toujours l'adversaire en face on change de strat
                FLAG_ASSERV.erreur = EVITEMENT;
                fin_deplacement();
            }
        }
    }
    else if(DETECTION == ON)
    {
        DETECTION = OFF;
        unbrake();
    }
}