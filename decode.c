#include "header.h"

FILE *input;
FILE *output;

struct node* readFile(struct bitreader*);//Reads in the file
int getBit(struct bitreader*);//Gets one bit
void eof(struct node*, struct bitreader*);//Hard codes the EOF
char getChar(struct node*, struct bitreader*);//Decodes one bit

struct node* readFile(struct bitreader* reader){ 
    int bit = getBit(reader);
    if(bit){
	int x = 0;
	for(int i = 0; i < CHAR_BIT; i++){
	    x = x*2;
	    x += getBit(reader);
	}
	if(x == 255){
	    x = EOF;
	}
	x = (char)x;
	struct node* new = createNode(x,1);
	return new;
    }
    else{
	struct node* node = malloc(sizeof(struct node));
	if(node == NULL){
	    printf("Malloc failed");
	    exit(-1);
	}
	node->leftc = readFile(reader);
	node->rightc = readFile(reader);
	return node;
    }
}
 
void eof(struct node* root, struct bitreader* reader){
    if(!root->leftc && !root->rightc){
	root->c = EOF;
    }else{
	int bit = getBit(reader);
	if(bit){
	    eof(root->rightc, reader);
	}else{
	    eof(root->leftc, reader);
	}
    }
}
int getBit(struct bitreader* reader){
    if(reader->count<0){
	reader->c = fgetc(input);
	unsigned int x = reader->c;
	reader->bits = x;
	reader->bits = reader->bits%(1 << CHAR_BIT);
	reader->count = CHAR_BIT-1;
    }
    int x = reader->bits/(1 << reader->count);
    reader->bits = reader->bits%(1 << reader->count);
    reader->count--;
    return x;
}

char getChar(struct node* root, struct bitreader* reader){
    if(!root->leftc && !root->rightc){
	return root->c;
    }else{
	int bit = getBit(reader);
	if(bit){
	    return getChar(root->rightc,reader);
	}else{
	    return getChar(root->leftc,reader);
	}
    }
}

int main(int argc, char* argv[]){
    input = stdin;
    output = stdout;
   
    if(argc > 1){
	input = fopen(argv[1],"r");
    }else{
	printf("Not enough arguements");
	exit(-1);
    }
    if(argc > 2){
	output = fopen(argv[2],"w");
    }
    if(argc > 3){
	printf("Too many arguements");
	exit(-1);
    }

    struct bitreader* reader = malloc(sizeof(struct bitreader));
    if(reader == NULL){
	printf("Malloc failed");
	exit(-1);
    }
    reader->count = 0;
   
    getBit(reader);
    struct node* root = readFile(reader);
    eof(root,reader);		  
    char c = getChar(root, reader);     
    while(c!=EOF){			 
	fputc(c,output);
	c = getChar(root,reader);
    }

    freeTree(root);
    free(reader);
    fclose(input);
    fclose(output);
}
