#include <stdio.h>	
#include <stdlib.h>	//header for memory management
#include <string.h>	//header for character management
#include <time.h>
#include <ctype.h>	//header for character operations


char **matrix;	//global variable
char **w;	//global variable
int *guessed;	//global variable
int SIZE,MAX;	//global variable


char random_letter() {	//function that return a random letter
	
	char random_number;	//variable definition
	
	random_number='A' + rand() % 26;	//needed equation from A to Z
	
    return random_number;
}


void create_matrix() {	//function that creation of dynamic matrix
	
	int t,j;	//variable definition
	
    matrix = (char **)malloc(SIZE * sizeof(char *));	//memory allocation for row part
    
    for (t=0; t<SIZE; t++) {
        matrix[t] = (char *)malloc(SIZE * sizeof(char));	//memory allocation for column part that every row
        for (j=0; j<SIZE; j++) {
            matrix[t][j] = random_letter();	//put letters randomly to matrix
        }
    }
}


void dynamic_put() {	//function that put the words dynamically
	
	int t,j;	//variable definition
	int direct, row, column;	//variable definition
	
	
    for (t = 0; t < MAX; t++) {
        
		direct = rand() % 3; // 0: horizontal, 1: vertical, 2: cross
		
        do {
            row = rand() % SIZE;	//select a random row
            column = rand() % SIZE;	//select a random row
        } while ((direct == 0 && column + strlen(w[t]) > SIZE) ||	//limitation control for horizontal side
                 (direct == 1 && row + strlen(w[t]) > SIZE) ||	//limitation control for vertical side
                 (direct == 2 && (row + strlen(w[t]) > SIZE || column + strlen(w[t]) > SIZE)));	//limitation control for cross side

        for (j = 0; j < strlen(w[t]); j++) {
            if (direct == 0)      // horizontal
                matrix[row][column + j] = toupper(w[t][j]);	//change to upper letter
            else if (direct == 1) // vertical
                matrix[row + j][column] = toupper(w[t][j]);	//change to upper letter
            else if (direct == 2) // cross
                matrix[row + j][column + j] = toupper(w[t][j]);	//change to upper letter
        }
    }
}


void matrix_screen() {	//function that display the matrix
	
	int t,j;	//variable definition
	
    printf("\n");
    for (t = 0; t < SIZE; t++) {
        for (j = 0; j < SIZE; j++) {
            printf("%c ", matrix[t][j]);	//print the terminal screen that character
        }
        printf("\n");
    }
}


int check_w(char *guess) {	//check mechanism for user's guess
	
	int t;	//variable definition
	
    for (t = 0; t < MAX; t++) {
        if (!guessed[t] && strcasecmp(guess, w[t]) == 0) {	//checking the match guess and the word
            guessed[t] = 1;
            return 1; //correct
        }
    }
    return 0; //wrong
}


int nonfounded_w() {	//function that counting the non-founding words
	
    int cnt = 0,t;	//variable definition
    
    for (t = 0; t < MAX; t++) {
        if (!guessed[t])	//non-founding yet
            cnt++;
    }
    return cnt;
}


void play() {	//The main playing game function
	
    char guess[100];	//variable definition
    int total = 0;	//variable definition

    while (nonfounded_w() != 0) {	//loop until find every words 
        matrix_screen();
        printf("\nNumber of unguessed words: %d\n", nonfounded_w());
        printf("Enter the word that you want to find: ");
        scanf("%s", guess);	//take a guess

        if (check_w(guess)==1) {	//if it's true
            printf("\nCORRECT! GET A POINT:)\n");
            total = total+10;	//increase point
        } else {	//or
            printf("\nWRONG! LOSE A POINT:(\n");
            total = total-5;	//decrease point
        }
    }

    printf("\nCONGRATS! ALL WORDS HAVE BEEN FOUNDED. TOTAL POINT IS %d\n", total);
}


void freeMemory() {	//function that unleash the memory
	
	int t;	//variable definition
	
    for (t = 0; t < SIZE; t++) {
        free(matrix[t]);	//make free that a row 
    }
    free(matrix);	//make free that main memory

    for (t = 0; t < MAX; t++) {	//make free that memory for wordlists
        free(w[t]);	
    }
    free(w);	//make free that main memory for wordlist
    free(guessed);	//make free that memory for founded words	
}


	main() {	//main functions
    srand(time(0)); //time dependent randomise

	int t,w_size;	//variable definition

    printf("Enter the size of matrix: ");
    scanf("%d", &SIZE);	//take a size of matrix

    printf("Enter the number of words: ");
    scanf("%d", &MAX);	//take a number of words

    
    w = (char **)malloc(MAX * sizeof(char *));	//memory allocation for wordlist
    guessed = (int *)calloc(MAX, sizeof(int));	//memory allocation for founded lists and make full with zero's
    for (t = 0; t < MAX; t++) {
        
        printf("Enter the size of number %d: ", t + 1);
        scanf("%d", &w_size);	//take size of number

        w[t] = (char *)malloc((w_size + 1) * sizeof(char));	//memory allocation for every word
        printf("Enter the number %d: ", t + 1);
        scanf("%s", w[t]);	//take a word
    }

    
    create_matrix();	//generate the matrix
    dynamic_put();	//put the words into the matrix
    play();	//start the game

    
    freeMemory();	//for avoiding memory leakage, clear memory allocation
}
