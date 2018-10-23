BOARD_UUID = sensors
BOARD_TAG  = nano
BOARD_SUB  = atmega328

# Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = Sensor_IR.ino
LOCAL_CPP_SRCS = \
	instructions.cpp \
	Sensor_IR.cpp \
	Traitements.cpp \
	VL53L0X.cpp \
	VL6180X.cpp \
	$(COMMON)/SerialTalks.cpp \
	$(COMMON)/CRC16.cpp



# Sketch libraries
ARDUINO_LIBS = \
	EEPROM \
	Wire

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk
