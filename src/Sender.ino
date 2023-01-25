#include <SPI.h>
#include <DecaDuino.h>

#define MAX_FRAME_LEN 120
uint8_t txData[MAX_FRAME_LEN];
uint8_t rxData[MAX_FRAME_LEN];
uint16_t txLen;
uint16_t rxLen;
DecaDuino decaduino;
int rxFrames;

// Timeout
bool estReceive = false;
unsigned long timeout = 10000;
unsigned long startTime;


void setup() {
  pinMode(13, OUTPUT); // Internal LED (pin 13 on DecaWiNo board)
  Serial.begin(115200); // Init Serial port
  SPI.setSCK(14); // Set SPI clock pin (pin 14 on DecaWiNo board)

  // Init DecaDuino and blink if initialisation fails
  if ( !decaduino.init() ) {
    Serial.println("decaduino init failed");
    while(1) { digitalWrite(13, HIGH); delay(50); digitalWrite(13, LOW); delay(50); }
  }
  
  // Set RX buffer and enable RX
  decaduino.setRxBuffer(rxData, &rxLen);
  decaduino.plmeRxEnableRequest();
  rxFrames = 0;
}


void loop() {
  decaduino.plmeRxDisableRequest();
  // send
  digitalWrite(13, HIGH);
  txData[0] = 7;
  for (int i=1; i<MAX_FRAME_LEN; i++) {
    txData[i] = i;
  }
  decaduino.pdDataRequest(txData, MAX_FRAME_LEN);
  while ( !decaduino.hasTxSucceeded() );
  digitalWrite(13, LOW);
  Serial.println("send");
  decaduino.plmeRxEnableRequest();



  // set timer
  startTime = millis();



  // waiting ack
  while (!estReceive) {
    if (decaduino.rxFrameAvailable()) {
      digitalWrite(13, HIGH);
      if (rxData[0] == 7) {
        Serial.println("received");
        
        Serial.print("#"); Serial.print(++rxFrames); Serial.print(" ");
        Serial.print(rxLen);
        Serial.print(" bytes received: |");
        for (int i=0; i<rxLen; i++) {
          Serial.print(rxData[i], HEX);
          Serial.print("|");
        }
        Serial.println();
        
        delay(1000);
        loop();
      }
      decaduino.plmeRxEnableRequest();
      digitalWrite(13, LOW);
    } else if (millis() - startTime > timeout) {
      Serial.println("timeout");
      loop();
    }
  }
}
