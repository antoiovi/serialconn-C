/**
 * Collezione di funzioni che utilizzano la libreria arduino-serial
 * di  todbot/arduino-serial tratta nel blog http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino/
 *
 * Sono create
 * 		una struct che contiene i dati di configurazione
 * 		Una funzione per inizzializzare la porta con dei dati di default
 * 		Una funzione che legge un certo numero di linee, e che per ogni linea esegue una funzione
 * 				passata come parametro;
 * 				La funzione passata come parametro deve avere un char* come parametro e restituire un int
 * 				Per esempio :
 * 				  int  (*functionPtr)(char* );
 *
 * 				 int taskonreadedsting(char* buf){
 *						printf("task %s ",buf);
 *						}
 *				serialconn_read_lines(&port, n_lines, &taskonreadedsting);
 *
 *	 2019 :Antonllo Iovino
 *
 *
 **/



#ifndef __SERIALCONN_H__
#define __SERIALCONN_H__

#include <stdint.h>   // Standard types 

struct serialport{
	char *port_name;
 	int fd ; //file description
	int baudrate ;  // default
	char eolchar ;
	int timeout ;
};

/*
 * Valori di default
port->port_name = "/dev/ttyUSB0";
port->fd = -1;
port->baudrate = 9600;
port->eolchar = '\n';
port->timeout = 5000;
*/
void set_default_serialport(struct serialport* port);


 /**
 * Ottiene il file descriptor della porta
 * 	: se la porta non 'e aperta restituisce -1
 */
int serialconn_init(struct serialport* port) ;


/**
 * Legge n_lines dalla porta, e per ogni stringa letta esegue
 *  una funzione che accetta come parametro una stringa
 */
 int serialconn_read_lines(struct serialport* port, int n_lines, int  (*functionPtr)(char* )) ;


#endif

