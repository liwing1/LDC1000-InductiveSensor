#include <SPI.h>
#include "LDC1000.h"


#define FREQ_PIN 9


void setup() {
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  
  Serial.begin(9600);
  SPI.begin();

  clkConfig();
  ldcConfig();
  
}

void loop() {
  Serial.print("READ REGISTER: 0x");
  uint32_t out = readRegister(PRXL_REG, 2);
  Serial.println(out, HEX);
  
  delay(5000);

}


uint32_t readRegister(uint8_t addr, uint8_t nBytes)
{
  uint32_t slaveOut = 0 ;
  
  digitalWrite(SS, LOW);
  
  SPI.transfer(RD|addr);
  slaveOut = SPI.transfer(0x00);

  nBytes--;
  while(nBytes)
  {
    slaveOut = ( slaveOut<<8 )  | SPI.transfer(0x00);
    nBytes--;
  }
  
  digitalWrite(SS, HIGH);

  return slaveOut;
}

void writeRegister(uint8_t addr, uint8_t data)
{
  digitalWrite(SS, LOW);
  SPI.transfer(addr);
  SPI.transfer(data);
  digitalWrite(SS, HIGH);  
}

void ldcConfig( void )
{
  writeRegister(RMAX_REG, 0x13);
  writeRegister(RMIN_REG, 0x3D);

  writeRegister(WDTF_REG, 0x93);
  writeRegister(LDCC_REG, 0x17);
  writeRegister(CLKC_REG, 0x00);

  writeRegister(CTHL_REG, 0x50);
  writeRegister(CTHM_REG, 0x14);
  writeRegister(CTLL_REG, 0xC0);
  writeRegister(CTLM_REG, 0x12);

  writeRegister(POWR_REG, 0x01);
}

void clkConfig( void )
{
  pinMode(FREQ_PIN, OUTPUT);
  TCCR1A = ( (1 << COM1A0));
  TCCR1B = ((1 << WGM12) | (1 << CS10));
  TIMSK1 = 0;
  OCR1A = 0;    //0-8MHz\1-4MHZ\3-2MHZ\7-1MHZ
}

float medeProx( void )
{
  uint32_t out = 0;
  float Y = 0;
  
  for (int i = 0; i < 100; i++){
    out  += readRegister(PRXL_REG, 2);  
  }
  out /= 100;

  Y = out/32768.0f;

  Y = (RP_MAX*RP_MIN)/( (RP_MIN*(1-Y)) + (RP_MAX*Y) );

  return Y;
}
