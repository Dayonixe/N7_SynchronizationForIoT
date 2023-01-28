// INCLUDE
#include <SPI.h>
#include <DecaDuino.h>

// DEFINITION DES VARIABLES
#define MAX_FRAME_LEN 120
uint8_t rxData[MAX_FRAME_LEN];
uint16_t rxLen;
DecaDuino decaduino;
int rxFrames;

uint64_t masterTimestamp;
uint8_t masterTimestamp8[8];
int nbTrameRecu;
uint64_t last;
uint64_t offsett;


void setup() {
	pinMode(13, OUTPUT);
	Serial.begin(115200);
	SPI.setSCK(14);

	// Initialise DecaDuino et renvoie false si ce n'est pas réussi
	if ( !decaduino.init() ) {
		Serial.println("decaduino init failed");
		while(1) { digitalWrite(13, HIGH); delay(50); digitalWrite(13, LOW); delay(50); }
	}

	// Set RX buffer and enable RX
	decaduino.setRxBuffer(rxData, &rxLen);
	rxFrames = 0;

	nbTrameRecu = 0;
}


void loop() {
	decaduino.plmeRxEnableRequest();  // Mise en écoute

	if (decaduino.rxFrameAvailable()) {  // Si un message est reçu
		digitalWrite(13, HIGH);

		if (rxData[0] == 7) {  // Si l'adresse de la trame est 7

			// Récupère le temps de réception de la première tame
			nbTrameRecu = nbTrameRecu + 1;
			if (nbTrameRecu == 1) {
				last = decaduino.getLastRxTimestamp();
			}

			// Affichage de la trame
			Serial.print("#"); Serial.print(++rxFrames); Serial.print(" ");
			Serial.print(rxLen);
			Serial.print("bytes received: |");
			for (int i=0; i<rxLen; i++) {
				Serial.print(rxData[i], HEX);
				Serial.print("|");
			}
			Serial.println();

			// Extraction du temps reçu du Master
			Serial.print("Temps reçue : ");
			for (int i=0; i<8; i++) {
				masterTimestamp8[i] = rxData[i+1];      
				Serial.print(masterTimestamp8[i], HEX);
				Serial.print("|");
			}
			Serial.println();

			masterTimestamp = decaduino.decodeUint64(masterTimestamp8);  // Décodage en 64

			// Calcule de l'offset
			if (nbTrameRecu == 2) {
				offsett = last - masterTimestamp;
				Serial.println("offsetcalculated");
				decaduino.printUint64(offsett);
			}
		}
		Serial.println();
		decaduino.plmeRxEnableRequest();
		digitalWrite(13, LOW);
	}
}