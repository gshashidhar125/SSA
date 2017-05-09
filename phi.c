#include <stdio.h>
#include "genir.h"

extern int cfg[MAXIMUM_VERTICES][MAXIMUM_VERTICES];
extern int statements[MAXIMUM_VERTICES][NO_OF_STATEMENTS][12];
extern int s[MAXIMUM_VERTICES][NO_OF_STATEMENTS][12];
extern int final_num_state[MAXIMUM_VERTICES];
extern int num_of_statements[MAXIMUM_VERTICES];
extern int DF[MAXIMUM_VERTICES][MAXIMUM_VERTICES];
extern int Aorig[MAXIMUM_VERTICES][MAXIMUM_VERTICES];
extern int phi_statements[MAXIMUM_VERTICES][MAXIMUM_VERTICES];
extern int variables[MAXIMUM_VERTICES];
extern int label_count ;

int defsites[MAXIMUM_VERTICES][MAXIMUM_VERTICES];	// These definations are used in having
int def_num[MAXIMUM_VERTICES] = {};					// the nodes(Basic_blocks) where each
													// variables are defined

int Aphi[MAXIMUM_VERTICES][MAXIMUM_VERTICES];	// This matrix holds the set of variables
												// that must have 'phi' function at some node


// Inserts the 'phi' functions
void insert_phi ()
{
	int w[MAXIMUM_VERTICES], i, j, k, l, a, m, n, y;
	FILE *fp;
	fp = fopen( "phi.dot", "w" );
	for (i = 0; i < MAXIMUM_VERTICES; i++)
	{		
		w[i] = NONE;
		for (j = 0; j < MAXIMUM_VERTICES; j++)		
		{											
			phi_statements[i][j] = NONE;			// Some initializations
			Aphi [i][j] = NONE;			
			defsites [i][j] = NONE;
		}	
	}		
	j = 0;		
	for (i = 0; i < label_count; i++)			// For each node 'n'
		for (k = 0; (a = Aorig[i][k]) != -1; k++)// 	For each variable 'a'defined in 'n'
			defsites[a][def_num[a]++] = i;		//		defsites[a] = defsites[a] U {a}



	for (i = 0; (a = variables[i]) != -1; i++)	// For each variable 'a'
	{	
		for (j = 0; j < MAXIMUM_VERTICES; j++)	
			w[j] = NONE;						
		j = 0;
		while (defsites[a][j] != -1)		
			w[j] = defsites[a][j++];			// W = defsites[a]
			
		for(j = 0; (n = w[j]) != -1; j++)		// while W is not empty
		{
			k = 0;								//		for some node 'n' from w
			while ((y = DF[n][k++]) != -1)
			{									//			for each 'y' in DF[n]
				l = 0;
				while ((m = Aphi[y][l++]) != -1)
				{
					if (m == a)					//				if a does not belong to A$[y]
					{
						l = -1;
						break; 
					}
				}				
				if (l > -1)		
				{
					for (l = 0; phi_statements[y][l] != -1; l++);// insert a = $(a,a,a,...)
					phi_statements[y][l] = a;		 		// statement in top of block 'y'

					for (l = 0; Aphi[y][l] != -1; l++);
					Aphi[y][l] = a;							// A$[y] = A$[y] U {a}
					
					for (l = 0; Aorig[y][l] != -1; l++)
					{
						if (Aorig[y][l] == a)
						{
							l = -1;
							break;
						}
					}	
					if (l > -1)					// if y does not belong to Aorig[n]
						w[j--] = y;				// 		W = W U {y}
				}	
			}	
		}			
	}
	
	
	fprintf(fp, "digraph cfg {\n");
	for (i = 0; i < label_count; i++)
	{												// Dumps the statements including
		for (j = 0; j < label_count; j++)			
		{											// 'phi' functions into a file in
			if (cfg[i][j] == 1)
				fprintf (fp, "%d -> %d \n", i, j);	// Dot format to be viewed from 'Dotty'
		}
		make_statement (i, fp, 1);		
	}
	
	fprintf(fp, "%d[shape = \"box\" label = \"Phi Function Placement\\n\"]", i);
	fprintf (fp, "}\n");
	
	for (i = 0; i < label_count; i++)
	{
		for (j = 0; j < num_of_statements[i]; j++)	// Resets the number of statements
		{
			for (k = 0; k < 12; k++)				// in each Basic_block including the
				statements[i][j + final_num_state[i]][k] = s[i][j][k];
		}											// 'phi' functions
		final_num_state[i] += num_of_statements[i];
	}
}
