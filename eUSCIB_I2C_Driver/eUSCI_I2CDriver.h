/*
 * eUSCI_I2CDriver.h
 *
 *  Created on: Dec 18, 2017
 *      Author: Russty
 */

#ifndef EUSCI_I2CDRIVER_H_
#define EUSCI_I2CDRIVER_H_

#include <msp430.h>

void I2CInit();     // This will be adapted in future revisions to allow for multiple I2C peripherals
void I2CWriteMultipleBytes(char I2CAddress, int NumOfBytes, char *Data);


#endif /* EUSCI_I2CDRIVER_H_ */
