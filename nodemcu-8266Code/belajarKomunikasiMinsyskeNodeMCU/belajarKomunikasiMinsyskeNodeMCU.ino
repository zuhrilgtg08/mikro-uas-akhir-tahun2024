const int TRIGPIN = 8;          
const int ECHOPIN = 9;
long timer;
int jarak;

void setup()
{
  Serial.begin(9600);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
}

void loop()
{
  digitalWrite(TRIGPIN, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);                  
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);                   

  timer = pulseIn(ECHOPIN, HIGH);
  jarak = timer/58;
  delay(1000);

  Serial.print("Jarak = ");
  Serial.print(jarak);
  Serial.print(" cm");
  Serial.println();
}