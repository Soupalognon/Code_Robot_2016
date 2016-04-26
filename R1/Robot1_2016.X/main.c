/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: main.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 22 octobre 2015, 23:55
 *******************************************************************************
 *
 *
 ******************************************************************************/

#include "system.h"
#include <xc.h>

/******************************************************************************/
/************************ Configurations Bits *********************************/
/******************************************************************************/

// DSPIC33FJ128MC804 Configuration Bit Settings

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Mode (Internal Fast RC (FRC) w/ PLL)
#pragma config IESO = OFF //ON                // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSECMD  //CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)
#pragma config LPOL = ON                // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
#pragma config HPOL = ON                // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
#pragma config PWMPIN = ON              // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)



/******************************************************************************/
/********************* DECLARATION DES VARIABLES GLOBALES *********************/
/******************************************************************************/

    uint8_t COULEUR;

    uint8_t DETECTION;
    uint8_t EVITEMENT_ADV_AVANT;
    uint8_t EVITEMENT_ADV_ARRIERE;
    uint8_t STRATEGIE_EVITEMENT;
    volatile uint16_t FLAG_ACTION;
    uint8_t FLAG_EVITEMENT_STRATEGIQUE;
    uint8_t ETAT_AUTOM;
    uint16_t COMPTEUR_MARCHE;
    uint8_t COMPTEUR_TEMPS_MATCH;
    uint8_t CONFIG_COQUILLAGE;


    int obstacle[30][20];
	int calcul_en_cours=OFF;
    int curseur;
    int curseur_obstacle;
    int itineraire[nb_point_max][3];
    int itineraire_basique[nb_point_max][3];
    int itineraire_court[nb_point_max][3];
    int point_obstacle[nb_point_max][3];
    int x_actuel;
    int y_actuel;
    int x_souhaite;
    int y_souhaite;
    int x_obj;
    int y_obj;
    int id_evitement = 0;
    int id_evitement_initial = 0;
    int chemin_court=0;
    uint16_t compteur_evitement=0;
    int precedent_obstacle[7];
    
    uint8_t ETAPE_TOUR_ALLIE;
    uint8_t ETAPE_TOUR_ADVERSAIRE;
    uint8_t ETAPE_POISSONS;
    uint8_t ETAPE_COQUILLAGE;
    volatile uint8_t fin_strategie_cause_evitement;
    uint8_t MOUVEMENT_AX12 = 0;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

    /*
     LEs capteur avant sont à l'état bas pour activé
     * Les capteurs arrières sont à l'état haut pour activé
     
     */
int main(int argc, char** argv)
{
    init_system();
    init_evitement();
    TIMER_DEBUG = DESACTIVE;
    delay_ms(500);
    
    //init_decalage_AX12();
    //init_position_AX12();
    while(SYS_JACK);
    
    PutsUART(UART_XBEE, "\n\n\n\n\n\nDebut du code : \n\r");
    strategie();
    //reglage_odometrie();
   
    while(1);
    return (EXIT_SUCCESS);
}


/*
 * angle_AX12(AX_CALAGE_CONE, 450, 1023, SANS_ATTENTE);    //Position déplié
 * angle_AX12(AX_CALAGE_CONE, 750, 1023, SANS_ATTENTE);    //Position replié
 * 
 * angle_AX12(ASCENSEUR, 315, 512, SANS_ATTENTE);   //Position haut
 * angle_AX12(ASCENSEUR, 183, 512, SANS_ATTENTE);   //Position basse
 * 
 * angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 200, SANS_ATTENTE);   //Position déployé
 * angle_AX12(DEPLOIMENT_BRAS_FILET, 820, 200, SANS_ATTENTE);   //Position remonté
 * angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 150, SANS_ATTENTE);   //Position intermédiaire (pour passé la barre du filet)
 * 
 * angle_AX12(ROT_FILET, 85, 300, SANS_ATTENTE);    //Position relevé (Tout début)
 * angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
 * angle_AX12(ROT_FILET, 690, 300, SANS_ATTENTE);   //Position dans l'eau
 * angle_AX12(ROT_FILET, 1005, 300, SANS_ATTENTE);  //Position Fin (poissons récupérés)
 * 
 * angle_AX12(OUVERTURE_FILET, 256, 300, SANS_ATTENTE);    //Position fermé
 * angle_AX12(OUVERTURE_FILET, 860, 300, SANS_ATTENTE);    //Position ouverte
 * 
 * angle_AX12(PINCE_D, 405, 300, SANS_ATTENTE); //Position rangé
 * angle_AX12(PINCE_D, 970, 300, SANS_ATTENTE); //Position où il attrappe
 * angle_AX12(PINCE_D, 870, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
 * 
 * angle_AX12(PINCE_G, 860, 300, SANS_ATTENTE); //Position rangé
 * angle_AX12(PINCE_G, 310, 300, SANS_ATTENTE); //Positions où il attrappe
 * angle_AX12(PINCE_G, 400, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
 * 
 * angle_AX12(PARASOL, 0, 250, SANS_ATTENTE); //Position replié
 * angle_AX12(PARASOL, 490, 250, SANS_ATTENTE); //Position déplié
 * 
 * angle_AX12(AX_US_GAUCHE, 465, 1023, SANS_ATTENTE);    //Position regarde coté droit
 * angle_AX12(AX_US_GAUCHE, 950, 1023, SANS_ATTENTE);   //Position regarde derrière gauche
 * angle_AX12(AX_US_GAUCHE, 750, 1023, SANS_ATTENTE);   //Position regarde avant gauche
 * 
 * angle_AX12(AX_US_DROIT, 100, 1023, SANS_ATTENTE);    //Position regarde derrière droit
 * angle_AX12(AX_US_DROIT, 400, 1023, SANS_ATTENTE);    //Position regarde avant droit
 * angle_AX12(AX_US_DROIT, 625, 1023, SANS_ATTENTE);    //Position regarde coté gauche
 */


void Action_poisson_lente()
{
        delay_ms(3000);
        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 200, SANS_ATTENTE);   //Position déployé
        delay_ms(3000);
        angle_AX12(OUVERTURE_FILET, 860, 300, SANS_ATTENTE);    //Position ouverte
        delay_ms(3000);
        angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
        delay_ms(3000);
        angle_AX12(ROT_FILET, 690, 300, SANS_ATTENTE);   //Position dans l'eau
        delay_ms(3000);
        angle_AX12(ROT_FILET, 1005, 300, SANS_ATTENTE);  //Position Fin (poissons récupérés)
        delay_ms(3000);
        angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 150, SANS_ATTENTE);   //Position intermédiaire (pour passé la barre du filet)
        delay_ms(3000);
        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 150, SANS_ATTENTE);   //Position déployé
        delay_ms(3000);
        angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
        delay_ms(3000);
        angle_AX12(ROT_FILET, 85, 300, SANS_ATTENTE);    //Position relevé (Tout début)
        delay_ms(3000);
        angle_AX12(OUVERTURE_FILET, 256, 300, SANS_ATTENTE);    //Position fermé
        delay_ms(3000);
        angle_AX12(DEPLOIMENT_BRAS_FILET, 820, 200, SANS_ATTENTE);   //Position remonté
        delay_ms(3000);
}

void Action_poisson_rapide()
{
        delay_ms(3000);
        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 200, SANS_ATTENTE);   //Position déployé
        delay_ms(1000);
        angle_AX12(OUVERTURE_FILET, 860, 300, AVEC_ATTENTE);    //Position ouverte
        delay_ms(10);
        angle_AX12(ROT_FILET, 375, 300, AVEC_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
        delay_ms(10);
        lancer_autom_AX12();
        delay_ms(1000);
        angle_AX12(ROT_FILET, 690, 300, SANS_ATTENTE);   //Position dans l'eau
        delay_ms(1000);
        angle_AX12(ROT_FILET, 1005, 300, SANS_ATTENTE);  //Position Fin (poissons récupérés)
        delay_ms(2000);
        angle_AX12(DEPLOIMENT_BRAS_FILET, 600, 150, SANS_ATTENTE);   //Position intermédiaire (pour passé la barre du filet)
        delay_ms(500);
        angle_AX12(DEPLOIMENT_BRAS_FILET, 530, 150, SANS_ATTENTE);   //Position déployé
        delay_ms(500);
        angle_AX12(ROT_FILET, 375, 300, SANS_ATTENTE);   //Position Intermédiaire (avant de rentrer dans l'eau)
        delay_ms(1500);
        angle_AX12(ROT_FILET, 85, 300, AVEC_ATTENTE);    //Position relevé (Tout début)
        delay_ms(10);
        angle_AX12(OUVERTURE_FILET, 256, 300, AVEC_ATTENTE);    //Position fermé
        delay_ms(10);
        lancer_autom_AX12();
        delay_ms(1000);
        angle_AX12(DEPLOIMENT_BRAS_FILET, 820, 200, SANS_ATTENTE);   //Position remonté
        delay_ms(3000);
}

void Action_attraper_cubes()
{
        delay_ms(3000);
        angle_AX12(PINCE_D, 285, 300, AVEC_ATTENTE); //Position rangé
        delay_ms(20);
        angle_AX12(PINCE_G, 735, 300, AVEC_ATTENTE); //Position rangé
        delay_ms(20);
        angle_AX12(AX_CALAGE_CONE, 750, 1023, AVEC_ATTENTE);    //Position replié
        delay_ms(20);
        angle_AX12(ASCENSEUR, 265, 512, AVEC_ATTENTE);   //Position basse
        delay_ms(20);
        lancer_autom_AX12();
        delay_ms(2000);
        angle_AX12(PINCE_D, 720, 300, AVEC_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        delay_ms(10);
        angle_AX12(PINCE_G, 320, 300, AVEC_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        delay_ms(10);
        lancer_autom_AX12();
        delay_ms(5000);
        angle_AX12(PINCE_D, 835, 300, AVEC_ATTENTE); //Position où il attrappe
        delay_ms(10);
        angle_AX12(PINCE_G, 200, 300, AVEC_ATTENTE); //Positions où il attrappe
        delay_ms(10);
        lancer_autom_AX12();
        delay_ms(1500);
        angle_AX12(AX_CALAGE_CONE, 450, 1023, SANS_ATTENTE);    //Position déplié
        delay_ms(1500);
        angle_AX12(ASCENSEUR, 500, 200, SANS_ATTENTE);   //Position haut
        delay_ms(3000);
        angle_AX12(ASCENSEUR, 265, 512, SANS_ATTENTE);   //Position basse
        delay_ms(3000);
}


void aller_chercher_cubes()
{
    //delay_ms(1000);
        
        angle_AX12(PINCE_D, 285, 300, AVEC_ATTENTE); //Position rangé
        angle_AX12(PINCE_G, 735, 300, AVEC_ATTENTE); //Position rangé
        angle_AX12(ASCENSEUR, 235, 512, AVEC_ATTENTE);   //Position basse
        lancer_autom_AX12();
        delay_ms(10);
        rejoindre(200, 1235,MARCHE_AVANT, 100);
        //rejoindre(650, 1600,MARCHE_AVANT, 100);
        angle_AX12(PINCE_D, 720, 300, AVEC_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        angle_AX12(PINCE_G, 320, 300, AVEC_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        lancer_autom_AX12();
        rejoindre(885, 1600,MARCHE_AVANT, 100);
        //cibler(885, 1900, 50);
        rejoindre(885, 1735, MARCHE_AVANT, 30);
        angle_AX12(PINCE_D, 815, 300, AVEC_ATTENTE); //Position où il attrappe
        angle_AX12(PINCE_G, 180, 300, AVEC_ATTENTE); //Positions où il attrappe
        lancer_autom_AX12();
        delay_ms(100);
        angle_AX12(ASCENSEUR, 380, 512, SANS_ATTENTE);   //Position haut
        
}


void recuperer_poissons()
{
    init_position_robot(78., 1235., 0.);

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
}

void faire_tourner_les_us_devant()
{
    angle_AX12(AX_US_GAUCHE, 100, 200, AVEC_ATTENTE);    //Position regarde coté droit
    angle_AX12(AX_US_DROIT, 625, 1023, AVEC_ATTENTE);    //Position regarde coté gauche
    lancer_autom_AX12();
    delay_ms(100);
    angle_AX12(AX_US_DROIT, 400, 1023, AVEC_ATTENTE);    //Position regarde avant droit
    angle_AX12(AX_US_GAUCHE, 350, 1023, AVEC_ATTENTE);   //Position regarde avant gauche
    lancer_autom_AX12();
    delay_ms(100);
    angle_AX12(AX_US_GAUCHE, 650, 1023, AVEC_ATTENTE);   //Position regarde derrière gauche
    angle_AX12(AX_US_DROIT, 100, 1023, AVEC_ATTENTE);    //Position regarde derrière droit
    lancer_autom_AX12();
    delay_ms(100);
}


void foncer_dans_le_tas_de_sable()
{
    passe_part(650, 1235, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
    passe_part(800, 1600, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
    passe_part(1000, 1750,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
    passe_part(1100, 1850,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
    passe_part(1200, 1800,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
    passe_part(1400, 1770,MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
}