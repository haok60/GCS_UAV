#include <SPI.h>
byte address = 0x11;
int CS1= 7;//throttle
int CS2= 8;//yaw
int CS3= 9;//pitch
int CS4= 10;//roll
String cmd;
void setup()
{
  //pos1=pos2=pos3=pos4=0x80;
  pinMode (CS1, OUTPUT);
  pinMode (CS2, OUTPUT);
  pinMode (CS3, OUTPUT);
  pinMode (CS4, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
  setToCenter();
//  digitalPotWrite4(0x00);
//  delay(500);
//  digitalPotWrite3(0x00);
//  delay(500);
//  digitalPotWrite2(0x00);
//  delay(500);
//  digitalPotWrite1(81);
}
void loop()
{
   //Serial.println("ok");
   while(Serial.available()>0){
    cmd = Serial.readString();
    //Serial.println("on");
   }
   //Serial.println(cmd.substring(0,5) == "right");
   //cat canh
   if(cmd.substring(0,7) == "connect"){
    Serial.println("connect");
    _connect();
   }
   if(cmd.substring(0,7) == "takeoff"){
    Serial.println("take off");
    _start();
   }
   if(cmd.substring(0,2) == "up"){
    Serial.println("up");
    liftUp();
   }
   if(cmd.substring(0,4) == "down"){
    Serial.println("down");
    liftDown();
   }
   if(cmd.substring(0,5) == "right"){
    Serial.println("turn right");
    turnRight();
   }
   if(cmd.substring(0,4) == "left"){
    Serial.println("turn left");
    turnLeft();
   }
   if(cmd.substring(0,5) == "front"){
    Serial.println("move forward");
    moveForward();
   }
   if(cmd.substring(0,4) == "back"){
    Serial.println("move backward");
    moveBack();
   }
   if(cmd.substring(0,4) == "stop"){
    Serial.println("stop");
    setToCenter();
   }
   if(cmd.substring(0,7) == "landoff"){
    Serial.println("land off");
    _stop();
   }
   if(cmd.substring(0,5) == "calib"){
    Serial.println("calib");
    calib();
   }
   cmd.remove(0);
   delay(400);
}
void setToCenter(){
  //digitalPotWrite1(125);//pitch
  digitalPotWrite1(84);//pitch
  digitalPotWrite2(77);//roll
  digitalPotWrite3(77);//throttle
  digitalPotWrite4(81);//yaw
}
void _connect(){
  digitalPotWrite3(157);
  delay(2000);
  digitalPotWrite3(77);
  delay(1000);
  digitalPotWrite3(0);
  delay(1000);
  digitalPotWrite3(77);
}
void _start()
{
  digitalPotWrite1(0);
  digitalPotWrite2(0);
  digitalPotWrite3(0);
  digitalPotWrite4(0);
  delay(3000);
  setToCenter();
  }
void liftUp(){
  digitalPotWrite3(120);
  //digitalPotWrite3(157);
  delay(3000);
  digitalPotWrite3(77);
}
void liftDown(){
  digitalPotWrite3(30);
//  digitalPotWrite3(0);
  delay(3000);
  digitalPotWrite3(77);
}
void turnRight(){
  digitalPotWrite2(120);
  delay(2000);
  digitalPotWrite2(77);
}
void turnLeft(){
  digitalPotWrite2(30);
  //digitalPotWrite2(0);
  delay(2000);
  digitalPotWrite2(77);
}
void moveForward(){
  digitalPotWrite1(120);
  //digitalPotWrite1(157);
  delay(3000);
  digitalPotWrite1(84);
}
void moveBack(){
  digitalPotWrite1(30);
  //digitalPotWrite1(0);
  delay(3000);
  digitalPotWrite1(84);
}
void calib(){
  digitalPotWrite1(157);
  digitalPotWrite2(157);
  digitalPotWrite3(157);
  digitalPotWrite4(157);
  delay(2000);
  setToCenter();
}
void _stop()
{
  setToCenter();
  delay(1000);
  digitalPotWrite3(0);
}

int digitalPotWrite1(int value)
{
  digitalWrite(CS1, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS1, HIGH);
}
int digitalPotWrite2(int value)
{
  digitalWrite(CS2, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS2, HIGH);
}
int digitalPotWrite3(int value)
{
  digitalWrite(CS3, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS3, HIGH);
}
int digitalPotWrite4(int value)
{
  digitalWrite(CS4, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS4, HIGH);
}
