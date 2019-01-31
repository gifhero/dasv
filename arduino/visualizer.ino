int colPins[] = {8,9,10,11,12,13,A0,A1};
int rowPins[] = {A2,A3,2,3,4,5,6,7};
unsigned long start_t, end_t; //keeps track of time to keep a low framerate
int levels[8];

// variables for receiving data
const byte numChars = 9;      //8 bytes + null
int receivedChars[numChars];  // an array to store the received data
boolean newData = false;      //flag to read for new data

//function headers
void allOff();
void colOn( int anode, int level);
void colOff(int anode, int level);
void recNewData();
void showLevels();
void updateLevels();

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 8; i++){
    pinMode(rowPins[i], OUTPUT);
    pinMode(colPins[i], OUTPUT);
  }
  allOff();
  start_t = millis();
}

void loop(){
    recNewData();
    if(newData == true){
      updateLevels();
      newData = false;
    }
    for(int i = 0; i<8; i++){
      if(i == 0){
        colOff(rowPins[7]);
      }
      else{
        colOff(rowPins[i-1]);
      }
      colOn(rowPins[i], levels[i]);
      delayMicroseconds(2000);
    }
}


void recNewData(){
  static byte ndx = 0;
  char endMarker = '\n';
  char receivedChar;
  
  while (Serial.available() > 0 && newData == false) {
      receivedChar = Serial.read();
      if (receivedChar != endMarker) {
          receivedChars[ndx] = (int)(receivedChar - '0');
          ndx++;
          //catch if more data is sent than should be
          if (ndx >= numChars) {
              ndx = numChars - 1;
          }
      }
      else {
          receivedChars[ndx] = '\0'; // terminate the string
          ndx = 0;
          newData = true;
      }
  }  
}

void updateLevels(){
  end_t = millis();
  if(end_t-start_t > 1){ //runs 15 times per second (every 66.6 milliseconds)
    for(int i = 0; i<8; i++){
      levels[i] = receivedChars[i];
    }
  start_t = millis();
  }
}

void showLevels(){
  char charLevels[8];
  for(int i=0; i<8; i++)
    charLevels[i] = (int)(levels[i] + '0');
  Serial.println(charLevels);
}

void allOff(){
  for(int i = 0; i<8; i++){
    digitalWrite(rowPins[i], LOW);
    digitalWrite(colPins[i], LOW);
  } 
}

void colOn(int anode, int level){
  if(level){
    digitalWrite(anode, HIGH);
    for(int i = 7; i >= level; i--){
      digitalWrite(colPins[i], HIGH);
    }
  }
}

void colOff(int anode){
  digitalWrite(anode, LOW);
  for(int i = 0; i < 8; i++){
    digitalWrite(colPins[i], LOW);
  }
}
