#include <iostream>
#include "IK2.h"
#include "Gmini_Tr.h" // Pastikan header trajektori Anda di-include
#include "movement.h"
using namespace std;

int main() {
    cout << "=========================================" << endl;
    cout << "   HEXAPOD TRIPOD GAIT CONTROLLER        " << endl;
    cout << "=========================================" << endl;

    // 1. Inisialisasi Port & Packet Handler Dynamixel
    if (!InitializeDynamixel()) {
        cerr << "[ERROR] Gagal menginisialisasi Dynamixel SDK!" << endl;
        return -1;
    }
    cout << "[INFO] Komunikasi Dynamixel Berhasil Dibuka." << endl;

    // 2. Aktifkan Torsi (Torque On) untuk semua 18 Servo (ID 1 sampai 18)
    cout << "[INFO] Mengaktifkan torsi untuk semua motor servo (ID 1-18)..." << endl;
    bool torque_success = true;
    uint8_t dxl_error = 0;

    for (int id = 1; id <= 18; id++) {
        // Menggunakan fungsi asli dari Dynamixel SDK packetHandler
        int dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
        if (dxl_comm_result != COMM_SUCCESS) {
            cerr << "[WARNING] Servo ID " << id << " gagal merespons aktifasi torsi! Result: " << dxl_comm_result << endl;
            torque_success = false;
        }
    }
    
    if (torque_success) {
        cout << "[INFO] Semua servo Dynamixel siap! Torque diaktifkan." << endl;
    } else {
        cout << "[WARNING] Beberapa servo bermasalah, periksa perkabelan, ID, atau Baudrate!" << endl;
    }

    // 3. Setup Parameter Pergerakan Robot
    double z_offset = 0;
    double speed        = 1.5;   // Frekuensi langkah (1.5 siklus per detik)
    double body_height  = 45.0;  // Tinggi badan netral (sesuai J3L Anda)
    double step_length  = 5.0;  // Panjang langkah / amplitudo X
    double h_step = z_offset + (body_height * 0.4);
    
    cout << "[INFO] Parameter pergerakan diatur: Speed=" << speed << ", Body Height=" << body_height 
         << ", Step Length=" << step_length << ", Step Height=" << h_step << endl;
    StandUp(body_height, 2.0, 2.0); // Berdiri tegak dalam 2 detik
    cout << "Robot mulai berjalan. Tekan Ctrl+C untuk berhenti..." << endl;

    // 4. Masuk ke Loop Utama Tripod Gait
    Forward(speed, body_height, step_length, h_step);

    // 5. Matikan Torsi sebelum program ditutup
    for (int id = 1; id <= 18; id++) {
        packetHandler->write1ByteTxRx(portHandler, id, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
    }
    ShutdownDynamixel();
    
    return 0;
}