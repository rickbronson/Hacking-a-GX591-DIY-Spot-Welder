  Hacking a GX591 DIY Spot Welder
======================================================================================

  I thought I'd try my luck on this spot welder but on the third weld sparks came out of the MOSFET's.  So I thought I'd see if I could put a program on the micro controller.  It has an STC8H1K08 micro on it which is a 8051 variant that is programmable from the 4 pin serial interface on the board.

![alt text](https://github.com/rickbronson/Hacking-a-GX591-DIY-Spot-Welder/blob/master/docs/hardware/gx591.png "orig board")

You will need one of the following:

 - A double throw switch to switch both Vcc and the TX line from the USB to serial converter.  If you don't switch the TX line it ends up powering the board and you can't do a power cycle to program the board.  You will be prompted when to toggle this switch.
 - A USB to serial converter hooked up according to:

![alt text](https://github.com/rickbronson/Hacking-a-GX591-DIY-Spot-Welder/blob/master/docs/hardware/board.jpg "my board")

  That's after I removed a bunch of components, including the fried MOSFET's.
	
  Here is a rough schematic of the important parts:

![alt text](https://github.com/rickbronson/Hacking-a-GX591-DIY-Spot-Welder/blob/master/docs/hardware/hookup9.png "hookup")

Steps for install:

 - On your Linux box do:

```
sudo apt-get install sdcc minicom
git clone https://github.com/IOsetting/stc8prog
make -C stc8prog
make
minicom -b 115200 -w -D /dev/ttyUSB0
```
