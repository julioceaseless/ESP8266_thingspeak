#include <SoftwareSerial.h>
#include <stdlib.h>
#include <String.h>
#include <dht.h>

dht DHT;

#define DHT11_PIN 5
char ledPin = 13;
//  thingspeak API key
String apiKey = "9107BL5PDTLSXXXX"; //thingspeak
String twitterAPIkey = "XXXX7G2ZH329V7AS"; // tweeting bird
String field[]= {"&field1=","&field2="} ;
String f2     = "&field2=";
String reset  = "AT+RST";
//tweet message
String tweet = "It is very hot in here @julioceaseless";

// connect 2 to TX of Serial USB
// connect 3 to RX of serial USB
SoftwareSerial ser(2,3); // RX, TX

// this runs once
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);    

  // enable debug serial
  Serial.begin(9600);
  // enable software serial
  ser.begin(9600);
  //AT+IPR=9600; set baud rate
  // reset ESP8266
  ser.println(reset);
}

// the loop
void loop() {

  // blink LED on board
  digitalWrite(ledPin, HIGH);   
  delay(200);               
  digitalWrite(ledPin, LOW);

//read value from the DHT11 temp and hum sensor
 
 int chk = DHT.read11(DHT11_PIN); //check status of DHT11
 while (chk == DHTLIB_OK){
    float temp = DHT.temperature; //read temperature
    float hum = DHT.humidity; // read humidity
    Serial.println(temp);
    Serial.println(hum);
    // convert temperature to string
    char buf_temp[16];
    String strTemp = dtostrf(temp, 4, 1, buf_temp);

    //convert humidity to string
    char buf_hum[16];    
    String strHum = dtostrf(hum, 4, 1, buf_hum);
    
    String weatherParam[] = {strTemp, strHum};
    
    // TCP connection
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += "184.106.153.149"; // api.thingspeak.com
    cmd += "\",80";
    ser.println(cmd);
    Serial.println(cmd);
    

    if(ser.find("Error")||ser.find("closed")){
      Serial.println("AT+CIPSTART error");
      return;
      }
  
    // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +=field[0];
  getStr += String(weatherParam[0]);
  getStr +=field[1];
  getStr +=String(weatherParam[1]);
  getStr += "\r\n";

  
    // send data length 
    cmd = "AT+CIPSEND=";
    cmd += String(getStr.length());
    Serial.println(cmd);
    ser.println(cmd);
    if(ser.find(">")){
      ser.print(getStr);
      Serial.println(getStr);
      } 
      else{
        ser.println("AT+CIPCLOSE");
        // alert user
        Serial.println("AT+CIPCLOSE");
        }
   // thingspeak needs 15 sec delay between updates
    delay(60000);
    
 }
    
  }

/**
 *  POST String
String postRequest (){
String postStr =
"POST " + uri + " HTTP/1.0\r\n" +
"Host: " + server + "\r\n" +
"Connection: "close\r\n" +
"Content-Type: application/x-www-form-urlencoded\r\n" +
"Content-Length: " + data.length() + "api_key="+twitterAPIkey+"&status="+tweet
return postStr;
}
*/
