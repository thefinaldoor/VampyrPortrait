/* code description here...


*/
#define DEBOUNCE_DLY 50       // Number of mS before a switch state is accepted

const int lightPin1 = A1;  //pin for portrait 1 attached to A1
const int lightPin2 = A2;  //pin for portrait 2 attached to A2
const int relayPin1 = 6;   //pin for relay outlet attached to pin 6 NO
const int relayPin2 = 7;   //pin for relay outlet attached to pin 7 NC
const int soundPin = 8;    //pin to BooTunes
const int overridePin = 9;  //Override puzzle completion

int sensorPin1 = A1;//select the input pin for portrait 1
int sensorPin2 = A2;//seleect the input pin for portrait 2

int sensorValue1 = 0;// variable to store the value coming from A1 portrait 1
int sensorValue2 = 0;// variable to store the value coming from A2 portrait 2

bool OverrideButtonState;


void setup()
{
    Serial.begin(9600);  //Begin serial communcation
    pinMode (sensorPin1,INPUT);//set the state of A1 as INPUT
    pinMode (sensorPin2,INPUT);//set the state of A2 as INPUT
    pinMode (relayPin1,OUTPUT);//set the state of pin 6 as OUTPUT
    pinMode (relayPin2,OUTPUT);//set the state of pin 7 as OUTPUT
    pinMode (soundPin,OUTPUT);
    pinMode (overridePin, INPUT_PULLUP); //Manual Solve Button
    digitalWrite(relayPin1,LOW);//turn on the relay outlet (mag lock)
    digitalWrite(relayPin2,LOW);//turn off the relay outlet (lights)
    digitalWrite(soundPin,LOW);
}

void loop()
{
    sensorValue1 = analogRead(sensorPin1);//Read the value of portrait 1    
    sensorValue2 = analogRead(sensorPin2);//Read the value of portrait 2
    
    Serial.println("Portrait 1");
    Serial.println(analogRead(lightPin1)); //Write the value of the photoresistor to the serial monitor.
    delay(100); //short delay for faster response to light.

    Serial.println("Portrait 2");
    Serial.println(analogRead(lightPin2)); //Write the value of the photoresistor to the serial monitor.
    delay(100); //short delay for faster response to light.

    //code to power high voltage electrical outlet when both photoresistors are covered
    //if
    //(sensorValue1<599 && sensorValue2<599)
    //{
    //  digitalWrite(relayPin1,LOW);//turn on the relay outlet
    //  digitalWrite(relayPin2,LOW);//turn off the relay outlet
    //}
    if
    (sensorValue1>600 && sensorValue2>600)
    {
      Serial.println("eyes covered");//print eyes covered to serial monitor
      SolvePuzzle();
    }
    Serial.print("Override Pin: ");
    Serial.println(digitalRead(overridePin));
    if (DebounceSW(overridePin) == true)
    { 
      Serial.println("Override Triggered");
      //Serial.print("Override Pin: ");
      //Serial.println(digitalRead(overridePin));
      SolvePuzzle();
      //OverrideButtonState = !OverrideButtonState;
      //if (OverrideButtonState) { }
    }
}

void SolvePuzzle() {
  digitalWrite(relayPin1,HIGH);//turn off the relay outlet
  digitalWrite(relayPin2,HIGH);//turn on the relay outlet
  digitalWrite(soundPin,HIGH);
  delay(100);
  digitalWrite(soundPin,LOW);
  delay(10000);
  digitalWrite(relayPin1,LOW);
  digitalWrite(relayPin2,LOW);
  //OverrideButtonState = 0;
}
   
bool DebounceSW(byte SWx)
{
  //read the passed switch twice and make sure both readings match
  //this prevents multiple triggers due to mechanical noise in 
  //the switch
  
  bool PossVal2;
  bool PossVal = !digitalRead(SWx);   //invert the reading due to the use of pullups
  
  while(true)
  {
    delay(DEBOUNCE_DLY);          //delay by the debounce amount
    PossVal2 = !digitalRead(SWx);     //re-read the switch
    
    if (PossVal == PossVal2)        //if the two reads are the same
    {
      return (PossVal);         //return the read value
    }
    
    //this code will only execute if the two reads did not match
    //Now read the pin again and look for a match.
    //If the button is cycling very fast, it is possible the code
    //will deadlock here.  This is a very slim possibility
    
    PossVal = !digitalRead(SWx);      //re-take the first reading
    //and loop back to the delay
  }
  return (PossVal);   //this line is never executed, but makes the compiler happy.
}
