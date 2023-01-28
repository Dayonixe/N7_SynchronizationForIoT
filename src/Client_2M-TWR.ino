// INCLUDE
#include <SPI.h>
#include <DecaDuino.h>

// Timeout parameters
#define TIMEOUT_WAIT_START_SENT 5 //ms
#define TIMEOUT_WAIT_ACK 10 //ms
#define TIMEOUT_WAIT_DATA_REPLY 20 //ms

// Ranging period parameter
#define RANGING_PERIOD 500 //ms

// TWR client states state machine enumeration: see state diagram on documentation for more details
enum { TWR_ENGINE_STATE_INIT, TWR_ENGINE_STATE_WAIT_START_SENT, TWR_ENGINE_STATE_MEMORISE_T1,
TWR_ENGINE_STATE_WAIT_ACK, TWR_ENGINE_STATE_MEMORISE_T4, TWR_ENGINE_STATE_WAIT_DATA_REPLY, 
TWR_ENGINE_STATE_EXTRACT_T2_T3 };

// Message types of the TWR protocol
#define TWR_MSG_TYPE_UNKNOWN 0
#define TWR_MSG_TYPE_START 1
#define TWR_MSG_TYPE_ACK 2
#define TWR_MSG_TYPE_DATA_REPLY 3

uint64_t t1, t2, t3, t4;
uint64_t mask = 0xFFFFFFFFFF;
int32_t tof;

DecaDuino decaduino;
uint8_t txData[128];
uint8_t rxData[128];
uint16_t rxLen;
int state;
uint32_t timeout;


void setup() {
	pinMode(13, OUTPUT);
	Serial.begin(115200);
	SPI.setSCK(14);

	// Initialise DecaDuino et renvoie false si ce n'est pas réussi
	if ( !decaduino.init() ) {
		Serial.println("decaduino init failed");
		while(1) { digitalWrite(13, HIGH); delay(50); digitalWrite(13, LOW); delay(50); }
	}

	// Set RX buffer
	decaduino.setRxBuffer(rxData, &rxLen);
	state = TWR_ENGINE_STATE_INIT;

	// Affichage de l'entête des colonnes
	Serial.println("ToF\td\tToF_sk\td_sk");
}

void loop() {
	float distance;

	switch (state) {

		case TWR_ENGINE_STATE_INIT:
			delay(RANGING_PERIOD);
			decaduino.plmeRxDisableRequest();
			Serial.println("New 2M-TWR");
			txData[0] = TWR_MSG_TYPE_START;
			txData[1] = 7;
			decaduino.pdDataRequest(txData, 2);

			// Affichage du message envoyé
			for (int i = 0; i < 2; i++) {
				Serial.print(txData[i], HEX);
				Serial.print("|");
			}
			Serial.println();

			timeout = millis() + TIMEOUT_WAIT_START_SENT;
			state = TWR_ENGINE_STATE_WAIT_START_SENT;
		break;

		case TWR_ENGINE_STATE_WAIT_START_SENT:
			if ( millis() > timeout ) {
				state = TWR_ENGINE_STATE_INIT;
			} else {
				if ( decaduino.hasTxSucceeded() ) {
					state = TWR_ENGINE_STATE_MEMORISE_T1;
				}
			}
		break;

		case TWR_ENGINE_STATE_MEMORISE_T1:
			t1 = decaduino.getLastTxTimestamp();
			timeout = millis() + TIMEOUT_WAIT_ACK;
			decaduino.plmeRxEnableRequest();
			state = TWR_ENGINE_STATE_WAIT_ACK;
		break;

		case TWR_ENGINE_STATE_WAIT_ACK:
			if ( millis() > timeout ) {
				state = TWR_ENGINE_STATE_INIT;
			} else {
				if ( decaduino.rxFrameAvailable() ) {
					if (rxData[0] == TWR_MSG_TYPE_DATA_REPLY && rxData[1] == 7) {
						// Affichage du message reçu
						for (int i = 0; i < 20; i++) {
							Serial.print(rxData[i], HEX);
							Serial.print("|");
						}
						Serial.println();

						state = TWR_ENGINE_STATE_MEMORISE_T4;
					} else {
						decaduino.plmeRxEnableRequest();
						state = TWR_ENGINE_STATE_WAIT_ACK;
					}
				}
			}
		break;

		case TWR_ENGINE_STATE_MEMORISE_T4:
			t4 = decaduino.getLastRxTimestamp();
			timeout = millis() + TIMEOUT_WAIT_DATA_REPLY;
			decaduino.plmeRxEnableRequest();
			state = TWR_ENGINE_STATE_EXTRACT_T2_T3;
		break;

		case TWR_ENGINE_STATE_EXTRACT_T2_T3:
			t2 = decaduino.decodeUint40(&rxData[2]);
			t3 = decaduino.decodeUint40(&rxData[7]);
			tof = (((t4 - t1) & mask) - ((t3 - t2) & mask))/2;
			distance = tof*RANGING_UNIT;
			Serial.print(tof);
			Serial.print("\t");
			Serial.print(distance);
			tof = (((t4 - t1) & mask) - (1+1.0E-6*decaduino.getLastRxSkew())*((t3 - t2) & mask))/2;
			distance = tof*RANGING_UNIT;
			Serial.print("\t");
			Serial.print(tof);
			Serial.print("\t");
			Serial.println(distance);
			state = TWR_ENGINE_STATE_INIT;
		break;

		default:
			state = TWR_ENGINE_STATE_INIT;
		break;
	}
}