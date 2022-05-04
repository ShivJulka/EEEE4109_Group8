/*
 * CANmsg.c
 *
 *  Example function for creating one of Solar Car CAN packets
 *
 *  You could add functions to generate other packets as needed...
 *  Created on: 10 Sep 2021
 *      Author: ezzpe
 */


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "CANMsg.h"

//--------------------------------------------------------------------------------------------------------------------------------
//ECU Function Definitions

void GenerateECUSwitchStatus( CANMsg *msg, uint8_t LeftIndicatorsSwitch, uint8_t RightIndicatorsSwitch, uint8_t FrontandRearLightsSwitch, uint8_t BrakeLightsSwitch, uint8_t ReverseLightsSwitch, uint8_t DaylightLightsSwitch, uint8_t CameraSwitch, uint8_t DashSwitch, uint8_t ECUSwitch, uint8_t MPPTSwitch, uint8_t TritiumSwitch, uint8_t ChargerSwitch) {

    msg->ID = 0x118;
    WriteToCANMsg( msg->payload, 0, &LeftIndicatorsSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 0, &RightIndicatorsSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 0, &FrontandRearLightsSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 0, &BrakeLightsSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 0, &ReverseLightsSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 0, &DaylightLightsSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 0, &CameraSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 0, &DashSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 1, &ECUSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 1, &MPPTSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 1, &TritiumSwitch, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 1, &ChargerSwitch, sizeof(uint8_t) );
}

//--------------------------------------------------------------------------------------------------------------------------------
//InverterControl Function Definitions

void GenerateInverterControlMotorDriveCommand( CANMsg *msg, float MotorCurrent, float MotorSpeedLimit) {

    msg->ID = 0x501;
    WriteToCANMsg( msg->payload, 4, &MotorCurrent, sizeof(float) );
    WriteToCANMsg( msg->payload, 0, &MotorSpeedLimit, sizeof(float) );
}

void GenerateInverterControlMotorPowerCommand( CANMsg *msg, float DCBusCurrentLimit) {

    msg->ID = 0x502;
    WriteToCANMsg( msg->payload, 4, &DCBusCurrentLimit, sizeof(float) );
}
//--------------------------------------------------------------------------------------------------------------------------------
//TRITR Function Definitions

//--------------------------------------------------------------------------------------------------------------------------------
//TRITL Function Definitions

void GenerateTRITLIDInfo( CANMsg *msg, uint32_t SerialNumber, uint32_t TritiumID) {

    msg->ID = 0x420;
    WriteToCANMsg( msg->payload, 4, &SerialNumber, sizeof(uint32_t) );
    WriteToCANMsg( msg->payload, 0, &TritiumID, sizeof(uint32_t) );
}

void GenerateTRITLStatusInfo( CANMsg *msg, uint8_t ReceiveErrorCounts, uint8_t TransmitErrorCounts, uint16_t ActiveMotor, uint8_t ErrorFlag, uint8_t LimitFlag) {

    msg->ID = 0x421;
    WriteToCANMsg( msg->payload, 7, &ReceiveErrorCounts, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 6, &TransmitErrorCounts, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 4, &ActiveMotor, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 3, &ErrorFlag, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 2, &ErrorFlag, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 0, &LimitFlag, sizeof(uint8_t) );
}

void GenerateTRITLBusMeasurement( CANMsg *msg, int32_t Current, int32_t Voltage) {

    msg->ID = 0x422;
    WriteToCANMsg( msg->payload, 4, &Current, sizeof(int32_t) );
    WriteToCANMsg( msg->payload, 0, &Voltage, sizeof(int32_t) );
}

void GenerateTRITLVelocityMeasurement( CANMsg *msg, int32_t Vehicle, int32_t Motor) {

    msg->ID = 0x423;
    WriteToCANMsg( msg->payload, 4, &Vehicle, sizeof(int32_t) );
    WriteToCANMsg( msg->payload, 0, &Motor, sizeof(int32_t) );
}

void GenerateTRITLPhaseCurrentMeasurement( CANMsg *msg, int32_t C, int32_t B) {

    msg->ID = 0x424;
    WriteToCANMsg( msg->payload, 4, &C, sizeof(int32_t) );
    WriteToCANMsg( msg->payload, 0, &B, sizeof(int32_t) );
}

void GenerateTRITLMotorVoltageVector( CANMsg *msg, int32_t Vd, int32_t Vq) {

    msg->ID = 0x425;
    WriteToCANMsg( msg->payload, 4, &Vd, sizeof(int32_t) );
    WriteToCANMsg( msg->payload, 0, &Vq, sizeof(int32_t) );
}

void GenerateTRITLMotorCurrentVector( CANMsg *msg, int32_t Id, int32_t Iq) {

    msg->ID = 0x426;
    WriteToCANMsg( msg->payload, 4, &Id, sizeof(int32_t) );
    WriteToCANMsg( msg->payload, 0, &Iq, sizeof(int32_t) );
}

