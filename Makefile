all:
	sdcc -mmcs51 --std-c99 hello.c

prog:
	./stc8prog/stc8prog -p /dev/ttyUSB0 -e -f hello.ihx
#	~/venv/bin/stcgal -p /dev/ttyUSB0 -P stc8g hello.ihx
