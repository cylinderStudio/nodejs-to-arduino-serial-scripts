# nodejs-to-arduino-serial-scripts
 A series of scripts I use as the basis for projects I build with Node.js signalling Arduino microcontrollers over a serial port connection. 

### Node.js

**nodejs-arduino-serial.js**
Basic node script to establish the serial port connection and establish listeners. [Node SerialPort](https://serialport.io/) is the only dependency.

### Arduino

**simple-serial-receiver.ino**
Very basic sketch making the serial connection and listening for commands.

**neopixel-strip.ino**
Sketch that receives commands to run a set of light animations on a strip of LED "neopixel" lights.

**neopixel-strip-ring.ino**
Same as the above sketch with a different set of animations that run on a larger ring of neopixels.

**neopixel-strip-voice-chrome.ino**
This sketch contains light animations to mimic the voice chrome on an Amazon Echo device.

