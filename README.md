# ASHIBOT

Program kontrol robot hexapod berbasis C++ yang menggunakan:

* Dynamixel SDK
* OpenCV 4
* NVIDIA CUDA
* NVIDIA TensorRT
* libgpiod
* C++17

Project memiliki dua program utama:

| Target    | Fungsi                     |
| --------- | -------------------------- |
| `ashibot` | Program utama robot        |


---

# 1. System Requirements

Project ini ditujukan untuk berjalan pada **Linux NVIDIA Jetson**, khususnya Jetson Nano.

Software yang diperlukan:

* Ubuntu
* GCC / G++
* Make
* Git
* OpenCV 4
* CUDA
* TensorRT
* Dynamixel SDK
* libgpiod
* pthread

---

# 2. Clone Repository

Clone repository:

```bash
git clone https://github.com/NyomanSugiarte/pakmang_lastCH.git
```

Masuk ke folder project:

```bash
cd pakmang_lastCH
```

---

# 3. Install Compiler dan Build Tools

Install compiler dan Make:

```bash
sudo apt update
sudo apt install build-essential make git
```

Cek:

```bash
g++ --version
make --version
```

Project menggunakan standar:

```text
C++17
```

yang ditentukan pada Makefile:

```makefile
CXXFLAGS := -std=c++17 ...
```

---

# 4. Install OpenCV

Project menggunakan OpenCV 4 melalui `pkg-config`.

Cek apakah OpenCV sudah tersedia:

```bash
pkg-config --modversion opencv4
```

Contoh output:

```text
4.x.x
```

Jika muncul:

```text
Package opencv4 was not found
```

berarti OpenCV 4 belum terdeteksi oleh `pkg-config`.

Install development package:

```bash
sudo apt install libopencv-dev
```

Kemudian cek kembali:

```bash
pkg-config --modversion opencv4
```

---

# 5. Install libgpiod

Project menggunakan GPIO melalui library `libgpiod`.

Install:

```bash
sudo apt install libgpiod-dev gpiod
```

Cek library:

```bash
pkg-config --modversion libgpiod
```

> Jika sistem tidak menyediakan `libgpiod` melalui pkg-config, library tetap harus tersedia karena program melakukan linking dengan `-lgpiod`.

---

# 6. Dynamixel SDK

Project menggunakan **Dynamixel SDK C++**.

Pada Makefile, lokasi SDK adalah:

```text
/home/ashibot/DynamixelSDK
```

Header SDK:

```text
/home/ashibot/DynamixelSDK/c++/include
```

Library:

```text
/home/ashibot/DynamixelSDK/c++/build/linux_sbc
```

Makefile menggunakan:

```makefile
-I/home/ashibot/DynamixelSDK/c++/include
```

dan:

```makefile
-L/home/ashibot/DynamixelSDK/c++/build/linux_sbc
```

serta library:

```makefile
-ldxl_sbc_cpp
```

## Install SDK

Jika SDK belum ada:

```bash
cd ~
git clone https://github.com/ROBOTIS-GIT/DynamixelSDK.git
```

Masuk ke folder C++:

```bash
cd ~/DynamixelSDK/c++
```

Build SDK sesuai platform Linux yang digunakan.

Setelah selesai, pastikan file berikut tersedia:

```bash
ls ~/DynamixelSDK/c++/build/linux_sbc/
```

Harus terdapat library:

```text
libdxl_sbc_cpp.so
```

Cek:

```bash
find ~/DynamixelSDK -name "libdxl_sbc_cpp.so"
```

---

# 7. CUDA

Project menggunakan CUDA.

Makefile menggunakan:

```makefile
-I/usr/local/cuda/include
```

dan:

```makefile
-L/usr/local/cuda/lib64
```

Cek instalasi CUDA:

```bash
nvcc --version
```

Pastikan directory berikut tersedia:

```bash
ls /usr/local/cuda/include
```

dan:

```bash
ls /usr/local/cuda/lib64
```

---

# 8. TensorRT

Program YOLO menggunakan NVIDIA TensorRT.

Makefile melakukan linking dengan:

```makefile
-lnvinfer
-lnvonnxparser
-lncudart
```

Library yang dibutuhkan:

```text
libnvinfer.so
libnvonnxparser.so
libcudart.so
```

Cek:

```bash
ls /usr/lib/aarch64-linux-gnu/libnvinfer.so*
```

dan:

```bash
ls /usr/lib/aarch64-linux-gnu/libnvonnxparser.so*
```

Lokasi dapat berbeda tergantung versi JetPack/Jetson yang digunakan.

---

# 9. Struktur Project

Struktur source code utama:

