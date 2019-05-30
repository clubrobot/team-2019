BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = experience.ino

BOARD_UUID = experience
COMMON = ../common
#Setup all libs used !
LIBS =  $(ESP_LIBS)/EEPROM \
		$(COMMON)/BLE \
		$(COMMON)/CRC16.cpp \
		$(COMMON)/ExperienceEffects.cpp \
		$(COMMON)/PeriodicProcess.cpp \
		$(COMMON)/Experience.cpp \
		$(COMMON)/ESP32-Arduino-Servo-Library/src/Servo.cpp \
		$(COMMON)/fastLED/FastLED.cpp \
		$(COMMON)/fastLED/bitswap.cpp \
		$(COMMON)/fastLED/colorpalettes.cpp \
		$(COMMON)/fastLED/colorutils.cpp \
		$(COMMON)/fastLED/hsv2rgb.cpp \
		$(COMMON)/fastLED/lib8tion.cpp \
		$(COMMON)/fastLED/noise.cpp \
		$(COMMON)/fastLED/power_mgt.cpp \
		$(COMMON)/fastLED/wiring.cpp \
		$(COMMON)/NeoPixel/Adafruit_NeoPixel.cpp\
		$(COMMON)/TaskManager.cpp \
		$(COMMON)/DCMotor.cpp\


CPPFLAGS = -fexceptions
CPPFLAGS += -DCONFIG_CXX_EXCEPTIONS=1
# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
CPP_EXTRA += -fexceptions
include $(MODULEMK_DIR)/Module.mk