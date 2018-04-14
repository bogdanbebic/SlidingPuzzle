#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_SIZE 3
#define MATRIX_ELEMENTS MATRIX_SIZE * MATRIX_SIZE

#define MAX 381366965	// MAX changes if MATRIX_SIZE changes
#define MIN 6053444	// MIN changes if MATRIX_SIZE changes

#define BITS_IN_BYTE 8


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
	int t;

	findCellWithValue(start, &iEmptyStart, &jEmptyStart, 0);
	findCellWithValue(end, &iEmptyEnd, &jEmptyEnd, 0);

	while (iEmptyStart != iEmptyEnd || jEmptyStart != jEmptyEnd) {
		if (iEmptyStart > iEmptyEnd) {
			t = start[iEmptyStart][jEmptyStart];
			start[iEmptyStart][jEmptyStart] = start[iEmptyStart - 1][jEmptyStart];
			start[iEmptyStart - 1][jEmptyStart] = t;
			iEmptyStart--;
		}
		if (iEmptyStart < iEmptyEnd) {
			t = start[iEmptyStart][jEmptyStart];
			start[iEmptyStart][jEmptyStart] = start[iEmptyStart + 1][jEmptyStart];
			start[iEmptyStart + 1][jEmptyStart] = t;
			iEmptyStart++;
		}
		if (jEmptyStart > jEmptyEnd) {
			t = start[iEmptyStart][jEmptyStart];
			start[iEmptyStart][jEmptyStart] = start[iEmptyStart][jEmptyStart - 1];
			start[iEmptyStart][jEmptyStart - 1] = t;
			jEmptyStart--;
		}
		if (jEmptyStart < jEmptyEnd) {
			t = start[iEmptyStart][jEmptyStart];
			start[iEmptyStart][jEmptyStart] = start[iEmptyStart][jEmptyStart + 1];
			start[iEmptyStart][jEmptyStart + 1] = t;
			jEmptyStart++;
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


void printMenu() {
	printf("1. Enter start and end positions for the puzzle\n");
	printf("2. Generate start and end positions for the puzzle\n");
	printf("3. Check if end position is reachable\n");
	printf("4. Simulate the puzzle\n");
	printf("0. Exit program\n");
	printf("Enter menu action as integer:\n");
	return;
}


char set[MAX - MIN];

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
			/* TODO: deallocation */
			exit(0);
		default:
			break;
		}
	}
	return 0;
}
