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
    angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rangé
    angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rangé
    //angle_AX12(ASCENSEUR, 183, 512, AVEC_ATTENTE);   //Position basse
    angle_AX12(ROT_FILET, 85, 300, SANS_ATTENTE);    //Position relevé (Tout début)
    angle_AX12(OUVERTURE_FILET, 256, 300, SANS_ATTENTE);    //Position fermé
    angle_AX12(AX_CALAGE_CONE, 750, 1023, SANS_ATTENTE);    //Position replié
    angle_AX12(DEPLOIMENT_BRAS_FILET, 820, 200, SANS_ATTENTE);   //Position remonté
    angle_AX12(AX_US_GAUCHE, 0, 1023, SANS_ATTENTE);    //Position regarde coté droit
    angle_AX12(AX_US_DROIT, 625, 1023, SANS_ATTENTE);    //Position regarde coté gauche
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
            {//Mettre toutes les fonctions de déplacement des AX12
                case TOUR_ALLIE:           
                    if(etape_tour_allie == 0)
                    {//positionne vers la barre, s'encastre dedant
                        if(get_X() > 800)
                            EVITEMENT_ADV_AVANT = OFF;
                        
                        angle_AX12(PINCE_D, 870, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
                        angle_AX12(PINCE_G, 400, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
                    }
                    if(etape_tour_allie == 1)
                    {//attrappe tour et recule (sort de la barre)
                        if(CAPT_PINCE == 0)
                        {//Alors on récupère la tour
                            if(mouvement_AX12 == ACTIVE)
                            {//Si on a activé le mouvement des AX12
                                angle_AX12(PINCE_D, 975, 800, SANS_ATTENTE); //Position où il attrappe
                                angle_AX12(PINCE_G, 290, 800, SANS_ATTENTE); //Positions où il attrappe
                                angle_AX12(AX_CALAGE_CONE, 450, 1023, SANS_ATTENTE);    //Position déplié
                                //angle_AX12(ASCENSEUR, 200, 512, SANS_ATTENTE);   //Position haut
                                mouvement_AX12 = DESACTIVE;
                            }
                        }
                        else
                        {
                            if(mouvement_AX12 == ACTIVE)
                            {//Si on a activé le mouvement des AX12
                                angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rangé
                                angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rangé
                                mouvement_AX12 = DESACTIVE;
                            }
                        }
                    }
                    if(etape_tour_allie == 2)
                    {//recule encore, se positionne pour poser la tour
                        
                    }
                    if(etape_tour_allie == 3)
                    {//avance, va dans la zone de largage
                        if(get_X() > 800)
                            EVITEMENT_ADV_AVANT = OFF;
                    }
                    if(etape_tour_allie == 4)
                    {//pose la tour, recule pour ne pas renverser la tour
                        if(mouvement_AX12 == ACTIVE)
                        {//Si on a activé le mouvement des AX12
                            angle_AX12(PINCE_D, 870, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
                            angle_AX12(PINCE_G, 400, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
                            mouvement_AX12 = DESACTIVE;
                        }
                    }
                    if(etape_tour_allie == 5)
                    {
                        angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rangé
                        angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rangé
                    }
                    break;
                    
                    
                case TOUR_ADVERSAIRE:
                    if(etape_tour_adversaire == 0)
                    {//positionne vers la barre, s'encastre dedant
                        
                    }
                    if(etape_tour_adversaire == 1)
                    {//attrappe tour et recule (sort de la barre)
                        
                    }
                    if(etape_tour_adversaire == 2)
                    {//recule encore, se positionne pour poser la tour
                        
                    }
                    if(etape_tour_adversaire == 3)
                    {//avance, va dans la zone de largage, pose la tour
                        
                    }
                    if(etape_tour_adversaire == 4)
                    {//recule pour ne pas renverser la tour
                        
                    }
                    break;
                    
                    
                case POISSONS:
                    if(etape_poissons == 0)
                    {//avance vers le le bac à poissons
                        if(get_X() < 600)
                            EVITEMENT_ADV_AVANT = OFF;
                        
                    }
                    if(etape_poissons == 1)
                    {//recule, attrape les poissons et relève le filet
                        if(mouvement_AX12 == ACTIVE)
                        {
                        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 300, SANS_ATTENTE);   //Position déployé
                        
                        if(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 520);
                            angle_AX12(OUVERTURE_FILET, 860, 400, SANS_ATTENTE);    //Position ouverte
                        
                        if(read_data(OUVERTURE_FILET, LIRE_POSITION_ACTU) > 850);
                            angle_AX12(ROT_FILET, 375, 200, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
                        
                        mouvement_AX12 = DESACTIVE;
                        }
                    }
                    if(etape_poissons == 2)
                    {//évite la barre de de la zone de largage des poissons, dépose les poissons
                        
                    }
                    break;
                    
                    
                case COQUILLAGES:
                    break;
                    
                    
                case OUVERTURE_PARASOL:
                    break;
                    
                    
                default:
                    break;
            }

        /**********************************************************************/
        /**************************** Evitement *******************************/
        /**********************************************************************/


        //Fonction permettant de lancer la fonction d'évitement
        if(EVITEMENT_ADV_AVANT == ON)
        {
            if ((CAPT_US_AV_DROIT == 0 || CAPT_US_AV_GAUCHE == 0) && DETECTION == OFF )
            {
                compteur = 0;
                DETECTION = ON;
                evitement_en_cours = OFF;
                if(STRATEGIE_EVITEMENT != DELAI_ACTION) //On ne veut pas le lancer maintenant pour ce type d'évitement
                    FLAG_ASSERV.erreur = EVITEMENT;
                brake();
                timer_delai_evitement = COMPTEUR_TEMPS_MATCH;
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
                
                if((COMPTEUR_TEMPS_MATCH - timer_delai_evitement) > 5)
                {//Si on dépasse le temps d'attente et qu'il y a toujours l'adversaire en face on change de strat
                    FLAG_ASSERV.erreur = EVITEMENT;
                    fin_deplacement();
                }
            }
            
            rotation_us_avant();
        }
        //Evitement arrière
        else if (EVITEMENT_ADV_ARRIERE == ON)
        {
            if ( (CAPT_US_AR_GAUCHE == 1 || CAPT_US_AR_DROITE == 1)  && DETECTION == OFF)
            {
                compteur = 0;
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
                compteur ++;
                
                if (compteur > 20)
                {
                    if (CAPT_US_AR_GAUCHE == 0 && CAPT_US_AR_DROITE == 0)
                        compteur_moyenne_evitement++;
                    else
                        compteur_moyenne_evitement = 0;
                    
                    if(compteur_moyenne_evitement > 100)
                        {
                            DETECTION = OFF;
                            unbrake();
                        }
                }
                
                if((COMPTEUR_TEMPS_MATCH - timer_delai_evitement) > 5)
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