#include <cmath>
#include <iostream>
#include <mutex>
#include "IK2.h"
#include <dynamixel_sdk/dynamixel_sdk.h>

dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

dynamixel::GroupSyncWrite groupSyncWrite(portHandler, packetHandler, ADDR_GOAL_POSITION, 2);

std::mutex print_mutex;
std::mutex motor_mutex;
// const double J2L = 50.0;  // Panjang link J2
// const double J3L = 75.0; // Panjang link J3
// const double J1L = 25.0;

// void CartesianMove(double X, double Y, double Z);
// void UpdatePosition(double J1, double J2, double J3);
float mapFloat(float input, float minAngle, float maxAngle, float minValue, float maxValue) {
    float result = (input - minAngle) / (maxAngle - minAngle) * (maxValue - minValue) + minValue;
    return std::max(minValue, std::min(result, maxValue));
}
bool InitializeDynamixel() {
    if (!portHandler->openPort()) {
        std::cerr << "Gagal membuka port!" << std::endl;
        return false;
    }
    if (!portHandler->setBaudRate(BAUDRATE)) {
        std::cerr << "Gagal mengatur baudrate!" << std::endl;
        return false;
    }

    // Aktifkan torque untuk semua motor
    packetHandler->write1ByteTxRx(portHandler, ID1, ADDR_TORQUE_ENABLE, TORQUE_ENABLE);
    packetHandler->write1ByteTxRx(portHandler, ID2, ADDR_TORQUE_ENABLE, TORQUE_ENABLE);
    packetHandler->write1ByteTxRx(portHandler, ID3, ADDR_TORQUE_ENABLE, TORQUE_ENABLE);

    std::cout << "Torque diaktifkan untuk semua motor!" << std::endl;
    return true;
}
void SetMotorPosition(int id, double angle) {
    // float angle; 
    float minAngle = 0.0;     // Sudut minimum (bisa -180 jika diperlukan)
    float maxAngle = 360.0;   // Sudut maksimum
    float minValue = 0.0;     // Nilai minimum
    float maxValue = 1023.0;  // Nilai maksimum
    int position = mapFloat(angle, minAngle, maxAngle, minValue, maxValue);

    // Pastikan nilai dalam batas aman
    if (position < DXL_MIN_POSITION) position = DXL_MIN_POSITION;
    if (position > DXL_MAX_POSITION) position = DXL_MAX_POSITION;

    // Kirim perintah ke motor
    int dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_GOAL_POSITION, position);
    if (dxl_comm_result != COMM_SUCCESS) {
        std::cerr << "Gagal mengatur posisi ID " << id << std::endl;
    } else {
        // std::cout << "Motor ID " << id << " bergerak ke posisi " << position << " (" << angle << " derajat)" << std::endl;
    }
}

void AddMotorToSyncWrite(int id, double angle) {
    uint16_t position = (uint16_t)mapFloat(angle, 0.0, 360.0, 0, 1023); 

    uint8_t param_goal_position[2];
    param_goal_position[0] = position & 0xFF;       // Byte low
    param_goal_position[1] = (position >> 8) & 0xFF; // Byte high

    groupSyncWrite.addParam(id, param_goal_position);
}
void ExecuteSyncWrite() {
    groupSyncWrite.txPacket();
    groupSyncWrite.clearParam();
}

// **Fungsi untuk mematikan motor dan menutup port**
void ShutdownDynamixel() {
    packetHandler->write1ByteTxRx(portHandler, ID1, ADDR_TORQUE_ENABLE, TORQUE_DISABLE);
    packetHandler->write1ByteTxRx(portHandler, ID2, ADDR_TORQUE_ENABLE, TORQUE_DISABLE);
    packetHandler->write1ByteTxRx(portHandler, ID3, ADDR_TORQUE_ENABLE, TORQUE_DISABLE);

    std::cout << "Torque dimatikan. Port ditutup." << std::endl;
    portHandler->closePort();
}


void CartesianMove(double X, double Y, double Z, int coxa, int femur, int tibia) {
    // Gunakan mutex untuk mencegah output tumpang tindih
    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "CartesianMove: X=" << X << ", Y=" << Y << ", Z=" << Z << std::endl;
    }
    
    // Validasi input
    if (std::abs(Y) < 0.0001) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "Error: Nilai Y terlalu kecil, menghindari pembagian oleh nol.\n";
        Y = 0.0001;
    }

    double J1 = atan2(X, Y) * (180 / M_PI);
    double H_b = sqrt((Y * Y) + (X * X));
    double H = H_b - J1L;
    double L = sqrt((H * H) + (Z * Z));

    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "J1=" << J1 << ", H_b=" << H_b << ", H=" << H << ", L=" << L << std::endl;
    }

    if (L > (J2L + J3L)) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "Error: Posisi terlalu jauh, tidak dapat dijangkau oleh lengan robot.\n";
        return;
    }

    // Hitung dan validasi nilai untuk acos()
    double cosArg = ((J2L * J2L) + (J3L * J3L) - (L * L)) / (2 * J2L * J3L);
    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "J2L+J3L=" << (J2L+J3L) << ", L=" << L << std::endl;
        std::cout << "Cosine arg=" << cosArg << std::endl;
    }
    
    // Validasi nilai cosArg
    if (cosArg > 1.0) cosArg = 1.0;
    if (cosArg < -1.0) cosArg = -1.0;
    
    double J3 = acos(cosArg) * (180 / M_PI);
    
    cosArg = ((L * L) + (J2L * J2L) - (J3L * J3L)) / (2 * L * J2L);
    // Validasi nilai cosArg lagi
    if (cosArg > 1.0) cosArg = 1.0;
    if (cosArg < -1.0) cosArg = -1.0;
    
    double B = acos(cosArg) * (180 / M_PI);
    double A = atan2(Z, H) * (180 / M_PI);
    double J2 = B + A;
    
    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "J1=" << J1 << ", J2=" << J2 << ", J3=" << J3 << std::endl;
    }
    
    // Gunakan mutex untuk mengakses motor
    {
        std::lock_guard<std::mutex> lock(motor_mutex);
        UpdatePosition(J1, J2, J3, coxa, femur, tibia);
    }
}

void UpdatePosition(double J1, double J2, double J3, int coxa, int femur, int tibia) {
    double theta1 = 180 - J1;
    double theta2 = 180 - J2;
    double theta3 = 180 - J3 - J3_LegAngle;
    // Tambahkan semua motor ke dalam buffer Sync Write
    // int legs[] = {0, 6, 12};  // Leg 1 dan Leg 3 (offset +6 untuk leg 3)
    // for (int i = 0; i < 3; i++) {
        AddMotorToSyncWrite(coxa , theta1);
        AddMotorToSyncWrite(femur , theta2);
        AddMotorToSyncWrite(tibia , theta3);
    // }

    // Eksekusi semua perintah dalam satu transaksi
    ExecuteSyncWrite();
    
}
