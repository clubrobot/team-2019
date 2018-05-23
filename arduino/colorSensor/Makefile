BOARD_UUID = colorSensor
BOARD_TAG  = nano
BOARD_SUB  = atmega328

# Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = sensors.ino
LOCAL_CPP_SRCS = \
	instructions.cpp \
	$(COMMON)/SerialTalks.cpp \
	$(COMMON)/CRC16.cpp \
	$(COMMON)/Clock.cpp \
	$(COMMON)/Adafruit_TCS34725.cpp \
	$(COMMON)/Wire.cpp

LOCAL_C_SRCS = \
	$(COMMON)/twi.c



# Sketch libraries
ARDUINO_LIBS = EEPROM

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk