#include <stdio.h>
#include <stdlib.h>
#include "sparse_lil.h"

int main(int argc, char* argv[])
{
	matrixPointer test1 =  mread();
	mwrite(test1);
	merase(test1);
	return 0;
}