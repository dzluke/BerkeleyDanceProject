/*---------------------------------------------------------------------------------------------

  GameTrak Controller

  Luke Dzwonczyk 2021

--------------------------------------------------------------------------------------------- */

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>

//char ssid[] = "Spaceship Kineviz";          // your network SSID (name)
//char pass[] = "12345678";                    // your network password

char ssid[] = "noisense";          // your network SSID (name)
char pass[] = "";                    // your network password


IPAddress dest_ip;        // remote IP of your computer, let's broadcast the data


//const char* GAMETRAK_ID = "3";

WiFiUDP Udp;                                  // A UDP instance to let us send and receive packets over UDP
const unsigned int dest_port = 7000;          // remote port to send OSC to
const unsigned int local_port = 7001;         // local port to receive OSC

const int num_pins = 3;
int pins[] = {32, 33, 36};
char* osc_addrs[] = {"/3/x", "/3/y", "/3/z"};

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(local_port);
  Serial.print("Local port: ");
  Serial.println(local_port);

// this doesnt work
//  for (int i = 0; i < sizeof(osc_addrs) / sizeof(osc_addrs[0]); i++) {
//    char addr[5];
//    strcpy(addr, "/");
//    strcat(addr, GAMETRAK_ID);
//    strcat(addr, osc_addrs[i]);
//    strcpy(osc_addrs[i], addr);
//    
//  }

}

void loop() {
  OSCBundle bundle;
  
  int packet_size = Udp.parsePacket();
  if (packet_size) {  // if UDP packet was received
    dest_ip = Udp.remoteIP();
  }
  
  // read pin values and add to OSC bundle
  for (int i = 0; i < num_pins; i++) {
    int val = analogRead(pins[i]);
    bundle.add(osc_addrs[i]).add(val);
  }

  // send values over UDP
  Udp.beginPacket(dest_ip, dest_port);
  bundle.send(Udp);
  Udp.endPacket();
  bundle.empty();

  delay(10);

}
