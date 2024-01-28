#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_STACK_SIZE 100

//TODO : Define a structure for the stack
typedef struct {
    int top;
    double items[MAX_STACK_SIZE];
} Stack;

//TODO : Function prototypes
double pop(Stack *);
double peek(Stack *);
int isFull(Stack *);
int isEmpty(Stack *);
void push(Stack *, double);
void initializeStack(Stack *);

char *input(const char *);
void removeWhiteSpaces(char *);
double evaluateExpression(char *);

int main() {
    printf("Result: %lf\n", evaluateExpression(input("Enter an expression : ")));
    return 0;
}


/**
 * @brief Function to initialize the stack by setting the top index to -1
 * @param s: A Pointer to the stack
 **/
void initializeStack(Stack *s) {
    s->top = -1;
}

/**
 * @brief Function to check if the stack is empty
 * @brief Returns 1 if the stack is empty, 0 otherwise
 * @param s: A Pointer to the stack
 **/
 int isEmpty(Stack *s) {
    return (s->top == -1);
}

/**
 * @brief Function to check if the stack is full
 * @brief Returns 1 if the stack is full, 0 otherwise
 * @param s: A Pointer to the stack
 **/
int isFull(Stack *s) {
    return (s->top == MAX_STACK_SIZE - 1);
}

/**
 * @brief Function to push a value onto the stack
 * @brief Checks for stack overflow before pushing
 * @param s: A Pointer to the stack structure
 * @param value: Value to push to the stack
 **/
void push(Stack *s, double value) {
    if (isFull(s)) {
        printf("Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    s->items[++(s->top)] = value;
}

/**
 * @brief Function to pop a value from the stack
 * @brief Checks for stack underflow before popping
 * @param s: A Pointer to the stack structure
 **/
double pop(Stack *s) {
    if (isEmpty(s)) {
        printf("Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return s->items[(s->top)--];
}

/**
 * @brief Function to peek at the top value of the stack without removing it
 * @brief Checks if the stack is empty before peeking
 * @param s: A Pointer to the stack structure
 **/
double peek(Stack *s) {
    if (isEmpty(s)) {
        printf("Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return s->items[s->top];
}

/**
 * @brief Function to evaluate a given math expression
 * @param expression: A Pointer to the math expression
 **/
double evaluateExpression(char *expression) {
    removeWhiteSpaces(expression);
    Stack operandStack;
    Stack operatorStack;

    initializeStack(&operandStack);
    initializeStack(&operatorStack);

    for (int i = 0; expression[i] != '\0'; i++) {
        if (isspace(expression[i])) {
            continue; // Skip white spaces
        } else if (isdigit(expression[i])) {
            int operand = 0;
            while (isdigit(expression[i])) {
                operand = operand * 10 + (expression[i] - '0');
                i++;
            }
            i--; // Move back one position to correctly process the next character
            push(&operandStack, operand);
        } else if (expression[i] == '(') {
            push(&operatorStack, '(');
        } else if (expression[i] == ')') {
            while (peek(&operatorStack) != '(') {
                double operand2 = pop(&operandStack);
                double operand1 = pop(&operandStack);
                char operator = (char)pop(&operatorStack);

                if (operator == '+') {
                    push(&operandStack, operand1 + operand2);
                } else if (operator == '-') {
                    push(&operandStack, operand1 - operand2);
                } else if (operator == '*') {
                    push(&operandStack, operand1 * operand2);
                } else if (operator == '/') {
                    if (operand2 == 0) {
                        printf("Error: Division by zero\n");
                        exit(EXIT_FAILURE);
                    }
                    push(&operandStack, operand1 / operand2);
                }
            }
            pop(&operatorStack); // Pop the '(' from the stack
        } else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            while (!isEmpty(&operatorStack) && peek(&operatorStack) != '(' && ((expression[i] == '+' || expression[i] == '-') || (expression[i] == '*' || expression[i] == '/') && (peek(&operatorStack) == '*' || peek(&operatorStack) == '/'))) {
                double operand2 = pop(&operandStack);
                double operand1 = pop(&operandStack);
                char operator = (char)pop(&operatorStack);

                if (operator == '+') {
                    push(&operandStack, operand1 + operand2);
                } else if (operator == '-') {
                    push(&operandStack, operand1 - operand2);
                } else if (operator == '*') {
                    push(&operandStack, operand1 * operand2);
                } else if (operator == '/') {
                    if (operand2 == 0) {
                        printf("Error: Division by zero\n");
                        exit(EXIT_FAILURE);
                    }
                    push(&operandStack, operand1 / operand2);
                }
            }
            push(&operatorStack, expression[i]);
        } else {
            printf("Error: Invalid character in expression\n");
            exit(EXIT_FAILURE);
        }
    }

    while (!isEmpty(&operatorStack)) {
        double operand2 = pop(&operandStack);
        double operand1 = pop(&operandStack);
        char operator = (char)pop(&operatorStack);

        if (operator == '+') {
            push(&operandStack, operand1 + operand2);
        } else if (operator == '-') {
            push(&operandStack, operand1 - operand2);
        } else if (operator == '*') {
            push(&operandStack, operand1 * operand2);
        } else if (operator == '/') {
            if (operand2 == 0) {
                printf("Error: Division by zero\n");
                exit(EXIT_FAILURE);
            }
            push(&operandStack, operand1 / operand2);
        }
    }

    free(expression);
    return peek(&operandStack);
}

/**
 * @brief This function reads input from the user with an optional prompt.
 * @brief It dynamically allocates memory for the input string,
 * @brief handles reallocation if necessary, and removes any 'backspace' characters during input.
 *
 * @param prompt: - An optional prompt to display to the user.
 *
 * @returns A dynamically allocated string containing the user input.
 * @returns Returns NULL in case of memory allocation failure.
 *
 * @note The caller is responsible for freeing the memory allocated by this function.
 **/

char *input(const char *prompt) {
    if (prompt)
        fprintf(stdout, "%s", prompt);

    int c;
    size_t i = 0, size = 16;

    //* Allocate initial memory for the input string & handle allocation errors.
    char *arr = (char *)malloc(sizeof(char) * size);
    if (arr == NULL) {
        fprintf(stderr, "Error: Memory Allocation Failed!...\n");
        return NULL;
    }

    //* Read characters from 'stdin' until 'Enter' or 'EOF' is encountered
    while ((c = fgetc(stdin)) != '\n' && c != EOF) {
        //* Check if the allocated size is not sufficient, and reallocate if needed
        if ((i + 1) == size) {
            size *= 2;
            char *tmp = (char *)realloc(arr, sizeof(char) * size);

            //* Handle memory reallocation error
            if (tmp == NULL) {
                fprintf(stderr, "Error: Memory Reallocation Failed!...\n");
                free(arr);
                return NULL;
            }
            arr = tmp;
        }

        //* Handle 'backspace' character & assign to string
        if (c != '\b')
            arr[i++] = (char)c;
        else if (i > 0)
            i -= 1;
    }

    //* Null-terminate & return dynamically allocated string
    arr[i] = '\0';
    return arr;
}
void removeWhiteSpaces(char *str) {
    if (str == NULL)
        return;

    char *src = str;
    char *dest = str;

    //* Iterate through the string & copy non-whitespace characters to the original string
    while (*src) {
        if (!isspace(*src)) {
            *dest = *src;
            dest++;
        }
        src++;
    }

    // Null-terminate the modified string
    *dest = '\0';
}