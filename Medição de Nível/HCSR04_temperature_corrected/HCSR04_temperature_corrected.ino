#include <LiquidCrystal.h> 
#include <DHT.h>
#include <Average.h>

#define trigPin PB6
#define echoPin PB7
#define DHTPIN PB8
#define DHTTYPE DHT22
#define amostra 500

const int rs = PB11, en = PB10, d4 = PB0, d5 = PB1, d6 = PC13, d7 = PC14; //mention the pin names to with LCD is connected to 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Initialize the LCD

DHT dht(DHTPIN, DHTTYPE);

long duration;
int distance;
float divisor, distancia_cm;
float h, t;
int i, distancia_int;

float media = 0;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  dht.begin();
  lcd.begin(16, 2);
}

void loop()
{
  Average<int> ave(500);
  t = mediaTemperatura();
  
  divisor = 10000/(sqrt((t+273.15)/273.15) * 331.45);   
      
  for(int i = 0; i < amostra; i++)
  {
    distancia_cm = calcularDistancia(divisor);

    distancia_int = distancia_cm;
    
    ave.push(distancia_int);
  }
  
  Serial.println(String("Distância: " + String(ave.mode())));
  delay(10);
}
    
void mostrarValores(float valor){

  Serial.println(valor);
  
  lcd.setCursor(0, 0); //At first row first column 
  lcd.print("Distancia:"); //Print this
  
  lcd.setCursor(0, 1); //At secound row first column 
  lcd.print(valor); //Print the value of secounds
}

float mediaTemperatura(){
  float mediatemperatura = 0, t;
  for (i = 0; i < 100; i++)
    {
      t = dht.readTemperature();

      if ( i == 0 ) mediatemperatura = t;

      else mediatemperatura=((mediatemperatura+t)/2);
      
      delay(10);
    }

    return t;
}

float calcularDistancia(float divisor)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int tempo = pulseIn(echoPin, HIGH);
  
  float distancia_cm = (tempo / divisor) / 2;

  return distancia_cm;
}

  
