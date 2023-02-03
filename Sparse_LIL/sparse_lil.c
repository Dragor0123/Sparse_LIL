#include "sparse_lil.h"
#include <stdio.h>
#include <stdlib.h>

matrixPointer hdnode[MAX_SIZE] = {0, };

matrixPointer newNode(tagfield tag)
{
	matrixPointer node = (matrixNode *)malloc(sizeof(matrixNode));
	if (node != NULL) {
		node->tag = tag;
		node->down = node->right = 0;
	}
	return node;
}

matrixPointer mread(void)
{
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;

	printf("Enter the number of rows, columns and number of nonzero terms: ");
	scanf("%d %d %d", &numRows, &numCols, &numTerms);
	numHeads = (numCols > numRows) ? numCols : numRows;

	/* Create header of heads*/
	node = newNode(entry);
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;
	
	if (!numHeads)
		node->right = node;
	else {
		for (i = 0; i < numHeads; ++i) {
			temp = newNode(head);
			hdnode[i] = temp;
			hdnode[i]->tag = temp->tag;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		}
		currentRow = 0;
		last = hdnode[0];	/* last node in current row */

		for (i = 0; i < numTerms; ++i) {
			printf("Enter row column and value : ");
			scanf("%d %d %d", &row, &col, &value);

			if (row > currentRow) { /* close current row */
				last->right = hdnode[currentRow];
				currentRow = row;
				last = hdnode[row];
			}

			temp = newNode(entry);
			temp->u.entry.row = row;
			temp->u.entry.col = col;
			temp->u.entry.value = value;
			last->right = temp;
			last = temp;

			/* link into column list */
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}

		/* close last row */
		last->right = hdnode[currentRow];
		
		/* close all column lists */
		for (i = 0; i < numCols; ++i)
			hdnode[i]->u.next->down = hdnode[i];

		/* link all header nodes together*/
		for (i = 0; i < numHeads - 1; ++i)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];
	}
	return node;
}

void mwrite(matrixPointer node)
{
	// print out the matrix in row major form
	int i;
	matrixPointer temp;
	matrixPointer head = node->right;

	/* matrix dimensions */
	printf("\n numRows = %d, numCols = %d \n",
		node->u.entry.row, node->u.entry.col);
	printf("The matrix by row, column and value: \n\n");
	for (i = 0; i < node->u.entry.row; ++i) {
		for (temp = head->right; temp != head; temp = temp->right) {
			printf("%5d%5d%5d \n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		}
		head = head->u.next;
	}
}

void merase(matrixPointer *node)
{
	int i;
	matrixPointer x, y, head;

	if (!(*node))
		return;
	head = (*node)->right;
	if (!head)
		return;

	/* free the entry and header nodes by row */
	for (i = 0; i < (*node)->u.entry.row; ++i) {
		y = head->right;
		while (y != head) {
			x = y; y = y->right; free(x);
		}
		x = head; head = head->u.next; free(x);
	}
	/* free remaining header nodes (because #col != #row) */
	y = head;
	while (y != *node) {
		x = y; y = y->u.next; free(x);
	}
	free(*node);
	*node = 0;
}
