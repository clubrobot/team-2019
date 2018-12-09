BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = anchor.ino

BOARD_UUID = anchor
COMMON = ../../common
DWM1000_LIB = ../../common/DWM1000
SKETCH_LIBS = ~/Arduino/libraries/ESP8266_and_ESP32_Oled_Driver_for_SSD1306_display/src
BEACONS_COMMON = ../common

#Setup all libs used !
LIBS = 	$(ESP_LIBS)/SPI \
		$(ESP_LIBS)/EEPROM \
		$(ESP_LIBS)/Wire \
		$(ESP_LIBS)/BLE \
	    $(COMMON)/SerialTalks.cpp \
		$(COMMON)/mathutils.cpp \
		$(COMMON)/CRC16.cpp\
		$(DWM1000_LIB)/DW1000Device.cpp\
		$(DWM1000_LIB)/DW1000.cpp\
		$(DWM1000_LIB)/DW1000Ranging.cpp\
		$(DWM1000_LIB)/DW1000Time.cpp\
		$(DWM1000_LIB)/DW1000Mac.cpp\
		$(SKETCH_LIBS)/OLEDDisplay.cpp\
		$(BEACONS_COMMON)/OLED_display.cpp\
		instructions.cpp 

# Define
CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x20
CPPFLAGS += -DPUREPURSUIT_MAX_WAYPOINTS=32
CPPFLAGS += -DCONFIG_CXX_EXCEPTIONS=1

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ../..
CPP_EXTRA += -fexceptions
include $(MODULEMK_DIR)/Module.mk

