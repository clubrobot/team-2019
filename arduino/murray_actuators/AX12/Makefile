BOARD_UUID = RobotArm
BOARD_TAG  = nano
BOARD_SUB  = atmega328

GLOBAL_DEFINE += USE_SHIFTREG

# Sketch sources
COMMON = ../../common
LOCAL_INO_SRCS = main.ino
LOCAL_CPP_SRCS = \
	instructions.cpp \
	$(COMMON)/SerialTalks.cpp \
	$(COMMON)/CRC16.cpp \
	$(COMMON)/ShiftRegister.cpp \
	$(COMMON)/ShiftRegAX12.cpp \
	$(COMMON)/SoftwareSerial.cpp \
	$(COMMON)/StepByStepMotor.cpp \
	$(COMMON)/PeriodicProcess.cpp \
	$(COMMON)/Clock.cpp \
	$(COMMON)/RobotArm.cpp


# Sketch libraries
ARDUINO_LIBS = EEPROM Servo


#Define constant

CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x30
# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ../..
include $(MODULEMK_DIR)/Module.mk