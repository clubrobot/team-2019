BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = main.ino

BOARD_UUID = arm_front
COMMON = ../common
#Setup all libs used !
LIBS = $(ESP_LIBS)/EEPROM \
	   $(COMMON)/SerialTalks.cpp \
	   $(COMMON)/PeriodicProcess.cpp \
	   $(COMMON)/mathutils.cpp \
	   $(COMMON)/PID.cpp \
	   $(COMMON)/CRC16.cpp \
	   $(COMMON)/IK \
	   $(COMMON)/AX12 \
	   $(COMMON)/VacumPump.cpp \
	   $(COMMON)/TaskManager.cpp \
	   $(COMMON)/PressureSensor.cpp \
	   instructions.cpp


CPPFLAGS += -DCONFIG_CXX_EXCEPTIONS=1
CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x30
# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
CPP_EXTRA += -fexceptions
include $(MODULEMK_DIR)/Module.mk
