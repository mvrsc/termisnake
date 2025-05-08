/*
Questo è il file header del progetto, qui vengono elencate tutte le funzioni che possono essere chiamate da un file diverso da quello in cui sono definite. Definiamo qui anche tutte le strutture usate dal progetto.
*/

/*
Sinceramente non ho capito a cosa servono queste direttive ma un tutorial mi ha detto che gli header file si fanno così e tutto sembra funzionare.
Il tutorial in questione: https://www.youtube.com/watch?v=NeOTr0u7ALk */
#ifndef LIB_H 
#define LIB_H

//Costanti da definire per il funzionamento del gioco:
#define WIDTH 32 //larghezza dello spazio di gioco
#define HEIGHT 16 //altezza dello spazio di gioco
#define SLEEPTIME 180000 //quanti microsecondi fra un fotogramma e l'altro
#define INIT_LEN 5 //lughezza iniziale del serpente

//struttura nodo per una lista
typedef struct node{
	int x;
	int y;
	struct node* next;
} node;

//Questa struttura contiene tutte le informazioni pertinenti alla logica del gioco
typedef struct Snake{
	node* head; //puntatore alla lista contenente tutti i pezzi del serpente
	char dir; //direzione verso cui guarda il serpente
	char state; // 'A': inizio 'B': in gioco 'C': game over
	int score; //punteggio
	node food;
/*Nota bene: food non è un puntatore ad un node ma proprio un tipo node. Si sarebbe potuto rendere un puntatore senza grossi cambiamenti. In entrambi i casi il campo "next" di food rimane sempre null il che è un leggero spreco di memoria ma è il modo che mi è risultato più comodo per scrivere il codice. */	
} Snake;


//Funzioni chiamate dal main che gestiscono il "rendering" del gioco
void init_map(char[HEIGHT][WIDTH]);
void update_map(char[HEIGHT][WIDTH], Snake*);
void draw(char[HEIGHT][WIDTH], Snake*);

//Funzioni chiamate dal main che gestiscono l'input
void set_input_mode (void);
char kb_read();
char update_dir(char, char);

//Funzioni che modificano la struttura Snake
Snake* init_snake();
void move_snake(Snake*);
void check_snake(Snake*);

//Funzioni che gestiscono le liste
node* append(node*, int, int); //aggiunta in coda(unico modo di modificare la lista)
node* prepend(node*, int, int); //aggiunta in testa solo per move_snake_old

//usate per il debug
void print_debug(Snake*, char);
void printlist(node*, int);

#endif
