#Board properties
BOARD_UUID = modulescollector
BOARD_TAG = nano
BOARD_SUB = atmega328


#Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = main.ino 
LOCAL_CPP_SRCS = \
    instructions.cpp \
    $(COMMON)/SerialTalks.cpp \
    $(COMMON)/CRC16.cpp \
    $(COMMON)/ShiftRegAX12.cpp \
    $(COMMON)/SoftwareSerial.cpp \
    $(COMMON)/ShiftRegister.cpp \
    $(COMMON)/IK/IK.cpp \
    $(COMMON)/IK/Joint.cpp \
    $(COMMON)/IK/Matrix.cpp \
    $(COMMON)/IK/Arm.cpp


    #$(COMMON)/IK \#
    
#sketch libraries
ARDUINO_LIBS = EEPROM Servo ArduinoSTL

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk
