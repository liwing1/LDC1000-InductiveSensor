#include <SPI.h>
#include "LDC1000.h"


#define FREQ_PIN 9

uint32_t freq_init = 0;

void setup() {
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  
  Serial.begin(9600);
  delay(100);
  SPI.begin();

  clkConfig();
  ldcConfig();

  freq_init = readFreq();
  
}

void loop() {
  uint32_t freq = readFreq();
  Serial.print("READ REGISTER: ");
  Serial.println( freq );

  if( freq_init - freq > 10 )
    digitalWrite(8, HIGH);
  else
    digitalWrite(8, LOW);
   
  delay(500);
  printBits(readRegister(STTS_REG, 1));

}


uint32_t readRegister(uint8_t addr, uint8_t nBytes)
{
  uint32_t slaveOut = 0;
  uint32_t readValue = 0;
  
  digitalWrite(SS, LOW);
  
  SPI.transfer(RD|addr);
  readValue = SPI.transfer(0x00);

  for( uint8_t i = 1; i != nBytes; i++ )
  {
    slaveOut = SPI.transfer(0x00);
    readValue |= slaveOut<<(8*i);
  }
  
  digitalWrite(SS, HIGH);

  return readValue;
}



void writeRegister(uint8_t addr, uint8_t data)
{
  digitalWrite(SS, LOW);
  SPI.transfer(addr);
  SPI.transfer(data);
  digitalWrite(SS, HIGH);  
}

void ldcConfig( void ){
  writeRegister(POWR_REG, 0x00);

  writeRegister(LDCC_REG, 0x17);
  writeRegister(CLKC_REG, 0x00);
  
  writeRegister(RMAX_REG, 0x13); //0x13
  writeRegister(RMIN_REG, 0x3D); //0x3D
  
  writeRegister(WDTF_REG, 0x45);

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

void printBits(byte myByte){
  for(byte mask = 0x80; mask; mask >>= 1){
    if(mask  & myByte)
      Serial.print('1');
    else
      Serial.print('0');
  }
  Serial.println();
}

uint32_t readFreq( void )
{
  uint32_t out = 0;
  for(uint8_t i = 0; i<100; i++){
    out += readRegister(FQCL_REG, 3);
  }
  out /= 100;

  return out;
}
