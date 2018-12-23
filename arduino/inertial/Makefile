BOARD_UUID = inertial
BOARD_TAG  = nano
BOARD_SUB  = atmega328

# Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = inertial.ino
LOCAL_CPP_SRCS = \
	instructions.cpp \
	AccelerationController.cpp \
	$(COMMON)/SerialTalks.cpp \
	$(COMMON)/CRC16.cpp \
	$(COMMON)/Clock.cpp \
	$(COMMON)/LSM303.cpp \
	$(COMMON)/MagneticCompas.cpp \
	$(COMMON)/Wire.cpp \
	$(COMMON)/PeriodicProcess.cpp

LOCAL_C_SRCS = \
	$(COMMON)/twi.c



# Sketch libraries
ARDUINO_LIBS = EEPROM

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk