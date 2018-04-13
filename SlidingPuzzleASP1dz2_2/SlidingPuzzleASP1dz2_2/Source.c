#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_SIZE 3


typedef struct StackElemStruct {
	int value;
	struct StackElemStruct *next;
} StackElem;

#pragma region STACK_FUNCTIONS_IMPLEMENTATION

void push(StackElem **stack, int elemValue) {
	StackElem *newElem;
	newElem = (StackElem *)malloc(sizeof(StackElem));
	newElem->value = elemValue;
	newElem->next = *stack;
	*stack = newElem;
	return;
}

int pop(StackElem **stack) {
	StackElem *old;
	int elemValue;
	old = *stack;
	elemValue = (*stack)->value;
	*stack = (*stack)->next;
	free(old);
	return elemValue;
}

int top(StackElem **stack) {
	return (*stack)->value;
}

int isStackEmpty(StackElem **stack) {
	return (*stack == NULL);
}

#pragma endregion

#pragma region MATRIX_READING_AND_GENERATING

void readMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
	int i, j;
	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			scanf_s("%d", &matrix[i][j]);
		}
	}
	return;
}

void generateMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
	int indices[MATRIX_SIZE * MATRIX_SIZE];
	int i, index, t;
	double randomNumber;
	for (i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
		indices[i] = i;
	}
	srand((int)time(NULL));
	for (i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
		randomNumber = (double)rand() / (RAND_MAX + 1);
		index = (int)(randomNumber * (MATRIX_SIZE * MATRIX_SIZE - i) + i);
		t = indices[i];
		indices[i] = indices[index];
		indices[index] = t;
	}
	for (i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
		matrix[indices[i] / MATRIX_SIZE][indices[i] % MATRIX_SIZE] = i;
	}
	return;
}

#pragma endregion

void printMenu() {
	printf("1. Enter start and end positions for the puzzle\n");
	printf("2. Generate start and end positions for the puzzle\n");
	printf("3. Check if end position is reachable\n");
	printf("4. Simulate the puzzle\n");
	printf("0. Exit program\n");
	printf("Enter menu action as integer:\n");
	return;
}

int main() {
	int menuOption;
	int start[MATRIX_SIZE][MATRIX_SIZE];
	int end[MATRIX_SIZE][MATRIX_SIZE];


	while (1) {
		printMenu();
		scanf_s("%d", &menuOption);
		switch (menuOption) {
		case 1:	// Read start and end matrix
			printf("Enter start matrix:\n");
			readMatrix(start);
			printf("Enter end matrix:\n");
			readMatrix(end);
			break;
		case 2:	// Generate start and end matrix
			generateMatrix(start);
			generateMatrix(end);
			break;
		case 3:	// Check if solution exists
			/* TODO: implement */
			break;
		case 4:	// Simulate puzzle
			/* TODO: implement */
			break;
		case 0:	// Exit program
			/* TODO: deallocation */
			exit(0);
		default:
			break;
		}
	}
	return 0;
}
