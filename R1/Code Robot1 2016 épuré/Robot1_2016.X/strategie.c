/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: srategie.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 07 février 2015, 22:47
 ******************************************************************************
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/



void strategie()
{
    static uint8_t temporisation = 0; //Permet les tempo pour les temps d'attentes

    COULEUR = couleur_depart();
    // Inits avant démarage du robot :
    allumer_LED_AX12(TOUS_LES_AX12); //Vérifie si les AX fonctionnent
    init_depart(); //Range tous les AX à leurs positions initiales

    init_position_robot(0., 0., 0.);

    delay_ms(500);

    while(!SYS_JACK);

    // Démarage du match
    TIMER_10ms = ACTIVE;
    TIMER_90s = ACTIVE;

    EVITEMENT_ADV_AVANT = OFF;
    EVITEMENT_ADV_ARRIERE = OFF;

    STRATEGIE_EVITEMENT = DELAI_ACTION;

    FLAG_ACTION = NE_RIEN_FAIRE;

    while(1)
    {
        switch(FLAG_ACTION)
        {//Mettre toutes les fonctions de déplacement du robot  
        }
        fin_strategie_cause_evitement = 0; //Reset du Flag qui bloque les déplacement dut à la backup strat
    }
}


void homologation()
{
    COULEUR = couleur_depart();

    // Inits avant démarage du robot :
    init_depart(); //Range tous les éléments à sa position initiale

    init_position_robot(78., 1238., 0.);

    delay_ms(500);
    //On se place correctement pour que les 2 robots puissent rentrés dans la zone de départ

    while(!SYS_JACK);

    // Démarage du match
    TIMER_10ms = ACTIVE;
    TIMER_90s = ACTIVE;
    EVITEMENT_ADV_AVANT = OFF; //On désactive tout les évitements au début pour permettre le 180° au début du match
    EVITEMENT_ADV_ARRIERE = OFF;

    STRATEGIE_EVITEMENT = STOP;

    FLAG_ACTION = NE_RIEN_FAIRE;

    while(1)
    {
        switch(FLAG_ACTION)
        {
        }
    }
}


void reglage_odometrie()
{
    delay_ms(2000);
    while(!SYS_JACK);
    COULEUR = couleur_depart();

    EVITEMENT_ADV_ARRIERE = OFF;
    EVITEMENT_ADV_AVANT = OFF;

    init_position_robot(0, 0, 0);

    //    faire_des_tours(64);
    //    faire_des_tours(-32);
    //    rejoindre(2000, 0, MARCHE_AVANT, 50);
    //    delay_ms(30000);
    //    carre(MARCHE_AVANT);

    delay_ms(10000);

    PutsUART(UART_XBEE, "\n\n\n\r X : ");
    PutLongUART((int32_t) get_X());
    PutcUART(UART_XBEE, '.');
    PutcUART(UART_XBEE, ((uint8_t) ((int32_t) ((double) get_X() * 10)) - (((int32_t) get_X()) * 10)) + 48);
    PutsUART(UART_XBEE, " Y : ");
    PutLongUART((int32_t) get_Y());
    PutcUART(UART_XBEE, '.');
    PutcUART(UART_XBEE, ((uint8_t) ((int32_t) ((double) (get_Y() * 10))) - (((int32_t) get_Y()) * 10)) + 48);
    PutsUART(UART_XBEE, " Teta : ");
    PutLongUART((int32_t) get_orientation());
    PutcUART(UART_XBEE, '.');
    PutcUART(UART_XBEE, ((uint8_t) ((int32_t) ((double) (get_orientation() * 10))) - (((int32_t) get_orientation()) * 10)) + 48);

    /*
    rejoindre(0, 0, MARCHE_AVANT, 100);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
     * */
    while(1);


    //        TIMER_DEBUG = ACTIVE;
    //        init_position_robot(0, 0, 0);

    //        //Horraire
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter(-90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter (-90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter (-90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter (-90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter (-90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter(-90, 50);


    // Anti horaire
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(-90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter(90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(-90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter (90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(-90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter (90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(-90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter (90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(-90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter (90, 50);
    //        rejoindre(2000, 0, MARCHE_AVANT, 50);
    //        orienter(-90, 50);
    //        rejoindre(300, 0, MARCHE_AVANT, 50);
    //        orienter(90, 50);
    //        rejoindre(500, 0, MARCHE_AVANT, 100);


    //        rejoindre(2000, 0, MARCHE_AVANT, 100);
    //        rejoindre(300, 0, MARCHE_AVANT, 100);
    //        rejoindre(2000, 0, MARCHE_AVANT, 100);
    //        rejoindre(300, 0, MARCHE_AVANT, 100);
    //        rejoindre(2000, 0, MARCHE_AVANT, 100);
    //        rejoindre(300, 0, MARCHE_AVANT, 100);
    //        rejoindre(2000, 0, MARCHE_AVANT, 100);
    //        rejoindre(300, 0, MARCHE_AVANT, 100);
    //        rejoindre(2000, 0, MARCHE_AVANT, 100);
    //        rejoindre(300, 0, MARCHE_AVANT, 100);
    //        rejoindre(2000, 0, MARCHE_AVANT, 100);
    //        rejoindre(300, 0, MARCHE_AVANT, 100);
    //        rejoindre(2000, 0, MARCHE_AVANT, 100);
    //        rejoindre(300, 0, MARCHE_AVANT, 100);
    //        rejoindre(500, 0, MARCHE_AVANT, 100);
    //        TIMER_DEBUG = DESACTIVE;
}
