BOARD_UUID = Servo_testu
BOARD_TAG  = nano
BOARD_SUB  = atmega328

# Sketch sources
COMMON = ../../common
LOCAL_INO_SRCS = main.ino
LOCAL_CPP_SRCS = \
	instructions.cpp \
	$(COMMON)/CRC16.cpp \
	$(COMMON)/SerialTalks.cpp

# Sketch libraries
ARDUINO_LIBS = EEPROM Servo


#Define constant

CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x30

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ../..
include $(MODULEMK_DIR)/Module.mk