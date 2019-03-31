/*
 * runmyserial
 * --------------
 *Un semplice programma per provare la libreria serialconn, e la libreria
 *arduino-serial  tratta dal blog http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino/
 *
 * In questo esempio viene inizializzata la porta seria con parametri di default,
 * poi viene utilizzata funzione serialconn_read_lines(&port, n_lines, functionPtr);
 * che esegue per ogni riga letta dalla porta seriale un funzione da noi creata:
 *
 * 		- una volta esegue  taskonreadedsting(char* buf), che stampa a video la riga letta
 *
 * 		- un'altra volta esegue (per 5 righe) int checkAnaloRead(char* buf)
 * 		  che verifica se la stringa letta inizia per "AnalogRead"
 *
 *
 * Compilare con
 * gcc -o runmyserial.out runmyserial.c serialconn.c arduino-serial-lib.c -lpthread
 *
 * or usare il Makefile incluso;
 *
 * Usare (linux): ./runmyserial.out
 */

#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // for usleep()
#include <getopt.h>
#include <stdbool.h>
#include <pthread.h>
#include "arduino-serial-lib.h"
#include "serialconn.h"

int checkAnaloRead(char* buf);
void readlast();
int startsWith(const char *pre, const char *str);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
char buf[8][256];

struct serialport port;
struct serialport *ptrPort;



void *serialconn_loop(void *vargp) {
	int count = 0;
	for (;;) {
 		// Legge una riga dalla porta seriale
		pthread_mutex_lock(&mutex1);
		serialport_read_until(ptrPort->fd, buf[count], ptrPort->eolchar, 256,
				ptrPort->timeout);
		sleep(1);
		pthread_mutex_unlock(&mutex1);
		count++;
		if (count == 8)
			count = 0;
		sleep(1);

	}
	return NULL;
}

//
void error(char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}



int checkAnaloRead(char* buf) {
	char check_string[] = "AnalogRead";
	if (buf == '\0')
		return -1;
	if (startsWith(check_string, buf))
		printf("(  %s\n", buf);
	else
		printf("( Non inizia per AnalogRead... \n");
	return 1;
}

int (*functionPtr)(char*);

int main(int argc, char *argv[]) {

	pthread_t thread_serialread;

	ptrPort = &port;
	set_default_serialport(ptrPort);

	if (serialconn_init(&port) == -1)
		error("Errore apertura porta!!");
	else {
		// Porta aperta, lancia il thread
		printf("Crea Thread... \n");
		pthread_create(&thread_serialread, NULL, serialconn_loop, NULL);
		printf("...Thread Ceato \n");

	}

	// pthread_join(thread_id, NULL);
	int c;
	do {
		printf("Inserire a per finire \n");
		printf("Inserire r per leggere l'ultima stringa \n");
		printf(">");
		int c;

		c = getchar();
		printf("\n");
		printf("c=%d   char='%c'\n", c, c);
		if (c == 'a')
			break;
		if (c == 'r')
			readlast();
		else
			printf("c!=r");
		// flush stdin :
		int f;
		while ((f = getchar()) != '\n' && f != EOF)
			;

	} while (c != (int) 'a');
	printf("Closing thread....\n");

	pthread_cancel(thread_serialread);
	printf("Closing port ...\n");
	serialport_close(ptrPort->fd);

	for (int x = 0; x < 8; x++) {
		printf("buf[%d]=%s ", x, buf[x]);
	}

	exit(EXIT_SUCCESS);
} // end main

void readlast() {
	printf("Readlast \n");

	int x = 7;
	pthread_mutex_lock(&mutex1);
	printf("First buf[%d]=%s ", 0, buf[0]);
	printf("Lsat buf[%d]=%s ", x, buf[x]);
	pthread_mutex_unlock(&mutex1);

}

/**
 * Utility : verifca se stringa str inizia con sottostringa pre
 * (0= false;   1= true; )
 */
int startsWith(const char *pre, const char *str) {
	size_t lenpre = strlen(pre), lenstr = strlen(str);
	return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

