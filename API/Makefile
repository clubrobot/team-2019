all: arduino python esp32

arduino: ../arduino/common/* ./index.md ../arduino/Doxyfile
	(cd ../arduino && doxygen)

python: ../raspberrypi/common/* ../raspberrypi/robots/* ../raspberrypi/arduino/* ./index.md ../raspberrypi/Doxyfile
	(cd ../raspberrypi && doxygen)


esp32:  ../esp32/common/* ./index.md ../esp32/Doxyfile
	(cd ../esp32 && doxygen)

clean:
	rm -r ./arduino
	rm -r ./python