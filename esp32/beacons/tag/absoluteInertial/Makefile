BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = absoluteInertial.ino

BOARD_UUID = bno
COMMON = ../../../common
#Setup all libs used !
LIBS = $(ESP_LIBS)/EEPROM \
	   $(COMMON)/SerialTalks.cpp \
	   $(COMMON)/CRC16.cpp \
	   $(COMMON)/PeriodicProcess.cpp \
	   $(ESP_LIBS)/Wire \
	   $(COMMON)/BNO055.cpp \
	   AccelerationController.cpp \
	   instructions.cpp


CPPFLAGS = -fexceptions
CPPFLAGS += -DCONFIG_CXX_EXCEPTIONS=1
CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x26
# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ../../..
CPP_EXTRA += -fexceptions
include $(MODULEMK_DIR)/Module.mk

