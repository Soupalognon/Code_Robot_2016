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
        init_jack();
        init_depart();

        while(!SYS_JACK);

        // Démarage du match
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = ON;
        EVITEMENT_ADV_ARRIERE = OFF;
        STRATEGIE_EVITEMENT = DELAI_ACTION;

        init_position_robot(78., 1235., 0.);
        
        FLAG_ACTION = TOUR_ALLIE;
        
        etape_tour_allie = 0;
        etape_tour_adversaire = 0;
        etape_poissons = 0;
        etape_coquillages = 0;
        fin_strategie_cause_evitement = 0;
        
        while(1)
        {
            switch(FLAG_ACTION)
            {//Mettre toutes les fonctions de déplacement du robot
                case TOUR_ALLIE:
                    if(etape_tour_allie == 0)
                    {//positionne vers la barre, s'encastre dedant
                        passe_part(300, 1235,MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
                        passe_part(800, 1400,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                        passe_part(895, 1500,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                        passe_part(895, 1600,MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                        orienter(90,100);
                        rejoindre(895, 1747, MARCHE_AVANT, 60);//895,1745
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            etape_tour_allie = 1;
                    }
                    else if(etape_tour_allie == 1)
                    {//attrappe tour et recule (sort de la barre)
                        mouvement_AX12 = ACTIVE;
                        while(mouvement_AX12 == ACTIVE);
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
                        while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0); 
                        rejoindre(895, 1600, MARCHE_ARRIERE, 30);
                        etape_tour_allie = 2;
                    }
                    else if(etape_tour_allie == 2)
                    {//recule encore, se positionne pour poser la tour
                        passe_part(895, 1500,MARCHE_ARRIERE, 30, DEBUT_TRAJECTOIRE);
                        passe_part(800, 1400,MARCHE_ARRIERE, 30, MILIEU_TRAJECTOIRE);
                        passe_part(400, 900,MARCHE_ARRIERE, 30, FIN_TRAJECTOIRE);
                        etape_tour_allie = 3;
                    }
                    else if(etape_tour_allie == 3)
                    {//avance, va dans la zone de largage
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = ON;
                        rejoindre(1000, 1000,MARCHE_AVANT, 60);
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            etape_tour_allie = 4;
                    }
                    else if(etape_tour_allie == 4)
                    {//pose la tour, recule pour ne pas renverser la tour
                        mouvement_AX12 = ACTIVE;
                        while(mouvement_AX12 == ACTIVE);
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;
                        //while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0); 
                        avancer_reculer(-250, 60);
                        etape_tour_allie = 5;
                    }
                    else if(etape_tour_allie == 5)
                    {//On range les pinces
                        mouvement_AX12 = ACTIVE;
                        while(mouvement_AX12 == ACTIVE);
                        FLAG_ACTION = POISSONS;
                    }
                    break;
                    
                    
                case TOUR_ADVERSAIRE:
                    if(etape_tour_allie == 0)
                    {//positionne vers la barre, s'encastre dedant
                        
                    }
                    else if(etape_tour_allie == 1)
                    {//attrappe tour et recule (sort de la barre)
                        
                    }
                    else if(etape_tour_allie == 2)
                    {//recule encore, se positionne pour poser la tour
                        
                    }
                    else if(etape_tour_allie == 3)
                    {//avance, va dans la zone de largage, pose la tour
                        
                    }
                    else if(etape_tour_allie == 4)
                    {//recule pour ne pas renverser la tour
                        
                    }
                    break;
                    
                    
                case POISSONS:
                    if(etape_poissons == 0)
                    {//avance vers le le bac à poissons
                        passe_part(1100, 250, MARCHE_AVANT, 60, DEBUT_TRAJECTOIRE);
                        passe_part(1000, 200,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                        passe_part(900, 175,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                        passe_part(800, 157,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                        passe_part(450, 157,MARCHE_AVANT, 80, FIN_TRAJECTOIRE);
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            etape_poissons = 1;
                    }
                    else if(etape_poissons == 1)
                    {//recule, attrape les poissons et relève le filet
                        mouvement_AX12 = ACTIVE;
                        while(mouvement_AX12 == ACTIVE);
                        while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 370);
                        rejoindre(640, 157, MARCHE_ARRIERE, 60);
                        angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);   //Position dans l'eau
                        
                        static uint16_t tempo = 0;
                        while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 685)
                        {
                            tempo++;
                            if(tempo > 10000)
                            {
                                rejoindre(683, 150, MARCHE_ARRIERE, 50);
                            }
                        }

                        rejoindre(790, 150, MARCHE_ARRIERE, 50);

                        angle_AX12(ROT_FILET, 1005, 600, SANS_ATTENTE);  //Position Fin (poissons récupérés)

                        // Gestion du cas ou le filet coince 

                        while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990)
                        {
                            delay_ms(1000);
                            static short Compteur_Bloquage_Filet = 0;
                            if(read_data(ROT_FILET, LIRE_POSITION_ACTU) <= 990 && read_data(ROT_FILET, LIRE_POSITION_ACTU) >= 690)
                            {//Tjrs pas atteint le seuil alors reculer un peu
                                rejoindre(740, 150, MARCHE_AVANT, 35);
                                while(get_X() > 755);
                                Compteur_Bloquage_Filet = 1;
                                delay_ms(100);
                            }
                            if(read_data(ROT_FILET, LIRE_POSITION_ACTU) <= 990 && read_data(ROT_FILET, LIRE_POSITION_ACTU) >= 690 && Compteur_Bloquage_Filet == 1)
                            {//Augmente la patate!
                                angle_AX12(ROT_FILET, 1005, 900, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                delay_ms(200);
                                Compteur_Bloquage_Filet = 2;
                                //delay_ms(200);
                                //angle_AX12(ROT_FILET, 1005, 900, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                            }
                            if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_POSITION_ACTU) >= 690 && Compteur_Bloquage_Filet == 2)   
                            {//Tjr pas atteint le seuil alors faire des accoups
                                //angle_AX12(ROT_FILET, 790, 800, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                unsigned short temp = 0;
                                while(temp < 3)
                                {//Répète le mouvement 3 fois
                                angle_AX12(DEPLOIMENT_BRAS_FILET, 590, 400, SANS_ATTENTE);   //Position déployé
                                delay_ms(100);
                                angle_AX12(DEPLOIMENT_BRAS_FILET, 460, 400, SANS_ATTENTE);   //Position déployé
                                delay_ms(100);
                                temp++;
                                }
                            }
                        }
                        angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 400, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                        uint8_t tempopo = COMPTEUR_TEMPS_MATCH;
                        while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 590)
                        {
                            if((COMPTEUR_TEMPS_MATCH - tempopo) > 2)
                            {
                                angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 600, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                            }
                        }
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            etape_poissons = 2;
                    }
                    else if(etape_poissons == 2)
                    {//évite la barre de de la zone de largage des poissons, dépose les poissons
                        passe_part(1100, 250, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);
                        passe_part(1150, 210,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                        passe_part(1250, 155,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                        passe_part(1300, 158,MARCHE_ARRIERE, 80, FIN_TRAJECTOIRE);
                        orienter(180,100);

                        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 150, SANS_ATTENTE);   //Position déployé
                        angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
                    }
                    break;
                    
                    
                case COQUILLAGES:
                    break;
                    
                    
                case OUVERTURE_PARASOL:
                    break;
                    
                    
                default:
                    break;
            }
            fin_strategie_cause_evitement = 0;    //Reset du Flag qui bloque les déplacement dut à la backup strat
        }
    #endif
  
    #ifdef PETIT_ROBOT
    #endif
}


void homologation()
{
    COULEUR = couleur_depart();

    #ifdef GROS_ROBOT

      // Inits avant démarage du robot :
        init_jack();
        
        while(!SYS_JACK);

        // Démarage du match
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = ON;
        STRATEGIE_EVITEMENT = EVITEMENT_NORMAL;

        init_position_robot(180., 988., 0.);
        
        while(1)
        {
            switch(FLAG_ACTION)
            {
                case TOUR_ALLIE:
                    
                    break;
                    
                case TOUR_ADVERSAIRE:
                    
                    break;
                    
                case POISSONS:
                    
                    break;
                    
                case COQUILLAGES:
                    
                    break;
                    
                default:
                    break;
            }
        }
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
