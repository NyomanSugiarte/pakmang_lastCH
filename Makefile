CXX      := g++
TARGET   := ashibot

CXXFLAGS := -std=c++17 -Iinclude -I/usr/local/cuda/include -I/home/ashibot/DynamixelSDK/c++/include $(shell pkg-config --cflags opencv4)

LDFLAGS  := -L/home/ashibot/DynamixelSDK/c++/build/linux_sbc -L/usr/local/cuda/lib64
LDLIBS   := -ldxl_sbc_cpp -lnvinfer -lnvonnxparser -lcudart $(shell pkg-config --libs opencv4) -lgpiod -lpthread
SRCS := main.cpp \
        src/arm/arm.cpp \
        src/arm/ServoController.cpp \
        src/cam/Camera.cpp \
        src/heading/kompas.cpp \
        src/movement/DynamixelController.cpp \
        src/movement/Gmini_Tr.cpp \
        src/movement/IK2.cpp \
        src/movement/movement.cpp \
        src/yolo/YoloDetector.cpp

OBJS := $(SRCS:.cpp=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	LD_LIBRARY_PATH=/home/ashibot/DynamixelSDK/c++/build/linux_sbc:$$LD_LIBRARY_PATH ./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)



# --- TARGET KHUSUS ONG ---
ONG_TARGET := ong
ONG_SRCS   := ong.cpp \
              ong-mission/case1.cpp \
              ong-mission/case2.cpp \
              src/arm/arm.cpp \
              src/arm/ServoController.cpp \
              src/cam/Camera.cpp \
              src/heading/kompas.cpp \
              src/movement/DynamixelController.cpp \
              src/movement/Gmini_Tr.cpp \
              src/movement/IK2.cpp \
              src/movement/movement.cpp \
              src/yolo/YoloDetector.cpp

ONG_OBJS   := $(ONG_SRCS:.cpp=.o)

$(ONG_TARGET): $(ONG_OBJS)
	$(CXX) $(ONG_OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

run-ong: $(ONG_TARGET)
	LD_LIBRARY_PATH=/home/ashibot/DynamixelSDK/c++/build/linux_sbc:$$LD_LIBRARY_PATH ./$(ONG_TARGET)