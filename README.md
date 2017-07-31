# GhostMessage

GhostMessage is fun toy that provides a way to send a message to a connected LED
Matrix from a web page.

# Requirements
GhostMessage runs on a NodeJS server, and requires a matrix to receive
data over a serial connection. The firmware utilizes the
[RGB Matrix Panel library](https://github.com/adafruit/RGB-matrix-Panel) and the
[Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library). You should
be able to install those through the Arduino IDE.

My Panel is backed with a Nootropic Design RGB LED Backpack. This firmware is
compatible with a regular Arduino Uno - you will need to swap the pin on line
14 with the correct Latch pin. For Arduino Unos, change the value to ```A3```.

# Installation
To install the dependencies for the Node server, from a command line run:

```
npm install
```

This will install the Serial library, and the supporting libraries for the
web socket and server.

# Running the Server
Run the server by typing in the command line:
```
node index.js /dev/serial-port-name web-server-port
```
Arguments are options, the serial port will default to ```/dev/ttyS0``` which is
a dummy port. You should use whatever serial connection your Serial device has
registered as. The web server port will defaul to ```8080```.

The default port is ```8080```. Once the server is running, you can go to
```http://localhost:8080``` and you should see the Ghost Message page with a
text box. If the Ghost is Red, Teal, Pink or Orange, you're in good shape. If the
ghost is is "dead" or the retreat colors as in Pacman - the websocket connection
is not active.
