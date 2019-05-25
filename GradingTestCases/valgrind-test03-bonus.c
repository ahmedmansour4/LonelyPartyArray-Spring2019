// Sean Szumlanski
// COP 3502, Spring 2019

// =========================================
// LonelyPartyArray: valgrind-test03-bonus.c
// =========================================
// Tests the functionality of cloneLonelyPartyArray(). This is a bonus test
// case.


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

// Check the struct contents and abort program if anything is out of sorts.
void party_check(LonelyPartyArray *party)
{
	int i;
	int num_fragments = 14;
	int fragment_length = 8;

	// Check struct setup.
	assert(party != NULL);
	assert(party->size == 6);
	assert(party->num_fragments == num_fragments);
	assert(party->fragment_length == fragment_length);
	assert(party->num_active_fragments == 5);
	assert(party->fragments != NULL);
	assert(party->fragment_sizes != NULL);

	// Check fragments. Only fragments 4, 10, 11, 12, and 13 should be active.
	for (i = 0; i < num_fragments; i++)
	{
		if (i == 4 || i == 10 || i == 11 || i == 12 || i == 13)
			assert(party->fragments[i] != NULL);
		else
			assert(party->fragments[i] == NULL);
	}

	// Check contents of fragment 4.
	for (i = 0; i < fragment_length; i++)
	{
		if (i == 2)
			assert(party->fragments[4][i] == 1);
		else
			assert(party->fragments[4][i] == UNUSED);
	}

	// Check contents of fragment 10.
	for (i = 0; i < fragment_length; i++)
	{
		if (i == 3)
			assert(party->fragments[10][i] == 2);
		else
			assert(party->fragments[10][i] == UNUSED);
	}

	// Check contents of fragment 11.
	for (i = 0; i < fragment_length; i++)
	{
		if (i == 4)
			assert(party->fragments[11][i] == 5);
		else if (i == 7)
			assert(party->fragments[11][i] == 8);
		else
			assert(party->fragments[11][i] == UNUSED);
	}

	// Check contents of fragment 12.
	for (i = 0; i < fragment_length; i++)
	{
		if (i == 2)
			assert(party->fragments[12][i] == 6);
		else
			assert(party->fragments[12][i] == UNUSED);
	}

	// Check contents of fragment 13.
	for (i = 0; i < fragment_length; i++)
	{
		if (i == 5)
			assert(party->fragments[13][i] == 3);
		else
			assert(party->fragments[13][i] == UNUSED);
	}
}

int main(void)
{
	LPA *party = createLonelyPartyArray(14, 8);
	LPA *clone;

	// Set a few elements.
	set(party, 34, 1);
	set(party, 83, 2);
	set(party, 92, 5);
	set(party, 95, 8);
	set(party, 98, 6);
	set(party, 109, 3);

	// Check structure and contents of the original lonely party array.
	party_check(party);

	// Clone the party array.
	clone = cloneLonelyPartyArray(party);

	// Check structure and contents of the clone.
	party_check(clone);

	// Check that the structure and contents of the original have survived.
	party_check(party);

	// Ensure that clone is not simply referring to the original lonely party
	// array or any of its constituent members.
	assert(clone != party);
	assert(clone->fragments != party->fragments);
	assert(clone->fragment_sizes != party->fragment_sizes);
	assert(clone->fragments[4] != party->fragments[4]);
	assert(clone->fragments[10] != party->fragments[10]);
	assert(clone->fragments[11] != party->fragments[11]);
	assert(clone->fragments[12] != party->fragments[12]);
	assert(clone->fragments[13] != party->fragments[13]);

	destroyLonelyPartyArray(party);
	destroyLonelyPartyArray(clone);

	return 0;
}
