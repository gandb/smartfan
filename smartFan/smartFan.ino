#include <TM1637Display.h> //INCLUSÃO DE BIBLIOTECA
#include <dht.h>

const int MIN_TEMPERATURA = 20;
const int MAX_TEMPERATURA = 32;

const int PINO_ZC =2; 
const int PINO_DIM = 9;
const int pinoDHT11 = A2; //PINO ANALÓGICO UTILIZADO PELO DHT11

const int PESO_TEMPERATURA = 100;
const int PESO_UMIDADE = 10;

const int CLK  = 7 ;//1//PINO DIGITAL UTILIZADO PELO CLK
const int DIO = 3; //PINO DIGITAL UTILIZADO PELO DIO

const uint8_t C = 0b00111001;
const uint8_t H = 0b01110110;
const uint8_t EMPTY = 0b00000000;
const uint8_t MINUS = 0b01000000;

/*
 * 
 *   A
 * ____
 * |F  | B
 * __G__
 * |E  | C
 * _____
 * D
 */ 
const uint8_t UP= 0b00000001;
const uint8_t UP_LEFT = 0b00100000;
const uint8_t DOWN_LEFT = 0b00010000;
const uint8_t DOWN = 0b00001000;
const uint8_t DOWN_RIGHT = 0b00000100;
const uint8_t UP_RIGHT = 0b00000010;


int percent =100;

dht DHT; //VARIÁVEL DO TIPO DHT

TM1637Display display(CLK, DIO); //PPASSA OS PARÂMETROS PARA UMA FUNÇÃO DA BIBLIOTECA TM1637Display
 
void setup()
{
  Serial.begin(9600);

  pinMode(PINO_DIM, OUTPUT);
  attachInterrupt(digitalPinToInterrupt( PINO_ZC), zeroCross, RISING);// chama a funçao zerocross quando mudar de estado high-low
  display.setBrightness(0x0f); //DEFINE A INTESIDADE LUMINOSA DOS SEGMENTOS DO DISPLAY
}

void setFrameAnimation(uint8_t* animation)
{
  const int SPEED_ANIMATION = 85;
  
  display.setSegments(animation);
  delay(SPEED_ANIMATION);
}

void handShake()
{
  /*
  
const uint8_t UP= 0b00000001;
const uint8_t UP_LEFT = 0b00100000;
const uint8_t DOWN_LEFT = 0b00010000;
const uint8_t DOWN = 0b00001000;
const uint8_t DOWN_RIGHT = 0b00000100;
const uint8_t UP_RIGHT = 0b00000010;
*/

  Serial.println("Iniciando processo de handshake");
  
  for (int count=0;count<3;count++)
  {
    Serial.print("Animacao ");
    Serial.println(count+1);
    
    uint8_t  animation[] = {UP, EMPTY,EMPTY,  EMPTY};
    setFrameAnimation(animation);
    animation[0] = UP_LEFT;    
    animation[1] = EMPTY;    
    setFrameAnimation(animation);
    animation[0] = DOWN_LEFT;
    setFrameAnimation(animation);
    animation[0] = DOWN;
    setFrameAnimation(animation);
    animation[0] = EMPTY;
    animation[1] = DOWN;
    setFrameAnimation(animation);
    animation[1] = EMPTY;
    animation[2] = DOWN;
    setFrameAnimation(animation);
    animation[2] = EMPTY;
    animation[3] = DOWN;
    setFrameAnimation(animation);
    animation[3] = DOWN_RIGHT;
    setFrameAnimation(animation);
    animation[3] = UP_RIGHT;
    setFrameAnimation(animation);
    animation[3] = UP;
    setFrameAnimation(animation);
    animation[3] = EMPTY;
    animation[2] = UP;
    setFrameAnimation(animation);
    animation[2] = EMPTY;
    animation[1] = UP;
  }
  
}

void print(bool humity, int value)
{
  Serial.println("print:100");
  uint8_t  clear[] = {EMPTY, EMPTY,EMPTY,  EMPTY};
  display.setSegments(clear);

  Serial.println("print:150");
      
  if(humity)
  {
    if(value<0)
    {
      value=value*-1;
    }
      Serial.println("print:200");
      uint8_t  data[] = {EMPTY, EMPTY,EMPTY,  H};
      display.setSegments(data);
      
  }
  else
  {
    Serial.println("print:300");
    if(value<0)
    {
      Serial.println("print:400");
       uint8_t  data[] = {MINUS, EMPTY,EMPTY,  C};
       display.setSegments(data);
       value = value * -1;
    }
    else
    {
      Serial.println("print:500");
      uint8_t  data[] = {EMPTY, EMPTY,EMPTY,  C};
       display.setSegments(data);
    }
  }
  Serial.println("print:600");

       
  display.showNumberDec(value,false,2,1); //IMPRIME NO DISPLAY A CONTAGEM ATUAL         

  
}

void zeroCross()  {
  if (percent>100) percent=100;
  if (percent<0) percent=0;
  long t1 = 8200L * (100L - percent) / 100L;      
  delayMicroseconds(t1);   
  digitalWrite(PINO_DIM, HIGH);  
  delayMicroseconds(6);      // t2
  digitalWrite(PINO_DIM, LOW);   
}

 

void updateTemperature()
{
  
  Serial.println("Atualizando temperatura");
  
  DHT.read11(pinoDHT11); //LÊ AS INFORMAÇÕES DO SENSOR

  Serial.print("Temperatura lida ");
  Serial.println(DHT.temperature );

  
  Serial.print("Umidade lida ");
  Serial.println(DHT.humidity);
}

void showTemperature()
{

   print(true,DHT.humidity);
  
   delay(5000); 

   
   print(false,DHT.temperature);
  
   delay(2000);
 
}


void changeVoltage()
{

  /*
   *  (DHT.temperature % 10);

  humidityDecimal = (DHT.humidity / 10);
  humidityUnidade = (DHT.humidity % 10);
  
   */

  int temperatureQuoficient = ((DHT.temperature-MIN_TEMPERATURA)*PESO_TEMPERATURA) ;
  int humidityQuoficient = DHT.humidity * PESO_UMIDADE;
  
  percent = (temperatureQuoficient+humidityQuoficient)/(MAX_TEMPERATURA-MIN_TEMPERATURA);
   
   
  Serial.print("A percentagem da voltagem eh ");
  Serial.println(percent);

  if(percent<0)
  {
    percent=0;
  }
  else if(percent > 100)
  {
    percent=100;
  }
  
  Serial.print("A percentagem corrigida  da voltagem eh ");
  Serial.println(percent);

  delay(30*1000); //30s
  delay(30*1000); //30s
   
 
}

void loop()
{
  handShake();
  updateTemperature();
  showTemperature();
  changeVoltage();
}