void GenerateTRITLMotorBEMF( CANMsg *msg, uint32_t Fd, int32_t Fq) {

    msg->ID = 0x427;
    WriteToCANMsg( msg->payload, 4, &Fd, sizeof(uint32_t) );
    WriteToCANMsg( msg->payload, 0, &Fq, sizeof(int32_t) );
}

void GenerateTRITLFifteenVoltsSuply( CANMsg *msg, int32_t Rail, int32_t Reserved) {

    msg->ID = 0x428;
    WriteToCANMsg( msg->payload, 4, &Rail, sizeof(int32_t) );
    WriteToCANMsg( msg->payload, 0, &Reserved, sizeof(int32_t) );
}

void GenerateTRITLLowVoltsSupply( CANMsg *msg, int32_t ThreeThreeVolts, int32_t OneNineVolts) {

    msg->ID = 0x429;
    WriteToCANMsg( msg->payload, 4, &ThreeThreeVolts, sizeof(int32_t) );
    WriteToCANMsg( msg->payload, 0, &OneNineVolts, sizeof(int32_t) );
}

void GenerateTRITLSinkMotorTemp( CANMsg *msg, int32_t Sink, int32_t Motor) {

    msg->ID = 0x42B;
    WriteToCANMsg( msg->payload, 4, &Sink, sizeof(int32_t) );
    WriteToCANMsg( msg->payload, 0, &Motor, sizeof(int32_t) );
}

void GenerateTRITLDSPBoardTemp( CANMsg *msg, int32_t DSPBoardTemp) {

    msg->ID = 0x42C;
    WriteToCANMsg( msg->payload, 0, &DSPBoardTemp, sizeof(int32_t) );

}

void GenerateTRITLOdometerBusAh( CANMsg *msg, uint32_t Odometer, uint32_t BusAh) {

    msg->ID = 0x42E;
    WriteToCANMsg( msg->payload, 4, &Odometer, sizeof(uint32_t) );
    WriteToCANMsg( msg->payload, 0, &BusAh, sizeof(uint32_t) );
}
//--------------------------------------------------------------------------------------------------------------------------------
//MPPT1 Function Definitions

void GenerateMPPT1InputPower( CANMsg *msg, float Vin, float Iin ) {

    msg->ID = 0x6A0;
    WriteToCANMsg( msg->payload, 0, &Vin, sizeof(float) );
    WriteToCANMsg( msg->payload, 4, &Iin, sizeof(float) );
}

void GenerateMPPT1OutputPower( CANMsg *msg, float Vout, float Iout ) {
    /* Lookup MPPT 1 power output message definition in spreadsheet:
     * ID = 0x6A1
     * Output voltage = float @ offset 0
     * Output current = float @ offset 4
     */

    msg->ID = 0x6A1;
    WriteToCANMsg( msg->payload, 0, &Vout, sizeof(float) );
    WriteToCANMsg( msg->payload, 4, &Iout, sizeof(float) );
}

void GenerateMPPT1Temperature( CANMsg *msg, float MOSFETTemperature, float ControllerTemperature ) {

    msg->ID = 0x6A2;
    WriteToCANMsg( msg->payload, 0, &MOSFETTemperature, sizeof(float) );
    WriteToCANMsg( msg->payload, 4, &ControllerTemperature, sizeof(float) );
}

void GenerateMPPT1AuxPowerSupply( CANMsg *msg, float TwelveVoltsPS, float ThreeVoltsPS ) {

    msg->ID = 0x6A3;
    WriteToCANMsg( msg->payload, 0, &TwelveVoltsPS, sizeof(float) );
    WriteToCANMsg( msg->payload, 4, &ThreeVoltsPS, sizeof(float) );
}

void GenerateMPPT1Limits( CANMsg *msg, float MaxPossibleVoltageOutput, float MaxPossibleCurrentOutput) {

    msg->ID = 0x6A4;
    WriteToCANMsg( msg->payload, 0, &MaxPossibleVoltageOutput, sizeof(float) );
    WriteToCANMsg( msg->payload, 4, &MaxPossibleCurrentOutput, sizeof(float) );
}

void GenerateMPPT1Status( CANMsg *msg, uint8_t CANRXErrorCounter, uint8_t CANTXErrorCounter, uint8_t CANTXOverflowCounter, uint8_t ErrorFlags, uint8_t LimitFlags, uint8_t Mode, uint8_t Reserved, uint8_t TestCounter) {

    msg->ID = 0x6A5;
    WriteToCANMsg( msg->payload, 0, &CANRXErrorCounter, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 1, &CANTXErrorCounter, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 2, &CANTXOverflowCounter, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 3, &ErrorFlags, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 4, &LimitFlags, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 5, &Mode, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 6, &Reserved, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 7, &TestCounter, sizeof(uint8_t) );

}

