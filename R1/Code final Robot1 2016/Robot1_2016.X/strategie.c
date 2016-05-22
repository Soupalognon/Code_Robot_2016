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
    uint8_t jaiLaTourouNon = NON;
    
    COULEUR = couleur_depart();
    /*
     * Definit la configuration des coquillages CONFIG_1 - CONFIG_5
     */
    //CONFIG_COQUILLAGE = CONFIG_4;
    
    #ifdef GROS_ROBOT
        // Inits avant démarage du robot :
        init_jack();
        init_depart();  //Range tous les éléments à sa position initiale
        
        int16_t data = read_data(PINCE_D, LIRE_POSITION_ACTU);
        PutIntUART(data);
        while(data == -1)
        {   //Tant qu'il y a une erreur
            data = read_data(PINCE_D, LIRE_POSITION_ACTU);
        }
        
        init_position_robot(78., 1238., 0.);
//        init_position_robot(2922., 1238., 0.);      //Départ côté vert (comme si il etait violet)
        
        delay_ms(1000);
        //On se place correctement pour que les 2 robots puissent rentrés dans la zone de départ
        if(COULEUR == VERT)   //Signifie VIOLET
        {
            rejoindre(380, 1238, MARCHE_AVANT, 40);
            orienter(180, 30);
            rejoindre(230, 1238, MARCHE_AVANT, 40);
        }
        
        while(!SYS_JACK);
