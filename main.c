#include <main.h>
#include "isr.c"

void main()
{
   pgm_init();
   usb_init_cs(); 
   
   while(TRUE)
   {
      usb_task();
      if(usb_enumerated()){// If the device has been enumerated by the PC
         if(usb_kbhit(1))
         {                  // If endpoint1 has data in its receive buffer
            Process_Input ();
         }
       }
   }
}


