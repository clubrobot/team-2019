#!/bin/bash
if [ "$(uname -s)" = 'Linux' ]; then
	REPOSITORY=$(dirname $(readlink -f "$BASH_SOURCE"))
else
	REPOSITORY="$(cd "$(dirname "$(readlink "$BASH_SOURCE")")"; pwd)"
fi

PROFILE="$HOME/.profile"
BASHRC="$HOME/.bashrc"
# Download and install Aduino IDE if it is not already installed

if [ -z "$ARDUINO_DIR" ]; then # ifndef ARDUINO_DIR

	# Download Arduino IDE

	case $(uname -m) in
		i686	) OS=linux32;;
		x86_64	) OS=linux64;;
		arm*	) OS=linuxarm;;
		*		) exit 1;;
	esac

	if [ "$(uname -s)" = 'Linux' ]; then
		ARDUINO_URL=https://downloads.arduino.cc/arduino-1.6.12-$OS.tar.xz

		ARDUINO_TAR=$(basename "$ARDUINO_URL")
		wget "$ARDUINO_URL" -O "$ARDUINO_TAR"

		# Install Arduino IDE

		ARDUINO_SRC=$(tar -tf "$ARDUINO_TAR" | head -1)
		ARDUINO_SRC=${ARDUINO_SRC%/} # Remove trailing '/'
		tar -xvf "$ARDUINO_TAR"
		rm "$ARDUINO_TAR"
		sudo mv "$ARDUINO_SRC" /opt
		echo export ARDUINO_DIR="/opt/$ARDUINO_SRC" >> "$PROFILE"

		# Launch Arduino IDE and give it time to settle in

		"/opt/$ARDUINO_SRC/arduino" &
		sleep 10
		pkill -n java

	else

		ARDUINO_URL=https://downloads.arduino.cc/arduino-1.6.12-macosx.zip

		ARDUINO_TAR=$(basename "$ARDUINO_URL")
		wget "$ARDUINO_URL" -O "$ARDUINO_TAR"
		# Install Arduino IDE

		ARDUINO_APP=$(tar -tf "$ARDUINO_TAR" | head -1)
		ARDUINO_APP=${ARDUINO_APP%/} # Remove trailing '/'
		ARDUINO_SRC=${ARDUINO_APP%.app} # Remove '.app'
		tar -xvf "$ARDUINO_TAR"
		rm "$ARDUINO_TAR"
		sudo mv "$ARDUINO_APP" /Applications
		echo export ARDUINO_DIR="/Applications/$ARDUINO_SRC" >> "$PROFILE"

		# Launch Arduino IDE and give it time to settle in

		open -a "/Applications/$ARDUINO_APP" &
		sleep 10
		pkill -n Arduino
	fi
fi # ifndef ARDUINO_DIR

# Install ESP-32 driver for Arduino IDE
if [ "$(uname -s)" = 'Linux' ]; then
	mkdir "/opt/$ARDUINO_SRC/hardware/espressif"
	cd  "/opt/$ARDUINO_SRC/hardware/espressif"
	git clone https://github.com/espressif/arduino-esp32.git esp32
	cd esp32
	git submodule update --init --recursive && \
	cd tools
	python get.py

	echo export ESP_ROOT="/opt/$ARDUINO_SRC/hardware/espressif/esp32" >> "$PROFILE"
	echo export PYTHONPATH="$REPOSITORY/raspberrypi/:\$PYTHONPATH" >> "$BASHRC"

else
	mkdir -p ~/Documents/Arduino/hardware/espressif && \
	cd ~/Documents/Arduino/hardware/espressif && \
	git clone https://github.com/espressif/arduino-esp32.git esp32 && \
	cd esp32 && \
	git submodule update --init --recursive && \
	cd tools && \
	python get.py

	echo export ESP_ROOT="~/Documents/Arduino/hardware/espressif/esp32" >> "$PROFILE"
	echo export PYTHONPATH="$REPOSITORY/raspberrypi/:\$PYTHONPATH" >> "$BASHRC"

	#install esptool.py to correct mac os esp32 error
	pip install esptool

	sudo mv $ESP_ROOT/tools/esptool.py $ESP_ROOT/tools/esptool/esptool.py
fi

# Install Arduino-Makefile if it is not already installed
if [ -z "$ARDMK_DIR" ]; then # ifndef ARDMK_DIR
	if [ "$(uname -s)" = 'Linux' ]; then
		sudo apt-get install arduino-mk

		sudo apt-get install python-serial
		
		sudo apt-get install python3-serial
		
		sudo apt-get install libigraph0-dev
		
		sudo apt-get install python3-pip
		
		pip3 install python-igraph
		
		sudo pip3 install python-igraph

		echo export ARDMK_DIR="/usr/share/arduino" >> "$PROFILE"
	else
		# installing homebrew for macos
		#/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
		# add tap
		brew tap sudar/arduino-mk
		# to install the last stable release
		brew install arduino-mk

		echo export ARDMK_DIR="/usr/local/opt/arduino-mk" >> "$PROFILE"

		sudo sed -i "" "s_/dev/tty.usbserial*__g" "$ARDMK_DIR/Arduino.mk"

		sudo easy_install pip

		pip install pyserial

		pip3 install pyserial

		pip3 install python-igraph
	fi

fi

if [ "$(uname -s)" = 'Linux' ]; then
	# Add the current user to the dialout group
	sudo usermod -a -G dialout $USER

	# Install udev rules

	PYTHONPATH=$(python3 -m site --user-site)

	UDEVRULES_DIRECTORY=/etc/udev/rules.d
	UDEVRULE='KERNEL=="ttyUSB*", PROGRAM="/usr/bin/env PATH='"$PATH"' PYTHONPATH='"$PYTHONPATH:$REPOSITORY/raspberrypi/"' '"$REPOSITORY/raspberrypi/robot getuuid"' /dev/%k", SYMLINK+="arduino/%c" \nKERNEL=="ttyACM*" , PROGRAM="/usr/bin/env PATH='"$PATH"' PYTHONPATH='"$PYTHONPATH:$REPOSITORY/raspberrypi/"' '"$REPOSITORY/raspberrypi/robot getuuid"' /dev/%k", SYMLINK+="arduino/%c" '

	echo -e $UDEVRULE | sudo tee "$UDEVRULES_DIRECTORY/serialtalks.rules" > /dev/null
	sudo udevadm control --reload-rules
else
	git clone https://github.com/clubrobot/SerialRenamerDaemon.git
	cd SerialRenamerDaemon
	sudo make install
fi

# Add the robot command to the user path if it is not already in it
if [ "$(uname -s)" = 'Linux' ]; then
	if [[ ":$PATH:" != *":$REPOSITORY/raspberrypi:"* ]]; then # if not **/team-2017/raspberrypi in PATH
		echo export PATH="$REPOSITORY/raspberrypi:\$PATH" >> "$PROFILE"
	fi
else
	if [[ "$(cat /etc/paths)" != *":$REPOSITORY/raspberrypi:"* ]]; then
		echo $REPOSITORY/raspberrypi | sudo tee -a /etc/paths
		sudo launchctl unload /Library/LaunchDaemons/fr.insa-rennes.clubrobot.serialrenamerd.plist
		sudo sed -i "" "s_REPOSITORY_$REPOSITORY/raspberrypi_g" /Library/LaunchDaemons/fr.insa-rennes.clubrobot.serialrenamerd.plist
		sudo launchctl load /Library/LaunchDaemons/fr.insa-rennes.clubrobot.serialrenamerd.plist
	fi
fi

# A reboot is required to source the .profile file
echo "My work here is done. Now it is time for me to retire."
echo "Please reboot your computer to take the changes into account"
