
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
#include "driverlib/can.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

/* These header files define the messages that will be sent on the CAN bus */
#include "CANMsg.h"



#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


/* A counter that keeps track of the number of times the RX interrupt has
 * occurred, which should match the number of TX messages that were sent. */
volatile uint32_t g_ui32MsgCount = 0;

/* A flag to indicate that some transmission error occurred. */
volatile bool g_bErrFlag = 0;

/* Timer counter (in 1/10s) */
volatile int32_t g_timeds;


/* Delay for x milliseconds */
void Delay(uint32_t ui32mSeconds)
{
    SysCtlDelay(ui32mSeconds*16000 / 3);
}

/* This function is the interrupt handler for the CAN peripheral.  It checks
 * for the cause of the interrupt, and maintains a count of all messages that
 * have been received. */
void CANIntHandler(void) {
    uint32_t ui32Status;

    /* Read the CAN interrupt status to find the cause of the interrupt */
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    /* If the cause is a controller status interrupt, then get the status */
    if(ui32Status == CAN_INT_INTID_STATUS) {
        /* Read the controller status. */
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        /* Set a flag to indicate some errors may have occurred. */
        g_bErrFlag = 1;
    }

    /* Check if the cause is message object 1, which what we are using for receiving messages. */
    else if(ui32Status == 1) {
        /* Getting to this point means that the RX interrupt occurred on
         * message object 1, and the message reception is complete.  Clear the
         * message object interrupt. */
        CANIntClear(CAN0_BASE, 1);

        /* Increment a counter to keep track of how many messages have been received. */
        g_ui32MsgCount++;

        /* Since a message was received, clear any error flags. */
        g_bErrFlag = 0;
    }
}


/* Configure the UART and its pins.  This must be called before UARTprintf(). */
void ConfigureUART(void) {
    /* Enable the GPIO Peripheral used by the UART. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /* Enable UART0 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Configure GPIO Pins for UART mode. */
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Use the internal 16MHz oscillator as the UART clock source. */
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    /* Initialize the UART for console I/O.
     * NOTE: The baud rate is 115200 - this must be same in Putty etc */
    UARTStdioConfig(0, 115200, 16000000);
}


void ConfigureCAN(void) {
    /* For this example CAN0 is used with RX and TX pins on pins B4 and B5.
     * GPIO port B needs to be enabled so these pins can be used. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    /* Enable these pins as CAN pins */
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    /* Configure these pins as CAN pins */
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    /* Enable the CAN peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    // Initialise the CAN controller
    CANInit(CAN0_BASE);

    /* Set CAN clock to 500 kHz */
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);

    /* Enable interrupts on the CAN peripheral. */
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    /* Enable the CAN interrupt on the processor (NVIC). */
    IntEnable(INT_CAN0);

    /* Enable the CAN for operation. */
    CANEnable(CAN0_BASE);
}


/* Sends a CANMsg using the microcontroller's CANperipheral */
void SendCANMsg( CANMsg *msg ) {
    tCANMsgObject sCANMessage;

    /* Fill CAN message object with message */
    sCANMessage.ui32MsgID = msg->ID;
    sCANMessage.ui32MsgIDMask = 0;
    sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    sCANMessage.ui32MsgLen = 8;
    sCANMessage.pui8MsgData = msg->payload;

    /* Send message */
    CANMessageSet( CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_TX );
}


void SysTickHandler(void) {
    g_timeds++;
}


int main(void) {
    CANMsg msg;

    /* Set the clocking to run directly from the external crystal/oscillator. */
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                      SYSCTL_XTAL_16MHZ);

    /* Initialize the UART. */
    ConfigureUART();

    /* Initialize the CAN message handler. */
    ConfigureCAN();

    /* Start a timers that counts in 0.1s intervals */
    SysTickPeriodSet(SysCtlClockGet() / 10);

    IntMasterEnable();
    SysTickIntEnable();
    SysTickEnable();

    /* You'll need a terminal listening to the relevant COM port to see this text */
    UARTprintf("Beginning CAN packet transmission...\n");

    while(1) {
        /* Generate and send some CAN messages */
        GenerateMPPT1OutputPower( &msg, 115.2, 4 );
        SendCANMsg( &msg );
        Delay(100);
        GenerateMPPT1Temperature( &msg, 27.6, 25.3 );
        SendCANMsg( &msg );
        Delay(100);

        /* You could add others here ... */
    }

    return(0);
}
