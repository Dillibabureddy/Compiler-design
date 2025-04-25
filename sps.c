#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables
int z = 0, i = 0, j = 0, c = 0;

// Modify array size to increase length of string to be parsed
char a[16], ac[20], stk[15], act[10]; 

// This function checks whether the stack contains a production rule to be reduced
void check()
{
    // Copying string to be printed as action
    strcpy(ac, "REDUCE TO S -> ");  

    // Check for production S -> a
    for(z = 0; z < c; z++) 
    {
        if(stk[z] == 'a') // 'a' is used instead of 'id'
        {
            printf("%sa", ac);
            stk[z] = 'S';  // Replace 'a' with 'S'
            stk[z + 1] = '\0';
            printf("\n$%s\t%s$\t", stk, a); 
        }
    }

    // Check for production S -> S * S
    for(z = 0; z < c - 2; z++)
    {
        if(stk[z] == 'S' && stk[z + 1] == '*' && stk[z + 2] == 'S') 
        {
            printf("%sS*S", ac);
            stk[z] = 'S';  // Replace 'S * S' with 'S'
            stk[z + 1] = '\0';
            stk[z + 2] = '\0';
            printf("\n$%s\t%s$\t", stk, a);
            i = i - 2;
        }
    }

    // Check for production S -> S + S
    for(z = 0; z < c - 2; z++)
    {
        if(stk[z] == 'S' && stk[z + 1] == '+' && stk[z + 2] == 'S') 
        {
            printf("%sS+S", ac);
            stk[z] = 'S';  // Replace 'S + S' with 'S'
            stk[z + 1] = '\0';
            stk[z + 2] = '\0';
            printf("\n$%s\t%s$\t", stk, a);
            i = i - 2;
        }
    }

    return;
}

// Driver function
int main()
{
    // User input
    printf("Enter the expression (using a, +, and *): ");
    fgets(a, sizeof(a), stdin);
    a[strcspn(a, "\n")] = 0;  // Remove newline character from input

    // Print the grammar for the user
    printf("\nGRAMMAR is -\nS -> S + S \nS -> S * S \nS -> a\n");    

    // Calculate the length of input string
    c = strlen(a); 

    // "SHIFT" is copied to act to be printed
    strcpy(act, "SHIFT"); 

    // Print the column headers
    printf("\nstack \t input \t action"); 
    
    // Print the initial values of stack and input
    printf("\n$\t%s$\t", a); 

    // Process the input string one symbol at a time
    for(i = 0; j < c; i++, j++) 
    {
        // Print the SHIFT action
        printf("%s", act); 

        // Push the current symbol onto the stack
        stk[i] = a[j];     
        stk[i + 1] = '\0';

        // Move the pointer in the input string
        a[j] = ' ';

        // Print the updated stack and remaining input
        printf("\n$%s\t%s$\t", stk, a); 

        // Call the check function to reduce the stack if possible
        check(); 
    }

    // Perform a final check for reductions after the entire input is shifted
    check(); 
    
    // If the top of the stack is 'S' (the starting symbol) and the stack is empty after reduction
    if(stk[0] == 'S' && stk[1] == '\0') 
        printf("Accept\n");  // Accepted
    else 
        printf("Reject\n");  // Rejected

    return 0;
}