```text
pakmang_lastCH/
│
├── Makefile
├── main.cpp
├── ong.cpp
│
├── ong-mission/
│   ├── case1.cpp
│   └── case2.cpp
│
├── src/
│   │
│   ├── arm/
│   │   ├── arm.cpp
│   │   └── ServoController.cpp
│   │
│   ├── cam/
│   │   └── Camera.cpp
│   │
│   ├── heading/
│   │   └── kompas.cpp
│   │
│   ├── movement/
│   │   ├── DynamixelController.cpp
│   │   ├── Gmini_Tr.cpp
│   │   ├── IK2.cpp
│   │   └── movement.cpp
│   │
│   └── yolo/
│       └── YoloDetector.cpp
│
└── include/
```

---

# 10. Compile Program

Setelah semua dependency tersedia, cukup jalankan:

```bash
make
```

Perintah tersebut akan membuat executable:

```text
ashibot
```

Prosesnya secara otomatis:

```text
.cpp
  ↓
.o
  ↓
Linking
  ↓
ashibot
```

Source yang di-compile untuk program utama:

```text
main.cpp
src/arm/arm.cpp
src/arm/ServoController.cpp
src/cam/Camera.cpp
src/heading/kompas.cpp
src/movement/DynamixelController.cpp
src/movement/Gmini_Tr.cpp
src/movement/IK2.cpp
src/movement/movement.cpp
src/yolo/YoloDetector.cpp
```

---

# 11. Menjalankan Program ASHIBOT

Setelah compile berhasil:

```bash
make run
```

Perintah `make run` menjalankan:

```bash
./ashibot
```

dengan `LD_LIBRARY_PATH` yang sudah diarahkan ke Dynamixel SDK:

```bash
LD_LIBRARY_PATH=/home/ashibot/DynamixelSDK/c++/build/linux_sbc:$LD_LIBRARY_PATH ./ashibot
```

Hal ini diperlukan agar sistem dapat menemukan:

```text
libdxl_sbc_cpp.so
```

---

# 12. Compile Program ONG

Selain program utama, terdapat target khusus:

```text
ong
```

Untuk compile:

```bash
make ong
```

Executable yang dihasilkan:

```text
ong
```

Source yang digunakan:

```text
ong.cpp
ong-mission/case1.cpp
ong-mission/case2.cpp
src/arm/arm.cpp
src/arm/ServoController.cpp
src/cam/Camera.cpp
src/heading/kompas.cpp
src/movement/DynamixelController.cpp
src/movement/Gmini_Tr.cpp
src/movement/IK2.cpp
src/movement/movement.cpp
src/yolo/YoloDetector.cpp
```

---

# 13. Menjalankan Program ONG

Setelah compile:

```bash
make run-ong
```

Perintah tersebut menjalankan:

```bash
./ong
```

dengan library Dynamixel SDK:

```text
/home/ashibot/DynamixelSDK/c++/build/linux_sbc
```

---

# 14. Membersihkan Hasil Compile

Untuk menghapus seluruh file object dan executable:

```bash
make clean
```

Perintah ini menjalankan:

```bash
rm -f $(OBJS) $(TARGET)
```

Jika sebelumnya juga sudah melakukan compile `ong`, dan ingin membersihkan object `ong`, dapat dilakukan manual:

```bash
rm -f ong ong.o ong-mission/*.o
```

---

# 15. Perintah Build Lengkap

## Program utama

```bash
make
```

Jalankan:

```bash
make run
```

---

## Program ONG

Compile:

```bash
make ong
```

Jalankan:

```bash
make run-ong
```

---

## Clean

```bash
make clean
```

---

# 16. Build dari Awal

Jika ingin melakukan build ulang dari kondisi bersih:

```bash
make clean
make
```

Kemudian jalankan:

```bash
make run
```

Untuk ONG:

```bash
make clean
make ong
make run-ong
```

---

# 17. Troubleshooting

## A. Dynamixel SDK tidak ditemukan

Jika muncul:

```text
fatal error: dynamixel_sdk/dynamixel_sdk.h: No such file or directory
```

pastikan SDK berada di:

```text
/home/ashibot/DynamixelSDK
```

Cek:

```bash
ls /home/ashibot/DynamixelSDK/c++/include/dynamixel_sdk
```

Jika menggunakan username berbeda, ubah path pada `Makefile`:

```makefile
-I/home/USERNAME/DynamixelSDK/c++/include
```

dan:

```makefile
-L/home/USERNAME/DynamixelSDK/c++/build/linux_sbc
```

---

## B. `libdxl_sbc_cpp.so` tidak ditemukan

Jika muncul:

```text
cannot find -ldxl_sbc_cpp
```

cek:

```bash
find /home/ashibot/DynamixelSDK -name "libdxl_sbc_cpp.so"
```

Pastikan hasilnya berada di:

```text
/home/ashibot/DynamixelSDK/c++/build/linux_sbc/
```

---

## C. Library ditemukan saat compile tetapi tidak saat run

Jika muncul:

```text
error while loading shared libraries:
libdxl_sbc_cpp.so
```

jalankan menggunakan:

```bash
make run
```

Jangan langsung:

```bash
./ashibot
```

karena target `run` sudah mengatur:

```bash
LD_LIBRARY_PATH
```

---

