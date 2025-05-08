/*
Questo file definisce le funzioni che modificano la struttura Snake e di conseguenza contiene tutte le istruzioni che controllano la logica del gioco.
*/

#include "lib.h" //per usare l'header file del progetto
#include <stdlib.h> //per rand(), srand(), malloc() e free()
#include <stdbool.h> //per usare i valori true e false
#include <time.h> //per dare un seed al valore casuale

//ritorna true se il cibo è comparso in un punto occupato dal serpente
bool food_blocked(Snake* snake) {
	node* curr = snake->head;
	while(curr!=NULL) {
		if ((curr->x==snake->food.x)&&(curr->y==snake->food.y)) {
			return true;
		}
		curr=curr->next;
	}
	return false;
}

//genera casualmente la posizione del cibo
void spawn_food(Snake* snake){
	do {
		snake->food.x = rand() % (WIDTH -2) + 1;
		snake->food.y = rand() % (HEIGHT-2) + 1;
	} while (food_blocked(snake));
	return;
}

//crea un nuovo serpente e imposta i valori iniziali
Snake* init_snake() {
	Snake* snake = malloc(sizeof(Snake));
	snake->dir = 'R'; //il serpente guarda verso destra inizialmente
	snake->score = 0; //il punteggio iniziale è zero
	snake->state = 'A'; //'A': inizio 'B': in gioco 'C': game over
	srand(time(NULL)); //imposto una volta sola il seed per rand()
	spawn_food(snake); //genero la posizione del cibo per la prima volta
	
	//Popolo la lista con i nodi iniziali
	snake->head = NULL;	
	for (int i = 0; i<INIT_LEN; i++) {
		snake->head = append(snake->head, WIDTH/2-i, HEIGHT/2); 
	}	
	return snake;	
}

//Muove il serpente avanti di un passo in base alla direzione
void move_snake(Snake* snake) {
/*Vogliamo che l'ultimo nodo abbia la posizione del penultimo nodo, il penultimo del terzultimo e così via. Attraversiamo tutta la lista copiando in ogni nodo la posizione del nodo che viene prima. Ci servono due coppie di variabili di supporto*/
	node* curr = snake->head;
	int new_x = snake->head->x;
	int new_y = snake->head->y;
	int old_x;
	int old_y;
	while (curr->next!=NULL) {
		old_x = curr->x;
		old_y = curr->y;
		curr->x = new_x;
		curr->y = new_y;
		new_x = old_x;
		new_y = old_y;
		curr = curr->next;
	}
	curr->x = new_x;
	curr->y = new_y;

/*a questo punto abbiamo spostato avanti di uno tutti i nodi del serpente, però il secondo e il primo nodo sono sovrapposti. A questo punto avanziamo il primo nodo (la testa) nella direzione indicata da dir*/	
	if (snake->dir=='L')
		snake->head->x--;
	if (snake->dir=='R')
		snake->head->x++;
		
	if (snake->dir=='U')
		snake->head->y--;
	if (snake->dir=='D')
		snake->head->y++;

	//se la testa è uscita dai bordi della mappa la facciamo tornare dalla parte opposta		
	if (snake->head->x<1)
		snake->head->x = WIDTH-2;
	if (snake->head->x>WIDTH-2)
		snake->head->x = 1; 
		
	if (snake->head->y<1)
		snake->head->y = HEIGHT-2;
	if (snake->head->y>HEIGHT-2)
		snake->head->y = 1;
	return;
}

//Ritorna true se il serpente ha morso se stesso
bool gameover_check(Snake* snake) {
	node* curr = snake->head->next;
	while (curr!=NULL) {
		if ((curr->x == snake->head->x)&&(curr->y == snake->head->y)) {
			return true;
		}
		curr = curr->next;
	}
	return false;
}

//Ritorna true se il serpente ha raggiunto il cibo.
bool score_check(Snake* snake) {
	if ((snake->head->x==snake->food.x)&&(snake->head->y==snake->food.y))
		return true;
	return false;
}

//Controlla ad ogni passo se il serpente ha morso se stesso o il cibo.
void check_snake(Snake* snake) {
	if (gameover_check(snake)) {
		snake->state = 'C'; //GAME OVER
		return;
	}
	if (score_check(snake)) {
	//Incremento il punteggio, allungo il serpente e genero il nuovo cibo
		snake->score++;
		snake->head = append(snake->head, snake->food.x, snake->food.y);
		spawn_food(snake);
	}
	return;
}

/*
Questa è una vecchia versione del movimento del serpente. Scriverla è stato un po' più semplice però richiedeva di fare una malloc ed una free ad ogni passo il che mi sembrava potenzialmente costoso. La mia ipotesi è che fare n copie di coppie di valori x,y rimanendo nello stack (forse nei registri?) sia più conveniente che allocare/deallocare memoria ad ogni passo. 
Mi sono anche accorto che c'è un modo ancora più efficiente di realizzare il movimento, usando una lista doppiamente concatenata e due puntatori nella struttura snake, uno alla testa e uno alla coda.
Credo che ci siano molti algoritmi per ottenere il movimento del serpente e pensarne di nuovi può essere un esercizio interesante.
*/
node* move_snake_old(node* head, char dir) {
	
	head = prepend(head, head->x, head->y);
	
	if (dir=='L')
		head->x--;
	if (dir=='R')
		head->x++;
		
	if (dir=='U')
		head->y--;
	if (dir=='D')
		head->y++;

		
	if (head->x<1)
		head->x = WIDTH-2;
	if (head->x>WIDTH-2)
		head->x = 1; 
		
	if (head->y<1)
		head->y = HEIGHT-2;
	if (head->y>HEIGHT-2)
		head->y = 1; 
	
	node* curr = head;
	while (curr->next->next!=NULL) {
		curr = curr->next;
	}
	
	free(curr->next); 	
	curr->next=NULL;
			
	return head;
}
