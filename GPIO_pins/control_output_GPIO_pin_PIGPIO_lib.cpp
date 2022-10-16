

#include <pigpiod_if2.h>
#include <iostream>

const int LED = 5;

using namespace std;


int PigpioSetup()
{
   char *addrStr = NULL;
   char *portStr = NULL;
   int pi = pigpio_start(addrStr, portStr);
   set_mode(pi, LED, PI_OUTPUT);
   gpio_write(pi, LED, 0);

   return pi;
}

int main()
{
   
   int pi = PigpioSetup();
   if(pi>=0)
   {
      cout<<"daemon interface started ok at "<<pi<<endl;
   }
   else
   {
      cout<<"Failed to connect to PIGPIO Daemon - Try running sudo pigpiod and try again."<<endl;
      return -1;
   }
   
   gpio_write(pi, LED, 1);
   time_sleep(3.2);
   gpio_write(pi, LED, 0);
   pigpio_stop(pi);
   return 0;
}
