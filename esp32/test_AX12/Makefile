BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = main.ino

BOARD_UUID = esp32
COMMON = ../common
#Setup all libs used !
LIBS = $(ESP_LIBS)/EEPROM \
	   $(COMMON)/SerialTalks.cpp \
	   $(COMMON)/CRC16.cpp \
	   $(COMMON)/IK \
	   $(COMMON)/ShiftRegister.cpp \
	   $(COMMON)/ShiftRegAX12.cpp \
	   $(COMMON)/new_AX12.cpp 



CPPFLAGS = -fexceptions
CPPFLAGS += -DCONFIG_CXX_EXCEPTIONS=1
# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
CPP_EXTRA += -fexceptions
include $(MODULEMK_DIR)/Module.mk

