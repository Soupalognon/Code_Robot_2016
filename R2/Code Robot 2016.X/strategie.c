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


void strategie()
{
    COULEUR = couleur_depart();
    /*
     * Definit la configuration des coquillages CONFIG_1 - CONFIG_5
     */
    CONFIG_COQUILLAGE = CONFIG_1;
    
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
        init_position_robot (175, 965, 270);
        EVITEMENT_ADV_AVANT = OFF;
        

        //Init départ
        //init_pinces_jack();

        
        while(SYS_JACK);
        delay_ms(2000); 
        while(!SYS_JACK);
        TIMER_90s = ACTIVE;

        STRATEGIE_EVITEMENT = EVITEMENT_NORMAL;
        
            synchro_AX12(PORTE_D, 90, 1023, SANS_ATTENTE);
            delay_ms(1000);
            synchro_AX12(PORTE_G, -90, 1023, SANS_ATTENTE);
            delay_ms(2000);
        
        /*
         * Pousser blocs du milieu jusqu'a la zone
         */
        
            passe_part(500, 1050, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
            passe_part(750,1000,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
            passe_part(1200, 1000, MARCHE_AVANT, 70, FIN_TRAJECTOIRE);
            rejoindre(1050, 1000, MARCHE_ARRIERE, 100);
            FLAG_ACTION=OUVRIR_PORTES;
            
        /*
         * Ecrire deplacement pour chacune des configurations possibles
         * utiliser : FLAG_ACTION = FERMER_PORTES pour fermer les portes
         */
        
        switch(CONFIG_COQUILLAGE)
        {
            case CONFIG_1:
                passe_part(1410, 475, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE); /*  */
                passe_part(1500, 150, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(230, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(230, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(230, 820, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                break;
            case CONFIG_2:
                passe_part(1200, 350, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                passe_part(900, 550, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(450, 350, MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
                passe_part(230, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(250, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(230, 900, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                break;
            case CONFIG_3:
                passe_part(700, 750, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                passe_part(1200, 350, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(700, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(300, 350, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(230, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(230, 900, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                break;
            case CONFIG_4:
                passe_part(1200, 350, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                passe_part(700, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(250, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(250, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(230, 900, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                break;
            case CONFIG_5:
                passe_part(700, 150, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                passe_part(230, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(230, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                passe_part(230, 900, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                rejoindre(230,500, MARCHE_ARRIERE,100);
                passe_part(700,750, MARCHE_AVANT,100, DEBUT_TRAJECTOIRE);
                passe_part(230, 1000,MARCHE_AVANT,100,FIN_TRAJECTOIRE);
                break;
            default:
                break;
        }
        
        /*
         * Strategie lever de drapeaux
         */
        
    #endif
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

        STRATEGIE_EVITEMENT = EVITEMENT_NORMAL;

    #endif

}

void reglage_odometrie()
{
    delay_ms(2000);
    //while(!SYS_JACK);
    COULEUR = couleur_depart();
    
    #ifdef GROS_ROBOT
        init_jack();
    #endif
    
    EVITEMENT_ADV_ARRIERE = OFF;
    EVITEMENT_ADV_AVANT = OFF;

    init_position_robot (0, 0, 0);
    init_position_robot (-100, 0, 0);
    rejoindre(0, 0, MARCHE_AVANT, 100);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);

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