#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<limits.h>
#define NO_OF_CHARS 256

struct stack {
    int top;
    int capacity;
    char *array;
};

struct stack * createStack(unsigned);
int isEmpty(struct stack *);
char peek(struct stack *);
char pop(struct stack *);
void push(struct stack *, int);
int precedence(char);
char variable(char *);
char * parsing(char *);
int isOperand(char);
char * infixToPostfix(char *);
char * createvariablearray();
void calculate(char *, char *, char);
void printstack(struct stack *);
void printstack2(struct stack *);

//stack basic functions
struct stack * createStack(unsigned capacity) {
    struct stack *stack = (struct stack *) malloc(sizeof(struct stack));

    if(!stack)
        return NULL;

    stack->top = -1;
    stack->capacity = capacity;
    stack->array = (char *) malloc(sizeof(char) * stack->capacity);

    if(!stack->array)
        return NULL;
    return stack;
}

int isEmpty(struct stack *stack) {
    return stack->top == -1;
}

char peek(struct stack *stack) {
    return stack->array[stack->top];
}

char pop(struct stack *stack) {
    if(!isEmpty(stack))
        return stack->array[stack->top--];
    return '$';
}

void push(struct stack *stack, int item) {
    stack->array[++stack->top] = item;
}
//--------------------------------------------------
int precedence(char ch) {
    switch(ch) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
    }
    return -1;
}

char variable(char *stm) {
    char var;
    var = stm[0];
    return var;
}

char * parsing(char *stm) {
    int start_stm = 4, len = 0, i, j;

    for(i = 4; stm[i]; i++)
        len++;

    char *exp = (char *) malloc(sizeof(char) * (len + 1));

    for(i = 0, j = 4; i < len; i++, j++)
        exp[i] = stm[j];

    exp[len] = '\0';
    return exp;
}

int isOperand(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

char * infixToPostfix(char *exp) {
	int i, j, k;

    struct stack *stack = createStack(strlen(exp));
    printf("\nINFIX-POSTFIX\n--------------------------------\n");
    //new array
    char *postfix = (char *) malloc(sizeof(char) * strlen(exp));

    for(i = 0, j = 0; exp[i]; i++) {
        if(isOperand(exp[i])) {
            postfix[j++] = exp[i];
            postfix[j++] = ' ';
        }

        else if(isdigit(exp[i])) {
            postfix[j++] = exp[i];
            if(exp[i + 1] == ' ')
                postfix[j++] = ' ';
        }

        else if(exp[i] == '(')
            push(stack, exp[i]);

        else if(exp[i] == ')') {
            while(!isEmpty(stack) && peek(stack) != '(') {
                postfix[j++] = pop(stack);
                postfix[j++] = ' ';
            }

            pop(stack);
        }

        else if(exp[i] != ' ') {
            if(isEmpty(stack))
                push(stack, exp[i]);
            else if(precedence(exp[i]) > precedence(peek(stack)))
                push(stack, exp[i]);
            else {
                while(!isEmpty(stack) && precedence(exp[i]) <= precedence(peek(stack))) {
                    postfix[j++] = pop(stack);
                    postfix[j++] = ' ';
                }

                push(stack, exp[i]);
            }
        }
        if(exp[i] != ' ' && exp[i] != ';') {
           printf("Postfix:");
            for(k = 0; k < j; k++)
                printf("%c", postfix[k]);
            printf("\tStack:");
            printstack2(stack);
            printf("\n");
        }

    }

    while(!isEmpty(stack)) {
        postfix[j++] = pop(stack);
        if(postfix[j - 1] != ';') {
            printf("Postfix:");
            for(k = 0; k < j; k++)
                printf("%c", postfix[k]);
            printf("\tStack:");
            printstack2(stack);
            printf("\n");
        }
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
    printf("Postfix: ");
    for(i = 0; postfix[i] != ';'; i++)
        if(postfix[i] != '(' && postfix[i] != ' ')
            printf("%c", postfix[i]);
    return postfix;
}

char * createvariablearray() {
    char *arr = (char *) malloc(sizeof(char) * NO_OF_CHARS);
    int i;
    for(i = 0; i < NO_OF_CHARS; i++)
        arr[i] = 0;
    return arr;
}


void calculate(char *postfix, char *varvalues, char var) {
    printf("\nSOLVING POSTFIX\n--------------------------------");
    int i, j;
    printf("\nFor:");
    for(i = 0; postfix[i] != ';'; i++)
        if(postfix[i] != '(' && postfix[i] != ' ') {
                printf("%c", postfix[i]);

           }


    printf("\t");
    for(i = 97; i <= 122; i++)
        for(j = 0; postfix[j]; j++)
            if(varvalues[i] != 0 && postfix[j] == (char) i) {
                printf(" %c<-%d", i, varvalues[i]);
            }
    printf("\n");
    struct stack *stack = createStack(strlen(postfix));
    i = 0;
    while(postfix[i] != ';') {
        if(isdigit(postfix[i])) {
            int len = 0, temp = 0, count = 1;
            j = i;
            while(postfix[j++] != ' ');
            len = j - i;
            j -= 2;
            while(j >= i) {
                temp += (postfix[j--] - '0') * count;
                count *= 10;
            }
            push(stack, temp + '0');
            i += len - 2;
        }


        else if(isOperand(postfix[i])) {
            int varvalue = varvalues[postfix[i]];
            push(stack, varvalue + '0');
        }


        else if(postfix[i] != ' ') {
            char operand = postfix[i];
            int x, y, res;
            y = pop(stack) - '0';
            x = pop(stack) - '0';
            if(operand == '+')
                res = x + y;
            else if(operand == '-')
                res = x - y;
            else if(operand == '*')
                res = x * y;
            else if(operand == '/')
                res = x / y;
            push(stack, res + '0');
        }

        if(postfix[i] != ' ') {
            printf("Stack: ");
            printstack(stack);
            printf("\n");
        }
        i++;
    }
    int val = pop(stack) - '0';
    varvalues[var] = val;
    printf("--------------------------------");
}

void printstack(struct stack *stack) {
    if(isEmpty(stack))
        printf("Null");
    else {
        int i;
        for(i = 0; i <= stack->top; i++)
            printf("%d ", stack->array[i] - '0');
    }
}

void printstack2(struct stack *stack) {
 if(isEmpty(stack))
        printf("Null");
    else {
        int i;
        for(i = 0; i <= stack->top; i++)
            printf("%c ", stack->array[i]);
    }
}

int main() {
    FILE *fp;
    char source[100] = "input.txt";
    printf("Source :%s\n\n", source);
    fp = fopen(source, "r");
    if(fp == NULL) {
        printf("Error opening the file\n\n");
        exit(1);
    }
    char *varvalues = createvariablearray();
    char line[200], *infix, *postfix, temp[2000];
    temp[0] = '\0';
    int i;
    while(!feof(fp)) {
        i = 0;
        while((line[i++] = fgetc(fp)) != ';');
        line[i++] = '\0';
        printf("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-\n\nFor Equation %s\n", line);
        infix = parsing(line);
        postfix = infixToPostfix(infix);
        printf("\n--------------------------\n\n");
        calculate(postfix, varvalues, line[0]);

        printf("\n\n/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-\n\n\n");
        printf("*******************************************************************************************************\n\n\n");
        fgetc(fp);
    }
    fclose(fp);
    for(i = 97; i <= 122; i++) {
        if(varvalues[i] != 0)
            printf("%c<-%d ", i, varvalues[i]);
    }
    printf("\n\n");
}
