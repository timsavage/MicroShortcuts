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

// Incoming buffer
byte buffer [100];
volatile byte pos;

void setup() {
  Serial.begin(115200);

  // Init SPI Pins
  pinMode(PIN_SCL, INPUT);
  pinMode(PIN_MISO, OUTPUT);
  pinMode(PIN_MOSI, INPUT);
  pinMode(PIN_CS, INPUT);
  
  // Enable SPI as Slave
  SPCR |= bit(SPE);

  // Enable interrupts
  SPI.attachInterrupt();

  Serial.write("\nSPI Slave Device Setup\n");
}

ISR(SPI_STC_vect)
{
  byte in = SPDR;

  // Check for space in the buffer
  if (pos < (sizeof(buffer) - 1)) {
    buffer[pos++] = in;
  }
}

void loop() {
  // If there is data in the buffer write it to the serial port
  if (pos) {
    Serial.write(buffer, pos);
    pos = 0;
  }
}
