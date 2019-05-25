// Ahmed Mansour
// COP3502, Spring 2019
// ah505081
// This program creates a space efficient array data structure called a LonelyPartyArray
#include "LonelyPartyArray.h"
#include <stdio.h>
#include <stdlib.h>


LonelyPartyArray *fillUnused(LonelyPartyArray *party, int fragmentNum);

// Creates a LonelyPartyArray data structure
LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
	if(num_fragments <= 0 || fragment_length <= 0)
		return NULL;
	LonelyPartyArray *newLPA = malloc(sizeof(LonelyPartyArray));
	if(newLPA == NULL)
	{
		free(newLPA);
		return NULL;
	}
	newLPA->num_fragments = num_fragments;
	newLPA->fragment_length = fragment_length;
	newLPA->num_active_fragments = 0;
	newLPA->size = 0;
	newLPA->fragments = malloc(sizeof(int*) * newLPA->num_fragments); 
	
	// Check if malloc call failed, if so free everything allocated so far
	if(newLPA->fragments == NULL)
	{
		free(newLPA->fragments);
		free(newLPA);
		return NULL;
	}
	
	for(int i = 0; i < num_fragments; i++)
	{
		// Set all fragment int arrays pointers to NULL
		newLPA->fragments[i] = NULL;
		
	}
	
	newLPA->fragment_sizes = malloc(sizeof(int*) * newLPA->fragment_length);
	for(int i = 0; i < num_fragments; i++)
	{
		newLPA->fragment_sizes[i] = 0;
	}
	printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", (fragment_length * num_fragments), num_fragments);
	return newLPA;
	
}
// Destroy and deallocate everything related to the passed in LonelyPartyArray
LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
	if(party != NULL)
	{
		for(int i = 0; i < party->num_fragments; i++)
		{
			// Free each fragment in LonelyPartyArray
			free(party->fragments[i]);
		}
		// Free everything else that was allocated
		free(party->fragments);
		free(party->fragment_sizes);
		free(party);
		printf("-> The LonelyPartyArray has returned to the void.\n");
	}
	return NULL;
}
// Sets one value inside the LonelyPartyArray to the passed in value
int set(LonelyPartyArray *party, int index, int key)
{
	if(party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in set().\n");
		return LPA_FAILURE;
	}
	int fragmentNum = index / party->fragment_length;
	// The exact cell that we need is found by using the modulo operator
	int cellNum = index % party->fragment_length;
	int capacity = (party->num_fragments * party->fragment_length);
	
	if(index < capacity && index >= 0)
	{
		// Check if specific int array has been uninitialized.
		if(party->fragments[fragmentNum] == NULL) 
		{
			
			party->fragments[fragmentNum] = malloc(sizeof(int) * party->fragment_length);
			// Fill the newly allocated fragment with UNUSED values
			party = fillUnused(party, fragmentNum);
			party->num_active_fragments++;
			printf("-> Spawned fragment %d. (capacity: %d, ", fragmentNum, party->fragment_length);
			printf("indices: %d..%d)\n", party->fragment_length * (fragmentNum), (party->fragment_length * (fragmentNum + 1)) - 1);
		}
		// If the cell being set was previously UNUSED, then increment the size and fragment_size (at the current fragment) members by 1
		if(party->fragments[fragmentNum][cellNum] == UNUSED)
		{
			party->fragment_sizes[fragmentNum]++;
			party->size++;
		}
		party->fragments[fragmentNum][cellNum] = key;
		return LPA_SUCCESS;
	}
	else
	{
		printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, fragmentNum, cellNum); 
	}
	return LPA_FAILURE;
}
// Helper function that fills the specific fragment of the LonelyPartyArray with the UNUSED value
LonelyPartyArray *fillUnused(LonelyPartyArray *party, int fragmentNum)
{
	for(int i = 0; i < party->fragment_length; i++)
		party->fragments[fragmentNum][i] = UNUSED;
	return party;
}
// Returns the value held at the specified index in the LonelyPartyArray
int get(LonelyPartyArray *party, int index)
{
	if(party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in get().\n");
		return LPA_FAILURE;
	}
	int fragmentNum = index / party->fragment_length;
	int cellNum = index % party->fragment_length;
	int capacity = (party->num_fragments * party->fragment_length);
	// Check if index is valid
	if(index < capacity && index >= 0)
	{	// Check if the fragment has been allocated already
		if(party->fragments[fragmentNum] != NULL)
		{
			// Check if the specific cell is not UNUSED
			if(party->fragments[fragmentNum][cellNum] != UNUSED) 
			{
				return party->fragments[fragmentNum][cellNum];
			}
			else if(party->fragments[fragmentNum][cellNum] == UNUSED)
			{
				return UNUSED;
			}
		}
		else
		{
			return UNUSED;
		}
	}
	printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, fragmentNum, cellNum);
	return LPA_FAILURE;
}
// Deletes the value held at the specified index
int delete(LonelyPartyArray *party, int index)
{
	if(party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in delete().\n");
		return LPA_FAILURE;
	}
	int fragmentNum = index / party->fragment_length;
	int cellNum = index % party->fragment_length;
	int capacity = (party->num_fragments * party->fragment_length);
	// Check if index is valid
	if(index < capacity && index >= 0)
	{	// Check if specific fragment has been allocated
		if(party->fragments[fragmentNum] != NULL)
		{	// Check if cell is UNUSED
			if(party->fragments[fragmentNum][cellNum] == UNUSED)
				return LPA_FAILURE;
				
			party->fragments[fragmentNum][cellNum] = UNUSED;
			party->size--;
			party->fragment_sizes[fragmentNum]--;
			// If removing that element caues the number of elements in fragment to be zero, free the whole fragment
			if(party->fragment_sizes[fragmentNum] == 0)
			{
				printf("-> Deallocated fragment %d. (capacity: %d, ", fragmentNum, party->fragment_length);
				printf("indices: %d..%d)\n", party->fragment_length * (fragmentNum), (party->fragment_length * (fragmentNum + 1)) - 1);
				free(party->fragments[fragmentNum]);
				party->num_active_fragments--;
				party->fragments[fragmentNum] = NULL;
			}
			return LPA_SUCCESS;
		}
		
	}
	else
	{
		printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, fragmentNum, cellNum);
	}
	return LPA_FAILURE;
}
// Checks if passed LonelyPartyArray contains the passed key in any of it's allocated fragments
int containsKey(LonelyPartyArray *party, int key)
{
		fflush(stdout);
	for(int i = 0; i < party->num_fragments; i++)
	{
		for(int j = 0; j < party->fragment_length; j++)
		{
			if(party->fragments[i] != NULL)
			{
				// if key is found, return 1
				if(key == party->fragments[i][j])
				return 1;
			}				
		}
	}
	return 0;
}
// Checks if the specificed index has a value set
int isSet(LonelyPartyArray *party, int index)
{
	int fragmentNum = index / party->fragment_length;
	int cellNum = index % party->fragment_length;
	
	if((party->fragments[fragmentNum] != NULL) &&  index < (party->num_fragments * (party->fragment_length - 1)) && index >= 0 && (party->fragments[fragmentNum][cellNum] != UNUSED))
		return 1;
	return 0;
}
// Prints out the value at the specificed index in the LonelyPartyArray
int printIfValid(LonelyPartyArray *party, int index)
{
	if(party == NULL)
		return LPA_FAILURE;
	int fragmentNum = index / party->fragment_length;
	int cellNum = index % party->fragment_length;
	int capacity = (party->num_fragments * party->fragment_length);
	if(index < capacity && index >= 0) 
	{
		if(party->fragments[fragmentNum] != NULL)
		{
			if(party->fragments[fragmentNum][cellNum] != UNUSED)
			{
				printf("%d\n", party->fragments[fragmentNum][cellNum]);
				return LPA_SUCCESS;
			}
		}
	}
	return LPA_FAILURE;
}
// Resets the LonelyPartyArray to the state when it was first created
LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
	if(party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().");
		return party;
	}
	for(int i = 0; i < party->num_fragments; i++)
	{
		party->fragment_sizes[i] = 0;
		if(party->fragments[i] != NULL)
			free(party->fragments[i]);
			// frees then sets each array in fragments to NULL
			party->fragments[i] = NULL;
	}
	
	party->size = 0;
	party->num_active_fragments = 0;
	printf("-> The LonelyPartyArray has returned to its nascent state. ");
	printf("(capacity: %d, fragments: %d)\n", (party->num_fragments * party->fragment_length), party->num_fragments);
	return party;
}
// Returns the size of the LonelyPartyArray
int getSize(LonelyPartyArray *party)
{
	if(party == NULL)
		return -1;
	return party->size;
}
// Returns the capacity of the LonelyPartyArray
int getCapacity(LonelyPartyArray *party)
{
	if(party == NULL)
		return -1;
	return party->num_fragments * party->fragment_length;
}
// Returns the number of cells that have been given a value (either UNUSED or other integers)
int getAllocatedCellCount(LonelyPartyArray *party)
{
	if(party == NULL)
		return -1;
	return party->num_active_fragments * party->fragment_length;
}
// Returns the number of bytes that an array would be if it had the same capacity as the passed LonelyPartyArray
long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
	if(party == NULL)
		return 0;
	return (long long unsigned int)(sizeof(int) * getCapacity(party));
}
// Returns the amount of bytes the passed LonelyPartyArray takes up
long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
	long long unsigned int byteCount = 0;
	if(party == NULL)
		return 0;
	
	// Add number of bytes a LonelyPartyArray pointer takes up
	byteCount += sizeof(LonelyPartyArray*);
	// Add number of bytes the LonelyPartyArray struct takes up
	byteCount += sizeof(LonelyPartyArray);
	// Add number of bytes the pointers in the fragments array takes up
	byteCount += (long long unsigned int)sizeof(int*) * party->num_fragments;
	// Add number of bytes  the fragment_sizes array takes up
	byteCount += (long long unsigned int)sizeof(int) * party->num_fragments;
	// Add number of bytes all the allocated integers in the all allocated fragments (including UNUSED cells)
	byteCount += (long long unsigned int)(sizeof(int) * party->fragment_length * party->num_active_fragments);
	return byteCount;
}
// Makes a seperate but equal copy of the passed LonelyPartyArray
LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
{
	if(party == NULL)
		return NULL;
	LonelyPartyArray *newLPA = malloc(sizeof(LonelyPartyArray));
	if(newLPA == NULL)
	{
		free(newLPA);
		return NULL;
	}
	// Set the various integers from the parent LonelyPartyArray to the new LonelyPartyArray	
	newLPA->size = party->size;
	newLPA->num_active_fragments = party->num_active_fragments;
	newLPA->fragment_length = party->fragment_length;
	newLPA->num_fragments = party->num_fragments;
	// Allocate the various arrays in the new LonelyPartyArray
	newLPA->fragments = malloc(sizeof(int*) * newLPA->num_fragments);
	newLPA->fragment_sizes = malloc(sizeof(int*) * newLPA->fragment_length);
	// Check if any calls to malloc failed, and free all newly created space
	if(newLPA->fragment_sizes == NULL)
	{
		free(newLPA->fragment_sizes);
		free(newLPA->fragments);
		free(newLPA);
		return NULL;
	}
	
	for(int i = 0; i < party->num_fragments; i++)
	{
		// Initializes all fragments to NULL
		newLPA->fragments[i] = NULL;
		if(party->fragments[i] != NULL)
		{
			// If parent LonelyPartyArray particular fragment was allocated, then also allocated the new LonelyPartyArray at the same fragment number
			newLPA->fragments[i] = malloc(sizeof(int) * newLPA->fragment_length);
			// Fill the newly created fragment with UNUSED values
			newLPA = fillUnused(newLPA, i);
			// If call to malloc failed,  free everything related to the new LonelyPartyArray
			if(newLPA->fragments[i] == NULL)
			{
				free(newLPA->fragments[i]);
				free(newLPA->fragments);
				free(newLPA->fragment_sizes);
				free(newLPA);
				return NULL;
			}
			for(int j = 0; j < newLPA->fragment_length; j++)
			{
				//Copy everything in allocated fragment of parent LonelyPartyArray to the allocated fragment of the new LonelyPartyArray
				newLPA->fragments[i][j] = party->fragments[i][j];
				// Because a new integer was placed in a fragment, increment the fragment_sizes array at that fragment by 1
				newLPA->fragment_sizes[i]++;
			}
		}
	}
	printf("-> Successfully cloned the LonelyPartyArray. ");
	printf("(capacity: %d, fragments: %d)\n", (newLPA->num_fragments * newLPA->fragment_length), newLPA->num_fragments);
	return newLPA;
	
}


double difficultyRating(void)
{
	return 4.0;
}

double hoursSpent(void)
{
	return 7.5;
}
