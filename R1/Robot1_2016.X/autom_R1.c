/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: autom.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 08 fevrier 2016, 19:00
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/***************************** FONCTIONS DIVERSES *****************************/
/******************************************************************************/

#ifdef GROS_ROBOT

void son_evitement (uint8_t melodie)
{
    //Nop();
    //commande_AX12(100, _4PARAM, WRITE_DATA, 0x29, 10, NC, NC, NC);
    //commande_AX12(100, _4PARAM, WRITE_DATA, 0x28, melodie, NC, NC, NC);
}

void rotation_us(void)
{
    static char sens = 0;
    static float angle = 0.;
   // if (read_data(AX_US, LIRE_MOUV_FLAG) == 0)
   // {
        if (sens == 0)
        {
            //synchro_AX12(AX_US, angle, 1023, SANS_ATTENTE);
            angle -= 6;
            if (angle < -60)
                sens = 1;
        }
        else
        {
            //synchro_AX12(AX_US, angle, 1023, SANS_ATTENTE);
            angle += 6;
            if (angle > 60)
            sens = 0;
        }
   // }
}

void rotation_us_avant()
{
    static char sens_D = 0, sens_G = 0;
    static uint16_t position_D = 0, position_G = 0;
    
    if (sens_D == 0)
    {
        position_D += 10;
        if (position_D > 500)
            sens_D = 1;
    }
    else
    {
        position_D -= 10;
        if (position_D < 400)
            sens_D = 0;
    }

    if (sens_G == 0)
    {
        position_G += 10;
        if (position_G > 200)
            sens_G = 1;
    }
    else
    {
        position_G -= 10;
        if (position_G < 100)
            sens_G = 0;
    }

    angle_AX12(AX_US_DROIT, position_D, 1023, SANS_ATTENTE);
    angle_AX12(AX_US_GAUCHE, position_G, 1023, SANS_ATTENTE);
}

uint8_t inversion_autom (uint8_t cote)
{
    if (COULEUR == VIOLET)
        return cote;
    else
    {
        if (cote == DROIT)
            return GAUCHE;
        else if (cote == GAUCHE)
            return DROIT;
        else
            return cote;
    }
}

uint8_t check_capteur (uint8_t cote)
{
    cote = inversion_autom(cote);

    if (cote == DROIT)
        return 1;
    else if (cote == GAUCHE)
        return 1;
    else
        return 1;
}


/******************************************************************************/
/********************************  FONCTION AX12  *****************************/
/******************************************************************************/



/******************************************************************************/
/**************************** FONCTIONS D'INITS *******************************/
/******************************************************************************/


void init_jack()
{
    allumer_LED_AX12(TOUS_LES_AX12);
    delay_ms(1000);
}

void init_depart()
{
//    PutsUART(UART_XBEE, "Init des pinces : \n\r");
    angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rangé
    delay_ms(5);
    angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rangé
    delay_ms(5);
    angle_AX12(CALAGE_CONE, 50, 512, SANS_ATTENTE);      //Position Replié    angle_AX12(DEPLOIMENT_BRAS_FILET, 820, 200, SANS_ATTENTE);   //Position remonté
    delay_ms(5);
    angle_AX12(ROT_FILET, 85, 300, SANS_ATTENTE);    //Position relevé (Tout début)
    delay_ms(5);
    angle_AX12(OUVERTURE_FILET, 256, 300, SANS_ATTENTE);    //Position fermé
    delay_ms(5);
    angle_AX12(AX_US_GAUCHE, 100, 1023, SANS_ATTENTE);    //Position regarde coté droit
    delay_ms(5);
    angle_AX12(AX_US_DROIT, 625, 1023, SANS_ATTENTE);    //Position regarde coté gauche
    delay_ms(5);
    angle_AX12(PARASOL, 0, 250, SANS_ATTENTE); //Position replié
    delay_ms(5);
    angle_AX12(DEPLOIMENT_BRAS_FILET, 862, 200, SANS_ATTENTE);   //Position remonté
    delay_ms(5);
    angle_AX12(ASCENSEUR, 1023, 512, SANS_ATTENTE);   //Position haut
    delay_ms(1000);
    angle_AX12(ASCENSEUR, 725, 512, SANS_ATTENTE);   //Position haut
    
    ETAPE_TOUR_ALLIE = 0;
    ETAPE_TOUR_ADVERSAIRE = 0;
    ETAPE_PORTES = 0;
    ETAPE_POISSONS = 0;
    ETAPE_COQUILLAGE = 0;
    
    ACTIVE_ROTATION_AX12 = ACTIVE;
}

