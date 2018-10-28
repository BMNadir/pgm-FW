#IFNDEF MAIN_H
#DEFINE MAIN_H


#include <18F2550.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#fuses  HSPLL PLL5 CPUDIV2 USBDIV VREGEN NOMCLR
#use delay(clock=48000000)


#include "usb/pic18_usb.h"
#include "usb/usb_desc_hid.h"
#include "usb/usb.c"

#include "pgm_lib.c"

#ENDIF


