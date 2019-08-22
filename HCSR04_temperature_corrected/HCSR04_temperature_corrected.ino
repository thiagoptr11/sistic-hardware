// defines pins numbers
#define trigPin PB6
#define echoPin PB7
#include <DHT.h>
#define DHTPIN PB8
#define DHTTYPE DHT22   // Sensor DHT 22  (AM2302)
#include <LiquidCrystal.h> // include the LCD library

const int rs = PB11, en = PB10, d4 = PB0, d5 = PB1, d6 = PC13, d7 = PC14; //mention the pin names to with LCD is connected to 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Initialize the LCD

DHT dht(DHTPIN, DHTTYPE);

// defines variables
long duration;
int distance;
float divisor;
float h, t;
int i;
float distance2;

void setup()
{
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication
    dht.begin();
    lcd.begin(16, 2);//We are using a 16*2 LCD
}

void loop()
{
    float soma = 0;
    for (i = 0; i<100; i++)
    {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      h = dht.readHumidity();
      t = dht.readTemperature();
      
      divisor = 10000/(sqrt((t+273.15)/273.15) * 331.45);
      
      distance = (duration/divisor)/2;

      soma += distance;
    }

    distance2 = soma/100;

    lcd.setCursor(0, 0); //At first row first column 
    lcd.print("Distância:"); //Print this
  
    lcd.setCursor(0, 1); //At secound row first column 
    lcd.print(distance2); //Print the value of secounds
    
    //Serial.print("Distance: ");
    //Serial.println(soma/10);

    delay(1000);
    
}
