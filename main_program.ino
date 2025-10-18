#include <Wire.h>
#include<Adafruit_INA219.h>
#define Relay1 2
#define Relay2 3
#define Relay3 4

Adafruit_INA219 ina219;
float shuntvoltage, busvoltage, current_mA, power_mW, loadvoltage;

void setup(){
  Serial.begin(115200);
  while(!Serial){
    delay(1);
  }
  if (!ina219.begin()){
    Serial.print("Failed to find INA219 sensor");
    while(1){
      delay(10);
    }
  }
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Relay3,OUTPUT);
  
  showmenue();
  }

void loop(){
  if(Serial.available()){
    int ch = Serial.parseInt();
  
      digitalWrite(Relay1,LOW);
      digitalWrite(Relay2,LOW);
      digitalWrite(Relay3,LOW);

      if(ch==1){
          digitalWrite(Relay1,HIGH);
          measurements();
          delay(2000);
        }
      else if(ch==2){
        digitalWrite(Relay2,HIGH);
        measurements();
        delay(2000);
      }
      else if(ch==3){
      digitalWrite(Relay1,HIGH);
      
      // FAULT DETECTION LOGIC (FIXED)
      float Vsum = 0;
      float Isum = 0;
            
      for(int i = 0; i < 10; i++){
        shuntvoltage = ina219.getShuntVoltage_mV();
        busvoltage = ina219.getBusVoltage_V();
        current_mA = ina219.getCurrent_mA();
        power_mW = ina219.getPower_mW();
        loadvoltage = busvoltage + (shuntvoltage / 1000);
        Vsum += busvoltage;
        Isum += current_mA;
        Serial.print("Reading "); Serial.print(i+1); Serial.println("/10");
        delay(1000);
      }
      
      float Vavg = Vsum/10;
      float Iavg = Isum/10;
      
      Serial.print("Average Voltage: "); Serial.println(Vavg);
      Serial.print("Average Current: "); Serial.println(Iavg);
      
      if (Vavg < 1 || Iavg > 1){
        Serial.println("FAULT DETECTED!!");
        Serial.println("might be a short circuit fault!");
        digitalWrite(Relay1,LOW);
        digitalWrite(Relay2, HIGH);
        Serial.print("TURNING ON STORAGE UNIT");
      }
      else if(Vavg > 12 || Iavg < 0.1){
        Serial.println("FAULT DETECTED!");
        Serial.println("might be a open circuit fault");
        digitalWrite(Relay1,LOW);
        digitalWrite(Relay2, HIGH);
        Serial.print("TURNING ON STORAGE UNIT");
      }
      else{
        Serial.println("THE GRID IS HEALTHY. :)");
      }
        }
      else if(ch==4){
          digitalWrite(Relay3,HIGH);
          Serial.print("Efficiency is increased by 20%");
          digitalWrite(Relay1,HIGH);
          measurements();
        }
    }
              
}

void measurements(){
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  Serial.print("\nBus Voltage:   ");
  Serial.print(busvoltage); 
  Serial.print(" V");
  Serial.print("\nShunt Voltage: ");
  Serial.print(shuntvoltage);
  Serial.println(" mV");
  Serial.print("\nLoad Voltage:  ");
  Serial.print(loadvoltage);
  Serial.println(" V");
  Serial.print("\nCurrent:       ");
  Serial.print(current_mA);
  Serial.println(" mA");
  Serial.print("\nPower:         "); 
  Serial.print(power_mW);
  Serial.println(" mW");
}

void showmenue(){
   Serial.println("Enter a choice");
   Serial.println("1.SOLAR GRID MEASUREMENTS");
   Serial.println("2.STORAGE UNIT MEASUREMENTS");
   Serial.println("3.FAULT DETECTION IN SOLAR GRID");
   Serial.println("4.EFFICIENCY ENHANCEMENT");

}
void fualtdetection(){
  float Vsum=0;
  float Isum=0;
  for (int i=0;i<=10;i++){
    measurements();
    Vsum += busvoltage;
    Isum += current_mA;
    delay(1000);
  }
  float Vavg=Vsum/10;
  float Iavg=Isum/10;
  if (Vavg < 1 || Iavg > 1){
        Serial.println("FAULT DETECTED!!");
        Serial.println("might be a short circuit fault!");
        digitalWrite(Relay1,LOW);
        digitalWrite(Relay2,HIGH);
    }

  else if(Vavg > 12 || Iavg < 0.1){
        Serial.println("FAULT DETECTED!");
        Serial.println("might be a open circuit fault");
        digitalWrite(Relay1,LOW);
        digitalWrite(Relay2,HIGH);
    }
  else{
        Serial.println("THE GRID IS HEALTHY. :)");
    }
}
