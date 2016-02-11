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
    uint16_t FLAG_ACTION;
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
    int x_obj;
    int y_obj;
    int id_evitement = 0;
    int id_evitement_initial = 0;
    int chemin_court=0;
    uint16_t compteur_evitement=0;
    int precedent_obstacle[7];


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

    
int main(int argc, char** argv)
{
    init_system();
    //init_evitement();
    TIMER_DEBUG = ACTIVE;
    delay_ms(500);

    
    //init_decalage_AX12();
    //init_position_AX12();
    
    //Init Supplémentaire
#ifdef GROS_ROBOT
#endif
    strategie();
    //reglage_odometrie();
    
    while(1);
    /*
    {
        //angle_AX12(PINCE_G, 735, 300, SANS_ATTENTE); //Position rangé
        //angle_AX12(ASCENSEUR, 500, 512, SANS_ATTENTE);   //Position haut
        //delay_ms(100);
        //angle_AX12(PINCE_G, 200, 300, SANS_ATTENTE); //Positions où il attrappe
        //angle_AX12(ASCENSEUR, 265, 512, SANS_ATTENTE);   //Position basse
        //delay_ms(100);
        //angle_AX12(PINCE_G, 320, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        //delay_ms(100);
        angle_AX12(PINCE_D, 285, 300, SANS_ATTENTE); //Position rangé
        delay_ms(100);
        angle_AX12(PINCE_D, 835, 300, SANS_ATTENTE); //Position où il attrappe
        delay_ms(100);
        //angle_AX12(PINCE_D, 720, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
        //delay_ms(100);
    }
    */
    return (EXIT_SUCCESS);
}


/*
 * angle_AX12(AX_CALAGE_CONE, 450, 1023, SANS_ATTENTE);    //Position déplié
 * angle_AX12(AX_CALAGE_CONE, 750, 1023, SANS_ATTENTE);    //Position replié
 * 
 * angle_AX12(ASCENSEUR, 500, 512, SANS_ATTENTE);   //Position haut
 * angle_AX12(ASCENSEUR, 265, 512, SANS_ATTENTE);   //Position basse
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
 * angle_AX12(PINCE_D, 285, 300, SANS_ATTENTE); //Position rangé
 * angle_AX12(PINCE_D, 835, 300, SANS_ATTENTE); //Position où il attrappe
 * angle_AX12(PINCE_D, 720, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
 * 
 * angle_AX12(PINCE_G, 735, 300, SANS_ATTENTE); //Position rangé
 * angle_AX12(PINCE_G, 200, 300, SANS_ATTENTE); //Positions où il attrappe
 * angle_AX12(PINCE_G, 320, 300, SANS_ATTENTE); //Position intermédiaire (où il est sur le point d'attraper)
 * 
 * angle_AX12(PARASOL, 350, 512, SANS_ATTENTE); //Position replié
 * 
 *        angle_AX12(PARASOL, 570, 150, SANS_ATTENTE);
        delay_ms(3000);
        angle_AX12(PARASOL, 350, 150, SANS_ATTENTE);
        delay_ms(3000);
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