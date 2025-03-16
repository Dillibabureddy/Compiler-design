#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Function to check if the parentheses are balanced
int checkBalance(const char* expression) {
    int balance = 0; // Counter to keep track of parentheses balance
    int i = 0;

    while (expression[i] != '\0') {
        if (expression[i] == '(') {
            balance++; // Opening parenthesis, increment balance
        } else if (expression[i] == ')') {
            balance--; // Closing parenthesis, decrement balance
            if (balance < 0) {
                printf("More closing parentheses than opening parentheses.\n");
                return 0; // Unbalanced
            }
        }
        i++;
    }

    // If balance is zero, parentheses are balanced
    return (balance == 0);
}

// Function to evaluate a simple expression
int evaluateExpression(const char* expression) {
    int numbers[100], top_num = -1;   // Stack for numbers
    char operators[100], top_op = -1; // Stack for operators
    int i = 0;

    // Helper function for operator precedence
    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    // Perform calculation
    int calculate(int a, int b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/':
                if (b == 0) {
                    printf("Error: Division by zero.\n");
                    exit(1); // Exit if division by zero occurs
                }
                return a / b;
        }
        return 0;
    }

    while (expression[i] != '\0') {
        if (isdigit(expression[i])) {
            // Extract full number (multi-digit support)
            int num = 0;
            while (isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            numbers[++top_num] = num;
        } else if (expression[i] == '(') {
            operators[++top_op] = expression[i];
            i++;
        } else if (expression[i] == ')') {
            while (top_op >= 0 && operators[top_op] != '(') {
                int b = numbers[top_num--];
                int a = numbers[top_num--];
                char op = operators[top_op--];
                numbers[++top_num] = calculate(a, b, op);
            }
            top_op--; // Remove '('
            i++;
        } else {
            // Process operators
            while (top_op >= 0 && precedence(operators[top_op]) >= precedence(expression[i])) {
                int b = numbers[top_num--];
                int a = numbers[top_num--];
                char op = operators[top_op--];
                numbers[++top_num] = calculate(a, b, op);
            }
            operators[++top_op] = expression[i];
            i++;
        }
    }

    // Process remaining operators
    while (top_op >= 0) {
        int b = numbers[top_num--];
        int a = numbers[top_num--];
        char op = operators[top_op--];
        numbers[++top_num] = calculate(a, b, op);
    }

    return numbers[top_num];
}

int main() {
    char expression[100];
    printf("Enter an expression: ");
    scanf("%s", expression);

    // Check if the parentheses are balanced
    if (!checkBalance(expression)) {
        printf("Error: Unbalanced parentheses\n");
        return 1;  // Exit the program with an error code
    }

    // If balanced, evaluate the expression
    int result = evaluateExpression(expression);
    printf("Result: %d\n", result);
    return 0;
}
