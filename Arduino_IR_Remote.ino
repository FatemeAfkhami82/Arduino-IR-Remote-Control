#include <IRremote.h>

// پین‌ها
#define ENA 6
#define IN1 5
#define IN2 4
#define IR_PIN 7

IRrecv irrecv(IR_PIN);
decode_results results;

int speedValue = 180; // مقدار اولیه PWM

// کد دکمه‌ها (جایگزین با دکمه‌های ریموت خودتان)
#define KEY_2  0xFF18E7  // جلو
#define KEY_8  0xFF42BD  // عقب
#define KEY_5  0xFF38C7  // ایست
#define KEY_4  0xFF10EF  // سرعت کمتر
#define KEY_6  0xFF5AA5  // سرعت بیشتر

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("IR Code: ");
    Serial.println(results.value, HEX);

    switch (results.value) {
      case KEY_2:  // جلو
        motorForward();
        break;

      case KEY_8:  // عقب
        motorBackward();
        break;

      case KEY_5:  // ایست
        motorStop();
        break;

      case KEY_4:  // سرعت کمتر
        speedValue -= 40;
        if (speedValue < 60) speedValue = 60;  // حداقل سرعت
        setSpeed();
        break;

      case KEY_6:  // سرعت بیشتر
        speedValue += 40;
        if (speedValue > 255) speedValue = 255;  // حداکثر سرعت
        setSpeed();
        break;

      case 0xFFFFFFFF:
        break;
      default:
        break;
    }
    irrecv.resume(); // آماده خواندن کد بعدی شو
  }
}

void motorForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedValue);
  Serial.println("Forward");
}

void motorBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speedValue);
  Serial.println("Backward");
}

void motorStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  Serial.println("Stop");
}

void setSpeed() {
  analogWrite(ENA, speedValue);
  Serial.print("Speed: ");
  Serial.println(speedValue);
}
