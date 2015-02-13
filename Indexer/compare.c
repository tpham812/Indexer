#include "compare.h"

/**
   Compare integer values. 
   Returns a value greater than 0 if i1 > i2, returns a value less than 0 if i1 < i2, returns 0 if i1 == i2.

*/
int compareInts(int p1, int p2)
{
	int i1 = p1;
	int i2 = p2;

	return i1 - i2;
}
