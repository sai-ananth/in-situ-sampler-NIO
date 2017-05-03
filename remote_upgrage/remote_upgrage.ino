/* LCD pin configuration
 * LCD RS pin to digital pin 19 (28)
 * LCD Enable pin to digital pin 18 (27)
 * LCD D4 pin to digital pin 17(26)
 * LCD D5 pin to digital pin 16 (25)
 * LCD D6 pin to digital pin 15 (24)
 * LCD D7 pin to digital pin 14  (23)
 * 
 * Keypad columns 3,4,5
 * Keypad Rows 6,7,8,9
 * 
 * rf12 pin configuuration
 * SDI - digital  11
 * N SEL - digital  10
 * N IRQ - digital  2
 * SCK - digital  13
 * SDO - digital  12
*/

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <JeeLib.h>

LiquidCrystal lcd(19, 18, 17, 16, 15, 14);

const byte ROWS = 4; 
const byte COLS = 3; 
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};
/* 

*/
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


int value=0;
typedef struct{int Volume,Sensor;}              //analog valuess to be sent
DataTxRx;                         
DataTxRx payload;
DataTxRx Packetin;
byte needToSend;
byte received;


void setup(){
  Serial.begin(57600);
  Serial.println("Hello");
  rf12_initialize(5, RF12_433MHZ, 135);
  lcd.begin(16, 2);
  
  lcd.print("Sampler Remote");
  delay(1000);
  lcd.clear();

  lcd.print("Node: 5");
  delay(1000);
  lcd.clear();

  lcd.print("Network: 135");
  delay(1000);
  lcd.clear();

  
  lcd.print("value ");
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    switch(customKey){
      case'*':{
        Delete();
        lcd.clear();
        lcd.print("value ");
        delay(1000);
        lcd.print(value);
        Serial.println(value);
        break;
      }
      case'#':{
        lcd.clear();
        lcd.print("Sending data");
        RFSend();
        delay(1000);
        lcd.print("Data Sent");
        Serial.println("Data Sent ");

        Serial.println(value);
        break;
      }
      default:{
        number(customKey);
        lcd.clear();
        lcd.print("value ");
        Serial.println("value ");
        Serial.println(value);
        delay(1000);
        lcd.print(value);
      }
    }
  }
  RFReceive();
}

void number(char x){
  value=(value*10)+(x-48);
}
  
void Delete(){
  value=(value/10);
}

void RFSend(){
  if (rf12_canSend()) {
    lcd.clear();
    payload.Volume = value;
      rf12_sendStart(0, &payload, sizeof payload);
      lcd.clear();

  }
}
void RFReceive(){
  if (rf12_recvDone() && rf12_crc == 0) {
    Packetin=*(DataTxRx*)rf12_data;
    lcd.clear();
    lcd.print("Sensor data: ");
    lcd.print(Packetin.Sensor);      
  }
}

