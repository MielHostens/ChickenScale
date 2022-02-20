#include <SerialTransfer.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SetterPinDS 2

SerialTransfer myTransfer;

bool Debug = false;
unsigned long RxTxTimer;

struct STRUCTTX {
  float Temperature = 0.0;
} chickenStructTX;


//Initiate DS sensors
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWireSetterDS(SetterPinDS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature SetterDS(&oneWireSetterDS);

void setup() {
  if (Debug) {
    Serial.begin(115200);
  } else {
    Serial.begin(115200);
    Serial3.begin(115200);
    myTransfer.begin(Serial3);
  }
  RxTxTimer = 0;
  // Try to initialize DS sensors
  SetterDS.begin();
  Serial.println("Setup compleet");
}


void loop() {
    SerialRxTx(5000);
}

void SerialRxTx(unsigned long interval) {
  if (millis() - RxTxTimer >= interval){
    RxTxTimer = millis();
    {
      Serial.println("Sending data to ESP8266");
      SetterDS.requestTemperatures();
      chickenStructTX.Temperature = SetterDS.getTempCByIndex(0);
      Serial.print("Temperature: ");
      Serial.println(chickenStructTX.Temperature);
      uint16_t sendSize = 0;// use this variable to keep track of how many bytes we're stuffing in the transmit buffer
      sendSize = myTransfer.txObj(chickenStructTX, sendSize);// Stuff buffer with struct
      myTransfer.sendData(sendSize);// Send buffer
      Serial.println("Data sent to ESP8266");
    }
  }
 }
