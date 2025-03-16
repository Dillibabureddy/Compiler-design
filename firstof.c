#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20
#define ALPHABET_SIZE 26

// Data structure to store grammar rules
char grammar[MAX][MAX];
char first[MAX][MAX]; // First set for each non-terminal
int n, m; // n -> number of non-terminals, m -> number of productions

// Function to check if a character is a terminal
int isTerminal(char c) {
    return !(c >= 'A' && c <= 'Z');
}

// Function to compute the FIRST set of a non-terminal
void computeFirst(char nonTerminal) {
    int i, j, k;
    for (i = 0; i < m; i++) {
        if (grammar[i][0] == nonTerminal) {
            // Process the production
            for (j = 3; grammar[i][j] != '\0'; j++) {
                if (isTerminal(grammar[i][j])) {
                    // If the character is terminal, add it to the first set
                    if (strchr(first[nonTerminal - 'A'], grammar[i][j]) == NULL) {
                        strncat(first[nonTerminal - 'A'], &grammar[i][j], 1);
                    }
                    break; // Stop after first terminal symbol
                } else {
                    // If it's a non-terminal, call computeFirst recursively
                    computeFirst(grammar[i][j]);
                    for (k = 0; first[grammar[i][j] - 'A'][k] != '\0'; k++) {
                        if (strchr(first[nonTerminal - 'A'], first[grammar[i][j] - 'A'][k]) == NULL) {
                            strncat(first[nonTerminal - 'A'], &first[grammar[i][j] - 'A'][k], 1);
                        }
                    }
                    // If the first non-terminal can lead to epsilon, continue to the next
                    if (strchr(first[grammar[i][j] - 'A'], '#') == NULL) {
                        break;
                    }
                }
            }
        }
    }
}

// Main function
int main() {
    int i, j;
    printf("Enter the number of productions: ");
    scanf("%d", &m);
    
    printf("Enter the productions in the form A->B or A->BC or A->ε\n");
    for (i = 0; i < m; i++) {
        printf("Enter production %d: ", i + 1);
        scanf("%s", grammar[i]);
    }
    
    // Initialize the first sets to be empty
    for (i = 0; i < MAX; i++) {
        first[i][0] = '\0';
    }
    
    // Compute the FIRST sets for each non-terminal
    for (i = 0; i < m; i++) {
        if (strchr(first[grammar[i][0] - 'A'], grammar[i][0]) == NULL) {
            computeFirst(grammar[i][0]);
        }
    }
    
    // Print the FIRST sets
    printf("\nFIRST sets:\n");
    for (i = 0; i < MAX; i++) {
        if (first[i][0] != '\0') {
            printf("FIRST(%c) = {", i + 'A');
            for (j = 0; first[i][j] != '\0'; j++) {
                if (first[i][j] == '#') {
                    printf("#");
                } else {
                    printf("%c", first[i][j]);
                }
                if (first[i][j + 1] != '\0') {
                    printf(", ");
                }
            }
            printf("}\n");
        }
    }

    return 0;
}
