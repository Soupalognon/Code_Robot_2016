/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: srategie.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 07 février 2015, 22:47
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/***************************PHASES DE JEU**************************************/
/******************************************************************************/

void init_phase_1(){
    
}

void phase_1(){
    init_phase_1();
    /*
     * Fonctions de deplacement
     */
    if(ARRET_DEPLACEMENT == 0){
        PHASE_STRATEGIE = 2;
    }
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void strategie()
{
    
/******************************************************************************/
/***************ACTIONS A EFFECTUER AVANT RETRAIT DU JACK**********************/
/******************************************************************************/
    
    /**
     * Set la couleur de depart
     */
    COULEUR = couleur_depart();
        
    /**
     * Init des pinces 
     */
    init_pinces_jack();
       
    /**
     * Attend que le jack soit retire
     */
    while(!SYS_JACK);

/******************************************************************************/
/***************ACTIONS A EFFECTUER APRES RETRAIT DU JACK**********************/
/******************************************************************************/
    
    /**
     * Demarrage du timer
     */
    TIMER_10ms = ACTIVE;
    TIMER_90s = ACTIVE;
    
    /**
     * Definit la strategie d'evitement
     */
    STRATEGIE_EVITEMENT = EVITEMENT_BACKUP;
    
    /**
     * Evitement a definir en fonction de la direction du mouvement
     */
    EVITEMENT_ADV_AVANT = ON;
    EVITEMENT_ADV_ARRIERE = OFF;
    
    /**
     * Definit la phase de strategie actuelle
     */
    PHASE_STRATEGIE = 1;
    
    /**
     * Boucle principale
     */
    while(1){
        switch(PHASE_STRATEGIE){
            case 1:
                phase_1();
                break;
            default:
                break;
        }
        ARRET_DEPLACEMENT = 0;
    }
        
}


void homologation()
{
    COULEUR = couleur_depart();

    #ifdef GROS_ROBOT

      // Inits avant démarage du robot :
        init_jack();

        if (COULEUR == JAUNE)
            synchro_AX12(AX_US, 45, 1023, SANS_ATTENTE);
        else
            synchro_AX12(AX_US, -45, 1023, SANS_ATTENTE);

        while(!SYS_JACK);

        // Démarage du match
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = ON;
        STRATEGIE_EVITEMENT = EVITEMENT_NORMAL;

        init_position_robot(180., 988., 0.);
        FLAG_ACTION = INIT_ASCENSEUR;

        delay_ms(1000);

    #endif

    #ifdef PETIT_ROBOT

        init_position_robot (153, 1030, 0);
        EVITEMENT_ADV_AVANT = OFF;

        //Init départ
        init_pinces_jack();

        while(!SYS_JACK);
        TIMER_90s = ACTIVE;

        STRATEGIE_EVITEMENT = EVITEMENT_BACKUP;

    #endif

}

void reglage_odometrie()
{
    delay_ms(2000);
    
    COULEUR = couleur_depart();
    
    #ifdef GROS_ROBOT
        init_jack();
    #endif
    init_position_robot (0, 0, 0);
    while(!SYS_JACK);
        
    EVITEMENT_ADV_ARRIERE = OFF;
    EVITEMENT_ADV_AVANT = OFF;

    //faire_des_tours(32);
    rejoindre(2000, 0, MARCHE_AVANT, 70);
    rejoindre(0, 0, MARCHE_AVANT, 70);
    rejoindre(2000, 0, MARCHE_AVANT, 70);
    rejoindre(0, 0, MARCHE_AVANT, 70);
    rejoindre(2000, 0, MARCHE_AVANT, 70);
    rejoindre(0, 0, MARCHE_AVANT, 70);
    rejoindre(2000, 0, MARCHE_AVANT, 70);
    rejoindre(0, 0, MARCHE_AVANT, 70);
    rejoindre(2000, 0, MARCHE_AVANT, 70);
//    trapeze(MARCHE_AVANT);
//    trapeze(MARCHE_AVANT);
//    trapeze(MARCHE_AVANT);
//    trapeze(MARCHE_AVANT);
//    trapeze(MARCHE_AVANT);

//           faire_des_tours(-32);
            while(1);
////
////            TIMER_DEBUG = ACTIVE;
//            init_position_robot(0, 0, 0);
            ////Horraire
//       rejoindre(2000, 0, MARCHE_AVANT, 50);
//       orienter(90, 50);
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


            //// Anti horaire
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter(90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter (90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter (90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter (90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter (90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter(90, 50);
        rejoindre(500, 0, MARCHE_AVANT, 100);


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

        delay_ms(10000);
}
