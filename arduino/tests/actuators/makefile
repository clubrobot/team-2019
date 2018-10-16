#Board properties
BOARD_UUID = actuators
BOARD_TAG = nano
BOARD_SUB = atmega328


#Sketch sources
COMMON = ../../common
LOCAL_INO_SRCS = actuators.ino 
LOCAL_CPP_SRCS = \
    instructions.cpp \
    $(COMMON)/AX12.cpp \
    $(COMMON)/SoftwareSerial.cpp \
    $(COMMON)/PeriodicProcess.cpp \
    $(COMMON)/SerialTalks.cpp \
    $(COMMON)/CRC16.cpp \
    $(COMMON)/EndStop.cpp \
    $(COMMON)/DCMotor.cpp 



#sketch libraries
ARDUINO_LIBS = EEPROM Servo

# Increas max opcode number
CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x20

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ../../
include $(MODULEMK_DIR)/Module.mk