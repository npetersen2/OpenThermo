/******************************************************************************
SparkFunTMP117.h
SparkFunTMP117 Library Header File
Madison Chodikov @ SparkFun Electronics
Original Creation Date: April 29, 2016
https://github.com/sparkfunX/Qwiic_TMP117

This file prototypes the TMP117 class, implemented in SparkFunTMP117.cpp.

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno
	TMP117 Breakout Version: 1.0.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef DRV_TMP117_H
#define DRV_TMP117_H

#include "drv_tmp117_registers.h"
#include <stdint.h>
#include <stdbool.h>

#define DEVICE_ID_VALUE 0x0117			// Value found in the device ID register on reset (page 24 Table 3 of datasheet)
#define TMP117_RESOLUTION 0.0078125f	// Resolution of the device, found on (page 1 of datasheet)
#define CONTINUOUS_CONVERSION_MODE 0b00 // Continuous Conversion Mode
#define ONE_SHOT_MODE 0b11				// One Shot Conversion Mode
#define SHUTDOWN_MODE 0b01				// Shutdown Conversion Mode

// Configuration register found on page 25 Figure 26 and Table 6
typedef union {
	struct
	{
		uint8_t EMPTY : 1;			// Empty bit in register
		uint8_t TMP_SOFT_RESET : 1; // Software reset bit
		uint8_t DR_ALERT : 1;		// ALERT pin select bit
		uint8_t POL : 1;			// ALERT pin polarity bit
		uint8_t T_NA : 1;			// Therm/alert mode select
		uint8_t AVG : 2;			// Conversion averaging modes
		uint8_t CONV : 3;			// Conversion cycle bit
		uint8_t MOD : 2;			// Set conversion mode
		uint8_t EEPROM_BUSY : 1;	// EEPROM busy flag
		uint8_t DATA_READY : 1;		// Data ready flag
		uint8_t LOW_ALERT : 1;		// Low Alert flag
		uint8_t HIGH_ALERT : 1;		// High Alert flag
	} CONFIGURATION_FIELDS;
	uint8_t CONFIGURATION_COMBINED;
} CONFIGURATION_REG;

// Device ID Register used for checking if the device ID is the same as declared
// This register is found on Page 30 of the datasheet in Table 15 and Figure 34
typedef union {
	struct
	{
		uint16_t DID : 12; // Indicates the device ID
		uint8_t REV : 4;   // Indicates the revision number
	} DEVICE_ID_FIELDS;
	uint16_t DEVICE_ID_COMBINED;
} DEVICE_ID_REG;


void drv_tmp117_init(void); 									// Constructor
bool drv_tmp117_begin(uint8_t sensorAddress);					// Checks for ACK over I2C, and sets the device ID of the TMP and chooses the wire port
uint8_t drv_tmp117_getAddress(void);							// Returns the address of the device
float drv_tmp117_readTempC(void);								// Returns the temperature in degrees C
float drv_tmp117_readTempF(void);								// Converts readTempC result to degrees F
void drv_tmp117_softReset(void);								// Performs a software reset on the Configuration Register Field bits
float drv_tmp117_getTemperatureOffset(void);					// Reads the temperature offset
void drv_tmp117_setTemperatureOffset(float offset);				// Writes to the temperature offset
float drv_tmp117_getLowLimit(void);								// Returns the low limit register
void drv_tmp117_setLowLimit(float lowLimit);					// Sets the low limit temperature for the low limit register
float drv_tmp117_getHighLimit(void);							// Returns the high limit register
void drv_tmp117_setHighLimit(float highLimit);					// Sets the low limit temperature for the low limit register
uint16_t drv_tmp117_getConfigurationRegister(void);				// Get Configuration Register
uint8_t drv_tmp117_getHighLowAlert(void);						// Reads in Alert mode for high and low alert flags
bool drv_tmp117_getHighAlert(void);								// Reads in Alert mode for a high alert flag
bool drv_tmp117_getLowAlert(void);								// Reads in Alert mode for a low alert flag
void drv_tmp117_setAlertFunctionMode(uint8_t setAlertMode);		// Set alert or therm mode
uint8_t drv_tmp117_getAlertFunctionMode(void);					// Check to see if in alert or therm mode
uint8_t drv_tmp117_getConversionMode(void);						// Checks to see the Conversion Mode the device is currently in
void drv_tmp117_setContinuousConversionMode(void);				// Sets the Conversion Mode of the Device to be Continuous
void drv_tmp117_setOneShotMode(void);							// Sets the Conversion Mode of the Device to be One Shot
void drv_tmp117_setShutdownMode(void);							// Sets the Conversion Mode of the Device to be Shutdown
void drv_tmp117_setConversionAverageMode(uint8_t convMode);		// Sets the conversion averaging mode
uint8_t drv_tmp117_getConversionAverageMode(void);				// Returns the Conversion Averaging Mode
void drv_tmp117_setConversionCycleBit(uint8_t convTime);		// Sets the conversion cycle time bit
uint8_t drv_tmp117_getConversionCycleBit(void);					// Returns the conversion cycle time bit value
bool drv_tmp117_dataReady(void);								// Checks to see if there is data ready from the device


#endif // DRV_TMP117_H
