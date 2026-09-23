# Hexapod Dynamixel Controller

Program kontrol gerak robot **hexapod** berbasis C++ menggunakan **Dynamixel SDK**. Program ini menggunakan metode **Inverse Kinematics (IK)** untuk mengubah koordinat Cartesian kaki robot menjadi sudut servo, kemudian mengirimkan perintah posisi ke Dynamixel menggunakan **Sync Write**.

Repository:

[https://github.com/NyomanSugiarte/pakmang_lastCH](https://github.com/NyomanSugiarte/pakmang_lastCH?utm_source=chatgpt.com)

---

## 1. Fitur

Program ini memiliki beberapa fungsi utama:

* Kontrol 18 servo Dynamixel untuk robot hexapod.
* Inverse Kinematics 3-DOF untuk setiap kaki.
* Perhitungan koordinat Cartesian `X`, `Y`, dan `Z`.
* Trajectory kaki untuk gerakan berjalan.
* Tripod gait.
* Sync Write untuk mengirim posisi beberapa servo secara bersamaan.
* Pengaturan kecepatan, tinggi badan, panjang langkah, dan tinggi langkah.
* Torque ON/OFF pada servo Dynamixel.
* Komunikasi menggunakan Dynamixel Protocol 1.0.

Program utama menjalankan robot dalam mode **Forward / tripod gait**. Pada `main.cpp`, parameter awal yang digunakan adalah:

```cpp
double speed = 1.5;
double body_height = 45.0;
double step_length = 5.0;
double h_step = z_offset + (body_height * 0.4);
```

Kemudian robot melakukan:

```cpp
StandUp(body_height, 2.0, 2.0);
Forward(speed, body_height, step_length, h_step);
```

---

# 2. Struktur Repository

```text
pakmang_lastCH/
│
├── main.cpp
│
├── IK2.cpp
├── IK2.h
│
├── Gmini_Tr.cpp
├── Gmini_Tr.h
│
├── movement.cpp
├── movement.h
│
└── README.md
```

### `main.cpp`

Program utama yang melakukan:

1. Inisialisasi Dynamixel.
2. Membuka komunikasi dengan servo.
3. Mengaktifkan torque.
4. Mengatur parameter gerakan.
5. Menjalankan posisi berdiri.
6. Menjalankan tripod gait.
7. Mematikan torque ketika program selesai.

### `IK2.cpp / IK2.h`

Berisi fungsi komunikasi Dynamixel dan perhitungan inverse kinematics.

Konfigurasi utama pada `IK2.h`:

```cpp
#define BAUDRATE 1000000
#define DEVICENAME "/dev/ttyUSB0"
#define PROTOCOL_VERSION 1.0
```

Panjang link kaki:

```cpp
const double J1L = 25.0;
const double J2L = 50.0;
const double J3L = 75.0;
```

### `Gmini_Tr.cpp / Gmini_Tr.h`

Berisi trajectory generator untuk pergerakan kaki.

Trajectory menghitung posisi kaki berdasarkan parameter:

```text
X
Y
Z
step height
fase swing
```

Kemudian koordinat tersebut dikirim ke fungsi inverse kinematics.

Program juga menggunakan trajectory **swing** dan **stance** untuk membentuk tripod gait.

### `movement.cpp / movement.h`

Berisi fungsi pergerakan robot seperti:

```cpp
Forward()
Backward()
```

Tripod gait menggunakan dua kelompok kaki:

```cpp
kaki_A = {0, 2, 4};
kaki_B = {1, 3, 5};
```

Setiap kelompok bergantian antara fase `Swing` dan `Stance`.

Perintah posisi kemudian dikirim menggunakan Sync Write setiap sekitar 10 ms.

---

# 3. Hardware

Program ini dibuat untuk robot hexapod dengan konfigurasi:

| Komponen       | Konfigurasi              |
| -------------- | ------------------------ |
| Controller     | Linux SBC / Jetson       |
| Servo          | Dynamixel AX-Series      |
| Jumlah servo   | 18                       |
| Servo per kaki | 3                        |
| Jumlah kaki    | 6                        |
| Protocol       | Dynamixel Protocol 1.0   |
| Baudrate       | 1,000,000                |
| Interface      | USB Dynamixel Controller |
| Port default   | `/dev/ttyUSB0`           |

Program menggunakan ID servo:

```text
1 - 18
```

Setiap kaki terdiri dari tiga servo:

```text
Coxa
Femur
Tibia
```

> **Perhatian:** Pastikan konfigurasi ID, baudrate, protocol, dan wiring sesuai dengan robot sebelum menjalankan program. Program akan mengaktifkan torque servo.

---

# 4. Software Requirements

Sistem membutuhkan:

* Linux
* GCC / G++
* Git
* Dynamixel SDK C++
* pthread / C++ thread support
* Dynamixel USB interface

Untuk Ubuntu/Debian, install compiler dan Git:

```bash
sudo apt update
sudo apt install build-essential git
```

Cek compiler:

```bash
g++ --version
```

---

# 5. Install Dynamixel SDK

Program menggunakan header:

```cpp
#include <dynamixel_sdk/dynamixel_sdk.h>
```

dan API:

```cpp
dynamixel::PortHandler
dynamixel::PacketHandler
dynamixel::GroupSyncWrite
```

SDK harus sudah ter-install sebelum melakukan compile.

Clone Dynamixel SDK:

```bash
cd ~
git clone https://github.com/ROBOTIS-GIT/DynamixelSDK.git
```

Masuk ke folder C++:

```bash
cd ~/DynamixelSDK/c++
```

Kemudian build SDK sesuai dengan platform Linux yang digunakan.

Setelah SDK berhasil di-build, pastikan library Dynamixel tersedia.

Contoh:

```bash
find ~/DynamixelSDK -name "*.so"
```

---

# 6. Clone Repository

Clone repository:

```bash
cd ~
git clone https://github.com/NyomanSugiarte/pakmang_lastCH.git
```

Masuk ke folder:

```bash
cd ~/pakmang_lastCH
```

Cek file:

```bash
ls
```

Output yang diharapkan:

```text
Gmini_Tr.cpp
Gmini_Tr.h
IK2.cpp
IK2.h
README.md
main.cpp
movement.cpp
movement.h
```

---

# 7. Compile Program

## Cara 1 — Jika Dynamixel SDK ter-install di system

Jika header Dynamixel SDK berada di:

```text
/usr/local/include
```

dan library berada di:

```text
/usr/local/lib
```

compile:

```bash
g++ -std=c++17 \
    main.cpp \
    IK2.cpp \
    Gmini_Tr.cpp \
    movement.cpp \
    -o ashibot \
    -ldxl_x64_c++ \
    -lpthread
```

Kemudian cek apakah executable berhasil dibuat:

```bash
ls -lh ashibot
```

---

# 8. Compile pada Jetson Nano

Jika menggunakan **Jetson Nano** dan library Dynamixel SDK berada pada folder build Linux SBC, gunakan lokasi library SDK sesuai instalasi.

Contoh struktur:

```text
~/DynamixelSDK/
└── c++/
    ├── include/
    │   └── dynamixel_sdk/
    └── build/
        └── linux_sbc/
            └── libdxl_sbc_cpp.so
```

Compile dengan:

```bash
g++ -std=c++17 \
    main.cpp \
    IK2.cpp \
    Gmini_Tr.cpp \
    movement.cpp \
    -I$HOME/DynamixelSDK/c++/include \
    -L$HOME/DynamixelSDK/c++/build/linux_sbc \
    -Wl,-rpath,$HOME/DynamixelSDK/c++/build/linux_sbc \
    -ldxl_sbc_cpp \
    -lpthread \
    -o ashibot
```

Jika berhasil:

```bash
ls -lh ashibot
```

Akan muncul executable:

```text
ashibot
```

---

# 9. Jika Library Tidak Ditemukan

Jika muncul error:

```text
fatal error: dynamixel_sdk/dynamixel_sdk.h: No such file or directory
```

berarti compiler belum mengetahui lokasi header SDK.

Gunakan:

```bash
-I/path/ke/DynamixelSDK/c++/include
```

Contoh:

```bash
-I$HOME/DynamixelSDK/c++/include
```

---

Jika muncul:

```text
/usr/bin/ld: cannot find -ldxl_sbc_cpp
```

berarti compiler belum menemukan file library `.so`.

Cari library:

```bash
find ~/DynamixelSDK -name "libdxl*.so"
```

Misalnya hasilnya:

```text
/home/ashibot/DynamixelSDK/c++/build/linux_sbc/libdxl_sbc_cpp.so
```

maka tambahkan:

```bash
-L/home/ashibot/DynamixelSDK/c++/build/linux_sbc
```

---

# 10. Cek Dynamixel USB Port

Program secara default menggunakan:

```cpp
#define DEVICENAME "/dev/ttyUSB0"
```

Hubungkan U2D2 / USB Dynamixel Controller kemudian cek:

```bash
ls /dev/ttyUSB*
```

Contoh:

```text
/dev/ttyUSB0
```

Jika device berbeda, misalnya:

```text
/dev/ttyUSB1
```

ubah:

```cpp
#define DEVICENAME "/dev/ttyUSB1"
```

di:

```text
IK2.h
```

---

# 11. Permission USB

Jika muncul:

```text
Gagal membuka port!
```

cek permission:

```bash
ls -l /dev/ttyUSB0
```

Jika user belum memiliki akses serial, tambahkan user ke group `dialout`:

```bash
sudo usermod -aG dialout $USER
```

Kemudian logout dan login kembali.

Atau reboot:

```bash
sudo reboot
```

Setelah login kembali:

```bash
groups
```

Pastikan terdapat:

```text
dialout
```

---

# 12. Cek Baudrate

Program menggunakan:

```cpp
#define BAUDRATE 1000000
```

atau:

```text
1,000,000 bps
```

Semua servo yang digunakan harus menggunakan baudrate yang sama.

Jika servo menggunakan baudrate lain, ubah:

```cpp
#define BAUDRATE 1000000
```

sesuai konfigurasi Dynamixel.

---

# 13. Cek Protocol

Program menggunakan:

```cpp
#define PROTOCOL_VERSION 1.0
```

Program ini ditujukan untuk Dynamixel yang menggunakan **Protocol 1.0**.

Pastikan servo yang digunakan mendukung protocol tersebut.

---

# 14. Menjalankan Program

Setelah berhasil compile:

```bash
./ashibot
```

Program akan menampilkan:

```text
=========================================
 HEXAPOD TRIPOD GAIT CONTROLLER
=========================================
```

Kemudian program melakukan inisialisasi Dynamixel.

Jika komunikasi berhasil:

```text
[INFO] Komunikasi Dynamixel Berhasil Dibuka.
```

Program kemudian mengaktifkan torque servo.

Setelah itu robot melakukan:

```text
Stand Up
     ↓
Tripod Gait
     ↓
Forward
```

---

# 15. Menghentikan Program

Program `Forward()` berjalan dalam loop:

```cpp
while (true)
```

sehingga program akan terus menjalankan robot.

Untuk menghentikan program:

```text
Ctrl + C
```

Setelah loop selesai, program akan mematikan torque dan menutup port Dynamixel.

---

# 16. Mengubah Parameter Gerakan

Parameter gerakan terdapat di:

```text
main.cpp
```

Contoh:

```cpp
double speed = 1.5;
double body_height = 45.0;
double step_length = 5.0;
double h_step = z_offset + (body_height * 0.4);
```

### Speed

```cpp
double speed = 1.5;
```

Mengatur kecepatan siklus gait.

Nilai lebih besar:

```cpp
speed = 2.0;
```

akan membuat siklus gerakan lebih cepat.

---

### Body Height

```cpp
double body_height = 45.0;
```

Mengatur tinggi koordinat badan robot terhadap kaki.

---

### Step Length

```cpp
double step_length = 5.0;
```

Mengatur panjang langkah kaki.

Nilai lebih besar menghasilkan perpindahan kaki yang lebih jauh.

---

### Step Height

```cpp
double h_step = z_offset + (body_height * 0.4);
```

Mengatur tinggi kaki ketika berada pada fase swing.

---

# 17. Inverse Kinematics

Panjang link kaki yang digunakan:

```text
J1 = 25 mm
J2 = 50 mm
J3 = 75 mm
```

Konfigurasi tersebut terdapat di `IK2.h`:

```cpp
const double J1L = 25.0;
const double J2L = 50.0;
const double J3L = 75.0;
```

Secara umum alur perhitungannya:

```text
Cartesian Position
       │
       ├── X
       ├── Y
       └── Z
       │
       ▼
Inverse Kinematics
       │
       ├── J1
       ├── J2
       └── J3
       │
       ▼
Servo Angle
       │
       ▼
Dynamixel Position
       │
       ▼
Sync Write
       │
       ▼
Servo
```

Fungsi `CartesianMoveBuffered()` pada `Gmini_Tr.cpp` melakukan perhitungan koordinat Cartesian menjadi sudut joint dan memasukkan hasilnya ke buffer Sync Write.

---

# 18. Tripod Gait

Robot menggunakan enam kaki yang dibagi menjadi dua kelompok.

### Group A

```cpp
int kaki_A[] = {0, 2, 4};
```

### Group B

```cpp
int kaki_B[] = {1, 3, 5};
```

Kelompok A dan B bergerak secara bergantian:

```text
Cycle 1

Group A → Swing
Group B → Stance


Cycle 2

Group A → Stance
Group B → Swing
```

Dengan metode tersebut robot menghasilkan pola **tripod gait**.

---

# 19. Sync Write

Program menggunakan:

```cpp
dynamixel::GroupSyncWrite
```

dengan:

```cpp
ADDR_GOAL_POSITION = 30
```

dan panjang data:

```text
2 byte
```

Posisi servo dikonversi dari sudut:

```text
0° - 360°
```

menjadi:

```text
0 - 1023
```

kemudian dikirim secara bersamaan menggunakan Sync Write.

---

# 20. Troubleshooting

## Dynamixel tidak terdeteksi

Cek:

```bash
ls /dev/ttyUSB*
```

Kemudian pastikan:

* USB controller terhubung.
* Kabel Dynamixel terhubung.
* Power servo aktif.
* Port sesuai.
* Baudrate sesuai.
* Protocol sesuai.
* ID servo sesuai.

---

## `Gagal membuka port!`

Cek:

```bash
ls -l /dev/ttyUSB0
```

Kemudian:

```bash
sudo usermod -aG dialout $USER
```

Logout/login kembali.

---

## Servo tidak bergerak

Periksa:

```text
Power Dynamixel
       ↓
ID servo
       ↓
Protocol
       ↓
Baudrate
       ↓
Torque
       ↓
Goal Position
```

Program menggunakan ID servo 1–18 pada bagian utama untuk mengaktifkan torque.

---

## `cannot find -ldxl_sbc_cpp`

Cari library:

```bash
find ~/DynamixelSDK -name "libdxl*.so"
```

Kemudian gunakan lokasi folder library tersebut dengan:

```bash
-L/path/library
```

Contoh:

```bash
-L$HOME/DynamixelSDK/c++/build/linux_sbc
```

---

## Header Dynamixel tidak ditemukan

Error:

```text
fatal error: dynamixel_sdk/dynamixel_sdk.h:
No such file or directory
```

Gunakan:

```bash
-I$HOME/DynamixelSDK/c++/include
```

---

# 21. Clean Build

Untuk compile ulang:

```bash
rm -f ashibot
```

kemudian:

```bash
g++ -std=c++17 \
    main.cpp \
    IK2.cpp \
    Gmini_Tr.cpp \
    movement.cpp \
    -I$HOME/DynamixelSDK/c++/include \
    -L$HOME/DynamixelSDK/c++/build/linux_sbc \
    -Wl,-rpath,$HOME/DynamixelSDK/c++/build/linux_sbc \
    -ldxl_sbc_cpp \
    -lpthread \
    -o ashibot
```

---

# 22. Ringkasan Build

Jika semua dependency sudah tersedia, proses dari awal:

```bash
git clone https://github.com/NyomanSugiarte/pakmang_lastCH.git
cd pakmang_lastCH
```

Compile:

```bash
g++ -std=c++17 \
    main.cpp \
    IK2.cpp \
    Gmini_Tr.cpp \
    movement.cpp \
    -I$HOME/DynamixelSDK/c++/include \
    -L$HOME/DynamixelSDK/c++/build/linux_sbc \
    -Wl,-rpath,$HOME/DynamixelSDK/c++/build/linux_sbc \
    -ldxl_sbc_cpp \
    -lpthread \
    -o ashibot
```

Jalankan:

```bash
./ashibot
```

---

# 23. Catatan Keselamatan

Program ini secara langsung mengaktifkan torque pada servo Dynamixel.

Sebelum menjalankan program:

1. Pastikan robot berada pada posisi yang aman.
2. Pastikan tidak ada benda yang menghalangi kaki.
3. Pastikan supply servo mencukupi.
4. Pastikan ID servo benar.
5. Pastikan arah pemasangan servo sesuai konfigurasi IK.
6. Untuk pengujian pertama, angkat robot dari lantai agar kaki tidak langsung memberikan gaya ke permukaan.
7. Siapkan akses `Ctrl+C` untuk menghentikan program.

**Jangan menjalankan robot pertama kali dengan kecepatan tinggi sebelum konfigurasi IK dan arah servo diverifikasi.**

---

# 24. Author

**Nyoman Sugiarte**

Repository:

https://github.com/NyomanSugiarte/pakmang_lastCH

---

## License

Tambahkan informasi lisensi di repository apabila proyek ini akan didistribusikan secara publik.
