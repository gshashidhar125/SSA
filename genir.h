#include <stdio.h>

#define MAX_DEPTH 2
#define NEST_SIZE_MAX 5  // A random size b/w MAX and MIN is picked for the size of nested
#define NEST_SIZE_MIN 2  // statements
#define NO_OF_STATEMENTS 1000
#define MAXIMUM_VERTICES 1000	// The maximum number of Basic_blocks that can be generated
#define RANDOM_VALUE 20

#define NONE -1			// These definations are used in some initializations
#define UNVISITED 0		// and in generating the DF_num of each Basic_blocks

void genir_random ( int current_depth, int required_generations);
