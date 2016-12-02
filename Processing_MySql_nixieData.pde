import de.bezier.data.sql.*;
import processing.serial.*;
Serial myPort;

PFont f;
String typing = "";
String saved = "";

MySQL msql;

Boolean sqlCheck = true;

String clockFunction[] = {"time","clock","c","timeSet","time set"};
String lineFunction[] = {"meter","w","worldLine","world Line","el psy congroo","El Psy Congroo","El psy congroo","Line","line","divergence","pingas"};
String sleepFunction[] = {"s","sleep","Sleep","Night","night","nite","Nite"};

int update;
String timeSet="";
String whichEnable="clock";
Boolean nullError= true;

Boolean serialUpdate=true;
int timeHolder;

void setup()
{
  size(300,200);
  f = createFont("Arial",16,true);
  String user = "mento";
  String pass = "pingas";
  String database = "nixieData";
  msql = new MySQL( this, "107.3.113.1:3306", database, user, pass );
  String portName = Serial.list()[0];
  println(Serial.list());
  myPort = new Serial(this, portName, 9600);
  background(255);
  delay(2000);
}

void ioCheck(){
  for(int i=0; i<clockFunction.length; i++){
    if(whichEnable.equals(clockFunction[i])){
      myPort.write('c');
      myPort.write(timeSet);
      println("Function Set: "+whichEnable);
      println("Time Set: "+timeSet);
      println("Time Updated");
    }
  }
  for(int i=0; i<lineFunction.length; i++){
    if(whichEnable.equals(lineFunction[i])){
      myPort.write('w');
      println("Function Set: "+whichEnable);
      println("World Line Updated");
    }
  }
  for(int i=0; i<sleepFunction.length; i++){
    if(whichEnable.equals(sleepFunction[i])){
      myPort.write('s');
      println("Function Set: "+whichEnable);
      println("Sleep Mode Activated");
    }
  }
}

boolean syncUpdate=true;

void draw(){
  //disabling MySQL checks until server is restarted
  //if(sqlCheck==true){
  if(1==2){
    if( msql.connect() ){
      println("Success");
      while(1==1){
        delay(1000);
        msql.query( "SELECT * FROM functions");
        msql.next();
        update=msql.getInt(4);
        if(update==1){
          println("Update request received");
          timeSet=msql.getString(3);
          whichEnable=msql.getString(2);
          msql.query("UPDATE `nixieData`.`functions` SET `update` = '0' WHERE `functions`.`id` =1;");
          nullError=true;
          ioCheck();
        }else{
          if(nullError==true){
            println("Update request is NULL");
            nullError=false;
          }
        }
      }
    }else{
      sqlCheck=false;
      println("Connection Failure");
      println("Client Side Access Only");
    }
  }else{
    int h=hour();
    int m=minute();
    int s=second();
    
    delay(30);
    
    int updateHolder=m;
    while(m==11 && s==30){
      if(syncUpdate==true){
        whichEnable="clock";
        myPort.write('c');
        myPort.write(timeSet);
        println("Function Set: "+whichEnable);
        println("Time Set: "+timeSet);
        syncUpdate=false;
        println("Time Synced");
      }else{
         h=hour();
         m=minute();
         s=second();
      }
    }
    syncUpdate=true;
    
    if(h<=9){
      timeSet="0" + Integer.toString(h);
    }else{
      timeSet=Integer.toString(h);
    }
    if(m<=9){
      timeSet=timeSet + "0" + Integer.toString(m);
    }else{
      timeSet=timeSet + Integer.toString(m);
    }
    if(s<=9){
      timeSet=timeSet + "0" + Integer.toString(s);
    }else{
      timeSet=timeSet + Integer.toString(s);
    }
    
    background(255);
    int indent = 12;
    textFont(f);
    fill(0);
    text("Connection to MySQL Database failed", indent, 40);
    text("\nNixie Clock Serial Connection Enabled", indent, 40);
    text("Input Function:" + typing,indent,90);
    text("Active Function: " + saved,indent,130);
    if(serialUpdate==true){
      ioCheck();
      serialUpdate=false;
      whichEnable="";
    }
  }
}
void keyPressed(){
  if(key=='\n'){
    saved = typing;
    serialUpdate=true;
    whichEnable=saved;
    typing = "";
  }else if(key=='\b'){
    typing = typing + key;
    println("test");
  }else{
    typing = typing + key;
  }
}
