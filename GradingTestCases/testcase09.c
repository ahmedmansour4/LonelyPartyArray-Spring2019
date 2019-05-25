// Sean Szumlanski
// COP 3502, Spring 2019

// ==============================
// LonelyPartyArray: testcase09.c
// ==============================
// A small test of various stat-related functions.


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

int main(void)
{
	LPA *party;

	////////////////////////////////////////////////////////////////////////////
	// Create a new LonelyPartyArray (1 of 4 for this test case).
	////////////////////////////////////////////////////////////////////////////
	party = createLonelyPartyArray(3, 12);

	// Set a single element.
	assert(set(party, 14, 28) == LPA_SUCCESS);

	// Print some stats.
	printf("Size: %d\n", getSize(party));
	printf("Capacity: %d\n", getCapacity(party));
	printf("Allocated Cells: %d\n", getAllocatedCellCount(party));
	printf("Traditional Array Size: %llu bytes\n", getArraySizeInBytes(party));
	printf("Lonely Party Array Size: %llu bytes\n", getCurrentSizeInBytes(party));

	destroyLonelyPartyArray(party);

	////////////////////////////////////////////////////////////////////////////
	// Create a new LonelyPartyArray (2 of 4 for this test case).
	////////////////////////////////////////////////////////////////////////////
	party = createLonelyPartyArray(1, 1009);

	// Set a single element.
	assert(set(party, 14, 28) == LPA_SUCCESS);

	// Print some stats.
	printf("Size: %d\n", getSize(party));
	printf("Capacity: %d\n", getCapacity(party));
	printf("Allocated Cells: %d\n", getAllocatedCellCount(party));
	printf("Traditional Array Size: %llu bytes\n", getArraySizeInBytes(party));
	printf("Lonely Party Array Size: %llu bytes\n", getCurrentSizeInBytes(party));

	destroyLonelyPartyArray(party);

	////////////////////////////////////////////////////////////////////////////
	// Create a new LonelyPartyArray (3 of 4 for this test case).
	////////////////////////////////////////////////////////////////////////////
	party = createLonelyPartyArray(1009, 1);

	// Set a single element.
	assert(set(party, 14, 28) == LPA_SUCCESS);

	// Print some stats.
	printf("Size: %d\n", getSize(party));
	printf("Capacity: %d\n", getCapacity(party));
	printf("Allocated Cells: %d\n", getAllocatedCellCount(party));
	printf("Traditional Array Size: %llu bytes\n", getArraySizeInBytes(party));
	printf("Lonely Party Array Size: %llu bytes\n", getCurrentSizeInBytes(party));

	destroyLonelyPartyArray(party);

	////////////////////////////////////////////////////////////////////////////
	// Create a new LonelyPartyArray (3 of 4 for this test case).
	// This sets up the frequency from pg. 4 of the assignment PDF.
	////////////////////////////////////////////////////////////////////////////
	party = createLonelyPartyArray(11, 10);

	// Set a few elements.
	set(party, 34, 1);
	set(party, 83, 2);
	set(party, 92, 5);
	set(party, 95, 8);
	set(party, 98, 6);
	set(party, 109, 3);

	// Print some stats.
	printf("Size: %d\n", getSize(party));
	printf("Capacity: %d\n", getCapacity(party));
	printf("Allocated Cells: %d\n", getAllocatedCellCount(party));
	printf("Traditional Array Size: %llu bytes\n", getArraySizeInBytes(party));
	printf("Lonely Party Array Size: %llu bytes\n", getCurrentSizeInBytes(party));

	destroyLonelyPartyArray(party);


	return 0;
}
