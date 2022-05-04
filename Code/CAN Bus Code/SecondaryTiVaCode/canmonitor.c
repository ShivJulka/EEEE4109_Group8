

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_can.h"
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/can.h"
#include "driverlib/i2c.h"
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



/* Global variables used for receiving a CAN message */
/* A CAN message object */
tCANMsgObject sCANMessage;
/* 8 bytes to store incoming message */
uint8_t pui8MsgData[8];
/* A counter that keeps track of the number of times the RX interrupt has
 * occurred, which should match the number of messages that were received. */
volatile uint32_t g_ui32MsgCount = 0;
/* A flag for the interrupt handler to indicate that a message was received. */
volatile bool g_bRXFlag = 0;
/* A flag to indicate that some reception error occurred. */
volatile bool g_bErrFlag = 0;


#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


/* Delay for x milliseconds */
void Delay( uint32_t ui32mSeconds ) {
    SysCtlDelay( ui32mSeconds*16000 / 3 );
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

        /* Set flag to indicate received message is pending. */
        g_bRXFlag = 1;

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

    /* Initialize a message object to be used for receiving CAN messages with
     * any CAN ID.  In order to receive any CAN ID, the ID and mask must both
     * be set to 0, and the ID filter enabled. */
    sCANMessage.ui32MsgID = 0;
    sCANMessage.ui32MsgIDMask = 0;
    sCANMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    sCANMessage.ui32MsgLen = 8;

    /* Now load the message object into the CAN peripheral.  Once loaded the
     * CAN will receive any message on the bus, and an interrupt will occur.
     * Use message object 1 for receiving messages (this is not the same as
     * the CAN ID which can be any value in this example). */
    CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_RX);
}

/* Prints message ID and payload as byte sequence - use functions from other project to decode payload if necessary */
//void PrintCANMsg( uint32_t id, uint8_t *d ) {
//    UARTprintf( "0x%03x    0x%2x 0x%02x 0x%02x 0x%02x 0x%2x 0x%02x 0x%02x 0x%02x\n", id, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7] );
//}

/* Prints message ID and payload as byte sequence - use functions from other project to decode payload if necessary */
void PrintCANMsg( uint32_t id, uint8_t *d ) {
    UARTprintf( "0x%03x   %d %d %d %d %d %d %d %d\n", id, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7] );
}
void InitI2C0(void)
{
    //enable I2C module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable GPIO peripheral that contains I2C 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    //I2CSlaveInit(I2C0_BASE, 0x09);

    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

//sends an I2C command to the specified slave
void I2CMasterSend(uint8_t slave_addr, uint8_t num_of_args, ...)
{
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //stores list of variable number of arguments
    va_list vargs;

    //specifies the va_list to "open" and the last fixed argument
    //so vargs knows where to start looking
    va_start(vargs, num_of_args);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));

    //if there is only one argument, we only need to use the
    //single send I2C function
    if(num_of_args == 1)
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //"close" variable argument list
        va_end(vargs);
    }

    //otherwise, we start transmission of multiple bytes on the
    //I2C bus
    else
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //send num_of_args-2 pieces of data, using the
        //BURST_SEND_CONT command of the I2C module

        uint8_t i;

        for(i = 1; i < (num_of_args - 1); i++)
        {
            //put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
            //send next data that was just placed into FIFO
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(I2C0_BASE));
        }

        //put last piece of data into I2C FIFO
        I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
        //send next data that was just placed into FIFO
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //"close" variable args list
        va_end(vargs);
    }
}

//sends an array of data via I2C to the specified slave
void I2CMasterSendString(uint32_t slave_addr, char *d)
{
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C0_BASE, *d);

    //if there is only one argument, we only need to use the
    //single send I2C function
    if(d[1] == '\0')
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));
    }

    //otherwise, we start transmission of multiple bytes on the
    //I2C bus
    else
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //initialize index into array
        uint8_t i = 1;

        //send num_of_args-2 pieces of data, using the
        //BURST_SEND_CONT command of the I2C module
        while(d[i + 1] != '\0')
        {
            //put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C0_BASE, d[i++]);

            //send next data that was just placed into FIFO
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(I2C0_BASE));
        }

        //put last piece of data into I2C FIFO
        I2CMasterDataPut(I2C0_BASE, d[i]);

        //send next data that was just placed into FIFO
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));
    }
}

void I2CSlaveSend(uint32_t id)
{
    I2CSlaveDataPut(I2C0_BASE, id);
}
void I2CSlaveStringSend(char *d)
{
    //initialize index into array
            uint8_t i = 1;

            while(d[i] != '\0')
            {
                //put next piece of data into I2C FIFO
                I2CSlaveDataPut(I2C0_BASE, d[i++]);

            }

}

int main(void) {
    /* Set the clocking to run directly from the external crystal/oscillator. */
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                      SYSCTL_XTAL_16MHZ);

    /* Initialize the UART for output console */
    ConfigureUART();

    /* Initialize the CAN message handler. */
    ConfigureCAN();

    /* Initialize the I2C Peripheral. */
    InitI2C0();

    //char My_Message[] = {'1', '2', '3', '4', '5', '6', '7', '8', '\0'};
    //uint8_t My_Message[10] = {6, 161, 54, 81, 56, 23, 55, 66, 38, 19};

    /* Enter infinite loop to receive messages */
    UARTprintf("Listening for CAN messages...\n");
    while( 1 ) {


        /* If the flag is set, a message is ready for processing */
        if( g_bRXFlag ) {
            /* This just sets the CAN message object data pointer to point to an array
             * for temporary storage of the message data */
            sCANMessage.pui8MsgData = pui8MsgData;

            /* Read the message into the CAN message object */
            CANMessageGet(CAN0_BASE, 1, &sCANMessage, 0);

            /* Clear the message received flag */
            g_bRXFlag = 0;

            /* Check to see if any data was lost */
            if(sCANMessage.ui32Flags & MSG_OBJ_DATA_LOST) {
                UARTprintf("CAN message loss detected\n");
            }

            /* Process message */
            PrintCANMsg( sCANMessage.ui32MsgID, sCANMessage.pui8MsgData );

            if(I2CSlaveStatus(I2C0_BASE)==I2C_SLAVE_ACT_TREQ);
            {
                I2CSlaveSend(sCANMessage.ui32MsgID);
                I2CSlaveStringSend(sCANMessage.pui8MsgData);
            }


            I2CMasterSend(0x08, 1, sCANMessage.ui32MsgID);
            I2CMasterSendString(0x08, sCANMessage.pui8MsgData);



        }
    }
}
