BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = panel.ino

BOARD_UUID = esp32
COMMON = ../common
#Setup all libs used !
LIBS = $(ESP_LIBS)/EEPROM \
	   $(ESP_LIBS)/BLE \
	   $(COMMON)/SerialTalks.cpp \
	   $(COMMON)/CRC16.cpp \
	   $(COMMON)/fastLED/FastLED.cpp \
	   $(COMMON)/fastLED/bitswap.cpp \
	   $(COMMON)/fastLED/colorpalettes.cpp \
	   $(COMMON)/fastLED/colorutils.cpp \
	   $(COMMON)/fastLED/hsv2rgb.cpp \
	   $(COMMON)/fastLED/lib8tion.cpp \
	   $(COMMON)/fastLED/noise.cpp \
	   $(COMMON)/fastLED/power_mgt.cpp \
	   $(COMMON)/fastLED/wiring.cpp \
	   $(COMMON)/WiFi/src/WiFi.cpp \
	   $(COMMON)/WiFi/src/ETH.cpp \
	   $(COMMON)/WiFi/src/WiFiAP.cpp \
	   $(COMMON)/WiFi/src/WiFiClient.cpp \
	   $(COMMON)/WiFi/src/WiFiGeneric.cpp \
	   $(COMMON)/WiFi/src/WiFiMulti.cpp \
	   $(COMMON)/WiFi/src/WiFiSTA.cpp \
	   $(COMMON)/WiFi/src/WiFiScan.cpp \
	   $(COMMON)/WiFi/src/WiFiServer.cpp \
	   $(COMMON)/WiFi/src/WiFiUdp.cpp \
	   $(COMMON)/PannelEffects.cpp \
	   $(COMMON)/Pickle.cpp \
	   $(COMMON)/tcptalks.cpp \
	   instructions.cpp


CPPFLAGS = -fexceptions
CPPFLAGS += -DCONFIG_CXX_EXCEPTIONS=1
# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
CPP_EXTRA += -fexceptions
include $(MODULEMK_DIR)/Module.mk

