#include <main.h>


unsigned int8 data[64];
void main()
{
   
   usb_init_cs(); 
   //pgm_init();
   
   #ASM 
   BSF BUSY_LED
   #ENDASM
   delay_ms(500);
   #ASM 
   BCF BUSY_LED
   #ENDASM
   
   while(TRUE)
   {
      usb_task();
      if(usb_enumerated()){// If the device has been enumerated by the PC
         if(usb_kbhit(1))
         {                  // If endpoint1 has data in it's receive buffer
         
         /*
           usb_get_packet(1, data, 64);
           delay_ms(1);                          // Wait 1 second
           for (int8 i = 0; i < 64; i++)
           {
               data[i] *= 2;
           }
           // Return the received bytes back
           usb_put_packet(1, data, 64, USB_DTS_TOGGLE);*/
         }
       }
   }
}


