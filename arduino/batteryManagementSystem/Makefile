BOARD_UUID = BMS
BOARD_TAG  = nano
BOARD_SUB  = atmega328

# Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = bms.ino
LOCAL_CPP_SRCS = \
	BMS.cpp \
	instructions.cpp \
	$(COMMON)/SerialTalks.cpp \
	$(COMMON)/CRC16.cpp \
	$(COMMON)/Wire.cpp

# Sketch libraries
ARDUINO_LIBS = Wire EEPROM

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk
