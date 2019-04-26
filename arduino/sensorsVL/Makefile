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
	$(COMMON)/Wire.cpp			\
	lib/Adafruit_VL53L0X.cpp	\
	lib/core/src/vl53l0x_api_calibration.cpp			\
	lib/core/src/vl53l0x_api_core.cpp 					\
	lib/core/src/vl53l0x_api_ranging.cpp				\
	lib/core/src/vl53l0x_api_strings.cpp				\
	lib/core/src/vl53l0x_api.cpp						\
	lib/platform/src/vl53l0x_i2c_comms.cpp				\
	lib/platform/src/vl53l0x_platform.cpp

LOCAL_C_SRCS = \
	$(COMMON)/twi.c


# Sketch libraries
ARDUINO_LIBS = EEPROM

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk
