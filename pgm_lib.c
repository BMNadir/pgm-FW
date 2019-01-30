/* This file contains all programmer functions */
#include "pgm_lib.h"  
#INCLUDE <stdlibm.h> //necessary for malloc 

volatile unsigned int8 DATAin[64];  //USB packet is copied here when received 
unsigned int8 i;     //used to iterate through DATAin in Process_Input()

unsigned int8 DATA_Out_Buffer[128]; //Data is stored here by scripts and read by USB
struct 
{
   unsigned int8 rd_idx;
   unsigned int8 wr_idx;
   unsigned int8 nbr_bytes; //number of bytes in DATA_On_Buffer
} DOM_mngnt;   //DATA_Out_Buffer Management

unsigned int8 DATA_In_Buffer[256];
struct 
{
   unsigned int8 rd_idx;
   unsigned int8 wr_idx;
   unsigned int8 nbr_bytes; //number of bytes in DATA_In_Buffer
} DIB_mngnt;   //DATA_In_Buffer Management 

//unsigned int8 scrpt_args[20];
//unsigned int8  scrpt_rd_idx = 0; //scrpt_args_num_bytes = 0,

struct {
   unsigned int8   VddThreshold;   // error detect threshold
   unsigned int8   VppThreshold;   // error detect threshold
    unsigned int8   VddErrCount;
    unsigned int8   VppErrCount;
    unsigned int16    BlinkClount;    // counter for blinking Busy on VDD/VPP error
} VddVppLevels;

struct {
   unsigned int8 CCPRSetPoint;
   unsigned int8 UppperLimit;
   unsigned int8 LowerLimit;   
} Vpp_PWM;

struct {
    unsigned int16    adc_calfactor;      // CalibratedResult = (ADRES * adc_calfactor) >> 8
    signed int8       vdd_offset;
    unsigned int8   vdd_calfactor;      // Calibrated CCP value = (((CCP >> 6) + vdd_offset) * vdd_calfactor) >> 7
} VoltageCalibration; 


void pgm_init()
{
   #ASM
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
   
   BSF tris_ICSPDAT        // pin A2 set as input
   BCF ICSPDAT_in
   BCF ICSPDAT_out
   
   BCF tris_ICSPCLK        // pin A3 set as output, why was it initially set as input ?
   BCF ICSPCLK_in
   BCF ICSPCLK_out
   
   BSF tris_AUX            // pin A4 set as input
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
   MOVWF CCP1CON           // PWM mode, datasheet page 151
   
   MOVLW Vdd_3V3            
   MOVWF CCPR1L
   
   // initialize CCP2 for VPP 
   //Maybe add later
   
   MOVLW 0x07              
   MOVWF CMCON          // Comparators off
   
   CLRF CVRCON          // Voltage Reference off
   
   // Set up Interrupts
    BSF PEIE         //enable Peripheral Interrupt Enable
    BSF GIE          //enable global interrupts
   
    // Start voltage monitoring
    //ppVddADCTmr1_Start();
    
    BSF PEIE         //Enable Peripheral interrupts 
    BSF GIE          //Enable global interrupts
   #ENDASM
   VoltageCalibration.adc_calfactor = 0x0100;  // Set default values. 
   VoltageCalibration.vdd_offset = 0x00;
   VoltageCalibration.vdd_calfactor = 0x80;
}

void Process_Input ()
{
/*
   #ASM 
      BSF BUSY_LED
   #ENDASM*/
   i = 1;   //Initialize index
   usb_get_packet(1, DATAin, 64);
   //usb_flush_out(1, USB_DTS_TOGGLE);
   delay_ms(1);
   unsigned int8 packet_length = DATAin[0];
   unsigned int8 offset;

   while (i < packet_length)
   {
      offset = DATAin[i];
      if (offset < 0x01) break; //unknown instruction
      
      offset -= 0x01;
      offset *= 2;
      offset += 8;
      
      #ASM
         MOVF     PCL, W  //Save current PCL value in Wreg, by reading PCL, PCH and PCU will be loaded into PCLATH and PCLATU 
         ADDWF    offset, W
         BTFSC    C
         INCF     PCLATH
         MOVWF    PCL
         BRA      getVersionLbl
         BRA      toggleLEDLbl
         BRA      setVDDLbl
         BRA      setVPPLbl
         BRA      readVoltagesLbl
         BRA      runROMScriptLbl
         BRA      downloadScrptArgsLbl
      #ENDASM 
      
getVersionLbl:
      getVersionNumber ();
      break;
toggleLEDLbl:
      #ASM
         BTG BUSY_LED
      #ENDASM
      break;
setVDDLbl:
      /*
       * DATAin[i+1] = CCPL
       * DATAin[i+2] = CCPH
       * DATAin[i+3] = VDDLim
      */
      // CCPH:CCPL = ((Vdd * 32) + 10.5) << 6, << 6 because CCP1 (holds the duty cycle, resolution is 10 bits) is left justified, 
      // and is left justified because the 2 LSB bits are located in CCP1CON.
      // Duty_cycle = vdd * 32+ 10.5
      calAndSetCCP (DATAin[i+2], DATAin[i+1]);
      VddVppLevels.VddThreshold = CalThresholdByte (DATAin[i+3]);
      i += 4;
      continue;
setVPPLbl:
      /*
       * DATAin[i+1] = CCPR2L, this is the duty cycle, generally = 0x40;
       * DATAin[i+2] = VPPADC = Vpp * 18.61          Vpp is the desired voltage.
       * DATAin[i+3] = VPPlim = Vfault * 18.61 
      */
      Vpp_PWM.CCPRSetPoint = DATAin[i+1];
      Vpp_PWM.UppperLimit = CalThresholdByte(DATAin[i+2])+1; //VPP upper limit = VPP + 1
      Vpp_PWM.LowerLimit = Vpp_PWM.UppperLimit - 2;        //VPP lower limit = VPP - 1
      VddVppLevels.VppThreshold = CalThresholdByte(DATAin[i+3]); //calibrate VPPLim 
      i += 4;
      continue;
      
readVoltagesLbl:
      sendVoltages ();
      i++;
      continue;
      
runROMScriptLbl:
      /*
       * DATAin[i+1] = Script's length
       * DATAin[i+2] = Least significant byte of the script's address
       * DATAin[i+3] = Most significant byte of the script's address
      */
      unsigned int8 *scriptBuffer = malloc (DATAin[i+1]);
      unsigned int16 address =  ((DATAin[i+3] * 0x100) + DATAin[i+2]);
      read_program_memory(address, scriptBuffer, DATAin[i+1]); 
      executeScript(DATAin[i+1], scriptBuffer);
      free(scriptBuffer);
      i += 4;
      continue;
      
downloadScrptArgsLbl:
   //downloadScriptArgs();
   i++;
   continue;
   }  
   /*
   #ASM
      BCF BUSY_LED
   #ENDASM*/
}

void getVersionNumber (void)
{
   unsigned int8 *DATAout  = malloc(4);
   *DATAout++ = 4;  //length of data to be sent, including this byte
   *DATAout++ = 0;
   *DATAout++ = 0;
   *DATAout = 1;
   usb_put_packet(1, (DATAout-3), 64, USB_DTS_TOGGLE);
   free(DATAout);
   
   /*
   DATAin [0] = 4;  //length of data to be sent, including this byte
   DATAin [1] = 0;
   DATAin [2] = 0;
   DATAin [3] = 1;
   usb_put_packet(1, DATAin, 64, USB_DTS_TOGGLE);
   */
}

//Used in setVDD
void calAndSetCCP (unsigned int8 ccph, unsigned int8 ccpl)
{
   signed int16 ccp1 = (ccph * 0x100) + ccpl; //ccp1 = ccph:ccpl
   unsigned int8 tempCal;
   ccp1 >>= 6; //
   ccp1 += VoltageCalibration.vdd_offset;
   ccp1 *= VoltageCalibration.vdd_calfactor;     //default is 0x80 = 0b10000000
   ccp1 >>= 1;                                   //shift by one to get the original >> 6 after multiplying by 0x80
   tempCal = (unsigned int8) (ccp1 >> 8);
   #ASM 
   MOVLW tempCal
   MOVWF CCPR1L
   #ENDASM
   
   //The following is to place the lower 2 bits of the duty cycle resolution in bits 4 and 5 of CCP1CON
   CCP1 &= 0xFF;
   tempCal = (unsigned int8) (CCP1 >> 2);
   /*
   CCP1CON = (CCP1CON & 0xCF) | tempCal;
   */
   #ASM 
   MOVLW 0xCF
   ANDWF CCP1CON,W
   IORLW tempCal
   MOVWF CCP1CON
   #ENDASM
}

unsigned int8 CalThresholdByte(unsigned int8 voltageVal)
{
    unsigned int8 inverse_cal = 0x0200 - VoltageCalibration.adc_calfactor; //adc_calfactor by default is 0x0100 so, 0x0200 - adc_calfactor is the same as 1 / adc_calfactor 
    inverse_cal *= voltageVal;
    inverse_cal >>= 8;

    return (unsigned int8) inverse_cal;
} //See CalADCWord(unsigned int rawValue)

//TODO: create a function to store cal and offset values in eeprom and a function to read them

//Read VDD and VPP voltages, used to detect self-powered targets 
void sendVoltages (void)
{
   unsigned int16 adc_result;
   unsigned int8 *DATAout = malloc(5);
   *DATAout++ = 5;   //Length of data to be sent
   ADC_VPP_VDD_control (0); //Stop ADC, VPP and VDD
   getADC (0x04); //CH1_VDD
   adc_result = (ADRESH * 0x100) + ADRESL;
   adc_result = calADCWord(adc_result);
   *DATAout++ = (unsigned int8) adc_result;
   *DATAout++ = (unsigned int8) (adc_result >> 8);
   
   getADC (0x00); //CH0_VPP
   adc_result = (ADRESH * 0x100) + ADRESL;
   adc_result = calADCWord(adc_result);
   *DATAout++ = (unsigned int8) adc_result;
   *DATAout = (unsigned int8) (adc_result >> 8);
   
   usb_put_packet(1, (DATAout-4), 64, USB_DTS_TOGGLE);
   free(DATAout);
   ADC_VPP_VDD_control (1); //Start ADC, VPP and VDD
}

void ADC_VPP_VDD_control (int1 state)
{
   if (state)
   {//Activate modules
      VddVppLevels.VddErrCount = 0;
      VddVppLevels.VppErrCount = 0;
      #ASM
         MOVLW 0x80
         MOVWF T1CON            // Rd16 1:1 prescale off Fosc/4
         MOVLW 0xFA
         MOVWF TMR1H            //// 65536-1480 = about 125us
         MOVLW 0x38
         MOVWF TMR1L
         
         BCF   TMR1IF           // clear int flag
         BSF   TMR1IE           // enable int
         BSF   TMR1ON           // start timer
         
         BCF   ADIF  // clear A/D Converter Interrupt Flag
         BCF   ADIE  // Disable ADC int
         MOVLW 0x26
         MOVWF ADCON2           //ADCON2 setting for voltage monitoring
         MOVLW 0x05
         MOVWF ADCON0           // Set ADC channel for VDD_TGT_FB and turn on ADC (ADCON0)
         BSF   GO                // Start conversion.
      #ENDASM
   }
   else 
   { //Deactivate modules
      #ASM
         BCF   TMR1IE   // Disable Timer1 interrupt
         BCF   TMR1ON   // Stop Timer1
         
         CLRF  ADCON0   //Shut off ADC conversion in progress
         
         BCF   ADIF  // clear A/D Converter Interrupt Flag
         BCF   ADIE  // Disable ADC int
      #ENDASM
   }
}

void getADC (unsigned int8 channel)
{
   ADCON0 = (channel + 1);     // set channel. ADD 1 to enable A/D Converter (bit 0) page 261
   #ASM
   BSF GO                      // begin conversion
   #ENDASM   
   while ((ADCON0 | 0x02));       // wait while GO is still set
}

unsigned int16 calADCWord(unsigned int16 Val)
{

    unsigned int32 cal_value = Val;
    cal_value *= VoltageCalibration.adc_calfactor;
    cal_value >>= 8;
    if (cal_value > 0xFFFF)
        cal_value = 0xFFFF;

    return (unsigned int16) cal_value;
}
/*
void downloadScriptArgs (void)
{
   unsigned int8 len = DATAin[++i];
   for (unsigned int8 j = 0; j < len; j++)
   {
      scrpt_args[j] = DATAin[++i];           //Copy the arguments from USB packet to scrpt_args buffer 
   }
   scrpt_rd_idx = 0;
}

*/





void executeScript(unsigned int8 scrpt_len, unsigned int16 *scriptLocation)
{
   unsigned int8 si = 0; //initialize script index to 0
   unsigned int8 offset, loop_buff_idx, temp;
   int1 first_iteration = 1; //used by LOOPBUFFER cmd
   unsigned int16 nbr_iterations;
   unsigned int8 *SFR_ptr; //used by the WRITE_SFR and READ_SFR commands
   
   #ASM
      BCF INT0IE //Disable Timer0 interrupt
      MOVLW 0x07
      MOVWF T0CON //16-bit timer, 1:256 prescale
   #ENDASM
   
   if (scrpt_len == 0) return; 
   while (si < scrpt_len)
   {
      offset = *(scriptLocation + si); 
      offset -= 0xD5;  // Consider 0xD5 to be the minimum for now
      offset *= 2;     
      offset += 8;     
      #ASM
         MOVF     PCL, W  //Save current PCL value in Wreg, by reading PCL, PCH and PCU will be loaded into PCLATH and PCLATU 
         ADDWF    offset, W
         BTFSC    C
         INCF     PCLATH
         MOVWF    PCL
         BRA      readN_BitsLbl  //Read N bits from target and store them in DATA_Out_Buffer
         BRA      readByteLbl
         BRA      VISI24Lbl
         BRA      NOP24Lbl
         BRA      COREINST24Lbl 
         BRA      COREINST18Lbl
         BRA      POP_DOWNLOAD_BUFFERLbl
         BRA      readICSP_StatesLbl
         BRA      LOOPBUFFERLbl
         BRA      WRITE_SFRLbl
         BRA      READ_SFRLbl
         BRA      EXIT_SCRIPTLbl
         BRA      GOTO_IDXLbl
         BRA      IF_GT_GOTOLbl
         BRA      IF_EQ_GOTOLbl
      #ENDASM 
readN_BitsLbl:
      write_upload_buff(readN_Bits(*(scriptLocation + ++si)));//WriteUploadBuffer(readN_Bits(*(scriptLocation + si + 1)));
      si++;
      continue;
   
readByteLbl:
      write_upload_buff(readN_Bits(8));
      si++;
      continue;

VISI24Lbl:
      ShiftBitsOutICSP(1, 4);
      ShiftBitsOutICSP(0, 8);
      write_upload_buff(readN_Bits(8));
      write_upload_buff(readN_Bits(8));
      si++;
      continue;
   
NOP24Lbl:
      ShiftBitsOutICSP(0, 8);
      ShiftBitsOutICSP(0, 8);
      ShiftBitsOutICSP(0, 8);
      ShiftBitsOutICSP(0, 8);
      si++;
      continue;
   
COREINST24Lbl:
      ShiftBitsOutICSP(0, 4);
      ShiftBitsOutICSP(*(scriptLocation + ++si), 8);
      ShiftBitsOutICSP(*(scriptLocation + ++si), 8);
      ShiftBitsOutICSP(*(scriptLocation + ++si), 8);
      si++;
      continue;

COREINST18Lbl:
      ShiftBitsOutICSP(0, 4);
      ShiftBitsOutICSP(*(scriptLocation + ++si), 8);
      ShiftBitsOutICSP(*(scriptLocation + ++si), 8);
      si++;
      continue;
   
POP_DOWNLOAD_BUFFERLbl:
      pop_down_buff ();
      si++;
      continue;
   
readICSP_StatesLbl:
      write_upload_buff(getICSP_States());
      si++;
      continue;
   
LOOPBUFFERLbl:  //will loop through a number of script commands, argument is the number if bytes to loop through 
      if (!first_iteration)
      {
         nbr_iterations--;
         if (nbr_iterations == 0)   //no iterations 
         {
            first_iteration = 1; //number of iterations endded for the current run 
            si ++;
            continue;
         }
         si = first_iteration;   //Still iterating
         continue;
      }
      loop_buff_idx = si - *(scriptLocation + ++si); //always loops to the instruction before it, except in the script n 255 in the original Firmware
      //scrpt_rd_idx -= 2;      //the instruction before LOOPBUFFER has 2 args
      nbr_iterations = (unsigned int16) pop_down_buff ();   //low byte
      nbr_iterations += (256  *  pop_down_buff ());         //upper byte
      if (nbr_iterations == 0)   //no iterations 
      {
         si++; //advance to next command 
         continue;
      }
      first_iteration = 0;
      si = loop_buff_idx;  
      continue;
      
WRITE_SFRLbl:
      SFR_ptr = (unsigned int8 *) 0x0F00 + *(scriptLocation + ++si);
      *SFR_ptr = *(scriptLocation + ++si);
      si++;
      continue;

READ_SFRLbl:
      SFR_ptr = (unsigned int8 *) 0x0F00 + *(scriptLocation + ++si);
      write_upload_buff(*SFR_ptr);
      si++;
      continue;
      
EXIT_SCRIPTLbl:
      si = scrpt_len;
      continue;
      
GOTO_IDXLbl:
      si = *(scriptLocation + ++si);
      continue;
      
IF_GT_GOTOLbl: //if last loaded byte in DATA_Out_Buffer is greater than arg[1], execution will branch to offset specified by arg[2] 
      temp = DATA_Out_Buffer[DOM_mngnt.wr_idx - 1]; //get last byte written to DATA_Out_Buffer, - 1 because DOM_mngnt.wr_idx is always post-incremented, and points to the next location to be written
      if (temp > *(scriptLocation + ++si))
      {
         si += (signed int8) *(scriptLocation + ++si);
      }
      else 
      {
         si++;
      }
      continue;
      
IF_EQ_GOTOLbl: //if last loaded byte in DATA_Out_Buffer is equal than arg[1], execution will branch to offset specified by arg[2] 
      temp = DATA_Out_Buffer[DOM_mngnt.wr_idx - 1]; //get last byte written to DATA_Out_Buffer, - 1 because DOM_mngnt.wr_idx is always post-incremented, and points to the next location to be written
      if (temp == *(scriptLocation + ++si))
      {
         si += (signed int8) *(scriptLocation + ++si);
      }
      else 
      {
         si++;
      }
      continue;
   } //end of while loop
}


/* 
 * FOR PIC24F
 * Reads N bits and copy them in the upload buffer 
 * **ShiftBitsInPIC24(n)**
*/
unsigned int8 readN_Bits(unsigned int8 numberOfBits)
{
   unsigned int8 bitsBuff = 0; //Bits are shifted in here 
   //unsigned int8 iteration = numberOfBits;
   #BIT buff0 = bitsBuff.0
   #ASM
      BSF      tris_ICSPDAT  //Set PGD pin as input
      //MOVLW 0x01        //this will be used to set the bits 
   READBITSLOOP:
      BTFSC    ICSPDAT_in  //read PGD pin
      BSF      buff0      //If PGD was high, set the corresponding bit starting from LSB
      BSF      ICSPCLK_out       //Clock rising edge
      //RLNCF    W,W               //Rotate W left, (no carry)
      NOP
      NOP
      NOP
      BCF      ICSPCLK_out    //Clock falling edge 
      NOP
      RRNCF    bitsBuff, f    //Rotate right (no carry)
      DECFSZ   numberOfBits, f
      BRA      READBITSLOOP
   #ENDASM
   return bitsBuff;
}


void write_upload_buff(unsigned int8 wrByte)
{
   if (DOM_mngnt.nbr_bytes > 127) 
   {
      return;
   }
   DATA_Out_Buffer[DOM_mngnt.wr_idx++] = wrByte;
   if (DOM_mngnt.wr_idx > 127)
      DOM_mngnt.wr_idx = 0;
   
   DOM_mngnt.nbr_bytes++;
}

/*
 * Note: ICSP pins must be set to outputs
 *
 *
*/
void shiftBitsOutICSP (unsigned int8 charToBeShifted, unsigned int8 numberOfBits)
{
   unsigned int8 tempChar = charToBeShifted;
   unsigned int8 tempNumBits = numberOfBits;
   
   #BIT char0=tempChar.0
   //#BIT num0=numberOfBits.0
   
   #ASM
      BITSOUTLOOP:
         BTFSS    char0
         BCF      ICSPDAT_out    //If LSB of charToBeShifted is 0, clear PGD pin
         BTFSC    char0
         BCF      ICSPDAT_out
         NOP
         BSF      ICSPCLK_out    //Clock rising edge 
         NOP
         BCF      ICSPCLK_out
         RRNCF    tempChar
         DECFSZ   numberOfBits
         BRA      BITSOUTLOOP
   #ENDASM
}

unsigned int8 getICSP_States(void)
{
   unsigned int8 state = 0;
   if (ICSPDAT_in == 1)    //ICSPDAT_in = PORTA.2
      state |= 0x02;
   if (ICSPCLK_in == 1)    //ICSPCLK_in = PORTA.3
      state |= 0x01;
      
   return state;
}

//Return a byte from the DATA_In_Buffer
unsigned int8 pop_down_buff (void)
{
   unsigned int8 popped; 
   if (DIB_mngnt.nbr_bytes == 0)
   {
      return 0;
   }
   popped = DATA_In_Buffer[DIB_mngnt.rd_idx++];
   DIB_mngnt.nbr_bytes--;
   if (DIB_mngnt.rd_idx > 255)
      DIB_mngnt.rd_idx = 0;
   
   return popped;
}














