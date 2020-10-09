
const int LED_DECIMAL = 13;
const int LED_UNIDADE = 12;

int sensorTemperatura = 0;


int temperaturaDecimal = 0;

int temperaturaUnidade = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(LED_DECIMAL, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(3, OUTPUT);
  pinMode(LED_UNIDADE, OUTPUT);
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
  
  
  //coletando temperatura  e convertendo para graus celsus  
  sensorTemperatura = -40 + 0.488155 * (analogRead(A0) - 20);
  
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

void loop()
{

	handShake();
  updateTemperature();
  showTemperature();  
}
