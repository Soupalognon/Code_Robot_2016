/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: autom.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 01 février 2015, 17:11
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/***************************** FONCTIONS DIVERSES *****************************/
/******************************************************************************/

#ifdef PETIT_ROBOT

void jack()
{
    while(!SYS_JACK);
    while(SYS_JACK);
}

void allumer_pompes ()
{
    envoit_pwm(MOTEUR_X, 100);
}

void eteindre_pompe ()
{
    envoit_pwm(MOTEUR_X, 0);
}

void son_evitement (uint8_t melodie)
{/*
    commande_AX12(100, _4PARAM, WRITE_DATA, 0x29, 10, NC, NC, NC);
    commande_AX12(100, _4PARAM, WRITE_DATA, 0x28, melodie, NC, NC, NC);

  */}

/******************************************************************************/
/********************************  FONCTION AX12  *****************************/
/******************************************************************************/

void porte (uint8_t ID, uint8_t etat)
{
    Nop();
}

void init_pinces_jack()
{
    Nop();
}

/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_10ms (void)
{

        static uint16_t compteur = 0;
        static uint8_t  evitement_en_cours = OFF;

        /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/

        //fonction qui definit les actions
        switch (FLAG_ACTION)
        {
            case NE_RIEN_FAIRE:
                break;
            /*
             * Ecrire fermeture des portes pour retenir coquillages
             */
            case FERMER_PORTES:
                synchro_AX12(PORTE_G, - 20, 1023, SANS_ATTENTE);
                synchro_AX12(PORTE_D,  20, 1023, SANS_ATTENTE);
                FLAG_ACTION = NE_RIEN_FAIRE;
            case OUVRIR_PORTES:
                synchro_AX12(PORTE_D, -10, 1023, SANS_ATTENTE);
                synchro_AX12(PORTE_G,  10, 1023, SANS_ATTENTE);
                FLAG_ACTION = NE_RIEN_FAIRE;
            /*
             * Ecrire sequence lever de drapeaux
             */
            case LEVER_DRAPEAU:
                break;
                
            default :
                break;
        }

        /**********************************************************************/
        /**************************** Evitement *******************************/
        /**********************************************************************/


        //Fonction permettant de lancer la fonction d'évitement
        if(EVITEMENT_ADV_AVANT == ON)
        {
            if ( (CAPT_US_GAUCHE == 1 || CAPT_US_BALISE == 1 || CAPT_US_DROIT == 1) )
            {
                if ( DETECTION == OFF)
                {
                    compteur = 0;
                    DETECTION = ON;
                    evitement_en_cours = OFF;
                    FLAG_ASSERV.erreur = EVITEMENT;
                    brake();
                    son_evitement(49);
                }
                else
                    son_evitement(2);
            }
            if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 20)
                {
                    compteur = 20;
                    if (CAPT_US_GAUCHE == 0 && CAPT_US_BALISE == 0 && CAPT_US_DROIT == 0)
                    {
                        DETECTION = OFF;
                        unbrake();
                    }
                }
            }
            else if (DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL ))
            {
                if (evitement_en_cours == OFF)
                {
                    compteur ++;
                    if (compteur > 40)
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
        }

        else if (EVITEMENT_ADV_ARRIERE == ON)
        {
            if ( (CAPT_IR_ARRIERE_CENTRE == 0 || CAPT_IR_ARRIERE_DROIT == 0 || CAPT_IR_ARRIERE_GAUCHE == 0)  && DETECTION == OFF)
            {
                compteur = 0;
                DETECTION = ON;
                evitement_en_cours = OFF;
                FLAG_ASSERV.erreur = EVITEMENT;
                brake();
            }
            if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 20)
                {
                    compteur = 20;
                    if (CAPT_IR_ARRIERE_CENTRE == 1 && CAPT_IR_ARRIERE_DROIT == 1 && CAPT_IR_ARRIERE_GAUCHE == 1)
                    {
                        DETECTION = OFF;
                        unbrake();
                    }
                }
            }
            else if (DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL ))
            {
                if (evitement_en_cours == OFF)
                {
                    compteur ++;
                    if (compteur > 40)
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
        }
        else if (DETECTION == ON)
        {
            DETECTION = OFF;
            unbrake();
        }
}

#endif