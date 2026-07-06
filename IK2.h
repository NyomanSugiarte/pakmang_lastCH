#ifndef IK2_H
#define IK2_H

#include <cmath>
#include <iostream>
#include <mutex>
// #include "DynamixelSDK/c++/include/dynamixel_sdk/dynamixel_sdk.h"
#include <dynamixel_sdk/dynamixel_sdk.h>
// Port dan baudrate Dynamixel
#define BAUDRATE 1000000          //9600, 57600, 115200, 1,000,000
#define DEVICENAME "/dev/ttyUSB0"
#define PROTOCOL_VERSION 1.0

// Control table address
#define ADDR_TORQUE_ENABLE 24     //xl320:24  1byte             // Control table address is different in Dynamixel model
#define ADDR_GOAL_POSITION 30    //xl320:30  2byte
#define ADDR_PRESENT_POSITION 37 //xl320:37  2byte

// ID Motor
#define ID1 1
#define ID2 2
#define ID3 3

// Torque Enable/Disable
#define TORQUE_ENABLE 1
#define TORQUE_DISABLE 0

// Batas posisi Dynamixel
#define DXL_MIN_POSITION 0
#define DXL_MAX_POSITION 1023
#define DXL_CENTER_POSITION 512
extern dynamixel::PortHandler *portHandler;
extern dynamixel::PacketHandler *packetHandler;
extern dynamixel::GroupSyncWrite groupSyncWrite;

extern std::mutex print_mutex;
extern std::mutex motor_mutex;


const float INPUT_MAX = 1023.0f;
const float CENTER_VALUE = 90.0f;
const float RANGE = 45.0f;

// Deklarasi Konstanta Panjang Link
const double J1L = 25.0;
const double J2L = 50.0;
const double J3L = 75.0;
const double J3_LegAngle = 30;

float mapFloat(float input, float minAngle, float maxAngle, float minValue, float maxValue);
void SetMotorPosition(int id, double angle);
bool InitializeDynamixel();
void ShutdownDynamixel();
void CartesianMove(double X, double Y, double Z, int coxa, int femur, int tibia);
void UpdatePosition(double J1, double J2, double J3, int coxa, int femur, int tibia);
void AddMotorToSyncWrite(int id, double angle);
void ExecuteSyncWrite();


#endif // ROBOT_ARM_H
