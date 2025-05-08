/*
Questo file contiene delle funzioni per gestire le liste dinamiche
*/

#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

//Creazione e inserimento in coda di una lista
node* append(node* head, int x, int y) {
	node* new = malloc(sizeof(node));
	new->x = x;
	new->y = y;
	new->next = NULL;
	
	if (head==NULL) {
		return new;	
	}
	
	node* curr = head;
	while(curr->next!=NULL) {
		curr = curr->next;
	}
	curr->next = new;
	return head;
}

//per il debug
void printlist(node* head, int lim) {
	node* curr = head;
	while ((curr!=NULL)&&(lim>0)) {
		printf("[%d, %d]-> ", curr->x, curr->y);
		curr = curr->next;
		lim--;	
	}
	printf("\n");
	return;
}

//inserimento in testa, usata solo in move_snake_old
node* prepend(node* head, int x, int y) {
	node* new = malloc(sizeof(node));
	new->x = x;
	new->y = y;
	new->next = head;
	return new;
}

