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

        //if (COULEUR == VIOLET)
            //synchro_AX12(AX_US, 45, 1023, SANS_ATTENTE);
        //else
            //synchro_AX12(AX_US, -45, 1023, SANS_ATTENTE);
        //delay_ms(1000);
        
         //Init de départ
        angle_AX12(PINCE_D, 285, 300, AVEC_ATTENTE); //Position rangé
        angle_AX12(PINCE_G, 735, 300, AVEC_ATTENTE); //Position rangé
        angle_AX12(ASCENSEUR, 180, 512, AVEC_ATTENTE);   //Position basse
        angle_AX12(ROT_FILET, 85, 300, AVEC_ATTENTE);    //Position relevé (Tout début)
        angle_AX12(OUVERTURE_FILET, 256, 300, AVEC_ATTENTE);    //Position fermé
        angle_AX12(AX_CALAGE_CONE, 750, 1023, AVEC_ATTENTE);    //Position replié
        lancer_autom_AX12();
        angle_AX12(DEPLOIMENT_BRAS_FILET, 820, 200, SANS_ATTENTE);   //Position remonté
    
        while(!SYS_JACK);

        // Démarage du match
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = OFF;
        STRATEGIE_EVITEMENT = EVITEMENT_NORMAL;

        init_position_robot(78., 1235., 0.);
        //init_position_robot(500., 1235., 0.);

        rejoindre(200, 1235,MARCHE_AVANT, 100);
        angle_AX12(PINCE_D, 720, 300, AVEC_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        angle_AX12(PINCE_G, 320, 300, AVEC_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        lancer_autom_AX12();
        rejoindre(900, 1600, MARCHE_AVANT, 100);
        rejoindre(900, 1735, MARCHE_AVANT, 30);
        /*
        passe_part(200, 1235,MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        passe_part(900, 1400,MARCHE_AVANT, 60, MILIEU_TRAJECTOIRE);
        passe_part(885, 1600,MARCHE_AVANT, 60, MILIEU_TRAJECTOIRE);
        passe_part(885, 1735,MARCHE_AVANT, 30, FIN_TRAJECTOIRE);
        */
        //delay_ms(1000);
        angle_AX12(PINCE_D, 850, 1023, AVEC_ATTENTE); //Position où il attrappe
        angle_AX12(PINCE_G, 170, 1023, AVEC_ATTENTE); //Positions où il attrappe
        angle_AX12(AX_CALAGE_CONE, 450, 1023, AVEC_ATTENTE);    //Position déplié
        lancer_autom_AX12();
        delay_ms(1000);
        //angle_AX12(ASCENSEUR, 315, 512, SANS_ATTENTE);   //Position haut
        //delay_ms(1000);
        rejoindre(900, 1600, MARCHE_ARRIERE, 60);
        rejoindre(200, 1000,MARCHE_ARRIERE, 60);
        rejoindre(1050, 850,MARCHE_AVANT, 100);
        delay_ms(1000);
        angle_AX12(PINCE_D, 720, 300, AVEC_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        angle_AX12(PINCE_G, 320, 300, AVEC_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        lancer_autom_AX12();
        delay_ms(1000);
        rejoindre(800, 850,MARCHE_ARRIERE, 70);
        //rejoindre(78, 1235,MARCHE_ARRIERE, 20);
        angle_AX12(PINCE_D, 285, 300, AVEC_ATTENTE); //Position rangé
        angle_AX12(PINCE_G, 735, 300, AVEC_ATTENTE); //Position rangé
        lancer_autom_AX12();
        
        if (COULEUR == VERT){
            passe_part(1100, 250, MARCHE_AVANT, 60, DEBUT_TRAJECTOIRE);
            passe_part(1000, 200,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
            FLAG_ACTION = POISSONS;
            passe_part(900, 175,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
            passe_part(800, 157,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
            passe_part(450, 157,MARCHE_AVANT, 80, FIN_TRAJECTOIRE);
            
            //passe_part(500, 1235, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
            //passe_part(1100, 250, MARCHE_AVANT, 60, MILIEU_TRAJECTOIRE);
            //passe_part(1000, 200,MARCHE_AVANT, 70, MILIEU_TRAJECTOIRE);
            //FLAG_ACTION = POISSONS;
            //passe_part(900, 175,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
            //passe_part(800, 157,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
            //passe_part(450, 157,MARCHE_AVANT, 80, FIN_TRAJECTOIRE);
            orienter(180,100);

            angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 300, SANS_ATTENTE);   //Position déployé
            while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) > 520);
            angle_AX12(OUVERTURE_FILET, 860, 400, SANS_ATTENTE);    //Position ouverte
            while(read_data(OUVERTURE_FILET, LIRE_POSITION_ACTU) < 850);
            angle_AX12(ROT_FILET, 375, 200, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 370);
            delay_ms(150);

            rejoindre(637, 157, MARCHE_ARRIERE, 60);

            angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);   //Position dans l'eau
            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 685);

            rejoindre(790, 150, MARCHE_ARRIERE, 50);

            angle_AX12(ROT_FILET, 1005, 600, SANS_ATTENTE);  //Position Fin (poissons récupérés)
            
            // Gestion du cas ou le filet coince 
             
            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 1000)
            {
                delay_ms(1000);
                static short Compteur_Bloquage_Filet = 0;
                if(read_data(ROT_FILET, LIRE_POSITION_ACTU) <= 1000 && read_data(ROT_FILET, LIRE_POSITION_ACTU) >= 690)
                {//Tjrs pas atteint le seuil alors reculer un peu
                    rejoindre(740, 150, MARCHE_AVANT, 35);
                    while(get_X() > 755);
                    Compteur_Bloquage_Filet = 1;
                    delay_ms(100);
                }
                if(read_data(ROT_FILET, LIRE_POSITION_ACTU) <= 1000 && read_data(ROT_FILET, LIRE_POSITION_ACTU) >= 690 && Compteur_Bloquage_Filet == 1)
                {//Augmente la patate!
                    angle_AX12(ROT_FILET, 1005, 800, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                    delay_ms(200);
                    Compteur_Bloquage_Filet = 2;
                    //delay_ms(200);
                    //angle_AX12(ROT_FILET, 1005, 900, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                }
                if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 1000 && read_data(ROT_FILET, LIRE_POSITION_ACTU) >= 690 && Compteur_Bloquage_Filet == 2)   
                {//Tjr pas atteint le seuil alors faire des accoups
                    //angle_AX12(ROT_FILET, 790, 800, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                    unsigned short temp = 0;
                    while(temp < 3)
                    {//Répète le mouvement 3 fois
                    angle_AX12(DEPLOIMENT_BRAS_FILET, 590, 400, SANS_ATTENTE);   //Position déployé
                    delay_ms(50);
                    angle_AX12(DEPLOIMENT_BRAS_FILET, 460, 400, SANS_ATTENTE);   //Position déployé
                    delay_ms(50);
                    temp++;
                    }
                }
            }
            angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 350, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
            while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 590);

            passe_part(1100, 250, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);
            passe_part(1150, 210,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
            passe_part(1250, 155,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
            passe_part(1300, 158,MARCHE_ARRIERE, 80, FIN_TRAJECTOIRE);
            orienter(180,100);

            angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 150, AVEC_ATTENTE);   //Position déployé
            angle_AX12(ROT_FILET, 375, 300, AVEC_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
            lancer_autom_AX12();
            delay_ms(1000);
        }
        else if (COULEUR == VIOLET){
            passe_part(500, 1235, MARCHE_ARRIERE, 100, DEBUT_TRAJECTOIRE);
            passe_part(1100, 250, MARCHE_ARRIERE, 60, MILIEU_TRAJECTOIRE);
            passe_part(1000, 200,MARCHE_ARRIERE, 70, MILIEU_TRAJECTOIRE);
            FLAG_ACTION = POISSONS;
            passe_part(900, 175,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
            //passe_part(800, 157,MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
            passe_part(450, 157,MARCHE_ARRIERE, 80, FIN_TRAJECTOIRE);
            orienter(180,100);

            angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 300, SANS_ATTENTE);   //Position déployé
            while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) > 520);
            angle_AX12(OUVERTURE_FILET, 860, 400, SANS_ATTENTE);    //Position ouverte
            while(read_data(OUVERTURE_FILET, LIRE_POSITION_ACTU) < 850);
            angle_AX12(ROT_FILET, 375, 200, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 370);
            delay_ms(150);

            rejoindre(637, 157, MARCHE_AVANT, 60);

            angle_AX12(ROT_FILET, 690, 150, SANS_ATTENTE);   //Position dans l'eau
            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 685);

            rejoindre(790, 150, MARCHE_AVANT, 50);

            angle_AX12(ROT_FILET, 1005, 600, SANS_ATTENTE);  //Position Fin (poissons récupérés)
            
             //Gestion du cas ou le filet coince 
             
            while(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 1000)
            {
                delay_ms(1000);
                static short Compteur_Bloquage_Filet = 0;
                if(read_data(ROT_FILET, LIRE_POSITION_ACTU) <= 1000 && read_data(ROT_FILET, LIRE_POSITION_ACTU) >= 690)
                {//Tjrs pas atteint le seuil alors reculer un peu
                    rejoindre(740, 150, MARCHE_ARRIERE, 35);
                    while(get_X() > 755);
                    Compteur_Bloquage_Filet = 1;
                    delay_ms(100);
                }
                if(read_data(ROT_FILET, LIRE_POSITION_ACTU) <= 1000 && read_data(ROT_FILET, LIRE_POSITION_ACTU) >= 690 && Compteur_Bloquage_Filet == 1)
                {//Augmente la patate!
                    angle_AX12(ROT_FILET, 1005, 800, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                    delay_ms(200);
                    Compteur_Bloquage_Filet = 2;
                    //delay_ms(200);
                    //angle_AX12(ROT_FILET, 1005, 900, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                }
                if(read_data(ROT_FILET, LIRE_POSITION_ACTU) < 1000 && read_data(ROT_FILET, LIRE_POSITION_ACTU) >= 690 && Compteur_Bloquage_Filet == 2)   
                {//Tjr pas atteint le seuil alors faire des accoups
                    //angle_AX12(ROT_FILET, 790, 800, SANS_ATTENTE);  //Position Fin (poissons récupérés)
                    unsigned short temp = 0;
                    while(temp < 3)
                    {//Répète le mouvement 3 fois
                    angle_AX12(DEPLOIMENT_BRAS_FILET, 590, 400, SANS_ATTENTE);   //Position déployé
                    delay_ms(50);
                    angle_AX12(DEPLOIMENT_BRAS_FILET, 460, 400, SANS_ATTENTE);   //Position déployé
                    delay_ms(50);
                    temp++;
                    }
                }
            }
            angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 350, SANS_ATTENTE);   //Position intermédiaire (pour passer la barre du filet)
            while(read_data(DEPLOIMENT_BRAS_FILET, LIRE_POSITION_ACTU) < 590);

            passe_part(1100, 250, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
            passe_part(1150, 210,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
            passe_part(1250, 155,MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
            passe_part(1300, 158,MARCHE_AVANT, 80, FIN_TRAJECTOIRE);
            orienter(180,100);

            angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 150, AVEC_ATTENTE);   //Position déployé
            angle_AX12(ROT_FILET, 375, 300, AVEC_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
            lancer_autom_AX12();
            delay_ms(1000);
        }
      
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