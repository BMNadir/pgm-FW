/* This file contains all programmer functions */
#include "pgm_lib.h"  



void pgm_init()
{
   #ASM
   MOVLB 0x0F               //Move literal to Bank Select Register<3:0>
   MOVLW 0x13
   MOVWF HLVDCON           //Configure HIGH/LOW-VOLTAGE DETECT CONTROL REGISTER, See datasheet page 281
   
   BCF HLVDIF          //Clear High/Low-Voltage Detect Interrupt Flag bit, Datasheet page 105
   BSF HLVDIE         //Enable High/Low-Voltage Detect Interrupt, datasheet page 107
   
   //Initialize I/O
   
   BSF TRISA0        // Set pin A0 as input, datasheet page 113
   BSF TRISA1        // Set pin A1 as input
   
   MOVLW 0x0D
   MOVWF ADCON1            // A0 & A1 Set as Analog inputs, DS page 262
   
   MOVLW 0x03
   MOVWF ADCON2            // A/D Conversion Clock Select bits, clock derived from A/D RC oscillator, DS page 263
   
   BSF tris_ICSPDAT        // pin A2 set as inputs
   BCF ICSPDAT_in
   BCF ICSPDAT_out
   
   BSF tris_ICSPCLK        // pin A3 set as inputs
   BCF ICSPCLK_in
   BCF ICSPCLK_out
   
   BSF tris_AUX            // pin A4 set as inputs
   BCF AUX
   
   BCF MCLR_TGT
   BCF tris_MCLR_TGT       // A5 Set as output
   
   BCF Vdd_TGT_N           // initialize half-bridge N-gate off
   BCF tris_Vdd_TGT_N
   
   BSF Vdd_TGT_P           // initialize half-bridge P-gate off
   BCF tris_Vdd_TGT_P
   
   BSF tris_PROG_SWITCH    // B5 input
   BSF PROG_SWITCH_pin     // initialize to 1
   BCF RBPU                // PORTB pull-ups are enabled by individual port latch value, DS page 102
   
   BCF Vpp_ON              // Initialize Vpp to off
   BCF tris_Vpp_ON         // B2 set as output
   
   BCF BUSY_LED            
   BCF tris_BUSY_LED       // Busy_led pin set as output
   
   BCF Vpp_PUMP            
   BCF tris_Vpp_PUMP       // C1 output (CCP2)
   
   BCF Vdd_TGT_ADJ
   BCF tris_Vdd_TGT_ADJ    // C2 output (CCP1)
   
   MOVLW 0x08
   MOVWF T0CON             //Timer0 off, 16-bit timer, internal clock, prescaler not assigned
   
   //Timer1 is used by VddVpp voltage monitoring interrupt or UART RX.
   //configure Timer2 (used by CCP1 and CCP2)
   
   MOVLW 0x04
   MOVWF T2CON             //1:1 Postscale, 1:1 Prescale, Timer2 on
   
   MOVLW PWM_150kHz        // PWM freq = 150kHz
   MOVWF PR2               //Timer2 Period Register (sets PWM Period for CCP1 and 2)
   
   MOVLW 0x0C              
   MOVWF CCP1CON           // PWM mode
   
   MOVLW Vdd_3V3            
   MOVWF CCPR1L
   
   // initialize CCP2 for VPP WEK
   //Maybe add later
   
   MOVLW 0x07              
   MOVWF CMCON          // Comparators off
   
   CLRF CVRCON          // Voltage Reference off
   
   // Set up Interrupts
    BSF PEIE         //enable Peripheral Interrupt Enable
    BSF GIE          //enable global interrupts
   
    // Start voltage monitoring
    //ppVddADCTmr1_Start();
   #ENDASM
}
























