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
unsigned long timeout = 10000;
unsigned long startTime;

uint64_t sclk;
uint8_t sclk8[8];
uint64_t lclk;
uint8_t lclk8[8];
uint64_t nodeClock;
uint8_t nodeClock8[8];


void setup() {
	pinMode(13, OUTPUT);
	Serial.begin(115200);
	SPI.setSCK(14);

	// Initialise DecaDuino et renvoie false si ce n'est pas réussi
	if ( !decaduino.init() ) {
		Serial.println("decaduino init failed");
		while (1) { digitalWrite(13, HIGH); delay(50); digitalWrite(13, LOW); delay(50); }
	}

	// Set RX buffer and enable RX
	decaduino.setRxBuffer(rxData, &rxLen);
	rxFrames = 0;

	lclk = 0;  // Horloge locale
	sclk = 0;  // Horloge partagée
}


void loop() {
	decaduino.plmeRxEnableRequest();
	for (int i = 0; i < 10; i++) {
		lclk = lclk + 1;  // Horloge locale
		sclk = sclk + 1;  // Horloge partagée

		if (lclk % 10 != 0) {
			// Ecoute
			if (decaduino.rxFrameAvailable()) {
				digitalWrite(13, HIGH);
				if (rxData[0] == 7) {
					// On affiche ce qu'on a reçu
					Serial.print("#"); Serial.print(++rxFrames); Serial.print(" ");
					Serial.print(rxLen);
					Serial.print("bytes received: |");
					for (int i = 0; i < 12; i++) {
						Serial.print(rxData[i], HEX);
						Serial.print("|");
					}
					Serial.println();

					// On récupère le temps reçu
					Serial.print("Temps reçu : |");
					for (int i = 0; i < 8; i++) {
						nodeClock8[i] = rxData[i + 1];
						Serial.print(nodeClock8[i], HEX);
						Serial.print("|");
					}
					Serial.println();

					// On le decode en 64
					nodeClock = decaduino.decodeUint64(nodeClock8);

					// On calcule la différence
					sclk = (lclk + nodeClock) / 2;
					Serial.print("Clock SCLK : ");
					decaduino.printUint64(sclk);
					Serial.println();
				}

				Serial.println();
				decaduino.plmeRxEnableRequest();
				digitalWrite(13, LOW);
			}
		} else {
			// Transmission
			decaduino.plmeRxDisableRequest();
			digitalWrite(13, HIGH);
			txData[0] = 7;   // Configuration de l'adresse

			decaduino.encodeUint64(sclk, sclk8);
			for (int i = 0; i < 8; i++) {
				txData[i + 1] = sclk8[i + 1];
			}

			decaduino.pdDataRequest(txData, MAX_FRAME_LEN);
			while ( !decaduino.hasTxSucceeded() );
			digitalWrite(13, LOW);
			Serial.println("send");
			decaduino.plmeRxEnableRequest();
		}

		delay(1000);
	}
}