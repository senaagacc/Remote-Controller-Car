// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;



String message;
String myString;

// for joystick
int xvalue;
int yvalue;
int buttonf;
int xdata;
int ydata;
int bfdata; // buttonflag

// for L298N motor driver
int ena = 5;
int enb = 6;

int in1 = 8;
int in2 = 9;
int in3 = 10;
int in4 = 12;


void setup()
{
  delay(100);
  Serial.begin(9600); // Debugging only
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(ena, 0);
  analogWrite(enb, 0);
  rf_driver.init();
  delay(1000);


  Serial.println("setup");

  // Initialise the IO and ISR


}
bool signal_check;
int counter = 0;
void loop()
{
 
  uint8_t buf[12];
  uint8_t buflen = sizeof(buf);

  signal_check = false;
  if (rf_driver.recv(buf, &buflen)) // Non-blocking
  {
    int i;
    signal_check = true;
    counter = 0;


    // Message with a good checksum received, dump it.

    for (i = 0; i < buflen; i++)
    {
      char c = (buf[i]);
      message = message + c ; // make a message from the received characters
    }
    myString = message;
    //Serial.print("message: ");
    //Serial.println(message);
    

    String l = getValue(myString, ',', 0); // xvalue
    String m = getValue(myString, ',', 1); // yvalue
    String n = getValue(myString, ',', 2); // buttonf


    xdata = l.toInt();
    ydata = m.toInt();
    bfdata = n.toInt();


      Serial.println("*********from Transmitter************");
      Serial.print("xdata: ");
      Serial.println(xdata);
      Serial.print("ydata: ");
      Serial.println(ydata);
      Serial.println(bfdata);

    /*
      when the joystic is at normal position we get
      x = 450 to 550
      y= 450 to 550
      for the these values we want the robot not to move
    */

    if ( ((xdata >= 480) && (xdata <= 510)) && ( (ydata >= 480) && (ydata <= 510) ) )
    {
      Serial.println("Stopped");
      analogWrite(ena, 0);
      analogWrite(enb, 0);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }

    // if button on the joystick is pressed

    if (bfdata == 0)
    {
      analogWrite(ena, 0);
      analogWrite(enb, 0);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);

    }

    /* for the forward movement
        the value of the x increases above 550 to 1023 while the value of the y statys between 450 and 550
    */

    if ( ((ydata > 550) && (ydata <= 1050)) && ( (xdata >= 480) && (xdata <= 550) ) )
    {
      Serial.println("forward");
      int xmapped = map(xmapped, xdata, 530, 255, 0);
      int ymapped = map(ymapped, xdata, 530, 255, 0);
      
      analogWrite(ena, ymapped);
      analogWrite(enb, ymapped);

      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }


    /* for the Reverse movement
        the value of the x increases above 550 to 1023 while the value of the y statys between 450 and 550
    */

    if ( ((ydata >= 0) && (ydata < 480)) && ( (xdata >= 480) && (xdata <= 550) ) )
    {
      Serial.println("reverse");
      int xmapped = map(xmapped, xdata, 479, 255, 0);
      int ymapped = map(ymapped, xdata, 479, 255, 0);
      analogWrite(ena, ymapped);
      analogWrite(enb, ymapped);

      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);

    }

    // for right movement
    if (  ( (xdata > 550) && (xdata <= 1033) ) && ( bfdata == 1) )
    {
      Serial.println("Right");
      int xmapped = map(xmapped, xdata, 1023, 255, 0);
      analogWrite(ena, xmapped);
      analogWrite(enb, 0);

      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);

    }

    // for left movement
    if (  ( (xdata >= 0) && (xdata <= 479) && ( bfdata == 1) ) )
    {
      Serial.println("left");
      int xmapped = map(xmapped, xdata, 479, 255, 0);
      analogWrite(ena, 0);
      analogWrite(enb, xmapped);

      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);


    }

    if ( ((ydata > 479) && (ydata <= 550)) && ( (xdata >= 479) && (xdata <= 550) ) )
    {
      Serial.println("on hold");

      analogWrite(ena, 0);
      analogWrite(enb, 0);

      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }

    message = "";
  }

  
    if(!signal_check)
    {
      counter++;
      if(counter > 20)
      {
        Serial.println("////////////////////sinyal gitti////////////////////");
        //rgb şerit led ve ekran(ekran arabanın üstünde pek göüzkmeyebilir uzaktan)
      }
      
    }

  
  delay(500);
}


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
