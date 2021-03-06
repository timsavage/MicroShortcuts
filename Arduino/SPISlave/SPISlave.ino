/*
 * Sketch to configure a Arduino device (default is for an UNO) as an SPI slave
 * device. Handy for testing SPI output from another device or as the basis of 
 * test device.
 *
 * Data is written to the serial device when the CS flag is pulled high.
 */
#include <SPI.h>

const int PIN_SCL = 13;   // Clock
const int PIN_MISO = 12;  // Master In/Slave Out
const int PIN_MOSI = 11;  // Master Out/Slave In
const int PIN_CS = 2;     // Chip Select

// Incoming buffer
byte buffer [100];
volatile byte pos;
volatile bool proc;

void chipSelectISR() 
{
  pinMode(PIN_MISO, OUTPUT);
}

void chipDeselectISR() 
{
  proc = true;
  pinMode(PIN_MISO, INPUT);
}

ISR(SPI_STC_vect)
{
  byte in = SPDR;

  // Check for space in the buffer
  if (pos < (sizeof(buffer) - 1)) {
    buffer[pos++] = in;
  }
}

void setup() 
{
  Serial.begin(115200);

  // Init SPI Pins
  pinMode(PIN_SCL, INPUT);
  pinMode(PIN_MISO, INPUT);
  pinMode(PIN_MOSI, INPUT);
  pinMode(PIN_CS, INPUT);
  
  // Enable SPI as Slave
  SPCR |= bit(SPE);

  pos = 0;
  proc = false;

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(PIN_CS), chipSelectISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_CS), chipDeselectISR, RISING);
  SPI.attachInterrupt();

  Serial.write("\nSPI Slave Device Setup\n");
}

void loop() {
  // If there is data in the buffer write it to the serial port
  if (proc) {
    Serial.write(buffer, pos);
    pos = 0;
    proc = false;
  }
}
