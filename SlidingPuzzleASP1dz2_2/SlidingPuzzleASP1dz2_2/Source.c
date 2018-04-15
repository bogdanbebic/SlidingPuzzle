#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_SIZE 3
#define MATRIX_ELEMENTS MATRIX_SIZE * MATRIX_SIZE

#define MAX 381367044	// MAX changes if MATRIX_SIZE changes
#define MIN 6053444	// MIN changes if MATRIX_SIZE changes

#define BITS_IN_BYTE 8

typedef struct StackDataStruct {
	int value[MATRIX_SIZE][MATRIX_SIZE];
	int timesOnStack;
} StackData;

typedef struct StackElemStruct {
	StackData data;
	struct StackElemStruct *next;
} StackElem;

#pragma region STACK_FUNCTIONS_IMPLEMENTATION

void push(StackElem **stack, StackData data) {
	StackElem *newElem;
	int i, j;
	newElem = (StackElem *)malloc(sizeof(StackElem));
	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			newElem->data.value[i][j] = data.value[i][j];
		}
	}
	newElem->data.timesOnStack = data.timesOnStack;
	newElem->next = *stack;
	*stack = newElem;
	return;
}

StackData pop(StackElem **stack) {
	StackElem *old;
	StackData elemData;
	old = *stack;
	elemData = (*stack)->data;
	*stack = (*stack)->next;
	free(old);
	return elemData;
}

StackData top(StackElem **stack) {
	return (*stack)->data;
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
	int indices[MATRIX_ELEMENTS];
	int i, index, t;
	double randomNumber;
	for (i = 0; i < MATRIX_ELEMENTS; i++) {
		indices[i] = i;
	}
	for (i = 0; i < MATRIX_ELEMENTS; i++) {
		randomNumber = (double)rand() / (RAND_MAX + 1);
		index = (int)(randomNumber * (MATRIX_ELEMENTS - i) + i);
		t = indices[i];
		indices[i] = indices[index];
		indices[index] = t;
	}
	for (i = 0; i < MATRIX_ELEMENTS; i++) {
		matrix[indices[i] / MATRIX_SIZE][indices[i] % MATRIX_SIZE] = i;
	}
	return;
}

#pragma endregion

#pragma region MATRIX_ELEMENT_MOVING

void moveLeft(int matrix[MATRIX_SIZE][MATRIX_SIZE], int *i, int *j) {
	int t;
	t = matrix[*i][*j];
	matrix[*i][*j] = matrix[*i][*j - 1];
	matrix[*i][*j - 1] = t;
	(*j)--;
	return;
}

void moveRight(int matrix[MATRIX_SIZE][MATRIX_SIZE], int *i, int *j) {
	int t;
	t = matrix[*i][*j];
	matrix[*i][*j] = matrix[*i][*j + 1];
	matrix[*i][*j + 1] = t;
	(*j)++;
	return;
}

void moveUp(int matrix[MATRIX_SIZE][MATRIX_SIZE], int *i, int *j) {
	int t;
	t = matrix[*i][*j];
	matrix[*i][*j] = matrix[*i - 1][*j];
	matrix[*i - 1][*j] = t;
	(*i)--;
	return;
}

void moveDown(int matrix[MATRIX_SIZE][MATRIX_SIZE], int *i, int *j) {
	int t;
	t = matrix[*i][*j];
	matrix[*i][*j] = matrix[*i + 1][*j];
	matrix[*i + 1][*j] = t;
	(*i)++;
	return;
}


int move(int matrix[MATRIX_SIZE][MATRIX_SIZE], int *i, int *j, int *direction) {
	switch (*direction) {
	case 0:
		if (*j != 0) {
			moveLeft(matrix, i, j);
			return 1;
		}
		break;
	case 1:
		if (*j != MATRIX_SIZE - 1) {
			moveRight(matrix, i, j);
			return 1;
		}
		break;
	case 2:
		if (*i != 0) {
			moveUp(matrix, i, j);
			return 1;
		}
		break;
	case 3:
		if (*i != MATRIX_SIZE - 1) {
			moveDown(matrix, i, j);
			return 1;
		}
		break;
	}
	return 0;
}

#pragma endregion

#pragma region PUZZLE_SOLVABILITY_CHECKING

void findCellWithValue(int matrix[MATRIX_SIZE][MATRIX_SIZE], int *iEmpty, int *jEmpty, int cellValue) {
	int i, j;
	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			if (matrix[i][j] == cellValue) {
				*iEmpty = i;
				*jEmpty = j;
				return;
			}
		}
	}
	return;
}

