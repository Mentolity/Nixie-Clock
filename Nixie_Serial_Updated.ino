//Mega Board
//int data=53;
//int clk=51;
//int latch=49;

//RedBoard
int data = 7; //Pin 14
int latch = 6; // Pin 12
int clk = 5; //Pin 11

int on=255;
int off=0;

unsigned long wait=999;

int H=1;
int h=8;
int M=5;
int m=5;
int S=5;
int s=8;

boolean colonAlt = false;
unsigned long currentTime = millis();
unsigned long previousTime = currentTime;

float aplhaWorldLine[] = {0.571046,0.571024,0.571015,0.523307,0.523299,0.456914,0.456903,0.409431,0.409420,0.334581,0.337187,0.000000};
float betaWorldLine[] = {1.130238,1.130205,1.129848,1.130212,1.130211,1.130209,1.130208,1.130206,1.130205,1.048596};

void setup() {
  Serial.begin(9600);
  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(clk, OUTPUT);
}

//World line function
void lineUpdate(char line){
  long lineHolder=0;
  int lineNumber[6];
  int numberLock=7;
  int whichNumber=0;
  
  long randomLine;
  if(line=='a'){
    randomLine=random(0,12);
  }else if (line=='b'){
    randomLine=random(0,10);
  }
  
  //sends the selected line to interger array
  if(line=='a'){
    lineHolder = aplhaWorldLine[randomLine]*1000000;
  }else if(line=='b'){
    lineHolder = betaWorldLine[randomLine]*1000000;
  }
  for (int i=0; i<6; i++) {
    lineNumber[i] = lineHolder%10;
    lineHolder /= 10;
  }
  
  int randomPost[] = {9,7,5,3,1,6,2};
  //disables random numbers
  for(int i=0;i<56;i++){
    delay(40);
    switch(i){
      case 7:
        randomPost[4]=11;
        break;
      case 14:
        randomPost[5]=11;
        break;
      case 21:
        randomPost[2]=11;
        break;
      case 28:
        randomPost[6]=11;
        break;
      case 35:
        randomPost[3]=11;
        break;
      case 42:
        randomPost[1]=11;
        break;
      case 49:
        randomPost[0]=11;
        break;
      case 56:
        randomPost[2]=11;
        break;
    }
    
    //pushes out random number unless disabled
    for(int x=0;x<7;x++){
      if(randomPost[x]<9){
        randomPost[x]++;  
      }else if(randomPost[x]>10){
      }else{
        randomPost[x]=0;
      }
    }
    digitalWrite(latch, LOW);
    shiftOut(data,clk,LSBFIRST, 10);
    shiftOut(data,clk,LSBFIRST, 3);
    for (int x=0; x<6; x++){
      if(randomPost[x]<10){
        shiftOut(data,clk,LSBFIRST, randomPost[x]);
      }else{
        shiftOut(data,clk,LSBFIRST, lineNumber[x]);
      }
    }
    shiftOut(data,clk,LSBFIRST, 10);
    if(randomPost[6]<10){
      shiftOut(data,clk,LSBFIRST, randomPost[6]);
    }else if(line=='a'){
      shiftOut(data,clk,LSBFIRST, 0);
    }else if(line=='b'){
      shiftOut(data,clk,LSBFIRST, 1);
    }
    digitalWrite(latch, HIGH);
  } 
}

void shiftOut(int dataPin, int clkPin, int bitOrder, byte val){
      int i;
      for (i = 0; i < 4; i++)  {
            if (bitOrder == LSBFIRST)
                  digitalWrite(dataPin, !!(val & (1 << i)));
            else      
                  digitalWrite(dataPin, !!(val & (1 << (7 - i))));                
            digitalWrite(clkPin, HIGH);
            digitalWrite(clkPin, LOW);            
      }
}

