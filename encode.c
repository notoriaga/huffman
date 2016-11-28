#include "header.h"

FILE *input;
FILE *output;

int* getFreq(void);//Gets the frequency of the chars
struct node* makeList(int*);//Turns the frequencies into a linked list
struct node* inList(char, struct node*);//Checks to see if char is in the list
void swap(struct node*);//Swaps two nodes
struct node* toTree(struct node*);//Turns the linked list into a tree
void setBitString(struct node*, int,char*,char** );
void traverse(struct node*, struct bitwriter*);//Traverses the tree
void write_bit(int, struct bitwriter*);//Puts a bit into the buffer
void write(char**, struct bitwriter*);//Takes the file and gets the bitstrings

void write(char** bitStrings, struct bitwriter* writer){ 
    rewind(input);
     while(!feof(input)){
	int c = fgetc(input);
	char* str = bitStrings[(int)c];
	for(int i = 0; i < (int)strlen(str); i++){
	    write_bit(str[i]-'0',writer);
	}
    }
    for(int i = 0; i < CHAR_BIT-1; i++){
	write_bit(0,writer);
    }
}


void traverse(struct node* root,struct bitwriter* writer){
   if(!root->leftc && !root->rightc){
	write_bit(1,writer);
	int val = (int)root->c;
	for(int i = CHAR_BIT-1; i >= 0; i--){
	    int temp = (1 << i);
	    int new = val/temp;
	    val = val%temp;
	    write_bit(new,writer);
	}
   }else{
	write_bit(0,writer);
	traverse(root->leftc,writer);
	traverse(root->rightc,writer);
   }
}


void write_bit(int bit, struct bitwriter *writer){ 
    writer->buffer = (writer->buffer)*2 + bit;
    writer->count++;
    if(writer->count == CHAR_BIT){
	fputc(writer->buffer, output);
	writer->count = 0;
    }
}


int* getFreq(){
    int* freq = calloc(sizeof(int),256);
    if(freq == NULL){
	printf("Malloc failed");
    }
    
    freq[0] = 1;
    freq++;
    while(!feof(input)){
	int c = fgetc(input);
	if(c==EOF){
	    break;
	}
	freq[c]++;
    }
    return freq;
}

struct node* makeList(int* array){ 
    struct node* head = createNode(EOF,1);
    for(int i = 0; i < 255; i++){
      if(array[i]!=0){
	struct node* temp = createNode((char)i,array[i]); 
	temp->next = head;
	head->prev = temp;
	head = temp;
	while((temp->next) && (temp->frequency > temp->next->frequency)){
	    swap(temp);
	    if(head->c  == temp->c){
		head = temp->prev;
	    }
	}
      }
    }
    free(array-1);
    return head;
}

void swap(struct node* n){
    struct node* temp = n->next;
    int flag=0;
    int flag2=0;
    if(n->prev){
	temp->prev = n->prev;
	n->prev->next = temp;
    }else{
	flag=1;
    }
    if(temp->next){
	temp->next->prev = n;
	n->next = temp->next;
    }else{
	flag2=2;
    }
    temp->next = n;
    n->prev = temp;
    if(flag){
	temp->prev=NULL;
    }if(flag2){
	n->next = NULL;
    }
    
}

struct node* inList(char c, struct node* head){  
    struct node* temp = head;
    while(temp){
	if(c == temp->c){
	    return temp;
	}
	temp = temp->next;
    }
    return NULL;
}

struct node* toTree(struct node* head){
    while(head->next){
	struct node* temp = malloc(sizeof(struct node));
	if(temp == NULL){
	    printf("Malloc failed");
	    exit(-1);
	}
	if(temp){
	    temp->leftc = head;
	    temp->rightc = head->next;
	    temp->frequency = head->frequency + head->next->frequency;
	    temp->next = head->next->next;
	    temp->prev = NULL;
	    temp->leftc->prev=NULL;
	    temp->leftc->next=NULL;
	    temp->rightc->prev=NULL;
	    temp->rightc->next=NULL;
	    head = temp;
	    int check = 0;
	    while((temp->next) && (temp->frequency > temp->next->frequency)){
		if(!check){
		    head = head->next;
		    check++;
		}
		swap(temp);
	    }
	}
    }
    return head;
}

void setBitString(struct node* root, int depth, char* bits, char** ret){ 
    if(!root->leftc && !root->rightc){
	ret[(int)root->c]= malloc(sizeof(char)*10);
	if(!ret[(int)root->c]){
	    printf("Failed to malloc");
	    exit(EXIT_FAILURE);
	}
	strcpy(ret[(int)root->c], bits);
    }else{
	bits[depth] = '0';
	bits[depth+1] = '\0';
	setBitString(root->leftc, depth+1, bits, ret);
	bits[depth] = '1';
	bits[depth + 1] = '\0';
	setBitString(root->rightc, depth+1, bits, ret);
    }
}

int main(int argc, char* argv[]){
    input = stdin;
    output = stdout;
   
    if(argc > 1){
	input = fopen(argv[1],"r");
    }else{
	printf("Two few arguments");
	exit(-1);
    }
    if(argc > 2){
	output = fopen(argv[2],"w");
    } 
    if(argc > 3){
	printf("Too many arguements");
	exit(-1);
    }
 
    char* s = malloc(sizeof(char)*10);
    if(s == NULL){
	printf("Malloc failed");
	exit(-1);
    }
    
    char** bitStrings = calloc(256, sizeof(s));
    if(bitStrings == NULL){
	printf("Malloc failed");
	exit(-1);
    }

    int* array = getFreq();
    struct node *head = makeList(array);
    bitStrings++; 
    head = toTree(head);
    setBitString(head,0,s, bitStrings);
    
    struct bitwriter* writer = malloc(sizeof(struct bitwriter));
    if(writer == NULL){
	printf("Malloc failed");
	exit(-1);
    }

    writer->buffer = 0;
    writer->count = 0;
    traverse(head,writer);
    
    for(int i = 0; i < (int)strlen(bitStrings[EOF]); i++){
	write_bit(bitStrings[EOF][i] - '0',writer);
    }
    write(bitStrings,writer);
    for(int i = -1; i < 255; i++){
	free(bitStrings[i]);
    }
	
    free(writer);
    free(s);
    free(--bitStrings);
    freeTree(head);
    fclose(input);
    fclose(output);
}
