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
    CONFIG_COQUILLAGE = CONFIG_4;
    
    #ifdef GROS_ROBOT
        // Inits avant démarage du robot :
        //init_jack();

        //if (COULEUR == VIOLET)
            //synchro_AX12(AX_US, 45, 1023, SANS_ATTENTE);
        //else
            //synchro_AX12(AX_US, -45, 1023, SANS_ATTENTE);

        //while(!SYS_JACK);

        // Démarage du match
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = OFF;
        STRATEGIE_EVITEMENT = EVITEMENT_NORMAL;

        init_position_robot(78., 1235., 0.);
        //FLAG_ACTION = INIT_ASCENSEUR;
        
        //Init de départ
        angle_AX12(PINCE_D, 285, 300, AVEC_ATTENTE); //Position rangé
        angle_AX12(PINCE_G, 735, 300, AVEC_ATTENTE); //Position rangé
        angle_AX12(ASCENSEUR, 235, 512, AVEC_ATTENTE);   //Position basse
        angle_AX12(ROT_FILET, 85, 300, AVEC_ATTENTE);    //Position relevé (Tout début)
        angle_AX12(OUVERTURE_FILET, 256, 300, AVEC_ATTENTE);    //Position fermé
        angle_AX12(AX_CALAGE_CONE, 750, 1023, AVEC_ATTENTE);
        lancer_autom_AX12();
        angle_AX12(DEPLOIMENT_BRAS_FILET, 820, 200, SANS_ATTENTE);   //Position remonté
        delay_ms(100);
        
        passe_part(500, 1235, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        passe_part(1100, 250, MARCHE_AVANT, 60, MILIEU_TRAJECTOIRE);
        passe_part(1000, 200,MARCHE_AVANT, 40, MILIEU_TRAJECTOIRE);
        passe_part(900, 175,MARCHE_AVANT, 40, MILIEU_TRAJECTOIRE);
        passe_part(800, 157,MARCHE_AVANT, 40, MILIEU_TRAJECTOIRE);
        passe_part(430, 157,MARCHE_AVANT, 40, FIN_TRAJECTOIRE);
        
        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 200, SANS_ATTENTE);   //Position déployé
        delay_ms(100);
        angle_AX12(OUVERTURE_FILET, 860, 300, SANS_ATTENTE);    //Position ouverte
        delay_ms(100);
        angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
        delay_ms(100);
        
        rejoindre(635, 158, MARCHE_ARRIERE, 40);
        
        angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);   //Position dans l'eau
        delay_ms(100);
        
        rejoindre(795, 158, MARCHE_ARRIERE, 40);
        
        angle_AX12(ROT_FILET, 1005, 600, SANS_ATTENTE);  //Position Fin (poissons récupérés)
        delay_ms(100);
        angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 350, SANS_ATTENTE);   //Position intermédiaire (pour passé la barre du filet)
        delay_ms(200);
        
        passe_part(900, 210, MARCHE_ARRIERE, 40, DEBUT_TRAJECTOIRE);
        passe_part(1000, 210,MARCHE_ARRIERE, 70, MILIEU_TRAJECTOIRE);
        passe_part(1200, 155,MARCHE_ARRIERE, 70, MILIEU_TRAJECTOIRE);
        passe_part(1300, 158,MARCHE_ARRIERE, 70, FIN_TRAJECTOIRE);
        
        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 150, AVEC_ATTENTE);   //Position déployé
        angle_AX12(ROT_FILET, 375, 300, AVEC_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
        lancer_autom_AX12();
        delay_ms(1000);
    #endif
  
    #ifdef PETIT_ROBOT
            passe_part(500, 1050, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
            passe_part(750,1000,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
            passe_part(1200, 1000, MARCHE_AVANT, 70, FIN_TRAJECTOIRE);
            rejoindre(1050, 1000, MARCHE_ARRIERE, 100);
    #endif
}



void homologation()
{
    COULEUR = couleur_depart();

    #ifdef GROS_ROBOT

      // Inits avant démarage du robot :
        init_jack();
        /*
        if (COULEUR == VIOLET)
            synchro_AX12(AX_US, 45, 1023, SANS_ATTENTE);
        else
            synchro_AX12(AX_US, -45, 1023, SANS_ATTENTE);
        */
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