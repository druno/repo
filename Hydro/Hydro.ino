#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

#include <dht.h>



LiquidCrystal lcd(P1_0, P1_1, P1_2, P1_3, P1_4, P1_5);
RTC_DS1307 RTC;

dht DHT;

const int butsel = 11;
const int butplus = 12;
const int butminus = 13;


void setup()
{
    Wire.begin();
    RTC.begin();

    pinMode(19, OUTPUT);  //смеситель
    digitalWrite(19, LOW); //сброс на 0 
    pinMode(17, OUTPUT);  //свет
    digitalWrite(17, LOW); //сброс на 0 
    
    pinMode(butsel, INPUT_PULLUP);  //кнопень    
    pinMode(butplus, INPUT_PULLUP);  //кнопень   
    pinMode(butminus, INPUT_PULLUP);  //кнопень
  //  Serial.begin(9600);
    
   lcd.begin(16, 2);

}

long interval, smesitel, previousMillis = 0;  
//long interval = 360000; //интервал между поливами
//long smesitel = 2000; //время работы смесителя


/////////////////////////////////////////////////
void loop()
{ 
  
     if (digitalRead(butplus) == LOW) {    //полив по нажатию кнопки вне очереди
      delay(500);
      poliv ();  
    } 
  
      if (digitalRead(butsel) == LOW)  
    {        
       delay(500);
       set_time();
    }  
    
    DateTime now = RTC.now();
 /*   if (now.hour() <10) Serial.print('0');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    if (now.minute() <10) Serial.print('0');
    Serial.print(now.minute(), DEC);
*/
    //lcd.clear();
    lcd.setCursor(0, 0);

    if (now.hour() <10) lcd.print('0');
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    if (now.minute() <10) lcd.print('0');
    lcd.print(now.minute(), DEC);

    interval=RTC.readByteInRam(0x15)*60000; //переводим минуты в секунды (умножить на 60) и потом в миллисекунды
     
    unsigned long currentMillis = millis();     //время с момента последнего запуска или резета
    long time=currentMillis - previousMillis;
    long time1=(interval-time)/1000; //Оставшееся время до запуска в sec
  //    Serial.print("do poliva: ");
  //    Serial.println(time1);
    lcd.setCursor(0, 1);
   // lcd.print("\xE0o \xBEo\xBB""\xB8\xB3""a ");  //do poliva
   int chk = DHT.read22(18);
   lcd.print (DHT.temperature);
    lcd.print ("\x99 "); 
   lcd.print (DHT.humidity);
     lcd.print ("\x25 ");     
     
     
     ///////////////////////////////////////////
     
     
     
     
     
     
    if(time > interval) {
          previousMillis = currentMillis;   
          poliv ();
          previousMillis = currentMillis=millis();   
     }
 
     if (time1>=90){
       lcd.print(time1/60+1);
       if (time1<=6000){ //если время меньше 99 мин
         lcd.print(" ");
       }  
       lcd.print("\xBC\xB8\xBD"); 
     }
     
     else{
       if (time1<=9){ //если время меньше 10 сек
         lcd.print("0");
       }
       lcd.print(time1);
       lcd.print(" ce\xBA\x20"); 
     }
     

   if ((now.hour() >= bcd2bin(RTC.readByteInRam(0x11))) and (now.hour() <= bcd2bin(RTC.readByteInRam(0x13))) ) {
    //   Serial.println(" svet on "); 
    lcd.setCursor(7, 0);
    lcd.print("C\xB3""e\xBF: \xB3\xBA\xBB");
    digitalWrite(18, HIGH);
    } 
   else {
   //    Serial.println(" svet off ");
    lcd.setCursor(7, 0);
    lcd.print("C\xB3""e\xBF \xB3\xC3\xBA\xBB");
    digitalWrite(18, LOW);      
    }


    
  delay(1000);     //задержка на каждый цикл    
}                 //end main loop



/////////////////////////////////////////////////////////


