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

#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // for usleep()
#include <getopt.h>
#include <stdbool.h>

#include "serialconn.h"
#include "arduino-serial-lib.h"

const int buf_max = 256;

#define MAX_LINES 10000

// uncomment this to debug reads
//#define SERIALCONN_DEBUG

void set_default_serialport(struct serialport* port) {
	port->port_name = "/dev/ttyUSB0";
	port->fd = -1;
	port->baudrate = 9600;
	port->eolchar = '\n';
	port->timeout = 5000;
}

/**
 * Ottiene il file descriptor della porta
 * 	: se la porta non 'e aperta restituisce -1
 */
int serialconn_init(struct serialport* port) {
	port->fd = serialport_init(port->port_name, port->baudrate);
	#ifdef SERIALCONN_DEBUG
		printf("port->fd= %d", port->fd);
	#endif
	return port->fd;
}



/**
 * Legge n_lines dalla porta, e per ogni stringa letta esegue
 *  una funzione che accetta come parametro una stringa
 *  restituisce il numero di righe lette , oppre -1 se porta non disponibile
 */
int serialconn_read_lines(struct serialport* port, int n_lines,
		int (*functionPtr)(char*)) {
	char buf[buf_max];
	if (port->fd == -1) {
		return -1;
	}
	#ifdef SERIALCONN_DEBUG
		printf("EOLCHAR %3d    \n", port->eolchar);
		printf("Timeout %3d    \n", port->timeout);
	#endif

	int count = 0;
	do {
		memset(buf, 0, buf_max);  //
		serialport_read_until(port->fd, buf, port->eolchar, buf_max,
				port->timeout);
		if (strlen(buf) < 1)
			continue;
		functionPtr(buf);
		count++;
	#ifdef SERIALCONN_DEBUG
			printf("(%3d)   %s\n", count, buf);
	#endif
	} while (count < n_lines);
	return count;
}