## D. OpenCV tidak ditemukan

Jika muncul:

```text
Package opencv4 was not found
```

cek:

```bash
pkg-config --modversion opencv4
```

Kemudian install:

```bash
sudo apt install libopencv-dev
```

---

## E. CUDA tidak ditemukan

Cek:

```bash
nvcc --version
```

dan:

```bash
ls /usr/local/cuda
```

Makefile mengasumsikan CUDA berada di:

```text
/usr/local/cuda
```

Jika lokasi berbeda, ubah:

```makefile
-I/usr/local/cuda/include
```

dan:

```makefile
-L/usr/local/cuda/lib64
```

---

## F. TensorRT tidak ditemukan

Jika muncul:

```text
cannot find -lnvinfer
```

atau:

```text
cannot find -lnvonnxparser
```

pastikan TensorRT sudah ter-install pada Jetson.

Cek:

```bash
ldconfig -p | grep nvinfer
```

dan:

```bash
ldconfig -p | grep nvonnxparser
```

---

## G. GPIO tidak ditemukan

Jika muncul:

```text
cannot find -lgpiod
```

install:

```bash
sudo apt install libgpiod-dev gpiod
```

---

# 18. Permission Dynamixel

Pastikan user mempunyai akses ke USB serial Dynamixel.

Cek device:

```bash
ls /dev/ttyUSB*
```

Jika menggunakan:

```text
/dev/ttyUSB0
```

cek permission:

```bash
ls -l /dev/ttyUSB0
```

Tambahkan user ke group `dialout`:

```bash
sudo usermod -aG dialout $USER
```

Kemudian logout dan login kembali.

Cek:

```bash
groups
```

Pastikan terdapat:

```text
dialout
```

---

# 19. Quick Start

Jika seluruh dependency sudah terpasang:

```bash
git clone https://github.com/NyomanSugiarte/pakmang_lastCH.git
cd pakmang_lastCH
make
make run
```

Untuk program ONG:

```bash
make ong
make run-ong
```

---

# 20. Build Flow

```text
                 SOURCE CODE
                     │
                     ▼
              ┌──────────────┐
              │    Makefile  │
              └──────┬───────┘
                     │
                     ▼
              ┌──────────────┐
              │     g++      │
              │    C++17     │
              └──────┬───────┘
                     │
                     ▼
              ┌──────────────┐
              │ Object Files │
              │    (*.o)     │
              └──────┬───────┘
                     │
                     ▼
             ┌─────────────────┐
             │     Linking     │
             │                 │
             │ Dynamixel SDK   │
             │ CUDA            │
             │ TensorRT        │
             │ OpenCV          │
             │ libgpiod        │
             │ pthread         │
             └────────┬────────┘
                      │
             ┌────────┴────────┐
             ▼                 ▼
        ┌─────────┐       ┌─────────┐
        │ ashibot │       │   ong   │
        └─────────┘       └─────────┘
```

---

# 21. Dependency Summary

| Dependency    | Kegunaan                   |
| ------------- | -------------------------- |
| GCC / G++     | Compile C++                |
| Make          | Build automation           |
| C++17         | Standard bahasa C++        |
| Dynamixel SDK | Komunikasi servo Dynamixel |
| OpenCV 4      | Pemrosesan kamera          |
| CUDA          | GPU acceleration           |
| TensorRT      | Inferensi YOLO             |
| ONNX Parser   | Membaca model ONNX         |
| libgpiod      | Kontrol GPIO               |
| pthread       | Multithreading             |

---

# 22. Makefile Targets

| Command        | Fungsi                                |
| -------------- | ------------------------------------- |
| `make`         | Compile `ashibot`                     |
| `make run`     | Compile dan menjalankan `ashibot`     |
| `make ong`     | Compile `ong`                         |
| `make run-ong` | Compile dan menjalankan `ong`         |
| `make clean`   | Menghapus object dan executable utama |

---

# 23. Hardware

Program dirancang untuk sistem robot hexapod yang menggunakan:

```text
Jetson
   │
   ├── Camera
   │
   ├── GPIO
   │
   ├── Dynamixel SDK
   │       │
   │       ▼
   │      U2D2
   │       │
   │       ▼
   │   Dynamixel
   │
   └── TensorRT / YOLO
```

Pastikan power supply servo sudah tersedia sebelum menjalankan program.

---

# 24. Important Notes

Program dapat mengaktifkan actuator secara langsung. Sebelum menjalankan:

1. Pastikan seluruh servo terpasang dengan benar.
2. Pastikan ID Dynamixel sesuai.
3. Pastikan power servo tersedia.
4. Pastikan komunikasi Dynamixel dapat digunakan.
5. Pastikan robot berada pada posisi aman.
6. Untuk pengujian pertama, lakukan pengujian tanpa beban pada kaki.
7. Gunakan `Ctrl+C` jika robot bergerak tidak sesuai.

---

# 25. Repository

GitHub:

https://github.com/NyomanSugiarte/pakmang_lastCH
