#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10  // Maximum number of non-terminals
#define SIZE 100 // Buffer size

char grammar[MAX][SIZE]; // Store grammar rules
char first[MAX][SIZE];   // FIRST sets
char follow[MAX][SIZE];  // FOLLOW sets
char nonTerminals[MAX];  // List of non-terminals
int numRules = 0;        // Number of production rules

// Function to check if a character is a non-terminal
int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Function to find FIRST set
void computeFirst(char c, char result[]) {
    if (!isNonTerminal(c)) {
        strncat(result, &c, 1); // Add terminal to FIRST
        return;
    }
    
    for (int i = 0; i < numRules; i++) {
        if (grammar[i][0] == c) {
            for (int j = 3; grammar[i][j] != '\0'; j++) {
                if (!isNonTerminal(grammar[i][j])) {
                    strncat(result, &grammar[i][j], 1);
                    break;
                } else {
                    char temp[SIZE] = "";
                    computeFirst(grammar[i][j], temp);
                    strcat(result, temp);
                    if (strchr(temp, '#') == NULL) // Stop if ε is not in FIRST
                        break;
                }
            }
        }
    }
}

// Function to compute FOLLOW set
void computeFollow(char c) {
    if (c == grammar[0][0]) // Start symbol
        strcat(follow[c - 'A'], "$");

    for (int i = 0; i < numRules; i++) {
        for (int j = 3; grammar[i][j] != '\0'; j++) {
            if (grammar[i][j] == c) {
                // Case 1: A → αBβ → Add FIRST(β) to FOLLOW(B)
                if (grammar[i][j + 1] != '\0') {
                    char temp[SIZE] = "";
                    computeFirst(grammar[i][j + 1], temp);
                    for (int k = 0; temp[k] != '\0'; k++) {
                        if (temp[k] != '#') {
                            strncat(follow[c - 'A'], &temp[k], 1);
                        }
                    }
                    
                    // If FIRST(β) has ε, add FOLLOW(A) to FOLLOW(B)
                    if (strchr(temp, '#') != NULL) {
                        strcat(follow[c - 'A'], follow[grammar[i][0] - 'A']);
                    }
                }
                // Case 2: A → αB → FOLLOW(A) to FOLLOW(B)
                else {
                    strcat(follow[c - 'A'], follow[grammar[i][0] - 'A']);
                }
            }
        }
    }
}

// Function to remove duplicates from FOLLOW set
void removeDuplicates(char *set) {
    char unique[SIZE] = "";
    for (int i = 0; set[i] != '\0'; i++) {
        if (!strchr(unique, set[i])) {
            strncat(unique, &set[i], 1);
        }
    }
    strcpy(set, unique);
}

int main() {
    int i, j;
    printf("Enter the number of productions: ");
    scanf("%d", &numRules);
    getchar(); // Consume newline

    printf("Enter the grammar (e.g., A->BC):\n");
    for (i = 0; i < numRules; i++) {
        fgets(grammar[i], SIZE, stdin);
        grammar[i][strcspn(grammar[i], "\n")] = '\0'; // Remove newline
        if (!strchr(nonTerminals, grammar[i][0])) {
            nonTerminals[strlen(nonTerminals)] = grammar[i][0];
        }
    }

    // Compute FIRST sets
    for (i = 0; i < strlen(nonTerminals); i++) {
        computeFirst(nonTerminals[i], first[nonTerminals[i] - 'A']);
    }

    // Compute FOLLOW sets
    for (i = 0; i < strlen(nonTerminals); i++) {
        computeFollow(nonTerminals[i]);
    }

    // Remove duplicates in FOLLOW sets
    for (i = 0; i < strlen(nonTerminals); i++) {
        removeDuplicates(follow[nonTerminals[i] - 'A']);
    }

    // Print FOLLOW sets
    printf("\nFOLLOW sets:\n");
    for (i = 0; i < strlen(nonTerminals); i++) {
        printf("FOLLOW(%c) = {", nonTerminals[i]);
        for (j = 0; follow[nonTerminals[i] - 'A'][j] != '\0'; j++) {
            printf("%c", follow[nonTerminals[i] - 'A'][j]);
            if (follow[nonTerminals[i] - 'A'][j + 1] != '\0') {
                printf(", ");
            }
        }
        printf("}\n");
    }

    return 0;
}
