#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <algorithm> // Untuk std::max dan std::min
#include "IK2.h"
#include "Gmini_Tr.h"

using namespace std;

Position getSwingPosAdvanced(double x_val, double t, double z_val, double h_step, bool fase_swing, int coxa, int femur, int tibia)
{
    Position p;
    double x_start, x_end;

    // Tentukan arah langkah X dan perilaku Z berdasarkan Fase
    if (fase_swing) {
        x_start = x_val;
        x_end = -x_val;
        p.z = -z_val + (h_step * sin(t * PI)); // Menggunakan -z_val agar di bawah bodi
    } else {
        x_start = -x_val;
        x_end = x_val;
        p.z = -z_val; // Terkunci stabil di lantai saat stance
    }

    p.x = x_start + t * (x_end - x_start);
    
    // Radius lintasan horizontal awal
    double Y_net = sqrt((50.0 * 50.0) - (p.x * p.x));
    
    // Koreksi Y mengikuti perubahan tinggi tibia (Logika andalan Anda)
    double tinggiEngsel = p.z + J3L; // J3L adalah panjang Tibia = 75
    if (tinggiEngsel < J3L) {
        double deltaY = sqrt((J3L * J3L) - (tinggiEngsel * tinggiEngsel));
        p.y = Y_net - (J3L - deltaY);
    } else {
        p.y = Y_net - J3L;
    }

    // Teruskan koordinat hasil kalkulasi ke fungsi IK & Buffer Servo
    CartesianMoveBuffered(p.x, p.y, p.z, coxa, femur, tibia);

    return p;
}

void CartesianMoveBuffered(double X, double Y, double Z, int coxa, int femur, int tibia)
{
    if (fabs(Y) < 0.0001) Y = 0.0001;
    
    double J1 = atan2(Y, X) * 180.0 / PI;
    double Hb = sqrt(X * X + Y * Y);
    double H = Hb - J1L;
    double L = sqrt(H * H + Z * Z);
    
    if (L > (J2L + J3L)) return; // Batas mekanis aman
    
    double cosArg = ((J2L * J2L) + (J3L * J3L) - (L * L)) / (2.0 * J2L * J3L);
    cosArg = max(-1.0, min(1.0, cosArg));
    double J3 = acos(cosArg) * 180.0 / PI;
    J3 = -J3; // Negatif karena orientasi fisik servo
    
    cosArg = ((L * L) + (J2L * J2L) - (J3L * J3L)) / (2.0 * L * J2L);
    cosArg = max(-1.0, min(1.0, cosArg));
    double B = acos(cosArg) * 180.0 / PI;
    
    double A = atan2(Z, H) * 180.0 / PI;
    double J2 = B + A;
    
    // Konversi Sudut ke Derajat Sesuai Orientasi Fisik Servo AX-12A Anda
    double theta1 = 180.0 - (90.0 - J1);
    double theta2 = 180.0 - J2;
    double theta3 = 10.0 - J3;
    cout << "[DEBUG] Servo ID: " << coxa << ", Theta1: " << theta1 << ", Theta2: " << theta2 << ", Theta3: " << theta3 << endl;
    
    // Simpan data posisi ke buffer SyncWrite
    AddMotorToSyncWrite(coxa, theta1);
    AddMotorToSyncWrite(femur, theta2);
    AddMotorToSyncWrite(tibia, theta3);
}