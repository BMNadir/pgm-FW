#IFNDEF PGM_LIB_H
#DEFINE PGM_LIB_H

//SFR Addresses 
#BYTE HLVDCON  = 0xFD2
#BYTE PIR2     = 0xFA1
#BYTE PIE2     = 0xFA0
#BYTE TRISA    = 0xF92
#BYTE ADCON1   = 0xFC1
#BYTE ADCON2   = 0xFC0
#BYTE PORTA    = 0xF80
#BYTE LATA     = 0xF89
#BYTE TRISB    = 0xF93
#BYTE LATB     = 0xF8A
#BYTE TRISC    = 0xF94
#BYTE LATC     = 0xF8B
#BYTE CCPR1L   = 0xFBE
#BYTE T0CON    = 0xFD5
#BYTE T2CON    = 0xFCA
#BYTE PR2      = 0xFCB
#BYTE CCP1CON  = 0xFBD
#BYTE CMCON    = 0xFB4
#BYTE CVRCON   = 0xFB5
#BYTE INTCON   = 0xFF2
#BYTE INTCON2  = 0xFF1
#BYTE PORTB    = 0xF81


//  PORT A  //
#BIT tris_ICSPDAT   =  TRISA.2
#BIT ICSPDAT_in     =  PORTA.2
#BIT ICSPDAT_out    =  LATA.2
#BIT tris_ICSPCLK   =  TRISA.3   
#BIT ICSPCLK_in     =  PORTA.3
#BIT ICSPCLK_out    =  LATA.3
#BIT tris_AUX       =  TRISA.4    
//#define AUX_in              PORTA, 4, 1
#BIT AUX            =  LATA.4

#BIT tris_MCLR_TGT  =  TRISA.5   
#BIT MCLR_TGT       =  LATA.5
//#define MCLR_TGT_pin        PORTA, 5   

//  PORT B  //
#BIT tris_Vpp_ON      =   TRISB.2
#BIT Vpp_ON           =   LATB.2
//#define Vpp_ON_pin    PORTB, 2      // WEK

#BIT tris_Vdd_TGT_N =  TRISB.3    // RB3 Output
#BIT Vdd_TGT_N      =  LATB.3
//#define Vdd_TGT_N_pin      PORTB, 3      // WEK

#BIT tris_Vdd_TGT_P =  TRISB.4  // RB4 Output
#BIT Vdd_TGT_P      =  LATB.4
//#define Vdd_TGT_P_pin      PORTB, 4      // WEK

#BIT tris_PROG_SWITCH =   TRISB.5   
#BIT PROG_SWITCH_pin  =   PORTB.5

//  PORT C  //
#BIT tris_BUSY_LED    =   TRISC.0
#BIT BUSY_LED         =   LATC.0
#BIT tris_Vpp_PUMP    =   TRISC.1   
#BIT Vpp_PUMP         =   LATC.1
#BIT tris_Vdd_TGT_ADJ =   TRISC.2
#BIT Vdd_TGT_ADJ      =   LATC.2
#BIT tris_WP          =  TRISC.6
#BIT WP               =  LATC.6


#BIT HLVDIF = PIR2.2
#BIT HLVDIE = PIE2.2
#BIT TRISA0 = TRISA.0
#BIT TRISA1 = TRISA.1
#BIT RBPU   = INTCON2.7
#BIT PEIE   = INTCON.6
#BIT GIE    = INTCON.7




//DEFINITIONS 
#define PWM_150kHz  79  
// PWM duty cycle for (CCPR1L register value)
#define Vdd_5V0     (192/4)         //  5.0V Vdd
#define Vdd_3V3     (116/4)         //  3.3V Vdd
#define Vdd_2V5     ( 88/4)         //  2.5V Vdd
#define Vdd_1V8     ( 64/4)         //  1.8V Vdd


#ENDIF








