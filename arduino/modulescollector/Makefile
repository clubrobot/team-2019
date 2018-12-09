#Board properties
BOARD_UUID = modulescollector
BOARD_TAG = nano
BOARD_SUB = atmega328


#Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = modulescollector.ino 
LOCAL_CPP_SRCS = \
    instructions.cpp \
    $(COMMON)/SerialTalks.cpp \
    $(COMMON)/CRC16.cpp \
    $(COMMON)/EndStop.cpp \
    $(COMMON)/DCMotor.cpp \
    $(COMMON)/PeriodicProcess.cpp \
    $(COMMON)/VelocityServo.cpp \
    $(COMMON)/FullSpeedServo.cpp \
    $(COMMON)/Clock.cpp

#sketch libraries
ARDUINO_LIBS = EEPROM Servo

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk
