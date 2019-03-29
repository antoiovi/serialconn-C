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


struct serialport port;
struct serialport *ptrPort;
// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *myThreadFun(void *vargp)
{
    sleep(5);
    printf("Printing GeeksQuiz from Thread \n");
    return NULL;
}
//
void error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

  int taskonreadedsting(char* buf){
	  if(buf=='\0') return -1;
	printf("task %s ",buf);
	return 1;
}

int checkAnaloRead(char* buf) {
    char check_string[]="AnalogRead";

	if (buf == '\0')return -1;
	if (startsWith(check_string, buf))
		printf("(  %s\n", buf);
	else
		printf("( Non inzia per AnalogRead... \n");
	return 1;
}

 int  (*functionPtr)(char* );

int main(int argc, char *argv[])
{

	 pthread_t thread_id;
	    printf("Before Thread\n");
	    pthread_create(&thread_id, NULL, myThreadFun, NULL);
	    pthread_join(thread_id, NULL);
	    printf("After Thread\n");

    ptrPort=&port;
    set_default_serialport(ptrPort);


    functionPtr = &taskonreadedsting;

 	int n_lines=5;
	if(serialconn_init(&port)==-1)
		error("Errore apertura porta!!");
	serialconn_read_lines(&port, n_lines, functionPtr);

	serialconn_read_lines(&port, n_lines, &checkAnaloRead);

    exit(EXIT_SUCCESS);
} // end main


/**
 * Utility : verifca se stringa str inizia con sottostringa pre
 * (0= false;   1= true; )
 */
int startsWith(const char *pre, const char *str) {
	size_t lenpre = strlen(pre), lenstr = strlen(str);
	return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

