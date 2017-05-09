#include <stdio.h>
#include <stdlib.h>
#include "genir.h"
 
extern int cfg[MAXIMUM_VERTICES][MAXIMUM_VERTICES];
extern int idom[MAXIMUM_VERTICES];
extern int label_count;

extern int DF[MAXIMUM_VERTICES][MAXIMUM_VERTICES];


void insert (int s[MAXIMUM_VERTICES], int w, int *j)
{
	int i = 0, flag = 0;				// This function inserts the element 'w' 
				
	while (s[i] != NONE)				// to the set 's' which contains the Dominance
	{
		if (s[i] == w)					// Frontiers for a given node
		{
			flag = 1;
			break;
		}
		i++;
	}
	if (flag == 0)
		s[(*j)++] = w;
}
void computeDF (int n)
{
	int s[MAXIMUM_VERTICES];
	int i = 0;
	int y, c, w, x;
	
	for (y = 0; y < label_count; y++)
		s[y] = NONE;					// S = {}
		
	for (y = 0; y < label_count; y++)	// This loop computes DF_local[n]
		if (cfg[n][y] == 1)
			if (idom[y] != n)			// Nodes in succession that are not dominated by 'n'
				insert (s, y, &i);						
				
	for (c = 0; c < label_count; c++)	// This loop calculates DF_up[n]
		if (idom[c] == n)				
		{								// Nodes that are children of the node 'n' in the
			computeDF (c);								// Dominator tree
			for (y = 0; y < label_count; y++)			
				if ( DF[c][y] != NONE)
				{	
					w = DF[c][y];
					if ( idom[w] != n || n == w)					
						insert (s, w, &i);												
				}					
		}	
	for (c = 0; c < label_count; c++)
		DF[n][c] = s[c];
}
