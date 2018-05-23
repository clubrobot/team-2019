BOARD_UUID = buttonCard
BOARD_TAG  = nano
BOARD_SUB  = atmega328

# Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = buttonCard.ino
LOCAL_CPP_SRCS = \
	$(COMMON)/SerialTalks.cpp \
	$(COMMON)/CRC16.cpp \
	$(COMMON)/Clock.cpp \
	$(COMMON)/ButtonCard.cpp \
	instructions.cpp\

LOCAL_H_SRCS = \
	$(COMMON)/ButtonCard.h

# Sketch libraries
ARDUINO_LIBS = EEPROM

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk

