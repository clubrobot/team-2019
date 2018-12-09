How to decode ESP32 traceback :

1/ copy and paste the traceback in the file dump.txt : for example :

Guru Meditation Error: Core  1 panic'ed (StoreProhibited). Exception was unhandled.
Register dump:
PC      : 0x400d25f2  PS      : 0x00060730  A0      : 0x800d56c6  A1      : 0x3ffcae20  
A2      : 0x00000000  A3      : 0x0000001e  A4      : 0x0000000a  A5      : 0x3ffc274a  
A6      : 0x00060320  A7      : 0x3ffc8fe0  A8      : 0x800d25f2  A9      : 0x3ffcae00  
A10     : 0x00000e1d  A11     : 0x3f403f55  A12     : 0x0000000a  A13     : 0x00000002  
A14     : 0x3ffc2696  A15     : 0x00000001  SAR     : 0x00000006  EXCCAUSE: 0x0000001d  
EXCVADDR: 0x0000006c  LBEG    : 0x400014fd  LEND    : 0x4000150d  LCOUNT  : 0xfffffff8  

Backtrace: 0x400d25f2:0x3ffcae20 0x400d56c3:0x3ffcae40 0x400d0f85:0x3ffcaf20 0x400e7391:0x3ffcaf40


2/ go in the folder containing EspStackTraceDecoder.jar

3/ run the command java -jar EspStackTraceDecoder.jar /opt/arduino-1.6.12/hardware/espressif/esp32/tools/xtensa-esp32-elf/bin/xtensa-esp32-elf-addr2line <.ELF file> <dump file>

where 	<.ELF file> is the compiled file (for example /tmp/mkESP/tag_esp32/tag.elf)
	<dump file> is the file containing the backtrace (for example dump.txt)
