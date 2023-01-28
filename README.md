# Project - Synchronization for IoT

Team : Nouhaila A. & Théo Pirouelle

<a href="https://www.arduino.cc/">
  <img src="https://img.shields.io/badge/language-Arduino-008184?style=flat-square" alt="laguage-arduino" />
</a>

---

## Installation and set up

First of all, you have to download the Arduino IDE in version 1.8.x : [Download link](https://www.arduino.cc/en/software)

We can then install Teensy : [Download link](https://www.pjrc.com/teensy/td_download.html)

And finally we can install the DecaDuino library : [Download link](https://github.com/irit-rmess/DecaDuino)

Simply access the menu `Sketch > Include Library > Add .ZIP Library`

To be able to use the IDE, you have to select the board in the menu `Tools > Board > Teensuduino > Teensy 3.2 / 3.1`

---

## Getting started with the UWB transceiver

### Getting to grips with the sample sketches

We can see the lack of addressing in the messages, it would be better to set up a logical isolation to receive only the messages that concern us.

We only modify `loop()` in the *Receiver* and *Sender* code.

**For the transmitter:**

```c
void loop() {
	digitalWrite(13, HIGH);

	// On envoie les trames avec une adresse 7
	txData[0] = 7;
	for (int i=1; i<MAX_FRAME_LEN; i++) {
		txData[i] = i;
	}

	decaduino.pdDataRequest(txData, MAX_FRAME_LEN);
	while ( !decaduino.hasTxSucceeded() );
	digitalWrite(13, LOW);
	Serial.println("send");

	delay(1000);
}
```

On the initial code, we add `txData[0] = 7;` to have an address field (with `7` as identifier) in order to have a logical isolation. For the rest of our message, we leave the `for` loop of automatic filling from index 1 of our `txData` array.

**For the receiver:**

```c
void loop() {
	if (decaduino.rxFrameAvailable()) {  // Si un message est reçu
		digitalWrite(13, HIGH);

		if (rxData[0] == 7) {  // Si l'adresse de la trame est 7 
			Serial.print("#"); Serial.print(++rxFrames); Serial.print(" ");
			Serial.print(rxLen);
			Serial.print("bytes received: |");

			// Affichage de la trame
			for (int i=0; i<rxLen; i++) {
				Serial.print(rxData[i], HEX);
				Serial.print("|");
			}
		}
		Serial.println();

		decaduino.plmeRxEnableRequest();
		digitalWrite(13, LOW);
	}
}
```

On the initial code, we add the condition `if (rxData[0] == 7)` to filter only on the messages which concern us. The rest of the frame is displayed with the `for` loop.

We then obtain the following display:

```
#1 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#2 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#3 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#4 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#5 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#6 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|
```

We notice that the receiver receives only the frames of address 7.

### Design of a first simple protocol

We will now implement a simple `Data+ACK` with the value `timeout` configurable.

The protocol is as follows:

<p align="center">
  <img src="/doc/dataAck.png" alt="dataAck" width="320"/>
</p>

When the sender sends a message, if the receiver responds in time, we have a simple exchange with a message (similar to the previous part with the address field) and a good reception message (`ACK`) similar to the sending message but in the other direction. However, if the receiver does not respond before the end of the timer, the sender automatically retransmits his message.

In this part as our two nodes, `Master` and `Slave`, have to send and receive messages, we have to be careful to indicate at the right times when they are ready to listen or not with `plmeRxEnableRequest()` and `plmeRxDisableRequest()`.

**For the transmitter:**

The message transmission part is the same as the previous part. For the part of waiting for the ACK, we loop in an infinite way on the reception of an ACK. If this one does not arrive in time, we pass in the condition of the `timeout` which is reached, we then retransmit a message.

**For the receiver:**

The receiver listens to the passing messages. If there is a message at the address we are interested in (address `7` here), we display the message as in the previous part and then we send a reception message (`ACK`). We then go back to listening mode and wait for a new message.

## MAC synchronization

### Implementation of a star synchronization

We will now implement a star synchronization protocol.

The protocol is as follows:

<p align="center">
  <img src="/doc/star.png" alt="star" width="480"/>
</p>

On a regular basis, the `Master` node broadcasts a `clap` message indicating the time at which it sends the `clap`. Slave nodes receive this message, calculate their `offset` with the `Master` node and apply it to their local time.

**For the transmitter:**

The sender sends regularly (here every 10 seconds) a frame which contains the address (configured in the same way as the previous parts) and the timestamp of the last transmitted frame.

**For the receiver:**

The receiver listens, when it receives frames it displays them as in the previous parts. When it receives the first frame, it records its `timestamp`, then when it receives the second frame, it extracts the `timestamp` of the first frame from the `Master` in its message. With his `timestamp` and that of the `Master`, he can calculate his `offset`.

This has not been implemented here but it would be necessary to modify the local receiver clock by adding or subtracting the `offset` to synchronize with the `Master`.

### Implementation of a SiSP synchronization

We will now implement the SiSP synchronization protocol.

The protocol is as follows:

<p align="center">
  <img src="/doc/sisp.png" alt="sisp" width="550"/>
</p>

In this protocol there is no more master or slave, it is simply nodes that share a "common" clock (built over time). Moreover, in this part all the nodes have the same code.

All nodes agree on a shared clock. Each node has two counters, `LCLK` and `SCLK`. `LCLK` is the local clock, incremented at the local clock rate, and `SCLK` is the clock shared with all other nodes. `SCLK` is initialized with `LCLK` at startup.

The `SCLK` clock is calculated on each node with the following formula:

$$
SCLK = \frac{LCLK+RCLK}{2}
$$

`RCLK` corresponds to the clock received from another node.

**For nodes:**

The code uses a local clock (`lclk`) to measure time locally and a shared time (`sclk`) to synchronize the clock with other nodes in the network.

It uses a main loop in which it listens for received frames every second except every 10 seconds when it sends a message containing its shared clock (`sclk`). With `printUint64()` we display the shared clock.

We then obtain the following results:

```
send

#9 120bytes received: |7|0|0|0|0|C0|81|3|7|0|0|0|
Temps reçu : |0|0|0|0|C0|81|3|7|
Clock SCLK : 0381c0e00000005b

send

#10 120bytes received: |7|0|0|70|E0|C0|81|3|7|0|0|0|
Temps reçu : |0|0|70|E0|C0|81|3|7|
Clock SCLK : 0381c0e070380060

send

#11 120bytes received: |7|1C|38|70|E0|C0|81|3|7|0|0|0|
Temps reçu : |1C|38|70|E0|C0|81|3|7|
Clock SCLK : 0381c0e070381c73
```

```
#1 120bytes received: |7|0|0|0|0|0|80|3|7|0|0|0|
Temps reçu : |0|0|0|0|0|80|3|7|
Clock SCLK : 0381c00000000004

send

#2 120bytes received: |7|0|0|0|E0|C0|81|3|7|0|0|0|
Temps reçu : |0|0|0|E0|C0|81|3|7|
Clock SCLK : 0381c0e070000009

send

#3 120bytes received: |7|0|38|70|E0|C0|81|3|7|0|0|0|
Temps reçu : |0|38|70|E0|C0|81|3|7|
Clock SCLK : 0381c0e070381c0e
```

We then notice that on the 2 nodes the `SCLK` clocks tend towards the same value without ever obtaining exactly the same one, which is normal because there is always a very slight offset due to the hardware. Moreover, between the moment when the node receives a `SCLK` from another node, calculates its new `SCLK` value and retransmits it to the other nodes, time continues to pass and thus `SCLK` continues to increment.

## Ranging (fine synchronization)

### Sample sketches

We will start by taking the various example sketches and understanding how the Two-Way Ranging (`TWR`) and Symmetric Double Sided Two-Way Ranging (`SDS-TWR`) protocols work.

**Two-Way Ranging :**

The protocol is as follows:

<p align="center">
  <img src="/doc/twr.png" alt="twr" width="250"/>
</p>

Two-Way Ranging (`TWR`) is a protocol used to determine the distance between two devices using the time-of-flight (`ToF`) measurement technique. It uses a two-way transmission to calculate distance by measuring the time it takes for a signal to travel from one device to the other and back. The `ToF` can be determined using the following formula:

$$
ToF = \frac{t_4 - t_1 - (t_3 - t_2)}{2}
$$

We can then determine the distance between the client and the server by multiplying the `ToF` to a `RANGING_UNIT`.

However, this method gives higher values than the reality. It is possible to compensate for this difference (`skew`) with the following formula:

$$
ToF = \frac{t_4 - t_1 - (1 - skew * 10^{-6})(t_3 - t_2)}{2}
$$

The results are then much closer to reality.

This can be seen in the following results:

```
ToF			d				ToF_sk	d_sk
117			0.52		86			0.38
114			0.50		75			0.33
113			0.50		75			0.33
116			0.51		77			0.34
114			0.50		74			0.33
118			0.52		86			0.38
113			0.50		75			0.33
112			0.50		74			0.33
120			0.53		85			0.38
118			0.52		83			0.37
```

**Symmetric Double Sided Two-Way Ranging :**

The protocol is as follows:

<p align="center">
  <img src="/doc/sdsTwr.png" alt="sdsTwr" width="250"/>
</p>

Symmetric Double Sided Two-Way Ranging (`SDS-TWR`) is a variant of `TWR` that uses symmetric two-way transmission to improve the accuracy of distance measurement. It uses two devices to transmit and receive signals simultaneously, rather than using one device as a transmitter and the other as a receiver. This reduces measurement errors caused by factors such as transmission power variations and time offsets. The `ToF` can be determined using the following formula:

$$
ToF = \frac{t_4 - t_1 - (t_3 - t_2) + t_6 - t_3 - (t_5 - t_4)}{4}
$$

We can then determine the distance between the client and the server by multiplying the `ToF` to a `RANGING_UNIT`.

We also notice here that we obtain results closer to reality than with the TWR protocol without compensation:

```
ToF		d
94		0.42
83		0.37
89		0.39
82		0.36
86		0.38
81		0.36
89		0.39
79		0.35
85		0.38
80		0.35
```

We can see the lack of addressing in the messages, it would be better to set up a logical isolation to receive only the messages that concern us.

We only modify `loop()` in the *Server* and *Client* code.

**For the server:**

```c
void loop() {
	switch (state) {

		case TWR_ENGINE_STATE_INIT:
			// [...]

		case TWR_ENGINE_STATE_WAIT_START:
			if (decaduino.rxFrameAvailable()) {
				if (rxData[0] == TWR_MSG_TYPE_START && rxData[1] == 7) {  // Ajout de la vérification de l'adresse
					state = TWR_ENGINE_STATE_MEMORISE_T2;
					Serial.println("TWR_ENGINE_STATE_WAIT_START");
				} else {
					state = TWR_ENGINE_STATE_INIT;
				}
			}
			break;

		case TWR_ENGINE_STATE_MEMORISE_T2:
			// [...]

		case TWR_ENGINE_STATE_SEND_ACK:
			txData[0] = TWR_MSG_TYPE_ACK;
			txData[1] = 7;  // Construction due la trame avec l'adresse
			decaduino.pdDataRequest(txData, 2);  // Transmission de 2 éléments
			timeout = millis() + TIMEOUT_WAIT_ACK_SENT;
			state = TWR_ENGINE_STATE_WAIT_ACK_SENT;
			Serial.println("TWR_ENGINE_STATE_SEND_ACK");
			break;

		case TWR_ENGINE_STATE_WAIT_ACK_SENT:
			// [...]

		case TWR_ENGINE_STATE_MEMORISE_T3:
			// [...]

		case TWR_ENGINE_STATE_SEND_DATA_REPLY:
			delay(ACK_DATA_REPLY_INTERFRAME);
			txData[0] = TWR_MSG_TYPE_DATA_REPLY;
			txData[1] = 7;  // Construction due la trame avec l'adresse
			decaduino.encodeUint40(t2, &txData[2]);  // Décalage de 1
			decaduino.encodeUint40(t3, &txData[7]);  // Décalage de 1
			decaduino.pdDataRequest(txData, 12);  // Transmission de 12 éléments
			timeout = millis() + TIMEOUT_WAIT_DATA_REPLY_SENT;
			state = TWR_ENGINE_STATE_WAIT_DATA_REPLY_SENT;
			Serial.println("TWR_ENGINE_STATE_SEND_DATA_REPLY");
			break;
 
		case TWR_ENGINE_STATE_WAIT_DATA_REPLY_SENT:
			// [...]

		default:
			// [...]
	}
}
```

On the initial code, we chose to add the address field at index 1 of txData in order to leave the message types in the first slot. We then add `txData[1] = 7;` to have an address "field" (with `7` as identifier) in order to have a logical isolation. Be careful to shift the locations of $t_2$ and $t_3$ in `TWR_ENGINE_STATE_SEND_DATA_REPLY` to avoid overwriting the address field. We also add the condition `if (rxData[0] == 7)` to filter only on the messages that concern us.

**For the client:**

```c
void loop() {
	float distance;

	switch (state) {

		case TWR_ENGINE_STATE_INIT:
			delay(RANGING_PERIOD);
			decaduino.plmeRxDisableRequest();
			Serial.println("New TWR");
			txData[0] = TWR_MSG_TYPE_START;
			txData[1] = 7;  // Construction due la trame avec l'adresse
			decaduino.pdDataRequest(txData, 2);  // Transmission de 2 éléments

			// Affichage du message envoyé
			for (int i = 0; i < 2; i++) {
				Serial.print(rxData[i], HEX);
				Serial.print("|");
			}
			Serial.println();

			timeout = millis() + TIMEOUT_WAIT_START_SENT;
			state = TWR_ENGINE_STATE_WAIT_START_SENT;
			break;

		case TWR_ENGINE_STATE_WAIT_START_SENT:
			// [...]

		case TWR_ENGINE_STATE_MEMORISE_T1:
			// [...]

		case TWR_ENGINE_STATE_WAIT_ACK:
			if ( millis() > timeout ) {
				state = TWR_ENGINE_STATE_INIT;
			} else {
				if ( decaduino.rxFrameAvailable() ) {
					if (rxData[0] == TWR_MSG_TYPE_ACK && rxData[1] == 7) {  // Ajout de la vérification de l'adresse
						// Affichage du message reçu
						for (int i = 0; i < 12; i++) {
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
			// [...]

		case TWR_ENGINE_STATE_WAIT_DATA_REPLY:
			if ( millis() > timeout ) {
				state = TWR_ENGINE_STATE_INIT;
			} else {
				if ( decaduino.rxFrameAvailable() ) {
					if ( rxData[0] == TWR_MSG_TYPE_DATA_REPLY && rxData[1] == 7) {  // Ajout de la vérification de l'adresse
						// Affichage du message reçu
						for (int i = 0; i < 12; i++) {
							Serial.print(rxData[i], HEX);
							Serial.print("|");
						}
						Serial.println();

						state = TWR_ENGINE_STATE_EXTRACT_T2_T3;
					} else {
						decaduino.plmeRxEnableRequest();
						state = TWR_ENGINE_STATE_WAIT_DATA_REPLY;
					}
				}
			}
			break;

		case TWR_ENGINE_STATE_EXTRACT_T2_T3:
			// [...]

		default:
			// [...]
	}
}
```

We make the same changes as the server.

We then obtain the following display:

```
ToF	d			ToF_sk	d_sk
New TWR
1|7|
2|7|7E|A3|50|AA|AF|4F|10|83|AB|AF|
3|7|CB|F7|EC|44|B7|4F|70|1D|46|B7|
137	0.61	103	0.46

New TWR
1|7|
2|7|CB|F7|EC|44|B7|4F|70|1D|46|B7|
3|7|55|CB|8A|DF|BE|4F|6E|BB|E0|BE|
118	0.52	83	0.37

New TWR
1|7|
2|7|55|CB|8A|DF|BE|4F|6E|BB|E0|BE|
3|7|54|74|2A|7A|C6|4F|68|5A|7B|C6|
120	0.53	86	0.38

New TWR
1|7|
2|7|54|74|2A|7A|C6|4F|68|5A|7B|C6|
3|7|C7|26|CA|14|CE|4F|96|FA|15|CE|
134	0.59	100	0.44

New TWR
1|7|
2|7|C7|26|CA|14|CE|4F|96|FA|15|CE|
3|7|90|80|6A|AF|D5|4F|B2|9A|B0|D5|
117	0.52	84	0.37
```

We notice that the client receives only the frames of address `7`.

### Implementation of 2M-TWR

The protocol is as follows:

<p align="center">
  <img src="/doc/2mTwr.png" alt="2mTwr" width="250"/>
</p>

The 2-Messages Two-Way Ranging (`2M-TWR`) protocol is a protocol used to determine the distance between two devices using the time-of-arrival (`ToA`) scanning technique. It is a two-dimensional location method that uses two message exchanges to determine the distance between the two devices.

**For the server:**

The server follows the same steps as the `TWR` protocol but the value of $t_3$ is "predicted" and included in the ACK to the client.

**For the client:**

As for the server, the client is very similar to the `TWR` protocol but it expects only one message instead of two.

We then obtain the following result:

```
ToF	d			ToF_sk	d_sk
New 2M-TWR
1|7|
3|7|DD|BC|6C|AD|9|4F|78|72|3A|2|C|D|E|F|10|11|12|13|
-852780025	-3771887.25	-2147483648	-9498424.00

New 2M-TWR
1|7|
3|7|AE|54|1B|E2|18|4F|7C|E2|6E|11|C|D|E|F|10|11|12|13|
-850692584	-3762654.25	-2147483648	-9498424.00

New 2M-TWR
1|7|
3|7|64|93|C9|16|28|4F|6C|93|A3|20|C|D|E|F|10|11|12|13|
-850805469	-3763153.25	-2147483648	-9498424.00

New 2M-TWR
1|7|
3|7|E3|8F|76|4B|37|4F|7E|43|D8|2F|C|D|E|F|10|11|12|13|
-850936806	-3763734.75	-2147483648	-9498424.00

New 2M-TWR
1|7|
3|7|71|25|23|80|46|4F|D0|EE|C|3F|C|D|E|F|10|11|12|13|
-850829275	-3763258.75	-2147483648	-9498424.00
```

Unfortunately we get inconsistent results. This can be due to a bad "prediction" of $t_3$, to errors in the measurements or to a transmission error.
