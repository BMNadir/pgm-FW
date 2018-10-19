#include <main.h>

void main()
{
   
   usb_init_cs(); 
   while(TRUE)
   {
      usb_task();
      //if(usb_enumerated()){}
   }
}