void GenerateMPPT1PowerConnector( CANMsg *msg, float OutputVoltage, float Temperature) {

    msg->ID = 0x6A6;
    WriteToCANMsg( msg->payload, 0, &OutputVoltage, sizeof(float) );
    WriteToCANMsg( msg->payload, 4, &Temperature, sizeof(float) );
}

void GenerateMPPT1Mode( CANMsg *msg, uint8_t Mode) {

    msg->ID = 0x6A8;
    WriteToCANMsg( msg->payload, 0, &Mode, sizeof(uint8_t) );S
}

void GenerateMPPT1MaxOutputVoltage( CANMsg *msg, float MaxOutputVoltage) {

    msg->ID = 0x6B0;
    WriteToCANMsg( msg->payload, 1, &MaxOutputVoltage, sizeof(float) );
}

void GenerateMPPT1MaxOutputCurrent( CANMsg *msg, float MaxOutputCurrent) {

    msg->ID = 0x6B1;
    WriteToCANMsg( msg->payload, 1, &MaxOutputCurrent, sizeof(float) );
}


//--------------------------------------------------------------------------------------------------------------------------------
//BMS Function Definitions

void GenerateBMUHeartbeatAndSerialNumber( CANMsg *msg, uint32_t DeviceID, uint32_t DeviceSerialNumber ) {

    msg->ID = 0x600;
    WriteToCANMsg( msg->payload, 0, &DeviceID, sizeof(uint32_t) );
    WriteToCANMsg( msg->payload, 4, &DeviceSerialNumber, sizeof(uint32_t) );
}

void GenerateCMUIdentfier( CANMsg *msg, int CMUNumber, uint32_t CMUSerialNumberValue, int16_t PCBTemperatureValue, int16_t CellTemperatureValue ) {

    int CMUIDMatcher[4] = {0x601, 0x604, 0x607, 0x610};
    msg->ID = CMUIDMatcher[CMUNumber - 1];
    uint32_t CMUSerialNumber[4];
    CMUSerialNumber[CMUNumber-1] = CMUSerialNumberValue;
    int16_t PCBTemperature[4];
    PCBTemperature[CMUNumber - 1] = PCBTemperatureValue;
    int16_t CellTemperature[4];
    CellTemperature[CMUNumber-1] = CellTemperatureValue;
    WriteToCANMsg( msg->payload, 0, &CMUSerialNumber[CMUNumber-1], sizeof(uint32_t) );
    WriteToCANMsg( msg->payload, 4, &PCBTemperature[CMUNumber-1], sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 6, &CellTemperature[CMUNumber-1], sizeof(int16_t) );
}

void GenerateCMUCellVoltage_ztth( CANMsg *msg, int CMUNumber, int16_t CellVoltageAValue, int16_t CellVoltageBValue, int16_t CellVoltageCValue, int16_t CellVoltageDValue) {


    int CMUIDMatcher[4] = {0x602, 0x605, 0x608, 0x611};
    msg->ID = CMUIDMatcher[CMUNumber - 1];
    int16_t CellVoltageA[4];
    CellVoltageA[CMUNumber-1] = CellVoltageAValue;
    int16_t CellVoltageB[4];
    CellVoltageB[CMUNumber-1] = CellVoltageBValue;
    int16_t CellVoltageC[4];
    CellVoltageC[CMUNumber-1] = CellVoltageCValue;
    int16_t CellVoltageD[4];
    CellVoltageD[CMUNumber-1] = CellVoltageDValue;
    WriteToCANMsg( msg->payload, 0, &CellVoltageA[CMUNumber-1], sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 2, &CellVoltageB[CMUNumber-1], sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 4, &CellVoltageC[CMUNumber-1], sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 6, &CellVoltageD[CMUNumber-1], sizeof(int16_t) );
}

void GenerateCMUCellVoltage_fts( CANMsg *msg, int CMUNumber, int16_t CellVoltageEValue, int16_t CellVoltageFValue, int16_t CellVoltageGValue, int16_t CellVoltageHValue) {


    int CMUIDMatcher[4] = {0x603, 0x606, 0x609, 0x612};
    msg->ID = CMUIDMatcher[CMUNumber - 1];
    int16_t CellVoltageE[4];
    CellVoltageE[CMUNumber-1] = CellVoltageEValue;
    int16_t CellVoltageF[4];
    CellVoltageF[CMUNumber-1] = CellVoltageFValue;
    int16_t CellVoltageG[4];
    CellVoltageG[CMUNumber-1] = CellVoltageGValue;
    int16_t CellVoltageH[4];
    CellVoltageH[CMUNumber-1] = CellVoltageHValue;
    WriteToCANMsg( msg->payload, 0, &CellVoltageE[CMUNumber-1], sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 2, &CellVoltageF[CMUNumber-1], sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 4, &CellVoltageG[CMUNumber-1], sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 6, &CellVoltageH[CMUNumber-1], sizeof(int16_t) );
}

