BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = test_balises.ino

BOARD_UUID = beacon
COMMON = ../common
DWM1000_LIB = ../common/DWM1000
#Setup all libs used !
LIBS =  $(ESP_LIBS)/EEPROM \
		$(ESP_LIBS)/SPI \
	    $(COMMON)/SerialTalks.cpp \
	    $(COMMON)/CRC16.cpp \
		$(COMMON)/mathutils.cpp \
		$(DWM1000_LIB)/DW1000Device.cpp\
		$(DWM1000_LIB)/DW1000.cpp\
		$(DWM1000_LIB)/DW1000Ranging.cpp\
		$(DWM1000_LIB)/DW1000Time.cpp\
		$(DWM1000_LIB)/DW1000Mac.cpp
		#instructions.cpp 

# Define
CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x20
CPPFLAGS += -DPUREPURSUIT_MAX_WAYPOINTS=32


# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk

