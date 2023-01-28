// INCLUDE
#include <SPI.h>
#include <DecaDuino.h>

// DEFINITION DES VARIABLES
#define MAX_FRAME_LEN 120
uint8_t txData[MAX_FRAME_LEN];
uint8_t rxData[MAX_FRAME_LEN];
uint16_t txLen;
uint16_t rxLen;
DecaDuino decaduino;
int rxFrames;

// Timeout
bool estReceive = false;
unsigned long timeout = 10000;  // 10 secondes

unsigned long startTime;


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
	decaduino.plmeRxDisableRequest();  // Arrêt de l'écoute

	// Partie émission
	digitalWrite(13, HIGH);

	// Construction de la trame
	txData[0] = 7;
	for (int i=1; i<MAX_FRAME_LEN; i++) {
		txData[i] = i;
	}

	// Transmission
	decaduino.pdDataRequest(txData, MAX_FRAME_LEN);
	while (!decaduino.hasTxSucceeded());
	digitalWrite(13, LOW);
	Serial.println("send");
	decaduino.plmeRxEnableRequest();  // Mise en écoute

	// Lancement le timer après l'envoie de la trame
	startTime = millis();


	// Attente d'un ACK
	while (!estReceive) {
		if (decaduino.rxFrameAvailable()) {
			digitalWrite(13, HIGH);

			// Filtre sur les messages d'adresse 7
			if (rxData[0] == 7) {
				Serial.println("received");

				// Affichage de la trame d'ACK reçue
				Serial.print("#"); Serial.print(++rxFrames); Serial.print(" ");
				Serial.print(rxLen);
				Serial.print(" bytes received: |");
				for (int i=0; i<rxLen; i++) {
					Serial.print(rxData[i], HEX);
					Serial.print("|");
				}
				Serial.println();

				// Transmission de la trame suivante
				delay(1000);
				loop();
			}

			decaduino.plmeRxEnableRequest();
			digitalWrite(13, LOW);

		// Si le timer est dépassé, on renvoie la trame
		} else if (millis() - startTime > timeout) {
			Serial.println("timeout");
			loop();
		}
	}
}