/*
Queste sono le funzioni che definiscono il modo in cui il gioco viene visualizzato da terminale. Sto rappresentando il mio "schermo" come una matrice di char. Le funzioni init_map() e update_map() modificano questa matrice mentre la funzione draw() si occupa di visualizzarla sul terminale.
*/

#include <stdio.h>
#include "lib.h"

/*Questa funzione i bordi della matrice con il valore '0', che verrà visualizzato come un blocco grigio chiaro*/
void init_map(char screen[HEIGHT][WIDTH]) {
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			if (y==0) {
				if (x==0) 
					screen[x][y]='0';
				else if (x==WIDTH-1)
					screen[x][y]='0';
				else 	
					screen[x][y]='0';			
			}
			else if (y==HEIGHT-1) {
				if (x==0) 
					screen[x][y]='0';
				else if (x==WIDTH-1)
					screen[x][y]='0';
				else
					screen[x][y]='0';		
			}
			else if ((x==0)||(x==WIDTH-1))
				screen[x][y]='0';
			else 
				screen[x][y]=' ';		
		}
	}
}

/*Aggiorna la matrice schermo con le nuove posizioni degli oggetti*/
void update_map(char screen[HEIGHT][WIDTH], Snake* snake) {

/*Per prima cosa svuoto tutto lo schermo all'interno dei bordi.*/	
	for (int y=1; y<HEIGHT-1; y++) {
		for (int x=1; x<WIDTH-1; x++) {
			screen[x][y]=' ';		
		}
	}
	
/*Una volta svuotato la matrice la ripopolo con le informazioni sul gioco prese dalla struttura snake. Attraverso la lista, poi specifico quale nodo è la testa e infine salvo la posizione del cibo.I caratteri che uso qui verranno poi tradotti dalla funzione draw()*/
	node* curr = snake->head;
	while(curr!=NULL) {
		screen[curr->x][curr->y] = 'x'; //pezzo della coda
		curr=curr->next;	
	}
	screen[snake->head->x][snake->head->y] = 'X'; //testa del serpente
	screen[snake->food.x][snake->food.y] = '*'; //cibo

}

/* Questa funzione chiama tutte le printf che visualizzano sul terminale lo schermo del gioco. Durante questo progetto mi sono accorto che mentre una variabile char può contenere solo caratteri ASCII la funzione printf gestisce senza problemi i caratteri Unicode. Questo mi ha permesso di rendere il gioco più visivamente piacevole.
La prima printf della funzione cancella il contenuto del terminale e riposiziona il cursore in alto a destra, pronto a ridisegnare. Può essere interessante cancellare questa riga per capirne meglio il funzionamento. 
Ci sono altre soluzioni pulire il terminale, ad esempio system("clear") o clrscr(). Questa ha il vantaggio di non essere legata al sistema operativo. 
Per capire come funziona la stringa: https://stackoverflow.com/questions/66927511/what-does-e-do-what-does-e11h-e2j-do */
void draw(char screen[HEIGHT][WIDTH], Snake* snake) {
	printf("\e[1;1H\e[2J"); 
	
	//dopo aver pulito il terminale stampiamo il contenuto della matrice
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			switch (screen[x][y]) {
				case '0':
					printf("▒"); //bordo dello schermo
					break;
				case '*': 
					printf("●"); //cibo
					break;	
				case 'x':
					printf("○"); //coda del serpente
					break;
				case 'X': { //testa del serpente
					//cambia in base alla direzione
					switch (snake->dir) {
						case 'U':
							printf("△");
							break;				
						case'L': 
							printf("◁");
							break;
						case'D':
							printf("▽");
							break;
						case'R':
							printf("▷");
							break;
						default: //non dovrebbe arrivare qua
							printf("?");
					}
					break;
				}
				default: //quando il carattere è uno spazio bianco
						printf("%c", screen[x][y]); 
			}
		}
		printf("\n");	//qui finisce una riga di caratteri	
	}
	printf("score: %i\n\n", snake->score);
}


//Visualizza qualche informazione sul gioco, per il debug
void print_debug(Snake* snake, char input) {
	printlist(snake->head, INIT_LEN);
	printf("INPUT: %c\n", input);
	printf("DIR: %c\n", snake->dir);

}
