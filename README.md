serialconn-C
===========================================

Programma dimostrativo della libreria arduino-serial descritta in questo blog L: http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino/,
il cui codice sorgente si trova https://github.com/todbot/arduino-serial.git

Con questa libreria si pu0' acceder ad una porta seriale (USB) e quindi anche
ad Arduino , per leggere e scrivere dati.

Nel file serialconn.c e' stata creata una struttura che accorpa i dati di
configurazione di una porta seriale , ed alcune funzioni che si appogiano alla
libreria arduino-serial; questo per rendere pi' semplice l'utilizzo, e per non
esporre direttamente la libreria al programmatore.

Il file runmyserial contiene un semplice esempio di utilizzo.


Per compilare basta eseguire
  $make
e per esegiire il programma
  $ ./bin/runmyserial

Linux users : se non si accede alla porta seriale verificare i privilegi.
