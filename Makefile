# Compiler used
CC = g++

# Variables
BIN_NAME = faceDetect
BUILD_DIR = build
# BIN_DIR = bin
OPENCV = `pkg-config opencv --cflags --libs`
INSTALL_PREFIX = ./bin

# Path to the source directory, relative to the makefile
SRC_PATH = .
# Add additional include paths
INCLUDES = -I $(SRC_PATH)

# General compiler flags
COMPILE_FLAGS = -Wall

# Space-separated pkg-config libraries used by this project
LIBS = $(OPENCV)

all: main.cpp GPIO $(BUILD_DIR)
	$(CC) $(LIBS) $(COMPILE_FLAGS) main.cpp $(BUILD_DIR)/GPIO.o -o $(BUILD_DIR)/$(BIN_NAME)

GPIO: $(BUILD_DIR) GPIO.cpp
	$(CC) -c GPIO.cpp -o $(BUILD_DIR)/GPIO.o 

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# If a command is passed with @foo it is not redirected to stdout
clean:
	rm -f $(BUILD_DIR)/GPIO.o	
	rm -rf $(BUILD_DIR)
