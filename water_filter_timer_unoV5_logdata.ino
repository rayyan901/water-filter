
unsigned long startMillis;
unsigned long currentMillis;
unsigned long time_m_started;

unsigned long elapseMillis;
const unsigned long period = 500;  //the value is a number of milliseconds, ie 1 second
const unsigned long period2 = 200;
boolean toggle = 0;

int pos=0;

int motor = 3;    // pwm
int dir = 2;

int filter = 4;
int timer_trigger = 5;
int stop_switch = 6;
int pwmMotor = 7;

int backwash_timer=10; //1200;   // in seconds  1200s=20m
int rinse_timer=5;//300;       // 300s=5m
int maintenance_timer=10;
int timer_bwash=1;
int recent_bwash=0;


int err_sense=0;

int j;
int fadeValue = 110;                                       
//===========================================================


                                                                                                                                                                                                                                                             
//=========================================================                
void setup() {
 // Serial.begin(9600);
  Serial.begin(115200);
//======================
  Serial.println("CLEARDATA");
Serial.println("LABEL,Time,Timer,Dir,Motor,Sensor");
Serial.println("RESETTIMER");


cli();//stop interrupts

//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
sei();//allow interrupts

//=====================
  
  Serial.println("--- Start Water Filter ---");
  
  pinMode(dir, OUTPUT);
  pinMode(pwmMotor, OUTPUT);
  pinMode(filter, INPUT_PULLUP);
  pinMode(timer_trigger, INPUT_PULLUP);
  pinMode(stop_switch, INPUT_PULLUP);
  delay(300);
  //======

  //====================
}

//=====
ISR(TIMER1_COMPA_vect){

Serial.print("DATA,TIME,");
//Serial.print(sensorValue);
Serial.print(bitRead(PORTD,5));
Serial.print(',');

//Serial.print(sensorValue);
Serial.print(bitRead(PORTD,2));
Serial.print(',');

Serial.print(bitRead(PORTD,7));
Serial.print(',');
//Serial.println(sensorValue);
Serial.println(bitRead(PORTD,4));


}
//====== 
void loop() {

int i,j;


if(recent_bwash==0)
{
filter_f();
}

timer_bwash=digitalRead(timer_trigger);
Serial.println(timer_bwash);
while(timer_bwash) 
{

 

  delay(10); 
 //===
if(digitalRead(stop_switch)==0){
Serial.println("--- Initiate stop");

digitalWrite(dir,1);  
analogWrite(motor, fadeValue); 
turnMotorON();
delay(300);
//while(digitalRead(filter));

while(digitalRead(filter)){
delay(1);
//Serial.println(digitalRead(filter)); 
}



analogWrite(motor, 0);
turnMotorOFF(); 
Serial.println(digitalRead(filter));
Serial.println("--- stop started");
Serial.println("--- motor stopped..please reset");
  while(1);
} // end if  stop switch
 //===

timer_bwash=digitalRead(timer_trigger);  
Serial.println(timer_bwash);
delay(1000);
} //while end
//===========add delay before backwash started here===========


//=====================================================

backwash_timer=10;   // in seconds  1200s=20m
rinse_timer=5;       // 300s=5m

backwash_f();
rinse_f();


backwash_timer=10;   // in seconds  900s=15m
rinse_timer=5;       // 300s=5m

backwash_f();
rinse_f();
//=====
filter_f();
recent_bwash=1;

//======delay based===
for(j=0;j<2;j++)
{
for(i=0;i<60;i++)
{
delay(1000);
}
}
//=====


//========timer input based=====

//========
}


//=========
void filter_f() {
Serial.println("--- Initiate filter");
digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
turnMotorON();
time_m_started=millis();
delay(300);

do{

do{
 delay(1);
 currentMillis = millis();  
 if((currentMillis - time_m_started >= 5000)){
 pos=1;  //filter position=1
 err_sense=1;
 }
 
}while(digitalRead(filter)&&(currentMillis - time_m_started <= 5000));

  
//while(digitalRead(filter));

 startMillis = millis();  //initial start time
do {
    currentMillis = millis(); 
 elapseMillis=currentMillis - startMillis;
    

}while(!digitalRead(filter)&&(elapseMillis <= period));

if(elapseMillis>=period)
{

analogWrite(motor, 0); 
turnMotorOFF();
Serial.println("--- filter started");
pos=1;
err_sense=0;
}
}while((elapseMillis<period)&&(err_sense!=1));
//===
if(err_sense==1)
{

analogWrite(motor, 0); 
turnMotorOFF();
Serial.println("--- filter started but sense error");
pos=1;

}


//======
}


//====

void backwash_f() {

int i;
Serial.println("--- Initiate backwash");
err_sense=0;
digitalWrite(dir,1);  
analogWrite(motor, fadeValue); 
turnMotorON();
delay(200);
//while(!digitalRead(bwash));
time_m_started=millis();

do{
//while(digitalRead(filter));
//============
do{
   delay(1);
   currentMillis = millis();  
   if((currentMillis - time_m_started >= 5000)){
    pos=4;  //filter position=4 for backwash
    err_sense=1;
    }
}while(digitalRead(filter)&&(currentMillis - time_m_started <= 5000));


//==========
 startMillis = millis();  //initial start time
do {
    currentMillis = millis(); 
 elapseMillis=currentMillis - startMillis;
    
//}while((currentMillis - startMillis <= period));
}while(!digitalRead(filter)&&(elapseMillis <= period2));

if(elapseMillis>=period2)
{

analogWrite(motor, 0); 
turnMotorOFF();
Serial.println("--- backwash started");
pos=4;
err_sense=0;
}
}while(elapseMillis<period2&&(err_sense!=1));
if(err_sense==1)
{

analogWrite(motor, 0); 
turnMotorOFF();
Serial.println("--- backwash  started but sense error");
pos=4;

}


for(i=0;i<(2*backwash_timer);i++){
//delay(500);


for(j=0;j<50;j++)
{
  delay(10); 
//===
 //===
if(digitalRead(stop_switch)==0){
Serial.println("--- Initiate stop");

digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
turnMotorON();
delay(300);
//while(!digitalRead(filter));
while(digitalRead(filter));
while(!digitalRead(filter));
delay(300);
while(digitalRead(filter));


analogWrite(motor, 0); 
turnMotorOFF();
Serial.println("--- stop started");
Serial.println("--- motor stopped..please reset");
  while(1);
}

}
 //=== 

 //===
}
  
}

//===================
void rinse_f() {

int i;
Serial.println("--- Initiate rinse");
err_sense=0;
digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
turnMotorON();
delay(300);
time_m_started=millis();
//while(digitalRead(filter));
//========
do{
   delay(1);
   currentMillis = millis();  
   if((currentMillis - time_m_started >= 5000)){
    pos=0;  //filter position=0 for unknown position
    err_sense=1;
   }
}while(digitalRead(filter)&&(currentMillis - time_m_started <= 5000));
if(err_sense==1)
{

analogWrite(motor, 0);
turnMotorOFF(); 
Serial.println("---   rinse failed sensor error");
 pos=0;  //filter position=0 for unknown position
return;
}



//=========
analogWrite(motor, 0); 
turnMotorOFF();
Serial.println("--- rinse started");
pos=3;
for(i=0;i<(2*rinse_timer);i++){
//delay(500);

 for(j=0;j<50;j++)
{
  delay(10); 
 //===
 //===
if(digitalRead(stop_switch)==0){
Serial.println("--- Initiate stop");

digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
turnMotorON();
delay(300);
while(digitalRead(filter));
analogWrite(motor, 0); 
turnMotorOFF();
Serial.println("--- stop started");
Serial.println("--- motor stopped..please reset");
  while(1);
}
}
 //=== 
 //=== 
}
  
}

 //================================ 
 void turnMotorON(){
 digitalWrite(pwmMotor,1);
 }

 void turnMotorOFF(){
 digitalWrite(pwmMotor,0);
 }

