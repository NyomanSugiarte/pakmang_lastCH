#ifndef GMINI_TR_H
#define GMINI_TR_H

#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include "IK2.h"    
static const double PI = 3.14159265358979323846;

struct Position {
    double x;
    double y;
    double z;
};

// Definisikan ID Servo Dynamixel AX-12A untuk 6 Kaki (Coxa, Femur, Tibia)
// Indeks 0 = Leg 1, Indeks 5 = Leg 6
const int AX_ID[6][3] = {
    {1, 2, 3},    // Leg 1 (Kelompok A)
    {4, 5, 6},    // Leg 2 (Kelompok B)
    {7, 8, 9},    // Leg 3 (Kelompok A)
    {10, 11, 12}, // Leg 4 (Kelompok B)
    {13, 14, 15}, // Leg 5 (Kelompok A)
    {16, 17, 18}  // Leg 6 (Kelompok B)
};

// Forward Declaration agar bisa dipanggil di TripodGait
Position getSwingPosAdvanced(double x_val, double t, double z_val, double h_step, bool fase_swing, int coxa, int femur, int tibia);
void CartesianMoveBuffered(double X, double Y, double Z, int coxa, int femur, int tibia);
#endif // GMINI_TR_H