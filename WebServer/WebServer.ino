// FukkenRF Web Server
// inspired by a Web Server example from year 2009 by David A. Mellis.

#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>

#include <HardwareSerial.h>
HardwareSerial Serial;

// Set your desired MAC and IP addresses, you might want to change these below
byte mac[] = { 0xCA, 0xFE, 0x01, 0x02, 0x03, 0x04 };
IPAddress ip(10, 10, 10, 10);

// Set port 
EthernetServer server(80);

// REMEMBER TO CHECK THE RIGHT CHIP SELECT PIN!
// 


#define HFENABLE 3

#define LOVHF1  54
#define LOVHF2   2
#define HF1      5
#define HF2      6
#define HF3      7
#define HF4      8
#define HF5      9
#define HF6     11
#define VHF1    12
#define VHF2    13
#define VHF3    14
#define VHF4    15
#define VHF5    16
#define VHF6    17


void setup() {
  pinMode(LOVHF1, OUTPUT);
  pinMode(LOVHF2, OUTPUT);
  pinMode(HF1, OUTPUT);
  pinMode(HF2, OUTPUT);
  pinMode(HF3, OUTPUT);
  pinMode(HF4, OUTPUT);
  pinMode(HF5, OUTPUT);
  pinMode(HF6, OUTPUT);
  pinMode(VHF1, OUTPUT);
  pinMode(VHF2, OUTPUT);
  pinMode(VHF3, OUTPUT);
  pinMode(VHF4, OUTPUT);
  pinMode(VHF5, OUTPUT);
  pinMode(VHF6, OUTPUT);

  // REMEMBER TO CHECK THE RIGHT CHIP SELECT PIN!
  Ethernet.init(ETHERNET_CS_PIN);

  Serial.begin(9600);
  while (!Serial) {}  // wait for it

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet shield's presense
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet: not found");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet: link down");
  }

  // start the server
  server.begin();
  Serial.print("listening at ");
  Serial.println(Ethernet.localIP());
}


unsigned int newPin = 0;
unsigned int currentPin = 0;


void setPin() {
  if (newPin != currentPin) {
    digitalWrite(HFENABLE, LOW);
    digitalWrite(LOVHF1, LOW);
    digitalWrite(LOVHF2, LOW);
    digitalWrite(HF1, LOW);
    digitalWrite(HF2, LOW);
    digitalWrite(HF3, LOW);
    digitalWrite(HF4, LOW);
    digitalWrite(HF5, LOW);
    digitalWrite(HF6, LOW);
    digitalWrite(VHF1, LOW);
    digitalWrite(VHF2, LOW);
    digitalWrite(VHF3, LOW);
    digitalWrite(VHF4, LOW);
    digitalWrite(VHF5, LOW);
    digitalWrite(VHF6, LOW);

    delay(100);

    if ((HF1==newPin) ||
        (HF2==newPin) ||
        (HF3==newPin) ||
        (HF4==newPin) ||
        (HF5==newPin) ||
        (HF6==newPin)) {
      digitalWrite(HFENABLE, HIGH);
    }

    digitalWrite(newPin, HIGH);
  }
  currentPin = newPin;
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    boolean firstLine = true;
    unsigned int firstCharN = 0;
    char firstLineBuf[5] = "";
    char postCompare[5] = "POST";
    boolean wasPostReq = false;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          if (wasPostReq) {
            //pin = client.read() - 65;
            newPin = c - 65;
          }
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Access-Control-Allow-Origin: *");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          if (wasPostReq) {
            client.println("Got POST!!");
            newPin = client.read() - 65;
            client.print("New pin: ");
            client.println(newPin);
          } else {
            client.println("Didn't get POST :(");
          }
          client.println("</html>");
          setPin();
          break;
        }
        if (c == '\n') {
          firstLine = false;
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }

        if (firstLine && firstCharN < sizeof(firstLineBuf)) {
          firstLineBuf[firstCharN] = c;
          if (0 == memcmp(firstLineBuf, postCompare, sizeof(postCompare))) {
            wasPostReq = true;
          }
          firstCharN++;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
