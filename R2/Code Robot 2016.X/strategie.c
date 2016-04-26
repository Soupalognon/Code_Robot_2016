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
    CONFIG_COQUILLAGE = CONFIG_3;

    #ifdef PETIT_ROBOT
        init_position_robot (83., 1229., 0.);
        //init_position_robot (175., 965., 270.);
        
        //Init départ
        init_pinces_jack();
        
        rejoindre(175 , 1229, MARCHE_AVANT, 30);
        orienter(270, 30);
        rejoindre(175 , 985, MARCHE_AVANT, 30);
        
        while(!SYS_JACK);
        
        TIMER_10ms = ACTIVE;
        TIMER_90s = ACTIVE;
        STRATEGIE_EVITEMENT = DELAI_ACTION;
        
        EVITEMENT_ADV_AVANT = ON;
        EVITEMENT_ADV_ARRIERE = OFF;
        
        FLAG_ACTION = POUSSER_TOUR;
          
        pousser_tour = 0;
        coquillage_phase_1 = 0;
        coquillage_phase_2 = 0;
        
        //rejoindre(175, 935, MARCHE_AVANT, 100);
        //passe_part(500, 935, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        
        while(1){
            switch(FLAG_ACTION){
            /*
             * Pousser blocs du milieu jusqu'a la zone
             */
                case POUSSER_TOUR:
                    FLAG_ACTION=FERMER_PORTES;
                    while(FLAG_ACTION == FERMER_PORTES);
                    
                    if (pousser_tour == 0){
                        passe_part(500, 1100, MARCHE_AVANT, 70, DEBUT_TRAJECTOIRE);
                        passe_part(600, 1100, MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                        passe_part(750,1030,MARCHE_AVANT,65,MILIEU_TRAJECTOIRE);
                        EVITEMENT_ADV_AVANT = OFF;
                        passe_part(1250, 1010, MARCHE_AVANT, 60, FIN_TRAJECTOIRE);
                        pousser_tour = 1;
                    }
                    if (pousser_tour == 1)
                    {
                        EVITEMENT_ADV_ARRIERE = ON;
                        rejoindre(900, 1010, MARCHE_ARRIERE, 100);
                        EVITEMENT_ADV_ARRIERE = OFF;
                        
//                        passe_part(1200,350, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
//                        passe_part(2000,350, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                        passe_part(2000,800, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                        pousser_tour = 2;
                        FLAG_ACTION = COQUILLAGE_ALLIE;
                    }
                    break;
                case COQUILLAGE_ALLIE:
                    FLAG_ACTION=OUVRIR_PORTES;
                    while(FLAG_ACTION == OUVRIR_PORTES);
                    
                    EVITEMENT_ADV_AVANT = OFF;
                    
                    switch(CONFIG_COQUILLAGE){
                        case CONFIG_1:
                            if(coquillage_phase_1 == 0){
                                passe_part(1200, 600, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                                synchro_AX12(PORTE_D,0,1023,SANS_ATTENTE);
                                synchro_AX12(PORTE_G,0,1023,SANS_ATTENTE);
                                passe_part(1410, 475, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                FLAG_ACTION = OUVRIR_PORTES;
                                passe_part(1730,350,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
                                passe_part(1600,200,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
                                passe_part(1460, 200, MARCHE_AVANT, 90, MILIEU_TRAJECTOIRE);
                                synchro_AX12(PORTE_D,0,1023,SANS_ATTENTE);
                                synchro_AX12(PORTE_G,0,1023,SANS_ATTENTE);
                                passe_part(1300,700,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
                                FLAG_ACTION = OUVRIR_PORTES;
                                passe_part(900,650,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
                                passe_part(230, 1100, MARCHE_AVANT, 60, FIN_TRAJECTOIRE);
//                                passe_part(230, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                                passe_part(230, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                                passe_part(230, 1100, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                                EVITEMENT_ADV_ARRIERE = ON;
                                while(EVITEMENT_ADV_ARRIERE != ON);
                                rejoindre(550,550,MARCHE_ARRIERE,100);
                                EVITEMENT_ADV_ARRIERE = OFF;
                                coquillage_phase_1 = 1;
                            }
                            if(coquillage_phase_1 == 1){
                                passe_part(270, 380, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                                passe_part(230, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(230, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(230, 1100, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
//                                rejoindre(1200, 500, MARCHE_AVANT, 100);
//                                orienter(180,100);
//                                rejoindre(230,1100,MARCHE_AVANT,100);
                                coquillage_phase_1 = 2;
                            }
                            break;
                            
                        case CONFIG_2:
                           if(coquillage_phase_1 == 0)
                           {
//                               passe_part(200, 450, MARCHE_AVANT, 60, DEBUT_TRAJECTOIRE);
//                               passe_part(1000, 220, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                               passe_part(1500, 220, MARCHE_AVANT, 90, MILIEU_TRAJECTOIRE);
//                               passe_part(1560, 300, MARCHE_AVANT, 90, MILIEU_TRAJECTOIRE);
//                               passe_part(1500, 400, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                               passe_part(1200, 400, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                               passe_part(900, 500, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                               passe_part(270, 800, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                               passe_part(220, 1229, MARCHE_AVANT, 80, FIN_TRAJECTOIRE);
                               
                               
                                passe_part(1200, 600, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                                passe_part(1410, 475, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                                passe_part(1500, 150, MARCHE_AVANT, 90, MILIEU_TRAJECTOIRE);
                                passe_part(1200, 350, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                                passe_part(900, 550, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(230, 1100, MARCHE_AVANT, 60, FIN_TRAJECTOIRE);
                                EVITEMENT_ADV_ARRIERE = ON;
                                while(!EVITEMENT_ADV_ARRIERE == ON);
                                rejoindre(550,550,MARCHE_ARRIERE,100);
                                EVITEMENT_ADV_ARRIERE = OFF;
                                
                                coquillage_phase_1 = 1;
                           }
                           if(coquillage_phase_1 == 1)
                           {
                                passe_part(270,380,MARCHE_AVANT,100,DEBUT_TRAJECTOIRE);
                                passe_part(230, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(250, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(230, 1100, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                                
                                coquillage_phase_1 = 2;
                           }
                            break;
                            
                        case CONFIG_3:
                           if(coquillage_phase_1 == 0)
                           {
//                               passe_part(200, 600, MARCHE_AVANT, 70, DEBUT_TRAJECTOIRE);
//                               passe_part(700, 650, MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
//                               passe_part(1100, 700, MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
//                               passe_part(1300, 600, MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
//                               passe_part(1200, 400, MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
//                               passe_part(800, 400, MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
//                               passe_part(650, 450, MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
//                               passe_part(300, 450, MARCHE_AVANT, 40, MILIEU_TRAJECTOIRE);
//                               passe_part(320,420 , MARCHE_ARRIERE, 70, MILIEU_TRAJECTOIRE);
//                               passe_part(200,600 , MARCHE_AVANT, 70, FIN_TRAJECTOIRE);
//                               
//                            
//                               
////                               rejoindre(300, 830, MARCHE_ARRIERE, 100);
//                               avancer_reculer(-200, 70);
//                               FLAG_ACTION = POUSSER_TOUR;
                               
                               
                                passe_part(1200, 350, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                                passe_part(700, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(700, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(230, 1100, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                                EVITEMENT_ADV_ARRIERE = ON;
                                while(!EVITEMENT_ADV_ARRIERE == ON);
                                rejoindre(550,550,MARCHE_ARRIERE,100);
                                EVITEMENT_ADV_ARRIERE = OFF;
                                coquillage_phase_1 = 1;
                           }
                           if(coquillage_phase_1 == 1){
                                passe_part(270,380,MARCHE_AVANT,100,DEBUT_TRAJECTOIRE);
                                passe_part(230, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(250, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(230,1100, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                                coquillage_phase_1 = 2;
                           }
                            break;
                            
                        case CONFIG_4:
                            if(coquillage_phase_1 == 0){
                                passe_part(1200, 600, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                                 passe_part(1410, 475, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                                passe_part(1730,350,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
                                passe_part(1200, 250, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(700, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(700, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(230, 1100, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                                EVITEMENT_ADV_ARRIERE = ON;
                                rejoindre(550,550,MARCHE_ARRIERE,100);
                                EVITEMENT_ADV_ARRIERE = OFF;
                                coquillage_phase_1 = 1;
                            }
                            if(coquillage_phase_1 == 1){
                                passe_part(270,380,MARCHE_AVANT,100,DEBUT_TRAJECTOIRE);
                                passe_part(230, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(230, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                                passe_part(230, 1100, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                                coquillage_phase_1 = 2;
                            }
                            break;
                            
                        case CONFIG_5:
                            if(coquillage_phase_1 == 0){
//                            passe_part(700, 750, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
//                            passe_part(500,350,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
                            passe_part(1200,350,MARCHE_AVANT,100,DEBUT_TRAJECTOIRE);
                            passe_part(700, 230, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                            passe_part(230, 450, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                            passe_part(230, 750, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                            passe_part(230, 1100, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                            EVITEMENT_ADV_ARRIERE = ON;
                            while(!EVITEMENT_ADV_ARRIERE == ON);
                            rejoindre(550,550,MARCHE_ARRIERE,100);
                            EVITEMENT_ADV_ARRIERE = OFF;
                            coquillage_phase_1 = 1;
                            }
                            if(coquillage_phase_1 == 1){
                                rejoindre(700, 700, MARCHE_AVANT, 100);
                                orienter(180,100);
                                rejoindre(230,1100,MARCHE_AVANT,100);
                                coquillage_phase_1 = 2;
                            }
                            break;
                        default:
                            break;
                    }
                    if(coquillage_phase_1 == 2){
                        //EVITEMENT_ADV_ARRIERE = ON;
                        //while(!EVITEMENT_ADV_ARRIERE == ON);
                        rejoindre(550,550,MARCHE_ARRIERE,100);
                        //EVITEMENT_ADV_ARRIERE = OFF;
                        passe_part(700,700,MARCHE_AVANT,100,DEBUT_TRAJECTOIRE);
                        passe_part(1200,350, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                        angle_AX12(PORTE_D, 350, 1023, SANS_ATTENTE);
//                        delay_ms(500);
//                        angle_AX12(PORTE_G, 655, 1023, SANS_ATTENTE);
                        passe_part(2000,350, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                        passe_part(2000,800, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                        coquillage_phase_1 = 3;
                    } 
                default:
                    break;
            }
            fin_strategie_cause_evitement = 0;//Activation du Flag qui bloque les déplacement du à la backup strat
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