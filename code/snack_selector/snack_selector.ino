int switchPinRev = 7;     //Input from the switch when in the Forward position
int switchPinFwd = 2;     //Input from the switch when in the Forward position
int potentiometerIn;      //variable to hold the potentiometer input

int fwdPin = 5;           //Logic level output to the H-Bridge (Forward)
int revPin = 6;           //Another logic level output to the H-Bridge (Reverse
void setup() {
  // put your setup code here, to run once:
  pinMode(switchPinRev, INPUT_PULLUP);
  pinMode(switchPinFwd, INPUT_PULLUP);


  pinMode(fwdPin, OUTPUT); //Set the forward pin to an output
  pinMode(revPin, OUTPUT); //Set the forward pin to an output
}

void loop() {
  // put your main code here, to run repeatedly:
  potentiometerIn = analogRead(A0);
  int output = potentiometerIn / 4; //divide the potentiometer input by 4 so it can be used in the AnalogWrite function
  if (digitalRead(switchPinFwd) == LOW)
  {
    //If the pin is HIGH, it must be set to forward
    analogWrite(fwdPin, output); //Output our potentiometer value on the forward pin.
    delay(5000);
    analogWrite(fwdPin, 0);
    digitalWrite(switchPinFwd,HIGH);
  }
  //else 
  if (digitalRead(switchPinRev) == LOW)
  {
    analogWrite(revPin, output); //Output our potentiometer value on the forward pin.
    delay(5000);
    analogWrite(revPin, 0);
    digitalWrite(switchPinRev,HIGH);
  }

  delay(10);
    

    
}
