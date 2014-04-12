#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 RTC;

const int butsel = 11;
const int butplus = 12;
const int butminus = 13;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    pinMode(butsel, INPUT_PULLUP);  //кнопень    
    pinMode(butplus, INPUT_PULLUP);  //кнопень   
    pinMode(butminus, INPUT_PULLUP);  //кнопень   
}

void loop()
{
    DateTime now = RTC.now();
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.println();
    
    if (digitalRead(butsel) == LOW)  
    {        
       delay(500);
       set_time();
    }  
    delay(1000);   
}



void set_time() {
       uint8_t data, addr;
       Serial.print ("Nastroika vremeni");
       Serial.println();

//установка часов

       addr=0x02;
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("Chasov: ");
       Serial.println(data);
       edit_ram(data,23,addr);
       data = bcd2bin(RTC.readByteInRam(addr));       
       Serial.print ("Chasov: ");
       Serial.println(data);   
       Serial.println();  
       delay(1000);    
    
       
//установка минут
       addr=0x01;
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("minut: ");
       Serial.println(data);
       edit_ram(data,59,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("minut: ");
       Serial.println(data);       
       Serial.println();
       delay(1000);
        
//установка времени включения света чч:11, мм:12
       addr=0x11;
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("start svet chasov: ");
       Serial.println(data);
       edit_ram(data,23,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("start svet chasov: ");
       Serial.println(data);       
       Serial.println();
       delay(1000);
       
       addr=0x12;
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("start svet minut: ");
       Serial.println(data);
       edit_ram(data,59,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("start svet minut: ");
       Serial.println(data);       
       Serial.println();
       delay(1000); 
              
//установка времени выключения света чч:13, мм:14
       addr=0x13;
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("off svet chasov: ");
       Serial.println(data);
       edit_ram(data,23,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("off svet chasov: ");
       Serial.println(data);       
       Serial.println();
       delay(1000);
       
       addr=0x14;
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("off svet minut: ");
       Serial.println(data);
       edit_ram(data,59,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("off svet minut: ");
       Serial.println(data);       
       Serial.println();
       delay(1000); 
                          
//регулярность смешивания мин:15

       addr=0x15;
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("regularnost smeshivania (min): ");
       Serial.println(data);
       edit_ram(data,59,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("regularnost smeshivania(min): ");
       Serial.println(data);       
       Serial.println();
       delay(1000); 
       
//длительность смешивания сек:16  
       addr=0x15;
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("vremya smeshivania(sec): ");
       Serial.println(data);
       edit_ram(data,59,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       Serial.print ("vremya smeshivania(sec): ");
       Serial.println(data);       
       Serial.println();
       delay(1000);       
       return;
}  

void edit_ram (uint8_t data, uint8_t max, uint8_t addr) {
       do{             
            if (digitalRead(butplus) == LOW){
                  delay(300);
                  data++;
                  if (data>max){data=0;}
                  RTC.writeByteInRam(addr, bin2bcd(data));
                  Serial.print(data);
             }           
  
             if (digitalRead(butminus) == LOW){
                  delay(300);
                  data--;
                  if (data==255){data=max;}
                  RTC.writeByteInRam(addr, bin2bcd(data));                 
                  Serial.print(data);
             } 
                  
             if (digitalRead(butsel) == LOW){
                  delay(300);
                  break;
             }    
       } 
       while (1); 
return;  
}
  
                
static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
