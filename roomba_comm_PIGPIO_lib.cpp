

#include "pigpiod_if2.h"

using namespace std;

int pi = -1;
int serHandle = -1;


void wake(){
    int R1 = 23;  
    set_mode(pi, R1, PI_OUTPUT);

    gpio_write(pi, R1, 0);    
    time_sleep(.1);
    gpio_write(pi, R1, 1);

    serial_write_byte(pi,serHandle,128);        
    time_sleep(.15);
    serial_write_byte(pi,serHandle,131);         
    time_sleep(.15);
}


void rev()
{
   char driveString[] = {137, 255, 136, 0, 0};
   serial_write(pi, serHandle, driveString, 5);
}


void fwd()
{
   char driveString[] = {137, 0, 120, 127, 255};
   serial_write(pi, serHandle, driveString, 5);
}


void stop()
{
   char driveString[] = {137, 0, 0, 0, 0};
   serial_write(pi, serHandle, driveString, 5);
}


void shutdown()
{
   serial_write_byte(pi,serHandle,133);
   time_sleep(.1);
   serial_close(pi, serHandle);
   pigpio_stop(pi);
}


int main()
{
   char *addrStr = NULL;
   char *portStr = NULL;
  
   pi = pigpio_start(addrStr, portStr);

   serHandle = serial_open(pi, "/dev/ttyUSB0",115200,0);

   wake();
   fwd();
   time_sleep(5);
   stop();
   time_sleep(1);
   rev();
   time_sleep(5);
   stop();
   shutdown();
}
