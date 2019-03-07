
unsigned long startMillis;
unsigned long currentMillis;

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

int backwash_timer=5;
int rinse_timer=3;
int maintenance_timer=10;
int timer_bwash=1;
int recent_bwash=0;

int j;
int fadeValue = 110;                                                                                                                                                                                                                                                                                                         ;
void setup() {


  Serial.begin(9600);
  Serial.println("--- Start Water Filter ---");
  
  pinMode(dir, OUTPUT);
  pinMode(filter, INPUT_PULLUP);
  pinMode(timer_trigger, INPUT_PULLUP);
  //pinMode(bwash, INPUT_PULLUP);
 // pinMode(stopp, INPUT_PULLUP);
 // pinMode(rinse, INPUT_PULLUP);
  pinMode(stop_switch, INPUT_PULLUP);
  delay(300);
  // nothing happens in setup
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
//for(i=0;i<(2*maintenance_timer);i++){
//delay(500);
 

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
backwash_f();
rinse_f();
backwash_f();
rinse_f();
//=====
filter_f();
recent_bwash=1;

//======delay based===
for(j=0;j<1;j++)
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

digitalWrite(dir,0);  
analogWrite(motor, fadeValue); 
delay(300);

while(digitalRead(filter));
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
