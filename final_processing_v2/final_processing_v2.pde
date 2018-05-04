      //  Final Project//
import ddf.minim.*;  // import sound library
Minim minim;
AudioPlayer player;

import processing.serial.*; // add the serial library
Serial myPort; // the serial port to monitor
int timediff;
PFont blackoak;
boolean rectOver = false;
int rectX = 100; int rectY = 30; int rectSize = 60;
int ticker = 0;
int ticker2 = 0;
int ticker3 = 0;
int randsym;
int endgame = 0;
int wintally = 0; int tietally = 0; int losetally = 0;
int playticker = 0; int lastkeypressed = 0;
color newcirclehighlight = color(249,166,2); //red

void setup() {
size(1000, 800); // set the window size, size() also sets width and height variables
// first argument = width, second argument = height
printArray(Serial.list()); // list all available serial ports
myPort = new Serial(this, Serial.list()[0], 9600); // define input port
myPort.clear(); // clear the port of any initial junk
background(0,0,0);  // dark gray background
noStroke(); //no outline
textSize(50);
blackoak = createFont("Blackoak Std",32);
  minim = new Minim(this);
  player = minim.loadFile("sample.mp3"); // name of audio file
  player.play();
}

void draw () {
  
  if (myPort.available () > 0) { // make sure port is open
  String inString = myPort.readStringUntil('\n'); // read input string
     if (inString != null) { // ignore null strings
       inString = trim(inString); // trim off any whitespace
       String[] xyRaw = splitTokens(inString, ","); // extract x & y into an array
       // proceed only if correct # of values extracted from the string:
        
          if (xyRaw.length == 4) {
          int enteredfield = int(xyRaw[0]);
          timediff = int(xyRaw[1]);
          int winbutton = int(xyRaw[3]);
          println(xyRaw[2]); //xyRaw 2 is the symbol that was sent over the port
             if (enteredfield == 1) //if hand is in field
             {
             background(75, 124, 204);
             
             fill(254, 254, 254); //white
             textFont(blackoak);
             textSize(40);
             text("Player has entered the playing field!", width/2-250, height/2-20);
                 if(timediff > 2000) // if hand has been in field for 2 sec
                 {
                     background(75, 124, 204);
                     textFont(blackoak);
                     textSize(50);
                     text("Ready", width/2-250, height/2-20);
                     if(timediff > 4000) // if hand has been in field for 4 sec
                     {
                         background(75, 124, 204);
                         text("Okay", width/2-250, height/2-20);
                         if(timediff > 5000) // if hand has been in field for 5 sec
                         {
                             background(75, 124, 204);
                             text("Let's Play!", width/2-250, height/2-20);
                             Countdown();
                         }
                     }
                 }
             }
             else { // if hand no longer in field
             background(75, 124, 204);
             //println("Where did you go?");
             fill(0, 220, 0);
             ellipse(56, 46, 25, 25); // green notification circle
             textSize(30);
             text("Wave your hand to begin",width/2-250, height/2-20);
             ticker = 0; //reset ticker, able to generate new symbol now
             }
     /////////////  After Symbol has been made  ////////////        
             if(int(xyRaw[2]) != 0){  //nonzero # sent after all servos return to open position
                 endgame = 1;  
             }  
     ////////////  End Game Screen  //////////////        
             if(endgame == 1){
               background(100,100,100);
               fill(255,255,255);
               textSize(50);
               text("Did you win?", width/2-250, height/2-20);
               textSize(20);
               text("Press a button to record your score and replay", width/2-250, height/2+10);
               if(winbutton == 1){
                 endgame = 0;
                 wintally++;
               }
               else if(winbutton == 2){
                 endgame = 0;
                 tietally++;
               }
               else if(winbutton == 3){
                 endgame = 0;
                 losetally++;
               }
             } // end of "End Game Screen" loop
          }

     }
  } // end of reading Serial Port
  
  /// Draw Play Button ///
  fill(newcirclehighlight); //red
             ellipse(width - 57, 50, 56, 56); // play/pause circle
             fill(200, 200, 200); //white
             triangle(width-70,30,width-70,70, width-50, 50); // x1, y1, x2, y2, x3, y3
             rect(width-50,32,7,36); 
             
  update(mouseX, mouseY);
    if (rectOver) 
    {
      fill(color(0,0,254)); //blue
      TallyBoard();
    }
    if(keyPressed){ //open tally board if 't' key is hit
        if (key == 't' || key == 'T') { // r for replay
          TallyBoard();
        }
    }
   if(keyPressed){
        if (key == 'p' && (lastkeypressed == 0)){
          if (playticker == 0)
              {
                player.pause();
                newcirclehighlight = color(200, 0, 50); //change it to red
                playticker = 1;
              }
              else{
                player.play();
                newcirclehighlight = color(249,166,2); //change it to orange
                playticker = 0;
              }
        lastkeypressed = 1; //1 if key was pressed the last run through the code
        }     
    }
    else{
    lastkeypressed=0;
  }
  
  fill(color(0,0,254)); //blue
  rect(rectX, rectY, rectSize, rectSize - 30);
  textSize(20);
  fill(color(254,254,254)); //white
  text("Tally", rectX+8, rectY+2, rectSize, rectSize - 30);
} //end draw()



void update(int x, int y) {
  if ( overRect(rectX, rectY, rectSize, rectSize) ) 
  { rectOver = true; }
  else {rectOver = false;}
}
boolean overRect(int x, int y, int width, int height)  {
  if (mouseX >= x && mouseX <= x+width && 
      mouseY >= y && mouseY <= y+height) {
    return true;
  } else {
    return false;
  }
}
void mouseClicked() {
  if(playticker ==1){
    newcirclehighlight = color(249,166,2); //change it to orange
    player.play();
    playticker = 0;
  }
  else if(playticker == 0){
    newcirclehighlight = color(200, 0, 50); //change it to red
    player.pause();
    playticker = 1;
  }
}
void TallyBoard() {
    fill(color(0,0,254)); //blue
    rect(rectX,rectY,rectSize+420,rectSize+200);
    textFont(blackoak);
    fill(255, 255, 255); //white
    text("Wins", rectX + 70, rectY + 30);
    text("Ties", rectX + 220, rectY + 30);
    text("Losses", rectX + 370, rectY + 30);
    if(wintally >= 1){
      for (int i=1; i <= wintally; i++){
        fill(color(250,250,250)); //white tally mark
        rect(rectX+40+i*20,rectY+60,10,40);
      }
    }
    if(tietally >= 1){
      for (int i=1; i <= tietally; i++){
        fill(color(250,250,250)); //white tally mark
        rect(rectX+190+i*20,rectY+60,10,40);
      }
    }
    if(losetally >= 1){
      for (int i=1; i <= losetally; i++){
        fill(color(250,250,250)); //white tally mark
        rect(rectX+340+i*20,rectY+60,10,40);
      }
    }
}
void Countdown() {
  if(timediff > 6500)
  {
      background(75, 124, 204);
      text("3", width/2-250, height/2-20);
      if(timediff > 7200)
      {
          background(75, 124, 204);
          text("2", width/2-250, height/2-20);
          if(timediff > 7900)
          {
              background(75, 124, 204);
              text("1", width/2-250, height/2-20);
              if(timediff > 8600)
              {
                  ticker2++;
                  background(75, 124, 204);
                  RandomSymbol();
                   if(randsym == 1){
                      text("Rock", width/2-250, height/2-20);
                    }
                    else if(randsym == 2){
                      text("Paper", width/2-250, height/2-20);
                    }
                    else if(randsym == 3){
                      text("Scissors", width/2-250, height/2-20);
                    }
                    //noLoop();
              }
          }
      }
  }
}
void RandomSymbol() {
  if(ticker == 0){
    randsym = int(random(1,4)); //randomly generates 1, 2, or 3
    ticker = 1;
    //println(randsym);                                                                            //
    myPort.write(randsym);    
  }
}

//void EndPrompt() {
//    while(endgame == true){
//        background(0,0,0);
//        text("Did you win?", width/2-250, height/2-20);
//        if(keyPressed){
//            if (key == 'r' || key == 'R') { // r for replay
//              endgame = false;
//            }
//        }
//    }
//}
