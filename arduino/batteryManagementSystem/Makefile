BOARD_UUID = BMS
BOARD_TAG  = nano
BOARD_SUB  = atmega328

MONITOR_PORT = /dev/cu.wchusbserial1410

# Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = bms/bms.ino
LOCAL_CPP_SRCS = \
	instructions.cpp \
	$(COMMON)/SerialTalks.cpp \
	$(COMMON)/CRC16.cpp
	#$(COMMON)/BMS.cpp 

# Sketch libraries
ARDUINO_LIBS = 

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk