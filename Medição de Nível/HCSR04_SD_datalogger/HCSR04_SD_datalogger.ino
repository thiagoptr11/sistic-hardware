#include <DHT.h>
#include <Average.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

/*
#define trigPin PB6
#define echoPin PB7
#define DHTPIN PB8
#define DHTTYPE DHT22
#define amostra 500

#define SPI1_NSS_PIN PA4 
*/

#define trigPin 6
#define echoPin 5
#define DHTPIN 7
#define DHTTYPE DHT22
#define amostra 100

DHT dht(DHTPIN, DHTTYPE);

File sdCard;

long duration;
float divisor, distancia_cm, h, t, media = 0;
int i, distancia_int;


String sdFileName = "HCSR04_3.txt";

void setup()
{
  Serial.begin(9600); // Starts the serial communication
 

  Serial.print("Initializing SD card...");

  if (!SD.begin(8)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  delay(3000);
  
  dht.begin();

  
}

void loop()
{
  int v[100], cont[100];
  delay(6000);
  float te = dht.readTemperature();

  Serial.println(te);
  
  divisor = 10000/(sqrt((te+273.15)/273.15) * 331.45);   
      
  for(int i = 0; i < amostra; i++)
  {
    distancia_cm = calcularDistancia(divisor);

    distancia_int = distancia_cm;

    v[i] = distancia_int;
    
  }
  
  //Serial.println(String("Distância: " + String(ave.mode())));
  int dist = calcularModa (100, v, cont);
  saveToSD(dist);
  
  delay(10);
}
    
void saveToSD(int valor){
  sdCard = SD.open(sdFileName, FILE_WRITE);// abre o arquivo de texto onde são salvas as informações;
  
  if (sdCard) {
    Serial.println("Salvando no SD card...");
    Serial.println(valor);
    sdCard.print("Distancia: ");
    sdCard.print(valor);
    sdCard.println(" cm");
    sdCard.close(); // fecha o arquivo de texto.
    Serial.println("DONE");
    Serial.println();
  }
}

float mediaTemperatura(){
  float mediatemperatura = 0, temp;
  for (int i = 0; i < 100; i++)
    {
      temp = dht.readTemperature();

      if ( i == 0 ) {
        mediatemperatura = temp;
      }

      else {
        mediatemperatura=((mediatemperatura + temp)/2);
      }
      
      delay(10);
    }
    Serial.println(mediatemperatura);
    return mediatemperatura;
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

int calcularModa (int tamanho, int v[], int cont[]){

  int conta = 0; int moda = 0;
  for(int i=0;i<tamanho;i++){
        for(int j=i+1;j<tamanho;j++){
          
      if(v[i]==v[j]){
        cont[i]++;
          if(cont[i]>conta){
            conta=cont[i];
            moda=v[i];
          }
      }
          
        }
        cont[i]=0;
    }

    return moda;
}

  
