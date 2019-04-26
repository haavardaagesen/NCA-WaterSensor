/*
NCA Water Sensor Project



Circuit:
* Arduino MKR GSM 1400
* SIM card with a data plan
* 2" Water meter on digital pin 0
* LED on digital pin 5
* Powerbank
* Battery 3.7V

*/


// MKRGSM - Version: Latest 
#include <MKRGSM.h>
#include <OneWire.h>

// Settings for the device
String uid = ""; //ONLY FOR SETTING UP NEW DEVICES
String name = ""; //Change this accordingly
String latitude = ""; //Change this accordingly
String longitude = ""; //Change this accordingly
double minutes = 30; //Change this accordingly. Minutes from successfull sending to next

// APN data & PIN Number
const char GPRS_APN[]      = "internet"; //Change this accordingly
const char GPRS_LOGIN[]    = ""; //Normally empty. Only change if operator requires it
const char GPRS_PASSWORD[] = ""; //Normally empty. Only change if operator requires it
const char PINNUMBER[]     = ""; //Make sure to have a SIM Card without PIN


// initialize the library instance
GSMClient client;
GPRS gprs;
GSM gsmAccess; 

volatile double waterFlow;
int led = 5;
double newtime = minutes*60000;

void setup()
{
  // initialize serial communications
  Serial.begin(9600);
  waterFlow = 0;
  attachInterrupt(0, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
  
  pinMode(led, OUTPUT);

}

void loop()
{
  
  String waterString = String(waterFlow);
  // read temperature sensor
    
  // put reading into path
  //uid = id number; na = location name; la = latitude; lo = longitude; f= flow
  String path = String("{{Add rest of string here}}" + uid + "&na=" + name + "&t=&la=" + latitude + "&lo="+ longitude + "&f=" + waterString); 
  // send result to server
  String uploadResult;
  uploadResult = uploadReading(path);
  Serial.println(uploadResult);

  
  // wait for half an hour
  if (uploadResult == "OK")
  {
    Serial.println(F("Success sending"));
    fastBlink();
    delay(newtime); // 28 minutes 1680000
    
  }
  else
  {
    Serial.println(F("Failure sending"));

 
    delay(300000); // retry in 5 minutes
  }
}



String uploadReading(String path)
{
  String uploadReturnStatus;

  // URL and port
  char server[] = "";//Change this accordingly
  int port = 80; // port 80 is the default for HTTP

  // Start the modem with GSM.begin()
  Serial.println(F("Connecting GSM"));
  // Retry 5 times before giving up
  unsigned loopCntGSM = 5;
  while(loopCntGSM--){
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
    {
      Serial.println(F("GSM connected"));
      fastBlink();
      break;
    }
    else
    {
      Serial.println(F("GSM not connected"));
      delay(1000);
    }
    if(!loopCntGSM)
    {
      Serial.println(F("GSM failed"));
      uploadReturnStatus = "GSM_FAIL";
      return uploadReturnStatus;
    }  
  }
  
  // attach the shield to the GPRS network with the APN, login and password
  Serial.println("Connecting GPRS.");
  // Retry 5 times before giving up
  unsigned loopCntGPRS = 5;
  while(loopCntGPRS--)
  {
    if(gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY)
    {
      Serial.println(F("GPRS connected"));
      fastBlink();
      break;
    }
    else
    {
      Serial.println(F("GPRS not connected"));
      delay(1000);
    }
    if(!loopCntGPRS)
    {
      Serial.println(F("GPRS failed"));
      Serial.println(F("Terminating GSM"));
      gsmAccess.shutdown();
      uploadReturnStatus = "GPRS_FAIL";
      return uploadReturnStatus;
    }   
  }

  // Connect to the server and send readings
  if (client.connect(server, port))
  {
    Serial.println(F("Connected to server"));
    // Make a HTTP request:
    client.print("GET ");
    client.print(path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
  } 
  else
  {
    Serial.println(F("Server connection failed"));
    Serial.println(F("Terminating GSM"));
    gsmAccess.shutdown();
    uploadReturnStatus = "SERVER_FAIL";
    return uploadReturnStatus;
  }

  // Read incoming server response
  String outputString = "";
  while (client.available() || client.connected())
  {
    // if there are incoming bytes available from the server, read them
    if (client.available())
    {
      char c = client.read();
      outputString = outputString + c;
      if (outputString.length() > 15)
      {
        // stop the client.
        Serial.println(F("Disconnecting server"));
        client.flush();
        client.stop();
        break;
      }
    }
  }
  Serial.println(F("Server response;"));
  Serial.println(outputString);
  
  Serial.println(F("Terminating GSM"));
  gsmAccess.shutdown();
  
  // check server response for 200 OK
  if (outputString.startsWith("HTTP/1.1 200 OK"))
  {
    uploadReturnStatus = "OK";
    return uploadReturnStatus;
  } else if (outputString.startsWith("HTTP/1.1 404 Not"))
  {
    uploadReturnStatus = "RESPONSE_FAIL";
    return uploadReturnStatus;
  } else {
    uploadReturnStatus = "OK";
    return uploadReturnStatus;
  }
}


void pulse(){   
  //measure the quantity of square wave
  waterFlow += 1.0 / 13.3;
}

void fastBlink(){
  for (int i = 0; i < 9; i++) {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(100);  
  }
}