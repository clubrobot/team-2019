BOARD_UUID = sensorsA
#BOARD_UUID = sensorsB
#BOARD_UUID = sensorsC
#BOARD_UUID = sensorsD
BOARD_TAG  = nano
BOARD_SUB  = atmega328

# Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = sensorsVL.ino
LOCAL_CPP_SRCS = 				\
	instructions.cpp 			\
	$(COMMON)/SerialTalks.cpp   \
	$(COMMON)/CRC16.cpp			\
	lib/Pololulu/VL53L0X/VL53L0X.cpp \
	lib/Pololulu/VL6180X/VL6180X.cpp

LOCAL_C_SRCS = \
	$(COMMON)/twi.c


# Sketch libraries
ARDUINO_LIBS = EEPROM Wire

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk
