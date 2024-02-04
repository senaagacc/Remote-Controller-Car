#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

const int transmit_pin = 12;
 
 String str;
 char cstr[100];
String message = "";
unsigned int mlength; // message length 
 
// for joystick
 int flag = 0; 
int buttonf = 0;
int power = 7; 
int button = 4; 
int vrx = A2; 
int vry = A1;
 
 int xdata = 0; 
 int ydata = 0; 

int start; 
int end; 
int interval; 

void setup()
{
      // Initialise the IO and ISR
    rf_driver.init();
  Serial.begin(9600);
  
  pinMode(vrx, INPUT); 
  pinMode(vry, INPUT); 
  pinMode(power, OUTPUT); 
  digitalWrite(power, HIGH); 
 
  pinMode(button, INPUT); 
  digitalWrite(button , HIGH);
  Serial.println("hi");
}
 
void loop()
{
  start = millis();
  control();
 
  if( buttonf == 1)
   {
  xdata = analogRead(vrx); 
  ydata = analogRead(vry); 
  Serial.print("xdata:");
  Serial.println(xdata);
  Serial.print("ydata:");
  Serial.println(ydata);
  Serial.println("///////////");
  message = "" + String(xdata) + "," + String(ydata) + "," + String(buttonf) ;
   Serial.print("Len:");
  Serial.println(message.length() + 1);
   }
   
   if(buttonf == 0)
   {
    message = "" + String(0) + "," + String(0) + "," + String(0) ;
   }
   
   rf_driver.send((uint8_t *)message.c_str(), message.length() + 1);
    rf_driver.waitPacketSent();
    delay(50);
                
}

 
 
void control()
{
if (( digitalRead(button) == LOW ) && (buttonf == 0))
{
 
   Serial.println(" Started");  
  buttonf = 1; 
  delay(500);
 
}
 
if (( digitalRead(button) == LOW ) && (buttonf == 1))
{
 
  Serial.println("ended"); 
  buttonf = 0; 
  delay(500); 
}
 digitalWrite(button , HIGH);
}
