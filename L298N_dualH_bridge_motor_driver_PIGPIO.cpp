


#include <iostream>
#include <pigpiod_if2.h>



const int PWM_A = 21;
const int MOTOR_A_FWD = 26;
const int MOTOR_A_REV = 13;

int pigpio_setup()
{
    char *addrStr = NULL;
    char *portStr = NULL;
    const int pi = pigpio_start(addrStr, portStr);

    set_mode(pi,PWM_A, PI_OUTPUT);
    set_mode(pi,MOTOR_A_FWD, PI_OUTPUT);
    set_mode(pi,MOTOR_A_REV, PI_OUTPUT);

    gpio_write(pi, MOTOR_A_FWD, 1);
    gpio_write(pi, MOTOR_A_REV, 1);
    return pi;
}

using namespace std;

int main()
{
    int pi = pigpio_setup();
    if(pi < 0)
    {
        cout<<"Failed to connect to Pigpio Daemon. Is it running?"<<endl;
        return -1;
    }

    gpio_write(pi, MOTOR_A_FWD, 0);

    set_PWM_dutycycle(pi, PWM_A, 127);
    time_sleep(3); 
    set_PWM_dutycycle(pi, PWM_A, 255);
    time_sleep(3);
    gpio_write(pi, MOTOR_A_FWD, 1);
    time_sleep(1);
    gpio_write(pi, MOTOR_A_REV, 0);
    set_PWM_dutycycle(pi, PWM_A, 127);
    time_sleep(3);
    set_PWM_dutycycle(pi, PWM_A, 255);
    time_sleep(3);
    gpio_write(pi, MOTOR_A_REV, 1);

    pigpio_stop(pi);
    return 0;
}
