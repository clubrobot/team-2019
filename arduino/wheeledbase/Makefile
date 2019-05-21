# Board properties
BOARD_UUID = wheeledbase
BOARD_TAG  = nano
BOARD_SUB  = atmega328

# Sketch sources
COMMON = ../common
LOCAL_INO_SRCS = wheeledbase.ino
LOCAL_CPP_SRCS = \
	instructions.cpp \
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
	$(COMMON)/mathutils.cpp

# Define
CPPFLAGS += -DPUREPURSUIT_MAX_WAYPOINTS=32
CPPFLAGS += -DSERIALTALKS_MAX_OPCODE=0x30

# Sketch libraries
ARDUINO_LIBS = EEPROM

# Congratulations! You made a pretty Makefile :)
# Now let the grown-ups do the hard work :D
MODULEMK_DIR = ..
include $(MODULEMK_DIR)/Module.mk
