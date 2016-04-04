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
    angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rang�
    angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rang�
    //angle_AX12(ASCENSEUR, 183, 512, AVEC_ATTENTE);   //Position basse
    angle_AX12(ROT_FILET, 85, 300, SANS_ATTENTE);    //Position relev� (Tout d�but)
    angle_AX12(OUVERTURE_FILET, 256, 300, SANS_ATTENTE);    //Position ferm�
    angle_AX12(AX_CALAGE_CONE, 750, 1023, SANS_ATTENTE);    //Position repli�
    angle_AX12(DEPLOIMENT_BRAS_FILET, 820, 200, SANS_ATTENTE);   //Position remont�
    angle_AX12(AX_US_GAUCHE, 0, 1023, SANS_ATTENTE);    //Position regarde cot� droit
    angle_AX12(AX_US_DROIT, 625, 1023, SANS_ATTENTE);    //Position regarde cot� gauche
    angle_AX12(PARASOL, 0, 250, SANS_ATTENTE); //Position repli�
    
    ETAPE_TOUR_ALLIE = 0;
    ETAPE_TOUR_ADVERSAIRE = 0;
    ETAPE_POISSONS = 0;
    ETAPE_COQUILLAGE = 0;
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
    
        /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/
    
    //fonction qui definit les actions
    switch(FLAG_ACTION)
    {//Mettre toutes les fonctions de d�placement des AX12
        case TOUR_ALLIE:           
            if(ETAPE_TOUR_ALLIE == 0)
            {//positionne vers la barre, s'encastre dedant
                if(get_X() > 800)
                    EVITEMENT_ADV_AVANT = OFF;

                angle_AX12(PINCE_D, 870, 300, SANS_ATTENTE); //Position interm�diaire (o� il est sur le point d'attraper)
                angle_AX12(PINCE_G, 400, 300, SANS_ATTENTE); //Position interm�diaire (o� il est sur le point d'attraper)
            }
            if(ETAPE_TOUR_ALLIE == 1)
            {//attrappe tour et recule (sort de la barre)
                if(CAPT_PINCE == 0)
                {//Alors on r�cup�re la tour
                    if(MOUVEMENT_AX12 == ACTIVE)
                    {//Si on a activ� le mouvement des AX12
                        angle_AX12(PINCE_D, 975, 800, SANS_ATTENTE); //Position o� il attrappe
                        angle_AX12(PINCE_G, 290, 800, SANS_ATTENTE); //Positions o� il attrappe
                        angle_AX12(AX_CALAGE_CONE, 450, 1023, SANS_ATTENTE);    //Position d�pli�
                        //angle_AX12(ASCENSEUR, 200, 512, SANS_ATTENTE);   //Position haut
                        MOUVEMENT_AX12 = DESACTIVE;
                    }
                }
                else
                {
                        angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rang�
                        angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rang�
                }
            }
            if(ETAPE_TOUR_ALLIE == 2)
            {//recule encore, se positionne pour poser la tour
                //if(get_X() > 1500)  //Permet de ne pas d�tecter le poteau balise lors de la back up strat
                    //EVITEMENT_ADV_AVANT = OFF;
            }
            if(ETAPE_TOUR_ALLIE == 3)
            {//avance, va dans la zone de largage
                if(get_X() > 800)
                    EVITEMENT_ADV_AVANT = OFF;
            }
            if(ETAPE_TOUR_ALLIE == 4)
            {//pose la tour, recule pour ne pas renverser la tour
                if(MOUVEMENT_AX12 == ACTIVE)
                {//Si on a activ� le mouvement des AX12
                    angle_AX12(PINCE_D, 870, 300, SANS_ATTENTE); //Position interm�diaire (o� il est sur le point d'attraper)
                    angle_AX12(PINCE_G, 400, 300, SANS_ATTENTE); //Position interm�diaire (o� il est sur le point d'attraper)
                    MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            if(ETAPE_TOUR_ALLIE == 5)
            {
                if(MOUVEMENT_AX12 == ACTIVE)
                {//Si on a activ� le mouvement des AX12
                    angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rang�
                    angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rang�
                    MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            break;


        case TOUR_ADVERSAIRE:
            if(ETAPE_TOUR_ADVERSAIRE == 0)
            {//positionne vers la barre, s'encastre dedant

            }
            if(ETAPE_TOUR_ADVERSAIRE == 1)
            {//attrappe tour et recule (sort de la barre)

            }
            if(ETAPE_TOUR_ADVERSAIRE == 2)
            {//recule encore, se positionne pour poser la tour

            }
            if(ETAPE_TOUR_ADVERSAIRE == 3)
            {//avance, va dans la zone de largage, pose la tour

            }
            if(ETAPE_TOUR_ADVERSAIRE == 4)
            {//recule pour ne pas renverser la tour

            }
            break;


        case POISSONS:
            if(ETAPE_POISSONS == 0)
            {//avance vers le le bac � poissons
               if(get_X() < 600)
                    EVITEMENT_ADV_AVANT = OFF;
            }
            
            
            if(ETAPE_POISSONS == 1)
            {
                
            }
            
            
            if(ETAPE_POISSONS == 2)
            {//on sort le filet
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 300, SANS_ATTENTE);   //Position d�ploy�

                    if(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 540)
                        angle_AX12(OUVERTURE_FILET, 860, 400, SANS_ATTENTE);    //Position ouverte

                    if(read_data(OUVERTURE_FILET, LIRE_POSITION_ACTU) > 850)
                        angle_AX12(ROT_FILET, 375, 200, SANS_ATTENTE);   //Position Interm�diaire (avant de rentrer dans l'eau)
                    
                    if(read_data(ROT_FILET, LIRE_POSITION_ACTU) > 365)//On attend que le derni�re AX12 est fini de bouger
                        MOUVEMENT_AX12 = DESACTIVE;
                }
            }
            
            
            if(ETAPE_POISSONS == 3)
            {//on recule un peu
            }
            
            
            if(ETAPE_POISSONS == 4)
            {//On positionne le filet dans le bac
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);   //Position dans l'eau
                    MOUVEMENT_AX12 = DESACTIVE;  
                }
            }
            
            
            if(ETAPE_POISSONS == 5)
            {//On avance dans le bac et on remonte le filet (on g�re les cas d'impossibilit� de remonter)
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(ROT_FILET, 1005, 600, SANS_ATTENTE);  //Position Fin (poissons r�cup�r�s)
                    MOUVEMENT_AX12 = DESACTIVE;  
                }
            }
            
            
            if(ETAPE_POISSONS == 6)
            {
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 150, SANS_ATTENTE);   //Position d�ploy�
                    angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Interm�diaire (avant de rentrer dans l'eau)
                    MOUVEMENT_AX12 = DESACTIVE;  
                }
            }
            
            
            if(ETAPE_POISSONS == 7)
            {
                if(MOUVEMENT_AX12 == ACTIVE)
                {
                    angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 150, SANS_ATTENTE);   //Position d�ploy�
                    angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Interm�diaire (avant de rentrer dans l'eau)
                    MOUVEMENT_AX12 = DESACTIVE;  
                }
            }
            
            break;


        case COQUILLAGES:
            break;


        case OUVERTURE_PARASOL: //Le d�ploiment du parasol se fait dans la fonction d'interruption du timer 90sec
            if(MOUVEMENT_AX12 == ACTIVE)
            {
                angle_AX12(PARASOL, 490, 250, SANS_ATTENTE); //Position d�pli�
                MOUVEMENT_AX12 = DESACTIVE;
            }
            break;

        default:
            break;
    }

        /**********************************************************************/
        /**************************** Evitement *******************************/        /* POUR CONNAITRE LES DIFFERENTS TYPES D'EVITEMENT QUI EXISTE ALORS ALLER DANS "autom.h" section Evitement*/
        /**********************************************************************/


        //Permettant de lancer la fonction d'�vitement
        if(EVITEMENT_ADV_AVANT == ON)
        {
            if ((CAPT_US_AV_DROIT == 0 || CAPT_US_AV_GAUCHE == 0) && DETECTION == OFF)
            {//Si les capteurs avant detecte quelque chose
                compteur = 0;
                compteur_moyenne_evitement = 0;
                timer_delai_evitement = COMPTEUR_TEMPS_MATCH;
                evitement_en_cours = OFF;
                
                if(STRATEGIE_EVITEMENT != DELAI_ACTION) //On ne veut pas le lancer maintenant pour ce type d'�vitement
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
                    compteur_moyenne_evitement++;		//Si les capteurs ne voient rien apr�s 10ms alors on incr�mente
                else
                    compteur_moyenne_evitement = 0;

                if(compteur_moyenne_evitement > 50)	
                {			//Si on ne voit rien durant 1s complete alors on relance
                    DETECTION = OFF;
                    unbrake();
                }
                
                if((COMPTEUR_TEMPS_MATCH - timer_delai_evitement) > 3)
                {//Si on d�passe le temps d'attente et qu'il y a toujours l'adversaire en face on change de strat
                    FLAG_ASSERV.erreur = EVITEMENT;
                    fin_deplacement();
                }
            }
            rotation_us_avant();
        }
        
        
        
        //Evitement arri�re
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
                {//Si on d�passe le temps d'attente et qu'il y a toujours l'adversaire en face on change de strat
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