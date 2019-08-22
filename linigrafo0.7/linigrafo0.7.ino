
//Bibliotecas de funções utilizadas no codigo
#include <DHT.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Definição de pinos e variáveis.
DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);
int gatilho = 4;
int echo = 5;
float tempo;
float distancia_cm; 
#define DHTPIN 9
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
const int analogInPin = A2;
File myFile;
int pinCS = 10;
int nivel;
Time TEMPO;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  rtc.begin();            //Inicialização do RTC DS3231
  //Descomente as linhas a seguir para configurar o horário, após comente e faça o upload novamente para o Arduino
  //rtc.setDOW(SATURDAY);     // Definir o dia da semana em inglês
  //rtc.setTime(15,30, 0);     // Definir a hora 12:00:00 (em formato de 24hrs)
  //rtc.setDate(16,12,2018);
  Serial.begin(9600);     //Inicialização da comunicação serial
  dht.begin();
  lcd.begin (16,2);       //Inicialização do LCD
  
  pinMode(gatilho,OUTPUT);
  digitalWrite(gatilho,LOW);// Deixa pino em LOW
  delayMicroseconds(10);
  // Configura pino ECHO como entrada
  pinMode(echo,INPUT);
 
  pinMode(pinCS, OUTPUT);//SD
  pinMode(3,OUTPUT);//LED ERRO

  // Inicialização do cartão SD
  if (SD.begin())
  {
    Serial.println("O cartao SD esta pronto para ser usado.");
    lcd.setCursor(0,1);
    lcd.print("SD PRONTO !");
  } else
  {
    Serial.println("A inicializacao do cartao SD falhou");
    lcd.setCursor(0,1);
    lcd.print("SD FALHOU !");
    digitalWrite(7, LOW);
    digitalWrite(3,HIGH);
    return;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  int divisor,basetempo,amostra=150;//tamanho da amostra das medidas de nível de água.
  float nivelagua,media,mediaresistencia,mediaumidade,mediatemperatura,h,t; 
  long microsec;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Média dos valores de temperatura e umidade
    for(int i=0;i<(amostra/5);i++){
    h = dht.readHumidity();
    t = dht.readTemperature();
    Serial.print(h);
    Serial.print(",");
    Serial.println(t);
    mediaumidade=((mediaumidade+h)/2);
    mediatemperatura=((mediatemperatura+t)/2);
    delay(1000);
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 divisor =10000/(sqrt((mediatemperatura+273.15)/273.15) * 331.45) ;
  for(int i=0;i<amostra;i++){//faz a media das leituras obtidas pelo sensor ultrassônico.
     // disparar pulso ultrassônico
    digitalWrite(gatilho, HIGH);
    delayMicroseconds(10);
    digitalWrite(gatilho, LOW);
    tempo = pulseIn(echo, HIGH);// medir tempo de ida e volta do pulso ultrassônico
    distancia_cm = tempo / divisor / 2;//mostra a distancia lida pelo ultrassonico variando a velocidade do som de acordo com a temperatura lida pelo DHT
    Serial.println(distancia_cm);
    
    while(distancia_cm >120||distancia_cm<0 && i<amostra){
        digitalWrite(gatilho, HIGH);
        delayMicroseconds(10);
        digitalWrite(gatilho, LOW);
        // medir tempo de ida e volta do pulso ultrassônico
        tempo = pulseIn(echo, HIGH);
        // calcular as distâncias em centímetros
        distancia_cm = tempo /divisor / 2;
        delay(190);
        i=i+1;
    }
    if(distancia_cm>120||distancia_cm<0){//se a leitura for maior que 150 cm, isso representa um erro e a medida não é considerada na média.
      media=media;
    }
    else//se for menor ela é utilizada para calcular a media.
    media =((media+distancia_cm)/2);//faz a media a cada vez q o valor lido pelo sensor é satisfatório.
    delay(190);
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  nivelagua = 140 - media; //determina o nivel do igarapé(altura do sensor em relação ao nivel zero - a leitura do sensor).

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEMPO = rtc.getTime();
while((TEMPO.min) % 5 != 0){
  TEMPO = rtc.getTime();
  delay(1000);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.print(rtc.getDOWStr());//mostra o dia da semana
  Serial.print(",");
  Serial.print(rtc.getDateStr());//mostra a data
  Serial.print(",");
  Serial.print(rtc.getTimeStr());//mostra a hora
  Serial.print(", Nivel igarape:");
  Serial.print(nivelagua);//mostra o nivel do igarapé.
  Serial.print(", Temperatura:");
  Serial.print(mediatemperatura);//mostra a temperatura.
  Serial.print(", Umidade:");
  Serial.println(mediaumidade);//mostra a umidade.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//mostra alguns valores no Display LCD
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,1);
  lcd.print(rtc.getTimeStr(FORMAT_SHORT));//mostra a hora
  lcd.print(" N:");
  lcd.print(nivelagua);//mostra o nivel do igarapé
  lcd.print("cm");
  lcd.setCursor(0,0);
  lcd.print(mediatemperatura);//mostra a temperatura
  lcd.print((char)223);//mostra o simbolo de graus °
  lcd.print(" U:");
  lcd.print(mediaumidade);//mostra a umidade
  lcd.print("%");  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String dia = rtc.getDateStr(FORMAT_SHORT);
dia.setCharAt( 2 , '-');
dia.setCharAt( 5 , '-');
String nomearquivo =String(dia + ".txt");
  myFile = SD.open(nomearquivo, FILE_WRITE);// abre o arquivo de texto onde são salvas as informações;
  if (myFile) {
    myFile.print(rtc.getDOWStr());//salva o dia da semana
    myFile.print(",");
    myFile.print(rtc.getDateStr());//salva a data
    myFile.print(" ");
    myFile.print(rtc.getTimeStr(FORMAT_SHORT));//salva a hora
    myFile.print(",Nivel igarape:,");
    myFile.print(nivelagua);//salva o nivel do igarapé
    myFile.print(",Temperatura:,");
    myFile.print(mediatemperatura);//salva a temperatura
    myFile.print(",Umidade:,");
    myFile.println(mediaumidade);//salva a umidade
    myFile.close(); // fecha o arquivo de texto.
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Se o arquivo não abrir, printa o erro:
  else {
    Serial.println("erro ao abrir o arquivo test.txt");
    lcd.setCursor(0,1);
    lcd.print("ERRO NO ARQUIVO!");
    digitalWrite(3,HIGH);//liga o LED de erro
  }
}
