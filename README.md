#  Clim intélligente 

**Pour faire fonctionner le PoC**

- Il faut installer la librairies Adafruit BME680 Library par adafruit.

- Indiquer dans les WLAN_SSID et WLAN_PASS le réseau wifi à utiliser.

- Il faut cabler sur la carte servant à l'envoi de donnée sur le Dashboard le capteur de présence sur la pin D4 correspondant a l'ext0 et le capteur de température BME680 en I2C.

- Sur la carte de reception le ventilateur est cablé a la pin D4 sois la pin ext0.

- Pour se connecter au Dashboard sur adafruit io il faut se connecter avec le nom d'utilisateur matteo_brunet et le mdp matteo2712.

# Explication du code des deux cartes SP32 (l’envoie et la réception)

Dans le code disponible sur Git, la carte d’envoi est programmée pour collecter des données à partir de capteurs de températures et de présence, puis pour transmettre ces informations à un serveur MQTT . voir 
L’importation des bibliothèques nécessaires pour le Wi-Fi et le MQTT 
L’objet WiFiClient gère la connexion internet, et Adafruit_MQTT_Client permet la communication avec le serveur MQTT.
Les flux (temperature et presence) sont crée pour publier les données sur le serveur 

**Dans la fonction setup()**
-	Connexion au réseau WiFi avec affichage du statut.
-	Initialisation et vérification du capteur BME680.
-	Configuration du capteur pour des données précises.

**Dans la fonction loop()**
-	Etablissement de la connexion au serveur MQTT.
-	Lecture des données de températures et de présence.
- Publication des valeurs sur le serveur avec affichage des résultats.
- Fonction MQTT_connect() gère les tentatives de connexions au serveur MQTT, avec des messages d’erreur en cas d’échec.
- Fonction de lecture du capteur : vérifie si le capteur MB680 fonctionne correctement et affiche u message d’erreur en cas d’échec avec la fonction test_lecture_bme680().

- La carte de réception est conçue pour recevoir des données de température et de présence via MQTT et pour contrôler un ventilateur en fonction de ces données. Voici les points clés : voir 
- Les bibliothèques WiFi et Adafruit_MQTT sont incluses pour gérer la connexion réseau et le protocole MQTT..
- Utilisation d’objets WiFiClient et Adafruit_MQTT_Client pour établir la connexion Internet et communiquer avec le serveur MQTT.
- Deux flux sont créés pour recevoir les mises à jour de température et de présence.
- Connexion au Wi-Fi, initialisation des abonnements et configuration du pin pour le ventilateur 
- Vérification de la connexion MQTT, lecture des données reçues, et affichage des valeurs.
- La fonction MQTT_connect() gère les tentatives de connexion au serveur, avec des messages d'erreur en cas d'échec. 
