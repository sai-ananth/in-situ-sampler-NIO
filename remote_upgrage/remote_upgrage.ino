#include <JeeLib.h>

#define rf_freq RF12_433MHZ

void setup() {
  Serial.begin(57600);
  Serial.println(57600);
  Serial.println("Send");    
  rf12_initialize(node_id,rf_freq, networkgroup);          //initializing the rf 12 module with its node id and its network group
  Serial.println("rf initialized");
}

void loop() {
  if (rf12_recvDone() && rf12_crc == 0) 
    {     
      Packetin=*(DataTxRx*)rf12_data;
        Serial.print(" Sensor value is : ");
        Serial.println(Packetin.Sensor);
    }

}
