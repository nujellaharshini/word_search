#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Declarations of the two functions I will implement
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void convertToUpper(char* word);
int bSize;

//global variables I added: 
int found_word = 0;
int** printPath;
int letterCounter = 0;
int reachTillEnd = 0;
int sizeOf_input_word = 0;

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle 
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));
    
    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

//function to print the puzzle/word search
void printPuzzle(char** arr) {
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            printf("%c ", *((*(arr + i)) + j));
        }
        printf("\n");
    }
}

//print function for the path 
void printThePath(int** arr) {
    // Print the array with adjusted spacing
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%-8d", *((*(arr + i)) + j)); //"%-8d" -> to adjust the spacing
        }
        printf("\n");
    }
}

//converting the word to uppercase
void convertToUpper(char* word){
    while (*word != '\0'){
        if (*word >= 'a' && *word <= 'z') {
            *word = *word - ('a' - 'A');
        }
        word++;
    }
}

//initial version of the path all 0's
void resetPrintPath(int** array){
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            *(*(array + i) + j) = 0; 
        }
    }
}

//updates the current location of the path matrix everyime a letter is found
void updatingPath(int letterCounter, int xPosition, int yPosition){
    //if statement for checking if a number is present at [x][y]
    if (*(*(printPath + xPosition) + yPosition) == 0) {
        *(*(printPath + xPosition) + yPosition) = letterCounter;
    } else {
        //if there is a number, add next to the current number
        *(*(printPath + xPosition) + yPosition) = (*(*(printPath + xPosition) + yPosition))*10 + letterCounter;
    }
}

//if incorrect path, reset current location to 0
void resetingLocation(int xPosition, int yPosition){
    *(*(printPath + xPosition) + yPosition) = 0;
}

//function to help find the next letter 
void find_next(int i_current, int j_current, char* word, char** puzzle){
    // checking if inputs are valid
    if ((puzzle == NULL) || (*word == '\0')){
        return;
    }

    // checking all 8 directions of the current letter
    for (int m = i_current - 1; m <= i_current + 1; m++){
        for (int n = j_current - 1; n <= j_current + 1; n++){
            // checking to see if word is found, if so return
            if (found_word == 1){ 
                return; 
            }

            // checking bounds
            if (m >= 0 && m < bSize && n >= 0 && n < bSize) {
                //strlen(word) == sizeOf_input_word --> when backtracking 
                if ((strlen(word) == sizeOf_input_word) || !(m == i_current && n == j_current)){
                    
                    if (*(*(puzzle + m) + n) == *word){ // if end of word
                        
                        if (*(word + 1) == '\0'){
                            found_word = 1;
                            letterCounter = sizeOf_input_word; //start the letterCounter at word size
                            updatingPath(letterCounter, m, n);
                            return; // Stop searching if the word is fully found
                        
                        } else {
                            // Recursive call starts from current position
                            find_next(m, n, word + 1, puzzle); 
                            if (found_word == 1){
                                letterCounter--;    //when found, decrement count and add
                                updatingPath(letterCounter, m, n);
                            }
                        }
                    }
                }
            }

            //reached till the end but word not found
            if (reachTillEnd == 1){ 
                // this means there are still letters in the word to search for  
                if (strlen(word) != sizeOf_input_word - 1){
                    //resets current location and continues search
                    resetingLocation(i_current, j_current);
                    continue;
                } else {
                    reachTillEnd = 0;
                }
            } 
        } 

        if (found_word == 1){
            return;
        }
    }

    if (found_word != 1) {
        reachTillEnd = 1;
        resetingLocation(i_current, j_current);        
    }

}

void searchPuzzle(char** arr, char* word){
    // checking if input word is a valid input
    if (*word == '\n'){
        return;
    }

    printf("\n"); //spacing
    convertToUpper(word);
    sizeOf_input_word = strlen(word);


    //allocate space for print path
    printPath = (int**)malloc(bSize * sizeof(int*));
    for(int i = 0; i < bSize; i++) {
        *(printPath + i) = (int*)malloc(bSize * sizeof(int));
    }
    resetPrintPath(printPath);

    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            if (found_word == 1){
                printf("Word found!\n");
                printf("Printing the search path:\n");
                printThePath(printPath);
                return;
            }
            find_next(i, j, word, arr);
            reachTillEnd = 0;
        }
    }
    printf("Word not found!");
}
