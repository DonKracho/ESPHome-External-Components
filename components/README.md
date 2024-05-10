ESPHome External Components

Quigg Funksteckdosenset (Steckdosen: GT-FSI-09 (innen), GT-FSA-04 (außen) Sender: GT-9000)

Dieses Funkstekdosenet wurde ab 2014 bei Aldi (Nord) angeboten. Es erwies sich als gut, weswegen ich nach und nach gleich mehrere dieser Sets gekauft habe.
Die hohe Störsicherheit wird durch einen sogenannten Rooling-Code erreicht, d. h. der Schalt-Code ändert sich mit jeder Bedienung. Dadurch kann kann man
aber nicht auf die Standard-Komponente Remote-Transmitter zurückgreifen die immer den gleichen Code für An und Aus senden würde.
Ich habe seiner Zeit intensiv am Reverse Engeniering getüftelt um die Stekdosen mit einem ESP und 433MHz Sender bedienen zu können.
Die Erkennnisse von damals habe ich hier nun in eine externe ESPHome Komponente migriert. Einige Bezeichnungen entspechen daher nicht den ESPHome konventionen. 

Jeder GT-9000 Sender verwendet eine individuelle Code-Basis mit der bis zu 16 Kanäle gesteuert werten können. Der Kanal 10 ist für die Master-Funktion
reserviert. Die Steckdosen habe keine eigene Codierung sondern können auf bis zu 4 Codes bzw. Sender gleichzeitig angelert werden. Das war in 2014 durchaus ein
revolutionäres Konzept, konnten doch 433MHz-Funksektosen zuvor noch durch Brute-Force-Methoden getriggert werden.

Nach dem Einstecken in eine Steckdose blinkt die blaue LED der Steckdose für ca. 15 Sekunden um die Programmier-Phase zu signalisieren. In dieser Zeit kann
die Steckdose duch Betätigung einer ON-Taste eines Kanals angelernt werden. Die Betätigung des OFF-Taster entfert den angelernten Code wieder aus der Steckdose.
Mit Master-OFF in der Programmier-Phase werden alle gespeicherten Codes der Steckdose wieder gelöscht.

Diese Stekdosen gelten auch heute noch als betriebsicher, warum also nicht weiterbenutzen? Der Standby-Verbrauch ist sogar etwas geringer als bei modernen smarten WLAN-Stekdosen.

In dieser externen ESPHome Komponente ist exemplarisch der Rolling-Code eines Senders implementiert. Es werden die Komonenten rf_switch und rf_outlet benötigt,
siehe README.md in den entsprechenden Verzeichnissen. Bislang ist es nur für ESP8266 inplementiert, z.B. Wemos D1 mini. Die ESP32 Fragmente wurden von der ac_dimmer
Komponente kopiert, sind aber ungetestet. We es braucht, kann es gerne für den ESP32 anpassen und einen PR beisteuern.

Fie Komponente rf_transmiter war mein erster Ansatz vergleichbar der Implemetation der ESPHome Komponente remote_transmitter. Dies erwies sich aber eher als eine Fingerübung für mich,
wie Automationen getriggert werden können. Eine Automation ist der der deutlich kompliziertere Weg. Nachdem ich nicht gleich herausgefunden habe,
wie ich der Automation Parameter übergeben kann, habe ich diesen Ansatz also wieder verworfen. Der Code kann nur als Demonstartion für Automations-Templates genutzt werden.
