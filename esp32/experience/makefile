BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = experience.ino

BOARD_UUID = electron
COMMON = ../common
#Setup all libs used !
LIBS = $(ESP_LIBS)/EEPROM \
	   $(ESP_LIBS)/BLE \
	   $(COMMON)/SerialTalks.cpp \
	   $(COMMON)/CRC16.cpp \
	   $(COMMON)/WiFi/src/WiFi.cpp \
	   $(COMMON)/WiFi/src/ETH.cpp \
	   $(COMMON)/WiFi/src/WiFiClient.cpp \
	   $(COMMON)/WiFi/src/WiFiGeneric.cpp \
	   $(COMMON)/WiFi/src/WiFiMulti.cpp \
	   $(COMMON)/WiFi/src/WiFiSTA.cpp \
	   $(COMMON)/WiFi/src/WiFiScan.cpp \
	   $(COMMON)/WiFi/src/WiFiServer.cpp \
	   $(COMMON)/WiFi/src/WiFiUdp.cpp \
	   $(COMMON)/ExperienceEffects.cpp \
	   $(COMMON)/Pickle.cpp \
	   $(COMMON)/tcptalks.cpp \
	   $(COMMON)/NeoPixel/Adafruit_NeoPixel.cpp\
	   instructions.cpp


CPPFLAGS = -fexceptions
CPPFLAGS += -DCONFIG_CXX_EXCEPTIONS=1
# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
CPP_EXTRA += -fexceptions
include $(MODULEMK_DIR)/Module.mk
