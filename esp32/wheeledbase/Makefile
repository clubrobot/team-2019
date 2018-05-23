BOARD = esp32
CHIP = esp32

#Arduino file
SKETCH = wheeledbase.ino

BOARD_UUID = wheeledbase
COMMON = ../common
#Setup all libs used !
LIBS =  $(ESP_LIBS)/EEPROM \
	    $(COMMON)/SerialTalks.cpp \
	    $(COMMON)/CRC16.cpp \
		$(COMMON)/DCMotor.cpp \
		$(COMMON)/Codewheel.cpp \
		$(COMMON)/PeriodicProcess.cpp \
		$(COMMON)/Odometry.cpp \
		$(COMMON)/PID.cpp \
		$(COMMON)/DifferentialController.cpp \
		$(COMMON)/VelocityController.cpp \
		$(COMMON)/PositionController.cpp \
		$(COMMON)/PurePursuit.cpp \
		$(COMMON)/TurnOnTheSpot.cpp \
		$(COMMON)/mathutils.cpp \
		instructions.cpp 
# Define
CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x20
CPPFLAGS += -DPUREPURSUIT_MAX_WAYPOINTS=32


# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk

