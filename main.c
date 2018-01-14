

#define pinAV 0
#define pinAR 1
#define pinServo 2
#define delayMoteur 2
#define delayServo 2


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Servo.h>

/* Set these to your desired credentials. */
const char *ssid = "Robot";
const char *password = "password";

ESP8266WebServer server(80); // on instancie un serveur ecoutant sur le port 80
Servo servo1;

int vitesseMoteur = 0;
int angleServo = 90;
int F_av=0, F_ar=0, F_ga=0, F_dr=0;


void handleRoot()
{
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup()
{
  //effacer ancienne config
  ESP.eraseConfig();

  //pinout
  pinMode(pinAV, OUTPUT);
  pinMode(pinAR, OUTPUT);
  pinMode(pinServo, OUTPUT);

  //servo
  servo1.attach(pinServo);
  servo1.write(angleServo);

  //port serie
  Serial.begin(115200);
  
//*******************WIFI CONFIG*******************
  
  delay(1000);
  Serial.begin(115200);
  Serial.println();                       
  Serial.print("Configuring access point...");
  
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("HTTP server started");

//*************************************************


  
  // conditions url
  server.on("/avd", []()
  {
    ObjPin(1,0,0,1); //av ar g d
    server.send(200, "text/plain", "avd");
  });
  server.on("/av", []()
  {
    ObjPin(1,0,0,0);
    server.send(200, "text/plain", "av");
  });
  server.on("/avg", []()
  {
    ObjPin(1,0,1,0);
    server.send(200, "text/plain", "avg");
  });
  server.on("/ard", []()
  {
    ObjPin(0,1,0,1);
    server.send(200, "text/plain", "ard");
  });
  server.on("/arg", []()
  {
    ObjPin(0,1,1,0);
    server.send(200, "text/plain", "arg");
  });
  server.on("/ar", []()
  {
    ObjPin(0,1,0,0);
    server.send(200, "text/plain", "ar");
  });
  server.on("/mid", []()
  {
    ObjPin(0,0,0,0);
    server.send(200, "text/plain", "mid");
  });
  server.on("/", []()
  {
    ObjPin(0,0,0,0);
    server.send(200, "text/plain", "accueil");
  });
  server.on("/g", []()
  {
    ObjPin(0,0,1,0);
    server.send(200, "text/plain", "g");
  });
  server.on("/d", []()
  {
    ObjPin(0,0,0,1);
    server.send(200, "text/plain", "d");
  });

  // on commence a ecouter les requetes venant de l'exterieur
  server.begin();
}

void loop(void)
{
  // a chaque iteration, on appelle handleClient pour que les requetes soient traitees
  server.handleClient();
  pinOut(F_av, F_ar, F_ga, F_dr);
}

void ObjPin(int av, int ar, int ga, int dr)
{
  F_av=av;
  F_ar=ar;
  F_ga=ga;
  F_dr=dr;
}

void pinOut(int av, int ar, int ga, int dr)
{
  /******MOTEUR******/
  //avant
  if(av == 1)
  {
    digitalWrite(pinAV, HIGH); //test
  }
  else //test
  {
    digitalWrite(pinAV, LOW);
  }
  
  //arrière
  if(ar == 1)
  {
    digitalWrite(pinAR, HIGH); //test
  }
  else //test
  {
    digitalWrite(pinAR, LOW);
  }
  
  //mid
  if(av == 0 && ar == 0) 
  {
    vitesseMoteur=0;
    analogWrite(pinAV, vitesseMoteur);
    analogWrite(pinAR, vitesseMoteur);
  }


  /******DIRECTION******/
  
  //gauche
  if(ga == 1 && angleServo<170)
  {
    angleServo++;
    delay(delayServo);
    servo1.write(angleServo);
  }
  //droite
  if(dr == 1 && angleServo>0)
  {
    angleServo--;
    delay(delayServo);
    servo1.write(angleServo);
  }
  //mid
  if(dr == 0 && ga == 0)
  {
    if(angleServo>90)
    {
      angleServo--;
      servo1.write(angleServo);
      delay(delayServo);
    }
    if(angleServo<90)
    {
      angleServo++;
      servo1.write(angleServo);
      delay(delayServo);
    }
  }
  Serial.print("angle: ");
  Serial.println(angleServo);
  Serial.print("vitesse Moteur: ");
  Serial.println(vitesseMoteur);
  Serial.println("");
  
}



////------------------------ENDL------------------------////
