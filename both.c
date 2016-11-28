#include "header.h"

struct node* createNode(char c, int f){
    struct node* temp = malloc(sizeof(struct node));
    if(temp == NULL){
	printf("Malloc failed");
	exit(-1);	
    } 
    temp->next = NULL;
    temp->prev = NULL;
    temp->leftc = NULL;
    temp->rightc = NULL;
    temp->c = c;
    temp->frequency = f;
    return temp;
}

void printTree(struct node* root){
    if(!root->leftc && !root->rightc){
	printf("%c%d\n", root->c,root->frequency);
    }else{
	printf("%d\n",root->frequency);
	printTree(root->leftc);
	printTree(root->rightc);
    }
}

void freeTree(struct node* root){
    if(!root->leftc && !root->rightc){
	free(root);
    }else{
	freeTree(root->leftc);
	freeTree(root->rightc);
	free(root);
    }
}

