/*
 * eUSCI_I2CDriver.c
 *
 *  Created on: Dec 18, 2017
 *      Author: Russty
 */

#include "eUSCI_I2CDriver.h"


int TransmissionComplete = 0;   //Zero indicates transmission is in progress, One is complete
int TXByteCtr = 0;
char *TXData;



void I2CInit()
{
    P4SEL1 |= BIT0 | BIT1;      // I2C Pins
    P4SEL0 &= ~BIT0 + ~BIT1;
    // Configure USCI_B0 for I2C mode
    UCB1CTLW0 = UCSWRST;                      // put eUSCI_B in reset state
    UCB1CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK; // I2C master mode, SMCLK
    UCB1BRW = 0x8;                            // baudrate = SMCLK / 8
    UCB1CTLW0 &= ~UCSWRST;                    // clear reset register
    UCB1IE |= UCTXIE0;             // transmit and NACK interrupt enable
}


void I2CWriteMultipleBytes(char I2CAddress, int NumOfBytes, char *Data)
{
    TransmissionComplete = 0;
    UCB1TBCNT = 0x02;       // Expecting to transmit 2 bytes of data
    UCB1CTLW1 |= UCASTP_2;  // Sends stop bit when UCTBCNT is reached
    TXByteCtr = NumOfBytes;
    TXData = Data;
    UCB1I2CSA = I2CAddress;     // Set the target slave address
    UCB1CTLW0 |= UCTR | UCTXSTT;    // I2C TX, start condition
    while(!TransmissionComplete);
}





#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B1_VECTOR))) USCI_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCB1IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE:          break;         // Vector 0: No interrupts
    case USCI_I2C_UCALIFG:   break;         // Vector 2: ALIFG
    case USCI_I2C_UCNACKIFG:                // Vector 4: NACKIFG
      //UCB1CTLW0 |= UCTXSTT;                 // resend start if NACK
      //break;
    case USCI_I2C_UCSTTIFG:  break;         // Vector 6: STTIFG
    case USCI_I2C_UCSTPIFG:  break;         // Vector 8: STPIFG
    case USCI_I2C_UCRXIFG3:  break;         // Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3:  break;         // Vector 12: TXIFG3
    case USCI_I2C_UCRXIFG2:  break;         // Vector 14: RXIFG2
    case USCI_I2C_UCTXIFG2:  break;         // Vector 16: TXIFG2
    case USCI_I2C_UCRXIFG1:  break;         // Vector 18: RXIFG1
    case USCI_I2C_UCTXIFG1:  break;         // Vector 20: TXIFG1
    case USCI_I2C_UCRXIFG0:  break;         // Vector 22: RXIFG0
    case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
        if(TXByteCtr)
        {
        UCB1TXBUF = *TXData;                // Load TX buffer
        TXByteCtr--;                        // Decrement TX byte counter
        TXData++;                           // Increment pointer to data
        if(TXByteCtr == 0)
            TransmissionComplete=1;
        }
        //if(TXByteCtr == 0)
        //{
        //    UCB1IFG &= ~UCTXIFG;                // Clear USCI_B0 TX int flag
        //    TransmissionComplete = 1;
        //}
      break;
    default: break;
  }
}
