#include <stdio.h>
#include "genir.h"

extern int cfg[MAXIMUM_VERTICES][MAXIMUM_VERTICES];
extern int statements[MAXIMUM_VERTICES][NO_OF_STATEMENTS][12];
extern int final_num_state[MAXIMUM_VERTICES];
extern int defined_variables[MAXIMUM_VERTICES][MAXIMUM_VERTICES][3];
extern int used_variables[MAXIMUM_VERTICES][MAXIMUM_VERTICES][3];
extern int stack[MAXIMUM_VERTICES][MAXIMUM_VERTICES];
extern int idom[MAXIMUM_VERTICES];
extern int label_count ;

int count[MAXIMUM_VERTICES] = {};
int top[MAXIMUM_VERTICES] = {};


// This function stores the variables that are defined or used in each statements in the 
// matrices
void def_and_use()
{
	int i, j, k;
	for (i = 0; i < label_count; i++)				
	{
		for (j = 0; j < final_num_state[i]; j++)
		{
			switch (statements[i][j][0])
			{
				case 0:	
							if (statements[i][j][9] == 100)
							{
								for (k = 0; used_variables[i][j][k] != -1; k++);
								used_variables[i][j][k] = statements[i][j][10];
							}	
				case 1:
				case 2:
							if (statements[i][j][5] != -1)
							{
								for (k = 0; used_variables[i][j][k] != -1; k++);
								used_variables[i][j][k] = statements[i][j][6];
							}	
							
							for (k = 0; defined_variables[i][j][k] != -1; k++);
							defined_variables[i][j][k] = statements[i][j][2];
							break;	
							
				case 3:
				case 4:		
							k = 0;		
							if (statements[i][j][5] != -1)
								used_variables[i][j][k++] = statements[i][j][6];
							if (statements[i][j][9] != -1)
								used_variables[i][j][k] = statements[i][j][10];
							break;
							
				case 5:		
							defined_variables[i][j][0] = statements[i][j][2];
							for (k =0; k < statements[i][j][1]; k++)
								used_variables[i][j][k] = statements[i][j][2];
							break;
	
			}
		}
	}
}

// This function prints the resulted renamed Basic_blocks in Dotty format
void print_statement (int i, FILE *fp)
{
	int j, k, n;
	fprintf(fp, "%d[label = \"%d\\n", i, i);
	for (j = 0; j < final_num_state[i]; j++)
	{	
		switch (statements[i][j][0])
		{				
			case 3:
			case 4:
					 fprintf(fp, "if ");
		}
		if (statements[i][j][0] == 5)
		{
			fprintf(fp, "T %d (%d) = $ (T %d (%d), T %d (%d))", statements[i][j][2], statements[i][j][3], statements[i][j][4], statements[i][j][5], statements[i][j][6], statements[i][j][7]);
			fprintf(fp, "\\n");
		}
		else 
		{
		if (statements[i][j][0] == 0 || statements[i][j][0] == 1 || statements[i][j][0] == 2)
		
		{
			fprintf(fp, "T %d (%d) ", statements[i][j][2], statements[i][j][3]);
				
			switch (statements[i][j][4])
			{	
				case 1:		fprintf(fp, "+ ");
							break;
				case 2:		fprintf(fp, "- ");
							break;				
				case 3:		fprintf(fp, "* ");
							break;				
				case 4:		fprintf(fp, "/ ");
							break;				
				case 5:		fprintf(fp, "= ");
							break;				
				case 6:		fprintf(fp, "== ");
							break;				
				case 7:		fprintf(fp, "!= ");
							break;				
				case 8:		fprintf(fp, "< ");
							break;				
				case 9:		fprintf(fp, "<= ");
							break;				
				case 10:	fprintf(fp, "> ");
							break;
				case 11:	fprintf(fp, ">= ");
							break;								
			}
		}
		if (statements[i][j][0] == 1)
			fprintf(fp, "- ");
		if (statements[i][j][5] == -1)
			fprintf(fp, "constant ");
		else	
			fprintf(fp, "T %d (%d) ", statements[i][j][6], statements[i][j][7]);
			
		switch (statements[i][j][8])
		{	
			case 1:		fprintf(fp, "+ ");
						break;
			case 2:		fprintf(fp, "- ");
						break;				
			case 3:		fprintf(fp, "* ");
						break;				
			case 4:		fprintf(fp, "/ ");
						break;				
			case 5:		fprintf(fp, "= ");
						break;				
			case 6:		fprintf(fp, "== ");
						break;				
			case 7:		fprintf(fp, "!= ");
						break;				
			case 8:		fprintf(fp, "< ");
						break;				
			case 9:		fprintf(fp, "<= ");
						break;				
			case 10:	fprintf(fp, "> ");
						break;
			case 11:	fprintf(fp, ">= ");
						break;								
		}	
		if (statements[i][j][9] == -1)
			fprintf(fp, "constant ");
		else if(statements[i][j][9] == 100)		
			 	fprintf(fp, "T %d (%d)", statements[i][j][10], statements[i][j][11]);
		fprintf(fp, "\\n");
		}
	}	
	fprintf(fp, "\"]");
}


void renaming (int n)
{
	int i, j, k, x, y, a;
	
	for (j = 0; j < final_num_state[n]; j++)		// for each statement S in node n
	{
		if (statements[n][j][0] != 5)					// if S is not a 'phi' function
		{
			k = 0;
			while ((x = used_variables[n][j][k++]) != -1)// for each use of some variable x 
			{																	// in S
			
				i = stack[x][top[x]];						// i = top(stack[x])
				if (x == statements[n][j][6])
					statements[n][j][7] = i;				// replace the use of x with	
				else if (x == statements[n][j][10])			// x(i) in statement S
						statements[n][j][11] = i;			
			}
		}
		if ((a = defined_variables[n][j][0]) != -1)		// for each defination of some 
		{																// variable a in S
		
			count[a]++;										// i = ++count[a]
			i = count[a];
			stack[a][++top[a]] = i;							// push i to stack[a]
			statements[n][j][3] = i;						// replace defination of a into
																	// a(i) in S
		}	
	}	
	
	for (y = 0; y < label_count; y++)
	{
		if(cfg[n][y] == 1)							// for each successor y of block n
		{
			j = 1;
			for (k = 0; k < n; k++)
				if (cfg[k][y] == 1)						// suppose n is the j th predecessor
					j++;									// of y
					
			for (k = 0; statements[y][k][0] == 5; k++)	// for each 'phi' function in y
			{												// j th operand of 'phi' function
				a = statements[y][k][2*(j+1)];							// is a
				i = stack[a][top[a]];						// i = top(stack[a])
				statements[y][k][2*(j+1)+1] = i;			// replace j th operand with a(i)
			}		
		}
	}
	for (x = 0; x < label_count; x++)				//for each child i of n in Dominator tree
		if (idom[x] == n)
			renaming(x);								// renaming(i)
	for (i = 0; i < final_num_state[n]; i++)
	{												// for each defination of some variable a
		if ((a = defined_variables[n][i][0]) != -1)						// in S
			top[a]--;									// pop(stack[a])
	}
}
