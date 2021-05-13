
#include <stdint.h>
#include <stdbool.h>

#include "inc/tm4c123ge6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"

uint32_t ui32Period;
/**
 * main.c
 */

void setup(void){

    ///////// CONFIGURACIÓN DEL RELOJ ///////////////////////
        SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
         //CLK FREQUENCY 40 MHz
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //ENABLES PORT F FOR TIMER
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); //ENABLES TIMER0

    ////////////////////////////////////////////////////////////
         ///////// CONFIGURACIÓN DE LOS PINES ///////////////////////
         GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);          //output for the leds rbg
         //GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); //pin PUSH1
         ////////////////////////////////////////////////////////////
    ///////// CONFIGURACIÓN DEL Timer  0,  como  un  timer  de  32-bits///////////////////////
         //T0CCP1
         TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
         ui32Period = (SysCtlClockGet()/2);     //calculates period of 1 second -SysCtlClockGet = 40000000
         TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period-1);
         IntEnable(INT_TIMER0A);
         TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
         IntMasterEnable();
         TimerEnable(TIMER0_BASE, TIMER_A);
         ////////////////////////////////////////////////////////////


}
int main(void)
{
	setup();

     while(1){

     }//while(1) "loop"

}//main.

void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    } //Timer0IntHandler

}


