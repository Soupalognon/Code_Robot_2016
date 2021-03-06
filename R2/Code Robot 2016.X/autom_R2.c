/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: autom.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 01 f�vrier 2015, 17:11
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
    angle_AX12(PORTE_D, 350, 1023, SANS_ATTENTE);
    delay_ms(1000);
    angle_AX12(PORTE_G, 655, 1023, SANS_ATTENTE);
}

/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_10ms (void)
{

        static uint16_t compteur = 0;
        static uint8_t  evitement_en_cours = OFF;
        static uint8_t compteur_moyenne_evitement = 0;
        static uint8_t timer_delai_evitement = 0;

        /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/

        //fonction qui definit les actions
        switch (FLAG_ACTION)
        {
            case NE_RIEN_FAIRE:
                break;
                
            default :
                break;
        }

        /**********************************************************************/
        /**************************** Evitement *******************************/
        /**********************************************************************/


        //Fonction permettant de lancer la fonction d'�vitement
        if(EVITEMENT_ADV_AVANT == ON)
        {
            if ( (CAPT_US_GAUCHE == 1 || CAPT_US_BALISE == 1 || CAPT_US_DROIT == 1) )
            {
                if ( DETECTION == OFF)
                {
                    compteur = 0;
                    compteur_moyenne_evitement = 0;
                    timer_delai_evitement = COMPTEUR_TEMPS_MATCH;
                    DETECTION = ON;
                    evitement_en_cours = OFF;
                    if(STRATEGIE_EVITEMENT != EVITEMENT_BACKUP) //On ne veut pas le lancer maintenant pour ce type d'�vitement
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
                if (compteur > 100)
                {
                    compteur = 100;
                    if (CAPT_US_GAUCHE == 0 && CAPT_US_BALISE == 0 && CAPT_US_DROIT == 0)
                    {
                        DETECTION = OFF;
                        unbrake();
                    }
                }
            }
            else if(DETECTION == ON && STRATEGIE_EVITEMENT == EVITEMENT_BACKUP)
            {
                if (CAPT_US_GAUCHE == 0 && CAPT_US_BALISE == 0 && CAPT_US_DROIT == 0)
                    compteur_moyenne_evitement++;		//Si les capteurs ne voient rien apr�s 10ms alors on incr�mente
                else
                    compteur_moyenne_evitement = 0;

                if(compteur_moyenne_evitement > 50)	
                {			//Si on ne voit rien durant 1s complete alors on relance
                    DETECTION = OFF;
                    unbrake();
                }
                
                if((COMPTEUR_TEMPS_MATCH - timer_delai_evitement) > 3)
                {//Si on d�passe le temps d'attente et qu'il y a toujours l'adversaire en face on change de strat
                    FLAG_ASSERV.erreur = EVITEMENT;
                    fin_deplacement();
                }
            }
        }
        
        
        else if (EVITEMENT_ADV_ARRIERE == ON)
        {
            if ( (CAPT_IR_ARRIERE_CENTRE == 0 || CAPT_IR_ARRIERE_DROIT == 0 || CAPT_IR_ARRIERE_GAUCHE == 0)  && DETECTION == OFF)
            {
                compteur = 0;
                compteur_moyenne_evitement = 0;
                timer_delai_evitement = COMPTEUR_TEMPS_MATCH;
                DETECTION = ON;
                evitement_en_cours = OFF;
                if(STRATEGIE_EVITEMENT != EVITEMENT_BACKUP) //On ne veut pas le lancer maintenant pour ce type d'�vitement
                    FLAG_ASSERV.erreur = EVITEMENT;
                brake();
            }
            
            
            if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 100)
                {
                    compteur = 100;
                    if (CAPT_IR_ARRIERE_CENTRE == 1 && CAPT_IR_ARRIERE_DROIT == 1 && CAPT_IR_ARRIERE_GAUCHE == 1)
                    {
                        DETECTION = OFF;
                        unbrake();
                    }
                }
            }
            
            else if(DETECTION == ON && STRATEGIE_EVITEMENT == EVITEMENT_BACKUP)
            {
                if ((CAPT_IR_ARRIERE_CENTRE == 0 && CAPT_IR_ARRIERE_DROIT == 0 && CAPT_IR_ARRIERE_GAUCHE == 0))
                    compteur_moyenne_evitement++;
                else
                    compteur_moyenne_evitement = 0;

                if(compteur_moyenne_evitement > 50)
                {
                    DETECTION = OFF;
                    unbrake();
                }

                if((COMPTEUR_TEMPS_MATCH - timer_delai_evitement) > 3)
                {//Si on d�passe le temps d'attente et qu'il y a toujours l'adversaire en face on change de strat
                    FLAG_ASSERV.erreur = EVITEMENT;
                    fin_deplacement();
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