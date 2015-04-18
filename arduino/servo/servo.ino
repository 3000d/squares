#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOCOUNT 16
#define DELAY 2000

int values[SERVOCOUNT];
int lastvalues[SERVOCOUNT];
int minvalues[SERVOCOUNT];
int maxvalues[SERVOCOUNT];
String str;

void setup() {
  Serial.begin(9600);
  Serial.println("hello");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  // setting default values in case of missing later config
  for (int i(0); i < SERVOCOUNT; i++) {
    minvalues[i] = 200;
    maxvalues[i] = 450;
  }
  
  minvalues[0] = 130;
  maxvalues[0] = 560;
  
  initAll();
  
  wait();
}

void test() {
  for (int i(0); i < SERVOCOUNT; i++)
    setServoDegree(i, 90);
    
  //refresh();

  wait();
  
  for (int i(0); i < SERVOCOUNT; i++)
    setServoDegree(i, 0);
    
  //refresh();
  
  wait();
}

void loop() {
  readSerial();
  //test();
}

void readSerial() {
  if (Serial.available() == 0)
    return;
  
  str = Serial.readStringUntil('\n');
  
  if (str[0] == 'm') {
    String servostr = str.substring(2, 4);
    String degreestr = str.substring(5, 8);
  
    int servonum = servostr.toInt();
    int degree = degreestr.toInt();
  
    setServoDegree(servonum, degree);
  }
  else if (str[0] == 'r') {
    refresh();
  }
  else if (str[0] == 'c') {
    String servostr = str.substring(2, 4);
    String minstr = str.substring(5, 8);
    String maxstr = str.substring(9, 12);
     
    int servonum = servostr.toInt();
    int minimum = minstr.toInt();
    int maximum = maxstr.toInt();
     
    minvalues[servonum] = minimum;
    maxvalues[servonum] = maximum;
  }
  else if (str[0] == 'i') {
    initAll();
  }
}

void setServoDegree(int servonum, int degree) {
  int value = map(degree, 0, 180, minvalues[servonum], maxvalues[servonum]);
  
  if (value == lastvalues[servonum])
    return;
  
  if (value < lastvalues[servonum]) {
    for (uint16_t pulselen = lastvalues[servonum]; pulselen > value; pulselen--) {
      pwm.setPWM(servonum, 0, pulselen);
    }
  }
  else {
    for (uint16_t pulselen = lastvalues[servonum]; pulselen < value; pulselen++) {
      pwm.setPWM(servonum, 0, pulselen);
    }
  }
  
  lastvalues[servonum] = value;
}

void setServoDegreeValue(int servonum, int degree) {
  values[servonum] = map(degree, 0, 180, minvalues[servonum], maxvalues[servonum]);
}

void refresh() {
  int servos[SERVOCOUNT];
  
  for (int i(0); i < SERVOCOUNT; i++)
    servos[i] = i;

  setServosDegree(SERVOCOUNT, servos);
}

void setServosDegree(int nb, int servos[]) {
  int minimum(4096);
  int maximum(-4096);
  
  for (int i(0); i < nb; i++) {
    if (minvalues[servos[i]] < minimum)
      minimum = minvalues[servos[i]];
    if (maxvalues[servos[i]] > maximum)
      maximum = maxvalues[servos[i]];
  }

  for (uint16_t pulselen(minimum); pulselen < maximum; pulselen++) {
    for (int i(0); i < nb; i++) {
      if (values[servos[i]] == lastvalues[servos[i]])
        continue;
  
      if (values[servos[i]] < lastvalues[servos[i]]) {
        if (pulselen >= lastvalues[servos[i]] - values[servos[i]]
        || (lastvalues[servos[i]] - pulselen) < minvalues[servos[i]]
        || (lastvalues[servos[i]] - pulselen) > maxvalues[servos[i]]
        ) {
          continue;
        }
        
        pwm.setPWM(servos[i], 0, lastvalues[servos[i]] - pulselen);
      }
      else {
        if (pulselen < lastvalues[servos[i]]
        || pulselen > values[servos[i]]
        || pulselen < minvalues[servos[i]]
        || pulselen > maxvalues[servos[i]]
        
        ) {
          continue;
        }
          
        pwm.setPWM(servos[i], 0, pulselen);
      }
    }
  }
  
  for (int i(0); i < nb; i++)
    lastvalues[servos[i]] = values[servos[i]];
}

void initAll() {
  int servos[SERVOCOUNT];
  
  for (int i(0); i < SERVOCOUNT; i++)
    servos[i] = i;
 
  initServos(SERVOCOUNT, servos);
}

void initServos(int nb, int servos[]) {
  int minimum(4096);
  int maximum(-4096);
  
  for (int i(0); i < nb; i++) {
    if (minvalues[servos[i]] < minimum)
      minimum = minvalues[servos[i]];
    if (maxvalues[servos[i]] > maximum)
      maximum = maxvalues[servos[i]];
  }
  
  for (uint16_t pulselen(maximum); pulselen > minimum; pulselen--) {
    for (int i(0); i < nb; i++) {
      if (pulselen > minvalues[servos[i]] && pulselen < maxvalues[servos[i]])
        pwm.setPWM(servos[i], 0, pulselen);
    }
  }
  
  for (int i(0); i < nb; i++) {
    lastvalues[servos[i]] = minvalues[servos[i]];
    values[servos[i]] = minvalues[servos[i]];
  }
}

void initServo(int servonum) {
  lastvalues[servonum] = minvalues[servonum];
  values[servonum] = minvalues[servonum];
  
  for (uint16_t pulselen(maxvalues[servonum]); pulselen > minvalues[servonum]; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }
}

void wait() {
  delay(DELAY);
}