void GeneratePackStateOfCharge( CANMsg *msg, float SOC, float BalanceSOC ) {

    msg->ID = 0x6F4;
    WriteToCANMsg( msg->payload, 0, &SOC, sizeof(float) );
    WriteToCANMsg( msg->payload, 4, &BalanceSOC, sizeof(float) );

}

void GeneratePackBalanceStateOfCharge( CANMsg *msg, float BalanceSOCPercentage, float BalanceSOCMismatch ) {

    msg->ID = 0x6F5;
    WriteToCANMsg( msg->payload, 0, &BalanceSOCPercentage, sizeof(float) );
    WriteToCANMsg( msg->payload, 4, &BalanceSOCMismatch, sizeof(float) );

}

void GenerateChargerControlInformation( CANMsg *msg, int16_t ChargingCellVoltageError, int16_t CellTeperatureMargin, int16_t DischargingCellVoltageError, int16_t TotalPackCapacity) {

    msg->ID = 0x6F6;
    WriteToCANMsg( msg->payload, 0, &ChargingCellVoltageError, sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 2, &CellTeperatureMargin, sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 4, &DischargingCellVoltageError, sizeof(int16_t) );
    WriteToCANMsg( msg->payload, 6, &TotalPackCapacity, sizeof(int16_t) );

}

void GenerateMinMaxCellVoltage( CANMsg *msg, uint16_t MaxCellVoltage, uint16_t MinCellVoltage, uint8_t MaxCMUNumber, uint8_t MinCMUNumber, uint8_t MaxCMUCellNumber, uint8_t MinCMUCellNumber) {

    msg->ID = 0x6F8;
    WriteToCANMsg( msg->payload, 0, &MaxCellVoltage, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 2, &MinCellVoltage, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 4, &MaxCMUNumber, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 5, &MinCMUNumber, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 6, &MaxCMUCellNumber, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 7, &MinCMUCellNumber, sizeof(uint8_t) );

}

void GenerateMinMaxCellTemperature( CANMsg *msg, uint16_t MaxCellTemperature, uint16_t MinCellTemperature, uint8_t MaxCMUNumber, uint8_t MinCMUNumber) {

    msg->ID = 0x6F9;
    WriteToCANMsg( msg->payload, 0, &MaxCellTemperature, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 2, &MinCellTemperature, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 4, &MaxCMUNumber, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 6, &MinCMUNumber, sizeof(uint8_t) );


}

void GenerateBatteryVoltageCurrent( CANMsg *msg, uint32_t BatteryVoltage, int32_t BatteryCurrent ) {

    msg->ID = 0x6FA;
    WriteToCANMsg( msg->payload, 0, &BatteryVoltage, sizeof(uint32_t) );
    WriteToCANMsg( msg->payload, 4, &BatteryCurrent, sizeof(int32_t) );

}

void GenerateBatteryPackStatus( CANMsg *msg, uint16_t RisingBalanceVoltageThreshold, uint16_t FallingBalanceVoltageThreshold, uint8_t BMUCMUCount, uint16_t BMSBMUFirmwareBuildNumber ) {

    msg->ID = 0x6FB;
    WriteToCANMsg( msg->payload, 0, &RisingBalanceVoltageThreshold, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 2, &FallingBalanceVoltageThreshold, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 5, &BMUCMUCount, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 6, &BMSBMUFirmwareBuildNumber, sizeof(uint16_t) );

}

void GenerateBatteryPackFanStatus( CANMsg *msg, uint16_t FanSpeedZero, uint16_t FanSpeedOne, uint16_t FanCurrentConsumption, uint16_t CMUCurrentConsumption ) {

    msg->ID = 0x6FC;
    WriteToCANMsg( msg->payload, 0, &FanSpeedZero, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 2, &FanSpeedOne, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 4, &FanCurrentConsumption, sizeof(uint16_t) );
    WriteToCANMsg( msg->payload, 6, &CMUCurrentConsumption, sizeof(uint16_t) );

}

void GenerateExtendedBatteryPackStatus( CANMsg *msg, uint8_t BMUHardwareVersion, uint8_t BMUModelID) {

    msg->ID = 0x6FD;
    WriteToCANMsg( msg->payload, 4, &BMUHardwareVersion, sizeof(uint8_t) );
    WriteToCANMsg( msg->payload, 5, &BMUModelID, sizeof(uint8_t) );


}

//--------------------------------------------------------------------------------------------------------------------------------
//MPPT2 Function Definitions

//--------------------------------------------------------------------------------------------------------------------------------
//MPPT3 Function Definitions
