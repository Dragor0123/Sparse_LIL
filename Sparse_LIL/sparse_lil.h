#pragma once
#define MAX_SIZE 50

typedef enum { head, entry } tagfield;
typedef struct matrixNode* matrixPointer;
typedef struct entryNode {
	int row;
	int col;
	int value;
}entryNode;

typedef struct matrixNode {
	matrixPointer down;
	matrixPointer right;

	tagfield tag;
	union {
		matrixPointer next;
		entryNode entry;
	} u;
}matrixNode;

extern matrixPointer hdnode[MAX_SIZE];

matrixPointer newNode(tagfield tag);
matrixPointer mread(void);
void mwrite(matrixPointer node);
void merase(matrixPointer* node);