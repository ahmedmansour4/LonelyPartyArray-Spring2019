// Sean Szumlanski
// COP 3502, Spring 2019

// ==============================
// LonelyPartyArray: testcase15.c
// ==============================
// A test of the containsKey() and isSet() functions.


#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

int main(void)
{
	LPA *party = createLonelyPartyArray(100000, 1000);

	// Set a few elements.
	set(party, 58105, 988);
	set(party, 27272, 535);
	set(party, 624362, 64);
	set(party, 72477, 167);
	set(party, 825682, 94);

	// The following checks should be relatively quick, since there are few active fragments.
	printf("%s\n", containsKey(NULL, 58105) ? "found key" : "key not found");     // not found
	printf("%s\n", containsKey(party, 58105) ? "found key" : "key not found");    // not found
	printf("%s\n", containsKey(party, 27272) ? "found key" : "key not found");    // not found
	printf("%s\n", containsKey(party, 624362) ? "found key" : "key not found");   // not found
	printf("%s\n", containsKey(party, 72477) ? "found key" : "key not found");    // not found
	printf("%s\n", containsKey(party, 825682) ? "found key" : "key not found");   // not found
	printf("%s\n", containsKey(party, 1000000) ? "found key" : "key not found");  // not found
	printf("%s\n", containsKey(party, 0) ? "found key" : "key not found");        // not found
	printf("%s\n", containsKey(party, 1) ? "found key" : "key not found");        // not found
	printf("%s\n", containsKey(party, -1) ? "found key" : "key not found");       // not found
	printf("%s\n", containsKey(party, 412) ? "found key" : "key not found");      // not found
	printf("%s\n", containsKey(party, 988) ? "found key" : "key not found");      // found
	printf("%s\n", containsKey(party, 535) ? "found key" : "key not found");      // found
	printf("%s\n", containsKey(party, 64) ? "found key" : "key not found");       // found
	printf("%s\n", containsKey(party, 167) ? "found key" : "key not found");      // found
	printf("%s\n", containsKey(party, 94) ? "found key" : "key not found");       // found

	// Print a few set vs. unset checks.
	printf("%s\n", isSet(NULL, 100) ? "set" : "unset");          // unset
	printf("%s\n", isSet(party, 0) ? "set" : "unset");           // unset
	printf("%s\n", isSet(party, 58104) ? "set" : "unset");       // unset
	printf("%s\n", isSet(party, 58105) ? "set" : "unset");       // set
	printf("%s\n", isSet(party, 58106) ? "set" : "unset");       // unset
	printf("%s\n", isSet(party, 100000) ? "set" : "unset");      // unset
	printf("%s\n", isSet(party, 200000) ? "set" : "unset");      // unset
	printf("%s\n", isSet(party, 300000) ? "set" : "unset");      // unset
	printf("%s\n", isSet(party, 400000) ? "set" : "unset");      // unset
	printf("%s\n", isSet(party, 500000) ? "set" : "unset");      // unset
	printf("%s\n", isSet(party, 600000) ? "set" : "unset");      // unset
	printf("%s\n", isSet(party, 825682) ? "set" : "unset");      // set
	printf("%s\n", isSet(party, 100000000) ? "set" : "unset");   // unset
	printf("%s\n", isSet(party, 200000000) ? "set" : "unset");   // unset
	printf("%s\n", isSet(party, 500000000) ? "set" : "unset");   // unset
	printf("%s\n", isSet(party, 1000000000) ? "set" : "unset");  // unset

	destroyLonelyPartyArray(party);

	return 0;
}
