// Sean Szumlanski
// COP 3502, Spring 2019

// ==============================
// LonelyPartyArray: testcase06.c
// ==============================
// Tests various aspects of delete() functionality. This builds on testcase05.c.


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

	// Call set() multiple times on same index.
	assert(set(party, 14, 100) == LPA_SUCCESS);
	assert(set(party, 14, 100) == LPA_SUCCESS);
	assert(set(party, 14, 100) == LPA_SUCCESS);

	// Check that the struct is set up correctly to begin with.
	assert(party != NULL);
	assert(party->size == 1);
	assert(party->num_fragments == num_fragments);
	assert(party->fragment_length == fragment_length);
	assert(party->num_active_fragments == 1);
	assert(party->fragments != NULL);
	assert(party->fragment_sizes != NULL);

	for (i = 0; i < num_fragments; i++)
	{
		if (i == 4)
			assert(party->fragments[i] != NULL);
		else
			assert(party->fragments[i] == NULL);
	}

	for (i = 0; i < num_fragments; i++)
	{
		if (i == 4)
			assert(party->fragment_sizes[i] == 1);
		else
			assert(party->fragment_sizes[i] == 0);
	}

	for (i = 0; i < fragment_length; i++)
	{
		if (i == 2)
			assert(party->fragments[4][i] == 100);
		else
			assert(party->fragments[4][i] == UNUSED);
	}

	// Check that delete() doesn't cause the deallocation of a fragment when
	// attempting to delete a cell that is actually UNUSED.
	assert(delete(party, 15) == LPA_FAILURE);
	assert(party->size == 1);
	assert(party->num_active_fragments == 1);
	assert(party->fragment_sizes[4] == 1);
	assert(party->fragments[4] != NULL);

	// Insert two new elements. Then check that calling delete() on the same
	// element many times in a row doesn't cause the LPA size or fragment size
	// to be reduced many times, resulting in a fragment deallocation.
	assert(set(party, 13, 100) == LPA_SUCCESS);
	assert(set(party, 15, 100) == LPA_SUCCESS);
	assert(delete(party, 15) == LPA_SUCCESS);
	assert(delete(party, 15) == LPA_FAILURE);
	assert(delete(party, 15) == LPA_FAILURE);
	assert(delete(party, 15) == LPA_FAILURE);

	// Also check that calling delete() on invalid indices produces the correct
	// output and doesn't unsettle the struct's member values. These first two
	// calls should not produce "invalid access" messages because the indices
	// are within bounds (even though they refer to UNUSED cells).
	assert(delete(party, 0) == LPA_FAILURE);
	assert(delete(party, 35) == LPA_FAILURE);
	assert(delete(party, 36) == LPA_FAILURE);
	assert(delete(party, 48) == LPA_FAILURE);
	assert(delete(party, 49) == LPA_FAILURE);
	assert(delete(party, 59) == LPA_FAILURE);

	// Check that the struct is still set up correctly.
	assert(party != NULL);
	assert(party->size == 2);
	assert(party->num_fragments == num_fragments);
	assert(party->fragment_length == fragment_length);
	assert(party->num_active_fragments == 1);
	assert(party->fragments != NULL);
	assert(party->fragment_sizes != NULL);

	for (i = 0; i < num_fragments; i++)
	{
		if (i == 4)
			assert(party->fragments[i] != NULL);
		else
			assert(party->fragments[i] == NULL);
	}

	for (i = 0; i < num_fragments; i++)
	{
		if (i == 4)
			assert(party->fragment_sizes[i] == 2);
		else
			assert(party->fragment_sizes[i] == 0);
	}

	for (i = 0; i < fragment_length; i++)
	{
		if (i == 1)
			assert(party->fragments[4][i] == 100);
		else if (i == 2)
			assert(party->fragments[4][i] == 100);
		else
			assert(party->fragments[4][i] == UNUSED);
	}

	destroyLonelyPartyArray(party);

	return 0;
}
