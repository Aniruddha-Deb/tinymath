#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>

#define STACK_SIZE (1<<10)
#define FLOAT_IMPL
#define TRUE 1
#define FALSE 0

typedef enum {
	NUMERIC,
	OPERATOR,
	PARENTHESIS,
	COMMAND
} TokenType;

typedef struct {
	TokenType type;
	char *value;
} Token;

typedef struct {
	// A better implementation would have a dynamic array
	Token *array[STACK_SIZE];
	int head;
} Stack;

// Token utility functions
Token* create_token(TokenType type, char* value) {
	Token *t = malloc(sizeof(Token));
	if (t != NULL) {
		t->type = type;
		t->value = strdup(value);
		// ! strdup(char*) is not defined in the C99 spec
	}
	return t;
}

void destroy_token(Token *t) {
	free(t->value);
	free(t);
}

// Stack utility functions
Stack* create_stack() {
	Stack* stack = malloc(sizeof(Stack));
	stack->head = 0;
	return stack;
}

int is_empty(Stack *s) {
	return s->head == 0 ? TRUE : FALSE;
}

int push(Stack *s, Token *t) {
	if (s->head+1 >= STACK_SIZE) {
		return FALSE;
	}
	s->array[++(s->head)] = t;
	return TRUE;
}

Token* pop(Stack *s) {
	if (s->head < 0) return NULL; 
	return s->array[(s->head)--];
}

void destroy_stack(Stack *s) {
	Token *t;
	while ((t=pop(s)) != NULL) {
		destroy_token(t);
	}
	free(s);
}

// Lexer/parser methods
int is_op(char ch) {
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

Token* next_token(char* str, int* i) {
	while ((*i) >= 0) {
		char c = str[*i];
		if (isspace(c)) {
			(*i)--;
			continue;
		}
		else if (is_op(c)) {
			return create_token(OPERATOR, &c);
		}
		else if (c == '(' || c == ')') {
			return create_token(PARENTHESIS, &c);
		}
		else if (isdigit(c) || c == '.') {
			char digit[32];
			digit[0] = c;
			int j = 1;
			(*i)--;
			while ((*i) >= 0 && ( isdigit(str[*i]) || str[*i] == '.' )) {
				digit[j] = str[*i];
				j++; // I should also implement error checking for j's bounds
				(*i)--;
				// but it doesn't matter because digit size shoudn't exceed 32.
			}
			// reverse digit because we have been lexing backwards
			char rev[32];
			for (int k=j-1; k>=0; k--) {
				rev[j-k-1] = digit[k];
			}
			rev[j] = '\0';
			(*i)++;
			return create_token(NUMERIC, rev);
		}
		(*i)--;
	}
	return NULL;
}

int tokenize(char* str, Stack *tokens) {
	if (tokens == NULL) {
		return -1;
	}
	int i=strlen(str)-1; // Tokenizing L->R so that stack is filled R->L
	Token* tok = next_token(str,&i);
	while (tok != NULL) {
		i--;
		int result = push(tokens, tok);
		if (result < 0) return -1;
		tok = next_token(str,&i);
	}
	return 0;
}

double eval(Stack *s, int *e);

double eval_md_operand(Stack *s, int *e) {
	double ans = 0;
	Token *token = pop(s);
	int sign = 1;

	if (token == NULL) {
		*e = TRUE;
		return 0;
	}
	if (token->value[0] == '-') {
		sign *= -1;
		destroy_token(token);
		token = pop(s);
	}
	else if (token->value[0] == '+') {
		// do nothing; sign is already +ve
		destroy_token(token);
		token = pop(s);
	}

	if (token == NULL) {
		*e = TRUE;
		return 0;
	}
	if (token->type == NUMERIC) {
		ans = strtod(token->value, NULL);
		destroy_token(token);
	}
	else if (token->type == PARENTHESIS) {
		if (token->value[0] == '(') {
			ans = eval(s, e);
			destroy_token(token);
			token = pop(s);
			if (token == NULL || token->value[0] != ')') {
				*e = TRUE;
				if (token != NULL) {
					destroy_token(token);
				}
				return 0;
			}
		}
		else if (token->value[0] == ')') {
			destroy_token(token);
			*e = TRUE;
			return 0;
		}
	}
	else {
		destroy_token(token);
		*e = TRUE;
		return 0;
	}

	return ans * sign;
}

double eval_as_operand(Stack *s, int *e) {
	double lhs_operand = eval_md_operand(s, e);
	double ans = lhs_operand;
	Token* token = pop(s);
	while (token != NULL) {
		if (token->value[0] == '*' || token->value[0] == '/') {
			double rhs_operand = eval_md_operand(s, e);
			if (token->value[0] == '*') {
				ans *= rhs_operand;
			}
			else {
				ans /= rhs_operand;
			}
			destroy_token(token);
			token = pop(s);
		}
		else {
			push(s,token);
			break;
		}
	}
	return ans;
}

double eval(Stack *s, int *e) {
	// TODO maybe abstract the math into an engine, which can use either 
	// a fixed point impl or double
	double lhs_operand = eval_as_operand(s, e);
	double ans = lhs_operand;
	Token* token = pop(s);
	while (token != NULL) {
		if (token->value[0] == '+' || token->value[0] == '-') {
			double rhs_operand = eval_as_operand(s, e);
			if (token->value[0] == '+') {
				ans += rhs_operand;
			}
			else if (token->value[0] == '-') {
				ans -= rhs_operand;
			}
			destroy_token(token);
			token = pop(s);
		}
		else {
			push(s,token);
			break;
		}
	}
	return ans;
}

void print_stack(Stack* s) {
	Token *t = pop(s);
	while (t != NULL) {
		printf("Token has value %s\n", t->value);
		destroy_token(t);
		t = pop(s);
	}
}

int main(int argc, char** argv) {
	char* test_str = (argc > 1) ? argv[1] : "5+2*3.14 - (5*2/3)";
	Stack *tokens = create_stack();
	int parse_err = FALSE;
	tokenize(test_str, tokens);
	double ans = eval(tokens, &parse_err);
	if (parse_err == TRUE) {
		printf("Invalid expression\n");
	}
	else {
		printf("%f\n", ans);
	}
	destroy_stack(tokens);
}
