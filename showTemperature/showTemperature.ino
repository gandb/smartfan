


const int MIN_TEMPERATURA = 20;
const int MAX_TEMPERATURA = 32;

const int PINO_ZC = 2; 
const int PINO_DIM = 9;
const int LED_UNIDADE = 12;
const int LED_DECIMAL = 13;
const int PINO_TEMPERATURE_SENSOR = A0;

int sensorTemperatura = 0;
int temperaturaDecimal = 0;
int temperaturaUnidade = 0;
int percent =100;

void setup()
{
  Serial.begin(9600);

  pinMode(LED_DECIMAL, OUTPUT);
  pinMode(PINO_TEMPERATURE_SENSOR, INPUT);
  pinMode(LED_UNIDADE, OUTPUT);
  pinMode(PINO_DIM, OUTPUT);
  attachInterrupt(digitalPinToInterrupt( PINO_ZC), zeroCross, RISING);// chama a funçao zerocross quando mudar de estado high-low
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

 
void handShake()
{
  
  int counter;

  Serial.println("Inicio do handshake");
 
  for (counter = 0; counter < 5; ++counter) {
    digitalWrite(LED_DECIMAL, HIGH);
    digitalWrite(LED_UNIDADE, HIGH);
    delay(200); // Wait for 200 millisecond(s)
    digitalWrite(LED_UNIDADE, LOW);
    digitalWrite(LED_DECIMAL, LOW);
    delay(200); // Wait for 200 millisecond(s)
  }
  delay(2000); // Wait for 2000 millisecond(s)
  
  //fim do handshake
  
}

void updateTemperature()
{
  
  Serial.println("Atualizando temperatura");
  

  //getting the voltage reading from the temperature sensor
  int reading = analogRead(A0);
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  // print out the voltage
  Serial.print(voltage); Serial.println(" volts");

  // now print out the temperature
  sensorTemperatura  =(int)( (voltage - 0.5) * 100) ;  //converting from 10 mv per degr

  
  Serial.println(sensorTemperatura); 
  
  bool isNegativo = sensorTemperatura < 0 ;

  if(isNegativo)
  {
    //a gente não quer trabalhar com negativo
    sensorTemperatura =  0 ;
  }
  
  //transformando temperatura para decimal e unidade
  temperaturaDecimal = (sensorTemperatura / 10);
  temperaturaUnidade = (sensorTemperatura % 10);
}

void showTemperature()
{
  int counter2;
  
  int contadorDecimal = 0;

  int contadorUnidade = 0;
  
  
  Serial.println("Iniciando exibicicao de temperatura");

  
  for (counter2 = 0; counter2 < 9; ++counter2) {
    if (contadorDecimal < temperaturaDecimal) {
      digitalWrite(LED_DECIMAL, HIGH);
      delay(1000); // Wait for 1000 millisecond(s)
       digitalWrite(LED_DECIMAL, LOW);
      delay(1000); // Wait for 1000 millisecond(s)
    }
      
    if (contadorUnidade < temperaturaUnidade) {
      digitalWrite(LED_UNIDADE, HIGH);
      delay(1000); // Wait for 1000 millisecond(s)
      digitalWrite(LED_UNIDADE, LOW);
      delay(1000); // Wait for 1000 millisecond(s)
    }
    contadorDecimal++;
    contadorUnidade++;
  }
}


void changeVoltage()
{
  int sensorTemperaturaCorrigido = sensorTemperatura;
  
  if(sensorTemperatura < MIN_TEMPERATURA)
  {
    sensorTemperaturaCorrigido = MIN_TEMPERATURA;
  }

  
  if(sensorTemperatura > MAX_TEMPERATURA)
  {
    sensorTemperaturaCorrigido = MAX_TEMPERATURA;
  }

  
  percent = map(sensorTemperaturaCorrigido, MIN_TEMPERATURA, MAX_TEMPERATURA, 1, 100);
   
   
  Serial.print("A percentagem da voltagem eh ");
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
