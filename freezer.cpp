

#include "pigpiod_if2.h"

using namespace std;


{
   int pi = -1;
   int serHandle = -1;
   char *addrStr = NULL;
   char *portStr = NULL;

   pi = pigpio_start(addrStr, portStr);

 
   serHandle = serial_open(pi, "/dev/ttyAMA0",115200,0);

   for(int i = 0; i < 100; i++)
   {
        serial_write_byte(pi, serHandle, 133);
        time_sleep(.01);
   }

   time_sleep(.1);
   serial_close(pi, serHandle);
   pigpio_stop(pi);
}
