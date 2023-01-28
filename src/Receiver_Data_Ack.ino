// INCLUDE
#include <SPI.h>
#include <DecaDuino.h>

// DEFINITION DES VARIABLES
#define MAX_FRAME_LEN 120
uint8_t rxData[MAX_FRAME_LEN];
uint16_t rxLen;
DecaDuino decaduino;
int rxFrames;
uint8_t txData[MAX_FRAME_LEN];
uint16_t txLen;


void setup() {
	pinMode(13, OUTPUT);
	Serial.begin(115200);
	SPI.setSCK(14);

	// Initialise DecaDuino et renvoie false si ce n'est pas réussi
	if (!decaduino.init()) {
		Serial.println("decaduino init failed");
		while(1) { digitalWrite(13, HIGH); delay(50); digitalWrite(13, LOW); delay(50); }
	}

	// Set RX buffer
	decaduino.setRxBuffer(rxData, &rxLen);
	rxFrames = 0;
}


void loop() {
	decaduino.plmeRxEnableRequest();  // Mise en écoute

	if (decaduino.rxFrameAvailable()) {  // Si un message est reçu
		digitalWrite(13, HIGH);

		if (rxData[0] == 7) {  // Si l'adresse de la trame est 7
			Serial.print("#"); Serial.print(++rxFrames); Serial.print(" ");
			Serial.print(rxLen);
			Serial.print("bytes received: |");
			for (int i=0; i<rxLen; i++) {
				Serial.print(rxData[i], HEX);
				Serial.print("|");
			}
			Serial.println();

			decaduino.plmeRxDisableRequest();  // Arrêt de l'écoute

			// Envoie d'un ACK après la réception d'une trame d'adresse 7
			txData[0] = 7;
			for (int i=1; i<MAX_FRAME_LEN; i++) {
				txData[i] = i;
			}
			delay(1000);
			decaduino.pdDataRequest(txData, MAX_FRAME_LEN);
			while (!decaduino.hasTxSucceeded());
			Serial.print("Ack sent");
		}

		digitalWrite(13, LOW);
	}
}