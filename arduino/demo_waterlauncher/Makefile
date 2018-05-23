#Board properties
BOARD_UUID = demo
BOARD_TAG = nano
BOARD_SUB = atmega328


#Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = demo.ino 
LOCAL_CPP_SRCS = \
    instructions.cpp \
    $(COMMON)/SerialTalks.cpp \
    $(COMMON)/CRC16.cpp \
    $(COMMON)/BrushlessMotor.cpp



#sketch libraries
ARDUINO_LIBS = EEPROM Servo

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk
