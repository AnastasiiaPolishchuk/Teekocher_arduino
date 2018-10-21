#include <CheapStepper.h>
#include "OneButton.h"

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_D1  37
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_G1  49
#define NOTE_A1  55
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_D2  73
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_G2  98
#define NOTE_A2  110
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_D3  147
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_D6  1175
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_G6  1568
#define NOTE_A6  1760
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_D7  2349
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_G7  3136
#define NOTE_A7  3520
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_D8  4699

// notes in the melody:
int melody[] = {
NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4,
NOTE_F4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_D4,
0, NOTE_A3, NOTE_C4, NOTE_B3, NOTE_D4, NOTE_B3, NOTE_E4, NOTE_F4,
NOTE_F4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4,
NOTE_D4, 0, 0, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_F4,
NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4,
NOTE_A4, NOTE_D4, 0, NOTE_D4, NOTE_E3, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4,
NOTE_D4, 0, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_D4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
4,8,4,8,4,8,8,8,8,4,8,4,8,4,8,8,8,8,4,8,4,8,
4,8,8,8,8,4,4,8,8,4,4,8,8,4,4,8,8,
8,4,8,8,8,4,4,8,8,4,4,8,8,4,4,8,4,
4,8,8,8,8,4,4,8,8,4,4,8,8,4,4,8,8,
8,4,8,8,8,4,4,4,8,4,8,8,8,4,4,8,8
};

//Richtungszuweisung des Motors
//boolean moveClockwise =true;
int buttonPin = 7;
int dreher = 0;
long timerTime = 0;
long prevTime = 0;

CheapStepper stepper;
OneButton button(buttonPin, false);

//int empfindlichkeit = 200;    // empfindlichkeit speichert wie oft der Taster abgefragt wird


void setup() {
  pinMode(buttonPin, INPUT);
  button.attachClick(clickedIt);

 for (int i = 2; i <= 6; i++)
  {
    pinMode(i , OUTPUT);   //Digital Pins 1-8 werden als OUTPUTs gesetzt
  }
  pinMode(12 , OUTPUT);   //Digital Pins 1-8 werden als OUTPUTs gesetzt
  pinMode(13 , OUTPUT);   //Digital Pins 1-8 werden als OUTPUTs gesetzt
  pinMode(0, OUTPUT);

  prevTime = 0;
}

int a[] = {1,0,1,1,0,1,1,1,1,1};
int b[] = {1,1,1,1,1,0,0,1,1,1};
int c[] = {1,1,0,1,1,1,1,1,1,1};
int d[] = {1,0,1,1,0,1,1,0,1,1};
int e[] = {1,0,1,0,0,0,1,0,1,0};
int f[] = {1,0,0,0,1,1,1,0,1,1};
int g[] = {0,0,1,1,1,1,1,0,1,1};


void updateDisplay(int zahl){

  digitalWrite(2, d[zahl]); // unten         d
  digitalWrite(3, c[zahl]); // unten rechts  c
  digitalWrite(4, e[zahl]); // unten links   e
  digitalWrite(5, b[zahl]);  // rechts       b
  digitalWrite(6, a[zahl]);   // oben        a
  digitalWrite(12, f[zahl]); // links        f
  digitalWrite(13, g[zahl]); // mitte         g
  }

int calculateTeeTime(int drehAnzahl){
  double t = ((double)drehAnzahl)/1024.0;
  return (int) (t * 8.99 + 1.0);
  }

int readTeeTime(){
   int teeTime = analogRead(dreher);
   return calculateTeeTime(teeTime);
  }


void timer(){
    timerTime = readTeeTime() * 60000;
    prevTime = millis();
  }


void playMelody() {
  for (int thisNote = 0; thisNote < sizeof(melody)/sizeof(int); thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(1, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(1);
  }
}

void makeTeeTasty(){
  int i = 0;
  while (i < 6){
     for(int s=0; s < 8096; s++){
        //Eine volle Umdrehung beinhaltet 4096 Schritte
        if ( i%2 == 0){
           stepper.step(true);
        } else {
           stepper.step(false);
        }
     }
     i = i + 1;
   }
 }

void getTheTeeSack() {
  for(int s=0; s < 18096; s++){
  //Eine volle Umdrehung beinhaltet 4096 Schritte
    stepper.step(true);
  }
  playMelody();
}


void loop() {
  // put your main code here, to run repeatedly:
   button.tick();
   delay(10);

  if (timerTime == 0)
   updateDisplay(readTeeTime());
  else  {
    long t = millis();
    long diff = t - prevTime;
    prevTime = t;
    timerTime -= diff;

    updateDisplay(timerTime / 60000);

    if (timerTime <= 0) {
      timerTime = 0;
      makeTeeTasty();

      getTheTeeSack();
    }
  }
}

void clickedIt() {
  timer();
}
