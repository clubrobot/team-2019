# Board properties
BOARD_UUID = display
BOARD_TAG  = nano
BOARD_SUB  = atmega328

# Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = display.ino
LOCAL_CPP_SRCS = \
	$(COMMON)/SerialTalks.cpp \
	$(COMMON)/CRC16.cpp \
	$(COMMON)/PeriodicProcess.cpp \
	IPDisplay.cpp \
	instructions.cpp \
	ledMatrix.cpp \
	eepromManagment.cpp \

# Sketch libraries
ARDUINO_LIBS = EEPROM

# Misc
CPPFLAGS += -std=c++11

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk
