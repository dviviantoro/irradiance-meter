#include <Arduino.h>
#include <Wire.h>

const int led =  2; 
const int SlaveNumber = 3;
// const char* slavenum = "AV3";
// const char* slavenum = "AV2";
// const char* slavenum = "AV3";
// const char* slavenum = "I1LF";
// const char* slavenum = "I2LF";
const char* slavenum = "I3LF";

int ayam = 0;

#define RXD2 16
#define TXD2 17
void setup() 
{ 
  Serial.begin(115200);  
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.setTimeout(250);
  // pinMode(Enable, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
} 

void loop() {
  digitalWrite(led, LOW);

  while(Serial2.available()) {
    String a= Serial2.readString();
    Serial.println(a);
    
    if (a == slavenum){
      digitalWrite(led,HIGH);
      ayam += SlaveNumber;      
      
      // Serial2.print("i");
      // Serial2.print(SlaveNumber);   
      // Serial2.print(ayam);
      // Serial2.print("f");
      // Serial2.flush();    

      String msg = "reply" + String(SlaveNumber) + ";" + String(ayam);
      Serial2.print(msg);

    }
    // delay(10);
  }

  // if(Serial2.available()) {
  //   if(Serial2.find("I")) {   
  //     int Slave = Serial2.parseInt();
        
  //     if(Slave == SlaveNumber) {   
  //       char command = Serial2.read();       
             
  //       if(command == 'L') {
  //         if(Serial2.read()=='F') { 
  //           ayam += 1;      
  //           digitalWrite(led, HIGH);
            
  //           Serial2.print("i");
  //           Serial2.print(SlaveNumber);   
  //           Serial2.print(ayam);
  //           Serial2.print("f");
  //           Serial2.flush();    
  //         }
  //       }   
  //     }
  //   }
  // }

  // if(Serial2.available()) {
  //   Serial.println(Serial2.read());
  // }

  




  // if (Serial2.find("I")) {
  //   bool slave = Serial2.find(slavenum);
  //   if (slave == true) {
  //     char command = Serial2.read();
  //     if(command == 'L') {
  //       if(Serial2.read()=='F') { 
  //         ayam += 1;      
  //         digitalWrite(led, HIGH);
          
  //         Serial2.print("i");
  //         Serial2.print(SlaveNumber);   
  //         Serial2.print(ayam);
  //         Serial2.print("f");
  //         Serial2.flush();    
  //       }
  //     } 
  //   }
  // }
  // delay(10);
}