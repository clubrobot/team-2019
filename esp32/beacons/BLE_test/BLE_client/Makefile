BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = BLE_client.ino

BOARD_UUID = BLEclient
COMMON = ../../../common

#Setup all libs used !
LIBS = 	$(ESP_LIBS)/BLE \
		$(COMMON)/mathutils.cpp 

# Define
CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x20
CPPFLAGS += -DPUREPURSUIT_MAX_WAYPOINTS=32
CPPFLAGS += -DCONFIG_CXX_EXCEPTIONS=1

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ../../..
CPP_EXTRA += -fexceptions
include $(MODULEMK_DIR)/Module.mk

