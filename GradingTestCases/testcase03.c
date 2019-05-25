// Sean Szumlanski
// COP 3502, Spring 2019

// ==============================
// LonelyPartyArray: testcase03.c
// ==============================
// Basic check to ensure createLonelyPartyArray() is initializing everything
// correctly.


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

	assert(party != NULL);
	assert(party->size == 0);
	assert(party->num_fragments == num_fragments);
	assert(party->fragment_length == fragment_length);
	assert(party->num_active_fragments == 0);
	assert(party->fragments != NULL);
	assert(party->fragment_sizes != NULL);

	for (i = 0; i < num_fragments; i++)
		assert(party->fragments[i] == NULL);

	for (i = 0; i < num_fragments; i++)
		assert(party->fragment_sizes[i] == 0);

	destroyLonelyPartyArray(party);

	return 0;
}
