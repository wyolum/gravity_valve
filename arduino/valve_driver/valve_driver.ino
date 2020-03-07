#include "pulse.h"

int v5_pins[3] = {9, 10, 11};
int v12_pins[3] = {3, 5, 6};
void setup(){
  Serial.begin(115200);
  int i;
  for(i = 0; i < 3; i++){
    pinMode(v5_pins[i], OUTPUT);
    pinMode(v12_pins[i], OUTPUT);
  }
  Serial.println("WyoLum!");
}
int count;
const int VALVE_PIN = v12_pins[0];

int bpm = 0;
float period = 0;
int last = 0;
float overhead = .035;
void beat(int hold){
  int i;
  unsigned long long start = millis();
  digitalWrite(VALVE_PIN, HIGH);
  delay(period * 1000 * hold/100.);
  digitalWrite(VALVE_PIN, LOW);
  delay(period * 1000 * (1 - hold/100.));
  //Serial.print("beat!:"); Serial.print(overhead, 6); Serial.print(' '); Serial.print(period, 6); Serial.print(' '); Serial.println(miss);
}

const int STOPPED = 0;
const int BEATING = 1;
const int OPEN = 2;
int state = STOPPED;
int hold = 50;
void interact(){
  char request[5];
  int i = 0;
  while(Serial.available() and i < 5){
    request[i++] = Serial.read();
    delay(100); // wait for more data
  }
  int length = i;
  
  if(length > 0){
    Serial.print("length: ");Serial.print(length);
    Serial.print(", echo: ");Serial.println(request);
    if(request[0] == 'h'){
      //if{
      // change hold value
      hold = String(request + 1).toInt();
      Serial.print("hold:");
      Serial.println(hold);
      //
    }
    else{
      bpm = String(request).toInt();
      if(bpm > 0){
	state = BEATING;
	period = 60./(bpm);
      }
      else if (bpm == -1){ // free flow
	state = OPEN;
	digitalWrite(VALVE_PIN, HIGH);
	Serial.println("OPEN");
      }
      else if (bpm == 0){
	bpm = 0;
	period = 0.;
	state = STOPPED;
	digitalWrite(VALVE_PIN, LOW);
      }
      Serial.print("bpm: ");
      Serial.print(bpm);
      Serial.print(", period: ");
      Serial.println(period);
    }
  }
}
void loop(){
  interact();
  if(state == BEATING){
    beat(hold);
  }
  if(state == OPEN){
  }
  else{
    digitalWrite(VALVE_PIN, LOW);
  }
}
