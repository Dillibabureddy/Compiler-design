#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int tempVar = 1;

char* newTemp() {
    static char temp[5];
    sprintf(temp, "t%d", tempVar++);
    return temp;
}

// Stack for operands (pointers to strings)
char* stack[100];
int top = -1;

void push(char* val) {
    stack[++top] = strdup(val);  // allocate and copy
}

char* pop() {
    return stack[top--];
}

void generateCodePrefix(char expr[]) {
    int len = strlen(expr);

    printf("Intermediate Code (Prefix):\n");

    for (int i = len - 1; i >= 0; i--) {
        char ch = expr[i];

        if (isalnum(ch)) {
            char op[2] = {ch, '\0'};
            push(op);
        } else {   
            char* op1 = pop();
            char* op2 = pop();
            char* temp = newTemp();
            printf("%s = %s %c %s\n", temp, op1, ch, op2);
            push(temp);
        }
    }
}

void generateCodePostfix(char expr[]) {
    int len = strlen(expr);

    printf("Intermediate Code (Postfix):\n");

    for (int i = 0; i < len; i++) {
        char ch = expr[i];

        if (isalnum(ch)) {
            char op[2] = {ch, '\0'};
            push(op);
        } else {
            char* op2 = pop();
            char* op1 = pop();
            char* temp = newTemp();
            printf("%s = %s %c %s\n", temp, op1, ch, op2);
            push(temp);
        }
    }
}

int main() {
    char prefixExpr[100], postfixExpr[100];

    printf("Enter prefix expression: ");
    scanf("%s", prefixExpr);
    generateCodePrefix(prefixExpr);

    // Reset stack
    top = -1;
    tempVar = 1;

    printf("\nEnter postfix expression: ");
    scanf("%s", postfixExpr);
    generateCodePostfix(postfixExpr);

    return 0;
}
