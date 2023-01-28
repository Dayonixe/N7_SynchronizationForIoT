# Project - Synchronization for IoT

Team : Nouhaila A. & Théo Pirouelle

<a href="https://www.arduino.cc/">
  <img src="https://img.shields.io/badge/language-Arduino-008184?style=flat-square" alt="laguage-arduino" />
</a>

---

## Installation et mise en place

Il faut dans un premier temps télécharger l'IDE de Arduino en version 1.8.x : [Download link](https://www.arduino.cc/en/software)

On peut ensuite installer Teensy : [Download link](https://www.pjrc.com/teensy/td_download.html)

Et finalement on peut installer la library DecaDuino : [Download link](https://github.com/irit-rmess/DecaDuino)

Il suffit d'accéder au menu `Sketch > Include Library > Add .ZIP Library`

Pour pouvoir ensuite utiliser l'IDE, il faut bien penser à séléctionner la board dans le menu `Tools > Board > Teensuduino > Teensy 3.2 / 3.1`

---

## Prise en main du transceiver UWB

### Prise en main des sketchs exemples

On peut constater l’absence d’adressage dans les messages, il serait préférable de mettre en place une isolation logique pour ne recevoir que les messages qui nous concerne.

On modifie uniquement `loop()` dans le code du *Receiver* et du *Sender*.

**Pour l’émetteur :**

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

Sur le code initial, on rajoute `txData[0] = 7;` pour avoir un “champ” d’adresse (avec `7` comme identificateur) afin d’avoir une isolation logique. Pour le reste de notre message, on laisse la boucle `for` de remplissage automatique à partir de l’indice 1 de notre tableau `txData`.

**Pour le récepteur :**

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

Sur le code initial, on rajoute la condition `if (rxData[0] == 7)` pour filtrer uniquement sur les messages qui nous concerne. Le reste de la trame est affichée avec la boucle `for`.

On obtient alors l’affichage suivant :

```
#1 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#2 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#3 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#4 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#5 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|

#6 120bytes received: |7|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|2F|30|31|32|33|34|35|36|37|38|39|3A|3B|3C|3D|3E|3F|40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51|52|53|54|55|56|57|58|59|5A|5B|5C|5D|5E|5F|60|61|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E|6F|70|71|72|73|74|75|76|77|
```

On remarque que le récepteur reçoie seulement les trames d’adresse 7.

### Conception d’un premier protocole simple

Nous allons à présent implémenter un simple `Data+ACK` avec la valeur `timeout` paramétrable.

Le protocole est le suivant :

<p align="center">
  <img src="/doc/dataAck.png" alt="dataAck" width="700"/>
</p>

Lorsque l’émetteur envoie un message, si le récepteur répond dans les temps, nous avons un échange simple avec un message (similaire à la partie précédente avec le champ d’adresse) et un message de bonne réception (`ACK`) similaire au message d’émission mais dans l’autre sens. Cependant, si le récepteur ne répond pas avant la fin du timer, l’émetteur retransmet automatiquement son message.

Dans cette partie comme nos deux nœuds, `Master` et `Slave`, doivent émettre et recevoir des messages, il faut faire attention à indiquer aux bons moments quand est-ce qu’ils sont prêt à écouter ou non avec `plmeRxEnableRequest()` et `plmeRxDisableRequest()`.

**Pour l’émetteur :**

La partie de transmission de message est la même que la partie précédente. Pour la partie d’attente de l’ACK, nous bouclons de manière infini sur la réception d’un ACK. Si celui-ci n’arrive pas à temps, nous passons dans la condition du `timeout` qui est atteint, on retransmet alors un message.

**Pour le récepteur :**

Le récepteur écoute les messages qui passent. Si un message à l’adresse qui nous intéresse (adresse `7` ici), on affiche le message comme dans la partie précédente puis on émet un message de réception (`ACK`). On se remet ensuite en mode d’écoute et on attend un nouveau message.

## Synchronisation MAC

### Implémentation d’une synchronisation en étoile

Nous allons à présent implémenter un protocole de synchronisation en étoile.

Le protocole est le suivant :

<p align="center">
  <img src="/doc/star.png" alt="star" width="700"/>
</p>

Régulièrement, le nœud `Master` diffuse un message `clap` indiquant l’heure à laquelle il envoie le `clap`. Les nœuds `Slave` reçoivent ce message, calculent leur `offset` avec le nœud `Master` et l’appliquent sur leur heure locale.

**Pour l’émetteur :**

L’émetteur envoie de manière régulière (ici toutes les 10 secondes) une trame qui contient l’adresse (configuré de la même manière que les parties précédentes) et le timestamp de la dernière trame transmise.

**Pour le récepteur :**

Le récepteur se met en écoute, lorsqu’il reçoit des trames il les affiches comme dans les parties précédentes. Lors de la réception de la première trame, il enregistre son `timestamp`, puis à la réception de la seconde trame, il extrait le `timestamp` de la première trame du `Master` contenu dans son message. Avec son `timestamp` et celui du `Master`, il peut calculer son `offset`.

Cela n’a pas été implémenté ici mais il faudrait modifier l’horloge local du récepteur en ajoutant ou soustrayant l’`offset` pour se synchroniser avec le `Master`.

### Implémentation d’une synchronisation de SiSP

Nous allons à présent implémenter le protocole de synchronisation de SiSP.

Le protocole est le suivant :

<p align="center">
  <img src="/doc/sisp.png" alt="sisp" width="700"/>
</p>

Dans ce protocole il n’y a plus de maître ou d’esclave, il s’agit simplement de nœuds qui se partagent une horloge “commune” (construire au fil du temps). De plus, dans cette partie tout les nœuds possèdent le même code.

Tous les nœuds sont d'accord sur une horloge partagée. Chaque nœud possède deux compteurs, `LCLK` et `SCLK`. `LCLK` est l'horloge locale, incrémentée à la vitesse de l'horloge locale et `SCLK` est l'horloge partagée avec tous les autres nœuds. `SCLK` est initialisée avec `LCLK` au démarrage.

L’horloge `SCLK` est calculée sur chaque nœud avec la formule suivante :

$$
SCLK = \frac{LCLK+RCLK}{2}
$$

`RCLK` correspond à l’horloge reçu d’un autre nœud.

**Pour les nœuds :**

Le code utilise une horloge locale (`lclk`) pour mesurer le temps localement et une heure partagée (`sclk`) pour synchroniser l'horloge avec les autres nœuds dans le réseau.

Il utilise une boucle principale dans laquelle il écoute pour les trames reçues toutes les secondes sauf toutes les 10 secondes où il envoie un message contenant son horloge partagée (`sclk`). Avec `printUint64()` on affiche l'horloge partagée.

On obtient alors les résultats suivant :

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

On remarque alors que sur les 2 nœuds les horloges `SCLK` tendent vers la même valeur sans jamais obtenir exactement la même ce qui est normal car il y a toujours un très léger décalage dû au matériel. De plus, entre le moment où le nœud reçoit un `SCLK` d’un autre nœud, calcul sa nouvelle valeur de `SCLK` et la retransmet aux autres nœuds, le temps continue de s’écouler et donc `SCLK` de s’incrémenter.

## Ranging (synchronisation fine)

### Sketchs exemples

Nous allons commencer par prendre en main les différents sketchs d’exemple et comprendre le fonctionnement des protocoles Two-Way Ranging (`TWR`) et Symmetric Double Sided Two-Way Ranging (`SDS-TWR`).

</aside>

**Two-Way Ranging :**

Le protocole est le suivant :

<p align="center">
  <img src="/doc/twr.png" alt="twr" width="700"/>
</p>

Two-Way Ranging (`TWR`) est un protocole utilisé pour déterminer la distance entre deux dispositifs en utilisant la technique de mesure de temps de vol (`ToF`). Il utilise une transmission à double sens pour calculer la distance en mesurant le temps qu'il faut à un signal pour aller d'un dispositif à l'autre et retourner. On peut déterminer le `ToF` à l’aide de la formule suivante :

$$
ToF = \frac{t_4 - t_1 - (t_3 - t_2)}{2}
$$

On peut par la suite déterminer la distance entre le client et le serveur en multipliant le `ToF` à une `RANGING_UNIT`.

Cependant cette méthode donne des valeurs supérieurs à la réalité. Il est possible de compenser cette différence (`skew`) à l’aide de la formule suivante :

$$
ToF = \frac{t_4 - t_1 - (1 - skew * 10^{-6})(t_3 - t_2)}{2}
$$

On obtient alors des résultats bien plus proche de la réalité.

On peut le remarquer dans les résultats suivants :

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

Le protocole est le suivant :

<p align="center">
  <img src="/doc/sdsTwr.png" alt="sdsTwr" width="700"/>
</p>

Symmetric Double Sided Two-Way Ranging (`SDS-TWR`) est une variante de `TWR` qui utilise une transmission à double sens symétrique pour améliorer la précision de la mesure de distance. Il utilise deux dispositifs pour émettre et recevoir des signaux simultanément, plutôt que d'utiliser un dispositif comme émetteur et l'autre comme récepteur. Cela permet de réduire les erreurs de mesure causées par des facteurs tels que les variations de la puissance de transmission et les décalages temporels. On peut déterminer le `ToF` à l’aide de la formule suivante :

$$
ToF = \frac{t_4 - t_1 - (t_3 - t_2) + t_6 - t_3 - (t_5 - t_4)}{4}
$$

On peut par la suite déterminer la distance entre le client et le serveur en multipliant le `ToF` à une `RANGING_UNIT`.

On remarque également ici qu’on obtient des résultats plus proche de la réalité qu’avec le protocole TWR sans compensation :

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

On peut constater l’absence d’adressage dans les messages, il serait préférable de mettre en place une isolation logique pour ne recevoir que les messages qui nous concerne.

On modifie uniquement `loop()` dans le code du *Server* et du *Client*.

**Pour le serveur :**

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

Sur le code initial, nous avons choisi d’ajouter le champ d’adresse à l’indice 1 de txData afin de laisser les types de message en premier slot. On ajoute alors `txData[1] = 7;` pour avoir un “champ” d’adresse (avec `7` comme identificateur) afin d’avoir une isolation logique. Il faut faire attention de bien décaler les emplacements de $t_2$ et $t_3$ dans `TWR_ENGINE_STATE_SEND_DATA_REPLY` pour éviter d’écraser le champ d’adresse. On ajoute également la condition `if (rxData[0] == 7)` pour filtrer uniquement sur les messages qui nous concerne.

**Pour le client :**

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

On effectue les mêmes modifications que le serveur.

On obtient alors l’affichage suivant :

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

On remarque que le client reçoie seulement les trames d’adresse 7.

### Implémentation de 2M-TWR

Le protocole est le suivant :

<p align="center">
  <img src="/doc/2mTwr.png" alt="2mTwr" width="700"/>
</p>

Le protocole 2-Messages Two-Way Ranging (`2M-TWR`) est un protocole utilisé pour déterminer la distance entre deux dispositifs à l'aide de la technique de balayage de temps d'arrivée (`ToA`). Il s'agit d'une méthode de localisation à deux dimensions qui utilise deux échanges de messages pour déterminer la distance entre les deux dispositifs.

**Pour le serveur :**

Le serveur suis les mêmes étapes que celle du protocole `TWR` mais la valeur de $t_3$ est “prédite” et intégrée à l’ACK vers le client.

**Pour le client :**

De même que pour le serveur, le client est très semblable au protocole `TWR` mais il n’attend qu’un seul message au lieu de 2.

On obtient alors le résultat suivant :

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

Malheureusement nous obtenons des résultats non cohérent. Cela peut être dû à une mauvaise “prédiction” de $t_3$, à des erreurs dans les mesures ou encore une erreur de transmission.
