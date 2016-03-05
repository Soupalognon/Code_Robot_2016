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
static uint8_t temporisation = 0;   //Permet les tempo pour les temps d'attentes;

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

        init_position_robot(78., 1238., 0.);
        
        delay_ms(1000);
        //On se place correctement pour que les 2 robots puissent rentrés dans la zone de départ
        rejoindre(380, 1235, MARCHE_AVANT, 40);
        orienter(180, 30);
        rejoindre(230, 1235, MARCHE_AVANT, 40);
        
        
        while(!SYS_JACK);

        // Démarage du match
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = OFF;//On désactive tout les évitements au début pour permettre le 180° au début du match
        EVITEMENT_ADV_ARRIERE = OFF;
        STRATEGIE_EVITEMENT = DELAI_ACTION;

        FLAG_ACTION = POISSONS;
        passe_part(700, 1000, MARCHE_ARRIERE, 70, DEBUT_TRAJECTOIRE);
        while(1)
        {
            switch(FLAG_ACTION)
            {//Mettre toutes les fonctions de déplacement du robot
                case TOUR_ALLIE:
                    if(etape_tour_allie == 0)
                    {//positionne vers la barre, s'encastre dedant
                        passe_part(450, 1235,MARCHE_ARRIERE, 80, DEBUT_TRAJECTOIRE);
                        passe_part(800, 1400,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                        passe_part(895, 1500,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                        passe_part(895, 1600,MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                        orienter(90,100);
                        rejoindre(895, 1745, MARCHE_AVANT, 40);//895,1745
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            etape_tour_allie = 1;
                    }
                    else if(etape_tour_allie == 1)
                    {//attrappe tour et recule (sort de la barre)
                        mouvement_AX12 = ACTIVE;
                        while(mouvement_AX12 == ACTIVE);
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
                        while(EVITEMENT_ADV_ARRIERE == OFF);//On s'assure que la variable a été bien changé
                        //while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0);
                        rejoindre(895, 1600, MARCHE_ARRIERE, 40);
                        etape_tour_allie = 2;
                    }
                    else if(etape_tour_allie == 2)
                    {//recule encore, se positionne pour poser la tour
                        passe_part(895, 1500,MARCHE_ARRIERE, 40, DEBUT_TRAJECTOIRE);
                        passe_part(800, 1400,MARCHE_ARRIERE, 40, MILIEU_TRAJECTOIRE);
                        passe_part(500, 950,MARCHE_ARRIERE, 40, FIN_TRAJECTOIRE);
                        etape_tour_allie = 3;
                    }
                    else if(etape_tour_allie == 3)
                    {//avance, va dans la zone de largage
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = ON;
                        while(EVITEMENT_ADV_AVANT == OFF);//On s'assure que la variable a été bien changé
                        
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
                        while(EVITEMENT_ADV_ARRIERE == OFF);//On s'assure que la variable a été bien changé
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
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = ON;
                        while(EVITEMENT_ADV_AVANT == OFF);
                        passe_part(1100, 300, MARCHE_AVANT, 70, DEBUT_TRAJECTOIRE); 
                        passe_part(950, 250,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                        passe_part(900, 200,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                        passe_part(800, 175,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                        passe_part(470, 150,MARCHE_AVANT, 70, FIN_TRAJECTOIRE);
                        
                        orienter(180,100);//Réoriente bien parallèle à la bordure
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            etape_poissons = 1;
                    }
                    
                    
                    else if(etape_poissons == 1)
                    {//on sort le filet
                        mouvement_AX12 = ACTIVE;
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;
                        while(mouvement_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
                                              
                        etape_poissons = 2;
                    }
                    
                    
                    else if(etape_poissons == 2)
                    {//on recule un peu
                        rejoindre(640, 150, MARCHE_ARRIERE, 60);//On se place près du bac
                        
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            etape_poissons = 3;
                    }
                    
                    
                    else if(etape_poissons == 3)
                    {//On positionne le filet dans le bac
                        mouvement_AX12 = ACTIVE;
                        while(mouvement_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
                        
                        temporisation = COMPTEUR_TEMPS_MATCH;
                        while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 685)
                        {//Vérifie si il est bien dans le bac
                            if((COMPTEUR_TEMPS_MATCH - temporisation) > 2)  //Si il ne l'est pas au bout de 2 sec alors on avance un peu
                                rejoindre(660, 150, MARCHE_ARRIERE, 50);
                        }
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            etape_poissons = 4;
                    }
                    
                    
                    else if(etape_poissons == 4)
                    {//On avance dans le bac et on remonte le filet (on gère les cas d'impossibilité de remonter)
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;
                        while(EVITEMENT_ADV_ARRIERE == OFF);//On s'assure que la variable a été bien changé
                        rejoindre(790, 150, MARCHE_ARRIERE, 50);//On se déplace dans tout le bac (pour récuperer les poissons)
                        
                        mouvement_AX12 = ACTIVE;
                        while(mouvement_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
                        
                        
                        temporisation = COMPTEUR_TEMPS_MATCH;
                        short Compteur_Bloquage_Filet = 0;//Compte tout les bloquages que le filet à eut pour gerer les corrections
                        
                        while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990)
                        {// Gestion du cas ou le filet coince
                            while((COMPTEUR_TEMPS_MATCH - temporisation) < 1 && read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990);//Attend une seconde tant que le filet n'a pas atteind sa position
                            
                            if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0)
                            {//Tjrs pas atteint le seuil alors reculer un peu
                                EVITEMENT_ADV_ARRIERE = OFF;
                                EVITEMENT_ADV_AVANT = ON;
                                while(EVITEMENT_ADV_AVANT == OFF);//On s'assure que la variable a été bien changé
                                
                                rejoindre(740, 150, MARCHE_AVANT, 35);
                                
                                while(get_X() > 755 && fin_strategie_cause_evitement == 0);   //On attend qu'il est atteind la zone à moins qu'il y est évitement
                                Compteur_Bloquage_Filet = 1;
                            }
                            else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 1)
                            {//Augmente la patate!
                                angle_AX12(ROT_FILET, 1005, 900, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                Compteur_Bloquage_Filet = 2;
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                while((COMPTEUR_TEMPS_MATCH - temporisation) < 1 && read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990); //On attend q'il est atteind la zone
                            }
                            else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 2)   
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
                        
                        temporisation = COMPTEUR_TEMPS_MATCH;
                        while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 590)
                        {
                            if((COMPTEUR_TEMPS_MATCH - temporisation) > 2)
                                angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 600, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                            if((COMPTEUR_TEMPS_MATCH - temporisation) > 4)
                                rejoindre(640, 157, MARCHE_ARRIERE, 60);//On recule un peu pour débloquer
                        }
                        
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            etape_poissons = 5;
                    }
                    
                    
                    else if(etape_poissons == 5)
                    {//évite la barre de de la zone de largage des poissons, dépose les poissons
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
                        passe_part(1100, 250, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);
                        passe_part(1150, 210,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                        passe_part(1250, 155,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                        passe_part(1300, 158,MARCHE_ARRIERE, 80, FIN_TRAJECTOIRE);
                        orienter(180,100);//Réoriente bien parallèle à la bordure
                        
                        mouvement_AX12 = ACTIVE;
                        while(mouvement_AX12 == ACTIVE);
                        
                        FLAG_ACTION = NE_RIEN_FAIRE;
                    }
                    break;
                    
                    
                case COQUILLAGES:
                    break;
                    
                    
                case OUVERTURE_PARASOL: //Le déploiment du parasol se fait dans la fonction d'interruption du timer 90sec
                    EVITEMENT_ADV_ARRIERE = OFF;
                    EVITEMENT_ADV_AVANT = OFF;
                    
                    mouvement_AX12 = ACTIVE;
                    while(mouvement_AX12 == ACTIVE);
                    break;
                    
                    
                case CACHE_1:
                    
                    break;
                    
                    
                case CACHE_2:
                    
                    break;
                    
                    
                case CACHE_3:
                    
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
