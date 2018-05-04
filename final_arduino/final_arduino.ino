#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

int buttonState = 0; int lastButtonState = 0;
int opentControl = 2200; int closedtControl = 700;
int WinButtonState = 0; int LastWinButtonState = 0; int won = 0;
int TieButtonState = 0; int LastTieButtonState = 0;
int LoseButtonState = 0; int LastLoseButtonState = 0;
int thumbPin = 11;int pointerPin = 9;int middlePin = 12;int ringPin = 10;int pinkyPin = 13;
boolean temp1 = LOW;

boolean handin; // low if not in field of vision
int ticker = 0;
int trigPin = 7;int echoPin = 5;
long duration;int distance;
unsigned long firstenteredfield; unsigned long leftthefield; unsigned long currenttime; long timediff = 0;

int tControl = 800; // range 600:2400
int T = 20000; // 20 ms

// tControl == 500  --> pos = 0
// tControl == 1450 --> pos = 90
// tControl == 2400 --> pos = 180

int incomingsymbol = 0;   // for incoming serial data [rock-paper-scissors]

void setup() {
  pinMode(2, INPUT); //control all servos (for debugging)
  pinMode(3, INPUT); //win
  pinMode(4, INPUT); //tie 
  pinMode(8, INPUT); //lose 
  pinMode(thumbPin, OUTPUT);
  pinMode(middlePin, OUTPUT);
  pinMode(pointerPin, OUTPUT);
  pinMode(ringPin, OUTPUT);
  pinMode(pinkyPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Paper(); //Start with an Open Hand
//  strip.begin();
//  strip.setBrightness(50);
//  strip.clear();
//  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
      if(IsHandInField() == true)
      {
        if(ticker == 0){ //first time in field: 1) record time 2)change ticker
          firstenteredfield = millis();
          ticker = 1;
          //theaterChase(strip.Color(127, 127, 127), 100); // White
        }
        else {
          currenttime = millis();
          timediff = currenttime - firstenteredfield; //counts up while hand is actively in field
        }
      }
      else  // if hand is not in field
      {
        timediff = 0;
        ticker = 0;
        //strip.show(); // turn neopixel ring off
      }
 /////////////////////////////////////////////////////////////////////////////////       
      WinButtonState = digitalRead(3);
      TieButtonState = digitalRead(4);
      LoseButtonState = digitalRead(8);
        if (WinButtonState != LastWinButtonState)
        {
          if (WinButtonState == HIGH) //if button pushed
          {
            won = 1;
          }
        }
        else if (TieButtonState != LastTieButtonState)
        {
          if (TieButtonState == HIGH) //if button pushed
          {
            won = 2;
          }
        }
        else if (LoseButtonState != LastLoseButtonState)
        {
          if (LoseButtonState == HIGH) //if button pushed
          {
            won = 3;
          }
        }
        else {
          won = 0;     
        }
        LastWinButtonState = WinButtonState;
        LastTieButtonState = TieButtonState;
        LastLoseButtonState = LoseButtonState;
///////////////////////////////////////////////////////////////////////////////////
        
      buttonState = digitalRead(2);
        if (buttonState != lastButtonState)
        {
          if (buttonState == HIGH) //if button pushed
          {
            
            if (temp1 == HIGH)
            {
              for (int i = 0; i<=50; i++){
              OpenThumb();
              OpenMiddle();
              OpenPointer();
              OpenRing();
              OpenPinky();
              delay(5);
              }
              temp1 = LOW;
              //Serial.println(tControl);
            }
            else
            {
              for (int j = 0; j<=50; j++){
              CloseThumb();
              CloseMiddle();
              ClosePointer();
              CloseRing();
              OpenPinky();
              delay(5);
              }
              temp1 = HIGH;
              //Serial.println(tControl);
            }
          }
       }
      lastButtonState = buttonState;
      
      String xyz = String(IsHandInField()) + "," + String(timediff) + "," + String(incomingsymbol) + "," + String(won);
      Serial.println(xyz);
      delay(20); //remove this

      if (Serial.available() > 0) {
                // read the incoming byte:
                incomingsymbol = Serial.read();
                if(incomingsymbol != 0){
                    if(incomingsymbol == 1){
                      Rock();
                    }
                    else if(incomingsymbol == 2){
                      Paper();
                    }
                    else if(incomingsymbol == 3){
                      Scissors();
                    }
                    delay(7000);
                }
                Paper(); //reset to open hand for next time
                delay(30);
                String xyz = String(IsHandInField()) + "," + String(timediff) + "," + String(incomingsymbol) + "," + String(won);
                Serial.println(xyz);
                incomingsymbol = 0;               
       }
        
} // end loop()


bool IsHandInField()
{
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);

   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);  
   digitalWrite(trigPin, LOW);

   duration = pulseIn(echoPin, HIGH);
   distance = (duration *0.01723); // for inches just divide by 2.54

   if (distance <= 20)
   {
     return true;
   }
   else return false;
}
void Rock()
{      
        for (int i = 0; i<=50; i++){
        CloseThumb();
        CloseMiddle();
        ClosePointer();
        CloseRing();
        ClosePinky();
        delay(5);
        }
}
void Paper()
{
        for (int i = 0; i<=50; i++){
        OpenThumb();
        OpenMiddle();
        OpenPointer();
        OpenRing();
        OpenPinky();
        delay(5);
        }
}
void Scissors()
{
        for (int i = 0; i<=50; i++){
        CloseThumb();
        OpenMiddle();
        OpenPointer();
        CloseRing();
        ClosePinky();
        delay(5);
        }
}
void CloseMiddle ()
{
      tControl = closedtControl;
      digitalWrite(middlePin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(middlePin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
void OpenMiddle ()
{
      tControl = opentControl;
      digitalWrite(middlePin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(middlePin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
///////////////////////////////////////////////
void ClosePointer ()
{
      tControl = closedtControl;
      digitalWrite(pointerPin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(pointerPin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
void OpenPointer ()
{
      tControl = opentControl;
      digitalWrite(pointerPin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(pointerPin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
///////////////////////////////////////////////
void CloseRing ()
{
      tControl = closedtControl;
      digitalWrite(ringPin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(ringPin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
void OpenRing ()
{
      tControl = opentControl;
      digitalWrite(ringPin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(ringPin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
///////////////////////////////////////////////
void ClosePinky()
{
      tControl = closedtControl;
      digitalWrite(pinkyPin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(pinkyPin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
void OpenPinky()
{
      tControl = opentControl;
      digitalWrite(pinkyPin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(pinkyPin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
///////////////////////////////////////////////
void CloseThumb()
{
      tControl = closedtControl;
      digitalWrite(thumbPin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(thumbPin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
void OpenThumb()
{
      tControl = opentControl;
      digitalWrite(thumbPin, HIGH);    // Set PWM signal HIGH for tControl usec
      delayMicroseconds(tControl);
      digitalWrite(thumbPin, LOW);     // Set PWM signal LOW for remainder of period T-tcontrol
      delayMicroseconds(T-tControl);
}
//////////////////////////////////////////////////
//Theatre-style crawling lights.
//void theaterChase(uint32_t c, uint8_t wait) {
//  for (int j=0; j<5; j++) {  //do 5 cycles of chasing
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, c);    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
