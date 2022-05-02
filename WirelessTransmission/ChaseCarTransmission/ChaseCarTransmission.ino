//Group 8

//needed for transceiver
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(10, 9); // CE, CSN for NRF24L01

const byte address[6] = "00001";


//to convert the data to bytes
union BtoF
{
  byte b[16]; //byte size
  float fval;
}u;

//declare variables
const int bufferSize=16;
byte buf[bufferSize];
float received;
float optimizedSpeed;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //set up transmission pins
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  //runs if there is something to receive from matlab
  if (Serial.available()>0)
    {
      optimizedSpeed=readMat();
      radio.write(&optimizedSpeed, sizeof(optimizedSpeed));
      delay(0.1);
    }
  // Check whether there is data to be received
  if (radio.available()) 
  {
    //read data from the driver car
    radio.read(&received, sizeof(received)); // Read the data sent
    writeToMat(received); //send this data to the matlab model
  }
}

//function to read the data from the com port which will be from the matlab model
float readMat()
{
  int reln=Serial.readBytesUntil("\r\n",buf,bufferSize);
  for (int i=0; i<bufferSize; i++)
  {
    u.b[i]=buf[i];
  }
  float output=u.fval;
  return output;
}

//function to send data as bytes to matlab via the serial port
void writeToMat(float number)
{
  byte *b=(byte *) &number;
  Serial.write(b,4);
  Serial.write(13); //"r" CR
  Serial.write(10); //"\n" LF
}
