// Sean Szumlanski
// COP 3502, Spring 2019

// ==============================
// LonelyPartyArray: testcase07.c
// ==============================
// Tests various aspects of printIfValid() functionality.


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

int main(void)
{
	int i;
	int num_fragments = 12;
	int fragment_length = 3;

	LPA *party = createLonelyPartyArray(num_fragments, fragment_length);

	// set() a few elements.
	assert(set(party, 35, 93899) == LPA_SUCCESS);
	assert(set(party, 8, 412) == LPA_SUCCESS);
	assert(set(party, 11, 83644) == LPA_SUCCESS);

	// Check basic printIfValid() functionality.
	for (i = -2; i <= 37; i++)
	{
		if (i == 8 || i == 11 || i == 35)
			continue;
		assert(printIfValid(party, i) == LPA_FAILURE);
	}

	// Call printIfValid() multiple times to ensure it isn't deleting or
	// modifying cells.
	assert(printIfValid(party, 8) == LPA_SUCCESS);
	assert(printIfValid(party, 8) == LPA_SUCCESS);
	assert(printIfValid(party, 11) == LPA_SUCCESS);
	assert(printIfValid(party, 11) == LPA_SUCCESS);
	assert(printIfValid(party, 35) == LPA_SUCCESS);

	destroyLonelyPartyArray(party);

	return 0;
}
