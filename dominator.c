#include <stdio.h>
#include <stdlib.h>
#include "genir.h"

/* The lengauer-Tarjan Algorithm, as described in the book by Andrew W Appel (compiler implementation */

struct bucket_name				// The buckets is used to store the possible 
{								// candidates for the Semi dominator of the
	int info;					// given node
	struct bucket_name * next;
};
struct bucket_name * bucket[(MAXIMUM_VERTICES)];

extern int cfg[MAXIMUM_VERTICES][MAXIMUM_VERTICES];
extern int semi[MAXIMUM_VERTICES];
extern int idom[MAXIMUM_VERTICES];
extern label_count;


// These arrays are used for storing Basic_blocks respective attributes
int dfnum[MAXIMUM_VERTICES] = {},vertex[MAXIMUM_VERTICES] = {},parent[MAXIMUM_VERTICES] = {};
int ancestor[MAXIMUM_VERTICES], samedom[MAXIMUM_VERTICES];

int N;

/* This function sets the DF_num for all the Basic_blocks in the CFG*/

static void dfs (int p, int n) 
{

	int i;
	if (dfnum[n] == UNVISITED ) 
	{
		dfnum[n] = N;			// Sets the DF_num of 'n' to N and the parent[n] to 'p'
		vertex[N-1] = n;
		parent[n] = p;
		N++;
		for (i = 0;i < label_count;i++) // For each successor 'i' of node 'n' that are not 
			if (cfg[n][i] == 1)			// visited go for dfs(n,i)
				dfs(n, i);
	}	
}

// This function returns the ancestor 'u' of the given node 'v' that has the Semi dominator
// of lowest DF_num

int ancestorWithLowestSemidominator (int v) 
{
	int u = v;
	while (ancestor [v] != NONE) 
	{
		if (dfnum[semi[v]] < dfnum[semi[u]])
			u = v;
		v = ancestor[v];		
	}
	return u;
}


// This function just attaches the node 'p' as the ancestor of 'n'

void link (int p, int n)
{
	ancestor[n] = p;
}


// Inserts node 'n' into the bucket of 's'

void bucket_insert(int s, int n)
{
	struct bucket_name * p, * q, *r;
	r = malloc(sizeof(struct bucket_name));
	r->info = n;
	r->next = NULL;
	p = q = bucket[s];
	while (p != NULL)
	{
		q = p;
		p = p->next;
	}	
	if (p == q)
		bucket[s] = r;
	else
	 	q->next = r;
}


// This function builds the Dominator tree
int build_dominator_tree () 
{
	int i, j, n, p, r, s, y;
	struct bucket_name * b;
	N = 1;
	for (i = 0; i < label_count; i++)
	{
		semi[i] = ancestor[i] = idom[i] = samedom[i] = -1;// Some initialisations
	}
	
	dfs(NONE,0);		// DFS visit from the 'root' node
	
	for (i = N-1; i > 0; i--)
	{
		n = vertex[i];
		p = parent[n];
		s = p;
		for (j = 0;(j < label_count); j++)
		{															// These lines calculate
			if (cfg[j][n] == 1)										//	the semidomintor
			{														// of 'n' by using the
				if (dfnum[j] <= dfnum[n])							// 'Semi dominator 
					r = j;											// Theorem'
				else												
					r = semi[ancestorWithLowestSemidominator(j)];	
				if (dfnum[r] < dfnum[s])
					s = r;
			}
		}
		semi[n] = s;				// Inserts node 'n' into the bucket[s] as
		bucket_insert(s,n);			// node 's' semidominates 'n'
		link(p,n);		
		b = bucket[p];
		for (; b != NULL; b = b->next)				
		{											// These lines calculates the Dominator
			j = b->info;
			y = ancestorWithLowestSemidominator(j);	// of 'i' based on the first clause
			if (semi[y] == semi[j])
				idom[j] = p;						// of the Dominator Theorem
			else
			 	samedom[j] = y;			 					 	
		}
		bucket[p] = NULL;
	}	
	for (i = 1; i < N; i++)							
	{												// The second clause of the Dominator
		n = vertex[i];
		if (samedom[n] != NONE)						// Theorem is implemented here
			idom[n] = idom[samedom[n]];
	}
	return 0; 
}
