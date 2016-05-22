/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: main.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 22 octobre 2015, 23:55
 *******************************************************************************
 ******************************************************************************/

#include "system.h"

/******************************************************************************
 ********************* DECLARATION DES VARIABLES GLOBALES *********************
 ******************************************************************************/

uint8_t COULEUR;
uint8_t STRATEGIE_EVITEMENT;
volatile uint8_t DETECTION;
volatile uint8_t EVITEMENT_ADV_AVANT;
volatile uint8_t EVITEMENT_ADV_ARRIERE;
volatile uint16_t FLAG_ACTION;
volatile uint8_t COMPTEUR_TEMPS_MATCH; //Permet de connaitre le temps du match
volatile uint16_t COMPTEUR_PERIODE_10MS; //Permet d'être plus précis que COMPTEUR_TEMPS_MATCH pour lancer les backup strat
volatile uint8_t fin_strategie_cause_evitement;
volatile uint8_t MOUVEMENT_AX12 = 0;
volatile uint8_t ROTATION_US = 0;


/******************************************************************************
 ******************************************************************************
 ******************************************************************************/


int main(int argc, char** argv)
{
    init_system();
    TIMER_DEBUG = DESACTIVE; //Active le débug pour voir les positions du robot en temps réel (fait rammer la strat et l'autom)
    delay_ms(500);

    //init_decalage_AX12();
    //init_position_AX12();

    while(SYS_JACK);
    PutsUART(UART_XBEE, "\n\n\n\n\n\nDebut du code : \n\r");

    strategie();
    //    reglage_odometrie();
    //    homologation();      

    while(1);
    return(EXIT_SUCCESS);
}