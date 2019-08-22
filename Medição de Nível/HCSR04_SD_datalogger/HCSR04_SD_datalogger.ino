#include <DHT.h>
#include <Average.h>
#include <SD.h>
#include <SPI.h>

#define trigPin PB6
#define echoPin PB7
#define DHTPIN PB8
#define DHTTYPE DHT22
#define amostra 500

DHT dht(DHTPIN, DHTTYPE);

File sdCard;

long duration;
float divisor, distancia_cm, h, t, media = 0;
int i, distancia_int;

String sdFileName = "HCSR04_ensaio_1.txt";

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  dht.begin();
  SD.begin()
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

  saveToSD(ave.mode());
  
  delay(10);
}
    
void saveToSD(int valor){
  sdCard = SD.open(sdFileName, FILE_WRITE);// abre o arquivo de texto onde são salvas as informações;
  
  if (sdCard) {
    sdCard.println(String("Distancia: " + valor + " cm"));
    sdCard.close(); // fecha o arquivo de texto.
  }
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

  
