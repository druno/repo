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
   
    set_time();   
}


void set_time() {

  Serial.println("start function set-time");
  delay (100);
  
  
  uint8_t data;
 //    uint8_t data = 0b00010101;
  //  uint8_t data = bin2bcd(10);


    
 //    RTC.writeByteInRam(0x00, data);
   //  RTC.writeByteInRam(0x01, data);
   //  RTC.writeByteInRam(0x02, data);

//     data = 255;
 //    RTC.writeByteInRam(0x37, data);    
 
   if (digitalRead(butsel) == LOW)  
   {
     delay(500);
     Serial.print ("Nastrouka vremeni");
     Serial.println();

  
 //установка часов
                Serial.print ("t1");
                Serial.println();
                delay(500);
                data = bcd2bin(RTC.readByteInRam(0x02));
                Serial.print ("Chasov: ");
                Serial.print(data);
                do
                {
                    
                   if (digitalRead(butplus) == LOW){
                  delay(300);
                  Serial.println ("step1 - plus");
                  Serial.println();
                  data++;
                  if (data>23){data=0;}
                  RTC.writeByteInRam(0x02, bin2bcd(data));
                  Serial.print ("Chasov: ");
                  Serial.print(data);
                  }           
  
                    if (digitalRead(butminus) == LOW){
                  delay(300);
                  Serial.println ("step1 - minus");
                  Serial.println();  
                    data--;
                  if (data==255){data=23;}
                  RTC.writeByteInRam(0x02, bin2bcd(data));
                  Serial.print ("Chasov: ");
                  Serial.print(data);                
                  } 
                  
                  if (digitalRead(butsel) == LOW){
                  delay(300);
                  Serial.print ("step1 - break");
                  Serial.println();
                  break;}    
                
                } while (1);
                
//установка минут

                data = bcd2bin(RTC.readByteInRam(0x01));
                Serial.print ("minut: ");
                Serial.print(data);
                do
                {
                    
                   if (digitalRead(butplus) == LOW){
                  delay(300);
                  Serial.println ("step1 - plus");
                  Serial.println();
                  data++;
                  if (data>59){data=0;}
                  RTC.writeByteInRam(0x01, bin2bcd(data));
                  Serial.print ("Minut: ");
                  Serial.print(data);
                  }           
  
                    if (digitalRead(butminus) == LOW){
                  delay(300);
                  Serial.println ("step1 - minus");
                  Serial.println();  
                    data--;
                  if (data==255){data=59;}
                  RTC.writeByteInRam(0x01, bin2bcd (data));
                  Serial.print ("Minut: ");
                  Serial.print(data);                
                  } 
                  
                  if (digitalRead(butsel) == LOW){
                  delay(300);
                  Serial.print ("step1 - break");
                  Serial.println();
                  break;}    
                
                } while (1);
                                
                
              //часы
              //минуты
//-----------------------------------------------
        
        
        
             //установка времени включения света чч:11, мм:12
                Serial.print ("t2");
                Serial.println();
              //часы
              //минуты
              
              //установка времени выключения света чч:13, мм:14
              //часы
              //минуты
  
            
              //регулярность смешивания мин:15
              //длительность смешивания сек:16  
       
     
   } 
     delay(1000);
     /*    data= bcd2bin(RTC.readByteInRam(0x02));
          Serial.print(data, DEC);
              Serial.print(':');
          data= bcd2bin(RTC.readByteInRam(0x01));
          Serial.print(data, DEC);
              Serial.print(':');
          data= bcd2bin(RTC.readByteInRam(0x00));
          Serial.print(data, DEC); 
      Serial.println();

            data= RTC.readByteInRam(0x37);
          Serial.print(data, DEC); 
             Serial.println();
          Serial.println();       
*/
 
  return;
}  

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