void matchEmptyCellsInBothMatrices(int start[MATRIX_SIZE][MATRIX_SIZE], int end[MATRIX_SIZE][MATRIX_SIZE]) {
	int iEmptyStart, jEmptyStart;
	int iEmptyEnd, jEmptyEnd;

	findCellWithValue(start, &iEmptyStart, &jEmptyStart, 0);
	findCellWithValue(end, &iEmptyEnd, &jEmptyEnd, 0);

	while (iEmptyStart != iEmptyEnd || jEmptyStart != jEmptyEnd) {
		if (iEmptyStart > iEmptyEnd) {
			moveUp(start, &iEmptyStart, &jEmptyStart);
		}
		if (iEmptyStart < iEmptyEnd) {
			moveDown(start, &iEmptyStart, &jEmptyStart);
		}
		if (jEmptyStart > jEmptyEnd) {
			moveLeft(start, &iEmptyStart, &jEmptyStart);
		}
		if (jEmptyStart < jEmptyEnd) {
			moveRight(start, &iEmptyStart, &jEmptyStart);
		}
	}
	return;
}

int isSolutionPossible(int x[MATRIX_SIZE][MATRIX_SIZE], int y[MATRIX_SIZE][MATRIX_SIZE]) {
	int i, j;
	int i1, j1;
	int counter = 0;
	int t;
	int start[MATRIX_SIZE][MATRIX_SIZE], end[MATRIX_SIZE][MATRIX_SIZE];

	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			start[i][j] = x[i][j];
			end[i][j] = y[i][j];
		}
	}

	matchEmptyCellsInBothMatrices(start, end);
	
	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			if (start[i][j] != end[i][j]) {
				findCellWithValue(start, &i1, &j1, end[i][j]);
				t = start[i][j];
				start[i][j] = start[i1][j1];
				start[i1][j1] = t;
				counter++;
			}
		}
	}
	return !(counter % 2);
}

#pragma endregion

#pragma region MATRIX_FUNCTIONS

int hashMatrixToInt(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
	int number = 0;
	int product = 1;
	int i, j;
	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			number += matrix[i][j] * product;
			product *= MATRIX_ELEMENTS;
		}
	}
	return number - MIN;
}

int areEqual(int x[MATRIX_SIZE][MATRIX_SIZE], int y[MATRIX_SIZE][MATRIX_SIZE]) {
	int i, j;
	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			if (x[i][j] != y[i][j]) return 0;
		}
	}
	return 1;
}

void outputMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
	int i, j;
	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			printf("%d ", matrix[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
	return;
}

#pragma endregion

char set[MAX - MIN];


void findPuzzleSolution(int start[MATRIX_SIZE][MATRIX_SIZE], int end[MATRIX_SIZE][MATRIX_SIZE]) {
	int iEmpty, jEmpty;
	int i, j;
	StackData data;
	StackElem *stack = NULL;
	StackElem *stack1 = NULL;
	int flag = 1;
	int counter = 0;
	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			data.value[i][j] = start[i][j];
		}
	}
	data.timesOnStack = 0;
	findCellWithValue(start, &iEmpty, &jEmpty, 0);

	while (!areEqual(data.value, end)) {
		while (flag) {
			if (areEqual(data.value, end)) break;
			if (set[hashMatrixToInt(data.value)]) {
				flag = 0;
			}
			else {
				set[hashMatrixToInt(data.value)] = 1;
				push(&stack, data);
				counter++;
				if (jEmpty != 0) {
					moveLeft(data.value, &iEmpty, &jEmpty);
					data.timesOnStack = 0;
				}
				else {
					flag = 0;
				}
			}
		}
		if (areEqual(data.value, end)) break;
		data = pop(&stack);
		counter--;
		findCellWithValue(data.value, &iEmpty, &jEmpty, 0);
		if (data.timesOnStack < 3) {
			data.timesOnStack++;
			push(&stack, data);
			counter++;
			flag = move(data.value, &iEmpty, &jEmpty, &data.timesOnStack);
			data.timesOnStack = 0;
		}
	}
	printf("%d\n", counter);
	push(&stack, data);

	while (!isStackEmpty(&stack)) {
		push(&stack1, pop(&stack));
	}
	while (!isStackEmpty(&stack1)) {
		data = pop(&stack1);
		outputMatrix(data.value);
	}
	return;
}


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
	int i;
	int flag = 0;

	srand((int)time(NULL));

	while (1) {
		printMenu();
		scanf_s("%d", &menuOption);
		switch (menuOption) {
		case 1:	// Read start and end matrix
			printf("Enter start matrix:\n");
			readMatrix(start);
			printf("Enter end matrix:\n");
			readMatrix(end);
			flag = 1;
			break;
		case 2:	// Generate start and end matrix
			generateMatrix(start);
			outputMatrix(start);
			generateMatrix(end);
			outputMatrix(end);
			flag = 1;
			break;
		case 3:	// Check if solution exists
			if (flag) {
				if (isSolutionPossible(start, end)) {
					printf("Solution is possible!\n");
				}
				else {
					printf("The puzzle is unsolvable :(\n");
				}
			}
			break;
		case 4:	// Simulate puzzle
			if (flag && isSolutionPossible(start, end)) {
				for (i = 0; i < MAX - MIN; set[i++] = 0);
				findPuzzleSolution(start, end);
			}
			else {
				printf("There is no puzle or it is unsolvable\n");
			}
			break;
		case 0:	// Exit program
			exit(0);
		default:
			break;
		}
	}
	return 0;
}
