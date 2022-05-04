/*
 * CANMsg.h
 *
 *  Created on: 10 Sep 2021
 *      Author: ezzpe
 */

#ifndef CANMSG_H_
#define CANMSG_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



/**
 * Datastructure to hold CAN msg */

typedef struct _CANMsg {
    uint32_t    ID;             /**< CAN Msg ID (see spreadsheet) */
    uint8_t     payload[8];     /**< 8 byte payload */
} CANMsg;



/** Write value to CAN payload.
 *
 * @param msgData is the CAN payload as an array of 8 bytes
 * @param byteOffset is the offset into message to write value (bytes)
 * @param value is ptr to the value to write
 * @param value is sizeof(value)
 *
 * @return 1 if successful; 0 if offset was invalid
 */
uint8_t WriteToCANMsg( uint8_t *msgData, uint8_t byteOffset, void *value, size_t size );


/** Read value from CAN payload.
 *
 * @param msgData is the CAN payload as an array of 8 bytes
 * @param byteOffset is the offset into message to read value (bytes)
 * @param value is ptr to read into
 * @param value is sizeof(value)
 *
 * @return 1 if successful; 0 if offset was invalid
 */
uint8_t ReadFromCANMsg( uint8_t *msgData, uint8_t byteOffset, void *value, size_t size ) ;

/** Interpret an array of 8 values as binary numbers and encode as bits in a byte.
 *
 * @param byte is a ptr to the byte where bits are set
 * @param b is an array of values to interpret as binary numbers
 */
void WriteByteBitField( uint8_t *byte, uint8_t *b );

/** Read bits in a byte and convert to array of integers.
 *
 * @param byte is a ptr to the byte to extract bits from
 * @param b is an array of bytes to be set as 1/0 according to bit values in byte.
 */
void ReadByteBitField( uint8_t *byte, uint8_t *b );



/*
 * MPPT1 Current and Voltage data
 *void GenerateMPPT1OutputPowerMessage( CANMsg *msg, float Vout, float Iout );
*/

//--------------------------------------------------------------------------------------------------------------------------------
//ECU Functions

void GenerateECUSwitchStatus( CANMsg *msg, uint8_t LeftIndicatorsSwitch, uint8_t RightIndicatorsSwitch, uint8_t FrontandRearLightsSwitch, uint8_t BrakeLightsSwitch, uint8_t ReverseLightsSwitch, uint8_t DaylightLightsSwitch, uint8_t CameraSwitch, uint8_t DashSwitch, uint8_t ECUSwitch, uint8_t MPPTSwitch, uint8_t TritiumSwitch, uint8_t ChargerSwitch);

//--------------------------------------------------------------------------------------------------------------------------------
//InverterControl Functions

void GenerateInverterControlMotorDriveCommand( CANMsg *msg, float MotorCurrent, float MotorSpeedLimit);

void GenerateInverterControlMotorPowerCommand( CANMsg *msg, float DCBusCurrentLimit);

//--------------------------------------------------------------------------------------------------------------------------------
//TRITL Functions

void GenerateTRITLIDInfo( CANMsg *msg, uint32_t SerialNumber, uint32_t TritiumID);

void GenerateTRITLStatusInfo( CANMsg *msg, uint8_t ReceiveErrorCounts, uint8_t TransmitErrorCounts, uint16_t ActiveMotor, uint8_t ErrorFlag, uint8_t LimitFlag);

void GenerateTRITLBusMeasurement( CANMsg *msg, int32_t Current, int32_t Voltage);

void GenerateTRITLVelocityMeasurement( CANMsg *msg, int32_t Vehicle, int32_t Motor);

void GenerateTRITLPhaseCurrentMeasurement( CANMsg *msg, int32_t C, int32_t B);

void GenerateTRITLMotorVoltageVector( CANMsg *msg, int32_t Vd, int32_t Vq);

void GenerateTRITLMotorCurrentVector( CANMsg *msg, int32_t Id, int32_t Iq);

void GenerateTRITLMotorBEMF( CANMsg *msg, uint32_t Fd, int32_t Fq);

void GenerateTRITLFifteenVoltsSuply( CANMsg *msg, int32_t Rail, int32_t Reserved);

void GenerateTRITLLowVoltsSupply( CANMsg *msg, int32_t ThreeThreeVolts, int32_t OneNineVolts);

