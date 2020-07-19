#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STACK_SIZE 1000

typedef struct {
	double x,y,z;
} vec3_t;

typedef struct {
	vec3_t *stack[STACK_SIZE];
	int top;
} Stack;

void push_to_stack(Stack *stack, vec3_t* vector) {
	stack->stack[++stack->top] = vector;	
}

void print_vec3(vec3_t *vector) {
	printf("%.2fi + %.2fj + %.2fk\n", vector->x, vector->y, vector->z);
}

vec3_t* pop_from_stack(Stack *stack) {
	return stack->stack[stack->top--];
}

vec3_t* get_vector(char* s, int* i) {
	vec3_t *vect = malloc(sizeof(vec3_t));
	vect->x = 0;
	vect->y = 0;
	vect->z = 0;
	char num[100];
	char c;
	int j=0;
	while ((c=s[++(*i)]) != ')') {
		if (c == '-' || c == '+' || isdigit(c) || c == '.') {
			num[j] = c;
			j++;
		}
		else if (c == 'i') {
			num[j] = '\0';
			j = 0;
			vect->x = atof(num);
			memset(num, 0, 100);
		}
		else if (c == 'j') {
			num[j] = '\0';
			vect->y = atof(num);
			j = 0;
			memset(num, 0, 100);
		}
		else if (c == 'k') {
			num[j] = '\0';
			vect->z = atof(num);
			j = 0;
			memset(num, 0, 100);
		}
	}
	printf("Found vector ");
	print_vec3(vect);
	return vect;
}

vec3_t* evaluate(char* s) {
	Stack* vec_stack = malloc(sizeof(Stack));
	int len = strlen(s);
	for (int i=0; i<len; i++) {
		char c = s[i];
		if (c == '(') {
			// new vector definition
			vec3_t *vect = get_vector(s, &i);
			push_to_stack(vec_stack, vect);
		}
		else if (c == '+') {
			// add vectors
			printf("Adding vectors\n");
			vec3_t *nextvect = get_vector(s,&(i));
			vec3_t *prevvect = pop_from_stack(vec_stack);
			vec3_t *ans = malloc(sizeof(vec3_t));
			ans->x = nextvect->x + prevvect->x;	
			ans->y = nextvect->y + prevvect->y;	
			ans->z = nextvect->z + prevvect->z;	
			push_to_stack(vec_stack, ans);
			free(nextvect);
			free(prevvect);
		}
		else if (c == '*') {
			printf("Vector cross product\n");
			vec3_t *nv = get_vector(s,&(i));
			vec3_t *pv = pop_from_stack(vec_stack);
			vec3_t *ans = malloc(sizeof(vec3_t));
			ans->x = (pv->y*nv->z) - (nv->y*pv->z); 
			ans->y = - ((pv->x*nv->z) - (nv->x*pv->z));	
			ans->z = (pv->x*nv->y) - (nv->x*pv->y);	
			push_to_stack(vec_stack, ans);
			free(nv);
			free(pv);
		}
		else if (c == '.') {
			printf("Vector dot product\n");
			vec3_t *nv = get_vector(s,&(i));
			vec3_t *pv = pop_from_stack(vec_stack);
			vec3_t *ans = malloc(sizeof(vec3_t));
			ans->x = (nv->x*pv->x) + (nv->y*pv->y) + (nv->z*pv->z);
			push_to_stack(vec_stack, ans);
			free(nv);
			free(pv);
		}
		else {
			// whitespace; ignore
		}
	}
	vec3_t *ans = pop_from_stack(vec_stack);
	free(vec_stack);
	return ans;
}

void main(int argc, char** argv) {
	char* s = "None";
	if (argc > 1) {
		s = argv[1];
	}
	if (strcmp(s,"None") != 0) {
		// do parsing
		vec3_t* vector = evaluate(s);
		printf("Got result: \n");
		print_vec3(vector);
	}
}
