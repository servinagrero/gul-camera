# Compiler used
CC = g++

# Variables
BIN_NAME = faceDetect
BUILD_DIR = build
# BIN_DIR = bin
OPENCV = `pkg-config opencv --cflags --libs`
QT5 = `pkg-config --libs --cflags Qt5SerialPort`
GST = `pkg-config --libs --cflags gstreamer-1.0`

INSTALL_PREFIX = ./bin

# Path to the source directory, relative to the makefile
SRC_PATH = .
# Add additional include paths
INCLUDES = -I $(SRC_PATH)

# General compiler flags
COMPILE_FLAGS = -Wall -fPIC

# Space-separated pkg-config libraries used by this project
LIBS = $(OPENCV) $(QT5) $(GST)

all: main.cpp $(BUILD_DIR)
	$(CC) $(LIBS) $(COMPILE_FLAGS) -g main.cpp -o $(BUILD_DIR)/$(BIN_NAME)

Serial: $(BUILD_DIR) Serial.cpp
	$(CC) -c Serial.cpp -o $(BUILD_DIR)/Serial.o 

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# If a command is passed with @foo it is not redirected to stdout
clean:
	rm -f $(BUILD_DIR)/Serial.o	
	rm -rf $(BUILD_DIR)
