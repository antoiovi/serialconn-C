serialconn-C
===========================================

Programma dimostrativo della libreria arduino-serial descritta in questo blog L: http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino/,
il cui codice sorgente si trova https://github.com/todbot/arduino-serial.git

Con questa libreria si puo' accedere ad una porta seriale (USB) e quindi anche
ad Arduino , per leggere e scrivere dati.

Nel file serialconn.c e' stata creata una struttura che accorpa i dati di
configurazione di una porta seriale , ed alcune funzioni che si appogiano alla
libreria arduino-serial; questo per rendere piu' semplice l'utilizzo, e per non
esporre direttamente la libreria al programmatore.

Vi sono due programmi per provare le funzioni : runmyserial e  runbythread

Il file runmyserial contiene un semplice esempio di utilizzo.


Per compilare basta eseguire
  $make runmyserial
e per esegiire il programma
  $ ./bin/runmyserial
  
Il file runbythread contiene un esempio che utiliza un thread per leggere 
in continuo i dati da prota seriale


Per compilare basta eseguire
  $make runbythread
e per esegiire il programma
  $ ./bin/runbythread

Linux users : se non si accede alla porta seriale verificare i privilegi.