//Updates clk values and shifts them out
void clkUpdate(int Hour,int hour,int Minute,int minute, int Secound,int secound){
  digitalWrite(latch, LOW); 
  if(colonAlt==false){
    shiftOut(data,clk,LSBFIRST,2);
    shiftOut(data,clk,LSBFIRST,4);
  }else{
    shiftOut(data,clk,LSBFIRST,3);
    shiftOut(data,clk,LSBFIRST,5);
  }
  shiftOut(data,clk,LSBFIRST,secound);
  shiftOut(data,clk,LSBFIRST,Secound);
  
  shiftOut(data,clk,LSBFIRST,10); //Over range to free colon
  
  shiftOut(data,clk,LSBFIRST,minute);
  shiftOut(data,clk,LSBFIRST,Minute);
  
  shiftOut(data,clk,LSBFIRST,10); //Over range to free colon
  
  shiftOut(data,clk,LSBFIRST,hour);
  shiftOut(data,clk,LSBFIRST,Hour);
  digitalWrite(latch, HIGH);

  currentTime = millis();
  if(currentTime - previousTime > wait){
    previousTime = currentTime;
    colonAlt = !colonAlt;
    if(s<9){
      s++;
    }else if(S<5){
      s=0;
      S++;
    }else if(m<9){
      S=-1;
      m++;
      //every minute program will excute anit-cathode poisioning sequence
      antiPoisonTwo();
    }else if(M<5){
      m=-1;
      M++;
    }else if(h<9){
      M=-1;
      h++;
    }else if(H<1){
      h=-1;
      H++;
    }else{
      h=-1;
      H++;
    }
    if(H>=2 && h>=4){
      H=0;
      h=0;
      M=0;
      m=0;
      S=0;
      s=0;
      antiPoisonTwo();
    }
  }
}

//Primary Anti-poisioning
void antiPoison(){
  for(int i=0; i<=9; i++){
    digitalWrite(latch, LOW);
    shiftOut(data,clk,LSBFIRST,10); //Disable Decimals
    shiftOut(data,clk,LSBFIRST,10); //Disable Decimals
    for(int x=0; x<8; x++){
      shiftOut(data,clk,LSBFIRST,i);
    }
    digitalWrite(latch, HIGH);
    delay(100);
  }
  s=1;
  S=0;
  clkUpdate(H,h,M,m,S,s);
}

//Secoundary Anti-poisioning *Requires more work*
void antiPoisonTwo(){
  int startPoint[8];
  for(int i; i<8; i++){
      startPoint[i] = random(0,10);
  }
  for(int i=0; i<25; i++){
    digitalWrite(latch, LOW);
    shiftOut(data,clk,LSBFIRST,10); //Disable Decimals
    shiftOut(data,clk,LSBFIRST,10); //Disable Decimals
    for(int x=0; x<8; x++){
      shiftOut(data,clk,LSBFIRST,startPoint[x]);
      if(startPoint[x]==9){
        startPoint[x]=0;
      }else{
        startPoint[x]=startPoint[x]+1;
      }
    }
    digitalWrite(latch, HIGH);
    delay(40);
  }
  s=1;
  S=0;
  clkUpdate(H,h,M,m,S,s);
}

void sleepMode(){
  digitalWrite(latch, LOW);  
  for(int i=0; i<10; i++){
    shiftOut(data,clk,LSBFIRST,10);
  }
  digitalWrite(latch, HIGH);
}


int val[]={s,S,m,M,h,H};
int serialValue;
int clkFunction=0;
int lineFunction=1;
int timerFunction=2;
int sleepFunction=3;
int function=0;

  
void loop(){
  if(Serial.available() == 0){
    if(function==clkFunction){
      clkUpdate(H,h,M,m,S,s);
    }else if(function==lineFunction){
      long randomLine=random(1,3);
      if(randomLine==1){
        lineUpdate('a');
      }else if(randomLine==2){
        lineUpdate('b');
      }
      while(Serial.available() == 0){}    
    }else if(function==sleepFunction){
      sleepMode();
    }
  }else{
    delay(2);
    serialValue=Serial.read();
    if(serialValue=='c'){
      Serial.println("Please input the time");
      Serial.flush();
      while(Serial.available() == 0){}
      delay(5);
      for(int i=0;i<6;i++){
        val[i] = Serial.read() - '0';
        Serial.print(val[i]);
      }
      s=val[5];
      S=val[4];
      m=val[3];
      M=val[2];
      h=val[1];
      H=val[0];
      function=clkFunction;
    }else if(serialValue=='w'){  
      delay(1);
      Serial.println("The world line has changed!");
      function=lineFunction;
    }else if(serialValue=='t'){
      function=timerFunction;
    }else if(serialValue=='s'){
      function=sleepFunction;
    }
    Serial.flush();
  }
}
