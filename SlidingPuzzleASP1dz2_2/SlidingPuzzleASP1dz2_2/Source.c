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

void push(StackElem **stack, int elemValue[MATRIX_SIZE][MATRIX_SIZE]) {
	StackElem *newElem;
	int i, j;
	newElem = (StackElem *)malloc(sizeof(StackElem));
	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			newElem->data.value[i][j] = elemValue[i][j];
		}
	}
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

int isSolutionPossible(int start[MATRIX_SIZE][MATRIX_SIZE], int end[MATRIX_SIZE][MATRIX_SIZE]) {
	int i, j;
	int i1, j1;
	int counter = 0;
	int t;

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
	return number;
}


char set[MAX - MIN];

/*
*	TODO:
*	implement
*/
void findPuzzleSolution(int start[MATRIX_SIZE][MATRIX_SIZE], int end[MATRIX_SIZE][MATRIX_SIZE]) {
	int m[MATRIX_SIZE][MATRIX_SIZE];
	int iEmpty, jEmpty;
	int i, j;

	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			m[i][j] = start[i][j];
		}
	}

	findCellWithValue(start, &iEmpty, &jEmpty, 0);
	
	/* TODO: implement */

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
			generateMatrix(end);
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
				/* TODO: implement */
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
