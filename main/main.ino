/*
  This application let you play with logic circuits 

  Uses 3 sensors to determine the circuit and 2 buttons as inputs for each circuit

  Circuits:
    0: Wire
    1: Not
    2: And
    3: Or
    4: Xor
    5: Latch
    6: Not Implemented
    7: Not Implemented
*/

#define LED      LED_BUILTIN
#define BUTTON_A 2
#define BUTTON_B 3
#define SENSOR_A A0
#define SENSOR_B A1
#define SENSOR_C A2

typedef struct Button {
  int  pin;
  bool pullUp;
};

typedef bool (*LevelFunc) (const bool inputA, const bool inputB);

bool latchValue = false;

int sensorAValue = 0;
int sensorBValue = 0;
int sensorCValue = 0;

bool buttonAValue = false;
bool buttonBValue = false;

Button buttonA = {BUTTON_A, true};
Button buttonB = {BUTTON_B, true};

void setup() {
  //start serial connection
  Serial.begin(9600);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() {
  buttonAValue = buttonRead(buttonA);
  buttonBValue = buttonRead(buttonB);
  
  sensorAValue = analogRead(SENSOR_A);
  sensorBValue = analogRead(SENSOR_B);
  sensorCValue = analogRead(SENSOR_C);

  LevelFunc lvl = getLevel(sensorAValue, sensorBValue, sensorCValue);
  
  bool lvlOutput = lvl(buttonAValue, buttonBValue);

  int ledState = getLedState(lvlOutput);

  digitalWrite(LED, ledState);
}

int getLedState(bool value) {
  return value ? HIGH : LOW;
}

bool buttonRead(Button button) {
    int buttonState = digitalRead(button.pin);
    int restValue = button.pullUp ? HIGH : LOW;
    return buttonState == restValue ? false : true;
}

LevelFunc levels[] = {levelWire, levelNot, levelAnd, levelOr, levelXor, levelLatch, levelWire, levelWire};

LevelFunc getLevel(int sensorA, int sensorB, int sensorC) {
    int P0 = sensorA > 1000 ? 1 : 0;
    int P1 = sensorB > 1000 ? 1 : 0;
    int P2 = sensorC > 1000 ? 1 : 0;
    
    int levelSelector = 4*P2 + 2*P1 + P0;

    return levels[levelSelector];
}

bool levelWire(const bool inputA, const bool inputB) {
  Serial.println("wire");
  return inputA;
}

bool levelNot(const bool inputA, const bool inputB) {
    Serial.println("not");
    return !inputA;
}

bool levelOr(const bool inputA, const bool inputB) {
    Serial.println("or");
    return inputA or inputB;
}

bool levelAnd(const bool inputA, const bool inputB) {
    Serial.println("and");
    return inputA and inputB;
}

bool levelXor(const bool inputA, const bool inputB) {
    Serial.println("xor");
    return inputA xor inputB;
}

bool levelLatch(const bool set, const bool reset) {
    Serial.println("latch");
    if (set) {
      latchValue = true;
    } else if (reset) {
      latchValue = false;
    }
    return latchValue;
}
