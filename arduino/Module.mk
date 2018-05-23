# Expand the sources path so that the Arduino.mk will be build them in the correct directory
ifdef LOCAL_C_SRCS
	LOCAL_C_SRCS := $(realpath $(LOCAL_C_SRCS))
endif
ifdef LOCAL_CPP_SRCS
	LOCAL_CPP_SRCS := $(realpath $(LOCAL_CPP_SRCS))
endif
ifdef LOCAL_CC_SRCS
	LOCAL_CC_SRCS := $(realpath $(LOCAL_CC_SRCS))
endif
ifdef LOCAL_PDE_SRCS
	LOCAL_PDE_SRCS := $(realpath $(LOCAL_PDE_SRCS))
endif
ifdef LOCAL_INO_SRCS
	LOCAL_INO_SRCS := $(realpath $(LOCAL_INO_SRCS))
endif
ifdef LOCAL_AS_SRCS
	LOCAL_AS_SRCS := $(realpath $(LOCAL_AS_SRCS))
endif

# Define the BOARD_UUID preprocessor variable
ifdef BOARD_UUID
    CPPFLAGS += -DBOARD_UUID=\"$(BOARD_UUID)\"
endif

# Guess MONITOR_PORT if not specified
ifndef MONITOR_PORT
    MONITOR_PORT = /dev/arduino/$(BOARD_UUID)
    ifeq ($(shell test -e "$(MONITOR_PORT)"; echo $$?),1)
        undefine MONITOR_PORT # Let Arduino.mk find it by itself
    endif
endif

# Create alternative upload. Abort if the arduino with the right uuid is not finded
ifdef MONITOR_PORT
upload_safe:
	$(MAKE) upload
endif

ifndef MONITOR_PORT
upload_safe:
	@echo $(BOARD_UUID) not connected
endif


# Here is where the REAL grown-ups come into play ;)
# Seriously they did an awesome work. Thank you guys!
include $(ARDMK_DIR)/Arduino.mk
