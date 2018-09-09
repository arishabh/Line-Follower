int mot11 = 3;
int mot12 = 4;
int mot21 = 5;
int mot22 = 6;
int en1 = 9;
int en2 = 10;
int spd1, spd2;
int read1 = 14;
int read2 = 15;
int read3 = 16;
int read4 = 17;
int read5 = 18;
int sensor[5] = {0, 0, 0, 0, 0};
int error, P, I, D, preverror, PIDvalue;
int Kp = 20;
int Ki = 0;
int Kd = 10;  

void setup() {
  Serial.begin(9600);
  pinMode(mot11, OUTPUT);
  pinMode(mot12, OUTPUT);
  pinMode(mot22, OUTPUT);
  pinMode(mot21, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);  
  pinMode(read1, INPUT);
  pinMode(read2, INPUT);
  pinMode(read3, INPUT);
  pinMode(read4, INPUT);
  pinMode(read5, INPUT);
}

void forward(){
  Serial.print("Speed1 Value: ");
  Serial.println(spd1);
  Serial.print("Speed2 Value: ");
    Serial.println(spd2);
  analogWrite(en1, spd1);
  analogWrite(en2, spd2);
  digitalWrite(mot11, HIGH);
  digitalWrite(mot12, LOW);
  digitalWrite(mot21, HIGH);
  digitalWrite(mot22, LOW);
  Serial.println("Forward");
}

void stay(){
  digitalWrite(mot11, LOW);
  digitalWrite(mot12, LOW);
  digitalWrite(mot21, LOW);
  digitalWrite(mot22, LOW);
  Serial.println("Stop");
}

void uturn(){
  analogWrite(en1, 127.5);
  analogWrite(en2, 127.5);
  digitalWrite(mot11, HIGH);
  digitalWrite(mot12, LOW);
  digitalWrite(mot22, HIGH);
  digitalWrite(mot21, LOW);
  Serial.println("U Turn");
}

void readsensor(){
  sensor[0] = digitalRead(read1);
  sensor[1] = digitalRead(read2);
  sensor[2] = digitalRead(read3);
  sensor[3] = digitalRead(read4);
  sensor[4] = digitalRead(read5);
  Serial.print("Read1 Value: ");
  Serial.println(sensor[0]);
  Serial.print("Read2 Value: ");
  Serial.println(sensor[1]);
  Serial.print("Read3 Value: ");
  Serial.println(sensor[2]);
  Serial.print("Read4 Value: ");
  Serial.println(sensor[3]);
  Serial.print("Read5 Value: ");
  Serial.println(sensor[4]);
  calerror();
}

void calerror(){
  if(sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0 && sensor[3] == 1 && sensor[4] == 1){ //Array 11011 is no error
    error = 0;
  }
  else if(sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 1 && sensor[4] == 1){ //Array 10011 is 1 error
    error = 1;
  }
  else if(sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0 && sensor[3] == 0 && sensor[4] == 1){ //Array 11001 is -1 error
    error = -1;
  }
  else if(sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1){ //Array 00111 is 2 error
    error = 2;
  }
  else if(sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 0 && sensor[4] == 0){ //Array 11100 is -2 error
    error = -2;
  }
  else if(sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 0){ //Array 11110 is -3 error
    error = -3;
  }
  else if(sensor[0] == 0 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1){ //Array 01111 is 3 error
    error = 3;
  }
  else {
    error = 0;
  }
  P = error;
  I = error + I;
  D = error - preverror;
  Serial.print("Error Value: ");
  Serial.println(error);
  Serial.print("P Value: ");
  Serial.println(P);
  Serial.print("I Value: ");
  Serial.println(I);
  Serial.print("D Value: ");
  Serial.println(D);
}

void PIDValue(){
  PIDvalue = (Kp*P) + (Ki*I) + (Kd*D);
  preverror = error;
  Serial.print("PID Value: ");
  Serial.println(PIDvalue);
}

void motorcontrol(){
  if(sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0 && sensor[3] == 1 && sensor[4] == 1){ //Array 11011 is no error
    spd1 = 127.5;
    spd2 = 127.5;
    forward();
  }
  else if(sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1  && sensor[3] == 1 && sensor[4] == 1){ //Array 11111 is U Turn
    spd1 = 127.5;
    spd2 = 127.5;
    uturn();
  }
  else if(sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 0 && sensor[4] == 0){ //Array 00000 is stop
    stay();
  }  
  else if(sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0 && sensor[3] == 0 && sensor[4] == 0){ //Array 11000 is 90 degree right
    spd2 = 127.5;
    spd1 = 0;
    forward();
  }
  else if(sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 1 && sensor[4] == 1){ //Array 00011 is 90 degree left;
    spd1 = 127.5;
    spd2 = 0;
    forward();
  }
  else if (error != 0){
    spd1 = 127.5 + PIDvalue;
    spd2 = 127.5 - PIDvalue;
    forward();
  }
}

void loop() {
  readsensor();
  PIDValue();
  motorcontrol();
  Serial.println(" ");
}
