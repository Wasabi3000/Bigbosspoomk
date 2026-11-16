// Motor A connections
int enA = 9;
int in1 = 13;
int in2 = 12;
// Motor B connections
int enB = 3;
int in3 = 11;
int in4 = 10;
int trigPins[3] = {5, 6, 8};
int echoPins[3] = {4, 7, 2};
const unsigned long turn_90 = 600;
bool finishedTurning = false;
bool turnReady = true;
int turnStep = 0;

void setup() {
// put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  for(int i = 0; i < 3; i++){
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
    digitalWrite(trigPins[i], LOW);
  }
}

float read_distance(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 30000UL);

  if (duration == 0){
    return 0 ;
  }
  return duration * 0.0343 / 2.0;
}


void turn_left_degrees(float degrees) {
  if (degrees <= 0) return;

  const int turnSpeed = 70;
  unsigned long turnTime =
      (unsigned long)(turn_90 * (degrees / 90.0f));

  // หยุดก่อนเปลี่ยนทิศทาง
  stop(255);
  delay(100);

  // ปิดขับมอเตอร์ชั่วคราวขณะตั้งทิศทาง
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  // Motor A: ล้อขวาเดินหน้า
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  // Motor B: ล้อซ้ายถอยหลัง
  // กลับทิศจาก LOW, HIGH ที่ใช้ใน forward()
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, turnSpeed);
  analogWrite(enB, turnSpeed);

  delay(turnTime);
  stop(255);
}

void loop(){
  float distance_1 = read_distance(trigPins[0], echoPins[0]);
  delay(2);
  float distance_2 = read_distance(trigPins[1], echoPins[1]);
  delay(2);
  float distance_3 = read_distance(trigPins[2], echoPins[2]);
  delay(2);

  Serial.print("Ultrasonic_1 : ");
  Serial.print(distance_1);
  Serial.print("cm  ");

  Serial.print("Ultrasonic_2 : ");
  Serial.print(distance_2);
  Serial.print("cm ");

  Serial.print("Ultrasonic_3 : ");
  Serial.print(distance_3);
  Serial.println("cm ");

// ค่า 0 หมายถึงอ่านระยะไม่ได้

if (distance_1 <= 0 || distance_2 <= 0) {
  // อ่านไม่ได้
  stop(255);

} else if (distance_1 <= 15 && distance_2 <= 15 && distance_3 > 20) {
  // อยู่ในทางเดิน เห็นกำแพงสองข้าง
  turnReady = true;
  forward(65);

} else if (turnReady &&
           distance_2 >= 20 && distance_1 <= 15) {
  // ซ้ายเปิด ขวายังมีกำแพง: เลี้ยวซ้ายหนึ่งครั้ง
  stop(255);
  turn_left_degrees(70);
  stop(255);
  delay(100);

  turnReady = false;

} else if (!turnReady && distance_1 <= 15) {
  // เดินออกจากปากแยก ไม่หมุนซ้ำที่ช่องเดิม
  forward(70);

} else if (distance_1 > 25 && distance_2 > 25) {
  forward(70);

} else if (distance_1 > distance_2) {
  // หมุนขวา แล้วหยุดก่อนอ่านค่ารอบถัดไป
  turn_right_degrees(70);
  stop(255);
  delay(100);

  turnReady = false;

} else if (!turnReady && distance_1 <= 15) {
  forward(70);

} else if (distance_1 > 25 && distance_2 > 25) {
  forward(70);

} else {
  stop(255);
}
}

  void forward(int speed) {

      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);

      analogWrite(enA, speed);
      analogWrite(enB, speed);
}

  void backward(int speed) {
  // ปิดขับมอเตอร์ชั่วคราวขณะเปลี่ยนทิศ
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  // Motor A ถอยหลัง
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Motor B ถอยหลัง
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, speed);
  analogWrite(enB, speed);
}

void backward_seconds(float seconds, int speed) {

  if (seconds <= 0) return;

  stop(255);
  delay(100);

  backward(speed);
  delay((unsigned long)(seconds * 1000.0f));

  stop(255);
}

void turn_right_degrees(float degrees) {
  if (degrees <= 0) return;

  const int turnSpeed = 70;
  unsigned long turnTime = (unsigned long)(turn_90 * (degrees / 90.0f));

  stop(255);
  delay(100);

  // ปิดกำลังมอเตอร์ก่อนเปลี่ยนทิศ
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  // Motor A: ล้อขวาถอยหลัง
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Motor B: ล้อซ้ายเดินหน้า
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, turnSpeed);
  analogWrite(enB, turnSpeed);

  delay(turnTime);

  stop(255);  // หยุดเมื่อหมุนเสร็จ
}

void stop(int speed){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  analogWrite(enA, speed);
  analogWrite(enB, speed);
}