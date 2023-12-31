/* 
 * File:   configuration.h
 * Author: hussam_babylon
 *
 * Created on March 1, 2021, 11:25 AM
 */

#include <xc.h>
#include <stdint.h>
#include "configuration.h"
#define _XTAL_FREQ 8000000
uint8_t Data ;
char byteReceived[7];
char byteNumber;

#define Indi RB0
#define Left RB1
#define Right RB2
#define Up RB3
#define Down RB4
//--------------------------------
// Functions Declarations
void UART_RX_Init(void);
// uint8_t UART_Read(void);
// Globals
uint8_t UART_Buffer = 0;
//--------------------------------
// Main Routine
void main(void)
{
  //--[ Peripherals & IO Configurations ]--
  UART_RX_Init(); // Initialize The UART in Master Mode @ 9600bps
  __delay_ms(100);
 // TRISB = 0x00; // Output Port (4-Pins)
 // PORTB = 0x00; // Initially OFF
//  TRISBbits.TRISB1 = 0;
//  TRISBbits.TRISB2 = 0;
  TRISBbits.TRISB0 = 0;
  TRISBbits.TRISB1 = 0;
  TRISBbits.TRISB2 = 0;
  TRISBbits.TRISB3 = 0;
  TRISBbits.TRISB4 = 0;
  //---------------------------
  while(1) 
  {
      
       if (byteNumber > 6)  // process it
    {

       byteNumber = 0;              // ready for next time
       Data = byteReceived[3];  // read the incoming byte:
        switch(Data)
        {
          case 0x00 :Right = 0; Left = 0; Up = 0; Down = 0; break;
          case 0x02 : Right = 1;  break;
          case 0x04 : Left =1;  break;
          case 0x010 : Down = 1;  break;
          case 0x08 : Up = 1;  break;
        }

    }
  }
  return;
}
//--------------------------------
// Functions Definitions
void UART_RX_Init()
{
  BRGH = 0; // Set For High-Speed Baud Rate
  SPBRG = 51; // Set The Baud Rate To Be 2400 bps
  // Enable The Ascynchronous Serial Port
  SYNC = 0;
  SPEN = 1;
  // Set The RX-TX Pins to be in UART mode (not io)
  TRISC6 = 1; // As stated in the datasheet
  TRISC7 = 1; // As stated in the datasheet
  //--[ Enable UART Receiving Interrupts ]--
  RCIE = 1; // UART Receving Interrupt Enable Bit
  PEIE = 1; // Peripherals Interrupt Enable Bit
  GIE = 1; // Global Interrupt Enable Bit
  //------------------
  CREN = 1; // Enable Data Continous Reception
}
void interrupt ISR (void)
{
  if (RCIF == 1)
  {
      Indi = ~Indi;
    byteReceived[byteNumber++] = RCREG; // Read The Received Data Buffer
   // PORTB = UART_Buffer; // Display The Received Data On LEDs
      if (byteReceived[0] != 0xFF)
      {
        byteNumber = 0;
      }
    // Data = UART_Buffer; // Display The Received Data On LEDs
    RCIF = 0; // Clear The Flag
  }
}