#include "Gmini_Tr.h"
#include "IK2.h"
#include <iostream>
#include <cmath>
using namespace std;
void Forward(double speed, double body_height, double step_length, double step_height)
{
    auto start_time = chrono::high_resolution_clock::now();
    
    cout << "Memulai Pergerakan Tripod Gait..." << endl;

    while (true)
    {
        auto now = chrono::high_resolution_clock::now();
        double elapsed_time = chrono::duration<double>(now - start_time).count();
        
        // global_t bergerak dari 0.0 sampai 1.0 berdasarkan speed
        double global_t = fmod(elapsed_time * speed, 1.0);
        
        double t_A, t_B;
        bool swing_A, swing_B;

        // TENTUKAN FASE KELOMPOK A & B
        if (global_t < 0.5) 
        {
            // Setengah siklus pertama: Kelompok A Swing, Kelompok B Stance
            t_A = global_t / 0.5;
            swing_A = true;

            t_B = global_t / 0.5;
            swing_B = false;
        } 
        else 
        {
            // Setengah siklus kedua: Kelompok A Stance, Kelompok B Swing
            t_A = (global_t - 0.5) / 0.5;
            swing_A = false;

            t_B = (global_t - 0.5) / 0.5;
            swing_B = true;
        }

        // --- GERAKKAN KELOMPOK A (Leg 1, Leg 3, Leg 5) ---
        int kaki_A[] = {0, 2, 4}; // Indeks array AX_ID
        for (int idx : kaki_A) {
            if(idx == 4){
                getSwingPosAdvanced(step_length, t_A, body_height, step_height, swing_A, 
                                    AX_ID[idx][0], AX_ID[idx][1], AX_ID[idx][2]);
            } else {
                getSwingPosAdvanced(-step_length, t_A, body_height, step_height, swing_A, 
                                    AX_ID[idx][0], AX_ID[idx][1], AX_ID[idx][2]);
            }
        }

        // --- GERAKKAN KELOMPOK B (Leg 2, Leg 4, Leg 6) ---
        int kaki_B[] = {1, 3, 5}; // Indeks array AX_ID
        for (int idx : kaki_B) {
            if(idx == 1){
                getSwingPosAdvanced(-step_length, t_B, body_height, step_height, swing_B, 
                                    AX_ID[idx][0], AX_ID[idx][1], AX_ID[idx][2]);
            } else {
                getSwingPosAdvanced(step_length, t_B, body_height, step_height, swing_B, 
                                    AX_ID[idx][0], AX_ID[idx][1], AX_ID[idx][2]);
            }
        }

        // KIRIM COMMAND SYNC WRITE SEKALIGUS KE DYNAMIXEL (Sekali per loop 10ms)
        ExecuteSyncWrite(); 

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void Backward(double speed, double body_height, double step_length, double step_height)
{
    auto start_time = chrono::high_resolution_clock::now();
    
    cout << "Memulai Pergerakan Tripod Gait..." << endl;

    while (true)
    {
        auto now = chrono::high_resolution_clock::now();
        double elapsed_time = chrono::duration<double>(now - start_time).count();
        
        // global_t bergerak dari 0.0 sampai 1.0 berdasarkan speed
        double global_t = fmod(elapsed_time * speed, 1.0);
        
        double t_A, t_B;
        bool swing_A, swing_B;

        // TENTUKAN FASE KELOMPOK A & B
        if (global_t < 0.5) 
        {
            // Setengah siklus pertama: Kelompok A Swing, Kelompok B Stance
            t_A = global_t / 0.5;
            swing_A = true;

            t_B = global_t / 0.5;
            swing_B = false;
        } 
        else 
        {
            // Setengah siklus kedua: Kelompok A Stance, Kelompok B Swing
            t_A = (global_t - 0.5) / 0.5;
            swing_A = false;

            t_B = (global_t - 0.5) / 0.5;
            swing_B = true;
        }

        // --- GERAKKAN KELOMPOK A (Leg 1, Leg 3, Leg 5) ---
        int kaki_A[] = {0, 2, 4}; // Indeks array AX_ID
        for (int idx : kaki_A) {
            if(idx == 4){
                getSwingPosAdvanced(-step_length, t_A, body_height, step_height, swing_A, 
                                    AX_ID[idx][0], AX_ID[idx][1], AX_ID[idx][2]);
            } else {
                getSwingPosAdvanced(step_length, t_A, body_height, step_height, swing_A, 
                                    AX_ID[idx][0], AX_ID[idx][1], AX_ID[idx][2]);
            }
        }

        // --- GERAKKAN KELOMPOK B (Leg 2, Leg 4, Leg 6) ---
        int kaki_B[] = {1, 3, 5}; // Indeks array AX_ID
        for (int idx : kaki_B) {
            if(idx == 1){
                getSwingPosAdvanced(step_length, t_B, body_height, step_height, swing_B, 
                                    AX_ID[idx][0], AX_ID[idx][1], AX_ID[idx][2]);
            } else {
                getSwingPosAdvanced(-step_length, t_B, body_height, step_height, swing_B, 
                                    AX_ID[idx][0], AX_ID[idx][1], AX_ID[idx][2]);
            }
        }

        // KIRIM COMMAND SYNC WRITE SEKALIGUS KE DYNAMIXEL (Sekali per loop 10ms)
        ExecuteSyncWrite(); 

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void StandUp(double target_body_height, double duration_fase1, double duration_fase2) {
    cout << "=========================================" << endl;
    cout << "   MEMULAI PROSEDUR BERDIRI ASHIBOT      " << endl;
    cout << "=========================================" << endl;
    
    int loop_rate_ms = 10; // Update servo setiap 10ms
    double current_x = 0.0;

    //==========================================================
    // FASE 1: Dari Posisi Tengah (Default) Menuju Tiarap (-20 mm)
    //==========================================================
    cout << "[FASE 1] Menurunkan bodi ke posisi tiarap..." << endl;
    
    double start_z_fase1 = -target_body_height; // Mulai dari posisi tengah/default bodi
    double target_z_fase1 = -30.0;             // Menekuk ke -20 mm
    int steps_fase1 = (duration_fase1 * 1000) / loop_rate_ms;

    for (int step = 0; step <= steps_fase1; step++) {
        double alpha = (double)step / steps_fase1;
        // Interpolasi dari default ke -20
        double current_z = start_z_fase1 + alpha * (target_z_fase1 - start_z_fase1);
        
        for (int idx = 0; idx < 6; idx++) {
            int coxa  = AX_ID[idx][0];
            int femur = AX_ID[idx][1];
            int tibia = AX_ID[idx][2];
            
            double current_y = sqrt((50.0 * 50.0) - (current_x * current_x));
            
            // Koreksi Y Tibia
            double tinggiEngsel = current_z + J3L;
            if (tinggiEngsel < J3L) {
                double deltaY = sqrt((J3L * J3L) - (tinggiEngsel * tinggiEngsel));
                current_y = current_y - (J3L - deltaY);
            } else {
                current_y = current_y - J3L;
            }
            
            CartesianMoveBuffered(current_x, current_y, current_z, coxa, femur, tibia);
        }
        ExecuteSyncWrite();
        std::this_thread::sleep_for(std::chrono::milliseconds(loop_rate_ms));
    }

    // Jeda sejenak di posisi tiarap (0.5 detik) agar robot stabil
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //==========================================================
    // FASE 2: Dari Tiarap (-20 mm) Mendorong Naik ke Target Tinggi
    //==========================================================
    cout << "[FASE 2] Mendorong bodi naik ke target berdiri tegak..." << endl;
    
    double start_z_fase2 = -30.0;
    double target_z_fase2 = -target_body_height; // Naik ke target (misal -75 mm)
    int steps_fase2 = (duration_fase2 * 1000) / loop_rate_ms;

    for (int step = 0; step <= steps_fase2; step++) {
        double alpha = (double)step / steps_fase2;
        // Interpolasi dari -20 ke target tinggi
        double current_z = start_z_fase2 + alpha * (target_z_fase2 - start_z_fase2);
        
        for (int idx = 0; idx < 6; idx++) {
            int coxa  = AX_ID[idx][0];
            int femur = AX_ID[idx][1];
            int tibia = AX_ID[idx][2];
            
            double current_y = sqrt((50.0 * 50.0) - (current_x * current_x));
            
            // Koreksi Y Tibia
            double tinggiEngsel = current_z + J3L;
            if (tinggiEngsel < J3L) {
                double deltaY = sqrt((J3L * J3L) - (tinggiEngsel * tinggiEngsel));
                current_y = current_y - (J3L - deltaY);
            } else {
                current_y = current_y - J3L;
            }
            
            CartesianMoveBuffered(current_x, current_y, current_z, coxa, femur, tibia);
        }
        ExecuteSyncWrite();
        std::this_thread::sleep_for(std::chrono::milliseconds(loop_rate_ms));
    }
    
    cout << "[INFO] Prosedur Stand Up Selesai. Ashibot Siap!" << endl;
}