void attrapper_cubes()
{
    angle_AX12(PINCE_D, 990, 855, SANS_ATTENTE); //Position où il attrappe
    angle_AX12(PINCE_G, 285, 855, SANS_ATTENTE); //Positions où il attrappe
}


/******************************************************************************/
/******************************** FONCTIONS AUTOM *****************************/
/******************************************************************************/


/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_10ms (void)
{

    static uint16_t compteur = 0;
    static uint8_t  evitement_en_cours = OFF;
    static uint8_t compteur_moyenne_evitement = 0;
    static uint8_t timer_delai_evitement = 0;
     static uint8_t detecter = 0;   //Permet de ne pas relancer le fin_de_deplacement dans le case PORTES
        /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/

    //fonction qui definit les actions
    switch(FLAG_ACTION)
    {//Mettre toutes les fonctions de déplacement des AX12
        case TOUR_ALLIE:           
            if(ETAPE_TOUR_ALLIE == 0)
            {//positionne vers la barre, s'encastre dedant
                if(get_X() > 800)
                    EVITEMENT_ADV_AVANT = OFF;

                angle_AX12(PINCE_D, 870, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
                angle_AX12(PINCE_G, 400, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
            }
            
            else if(ETAPE_TOUR_ALLIE == 1)
            {//attrappe tour et recule (sort de la barre)
                if(MOUVEMENT_AX12 == ACTIVE)
                {//Si on a activé le mouvement des AX12
                    if(CAPT_PINCE == 0)
                    {//Alors on récupère la tour
                        angle_AX12(CALAGE_CONE, 495, 512, SANS_ATTENTE);     //Position déplié
//                        angle_AX12(PINCE_D, 985, 860, SANS_ATTENTE); //Position où il attrappe
//                        angle_AX12(PINCE_G, 280, 860, SANS_ATTENTE); //Positions où il attrappe
//                        angle_AX12(PINCE_D, 990, 850, SANS_ATTENTE); //Position où il attrappe
//                        angle_AX12(PINCE_G, 285, 850, SANS_ATTENTE); //Positions où il attrappe
                        attrapper_cubes();
                        //angle_AX12(ASCENSEUR, 200, 512, SANS_ATTENTE);   //Position haut
                        if(read_data(PINCE_G, LIRE_POSITION_ACTU) < 315)
                            MOUVEMENT_AX12 = DESACTIVE;
                    }
                    else
                    {
                        angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rangé
                        angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rangé
                        if(read_data(PINCE_G, LIRE_POSITION_ACTU) > 850)
                            MOUVEMENT_AX12 = DESACTIVE;
                    }
                }
            }
            
            else if(ETAPE_TOUR_ALLIE == 2)
            {//recule encore, se positionne pour poser la tour
                //if(get_X() > 1500)  //Permet de ne pas détecter le poteau balise lors de la back up strat
                    //EVITEMENT_ADV_AVANT = OFF;
                attrapper_cubes();
//                angle_AX12(PINCE_D, 985, 850, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 280, 850, SANS_ATTENTE); //Positions où il attrappe
//                angle_AX12(PINCE_D, 990, 900, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 285, 900, SANS_ATTENTE); //Positions où il attrappe
            }
            
            else if(ETAPE_TOUR_ALLIE == 3)
            {//avance, va dans la zone de largage
                if(get_X() > 800)
                    EVITEMENT_ADV_AVANT = OFF;
                attrapper_cubes();
//                angle_AX12(PINCE_D, 985, 850, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 280, 850, SANS_ATTENTE); //Positions où il attrappe
//                angle_AX12(PINCE_D, 985, 900, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 280, 900, SANS_ATTENTE); //Positions où il attrappe
            }
            
            else if(ETAPE_TOUR_ALLIE == 4)
            {//pose la tour, recule pour ne pas renverser la tour
                if(MOUVEMENT_AX12 == ACTIVE)
                {//Si on a activé le mouvement des AX12
                    angle_AX12(PINCE_D, 870, 1023, SANS_ATTENTE);        //Position intermédiaire (où il est sur le point d'attraper)
                    angle_AX12(PINCE_G, 400, 1023, SANS_ATTENTE);        //Position intermédiaire (où il est sur le point d'attraper)
                    angle_AX12(CALAGE_CONE, 50, 512, SANS_ATTENTE);      //Position replié
                    MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            
            else if(ETAPE_TOUR_ALLIE == 5)
            {
                if(MOUVEMENT_AX12 == ACTIVE)
                {//Si on a activé le mouvement des AX12
                    angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE);        //Position rangé
                    angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE);        //Position rangé
                    angle_AX12(CALAGE_CONE, 50, 512, SANS_ATTENTE);      //Position replié
                    MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            break;


        case TOUR_ADVERSAIRE:
            if(ETAPE_TOUR_ADVERSAIRE == 0)
            {//positionne vers la barre, s'encastre dedant
                if(get_X() > 800 && get_X() < 2000)
                    EVITEMENT_ADV_AVANT = ON;
                if(get_X() > 2000)
                    EVITEMENT_ADV_AVANT = OFF;
                
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(AX_US_GAUCHE, 95, 1023, SANS_ATTENTE);    //Position regarde devant
                    angle_AX12(AX_US_DROIT, 505, 1023, SANS_ATTENTE);    //Position regarde devant
                    MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            
            
            else if(ETAPE_TOUR_ADVERSAIRE == 50)
            {
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(PINCE_D, 870, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
                    angle_AX12(PINCE_G, 400, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
                    MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            
            
            else if(ETAPE_TOUR_ADVERSAIRE == 1)
            {//attrappe tour et recule (sort de la barre)
                if(MOUVEMENT_AX12 == ACTIVE)
                {//Si on a activé le mouvement des AX12
                    if(CAPT_PINCE == 0)
                    {//Alors on récupère la tour
                        angle_AX12(CALAGE_CONE, 495, 512, SANS_ATTENTE);     //Position déplié
                        attrapper_cubes();
//                        angle_AX12(PINCE_D, 985, 840, SANS_ATTENTE); //Position où il attrappe
//                        angle_AX12(PINCE_G, 280, 840, SANS_ATTENTE); //Positions où il attrappe
//                        angle_AX12(PINCE_D, 990, 900, SANS_ATTENTE); //Position où il attrappe
//                        angle_AX12(PINCE_G, 285, 900, SANS_ATTENTE); //Positions où il attrappe
                        //angle_AX12(ASCENSEUR, 200, 512, SANS_ATTENTE);   //Position haut
                        if(read_data(PINCE_G, LIRE_POSITION_ACTU) < 315)
                            MOUVEMENT_AX12 = DESACTIVE;
                    }
                    else
                    {
                        angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rangé
                        angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rangé
                        if(read_data(PINCE_G, LIRE_POSITION_ACTU) > 850)
                            MOUVEMENT_AX12 = DESACTIVE;
                    }
                }
            }
            
            
            else if(ETAPE_TOUR_ADVERSAIRE == 2)
            {//recule encore, se positionne pour poser la tour
                if(get_X() > 800)
                    EVITEMENT_ADV_AVANT = OFF;
                attrapper_cubes();
//                angle_AX12(PINCE_D, 985, 840, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 280, 840, SANS_ATTENTE); //Positions où il attrappe
//                angle_AX12(PINCE_D, 990, 850, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 285, 850, SANS_ATTENTE); //Positions où il attrappe
            }
            
            else if(ETAPE_TOUR_ADVERSAIRE == 3)
            {//avance, va dans la zone de largage, pose la tour
                if(MOUVEMENT_AX12 == ACTIVE)
                {//Si on a activé le mouvement des AX12
                    angle_AX12(PINCE_D, 870, 1023, SANS_ATTENTE);        //Position intermédiaire (où il est sur le point d'attraper)
                    angle_AX12(PINCE_G, 400, 1023, SANS_ATTENTE);        //Position intermédiaire (où il est sur le point d'attraper)
                    angle_AX12(CALAGE_CONE, 50, 512, SANS_ATTENTE);      //Position replié
                    MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            
            
            else if(ETAPE_TOUR_ADVERSAIRE == 4)
            {//recule pour ne pas renverser la tour
                if(MOUVEMENT_AX12 == ACTIVE)
                {//Si on a activé le mouvement des AX12
                    angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE);        //Position rangé
                    angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE);        //Position rangé
                    angle_AX12(CALAGE_CONE, 50, 512, SANS_ATTENTE);      //Position replié
                    MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            break;

            
        case PORTES:
            if(ETAPE_PORTES == 10)
            {
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(PINCE_D, 640, 300, SANS_ATTENTE); //Position intermédiaire (où il ferme les cabines)
                    angle_AX12(PINCE_G, 620, 300, SANS_ATTENTE); //Position intermédiaire (où il ferme les cabines)
                    MOUVEMENT_AX12 = DESACTIVE;
                }
                if(CAPT_PINCE == 0)
                {
                    if(detecter == 0)
                    {
                        delay_ms(500);
                        fin_deplacement();
                        detecter = 1;
                        ETAPE_PORTES = 11;
                    }
                }
            }
            
            else if(ETAPE_PORTES == 0)
            {
                
                if(CAPT_PINCE == 0)
                {
                    attrapper_cubes();
//                    angle_AX12(PINCE_D, 985, 850, SANS_ATTENTE); //Position où il attrappe
//                    angle_AX12(PINCE_G, 280, 850, SANS_ATTENTE); //Positions où il attrappe
//                    angle_AX12(PINCE_D, 990, 900, SANS_ATTENTE); //Position où il attrappe
//                    angle_AX12(PINCE_G, 285, 900, SANS_ATTENTE); //Positions où il attrappe
                    
                    if(CAPT_PINCE2 == 0)
                    {
                        delay_ms(250);
                        fin_deplacement();
                        ETAPE_PORTES = 1;
                    }
                }
                
                
            }
            
            else if(ETAPE_PORTES == 1)
            {
//                angle_AX12(PINCE_D, 985, 840, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 280, 840, SANS_ATTENTE); //Positions où il attrappe
                if(CAPT_PINCE == 0)
                {
                    attrapper_cubes();
//                    angle_AX12(PINCE_D, 990, 850, SANS_ATTENTE); //Position où il attrappe
//                    angle_AX12(PINCE_G, 285, 850, SANS_ATTENTE); //Positions où il attrappe
                }   
            }
            
            else if(ETAPE_PORTES == 2)
            {
//                angle_AX12(PINCE_D, 985, 840, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 280, 840, SANS_ATTENTE); //Positions où il attrappe
                if(CAPT_PINCE == 0)
                {
                    attrapper_cubes();
//                    angle_AX12(PINCE_D, 990, 850, SANS_ATTENTE); //Position où il attrappe
//                    angle_AX12(PINCE_G, 285, 850, SANS_ATTENTE); //Positions où il attrappe
                }       
            }
            
            else if(ETAPE_PORTES == 3)
            {
//                angle_AX12(PINCE_D, 985, 840, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 280, 840, SANS_ATTENTE); //Positions où il attrappe
                if(CAPT_PINCE == 0)
                {
                    attrapper_cubes();
//                    angle_AX12(PINCE_D, 990, 850, SANS_ATTENTE); //Position où il attrappe
//                    angle_AX12(PINCE_G, 285, 850, SANS_ATTENTE); //Positions où il attrappe
                    
                    if(CAPT_PINCE2 == 0)
                    {
                        delay_ms(250);
                        fin_deplacement();
                        ETAPE_PORTES = 4;
                    }
                }      
            }
            
            else if(ETAPE_PORTES == 4)
            {
                attrapper_cubes();
//                angle_AX12(PINCE_D, 985, 840, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 280, 840, SANS_ATTENTE); //Positions où il attrappe
//                angle_AX12(PINCE_D, 990, 850, SANS_ATTENTE); //Position où il attrappe
//                angle_AX12(PINCE_G, 285, 850, SANS_ATTENTE); //Positions où il attrappe
            }
            break;
            

        case POISSONS:
            if(ETAPE_POISSONS == 0)
            {//avance vers le le bac à poissons
                angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rangé
               if(get_X() < 600)
                    EVITEMENT_ADV_AVANT = OFF;
            }
            
            else if(ETAPE_POISSONS == 1)
            {
                if(get_X() < 250 && get_Y() < 550)
                {
                    EVITEMENT_ADV_ARRIERE = OFF;
                    EVITEMENT_ADV_AVANT = OFF;
                }
                else
                {
                    EVITEMENT_ADV_ARRIERE = ON;
                    EVITEMENT_ADV_AVANT = OFF;
                }
            }
            
            else if(ETAPE_POISSONS == 2)
            {
                
            }
            
            
            else if(ETAPE_POISSONS == 3)
            {//on sort le filet
                
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    if(COULEUR == VERT)     //Signifie VIOLET
                    {
                        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 300, SANS_ATTENTE);   //Position déployé

                        if(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 540)
                            angle_AX12(OUVERTURE_FILET, 860, 400, SANS_ATTENTE);    //Position ouverte

                        if(read_data(OUVERTURE_FILET, LIRE_POSITION_ACTU) > 850)
                            angle_AX12(ROT_FILET, 375, 200, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)

                        if(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 365)//On attend que le dernière AX12 est fini de bouger
                            MOUVEMENT_AX12 = DESACTIVE;
                    }
                    
                    else if(COULEUR == VIOLET)  //Signifie VERT
                    {
                        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 300, SANS_ATTENTE);   //Position déployé

                        if(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 545)
                            angle_AX12(OUVERTURE_FILET, 860, 400, SANS_ATTENTE);    //Position ouverte

                        if(read_data(OUVERTURE_FILET, LIRE_POSITION_ACTU) > 845)
                            angle_AX12(ROT_FILET, 1005, 200, SANS_ATTENTE);  //Position Fin (poissons récupérés)    (INVERSE POUR COTE VERT)

                        if(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 990)//On attend que le dernière AX12 est fini de bouger
                            MOUVEMENT_AX12 = DESACTIVE;
                    }
                }
            }
            
            
            else if(ETAPE_POISSONS == 4)
            {//on recule un peu
            }
            
            
            else if(ETAPE_POISSONS == 5)
            {//On positionne le filet dans le bac
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);   //Position dans l'eau
                    MOUVEMENT_AX12 = DESACTIVE; 
                }
            }
            
            
            else if(ETAPE_POISSONS == 6)
            {//On avance dans le bac et on remonte le filet (on gère les cas d'impossibilité de remonter)
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    if(COULEUR == VERT)     //Signifie VIOLET
                        angle_AX12(ROT_FILET, 1005, 600, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                    
                    else if(COULEUR == VIOLET)     //Signifie VERT
                        angle_AX12(ROT_FILET, 375, 600, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)

                    MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            
            
            else if(ETAPE_POISSONS == 7)
            {
                
            }
            
            
            else if(ETAPE_POISSONS == 8)
            {
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 150, SANS_ATTENTE);   //Position déployé
                    if(COULEUR == VERT)     //Signifie Violet
                        angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
                    
                    else if(COULEUR == VIOLET)       //Signifie Vert
                        angle_AX12(ROT_FILET, 1005, 600, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                    
                    MOUVEMENT_AX12 = DESACTIVE;  
                }
            }
            
            break;

            
            case DEFONCER_DUNE:
                if(get_X() > 700 && get_X() < 900)
                    EVITEMENT_ADV_AVANT = ON;
                if(get_X() > 900)
                    EVITEMENT_ADV_AVANT = OFF;
                
                
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(AX_US_GAUCHE, 200, 1023, SANS_ATTENTE);    //Position regarde devant
                    angle_AX12(AX_US_DROIT, 625, 1023, SANS_ATTENTE);    //Position regarde devant
                    MOUVEMENT_AX12 = DESACTIVE;
                }
                break;
                

        case COQUILLAGES:
            break;


        case OUVERTURE_PARASOL: //Le déploiment du parasol se fait dans la fonction d'interruption du timer 90sec
            if(MOUVEMENT_AX12 == ACTIVE)
            {
                angle_AX12(PARASOL, 490, 250, SANS_ATTENTE); //Position déplié
                MOUVEMENT_AX12 = DESACTIVE;
            }
            break;

        default:
            break;
    }

        /**********************************************************************/
        /**************************** Evitement *******************************/        /* POUR CONNAITRE LES DIFFERENTS TYPES D'EVITEMENT QUI EXISTE ALORS ALLER DANS "autom.h" section Evitement*/
        /**********************************************************************/


        //Permettant de lancer la fonction d'évitement
        if(EVITEMENT_ADV_AVANT == ON)
        {
            if ((CAPT_US_AV_DROIT == 0 || CAPT_US_AV_GAUCHE == 0) && DETECTION == OFF)
            {//Si les capteurs avant detecte quelque chose
                compteur = 0;
                compteur_moyenne_evitement = 0;
                timer_delai_evitement = COMPTEUR_TEMPS_MATCH;
                evitement_en_cours = OFF;
                
                if(STRATEGIE_EVITEMENT != DELAI_ACTION) //On ne veut pas le lancer maintenant pour ce type d'évitement
                    FLAG_ASSERV.erreur = EVITEMENT;
                
                DETECTION = ON;
                brake();
            }
            else if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 20)
                {
                    if (CAPT_US_AV_DROIT == 1 && CAPT_US_AV_GAUCHE == 1)
                        compteur_moyenne_evitement++;
                    else
                        compteur_moyenne_evitement = 0;
                    
                    if(compteur_moyenne_evitement > 100)
                        {
                            compteur_moyenne_evitement = 0;
                            DETECTION = OFF;
                            unbrake();
                        }
                }
            }
            else if (DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL ))
            {
                if (evitement_en_cours == OFF)
                {
                    compteur ++;
                    if (compteur > 40)
                    {
                        evitement_en_cours = ON;
                        compteur = 0;
                        fin_deplacement();
                    }
                }
            }
            
            else if(DETECTION == ON && STRATEGIE_EVITEMENT == DELAI_ACTION)
            {
                if (CAPT_US_AV_DROIT == 1 && CAPT_US_AV_GAUCHE == 1)
                    compteur_moyenne_evitement++;		//Si les capteurs ne voient rien après 10ms alors on incrémente
                else
                    compteur_moyenne_evitement = 0;

                if(compteur_moyenne_evitement > 50)	
                {			//Si on ne voit rien durant 1s complete alors on relance
                    DETECTION = OFF;
                    unbrake();
                }
                
                if((COMPTEUR_TEMPS_MATCH - timer_delai_evitement) > 3)
                {//Si on dépasse le temps d'attente et qu'il y a toujours l'adversaire en face on change de strat
                    FLAG_ASSERV.erreur = EVITEMENT;
                    fin_deplacement();
                }
            }
            if(ACTIVE_ROTATION_AX12 == ACTIVE)
                rotation_us_avant();
        }
        
        
        
        //Evitement arrière
        else if(EVITEMENT_ADV_ARRIERE == ON)
        {
            if ( (CAPT_US_AR_GAUCHE == 1 || CAPT_US_AR_DROITE == 1)  && DETECTION == OFF)
            {
                compteur = 0;
                compteur_moyenne_evitement = 0;
                timer_delai_evitement = COMPTEUR_TEMPS_MATCH;
                
                DETECTION = ON;
                evitement_en_cours = OFF;
                if(STRATEGIE_EVITEMENT != DELAI_ACTION)
                    FLAG_ASSERV.erreur = EVITEMENT;
                brake();
            }
            else if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 20)
                {
                    if (CAPT_US_AV_DROIT == 1 && CAPT_US_AV_GAUCHE == 1)
                        compteur_moyenne_evitement++;
                    else
                        compteur_moyenne_evitement = 0;
                    if(compteur_moyenne_evitement > 100)
                        {
                            DETECTION = OFF;
                            unbrake();
                        }
                }
            }
            else if (DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL ))
            {
                if (evitement_en_cours == OFF)
                {
                    compteur ++;
                    if (compteur > 40)
                    {
                        evitement_en_cours = ON;
                        compteur = 0;
                        fin_deplacement();
                        son_evitement(30);
                    }
                     else
                     {
                        son_evitement(2);
                    }
                }
            }
            
            else if(DETECTION == ON && STRATEGIE_EVITEMENT == DELAI_ACTION)
            {
                if (CAPT_US_AR_GAUCHE == 0 && CAPT_US_AR_DROITE == 0)
                    compteur_moyenne_evitement++;
                else
                    compteur_moyenne_evitement = 0;

                if(compteur_moyenne_evitement > 50)
                {
                    DETECTION = OFF;
                    unbrake();
                }

                if((COMPTEUR_TEMPS_MATCH - timer_delai_evitement) > 3)
                {//Si on dépasse le temps d'attente et qu'il y a toujours l'adversaire en face on change de strat
                    FLAG_ASSERV.erreur = EVITEMENT;
                    fin_deplacement();
                }
            }
        }
        else if(DETECTION == ON)
        {
            DETECTION = OFF;
            unbrake();
        }
}


#endif