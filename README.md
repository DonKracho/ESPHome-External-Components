# ESPHome External Components

## Quigg Funksteckdosen-Set
### Steckdosen: GT-FSI-09 (innen), GT-FSA-04 (außen) Sender: GT-9000

![Quigg Funksteckdosen-Set](https://github.com/DonKracho/ESPHome-External-Components/blob/main/images/quigg_funksteckdosen.jpg)

Dieses Funksteckdosen-Set wurde bei Aldi (Nord) ab 2014 angeboten. Es erwies sich als gut, weswegen ich nach und nach gleich mehrere dieser Sets gekauft habe. Die hohe Störsicherheit wird durch einen sogenannten Rolling-Code erreicht, d. h. der Schalt-Code ändert sich mit jeder Bedienung. Dadurch kann kann man aber nicht auf die ESPHome Standard-Komponente Remote-Transmitter zurückgreifen, die immer den gleichen Code für An und Aus senden würde. Ich habe seiner Zeit intensiv am Reverse Engeniering getüftelt um die Stekdosen mit einem ESP und 433MHz Sender bedienen zu können. Die Erkennnisse von damals habe ich hier nun in eine externe ESPHome Komponente migriert. Einige Bezeichnungen entspechen daher nicht den ESPHome konventionen. 

Jeder GT-9000 Sender verwendet eine individuelle Code-Basis mit der bis zu 16 Kanäle (15 Steckdosen + Master) gesteuert werten können. Der Kanal 10 ist dabei für die Master-Funktion reserviert. Die Steckdosen besitzen keine eigene Codierung sondern können auf bis zu 4 Codes bzw. Sender gleichzeitig angelert werden. Das war in 2014 durchaus ein revolutionäres Konzept, konnten doch 433MHz-Funkseckdosen zuvor noch durch Brute-Force-Methoden getriggert werden.

Nach dem Einstecken in eine Steckdose blinkt die blaue LED der Funksteckdose für ca. 15 Sekunden um die Programmier-Phase zu signalisieren. In dieser Zeit kann die Steckdose durch die Betätigung der ON-Taste eines Kanals angelernt werden. Die Betätigung der OFF-Taste entfert den angelernten Code wieder aus der Steckdose. Mit Betätigung von Master OFF-Taste in der Programmier-Phase werden alle gespeicherten Codes der Funksteckdose wieder gelöscht. Nach jeder Progammier-Aktion beendet die Funksteckdose den Progammier-Modus und die LED hört auf zu blinken. Will man mehr, muss man sie ggf. herausziehen und neu einstecken.

Diese Funksteckdosen gelten für mich auch heute noch als betriebsicher. Ich habe nie einen falschen Trigger bemerkt. Warum also nicht weiterbenutzen? Der Standby-Verbrauch ist sogar etwas geringer als bei modernen smarten WLAN-Stekdosen.

In dieser externen ESPHome Komponente ist exemplarisch der Rolling-Code eines Senders implementiert. Es werden die Komonenten __rf_switch__ und __rf_outlet__ benötigt, siehe README.md im __rf_switch__ Verzeichniss. Bislang ist es nur für ESP8266 inplementiert, z. B. Wemos D1 mini. Die ESP32 Fragmente wurden von der __ac_dimmer__ Komponente kopiert, sind aber ungetestet. We es braucht, kann es gerne für den ESP32 anpassen und einen PR beisteuern.

Fie Komponente __rf_transmiter__ war mein erster Ansatz vergleichbar der Implemetation der ESPHome Komponente __remote_transmitter__. Dies erwies sich aber eher als eine Fingerübung für mich, wie Automationen getriggert werden können. Eine Automation ist der der deutlich kompliziertere Weg. Nachdem ich nicht gleich herausgefunden habe, wie ich der Automation Parameter übergeben kann, habe ich diesen Ansatz also wieder verworfen. Der Code kann als Demonstartion für Automations-Templates genutzt werden.

Für den einfachen Nachbau noch ein kleines Fritzing Schema. Als 433MHz Sender ist ein simpler FS1000A (oder ähnlicher) hinreichend. Als TX Pin ist im der ESPHome Beispielkonfiguration __quigg-rf-outlets.yaml__ D4 (GPIO2) gewählt worden, das kann aber an die eigenen Bedürfnisse angepasst werden.

![Quigg Funksteckdosen-Set](https://github.com/DonKracho/ESPHome-External-Components/blob/main/images/quigg_funksteckdose_schaltplan.jpg)
