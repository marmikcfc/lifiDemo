int led = 13;

int sensorLed = 0;

int currentBit = 0;

int interval = 60; // Any smaller than this and the transmission seems to fail. The pull down resistor attached to the photoresistor may need to be reduced to allow the voltage to drop more quickly.

int currentReadPosition = 7; // 8 bit zero based position, starting at the end (ie. 7) and decrementing.

byte incomingByte;

bool verboseOutput = true;

void setup() {                
  Serial.begin(9600);

  pinMode(led, OUTPUT);     
}

void loop() {
  byte letter = 'M';

  sendChar(led, letter);

  exit(0);  //The 0 is required to prevent compile error.

}

void sendChar(int pin, char c)
{
  Serial.print("Sending char: ");
  Serial.println(c);
  
  if (verboseOutput)
    Serial.println("Bits:");

  for (int i = 7; i >= 0; --i)
  {
    bool latestBit = bitRead(c, i);

    if (verboseOutput)
      Serial.println(latestBit);

    sendBit(pin, latestBit);
    
    delay(interval);
  }
}

int sendBit(int pin, int binaryValue)
{
  if (verboseOutput)
  {
    Serial.print("Sending: ");
    Serial.println(binaryValue);
  }

  digitalWrite(pin, binaryValue);

  delay(interval);

  int reading = readBit(sensorLed);

  compareValues(binaryValue, reading);
}

int readBit(int pin)
{
  int output = sensorToBit(pin);

  if (verboseOutput)
  {
    Serial.print("Reading: ");
    Serial.println(output);
  }
  
  incomingByte = incomingByte | output<<currentReadPosition;
  
  decrementPosition();

  return output;
}

void compareValues(int sentBit, int receivedBit)
{
  if (sentBit != receivedBit)
  {
    Serial.println("Transmission error! Bits don't match");
    Serial.print("Sent: ");
    Serial.println(sentBit);
    Serial.print("Received: ");
    Serial.println(receivedBit);
  }
}

char byteToChar(byte b)
{
  return b;
}

bool sensorToBit(int pin)
{
  int reading = analogRead(pin);

  Serial.print("Received Analog Reading: ");
  Serial.println(reading);
  
  int output; 

  if (reading > (480))
    output = 1;
  else
    output = 0;
    
  return output;
}

void decrementPosition()
{
  currentReadPosition --;
  
  if (currentReadPosition < 0)
  {
    Serial.print("Reading char:");
    Serial.println(char(incomingByte));
    
    currentReadPosition = 7;
  }
}
