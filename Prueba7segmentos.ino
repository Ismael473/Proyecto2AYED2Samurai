// Pin configuration
const int segA = 7;
const int segB = 10;
const int segC = 11;
const int segD = 13;
const int segE = 12;
const int segF = 6;
const int segG = 9;
const int segDP = 8;
const int digit1Pin = 2;  // DS1 (unidades)
const int digit2Pin = 3;  // DS2 (decenas)
const int buzzerPin = 4;  // Buzzer

// Array for number display for common cathode
const byte numbers[10][7] = {
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},   // 0
  {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW},       // 1
  {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH},    // 2
  {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH},    // 3
  {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH},     // 4
  {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH},    // 5
  {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH},   // 6
  {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW},      // 7
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH},  // 8
  {HIGH, HIGH, HIGH, LOW, LOW, HIGH, HIGH}     // 9
};

int currentNumber = 0;
unsigned long previousMillis = 0;
bool buzzerActive = false;

void setup() {
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(segDP, OUTPUT);
  pinMode(digit1Pin, OUTPUT);
  pinMode(digit2Pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "incrementNumber") {
      incrementNumber();
    } else if (command == "soundDmg") {
      soundDmg();
    }
  }

  displayNumber(currentNumber);
  delay(10);

  if (buzzerActive && millis() - previousMillis >= 1000) {
    digitalWrite(buzzerPin, LOW);
    buzzerActive = false;
  }
}

void incrementNumber() {
  currentNumber++;
  if (currentNumber > 99) {
    currentNumber = 0;
  }
}

void displayNumber(int number) {
  int tens = number / 10;
  int ones = number % 10;

  // Display tens (decenas)
  digitalWrite(digit2Pin, HIGH);
  showDigit(tens);
  delay(5);
  clearSegments();

  // Display ones (unidades)
  digitalWrite(digit1Pin, HIGH);
  showDigit(ones);
  delay(5);
  clearSegments();
}

void showDigit(int num) {
  digitalWrite(segA, numbers[num][0]);
  digitalWrite(segB, numbers[num][1]);
  digitalWrite(segC, numbers[num][2]);
  digitalWrite(segD, numbers[num][3]);
  digitalWrite(segE, numbers[num][4]);
  digitalWrite(segF, numbers[num][5]);
  digitalWrite(segG, numbers[num][6]);
}

void clearSegments() {
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
  digitalWrite(digit1Pin, LOW);
  digitalWrite(digit2Pin, LOW);
}

void soundDmg() {
  digitalWrite(buzzerPin, HIGH);
  previousMillis = millis();
  buzzerActive = true;
}
