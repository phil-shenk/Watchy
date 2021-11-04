#include "Watchy_Phil.h"

WatchyPhil watchy;

void setup(){
  Serial.begin(9600);
  watchy.init();
}

void loop(){}