//
//        while(1)
//        {
//            lacher_cubes();
//            delay_ms(3000);
//            attrapper_cubes();
//            delay_ms(3000);
////            angle_AX12(ASCENSEUR, 780, 512, SANS_ATTENTE);   //Position nominal
//            delay_ms(1000);
//            avancer_reculer(-200, 3);
//            while(1);
//        }
        
        // Démarage du match
        TIMER_10ms = ACTIVE;
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = OFF;//On désactive tout les évitements au début pour permettre le 180° au début du match
        EVITEMENT_ADV_ARRIERE = OFF;
        STRATEGIE_EVITEMENT = DELAI_ACTION;
        
        
        FLAG_ACTION = TOUR_ADVERSAIRE;
        
        if(COULEUR == VERT)   //Signifie VIOLET
        {
//            passe_part(900, 700, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);//A rajouter si on commence par les poissons coté violet
             passe_part(450, 1238,MARCHE_ARRIERE, 100, DEBUT_TRAJECTOIRE);    //A rajouter si on commence par une des 2 tours coté violet
        }
        if(COULEUR == VIOLET)   //Signifie VERT
            passe_part(450, 1238, MARCHE_AVANT, 100, DEBUT_DEPLACEMENT);//A rajouter si on commence coté vert
        
        while(1)
        {
            switch(FLAG_ACTION)
            {//Mettre toutes les fonctions de déplacement du robot
                case TOUR_ALLIE:
                    if(ETAPE_TOUR_ALLIE == 0)
                    {//positionne vers la barre, s'encastre dedant
                        passe_part(700, 1450,MARCHE_AVANT, 90, DEBUT_DEPLACEMENT);
                        passe_part(750, 1500,MARCHE_AVANT, 90, DEBUT_DEPLACEMENT);
                        passe_part(890, 1500,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                        passe_part(890, 1600,MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                        orienter(90,100);
                        
                         MOUVEMENT_AX12 = ACTIVE;
                         while(MOUVEMENT_AX12 == ACTIVE);
                         
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 50;
                    }
                    
                    else if(ETAPE_TOUR_ALLIE == 50)
                    {
                        rejoindre(890,1730, MARCHE_AVANT, 20);
                        
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                         
//                        avancer_reculer(25, 5);
                        _calage(100,100);
                        
                        ETAPE_TOUR_ALLIE = 1;
                    }
                    
                    else if(ETAPE_TOUR_ALLIE == 1)
                    {//attrappe tour et recule (sort de la barre)
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        delay_ms(1000);
                        
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
                        delay_ms(7);
//                        while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0);
                        rejoindre(890, 1450, MARCHE_ARRIERE, 5);

                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 2;
                    }
                    
                    
                    else if(ETAPE_TOUR_ALLIE == 2)
                    {//Ferme les portes des cabines
                        
//                        FLAG_ACTION = PORTES;

//                        if(CAPT_PINCE == 1) //Si il n'a pas la tour dans les pinces
//                                ETAPE_PORTES = 10;  //On lance une strat spéciale
                        
                        ETAPE_TOUR_ALLIE = 3;
                    }
                    
                    
                    else if(ETAPE_TOUR_ALLIE == 3)
                    {//avance, va dans la zone de largage
                        ACTIVE_ROTATION_AX12 = ACTIVE;
                        EVITEMENT_ADV_ARRIERE = OFF;
                        delay_ms(7);
                        EVITEMENT_ADV_AVANT = OFF;
                        delay_ms(7);
                        
                        passe_part(650, 1450, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
                        EVITEMENT_ADV_AVANT = ON;
                        delay_ms(17);
                        passe_part(700, 1000, MARCHE_AVANT, 90, MILIEU_TRAJECTOIRE);
                        passe_part(800, 1000, MARCHE_AVANT, 90, MILIEU_TRAJECTOIRE);
                        passe_part(1000, 1000, MARCHE_AVANT, 90, FIN_TRAJECTOIRE);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 4;
                    }
                    
                    
                    else if(ETAPE_TOUR_ALLIE == 4)
                    {//pose la tour, recule pour ne pas renverser la tour
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;
                        //while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0); 
                        //rejoindre(950, 1000, MARCHE_ARRIERE, 60);
//                        delay_ms(15000);
                        rejoindre(800, 1000, MARCHE_ARRIERE, 40);

                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 5;
                    }
                    
                    
                    else if(ETAPE_TOUR_ALLIE == 5)
                    {//On range les pinces
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        //orienter(290, 100); //Réoriete le robot pour voir si il y a un adversaire avant de faire les poissons
                        FLAG_ACTION = POISSONS;
                        
                        //Reset des AX12!!!!
                        INHIBIT_AX12 = ETEINT;
                        delay_ms(500);
                        INHIBIT_AX12 = ALLUME;
                        delay_ms(500);
                        init_depart();
                        
                        ETAPE_TOUR_ALLIE = 6;
//                        FLAG_ACTION = NE_RIEN_FAIRE;
                    }
                    break;
                    
                    
                case TOUR_ADVERSAIRE:

                    if(ETAPE_TOUR_ADVERSAIRE == 0)
                    {//positionne vers la barre, s'encastre dedant
                        ACTIVE_ROTATION_AX12 = DESACTIVE;
                        
                        MOUVEMENT_AX12 = ACTIVE;    //Attend qu'il positionne ses US devant lui
                        while(MOUVEMENT_AX12 == ACTIVE);
                        
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = OFF;  //Activé dans l'autom
                        delay_ms(17);
                        
                        passe_part(600, 1500, MARCHE_AVANT, 70, DEBUT_DEPLACEMENT);
                        passe_part(900, 1500, MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                        passe_part(2130, 1500, MARCHE_AVANT, 100, FIN_DEPLACEMENT);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ADVERSAIRE = 50;
                    }
                    
                    
                    else if(ETAPE_TOUR_ADVERSAIRE == 50)
                    {
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = OFF;
                        delay_ms(7);
                        
                        orienter(90,100);
                        MOUVEMENT_AX12 = ACTIVE;
                        delay_ms(500);
                        
                        rejoindre(2130,1720, MARCHE_AVANT, 20);
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        delay_ms(1000);
                        
//                        avancer_reculer(15, 5); //CHANGEMENT DE LA VITESSE 60 TO 40
                        _calage(100,100);
                        
                        if(CAPT_PINCE == 0) //Si tu vois la tour alors enregistre le
                        {
                            jaiLaTourouNon = OUI;
                        }
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ADVERSAIRE = 1;
                    }
                    

                    else if(ETAPE_TOUR_ADVERSAIRE == 1)
                    {//attrappe tour et recule (sort de la barre)
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        delay_ms(1000);
                        
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
//                        while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0);
                        rejoindre(2130, 1450, MARCHE_ARRIERE, 5);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ADVERSAIRE = 2;
                    }
                    
                    
                    else if(ETAPE_TOUR_ADVERSAIRE == 2)
                    {//recule encore, se positionne pour poser la tour
                        orienter(180, 50);
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = ON;
                        rejoindre(500, 1500, MARCHE_AVANT, 100);
                        
                        ACTIVE_ROTATION_AX12 = ACTIVE;
                        
                        if(jaiLaTourouNon == NON) //Si il n'a pas la tour dans les pinces
                        {
                            orienter(0, 50);
                            FLAG_ACTION = TOUR_ALLIE;
                            break;
                        }
                        
                        EVITEMENT_ADV_AVANT = ON;
                        passe_part(700, 1000, MARCHE_AVANT, 90, DEBUT_DEPLACEMENT);
                        passe_part(800, 1000, MARCHE_AVANT, 90, MILIEU_TRAJECTOIRE);
                        passe_part(900, 800, MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                        passe_part(1150, 800, MARCHE_AVANT, 70, FIN_TRAJECTOIRE);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ADVERSAIRE = 3;
                    }
                    
                    
                    else if(ETAPE_TOUR_ADVERSAIRE == 3)
                    {//avance, va dans la zone de largage, pose la tour
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;
                        //while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0); 
                        //rejoindre(950, 1000, MARCHE_ARRIERE, 60);
                        rejoindre(450, 900, MARCHE_ARRIERE, 40);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ADVERSAIRE = 4;
                    }
                    
                    
                    else if(ETAPE_TOUR_ADVERSAIRE == 4)
                    {//recule pour ne pas renverser la tour
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = OFF;
                        
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        //orienter(290, 100); //Réoriete le robot pour voir si il y a un adversaire avant de faire les poissons
                        
                        FLAG_ACTION = TOUR_ALLIE;
                        ETAPE_TOUR_ADVERSAIRE = 5;
                        
                        INHIBIT_AX12 = ETEINT;
                        delay_ms(500);
                        INHIBIT_AX12 = ALLUME;
                        delay_ms(500);
                        init_depart();
                    }
                    break;
                    
                
                case PORTES:
                    if(ETAPE_PORTES == 10)
                    {   //Si on a pas eu la tour
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = OFF;
                        
                        rejoindre(480, 1600, MARCHE_AVANT, 80);
                        orienter(90, 100);
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        
                        //rejoindre(450, 1700, MARCHE_AVANT, 50);
                        avancer_reculer(240, 80);   ///////////////////////////////!!!!A MODIFIER!!!!
//                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 11;
                    }

                    else if(ETAPE_PORTES == 11)
                    {
                        rejoindre(480, 1600, MARCHE_ARRIERE, 50);
                        angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE);        //Position rangé
                        angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE);        //Position rangé
                        
//                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            FLAG_ACTION = TOUR_ALLIE;
                    }
                     
                    else if(ETAPE_PORTES == 0)
                    {   //Ferme la porte 1  //Si on a la tour dans les pinces
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = OFF;
                        passe_part(600, 1500, MARCHE_AVANT, 70, DEBUT_DEPLACEMENT);
//                        passe_part(600, 1860, MARCHE_AVANT, 50, FIN_DEPLACEMENT);
                        passe_part(600, 1850, MARCHE_AVANT, 50, FIN_DEPLACEMENT);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 1;
                    }
                    
                    
                    else if(ETAPE_PORTES == 1)
                    {   //Ferme la porte 2
                        EVITEMENT_ADV_ARRIERE = ON;
                        rejoindre(600, 1500, MARCHE_ARRIERE, 70);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 2;
                    }
                    
                    
                    else if(ETAPE_PORTES == 2)
                    {
                        EVITEMENT_ADV_ARRIERE = OFF;
                        //EVITEMENT_ADV_AVANT = ON;
                        passe_part(305, 1500, MARCHE_AVANT, 70, DEBUT_DEPLACEMENT);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 3;
                    }
                    
                    
                    else if(ETAPE_PORTES == 3)
                    {
                        EVITEMENT_ADV_ARRIERE = OFF;
                        passe_part(310, 1950, MARCHE_AVANT, 50, DEBUT_DEPLACEMENT);
//                        passe_part(310, 1900, MARCHE_AVANT, 50, DEBUT_DEPLACEMENT);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 4;
                    }
                    
                    
                    else if(ETAPE_PORTES == 4)
                    {
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;

                        passe_part(320, 1500, MARCHE_ARRIERE, 70, DEBUT_DEPLACEMENT);
                        passe_part(320, 1500, MARCHE_ARRIERE, 70, FIN_DEPLACEMENT);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            FLAG_ACTION = TOUR_ALLIE;
                    }
                    break;
                    
                    
                case POISSONS:
                    /***********************************************************************/
                    /***********************************************************************/
                    if(COULEUR == VERT)         //Signifie VIOLET
                    {
                        if(ETAPE_POISSONS == 10)
                        {
                            EVITEMENT_ADV_AVANT = ON;
                            EVITEMENT_ADV_ARRIERE = OFF;

                            passe_part(1100, 300, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 11;
                        }

                        else if(ETAPE_POISSONS == 11)
                        {
                            passe_part(950, 250,MARCHE_AVANT, 60, DEBUT_TRAJECTOIRE);
                            passe_part(900, 200,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                            passe_part(800, 175,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
                            passe_part(470, 170,MARCHE_AVANT, 70, FIN_TRAJECTOIRE);
//                            passe_part(470, 150,MARCHE_AVANT, 70, FIN_TRAJECTOIRE);
                            orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 1;
                        }

                        if(ETAPE_POISSONS == 0)
                        {//avance vers le le bac à poissons
                            
                            EVITEMENT_ADV_ARRIERE = ON;
                            delay_ms(17);
                            EVITEMENT_ADV_AVANT = OFF;
                            
                            passe_part(500, 900, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);
                            passe_part(300, 700, MARCHE_ARRIERE, 50, MILIEU_TRAJECTOIRE);

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 1;
                        }

                        else if(ETAPE_POISSONS == 1)
                        {
                            passe_part(200, 500, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);
                            passe_part(200, 320, MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                            passe_part(300, 270, MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                            passe_part(350, 250, MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                            passe_part(430, 250, MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                            passe_part(450, 220, MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                            passe_part(470, 200, MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                            passe_part(500, 180, MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                            passe_part(530, 185, MARCHE_ARRIERE, 80, FIN_TRAJECTOIRE);
                            rejoindre(450, 160, MARCHE_AVANT, 50);          //!!!  ATTENTION - Il y a une translation donc un décalage d'odométrie - faire un calage   !!!

    //                        passe_part(1100, 300, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
    //                        passe_part(950, 250,MARCHE_AVANT, 60, MILIEU_TRAJECTOIRE);
    //                        passe_part(900, 200,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
    //                        passe_part(800, 175,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
    //                        passe_part(470, 150,MARCHE_AVANT, 70, FIN_TRAJECTOIRE);

    //                        orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 2;
                        }

                        else if (ETAPE_POISSONS == 2)
                        {
                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 3;
                        }


                        else if(ETAPE_POISSONS == 3)
                        {//on sort le filet
                            EVITEMENT_ADV_ARRIERE = OFF;
                            EVITEMENT_ADV_AVANT = OFF;

                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
                            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 365);//On attend que le dernière AX12 est fini de bouger

                            ETAPE_POISSONS = 4;
                        }


                        else if(ETAPE_POISSONS == 4)
                        {//on recule un peu
                            EVITEMENT_ADV_ARRIERE = OFF;
                            EVITEMENT_ADV_AVANT = OFF;

                            rejoindre(580, 160, MARCHE_ARRIERE, 60);//On se place près du bac

                            orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 5;
                        }


                        else if(ETAPE_POISSONS == 5)
                        {//On positionne le filet dans le bac
                            EVITEMENT_ADV_ARRIERE = OFF;//On désactive l'évitement car on ne bouge casiment pas donc on ne veu pas d'évitement inutile
                            EVITEMENT_ADV_AVANT = OFF;

                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger

                            temporisation = COMPTEUR_TEMPS_MATCH;
                            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 685)
                            {//Vérifie si il est bien dans le bac
                                if((COMPTEUR_TEMPS_MATCH - temporisation) > 1)
                                {//Si il ne l'est pas au bout de 2 sec alors on avance un peu
                                    //temporisation = COMPTEUR_TEMPS_MATCH;
                                    angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
                                    while(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 385);
//                                    rejoindre(625, 160, MARCHE_ARRIERE, 50);
                                      avancer_reculer(-25, 20);
                                    angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);  //Position dans l'eau
                                    temporisation = COMPTEUR_TEMPS_MATCH;

                                }
                            }
                            ETAPE_POISSONS = 6;
                        }


                        else if(ETAPE_POISSONS == 6)
                        {//On avance dans le bac et on remonte le filet (on gère les cas d'impossibilité de remonter)
                            EVITEMENT_ADV_ARRIERE = ON;
                            EVITEMENT_ADV_AVANT = OFF;

//                            rejoindre(790, 150, MARCHE_ARRIERE, 50);//On se déplace dans tout le bac (pour récuperer les poissons)
                            rejoindre(785, 160, MARCHE_ARRIERE, 50);//On se déplace dans tout le bac (pour récuperer les poissons)

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            {
                                //On désactive l'évitement car on ne veut pas qu'elle bloque la remonté du filet
                                EVITEMENT_ADV_ARRIERE = OFF;
                                EVITEMENT_ADV_AVANT = OFF;

                                MOUVEMENT_AX12 = ACTIVE;
                                while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
                                
                                
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                short Compteur_Bloquage_Filet = 0;//Compte tout les bloquages que le filet à eut pour gerer les corrections

                                while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990)
                                {// Gestion du cas ou le filet coince
                                    while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990);//Attend 2 secondes tant que le filet n'a pas atteind sa position

                                    if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0)
                                    {//Tjrs pas atteint le seuil alors reculer un peu

                                        rejoindre(740, 160, MARCHE_AVANT, 35);

                                        if(get_X() < 750 && fin_strategie_cause_evitement == 0);   //On attend qu'il est atteind la zone à moins qu'il y est évitement
                                            Compteur_Bloquage_Filet = 1;
                                    }
                                    else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 1)
                                    {//Augmente la patate!
                                        angle_AX12(ROT_FILET, 1005, 900, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                        Compteur_Bloquage_Filet = 2;
                                        temporisation = COMPTEUR_TEMPS_MATCH;
                                        while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990); //On attend q'il est atteind la zone
                                    }
                                    else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 2)   
                                    {//Tjr pas atteint le seuil alors faire des accoups
                                        //angle_AX12(ROT_FILET, 790, 800, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                        unsigned short temp = 0;
                                        while(temp < 3)
                                        {//Répète le mouvement 3 fois
                                            angle_AX12(DEPLOIMENT_BRAS_FILET, 660, 400, SANS_ATTENTE);   //Position déployé
                                            delay_ms(150);
                                            angle_AX12(DEPLOIMENT_BRAS_FILET, 700, 400, SANS_ATTENTE);   //Position intermédiaire (pour passé la barre du filet)
                                            delay_ms(150);
                                            temp++;
                                        }
                                    }
                                }

                                angle_AX12(DEPLOIMENT_BRAS_FILET, 705, 400, SANS_ATTENTE);   //Position intermédiaire (pour passé la barre du filet)
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 690 && fin_strategie_cause_evitement == 0)
                                {
                                    if((COMPTEUR_TEMPS_MATCH - temporisation) > 2)
                                        angle_AX12(DEPLOIMENT_BRAS_FILET, 705, 600, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                                    if((COMPTEUR_TEMPS_MATCH - temporisation) > 4)
                                        rejoindre(640, 160, MARCHE_AVANT, 60);//On recule un peu pour débloquer
                                }

                                ETAPE_POISSONS = 7;
                            }
                        }


                        else if(ETAPE_POISSONS == 7)
                        {//évite la barre de la zone de largage des poissons
                            EVITEMENT_ADV_AVANT = OFF;
                            EVITEMENT_ADV_ARRIERE = ON;
                            passe_part(1100, 250, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);
                            passe_part(1150, 210,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                            passe_part(1250, 155,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
                            passe_part(1300, 150, MARCHE_ARRIERE, 70, FIN_TRAJECTOIRE);
                            rejoindre(1240, 140, MARCHE_AVANT, 50);
                            orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 8;
                                //FLAG_ACTION = NE_RIEN_FAIRE;
                        }


                        else if(ETAPE_POISSONS == 8)
                        {//Depose les poissons
                            EVITEMENT_ADV_AVANT = OFF;
                            EVITEMENT_ADV_ARRIERE = OFF;
                            
                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);
                            
                            delay_ms(1000);
                            FLAG_ACTION = TRANSITION_POISSON_TOUR_ADVERSAIRE;
                        }
                    }
                    
                    /****************************************************************************************************************/
                    /****************************************************************************************************************/
                    /****************************************************************************************************************/
                    /****************************************************************************************************************/
                    /****************************************************************************************************************/
                    if(COULEUR == VIOLET)//Signifie VERT     /***********************************************************************/
                    {
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 != ACTIVE);
//                        STRATEGIE_EVITEMENT = STOP;
                        
                        if(ETAPE_POISSONS == 10)
                        {
                            EVITEMENT_ADV_AVANT = ON;
                            EVITEMENT_ADV_ARRIERE = OFF;

                            passe_part(1100, 300, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
                            passe_part(1200, 300, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 11;
                        }

                        else if(ETAPE_POISSONS == 11)
                        {
                            EVITEMENT_ADV_AVANT = OFF;
                            EVITEMENT_ADV_ARRIERE = ON;
                            
                            passe_part(950, 250,MARCHE_ARRIERE, 30, DEBUT_TRAJECTOIRE);
                            passe_part(900, 200,MARCHE_ARRIERE, 30, MILIEU_TRAJECTOIRE);
                            passe_part(500, 175,MARCHE_ARRIERE, 30, MILIEU_TRAJECTOIRE);
                            passe_part(350, 180,MARCHE_ARRIERE, 30, FIN_TRAJECTOIRE);
                            orienter(0,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 1;
                        }

                        else if(ETAPE_POISSONS == 0)
                        {//avance vers le le bac à poissons
//                            EVITEMENT_ADV_ARRIERE = OFF;
//                            delay_ms(17);
                            orienter(255, 50);  //essai de ne pas viser la zone centrale
                            
                            EVITEMENT_ADV_AVANT = ON;
                            delay_ms(17);
//                            passe_part(400, 1238, MARCHE_ARRIERE, 80, DEBUT_TRAJECTOIRE);
                            passe_part(400, 800, MARCHE_AVANT, 50, DEBUT_DEPLACEMENT);
                            passe_part(300, 600, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 1;
                        }

                        else if(ETAPE_POISSONS == 1)
                        {
//                            EVITEMENT_ADV_AVANT = OFF;
                            delay_ms(17);
//                            EVITEMENT_ADV_ARRIERE = OFF;
//                            delay_ms(17);
                            
                            passe_part(300, 350, MARCHE_AVANT, 80, DEBUT_TRAJECTOIRE);
                            passe_part(350, 300, MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                            passe_part(400, 250, MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                            passe_part(450, 210, MARCHE_AVANT, 80, FIN_TRAJECTOIRE);
                            rejoindre(500, 180, MARCHE_AVANT, 100);          /*!!!  ATTENTION - Il y a une translation donc un décalage d'odométrie - faire un calage   !!!*/
                            orienter(0,100);//Réoriente bien parallèle à la bordure
                            
                            avancer_reculer(-150, 100);
                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 2;
                        }

                        else if (ETAPE_POISSONS == 2)
                        {
                                ETAPE_POISSONS = 3;
                        }


                        else if(ETAPE_POISSONS == 3)
                        {//on sort le filet
//                            EVITEMENT_ADV_ARRIERE = OFF;
//                            delay_ms(17);
                            EVITEMENT_ADV_AVANT = OFF;
                            delay_ms(17);

                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
//                            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 365);//On attend que le dernière AX12 est fini de bouger

                            ETAPE_POISSONS = 4;
                        }


                        else if(ETAPE_POISSONS == 4)
                        {//on recule un peu
//                            EVITEMENT_ADV_ARRIERE = OFF;
//                            delay_ms(17);
                            EVITEMENT_ADV_AVANT = OFF;
                            delay_ms(17);

                            rejoindre(640, 170, MARCHE_AVANT, 60);//On se place près du bac
                            orienter(0,100);//Réoriente bien parallèle à la bordure
                            //orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 5;
                        }


                        else if(ETAPE_POISSONS == 5)
                        {//On positionne le filet dans le bac
//                            EVITEMENT_ADV_ARRIERE = OFF;//On désactive l'évitement car on ne bouge casiment pas donc on ne veu pas d'évitement inutile
//                            delay_ms(17);
                            EVITEMENT_ADV_AVANT = OFF;
                            delay_ms(17);

                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
                            
                            temporisation = COMPTEUR_TEMPS_MATCH;
                            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 705)
                            {//Vérifie si il est bien dans le bac
                                if((COMPTEUR_TEMPS_MATCH - temporisation) > 1)
                                {//Si il ne l'est pas au bout de 2 sec alors on avance un peu
                                    temporisation = COMPTEUR_TEMPS_MATCH;
                                    angle_AX12(ROT_FILET, 1005, 300, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                    while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 995);
//                                    rejoindre(650, 170, MARCHE_AVANT, 50);
                                    avancer_reculer(25, 20);
                                    angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);  //Position dans l'eau
                                    while(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 705); 

                                }
                            }
                            ETAPE_POISSONS = 6;
                        }


                        else if(ETAPE_POISSONS == 6)
                        {//On avance dans le bac et on remonte le filet (on gère les cas d'impossibilité de remonter)
//                            EVITEMENT_ADV_ARRIERE = OFF;
//                            delay_ms(17);
                            EVITEMENT_ADV_AVANT = ON;
                            delay_ms(17);

                            rejoindre(820, 170, MARCHE_AVANT, 50);//On se déplace dans tout le bac (pour récuperer les poissons)
                            
                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            {
                                //On désactive l'évitement car on ne veut pas qu'elle bloque la remonté du filet
                                EVITEMENT_ADV_AVANT = OFF;
                                delay_ms(17);
                                
//                                MOUVEMENT_AX12 = ACTIVE;
//                                while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
//                                while(get_X() < 807);
                                //Correction d'un bug chelou... le mouvement s'activait directement pendant le rejoindre précédent... (seulement pour le coté vert)
                                angle_AX12(ROT_FILET, 375, 600, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
                                
                                
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                short Compteur_Bloquage_Filet = 0;//Compte tout les bloquages que le filet à eut pour gerer les corrections

                                while(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 390)
                                {// Gestion du cas ou le filet coince
                                    while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) > 390);//Attend 2 secondes tant que le filet n'a pas atteind sa position

                                    if(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 390 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0)
                                    {//Tjrs pas atteint le seuil alors reculer un peu
                                        EVITEMENT_ADV_ARRIERE = OFF;
                                        EVITEMENT_ADV_AVANT = OFF;

                                        rejoindre(740, 170, MARCHE_ARRIERE, 35);

                                        //On désactive encore pour être sur de pas géner le mouvement du filet
                                        EVITEMENT_ADV_ARRIERE = OFF;
                                        EVITEMENT_ADV_AVANT = OFF;

                                        if(get_X() < 750 && fin_strategie_cause_evitement == 0);   //On attend qu'il est atteind la zone à moins qu'il y est évitement
                                            Compteur_Bloquage_Filet = 1;
                                    }
                                    
                                    else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 390 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 1)
                                    {//Augmente la patate!
                                        angle_AX12(ROT_FILET, 375, 900, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
                                        Compteur_Bloquage_Filet = 2;
                                        temporisation = COMPTEUR_TEMPS_MATCH;
                                        while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) > 415); //On attend q'il est atteind la zone
                                    }
                                    
                                    else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 390 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 2)   
                                    {//Tjr pas atteint le seuil alors faire des accoups
                                        //angle_AX12(ROT_FILET, 790, 800, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                        unsigned short temp = 0;
                                        while(temp < 3)
                                        {//Répète le mouvement 3 fois
                                            angle_AX12(DEPLOIMENT_BRAS_FILET, 660, 400, SANS_ATTENTE);   //Position déployé
                                            delay_ms(150);
                                            angle_AX12(DEPLOIMENT_BRAS_FILET, 700, 400, SANS_ATTENTE);   //Position intermédiaire (pour passé la barre du filet)
                                            delay_ms(150);
                                            temp++;
                                        }
                                    }
                                }

                                angle_AX12(DEPLOIMENT_BRAS_FILET, 705, 400, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 690 && fin_strategie_cause_evitement == 0)
                                {
                                    if((COMPTEUR_TEMPS_MATCH - temporisation) > 2)
                                        angle_AX12(DEPLOIMENT_BRAS_FILET, 705, 600, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                                    if((COMPTEUR_TEMPS_MATCH - temporisation) > 4)
                                        rejoindre(720, 160, MARCHE_ARRIERE, 40);//On recule un peu pour débloquer
                                }

                                ETAPE_POISSONS = 7;
                            }
                        }


                        else if(ETAPE_POISSONS == 7)
                        {//évite la barre de la zone de largage des poissons
                            EVITEMENT_ADV_AVANT = ON;
                            delay_ms(17);
//                            EVITEMENT_ADV_ARRIERE = OFF;
//                            delay_ms(17);
                            passe_part(1100, 250, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
                            passe_part(1150, 210,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                            passe_part(1250, 155,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                            passe_part(1300, 150, MARCHE_AVANT, 70, FIN_TRAJECTOIRE);
                            
                            EVITEMENT_ADV_AVANT = OFF;
                            delay_ms(17);
                            
                            rejoindre(1240, 140, MARCHE_ARRIERE, 50);
                            orienter(0,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 8;
                                //FLAG_ACTION = NE_RIEN_FAIRE;
                        }


                        else if(ETAPE_POISSONS == 8)
                        {//Depose les poissons
                            EVITEMENT_ADV_AVANT = OFF;
                            delay_ms(17);
//                            EVITEMENT_ADV_ARRIERE = OFF;
//                            delay_ms(17);
                            
                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);
                            
                            delay_ms(1000);
                            FLAG_ACTION = TRANSITION_POISSON_TOUR_ADVERSAIRE;
//                            ETAPE_POISSONS = 9;
                        }
                    }
                    break;
                    
                    
                case COQUILLAGES:
                    break;
                    
                    
                case OUVERTURE_PARASOL: //Le déploiment du parasol se fait dans la fonction d'interruption du timer 90sec
                    EVITEMENT_ADV_ARRIERE = OFF;
                    EVITEMENT_ADV_AVANT = OFF;
                    
                    MOUVEMENT_AX12 = ACTIVE;
                    while(MOUVEMENT_AX12 == ACTIVE);
                    break;
                    
                    
                case DEFONCER_DUNE:
                    ACTIVE_ROTATION_AX12 = DESACTIVE;
                    MOUVEMENT_AX12 = ACTIVE;    //Attend qu'il positionne ses US devant lui
                        
                    EVITEMENT_ADV_ARRIERE = OFF;
                    
                    passe_part(800, 1500,MARCHE_AVANT, 100, DEBUT_DEPLACEMENT);
                    passe_part(895, 1500,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                    passe_part(900, 1600,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                    passe_part(950, 1700,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
                    passe_part(1150, 1810,MARCHE_AVANT, 90, MILIEU_TRAJECTOIRE);
                    passe_part(1200, 1830,MARCHE_AVANT, 90, MILIEU_TRAJECTOIRE);
                    passe_part(1230, 1830,MARCHE_AVANT, 70, FIN_DEPLACEMENT);
                    orienter(0, 100);
//                    avancer_reculer(10, 100);
//                    passe_part(1200, 1800,MARCHE_ARRIERE, 30, DEBUT_DEPLACEMENT);
                    passe_part(1150, 1800,MARCHE_ARRIERE, 50, DEBUT_DEPLACEMENT);
                    passe_part(895, 1400,MARCHE_ARRIERE, 70, MILIEU_TRAJECTOIRE);
                    passe_part(800, 1400,MARCHE_ARRIERE, 70, FIN_DEPLACEMENT);
                    FLAG_ACTION = TOUR_ALLIE;
//                    ETAPE_TOUR_ALLIE = 0;
                    break;
                    
                case TRANSITION_POISSON_TOUR_ADVERSAIRE:
                    ranger_filet();
                    
                    if(COULEUR == VERT)     //Signifie VIOLET
                    {
                        rejoindre(1200, 150, MARCHE_AVANT, 50);
                        rejoindre(1100, 160, MARCHE_AVANT, 50);
                        rejoindre(1050, 180, MARCHE_AVANT, 50);
                        rejoindre(1000, 200, MARCHE_AVANT, 50);
                        rejoindre(750, 500, MARCHE_AVANT, 50);
                    }
                    else if(COULEUR == VIOLET)     //Signifie VERT
                    {
                        rejoindre(1100, 170, MARCHE_ARRIERE, 50);
                        rejoindre(1050, 180, MARCHE_ARRIERE, 50);
                        rejoindre(1000, 200, MARCHE_ARRIERE, 50);
                        rejoindre(500, 500, MARCHE_ARRIERE, 50);
                        rejoindre(750, 500, MARCHE_AVANT, 50);
                    }
                    
                    FLAG_ACTION = TOUR_ADVERSAIRE;
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
    /*
     * Definit la configuration des coquillages CONFIG_1 - CONFIG_5
     */
    CONFIG_COQUILLAGE = CONFIG_3;
    
    #ifdef GROS_ROBOT
        // Inits avant démarage du robot :
        init_jack();
        init_depart();  //Range tous les éléments à sa position initiale
        
        int16_t data = read_data(PINCE_D, LIRE_POSITION_ACTU);
        PutIntUART(data);
        while(data == -1)
        {   //Tant qu'il y a une erreur
            data = read_data(PINCE_D, LIRE_POSITION_ACTU);
        }
        
        init_position_robot(78., 1238., 0.);
//        init_position_robot(2922., 1238., 0.);      //Départ côté vert (comme si il etait violet)
        
        delay_ms(1000);
        //On se place correctement pour que les 2 robots puissent rentrés dans la zone de départ
        if(COULEUR == VERT)   //Signifie VIOLET
        {
        rejoindre(380, 1238, MARCHE_AVANT, 40);
        orienter(180, 30);
        rejoindre(230, 1238, MARCHE_AVANT, 40);
        }
        
        while(!SYS_JACK);

        // Démarage du match
        TIMER_10ms = ACTIVE;
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = OFF;//On désactive tout les évitements au début pour permettre le 180° au début du match
        EVITEMENT_ADV_ARRIERE = OFF;
        STRATEGIE_EVITEMENT = STOP;
        
        FLAG_ACTION = TOUR_ALLIE;
//        passe_part(1000, 700, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);//A rajouter si on commence par les poissons coté violet
        if(COULEUR == VERT)   //Signifie VIOLET
            passe_part(450, 1238,MARCHE_ARRIERE, 100, DEBUT_TRAJECTOIRE);    //A rajouter si on commence par une des 2 tours coté violet
        if(COULEUR == VIOLET)   //Signifie VERT
            passe_part(450, 1238, MARCHE_AVANT, 100, DEBUT_DEPLACEMENT);//A rajouter si on commence coté vert
        
        while(1)
        {
            switch(FLAG_ACTION)
            {
                case TOUR_ALLIE:
                    if(ETAPE_TOUR_ALLIE == 0)
                    {//positionne vers la barre, s'encastre dedant
                        passe_part(800, 1400,MARCHE_AVANT, 40, DEBUT_DEPLACEMENT);
//                        passe_part(890, 1500,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
//                        passe_part(890, 1600,MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
                        passe_part(890, 1500,MARCHE_AVANT, 40, MILIEU_TRAJECTOIRE);
                        passe_part(890, 1600,MARCHE_AVANT, 30, FIN_TRAJECTOIRE);
                        orienter(90,30);
                        if(COULEUR == VERT) //Signifie Violet
                            avancer_reculer(156, 20); //CHANGEMENT DE LA VITESSE 60 TO 40
                        if(COULEUR == VIOLET) //Signifie Vert
                            avancer_reculer(160, 20); //CHANGEMENT DE LA VITESSE 60 TO 40
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 1;
                    }
                    
                    
                    else if(ETAPE_TOUR_ALLIE == 1)
                    {//attrappe tour et recule (sort de la barre)
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        delay_ms(2000);
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = ON;
                        delay_ms(17);
//                        while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0);
                        rejoindre(890, 1450, MARCHE_ARRIERE, 30);

                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 2;
                    }
                    
                    
                    else if(ETAPE_TOUR_ALLIE == 2)
                    {//Ferme les portes des cabines
//                        if(CONFIG_COQUILLAGE != CONFIG_5)
//                            FLAG_ACTION = PORTES;
//                        if(CAPT_PINCE == 1) //Si il n'a pas la tour dans les pinces
//                                ETAPE_PORTES = 10;  //On lance une strat spéciale
////                        
                        ETAPE_TOUR_ALLIE = 3;
                    }
                    
                    
                    else if(ETAPE_TOUR_ALLIE == 3)
                    {//avance, va dans la zone de largage
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = OFF;
                        passe_part(650, 1500, MARCHE_AVANT, 40, DEBUT_TRAJECTOIRE);
                        EVITEMENT_ADV_AVANT = ON;
                        passe_part(700, 1000, MARCHE_AVANT, 40, MILIEU_TRAJECTOIRE);
                        passe_part(800, 1000, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                        passe_part(1000, 1000, MARCHE_AVANT, 50, FIN_TRAJECTOIRE);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 4;
                    }
                    
                    
                    else if(ETAPE_TOUR_ALLIE == 4)
                    {//pose la tour, recule pour ne pas renverser la tour
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        
                        EVITEMENT_ADV_ARRIERE = OFF;
                        EVITEMENT_ADV_AVANT = OFF;
                        //while(read_data(PINCE_D, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_G, LIRE_MOUV_FLAG) != 0); 
                        //rejoindre(950, 1000, MARCHE_ARRIERE, 60);
                        rejoindre(800, 1000, MARCHE_ARRIERE, 40);
                        
                        //temporisation = COMPTEUR_TEMPS_MATCH;//Lance un timer de 2 sec pour attendre un peu avant de réessayer
                        //while((COMPTEUR_TEMPS_MATCH - temporisation) < 4);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_TOUR_ALLIE = 5;
                    }
                    
                    
                    else if(ETAPE_TOUR_ALLIE == 5)
                    {//On range les pinces
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        //orienter(290, 100); //Réoriete le robot pour voir si il y a un adversaire avant de faire les poissons
                        FLAG_ACTION = NE_RIEN_FAIRE;
                        
//                        delay_ms(5000);
//                        
//                        ETAPE_POISSONS = 10;
//                        FLAG_ACTION = NE_RIEN_FAIRE;
                    }
                    break;
                    
                    
                    
                    case PORTES:
                    if(ETAPE_PORTES == 10)
                    {   //Si on a pas eu la tour
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = OFF;
                        
                        rejoindre(480, 1600, MARCHE_AVANT, 60);
                        orienter(90, 50);
                        MOUVEMENT_AX12 = ACTIVE;
                        while(MOUVEMENT_AX12 == ACTIVE);
                        
                        //rejoindre(450, 1700, MARCHE_AVANT, 50);
                        avancer_reculer(240, 50);   ///////////////////////////////!!!!A MODIFIER!!!!
//                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 11;
                    }

                    else if(ETAPE_PORTES == 11)
                    {
                        rejoindre(480, 1600, MARCHE_ARRIERE, 50);
                        angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE);        //Position rangé
                        angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE);        //Position rangé
                        
//                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            FLAG_ACTION = TOUR_ALLIE;
                    }
                    
                    else if(ETAPE_PORTES == 0)
                    {   //Ferme la porte 1  //Si on a la tour dans les pinces
                        EVITEMENT_ADV_AVANT = OFF;
                        EVITEMENT_ADV_ARRIERE = OFF;
                        passe_part(600, 1500, MARCHE_AVANT, 50, DEBUT_DEPLACEMENT);
//                        passe_part(600, 1860, MARCHE_AVANT, 50, FIN_DEPLACEMENT);
                        passe_part(600, 1850, MARCHE_AVANT, 50, FIN_DEPLACEMENT);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 1;
                    }
                    
                    
                    else if(ETAPE_PORTES == 1)
                    {   //Ferme la porte 2
                        EVITEMENT_ADV_ARRIERE = ON;
                        rejoindre(600, 1500, MARCHE_ARRIERE, 50);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 2;
                    }
                    
                    
                    else if(ETAPE_PORTES == 2)
                    {
                        EVITEMENT_ADV_ARRIERE = OFF;
                        //EVITEMENT_ADV_AVANT = ON;
                        passe_part(305, 1500, MARCHE_AVANT, 50, DEBUT_DEPLACEMENT);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 3;
                    }
                    
                    
                    else if(ETAPE_PORTES == 3)
                    {
                        EVITEMENT_ADV_ARRIERE = OFF;
                        passe_part(310, 1950, MARCHE_AVANT, 50, DEBUT_DEPLACEMENT);
//                        passe_part(310, 1900, MARCHE_AVANT, 50, DEBUT_DEPLACEMENT);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            ETAPE_PORTES = 4;
                    }
                    
                    
                    else if(ETAPE_PORTES == 4)
                    {
                        EVITEMENT_ADV_ARRIERE = ON;
                        EVITEMENT_ADV_AVANT = OFF;

                        passe_part(320, 1500, MARCHE_ARRIERE, 50, DEBUT_DEPLACEMENT);
                        passe_part(320, 1500, MARCHE_ARRIERE, 50, FIN_DEPLACEMENT);
                        
                        if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            FLAG_ACTION = TOUR_ALLIE;
                    }
                    break;
                    
                    
                    case POISSONS:
                    /***********************************************************************/
                    /***********************************************************************/
                    if(COULEUR == VERT)         //Signifie VIOLET
                    {
                        if(ETAPE_POISSONS == 10)
                        {
                            EVITEMENT_ADV_AVANT = ON;
                            EVITEMENT_ADV_ARRIERE = OFF;

                            passe_part(1100, 300, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 11;
                        }

                        else if(ETAPE_POISSONS == 11)
                        {
                            passe_part(950, 250,MARCHE_AVANT, 40, DEBUT_TRAJECTOIRE);
                            passe_part(900, 200,MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                            passe_part(800, 175,MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                            passe_part(470, 150,MARCHE_AVANT, 50, FIN_TRAJECTOIRE);
                            orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 1;
                        }

                        if(ETAPE_POISSONS == 0)
                        {//avance vers le le bac à poissons
                            orienter(260,50);
                            
                            EVITEMENT_ADV_ARRIERE = ON;
                            EVITEMENT_ADV_AVANT = OFF;
                            
                            passe_part(500, 900, MARCHE_ARRIERE, 60, DEBUT_TRAJECTOIRE);
                            passe_part(300, 700, MARCHE_ARRIERE, 60, MILIEU_TRAJECTOIRE);
                                    
                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 1;
                        }

                        else if(ETAPE_POISSONS == 1)
                        {
                            EVITEMENT_ADV_AVANT = OFF;
                            EVITEMENT_ADV_ARRIERE = ON;
                            passe_part(200, 500, MARCHE_ARRIERE, 60, DEBUT_TRAJECTOIRE);
                            passe_part(200, 320, MARCHE_ARRIERE, 60, MILIEU_TRAJECTOIRE);
                            passe_part(300, 270, MARCHE_ARRIERE, 60, MILIEU_TRAJECTOIRE);
                            passe_part(350, 250, MARCHE_ARRIERE, 60, MILIEU_TRAJECTOIRE);
                            passe_part(430, 250, MARCHE_ARRIERE, 60, MILIEU_TRAJECTOIRE);
                            passe_part(450, 220, MARCHE_ARRIERE, 60, MILIEU_TRAJECTOIRE);
                            passe_part(470, 200, MARCHE_ARRIERE, 60, MILIEU_TRAJECTOIRE);
                            passe_part(500, 180, MARCHE_ARRIERE, 60, MILIEU_TRAJECTOIRE);
                            passe_part(530, 180, MARCHE_ARRIERE, 60, FIN_TRAJECTOIRE);
                            rejoindre(450, 165, MARCHE_AVANT, 50);          //!!!  ATTENTION - Il y a une translation donc un décalage d'odométrie - faire un calage   !!!

    //                        passe_part(1100, 300, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
    //                        passe_part(950, 250,MARCHE_AVANT, 60, MILIEU_TRAJECTOIRE);
    //                        passe_part(900, 200,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
    //                        passe_part(800, 175,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
    //                        passe_part(470, 150,MARCHE_AVANT, 70, FIN_TRAJECTOIRE);

    //                        orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 2;
                        }

                        else if (ETAPE_POISSONS == 2)
                        {
                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 3;
                        }


                        else if(ETAPE_POISSONS == 3)
                        {//on sort le filet
                            EVITEMENT_ADV_ARRIERE = OFF;
                            EVITEMENT_ADV_AVANT = OFF;

                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
                            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 365);//On attend que le dernière AX12 est fini de bouger

                            ETAPE_POISSONS = 4;
                        }


                        else if(ETAPE_POISSONS == 4)
                        {//on recule un peu
                            EVITEMENT_ADV_ARRIERE = ON;
                            EVITEMENT_ADV_AVANT = OFF;
                            while(EVITEMENT_ADV_ARRIERE == OFF);

                            rejoindre(600, 150, MARCHE_ARRIERE, 60);//On se place près du bac       //600, 150

                            orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 5;
                        }


                        else if(ETAPE_POISSONS == 5)
                        {//On positionne le filet dans le bac
                            EVITEMENT_ADV_ARRIERE = OFF;//On désactive l'évitement car on ne bouge casiment pas donc on ne veu pas d'évitement inutile
                            EVITEMENT_ADV_AVANT = OFF;

                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger

                            temporisation = COMPTEUR_TEMPS_MATCH;
                            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 685)
                            {//Vérifie si il est bien dans le bac
                                if((COMPTEUR_TEMPS_MATCH - temporisation) > 2)
                                {//Si il ne l'est pas au bout de 2 sec alors on avance un peu
                                    //temporisation = COMPTEUR_TEMPS_MATCH;
                                    angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
                                    while(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 385);
                                    rejoindre(625, 150, MARCHE_ARRIERE, 50);
                                    angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);  //Position dans l'eau
                                    while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 685);

                                }
                            }
                            ETAPE_POISSONS = 6;
                        }


                        else if(ETAPE_POISSONS == 6)
                        {//On avance dans le bac et on remonte le filet (on gère les cas d'impossibilité de remonter)
                            EVITEMENT_ADV_ARRIERE = ON;
                            EVITEMENT_ADV_AVANT = OFF;
                            while(EVITEMENT_ADV_ARRIERE == OFF);//On s'assure que la variable a été bien changé

//                            rejoindre(790, 150, MARCHE_ARRIERE, 50);//On se déplace dans tout le bac (pour récuperer les poissons)
                            rejoindre(780, 150, MARCHE_ARRIERE, 50);//On se déplace dans tout le bac (pour récuperer les poissons)

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            {
                                //On désactive l'évitement car on ne veut pas qu'elle bloque la remonté du filet
                                EVITEMENT_ADV_ARRIERE = OFF;
                                EVITEMENT_ADV_AVANT = OFF;
                                while(EVITEMENT_ADV_ARRIERE == ON);//On s'assure que la variable a été bien changé

                                MOUVEMENT_AX12 = ACTIVE;
                                while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger

                                temporisation = COMPTEUR_TEMPS_MATCH;
                                short Compteur_Bloquage_Filet = 0;//Compte tout les bloquages que le filet à eut pour gerer les corrections

                                while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990)
                                {// Gestion du cas ou le filet coince
                                    while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990);//Attend 2 secondes tant que le filet n'a pas atteind sa position

                                    if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0)
                                    {//Tjrs pas atteint le seuil alors reculer un peu
                                        EVITEMENT_ADV_ARRIERE = OFF;
                                        EVITEMENT_ADV_AVANT = ON;
                                        while(EVITEMENT_ADV_AVANT == OFF);//On s'assure que la variable a été bien changé

                                        rejoindre(740, 150, MARCHE_AVANT, 35);

                                        //On désactive encore pour être sur de pas géner le mouvement du filet
                                        EVITEMENT_ADV_ARRIERE = OFF;
                                        EVITEMENT_ADV_AVANT = OFF;
                                        while(EVITEMENT_ADV_AVANT == ON);//On s'assure que la variable a été bien changé

                                        if(get_X() < 750 && fin_strategie_cause_evitement == 0);   //On attend qu'il est atteind la zone à moins qu'il y est évitement
                                            Compteur_Bloquage_Filet = 1;
                                    }
                                    else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 1)
                                    {//Augmente la patate!
                                        angle_AX12(ROT_FILET, 1005, 900, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                        Compteur_Bloquage_Filet = 2;
                                        temporisation = COMPTEUR_TEMPS_MATCH;
                                        while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990); //On attend q'il est atteind la zone
                                    }
                                    else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 2)   
                                    {//Tjr pas atteint le seuil alors faire des accoups
                                        //angle_AX12(ROT_FILET, 790, 800, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                        unsigned short temp = 0;
                                        while(temp < 3)
                                        {//Répète le mouvement 3 fois
                                            angle_AX12(DEPLOIMENT_BRAS_FILET, 660, 200, SANS_ATTENTE);   //Position déployé
                                            delay_ms(150);
                                            angle_AX12(DEPLOIMENT_BRAS_FILET, 745, 200, SANS_ATTENTE);   //Position intermédiaire (pour passé la barre du filet)
                                            delay_ms(150);
                                            temp++;
                                        }
                                    }
                                }

                                angle_AX12(DEPLOIMENT_BRAS_FILET, 730, 400, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 720 && fin_strategie_cause_evitement == 0)
                                {
                                    if((COMPTEUR_TEMPS_MATCH - temporisation) > 2)
                                        angle_AX12(DEPLOIMENT_BRAS_FILET, 730, 600, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                                    if((COMPTEUR_TEMPS_MATCH - temporisation) > 4)
                                        rejoindre(720, 160, MARCHE_ARRIERE, 40);//On recule un peu pour débloquer
                                }

                                ETAPE_POISSONS = 7;
                            }
                        }


                        else if(ETAPE_POISSONS == 7)
                        {//évite la barre de la zone de largage des poissons
                            EVITEMENT_ADV_AVANT = OFF;
                            EVITEMENT_ADV_ARRIERE = ON;
                            passe_part(1100, 250, MARCHE_ARRIERE, 40, DEBUT_TRAJECTOIRE);
                            passe_part(1150, 210,MARCHE_ARRIERE, 50, MILIEU_TRAJECTOIRE);
                            passe_part(1250, 155,MARCHE_ARRIERE, 50, MILIEU_TRAJECTOIRE);
                            passe_part(1300, 150, MARCHE_ARRIERE, 40, FIN_TRAJECTOIRE);
                            rejoindre(1240, 140, MARCHE_AVANT, 50);
                            orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 8;
                                //FLAG_ACTION = NE_RIEN_FAIRE;
                        }


                        else if(ETAPE_POISSONS == 8)
                        {//Depose les poissons
                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);
                        }
                    }
                    
                    /***********************************************************************/
                    /***********************************************************************/
                    if(COULEUR == VIOLET)//Signifie VERT     /***********************************************************************/
                    {
                        STRATEGIE_EVITEMENT = STOP;
                        
                        if(ETAPE_POISSONS == 10)
                        {
                            EVITEMENT_ADV_AVANT = ON;
                            EVITEMENT_ADV_ARRIERE = OFF;

                            passe_part(1100, 300, MARCHE_AVANT, 40, DEBUT_TRAJECTOIRE);
                            passe_part(1200, 300, MARCHE_AVANT, 40, DEBUT_TRAJECTOIRE);

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 11;
                        }

                        else if(ETAPE_POISSONS == 11)
                        {
                            EVITEMENT_ADV_AVANT = OFF;
                            EVITEMENT_ADV_ARRIERE = ON;
                            passe_part(950, 250,MARCHE_ARRIERE, 30, DEBUT_TRAJECTOIRE);
                            passe_part(900, 200,MARCHE_ARRIERE, 30, MILIEU_TRAJECTOIRE);
                            passe_part(500, 175,MARCHE_ARRIERE, 30, MILIEU_TRAJECTOIRE);
                            passe_part(350, 180,MARCHE_ARRIERE, 30, FIN_TRAJECTOIRE);
                            orienter(0,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 1;
                        }

                        else if(ETAPE_POISSONS == 0)
                        {//avance vers le le bac à poissons
                            EVITEMENT_ADV_ARRIERE = OFF;
                            EVITEMENT_ADV_AVANT = ON;
//                            passe_part(400, 1238, MARCHE_ARRIERE, 80, DEBUT_TRAJECTOIRE);
                            passe_part(400, 800, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                            passe_part(300, 600, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 1;
                        }

                        else if(ETAPE_POISSONS == 1)
                        {
                            EVITEMENT_ADV_ARRIERE = OFF;
                            EVITEMENT_ADV_AVANT = ON;
                            passe_part(300, 350, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
                            passe_part(350, 300, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                            passe_part(400, 250, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                            passe_part(450, 210, MARCHE_AVANT, 50, FIN_TRAJECTOIRE);
                            rejoindre(500, 180, MARCHE_AVANT, 40);          /*!!!  ATTENTION - Il y a une translation donc un décalage d'odométrie - faire un calage   !!!*/
                            orienter(0,100);//Réoriente bien parallèle à la bordure
                            
                            avancer_reculer(-150, 40);
                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 2;
                        }

                        else if (ETAPE_POISSONS == 2)
                        {
                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 3;
                        }

                        else if(ETAPE_POISSONS == 3)
                        {//on sort le filet
                            EVITEMENT_ADV_ARRIERE = OFF;
                            EVITEMENT_ADV_AVANT = OFF;

                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
//                            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 365);//On attend que le dernière AX12 est fini de bouger

                            ETAPE_POISSONS = 4;
                        }


                        else if(ETAPE_POISSONS == 4)
                        {//on recule un peu
                            EVITEMENT_ADV_ARRIERE = OFF;
                            EVITEMENT_ADV_AVANT = ON;

                            rejoindre(645, 160, MARCHE_AVANT, 40);//On se place près du bac
                            orienter(0,100);//Réoriente bien parallèle à la bordure
                            //orienter(180,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 5;
                        }


                        else if(ETAPE_POISSONS == 5)
                        {//On positionne le filet dans le bac
                            EVITEMENT_ADV_ARRIERE = OFF;//On désactive l'évitement car on ne bouge casiment pas donc on ne veu pas d'évitement inutile
                            EVITEMENT_ADV_AVANT = ON;

                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger

                            temporisation = COMPTEUR_TEMPS_MATCH;
                            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 695)
                            {//Vérifie si il est bien dans le bac
                                if((COMPTEUR_TEMPS_MATCH - temporisation) > 2)
                                {//Si il ne l'est pas au bout de 2 sec alors on avance un peu
                                    //temporisation = COMPTEUR_TEMPS_MATCH;
                                    angle_AX12(ROT_FILET, 1005, 300, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                    while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 995);
                                    rejoindre(660, 160, MARCHE_AVANT, 40);
                                    angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);  //Position dans l'eau
                                    while(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 685);

                                }
                            }
                            ETAPE_POISSONS = 6;
                        }


                        else if(ETAPE_POISSONS == 6)
                        {//On avance dans le bac et on remonte le filet (on gère les cas d'impossibilité de remonter)
                            EVITEMENT_ADV_ARRIERE = OFF;
                            EVITEMENT_ADV_AVANT = ON;

                            rejoindre(807, 160, MARCHE_AVANT, 40);//On se déplace dans tout le bac (pour récuperer les poissons)

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                            {
                                //On désactive l'évitement car on ne veut pas qu'elle bloque la remonté du filet
                                EVITEMENT_ADV_ARRIERE = OFF;
                                EVITEMENT_ADV_AVANT = ON;
                                while(EVITEMENT_ADV_ARRIERE == ON);//On s'assure que la variable a été bien changé

//                                MOUVEMENT_AX12 = ACTIVE;
//                                while(MOUVEMENT_AX12 == ACTIVE);//On attend le mouvement des AX12 pour continuer à bouger
                                angle_AX12(ROT_FILET, 375, 600, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
                                
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                short Compteur_Bloquage_Filet = 0;//Compte tout les bloquages que le filet à eut pour gerer les corrections

                                while(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 390)
                                {// Gestion du cas ou le filet coince
                                    while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) > 385);//Attend 2 secondes tant que le filet n'a pas atteind sa position

                                    if(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 390 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0)
                                    {//Tjrs pas atteint le seuil alors reculer un peu
                                        EVITEMENT_ADV_ARRIERE = OFF;
                                        EVITEMENT_ADV_AVANT = ON;
                                        while(EVITEMENT_ADV_AVANT == OFF);//On s'assure que la variable a été bien changé

                                        rejoindre(740, 160, MARCHE_ARRIERE, 35);

                                        //On désactive encore pour être sur de pas géner le mouvement du filet
                                        EVITEMENT_ADV_ARRIERE = OFF;
                                        EVITEMENT_ADV_AVANT = OFF;
                                        while(EVITEMENT_ADV_AVANT == ON);//On s'assure que la variable a été bien changé

                                        if(get_X() < 685 && fin_strategie_cause_evitement == 0);   //On attend qu'il est atteind la zone à moins qu'il y est évitement
                                            Compteur_Bloquage_Filet = 1;
                                    }
                                    
                                    else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 390 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 1)
                                    {//Augmente la patate!
                                        angle_AX12(ROT_FILET, 375, 900, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
                                        Compteur_Bloquage_Filet = 2;
                                        temporisation = COMPTEUR_TEMPS_MATCH;
                                        while((COMPTEUR_TEMPS_MATCH - temporisation) < 2 && read_data(ROT_FILET, LIRE_POSITION_ACTU) < 990); //On attend q'il est atteind la zone
                                    }
                                    
                                    else if(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 390 && read_data(ROT_FILET, LIRE_MOUV_FLAG) == 0 && Compteur_Bloquage_Filet == 2)   
                                    {//Tjr pas atteint le seuil alors faire des accoups
                                        //angle_AX12(ROT_FILET, 790, 800, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                                        unsigned short temp = 0;
                                        while(temp < 3)
                                        {//Répète le mouvement 3 fois
                                            angle_AX12(DEPLOIMENT_BRAS_FILET, 700, 400, SANS_ATTENTE);   //Position déployé
                                            delay_ms(150);
                                            angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 400, SANS_ATTENTE);   //Position déployé
                                            delay_ms(150);
                                            temp++;
                                        }
                                    }
                                }

                                angle_AX12(DEPLOIMENT_BRAS_FILET, 730, 400, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                                temporisation = COMPTEUR_TEMPS_MATCH;
                                while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 720 && fin_strategie_cause_evitement == 0)
                                {
                                    if((COMPTEUR_TEMPS_MATCH - temporisation) > 2)
                                        angle_AX12(DEPLOIMENT_BRAS_FILET, 730, 600, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
                                    if((COMPTEUR_TEMPS_MATCH - temporisation) > 4)
                                        rejoindre(720, 160, MARCHE_ARRIERE, 40);//On recule un peu pour débloquer
                                }

                                ETAPE_POISSONS = 7;
                            }
                        }


                        else if(ETAPE_POISSONS == 7)
                        {//évite la barre de la zone de largage des poissons
                            EVITEMENT_ADV_AVANT = ON;
                            EVITEMENT_ADV_ARRIERE = OFF;
                            passe_part(1100, 250, MARCHE_AVANT, 40, DEBUT_TRAJECTOIRE);
                            passe_part(1150, 210,MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                            passe_part(1250, 155,MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                            passe_part(1300, 150, MARCHE_AVANT, 50, FIN_TRAJECTOIRE);
                            rejoindre(1240, 140, MARCHE_ARRIERE, 40);
                            orienter(0,100);//Réoriente bien parallèle à la bordure

                            if(fin_strategie_cause_evitement == 0)//Si il n'y a pas eu la backup strat
                                ETAPE_POISSONS = 8;
                                //FLAG_ACTION = NE_RIEN_FAIRE;
                        }


                        else if(ETAPE_POISSONS == 8)
                        {//Depose les poissons
                            MOUVEMENT_AX12 = ACTIVE;
                            while(MOUVEMENT_AX12 == ACTIVE);
                        }
                    }
                    break;
            }
        }
    #endif
}


void reglage_odometrie()
{
    delay_ms(2000);
    while(!SYS_JACK);
    COULEUR = couleur_depart();
    
    #ifdef GROS_ROBOT
        //init_jack();
    #endif
    
    EVITEMENT_ADV_ARRIERE = OFF;
    EVITEMENT_ADV_AVANT = OFF;

    init_position_robot (0, 0, 0);
    //init_position_robot (-100, 0, 0);
    
    
    
    //faire_des_tours(64);
    //rejoindre(2000, 0, MARCHE_AVANT, 50);
    //delay_ms(30000);
    carre(MARCHE_AVANT);
    
    delay_ms(10000);
    
    PutsUART(UART_XBEE, "\n\n\n\r X : ");
    PutLongUART((int32_t) get_X());
    PutcUART(UART_XBEE, '.');
    PutcUART(UART_XBEE, ((uint8_t)((int32_t) ((double) get_X() * 10)) - (((int32_t) get_X()) * 10 )) +48);
    PutsUART(UART_XBEE, " Y : ");
    PutLongUART((int32_t) get_Y());
    PutcUART(UART_XBEE, '.');
    PutcUART(UART_XBEE, ((uint8_t)( (int32_t) ((double) (get_Y() * 10))) - (((int32_t) get_Y()) * 10 )) +48);
    PutsUART(UART_XBEE, " Teta : ");
    PutLongUART((int32_t) get_orientation());
    PutcUART(UART_XBEE, '.');
    PutcUART(UART_XBEE, ((uint8_t)( (int32_t) ((double) (get_orientation() * 10))) - (((int32_t) get_orientation()) * 10 )) +48);
    
    /*
    rejoindre(0, 0, MARCHE_AVANT, 100);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
    trapeze(MARCHE_AVANT);
     * */

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
}
