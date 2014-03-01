#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 RTC;

void setup()
{
  
 
  Wire.begin();
  RTC.begin();
  
  pinMode(2, OUTPUT); //led1
  digitalWrite(2, LOW); //сброс на 0  
 // pinMode(14, OUTPUT);  //led2
 // digitalWrite(14, LOW); //сброс на 0

  pinMode(19, OUTPUT);  //смеситель
  digitalWrite(19, LOW); //сброс на 0 

  Serial.begin(9600);

}

long previousMillis = 0;  
long interval = 10000; //интервал между поливами
long smesitel = 2000; //время работы смесителя


/////////////////////////////////////////////////
void loop()
{
   if (voda()>0)       //уровень воды нормальный
   {
  //  digitalWrite(14, HIGH); //зажигаем зеленый
    digitalWrite(2, LOW); //гасим красный

    unsigned long currentMillis = millis();     //время с момента последнего запуска или резета
    long time=currentMillis - previousMillis;
    long time1=(interval-time)/1000; //Оставшееся время до запуска в sec
     
    Serial.println(time1); 
    if(time > interval) {
        previousMillis = currentMillis;   
        poliv ();
        }
    }
    else {     //Уровень воды ниже нормы 
       digitalWrite(2, HIGH); //зажигаем красный
  //     digitalWrite(14, LOW); //гасим зеленый
    } 
  
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();  
    
    
  delay(100);     //задержка на каждый цикл  
  
}                 //end main loop



//////////////////////////////////////////////
int voda () {
//рандомно проверяем уровень воды, если 1 то норм
  return 1; //random (2);
}

void poliv() {
//процедура полива
    digitalWrite(19, HIGH);  
    delay (smesitel);
    digitalWrite(19, LOW);  
    Serial.println("poliv");
    return;  
}
