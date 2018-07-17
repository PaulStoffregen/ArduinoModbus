/*
  WiFi Modbus TCP Client Toggle

  This sketch toggles the coil of a Modbus TCP server connected
  on and off every second.

  Circuit:
   - MKR1000 or MKR WiFi 1010 board

  created 16 July 2018
  by Sandeep Mistry
*/

#include <SPI.h>
#include <WiFiNINA.h> // for MKR WiFi 1010
// #include <WiFi101.h> // for MKR1000

#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiClient wifiClient;
ModbusTCPClient modbusTCPClient(wifiClient);

IPAddress server(192, 168, 1, 10); // update with the IP Address of your Modbus server

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Modbus TCP Client Toggle");

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the status:
  printWifiStatus();

  // start the Modbus RTU client
  if (!modbusTCPClient.begin(server)) {
    Serial.println("Failed to start Modbus TCP Client!");
    while (1);
  }
}

void loop() {
  // write the value of 0x01, to the coil at address 0x00 
  if (!modbusTCPClient.writeCoil(0x00, 0x01)) {
    Serial.print("Failed to write coil! ");
    Serial.println(modbusTCPClient.lastError());
  }

  // wait for 1 second
  delay(1000);

  // write the value of 0x00, to the coil at address 0x00 
  if (!modbusTCPClient.writeCoil(0x00, 0x00)) {
    Serial.print("Failed to write coil! ");
    Serial.println(modbusTCPClient.lastError());
  }

  // wait for 1 second
  delay(1000);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}