/* 
Questo file contiene le funzioni che  gestiscono l'input dell'utente.
*/

#include "lib.h"
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>

/* Gestire correttamente l'input è stata senza dubbio la parte più difficoltosa di questo progetto. La difficoltà deriva dal fatto che volevo realizzare un gioco di snake eseguibile da terminale ma mentre un terminale è programmato per ASPETTARE l'input dell'utente il gioco snake deve continuare a generare fotogrammi anche in assenza di input.
Se doveste trovarvi in una situazione simile sappiate che la libreria ncurses è progettata appositamente per realizzare interfacce grafiche attraverso i terminali. Se la comodità fosse stata la mia priorità ncurses sarebbe stata la scelta più saggia. Tuttavia questo avrebbe significato introdurre una dipendenza esterna ed ero determinato a realizzare un programma che avesse bisogno solo di Linux e della libreria standard del C.
Mi sono quindi imbattuto nella libreria termios.h, la quale permette di interfacciarsi con le proprietà del terminale, e nel concetto di input non canonico. Brevemente, un terminale in modalita non canonica può decidere per quanto tempo aspettare l'input dell'utente. Per realizzare questa funzionalità ho modificato leggermente, non comprendendo appieno quello che stavo facendo, un esempio reperito sul manuale della libreria standard del c.
Documentazione: https://www.gnu.org/software/libc/manual/html_node/Noncanonical-Input.html
Esempio: https://www.gnu.org/software/libc/manual/html_node/Noncanon-Example.html
*/

/*Devo creare una variabile globale per salvare le proprietà del terminale in modalità canonica. Sarebbe bene evitare le variabili globali ma non ho trovato una soluzione migliore*/
struct termios saved_attributes;

/*Questa funzione viene chiamata all'uscita del programma e riporta il terminale in modalità canonica. Senza di essa all'uscita dal gioco il terminale risulta inutilizzabile*/
void reset_input_mode (void) {
	tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

//Imposta la modalità non-canonica
void set_input_mode (void) {
	struct termios tattr; // Scriveremo qui le nuove proprietà del terminale

	/* Make sure stdin is a terminal. */
	if (!isatty (STDIN_FILENO)) {
		fprintf (stderr, "Not a terminal.\n");
		exit (EXIT_FAILURE);
	}

	//Salva le vecchie proprietà del terminale
	tcgetattr (STDIN_FILENO, &saved_attributes);
	//La funzione per ripristinarle verrà chiamata all'uscita del programma
	atexit (reset_input_mode); 

	/* Set the funny terminal modes. */
	tcgetattr (STDIN_FILENO, &tattr); //GET ATTRIBUTES dal terminale
	//Su questa prossima linea non sono sicuro, la tilde è un bitwise not
	tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */ 
	tattr.c_cc[VMIN] = 0;  //Min è il numero minimo di byte da leggere, quindi 0
	tattr.c_cc[VTIME] = 0; //Tempo da attendere, quindi 0
	tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr); //SET ATTRIBUTES del terminale
	//TCSAFLUSH specifica quando effettuare il cambiamento ossia dopo aver mandato tutto l'output all'fd ma prima di aver ricevuto nuovi input(cancella i pending input)
}
/* Fine del pasticcio incomprensibile, il resto del codice è scritto da me e so spiegare come funziona */

/*Questa funzione legge da standard input, quindi dalla tastiera, i tasti premuti dall'utente, un carattere alla volta. Se i tasti premuti non sono utili al controllo del gioco la funzione restituisce 0, come se non avesse letto niente*/
char kb_read() {
	char ch;

	//Legge un carattere dallo standard input e lo mette in ch
	read (STDIN_FILENO, &ch, 1);
	//Ritorna il valore di ch se esso corrisponde a WASD o al tasto invio.
	if ((ch=='w')||(ch=='a')||(ch=='s')||(ch=='d')||(ch=='\n')) {
			return ch;
		}
		
/*Questa sequenza di istruzioni serve a catturare la pressione delle frecce direzionali. Esse sono rappresentate dal terminale come ANSI Escape Sequences, quindi sequenze di più caratteri. Le sequenze per le frecce direzionali sono "\033[A","\033[B","\033[C,"\033[D". Se catturiamo il primo carattere della sequenza leggiamo il secondo e così via. Se ad un certo punto la sequenza non corrisponde la scartiamo completamente e usciamo dalla funzione.
Per saperene di più: https://stackoverflow.com/questions/4130048/recognizing-arrow-keys-with-stdin */
	if (ch=='\033'){
		read (STDIN_FILENO, &ch, 1);
		if (ch =='[') {
			read (STDIN_FILENO, &ch, 1);
			//"Traduco" la pressione delle frecce in WASD per comodità
			switch	(ch) {
				case 'A':
					return 'w';
				case 'B':
					return 's';
				case 'C':
					return 'd';
				case 'D':
					return 'a';
				default: //scarto le altre escape sequences
					return 0;			
			}
		}				
	}
	//Scarto la pressione di tutti gli altri tasti		
	return 0;
}

/*Questa funzione modifica la direzione del serpente in base all'input. La direzione è memorizzata in un char secondo la codifica UDLR(Up, Down, Left, Right). E' importante notare che il serpente non può fare inversioni di 180 gradi, quindi se per esempio la direzione è su l'utente preme S, il comando viene ignorato*/
char update_dir(char dir, char input) {
	//WASD
	if (((input == 'w')||(input == 'W'))&&(dir!='D'))
		return 'U';
	if (((input == 'a')||(input == 'A'))&&(dir!='R'))
		return 'L';
	if (((input == 's')||(input == 'S'))&&(dir!='U'))
		return 'D';
	if (((input == 'd')||(input == 'D'))&&(dir!='L'))
		return 'R';
	return dir; //Se la direzione non cambia
}
