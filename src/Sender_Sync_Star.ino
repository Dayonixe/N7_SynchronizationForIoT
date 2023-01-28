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

unsigned long clap = 10000;  // 10 secondes

uint64_t timestamp;
uint8_t timestamp8[8];


void setup() {
	pinMode(13, OUTPUT);
	Serial.begin(115200);
	SPI.setSCK(14);

	// Initialise DecaDuino et renvoie false si ce n'est pas réussi
	if ( !decaduino.init() ) {
		Serial.println("decaduino init failed");
		while(1) { digitalWrite(13, HIGH); delay(50); digitalWrite(13, LOW); delay(50); }
	}
}


void loop() {
	decaduino.plmeRxDisableRequest();

	// Partie émission
	digitalWrite(13, HIGH);

	// Construction de la trame
	txData[0] = 7;
	timestamp = decaduino.getLastTxTimestamp();
	decaduino.encodeUint64(timestamp, timestamp8);
	for (int i=0; i<8; i++) {
		txData[i+1] = timetemp[i+1];
	}

	// Transmission
	decaduino.pdDataRequest(txData, MAX_FRAME_LEN);
	while (!decaduino.hasTxSucceeded());
	digitalWrite(13, LOW);
	Serial.println("send");

	// Attente du prochain CLAP
	delay(clap);
}