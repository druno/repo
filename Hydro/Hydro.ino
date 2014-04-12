#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(P1_0, P1_1, P1_2, P1_3, P1_4, P1_5);
RTC_DS1307 RTC;



void setup()
{
  Wire.begin();
    RTC.begin();


  //pinMode(5, INPUT_PULLUP);  //кнопень

  pinMode(19, OUTPUT);  //смеситель
  digitalWrite(19, LOW); //сброс на 0 

Serial.begin(9600);
    
//  lcd.begin(16, 2);
  //lcd.print("hello, world!");

}

long previousMillis = 0;  
long interval = 360000; //интервал между поливами
long smesitel = 2000; //время работы смесителя


/////////////////////////////////////////////////
void loop()
{
 
  
 //  if (digitalRead(5) == LOW) {    //полив по нажатию кнопки вне очереди
   //   poliv ();  
   // } 
  
   if (voda()>0)       //уровень воды нормальный
   {
     // digitalWrite(2, LOW); //гасим красный
  
      unsigned long currentMillis = millis();     //время с момента последнего запуска или резета
      long time=currentMillis - previousMillis;
      long time1=(interval-time)/1000; //Оставшееся время до запуска в sec
       
      Serial.println(time1); 
      if(time > interval) {
          previousMillis = currentMillis;   
          poliv ();
          }
    }
    else {    
     //  digitalWrite(2, HIGH); //зажигаем красный
    }
   
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
   if (now.month() <10) Serial.print('0');    
    Serial.print(now.month(), DEC);
    Serial.print('/');
    if (now.day() <10) Serial.print('0');    
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    if (now.hour() <10) Serial.print('0');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    if (now.minute() <10) Serial.print('0');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    if (now.second() <10) Serial.print('0');
    Serial.print(now.second(), DEC);
    Serial.println();  

   if ((now.hour() >= 8) and (now.hour() <=10) ) {
          Serial.print(now.hour());
       Serial.println(" svet on "); 
      svet(); 
    } 
   else {    
     Serial.print(now.hour());
       Serial.println(" svet off ");

    }
    
    
  delay(1000);     //задержка на каждый цикл  
  
}                 //end main loop



/////////////////////////////////////////////////////////

int voda () {
//проверяем уровень воды, если 1 то норм
  return 1; //random (2);
}

void poliv() {
//процедура полива
    digitalWrite(19, HIGH); 
    Serial.println("idet poliv"); 
    delay (smesitel);
    digitalWrite(19, LOW);  
    Serial.println("poliv okonchen");
    delay (800);
    return;  
}

void svet() {
//процедура полива
   // digitalWrite(2, HIGH); 
    delay (1000);
   // digitalWrite(2, LOW);  
    delay (1000);
    return;  
}


