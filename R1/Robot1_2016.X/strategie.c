/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: srategie.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 07 f�vrier 2015, 22:47
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
        // Inits avant d�marage du robot :
        init_jack();
        init_depart();  //Range tous les �l�ments � sa position initiale

        init_position_robot(78., 1238., 0.);
        
        delay_ms(1000);
        //On se place correctement pour que les 2 robots puissent rentr�s dans la zone de d�part
        rejoindre(380, 1235, MARCHE_AVANT, 40);
        orienter(180, 30);
        rejoindre(230, 1235, MARCHE_AVANT, 40);
        
        while(!SYS_JACK);

        // D�marage du match
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = OFF;//On d�sactive tout les �vitements au d�but pour permettre le 180� au d�but du match
        EVITEMENT_ADV_ARRIERE = OFF;
        STRATEGIE_EVITEMENT = DELAI_ACTION;

        FLAG_ACTION = POISSONS;
        passe_part(700, 1000, MARCHE_ARRIERE, 70, DEBUT_TRAJECTOIRE);//A rajouter si on commence par les poissons
                
        while(1)
        {
            switch(FLAG_ACTION)
            {//Mettre toutes les fonctions de d�placement du robot
                case TOUR_ALLIE:
                    if(ETAPE_TOUR_ALLIE == 0)
                    {//positionne vers la barre, s'encastre dedant
                        passe_part(450, 1235,MARCHE_ARRIERE, 80, DEBUT_TRAJECTOIRE);
                        passe_part(800, 1400,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                        passe_part(890, 1500,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                        passe_part(890, 1600,MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                        orienter(90,100);
                        //rejoindre(890, 1745, MARCHE_AVANT, 40);//895,1745
                        avancer_reculer(155, 60);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 1;
                    }
                    else if(ETAPE_TOUR_ALLIE == 1)
                    {//attrappe tour et recule (sort de la barre)
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
                        while(EVITEMENT_ADV_ARRIERE == OFF);//On s'assure que la variable a �t� bien chang�
                        //while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0);
                        rejoindre(895, 1600, MARCHE_ARRIERE, 40);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 2;
                    }
                    else if(ETAPE_TOUR_ALLIE == 2)
                    {//recule encore, se positionne pour poser la tour
                        passe_part(895, 1500,MARCHE_ARRIERE, 40, DEBUT_TRAJECTOIRE);
                        passe_part(800, 1400,MARCHE_ARRIERE, 40, MILIEU_TRAJECTOIRE);
                        passe_part(500, 950,MARCHE_ARRIERE, 40, FIN_TRAJECTOIRE);
                        ETAPE_TOUR_ALLIE = 3;
                    }
                    else if(ETAPE_TOUR_ALLIE == 3)
                    {//avance, va dans la zone de largage
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = ON;
                        while(EVITEMENT_ADV_AVANT == OFF);//On s'assure que la variable a �t� bien chang�
                        
                        rejoindre(1000, 1000,MARCHE_AVANT, 60);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 4;
                    }
                    else if(ETAPE_TOUR_ALLIE == 4)
                    {//pose la tour, recule pour ne pas renverser la tour
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;
                        while(EVITEMENT_ADV_ARRIERE == OFF);//On s'assure que la variable a �t� bien chang�
                        //while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0); 
                        rejoindre(750, 1000, MARCHE_ARRIERE, 60);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 5;
                    }
                    else if(ETAPE_TOUR_ALLIE == 5)
                    {//On range les pinces
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        FLAG_ACTION = POISSONS;
                    }
                    break;
                    
                    
                case TOUR_ADVERSAIRE:
                    if(ETAPE_TOUR_ALLIE == 0)
                    {//positionne vers la barre, s'encastre dedant
                        
                    }
                    else if(ETAPE_TOUR_ALLIE == 1)
                    {//attrappe tour et recule (sort de la barre)
                        
                    }
                    else if(ETAPE_TOUR_ALLIE == 2)
                    {//recule encore, se positionne pour poser la tour
                        
                    }
                    else if(ETAPE_TOUR_ALLIE == 3)
                    {//avance, va dans la zone de largage, pose la tour
                        
                    }
                    else if(ETAPE_TOUR_ALLIE == 4)
                    {//recule pour ne pas renverser la tour
                        
                    }
                    break;
                    
                
                case POISSONS:
                    if(ETAPE_POISSONS == 0)
                    {//avance vers le le bac � poissons
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = ON;
                        while(EVITEMENT_ADV_AVANT == OFF);
                        
                        passe_part(1100, 300, MARCHE_AVANT, 70, DEBUT_TRAJECTOIRE); 
                        passe_part(950, 250,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                        passe_part(900, 200,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                        passe_part(800, 175,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                        passe_part(470, 150,MARCHE_AVANT, 70, FIN_TRAJECTOIRE);
                        
                        orienter(180,100);//R�oriente bien parall�le � la bordure
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_POISSONS = 1;
                    }
                    
                    
                    else if (ETAPE_POISSONS == 1)
                    {
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_POISSONS = 2;
                    }
                    
                    
                    else if(ETAPE_POISSONS == 2)
                    {//on sort le filet
                        MOUVEMENT_AX12 = ACTIVE;
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;
                        while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer � bouger
                                              
                        ETAPE_POISSONS = 2;
                    }
                    
                    
                    else if(ETAPE_POISSONS == 3)
                    {//on recule un peu
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;
                        while(EVITEMENT_ADV_ARRIERE == OFF);
                        
                        rejoindre(640, 150, MARCHE_ARRIERE, 60);//On se place pr�s du bac
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_POISSONS = 3;
                    }
                    
                    
                    else if(ETAPE_POISSONS == 4)
                    {//On positionne le filet dans le bac
                        EVITEMENT_ADV_ARRIERE = OFF;//On d�sactive l'�vitement car on ne bouge casiment pas donc on ne veu pas d'�vitement inutile
                        EVITEMENT_ADV_AVANT = OFF;
                        
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer � bouger
                        
                        temporisation = COMPTEUR_TEMPS_MATCH;
                        while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 685)
                        {//V�rifie si il est bien dans le bac
                            if((COMPTEUR_TEMPS_MATCH - temporisation) > 2) 
                            {//Si il ne l'est pas au bout de 2 sec alors on avance un peu
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                rejoindre(660, 150, MARCHE_ARRIERE, 50);
                            }
                        }
                        ETAPE_POISSONS = 4;
                    }
                    
                    
                    else if(ETAPE_POISSONS == 5)
                    {//On avance dans le bac et on remonte le filet (on g�re les cas d'impossibilit� de remonter)
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;
                        while(EVITEMENT_ADV_ARRIERE == OFF);//On s'assure que la variable a �t� bien chang�
                        
                        rejoindre(790, 150, MARCHE_ARRIERE, 50);//On se d�place dans tout le bac (pour r�cuperer les poissons)
                        
                        //On d�sactive l'�vitement car on ne veut pas qu'elle bloque la remont� du filet
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = OFF;
                        while(EVITEMENT_ADV_ARRIERE == ON);//On s'assure que la variable a �t� bien chang�
                        
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer � bouger
                        
                        temporisation = COMPTEUR_TEMPS_MATCH;
                        short Compteur_Bloquage_Filet = 0;//Compte tout les bloquages que le filet � eut pour gerer les corrections
                        
                        while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990)
                        {// Gestion du cas ou le filet coince
                            while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990);//Attend 2 secondes tant que le filet n'a pas atteind sa position
                            
                            if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0)
                            {//Tjrs pas atteint le seuil alors reculer un peu
                                EVITEMENT_ADV_ARRIERE = OFF;
                                EVITEMENT_ADV_AVANT = ON;
                                while(EVITEMENT_ADV_AVANT == OFF);//On s'assure que la variable a �t� bien chang�
                                
                                rejoindre(740, 150, MARCHE_AVANT, 35);
                                
                                //On d�sactive encore pour �tre sur de pas g�ner le mouvement du filet
                                EVITEMENT_ADV_ARRIERE = OFF;
                                EVITEMENT_ADV_AVANT = OFF;
                                while(EVITEMENT_ADV_AVANT == ON);//On s'assure que la variable a �t� bien chang�
                                
                                if(get_X() < 750 && fin_strategie_cause_evitement == 0);   //On attend qu'il est atteind la zone � moins qu'il y est �vitement
                                    Compteur_Bloquage_Filet = 1;
                            }
                            else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 1)
                            {//Augmente la patate!
                                angle_AX12(ROT_FILET, 1005, 900, SANS_ATTENTE);  //Position Fin (poissons r�cup�r�s)
                                Compteur_Bloquage_Filet = 2;
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990); //On attend q'il est atteind la zone
                            }
                            else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 2)   
                            {//Tjr pas atteint le seuil alors faire des accoups
                                //angle_AX12(ROT_FILET, 790, 800, SANS_ATTENTE);  //Position Fin (poissons r�cup�r�s)
                                unsigned short temp = 0;
                                while(temp < 3)
                                {//R�p�te le mouvement 3 fois
                                    angle_AX12(DEPLOIMENT_BRAS_FILET, 590, 400, SANS_ATTENTE);   //Position d�ploy�
                                    delay_ms(150);
                                    angle_AX12(DEPLOIMENT_BRAS_FILET, 460, 400, SANS_ATTENTE);   //Position d�ploy�
                                    delay_ms(150);
                                    temp++;
                                }
                            }
                        }
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 400, SANS_ATTENTE);   //Position interm�diaire (pour passer la barre du filet)
                        
                        temporisation = COMPTEUR_TEMPS_MATCH;
                        while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 590 && fin_strategie_cause_evitement == 0)
                        {
                            if((COMPTEUR_TEMPS_MATCH - temporisation) > 2)
                                angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 600, SANS_ATTENTE);   //Position interm�diaire (pour passer la barre du filet)
                            if((COMPTEUR_TEMPS_MATCH - temporisation) > 4)
                                rejoindre(640, 150, MARCHE_AVANT, 60);//On recule un peu pour d�bloquer
                        }
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_POISSONS = 5;
                    }
                    
                    
                    else if(ETAPE_POISSONS == 6)
                    {//�vite la barre de la zone de largage des poissons
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
                        passe_part(1100, 250, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);
                        passe_part(1150, 210,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                        passe_part(1250, 155,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                        passe_part(1300, 158,MARCHE_ARRIERE, 80, FIN_TRAJECTOIRE);
                        orienter(180,100);//R�oriente bien parall�le � la bordure
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_POISSONS = 6;
                            //FLAG_ACTION = NE_RIEN_FAIRE;
                    }
                    
                    
                    else if(ETAPE_POISSONS == 7)
                    {//Depose les poissons
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            FLAG_ACTION = OUVERTURE_PARASOL;
                    }
                    break;
                    
                    
                case COQUILLAGES:
                    break;
                    
                    
                case OUVERTURE_PARASOL: //Le d�ploiment du parasol se fait dans la fonction d'interruption du timer 90sec
                    EVITEMENT_ADV_ARRIERE = OFF;
                    EVITEMENT_ADV_AVANT = OFF;
                    
                    MOUVEMENT_AX12 = ACTIVE;
                    while(MOUVEMENT_AX12 == ACTIVE);
                    break;
                    
                    
                case CACHE_1:
                    if(EVITEMENT_ADV_ARRIERE == ON)
                    {//2 cas en fonction qu'il soit en marche avant ou marche arri�re
                        EVITEMENT_ADV_AVANT = ON;
                        EVITEMENT_ADV_ARRIERE = OFF;
                        rejoindre(300, 1600, MARCHE_AVANT, 50);//On va � la cache 1

                        uint8_t tempo = COMPTEUR_TEMPS_MATCH;//Lance un timer de 2 sec pour attendre un peu avant de r�essayer
                        while((COMPTEUR_TEMPS_MATCH - tempo) < 2);
                        

                        //Relance un peu les pinces pour pas qu'elles laches
                        angle_AX12(PINCE_D, 975, 850, SANS_ATTENTE); //Position o� il attrappe
                        angle_AX12(PINCE_G, 290, 850, SANS_ATTENTE); //Positions o� il attrappe

                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
                        rejoindre(300, 1000, MARCHE_ARRIERE, 50);//on reprend ce qu'on fesait
                    }

                    else
                    {//Si on rencontre quelqu'un en allant se cacher
                        //Alors on fini l'�tape 2 et on passe � la 3
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
                        rejoindre(300, 1000, MARCHE_ARRIERE, 50);//on reprend ce qu'on fesait
                        
                        //Relance un peu les pinces pour pas qu'elles laches
                        angle_AX12(PINCE_D, 975, 850, SANS_ATTENTE); //Position o� il attrappe
                        angle_AX12(PINCE_G, 290, 850, SANS_ATTENTE); //Positions o� il attrappe
                    }
                    
                    FLAG_ACTION = TOUR_ALLIE;
                    break;
                    
                    
                case CACHE_2:
                    
                    break;
                    
                    
                case CACHE_3:
                    
                    break;
                    
                default:
                    break;
            }
            fin_strategie_cause_evitement = 0;    //Reset du Flag qui bloque les d�placement dut � la backup strat
        }
    #endif
  
    #ifdef PETIT_ROBOT
    #endif
}


void homologation()
{
    COULEUR = couleur_depart();

    #ifdef GROS_ROBOT

      // Inits avant d�marage du robot :
        init_jack();
        
        while(!SYS_JACK);

        // D�marage du match
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
