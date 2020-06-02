#define NOP() __asm__ __volatile__("nop")
#define SCK 4
#define CS 3
#define DATA A1


#define CS_on PORTD |= _BV(3)  // PD3
#define CS_off PORTD &= ~_BV(3)// PD3
#define SCK_on PORTD |= _BV(4)   // PD4
#define SCK_off PORTD &= ~_BV(4) // PD4
#define DATA_on PORTC |= _BV(1)    // PC1
#define DATA_off PORTC &= ~_BV(1)  // PC1
#define DATA_IN PINC & _BV(1)

#define CS_IN PIND & _BV(3)
#define SCK_IN PIND & _BV(4)
uint32_t payload = 0;
int a;
void setup() {
  DDRD = 0b00011000;
//  DDRD = 0b00000000;
//  DDRC = 0b00000000;
  Serial.begin(9600);
  initHW();
  openReadingPipe0();
  startListening();
  delay(2000);
  //configRX();
  Serial.println("ok");
}
void loop() {  
//  while(CS_IN != 0){
//    a = SCK_IN;
//    if(a==0 && SCK_IN){
//      Serial.print("a");
//      a = SCK_IN;
//    }
//  }
    uint32_t _status = readRegister(0x07,1);
    Serial.println(_status,HEX);
//    while(_status & 0x40){
//      Serial.println("reading data");
//      payload = readDataPayload();
//      Serial.println(payload,HEX);
//    }
  delay(1000);
}

void initHW(){
  CS_on;
  SCK_off;
  writeRegister(0x39,0x01,1); //DEMOD_CAL
  writeRegister(0x3A,0x465A2CEF2147,6); //RF_CAL2
  writeRegister(0x3B,0x02DF0B,3); //DEM_CAL2
  writeRegister(0x3E,0x5D3FF6,3); //RF_CAL
  writeRegister(0x3F,0x469C67ED12,5); //BB_CAL
  
  writeRegister(0xFC,0x00,1);// CE off
  writeRegister(0x20,0x04,1); //config
  writeRegister(0x21,0x00,1); // EN_AA
  writeRegister(0x24,0x00,1); //setup re_tr
  writeRegister(0x26,0x27,1); //rf_setup
  //writeRegister(0x50,0x73,1); //active
  writeRegister(0x3D,0x20,1); // FEATURE
  writeRegister(0x3C,0x00,1); //dynamic payload DYNPD
  //writeRegister(0x27,0x70,1); // status reg
  writeRegister(0x25,0x4E,1); // RFchannel
  FlushRX();
  FlushTX();
  powerUp();
  writeRegister(0x20,readRegister(0x00,1) | _BV(0),1); //PRIM_RX
  
}

void openReadingPipe0(){
   writeRegister(0x2B,0x2B01CA055A,5);//set RX address
   writeRegister(0x32,0x08,1); // bytes in RX payload
   writeRegister(0x22,0x03,1); // EN_RXADDR en data pipe
}
void startListening(){
  powerUp();
  writeRegister(0x20,readRegister(0x00,1) | _BV(0),1);
  writeRegister(0x27,0x70,1); // status reg
  writeRegister(0xFD,0x00,1); //CE on
}
uint32_t readDataPayload(){
  CS_off;
  write_byte(0x61);
  uint32_t data = readRegister(0x61,4);
  CS_on;
  return data;
}
void writeRegister(int address, uint64_t data, int bytesToWrite){ //LS byte first
  
  CS_off;
  int i = 8;
  write_byte(address);
//  delay(3);
  write_byte(data& 0xFF);
//  delay(3);
  bytesToWrite--;
  while(bytesToWrite--){
    write_byte((data>>i)& 0xFF);
    i+=8;
  }

  SCK_off;
  CS_on;
//  write_byte(data);
}
uint32_t readRegister(int address,int bytesToRead){ //LS byte first
  CS_on;
  CS_off;
  uint32_t result = 0;
  int inByte;
  write_byte(address);
  result = read_Byte();
  bytesToRead--;
   //Serial.println(result,HEX);
  while(bytesToRead--){
    result = result << 8;
    inByte = read_Byte();
    result = result | inByte;
  }
  CS_on;
  return result;
  
}
void write_byte(int data){
  int n =8;
  DDRC = 0b00000010;
  while(n--){
    SCK_off;
    NOP();
    if(data & 0x80)
        DATA_on;
    else {
        DATA_off;
    }
    SCK_on;
    NOP();
    data = data << 1;
  }
}
int read_Byte(){
  int result = 0;
  DDRC = 0b00000000;
  for(int i = 0; i < 8; i++){
    SCK_off;
    NOP();
    SCK_on;
    NOP();
    if(DATA_IN){
      result = (result<<1)|0x01;
    }
    else result = result<<1;
  }
  return result;
}
void FlushRX(){
  CS_off;
  write_byte(0xE2);
  CS_on;
}
void FlushTX(){
  CS_off;
  write_byte(0xE1);
  CS_on;
}
void powerUp(){
  uint32_t a = readRegister(0x00,1);
  if(!(a & _BV(1))){
    writeRegister(0x20,a | _BV(1),1);
  }
}
void configRX(){
  //normal mode
  writeRegister(0x53,0xA5,1); //reset
  writeRegister(0x2A,0x2B01CA055A,5);//set RX address
  FlushRX();
  writeRegister(0x27,0x70,1); //STATUS reg clear data ready
  
  writeRegister(0x21,0x00,1); // EN_AA
  writeRegister(0x22,0x01,1); // EN_RXADDR en data pipe
  //writeRegister(0x25,0xFF,1); // RFchannel
  writeRegister(0x26,0x27,1); // data rate, power
  
  writeRegister(0x50,0x73,1); // activate
  
   writeRegister(0x24,0x00,1); // SETUP_RETR
  //writeRegister(0x30,0x2B01CA055A,5); //set TX addres
  writeRegister(0x3C,0x00,1); //en dynamic payload DYNPD
  writeRegister(0x3D,0x20,1); // FEATURE
  writeRegister(0x31,0x08,1); // bytes in RX payload
  
  writeRegister(0x39,0x01,1); //DEMOD_CAL
  writeRegister(0x3A,0x465A2CEF2147,6); //RF_CAL2
  writeRegister(0x3B,0x02DF0B,3); //DEM_CAL2
  writeRegister(0x3E,0x5D3FF6,3); //RF_CAL
  writeRegister(0x3F,0x469C67ED12,5); //BB_CAL
  FlushRX();
  writeRegister(0x20,0xFF,1); // configured
  writeRegister(0xFD,0x00,1); //CE on
}