void GenerateTRITLSinkMotorTemp( CANMsg *msg, int32_t Sink, int32_t Motor);

void GenerateTRITLDSPBoardTemp( CANMsg *msg, int32_t DSPBoardTemp);

void GenerateTRITLOdometerBusAh( CANMsg *msg, uint32_t Odometer, uint32_t BusAh);

//--------------------------------------------------------------------------------------------------------------------------------
//MPPT1 Functions

void GenerateMPPT1InputPower( CANMsg *msg, float Vin, float Iin );

void GenerateMPPT1OutputPower( CANMsg *msg, float Vout, float Iout );

void GenerateMPPT1Temperature( CANMsg *msg, float MOSFETTemperature, float ControllerTemperature );

void GenerateMPPT1AuxPowerSupply( CANMsg *msg, float TwelveVoltsPS, float ThreeVoltsPS );

void GenerateMPPT1Limits( CANMsg *msg, float MaxPossibleVoltageOutput, float MaxPossibleCurrentOutput);

void GenerateMPPT1Status( CANMsg *msg, uint8_t CANRXErrorCounter, uint8_t CANTXErrorCounter, uint8_t CANTXOverflowCounter, uint8_t ErrorFlags, uint8_t LimitFlags, uint8_t Mode, uint8_t Reserved, uint8_t TestCounter);

void GenerateMPPT1PowerConnector( CANMsg *msg, float OutputVoltage, float Temperature);

void GenerateMPPT1Mode( CANMsg *msg, uint8_t Mode);

void GenerateMPPT1MaxOutputVoltage( CANMsg *msg, float MaxOutputVoltage);

void GenerateMPPT1MaxOutputCurrent( CANMsg *msg, float MaxOutputCurrent);

//---------------------------------------------------------------------------------------------------------------------------------
//BMS Functions

void GenerateBMUHeartbeatAndSerialNumber( CANMsg *msg, uint32_t DeviceID, uint32_t DeviceSerialNumber );

void GenerateCMUIdentfier( CANMsg *msg, int CMUNumber, uint32_t CMUSerialNumberValue, int16_t PCBTemperatureValue, int16_t CellTemperatureValue );

void GenerateCMUCellVoltage_ztth( CANMsg *msg, int CMUNumber, int16_t CellVoltageAValue, int16_t CellVoltageBValue, int16_t CellVoltageCValue, int16_t CellVoltageDValue);

void GenerateCMUCellVoltage_fts( CANMsg *msg, int CMUNumber, int16_t CellVoltageEValue, int16_t CellVoltageFValue, int16_t CellVoltageGValue, int16_t CellVoltageHValue);

void GeneratePackStateOfCharge( CANMsg *msg, float SOC, float BalanceSOC );

void GeneratePackBalanceStateOfCharge( CANMsg *msg, float BalanceSOCPercentage, float BalanceSOCMismatch );

void GenerateChargerControlInformation( CANMsg *msg, int16_t ChargingCellVoltageError, int16_t CellTeperatureMargin, int16_t DischargingCellVoltageError, int16_t TotalPackCapacity);

void GenerateMinMaxCellVoltage( CANMsg *msg, uint16_t MaxCellVoltage, uint16_t MinCellVoltage, uint8_t MaxCMUNumber, uint8_t MinCMUNumber, uint8_t MaxCMUCellNumber, uint8_t MinCMUCellNumber);

void GenerateMinMaxCellTemperature( CANMsg *msg, uint16_t MaxCellTemperature, uint16_t MinCellTemperature, uint8_t MaxCMUNumber, uint8_t MinCMUNumber);

void GenerateBatteryVoltageCurrent( CANMsg *msg, uint32_t BatteryVoltage, int32_t BatteryCurrent );

void GenerateBatteryPackStatus( CANMsg *msg, uint16_t RisingBalanceVoltageThreshold, uint16_t FallingBalanceVoltageThreshold, uint8_t BMUCMUCount, uint16_t BMSBMUFirmwareBuildNumber );

void GenerateBatteryPackFanStatus( CANMsg *msg, uint16_t FanSpeedZero, uint16_t FanSpeedOne, uint16_t FanCurrentConsumption, uint16_t CMUCurrentConsumption );

void GenerateExtendedBatteryPackStatus( CANMsg *msg, uint8_t BMUHardwareVersion, uint8_t BMUModelID);

#endif /* CANMSG_H_ */
