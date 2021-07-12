/*
 * Sketch to configure a Arduino device (default is for an UNO) as an SPI slave
 * device. Handy for testing SPI output from another device or as the basis of 
 * test device.
 */
#include <SPI.h>

const int PIN_SCL = 13;   // Clock
const int PIN_MISO = 12;  // Master In/Slave Out
const int PIN_MOSI = 11;  // Master Out/Slave In
const int PIN_CS = 10;    // Chip Select

void setup() {
  Serial.begin(1000 * 1000);  // 1Mbps

  // Init SPI Pins
  pinMode(PIN_SCL, INPUT);
  pinMode(PIN_MISO, OUTPUT);
  pinMode(PIN_MOSI, INPUT);
  pinMode(PIN_CS, INPUT);
  
  // Enable SPI as Slave
  SPCR = (1 << SPE);

  Serial.write("\nSPI Slave Device Setup\n");
}

/**
 * Blocking read/write of a byte
 */
byte spiTransfer(byte value) {
  SPDR = value;
  while(!(SPSR & (1<<SPIF)));
  return SPDR;
}


void loop() {
  // Read a value and echo to serial port
  char value = spiTransfer(0);
  Serial.write(value);
}