void poliv() {
//процедура полива
    digitalWrite(19, HIGH); 
   // Serial.println("idet poliv"); 
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("\xB8\xE3\xB5\xBF \xBEo\xBB\xB8\xB3");//idet poliv
    smesitel=RTC.readByteInRam(0x16);
    for (int i=smesitel; i>0; i--){
      lcd.setCursor(5, 1);
      lcd.print(i);
      lcd.print(" ce\xBA\x20");    //sek
      delay(1000);
    }  
   
    digitalWrite(19, LOW);   
    delay (100);
    lcd.clear();   
    return;  
}

void set_time() {
       uint8_t data, addr;
//       Serial.print ("Nastroika vremeni");
//       Serial.println();
    lcd.clear();
    lcd.print("Hac\xBF""po\xB9\xBA""a");//настройка системы
    lcd.setCursor(0, 1);
    lcd.print("c\xB8""c\xBF""e\xBC\xC3");
         
    delay(1500);     

       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Ce\xB9\xC0""ac \xAB\xAB"":MM"); //Сейчас ЧЧ:ММ

       addr=0x01;//читаем минуты и выводим их правее часов
       data = bcd2bin(RTC.readByteInRam(addr));
       lcd.setCursor(3, 1);
       lcd.print(":");
       lcd.print(data);
       addr=0x02;
       data = bcd2bin(RTC.readByteInRam(addr));//читаем часы и выводим слева+мигающий курсор
       lcd.setCursor(0, 1);
       lcd.print(data);
       lcd.blink();
       edit_ram(data,23,addr);
       data = bcd2bin(RTC.readByteInRam(addr));       
       //Serial.print ("Chasov: ");
       //Serial.println(data);
       delay(1000);    
  
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Ce\xB9\xC0""ac \xAB\xAB"":MM");

       addr=0x02;//читаем часы
       data = bcd2bin(RTC.readByteInRam(addr));
       lcd.setCursor(0, 1);
       lcd.print(data);
       lcd.print(":");
       addr=0x01;
       data = bcd2bin(RTC.readByteInRam(addr));//читаем минуты
       lcd.setCursor(3, 1);
       lcd.print(data);
       lcd.blink();
       edit_ram(data,59,addr);
       data = bcd2bin(RTC.readByteInRam(addr));       
       //Serial.print ("Chasov: ");
       //Serial.println(data);
       delay(1000);    
       
      


//установка часов
/*
       addr=0x02;
       data = bcd2bin(RTC.readByteInRam(addr));
       //Serial.print ("Seychas Chasov: ");
       //Serial.println(data);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Seychas chasov: ");
       lcd.setCursor(0, 1);
       lcd.print(data);
       lcd.blink();
       edit_ram(data,23,addr);
       data = bcd2bin(RTC.readByteInRam(addr));       
       //Serial.print ("Chasov: ");
       //Serial.println(data);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Seychas chasov: ");
       lcd.setCursor(0, 1);
       lcd.print(data);       
       delay(1000);    
    
       
//установка минут
       addr=0x01;
       data = bcd2bin(RTC.readByteInRam(addr));
       //Serial.print ("Seychas minut: ");
       //Serial.println(data);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Seychas minut: ");
       lcd.setCursor(0, 1);
       lcd.print(data);
       edit_ram(data,59,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       //Serial.print ("minut: ");
       //Serial.println(data);       
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Seychas minut: ");
       lcd.setCursor(0, 1);
       lcd.print(data);
       delay(1000);
  */      
//установка времени включения света чч:11, мм:12
       addr=0x11;
       data = bcd2bin(RTC.readByteInRam(addr));
       //Serial.print ("Vkluchenie sveta v: ");
       //Serial.println(data);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Vkl. sveta v: ");
       lcd.setCursor(0, 1);
       lcd.print(data);
       edit_ram(data,23,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       //Serial.print ("Vkluchenie sveta v: ");
       //Serial.println(data);       
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Vkl. sveta v: ");
       lcd.setCursor(0, 1);
       lcd.print(data);
       delay(1000);
  /*     
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
    */          
//установка времени выключения света чч:13, мм:14
       addr=0x13;
       data = bcd2bin(RTC.readByteInRam(addr));
       //Serial.print ("Vikluchenie sveta v: ");
       //Serial.println(data);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Vikl. sveta v: ");
       lcd.setCursor(0, 1);
       lcd.print(data);      
       edit_ram(data,23,addr);
       data = bcd2bin(RTC.readByteInRam(addr));
       //Serial.print ("Vikluchenie sveta v: ");
       //Serial.println(data);       
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Vikl. sveta v: ");
       lcd.setCursor(0, 1);
       lcd.print(data);
       delay(1000);
      /* 
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
       */                   
//регулярность смешивания мин:15

       addr=0x15;
       data = RTC.readByteInRam(addr);
       //Serial.print ("regularnost smeshivania (min): ");
       //Serial.println(data);
       lcd.clear();
       lcd.setCursor(0, 0);      
       lcd.print("Chastota poliva ");
       lcd.setCursor(4, 1);
       lcd.print("min");          
       lcd.setCursor(0, 1);
       lcd.print(data);
       edit_ram_255(data,240,addr);
       data = RTC.readByteInRam(addr);
       //Serial.print ("regularnost smeshivania(min): ");
       //Serial.println(data);       
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Chastota poliva ");
       lcd.setCursor(4, 1);
       lcd.print("min");      
       lcd.setCursor(0, 1);
       lcd.print(data);
       delay(1000); 
       
//длительность смешивания сек:16  
       addr=0x16;
       data = RTC.readByteInRam(addr);
       //Serial.print ("vremya smeshivania(sec): ");
       //Serial.println(data);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Vremya poliva ");
       lcd.setCursor(4, 1);
       lcd.print("sec"); 
       lcd.setCursor(0, 1);
       lcd.print(data);
       edit_ram_255(data,240,addr);
       data = RTC.readByteInRam(addr);
       //Serial.print ("vremya smeshivania(sec): ");
       //Serial.println(data);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Vremya poliva ");
       lcd.setCursor(4, 1);
       lcd.print("sec");       
       lcd.setCursor(0, 1);
       lcd.print(data);
       delay(1000);     
       lcd.noBlink();
       lcd.clear(); 
       return;
}  


void edit_ram (uint8_t data, uint8_t max, uint8_t addr) {
       do{             
            if (digitalRead(butplus) == LOW){
                  delay(300);
                  data++;
                  if (data>max){data=0;}
                  RTC.writeByteInRam(addr, bin2bcd(data));
                  //Serial.print(data);
                 lcd.clear();
                 lcd.setCursor(0, 0);
                 lcd.print(data);                 
             }           
  
             if (digitalRead(butminus) == LOW){
                  delay(300);
                  data--;
                  if (data==255){data=max;}
                  RTC.writeByteInRam(addr, bin2bcd(data));                 
                  //Serial.print(data);
                 lcd.clear();
                 lcd.setCursor(0, 0);
                 lcd.print(data);  
             } 
                  
             if (digitalRead(butsel) == LOW){
                  delay(300);
                  break;
             }    
       } 
       while (1); 
return;  
}
 
 
void edit_ram_255 (uint8_t data, uint8_t max, uint8_t addr) {
       do{             
            if (digitalRead(butplus) == LOW){
                  delay(300);
                  data++;
                  if (data>max){data=0;}
                  RTC.writeByteInRam(addr, data);
                  //Serial.print(data);
                 lcd.clear();
                 lcd.setCursor(0, 0);
                 lcd.print(data);                 
             }           
  
             if (digitalRead(butminus) == LOW){
                  delay(300);
                  data--;
                  if (data==255){data=max;}
                  RTC.writeByteInRam(addr, data);                 
                  //Serial.print(data);
                 lcd.clear();
                 lcd.setCursor(0, 0);
                 lcd.print(data);  
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
