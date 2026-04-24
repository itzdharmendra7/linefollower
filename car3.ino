// ===== SENSOR PINS =====
int sensorPins[10] = {34,35,32,33,25,26,27,14,12,13};

// ===== MOTOR PINS =====
#define ENA 5
#define IN1 18
#define IN2 19

#define ENB 17
#define IN3 16
#define IN4 4

// ===== BUTTONS =====
#define START_BTN 21
#define STOP_BTN 22

// ===== SPEED =====
int baseSpeed = 150;
bool running = false;

void setup() {

  Serial.begin(115200);

  // Sensor setup
  for(int i=0;i<10;i++){
    pinMode(sensorPins[i], INPUT);
  }

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  ledcAttach(ENA, 1000, 8);
  ledcAttach(ENB, 1000, 8);

  // Buttons
  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(STOP_BTN, INPUT_PULLUP);

  stopMotors();
}

// ===== MOTOR FUNCTIONS =====
void moveForward(int leftSpeed, int rightSpeed){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(ENA, leftSpeed);
  ledcWrite(ENB, rightSpeed);
}

void stopMotors(){
  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);
}

// ===== READ SENSORS =====
int readLinePosition() {

  int weights[10] = {-4,-3,-2,-1,0,0,1,2,3,4};
  int sum = 0;
  int count = 0;

  for(int i=0;i<10;i++){
    int val = digitalRead(sensorPins[i]);

    if(val == LOW){   // black detected
      sum += weights[i];
      count++;
    }
  }

  // If all sensors see black → T strip detected
  if(count == 10){
    return 999;
  }

  if(count == 0){
    return 888; // line lost
  }

  return sum / count;
}

// ===== LOOP =====
void loop() {

  // START BUTTON
  if(digitalRead(START_BTN) == LOW){
    running = true;
    delay(300);
  }

  // STOP BUTTON (Kill Switch)
  if(digitalRead(STOP_BTN) == LOW){
    running = false;
    stopMotors();
  }

  if(!running) return;

  int position = readLinePosition();

  // END T strip
  if(position == 999){
    stopMotors();
    running = false;
    return;
  }

  // Line lost
  if(position == 888){
    moveForward(80, 80);
    return;
  }

  // ===== LOGIC BASED TURNING =====
  int leftSpeed = baseSpeed;
  int rightSpeed = baseSpeed;

  if(position < 0){           // line is left
    leftSpeed = baseSpeed - (abs(position) * 25);
  }
  else if(position > 0){      // line is right
    rightSpeed = baseSpeed - (abs(position) * 25);
  }

  moveForward(leftSpeed, rightSpeed);
}