
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

int backwash_timer=10; //1200;   // in seconds  1200s=20m
int rinse_timer=5;//300;       // 300s=5m
int maintenance_timer=10;
int timer_bwash=1;
int recent_bwash=0;


int err_sense=0;

int j;
int fadeValue = 110;                                                                                                                                                                                                                                                                                                         ;
void setup() {


  Serial.begin(9600);
  Serial.println("--- Start Water Filter ---");
  
  pinMode(dir, OUTPUT);
  pinMode(filter, INPUT_PULLUP);
  pinMode(timer_trigger, INPUT_PULLUP);
  pinMode(stop_switch, INPUT_PULLUP);
  delay(300);
  
}
 
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
delay(300);
//while(digitalRead(filter));

while(digitalRead(filter)){
delay(1);
//Serial.println(digitalRead(filter)); 
}



analogWrite(motor, 0); 
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

backwash_timer=1200;   // in seconds  1200s=20m
rinse_timer=300;       // 300s=5m

backwash_f();
rinse_f();


backwash_timer=900;   // in seconds  900s=15m
rinse_timer=300;       // 300s=5m

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
//timer_bwash=digitalRead(timer_trigger);
//Serial.println(timer_bwash);
//while(!timer_bwash) 
//{
//Serial.println("waiting until timer off...");
//
//delay(1000);  
//timer_bwash=digitalRead(timer_trigger);
//}

//========
}


//=========
void filter_f() {
Serial.println("--- Initiate filter");
digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
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
Serial.println("--- filter started");
pos=1;
err_sense=0;
}
}while((elapseMillis<period)&&(err_sense!=1));
//===
if(err_sense==1)
{

analogWrite(motor, 0); 
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
Serial.println("--- backwash started");
pos=4;
err_sense=0;
}
}while(elapseMillis<period2&&(err_sense!=1));
if(err_sense==1)
{

analogWrite(motor, 0); 
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
Serial.println("---   rinse failed sensor error");
 pos=0;  //filter position=0 for unknown position
return;
}



//=========
analogWrite(motor, 0); 
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
delay(300);
while(digitalRead(filter));
analogWrite(motor, 0); 
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
