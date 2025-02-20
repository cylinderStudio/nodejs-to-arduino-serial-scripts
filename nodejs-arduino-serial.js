// If using ES modules
// import { SerialPort } from 'serialport'

// Or using require
const { SerialPort } = require('serialport')

const port = new SerialPort('/dev/cu.usbmodem1421', {
  baudRate: 57600,    // 9600 is the default
  dataBits: 8,        // these are the other defaults for Arduino serial communication
  parity: 'none',
  stopBits: 1,
  flowControl: false,
});

port.on('open', () => {
  console.log('Serial port open');

  port.on('data', data => {
    console.log(`data received: ${data}`);
  });

  // Arduino auto-resets when the serial port is opened,
  // so allow some time before sending data
  setTimeout(write, 2000);

  port.on('error', err => {
    console.error('Error:', err);
  });
});

const write = () => {
  port.write('Red', (err, results) => {
    if (err) {
      console.error(`Error writing: ${err}`);
    } else {
      console.log(`Write results: ${results}`);
    }
  });
};
