/*
Questo è un piccolo progetto che ho sviluppato come esercizio per me stesso. Mi sono preso il tempo di commentare dettagliatamente tutto il codice nel caso possa risultare utile a qualcuno. Spero che lo sia.
-MV
*/

#include "lib.h" //per usare l'header file del progetto
#include <stdio.h>
#include <unistd.h> //per usare usleep()
#include <stdlib.h>

int main() {
	set_input_mode(); //Impostiamo l'input non canonico

	//Visualizziamo la schermata iniziale
	char screen[HEIGHT][WIDTH];
	Snake* snake = init_snake();	
	init_map   (screen);
	update_map (screen, snake);
	draw       (screen, snake);	
	printf("Giochiamo a Snake! \nTi puoi muovere con WASD o le frecce direzionali\n");
	printf("Per uscire premi INVIO\nMuoviti per iniziare a giocare\n");	

	
	//Restiamo sulla schermata iniziale (snake->state='A') finchè il giocatore non si muove.
	char input;
	while (snake->state=='A') {
		input = kb_read();
		if (input) {
			snake->dir = update_dir(snake->dir, input);	
			snake->state='B';
		}	
	}


	//Loop che genera ogni fotogramma
	while (snake->state == 'B') {
			
		input = kb_read(); //leggi l'input da tastiera
		
		//Se l'utente preme invio, esci dal gioco
		if (input=='\n')
			snake->state = 'C';
		
		//prima cambia la direzione del serpente, poi muovilo, infine controlla se ha toccato se stesso o il cibo	
		snake->dir = update_dir(snake->dir, input); 
		move_snake(snake);
		check_snake(snake);

		//aggiorna la matrice che rappresenta lo schermo  e poi disegnala			
		update_map (screen, snake);
		draw       (screen, snake);
//		print_debug(snake, input); //stampa informazioni di debug	
		
		usleep(SLEEPTIME); //aspetta prima di generare il prossimo fotogramma

	} 
	//Usciamo dal programma
	printf("GAME OVER\n");
	exit(EXIT_SUCCESS); //Qui viene chiamata reset_input_mode()
}



