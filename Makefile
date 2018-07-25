# Compiler used
CXX ?= g++

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

all: main.cpp GPIO.cpp $(BUILD_DIR)
	g++ $(LIBS) $(COMPILE_FLAGS) main.cpp -o $(BUILD_DIR)/$(BIN_NAME)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# If a command is passed with @foo it is not redirected to stdout
clean:
	rm -rf $(BUILD_DIR)
