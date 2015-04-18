#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOCOUNT 16
#define DELAY 200

int lastvalues[SERVOCOUNT];
int minvalues[SERVOCOUNT];
int maxvalues[SERVOCOUNT];

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  // setting default values in case of missing later config
  for (int i(0); i < SERVOCOUNT; i++) {
    minvalues[i] = 150;
    maxvalues[i] = 500;
  }
  
  minvalues[0] = 130;
  maxvalues[0] = 560;
  
  for (int i(0); i < SERVOCOUNT; i++) {
    lastvalues[i] = 0;
    initServo(i);
  }
  
  wait();
}

void loop() {
  setServoDegree(0, 180);
  setServoDegree(7, 180);
  setServoDegree(15, 180);
  wait();
  
  setServoDegree(0, 0);
  setServoDegree(7, 0);
  setServoDegree(15, 0);
  wait();
}

void setServoDegree(int servonum, int degree) {
  int value = map(degree, 0, 180, minvalues[servonum], maxvalues[servonum]);
  
  if (value == lastvalues[servonum])
    return;
  
  if (value < lastvalues[servonum]) {
    for (uint16_t pulselen = lastvalues[servonum]; pulselen > value; pulselen--) {
      pwm.setPWM(servonum, 0, pulselen);
      //delay(10);
    }
  }
  else {
    for (uint16_t pulselen = lastvalues[servonum]; pulselen < value; pulselen++) {
      pwm.setPWM(servonum, 0, pulselen);
      //delay(10);
    }
  }
  
  lastvalues[servonum] = value;
}

void initServos(int nb, int servos[]) {
}

void initServo(int servonum) {
  for (uint16_t pulselen = maxvalues[servonum]; pulselen > minvalues[servonum]; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }
}

void wait() {
  delay(DELAY);
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}
