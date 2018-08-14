

unsigned long startMillis;
unsigned long currentMillis;

unsigned long elapseMillis;
const unsigned long period = 500;  //the value is a number of milliseconds, ie 1 second
const unsigned long period2 = 200;
boolean toggle = 0;

int pos=0;
//pinMode(3, OUTPUT);
int motor = 9;    // pwm
int dir = 8;

int filter = 2;
int bwash = 2;
int stopp = 4;
int rinse = 2;
int stop_switch = 6;

int backwash_timer=5;
int rinse_timer=3;
int maintenance_timer=10;

int fadeValue = 90;                                                                                                                                                                                                                                                                                                         ;
void setup() {


  Serial.begin(9600);
  Serial.println("--- Start Serial Monitor SEND_RCVE ---");
  
  pinMode(dir, OUTPUT);
  pinMode(filter, INPUT_PULLUP);
  pinMode(bwash, INPUT_PULLUP);
  pinMode(stopp, INPUT_PULLUP);
  pinMode(rinse, INPUT_PULLUP);
  pinMode(stop_switch, INPUT_PULLUP);
  // nothing happens in setup
}
 
void loop() {

int i;

filter_f();


for(i=0;i<(2*maintenance_timer);i++){
delay(500);
 //===
if(digitalRead(stop_switch)==0){
Serial.println("--- Initiate stop");

digitalWrite(dir,1);  
analogWrite(motor, fadeValue); 
delay(300);
while(digitalRead(filter));
analogWrite(motor, 0); 
Serial.println("--- stop started");
Serial.println("--- motor stopped..please reset");
  while(1);
}




 //=== 
}
backwash_f();
rinse_f();
backwash_f();
rinse_f();



}


//=========
void filter_f() {
Serial.println("--- Initiate filter");
digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
delay(300);

do{
while(digitalRead(filter));

 startMillis = millis();  //initial start time
do {
    currentMillis = millis(); 
 elapseMillis=currentMillis - startMillis;
    
//}while((currentMillis - startMillis <= period));
}while(!digitalRead(filter)&&(currentMillis - startMillis <= period));

if(elapseMillis>=period)
{

analogWrite(motor, 0); 
Serial.println("--- filter started");
pos=1;
}
}while(elapseMillis<period);

}


//====

void backwash_f() {

int i;
Serial.println("--- Initiate backwash");

digitalWrite(dir,1);  
analogWrite(motor, fadeValue); 
delay(200);
//while(!digitalRead(bwash));

do{
while(digitalRead(filter));

 startMillis = millis();  //initial start time
do {
    currentMillis = millis(); 
 elapseMillis=currentMillis - startMillis;
    
//}while((currentMillis - startMillis <= period));
}while(!digitalRead(filter)&&(currentMillis - startMillis <= period2));

if(elapseMillis>=period2)
{

analogWrite(motor, 0); 
Serial.println("--- backwash started");
pos=4;
}
}while(elapseMillis<period2);

for(i=0;i<(2*backwash_timer);i++){
delay(500);

//===
 //===
if(digitalRead(stop_switch)==0){
Serial.println("--- Initiate stop");

digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
delay(300);
//while(!digitalRead(filter));
while(digitalRead(filter));
while(!digitalRead(filter));
delay(300);
while(digitalRead(filter));


analogWrite(motor, 0); 
Serial.println("--- stop started");
Serial.println("--- motor stopped..please reset");
  while(1);
}

 //=== 

 //===
}
  
}

//===================
void rinse_f() {

int i;
Serial.println("--- Initiate rinse");

digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
delay(300);

while(digitalRead(filter));
analogWrite(motor, 0); 
Serial.println("--- rinse started");
pos=3;
for(i=0;i<(2*rinse_timer);i++){
delay(500);
 //===
 //===
if(digitalRead(stop_switch)==0){
Serial.println("--- Initiate stop");

digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
delay(300);
while(digitalRead(filter));
analogWrite(motor, 0); 
Serial.println("--- stop started");
Serial.println("--- motor stopped..please reset");
  while(1);
}

 //=== 
 //=== 
}
  
}

 //================================ 


//=======================
