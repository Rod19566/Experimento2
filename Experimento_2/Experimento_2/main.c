
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

extern void Timer0IntHandler(void);
extern void UARTIntHandler(void);

uint32_t ui32Period;

unsigned char l = 'a';
uint8_t r = 0;
uint8_t b = 0;
uint8_t g = 0;
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
         TimerEnable(TIMER0_BASE, TIMER_A);
         ////////////////////////////////////////////////////////////

         //CONFIGURACION UART//////////////////////////
         SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //SE HABILITA PUERTO A
         SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //SE HABILITA EL UART
         UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
  //       GPIOPinConfigure(0x00000001); //RX
  //       GPIOPinConfigure(0x00000401); //TX
         UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
         GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
         UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
         IntEnable(INT_UART0);
         UARTEnable(UART0_BASE);

         UARTIntRegister(UART0_BASE, UARTIntHandler);
         IntMasterEnable();    // SE HABILITAN LAS INTERRUPCIONES GLOBALES
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
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //se apaga
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //se prende
    } //Timer0IntHandler

}

void UARTIntHandler(void){

    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT); //SE LIMPIA EL UART
    l = UARTCharGet(UART0_BASE);

    switch(l){
    case 'r':
        if (r==0){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
            r = 1;
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0);
            r = 0;
        } //if // else
        break;
    case 'b':
        if (b==0){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            b = 1;
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0);
            b = 0;
        } //if // else
        break;
    case 'g':
        if (g==0){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            g = 1;
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0);
            g = 0;
        } //if // else
            break;
    } //switch l

}//void UARTIntHandler


