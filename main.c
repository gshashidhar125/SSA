#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genir.h"
#include<time.h>

int cfg[MAXIMUM_VERTICES][MAXIMUM_VERTICES] = {};	// This is the adjacency matrix for
													// Basic_blocks

int label_count = 0;		// This count value is used in the generation of the Basic_blocks

int statements[MAXIMUM_VERTICES][NO_OF_STATEMENTS][12] = {};// These 3-dimensional matrices
int s[MAXIMUM_VERTICES][NO_OF_STATEMENTS][12] = {};			// are used to store the 
															// individual statements in each
															// Basic_block


int num_of_statements[MAXIMUM_VERTICES] = {};	// These arrays store the number of
int final_num_state[MAXIMUM_VERTICES] = {};		// statements in each Basic_block before and
												// after 'phi' statements are inserted

int variables[MAXIMUM_VERTICES];// This array holds all the variables geneated by the
								// genir_random()

int semi[MAXIMUM_VERTICES];		// These arrays hold the Semi_dominator and Dominator
int idom[MAXIMUM_VERTICES];		// of each Basic_block in the CFG

int DF[MAXIMUM_VERTICES][MAXIMUM_VERTICES]; // This holds the Dominance Frontiers of each 
											// Basic_block

int Aorig[MAXIMUM_VERTICES][MAXIMUM_VERTICES];  // This matrix has the variables that are
												// defined at each Basic_block

int phi_statements[MAXIMUM_VERTICES][MAXIMUM_VERTICES]; // This holds the set of variables
														// in each Basic_block that must
														// have 'phi' functions

int defined_variables[MAXIMUM_VERTICES][MAXIMUM_VERTICES][3];// These matrices have variables
int used_variables[MAXIMUM_VERTICES][MAXIMUM_VERTICES][3];	// that are defined and used
															// in each statements of each
															// Basic_block

int stack[MAXIMUM_VERTICES][MAXIMUM_VERTICES] = {};	// This is the stack for each variable


// This function is used to print the Basic_blocks in the 'Dotty' output format

void make_statement (int i, FILE *fp, int ch)
{
	int j, k, n;								// Here 'ch' is used as conditional flag
	fprintf(fp, "%d[label = \"%d\\n", i, i);	// in Switch statement, so that when it has
	switch(ch)									// the value '1' the functon prints the whole
	{											// Basic_blocks including the 'phi' statement
		case 1:									// and when it is '2' it excludes the 'phi'
				for (j = 0; phi_statements[i][j] != -1; j++)// statements. Hence 2 calls, one
				{								// for the CFG, and other for the full 'phi'
					n = 0;						// functions included CFG are made.
					if (i == 0)
						n = 1;
					for (k = 0; k < label_count; k++)
						if (cfg[k][i] == 1)
							n++;
					fprintf(fp, "T %d = $( ", phi_statements[i][j]);		
					for (k = 0; k < n; k++)
						fprintf(fp, "T %d, ", phi_statements[i][j]);
					fprintf(fp, ")\\n");		
					
					statements[i][j][0] = 5;
					statements[i][j][1] = n;
					statements[i][j][2] = statements[i][j][4] = phi_statements[i][j];
					statements[i][j][6] = statements[i][j][8] = phi_statements[i][j];	
					final_num_state[i]++;
				}
		case 2:		
				for (j = 0; j < num_of_statements[i]; j++)
				{	
					switch (s[i][j][0])
					{	
						case 0:
						case 1:	
						case 2:	
								 for (k = 0; Aorig[i][k] != -1; k++)
								 {
								 	if (Aorig[i][k] == s[i][j][2])
								 	{
								 		k = -1;
								 		break;
								 	}
								 }
								 if (k != -1)	// for all variables 'a' defined in S[i][j]
									 	Aorig[i][k] = s[i][j][2];// Aorig[i] = Aorig[i] U {a}
								 break;
						case 3:
						case 4:
								 fprintf(fp, "if ");
					}
					if (s[i][j][0] == 0 || s[i][j][0] == 1 || s[i][j][0] == 2)
					{
						for (k = 0; variables[k] != -1; k++)
						{
						 	if (variables[k] == s[i][j][2])
						 	{
						 		k = -1;
						 		break;
						 	}
						}
						if (k != -1)
							variables[k] = s[i][j][2];	// Adds variable to the set
	
						fprintf(fp, "T %d ", s[i][j][2]);
						switch (s[i][j][4])
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
					if (s[i][j][0] == 1)
						fprintf(fp, "- ");
					if (s[i][j][5] == -1)
						fprintf(fp, "constant ");
					else	
					{	
						for (k = 0; variables[k] != -1; k++)
						{
						 	if (variables[k] == s[i][j][6])
						 	{
						 		k = -1;
						 		break;
						 	}
						}
						if (k != -1)
							variables[k] = s[i][j][6];	// Adds variable to the set

						fprintf(fp, "T %d ", s[i][j][6]);		
					}	
	
					switch (s[i][j][8])
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
					if (s[i][j][9] == -1)
						fprintf(fp, "constant ");
					else if(s[i][j][9] == 100)		
						{
							for (k = 0; variables[k] != -1; k++)
							{
						 		if (variables[k] == s[i][j][10])
						 		{
						 			k = -1;
						 			break;
						 		}
							}
							if (k != -1)
								variables[k] = s[i][j][10];	// Adds variable to the set

						 	fprintf(fp, "T %d ", s[i][j][10]);				
						 }	
					fprintf(fp, "\\n");
				}	
	}	
	fprintf(fp, "\"]");
}	

/*Function that write the output of the adjacency matrix to "cfg.dot" for viewing it in 'dotty'*/

void print_dot_input_cfg ( const char *name) 
{
	int i, j;
	FILE *fp;
	fp = fopen( name, "w" );
	printf("\nNumber of Basic Blocks produced is = %d\n", label_count);
	fprintf (fp, "digraph cfg {\n");
	for (i = 0; i < label_count; i++)
	{
		for (j = 0; j < label_count; j++)
		{	

			if (cfg[i][j] == 1)
				fprintf (fp, "%d -> %d \n", i, j);
		}
		make_statement (i, fp, 2);	// Makes call to print the statements in Basic_block 'i'
	}	
	fprintf(fp, "%d[shape = \"box\" label = \"Control Flow Graph\\n\"]", i);
	fprintf (fp, "}\n");
}

/* Functon to print the matrix in 'Dotty' format*/
void print_dot_input_matrix ( int x[MAXIMUM_VERTICES], const char *name) 
{
	int i, j;
	FILE *fp;
	fp = fopen(name, "w");
	fprintf (fp, "digraph cfg {\n");
	for (j = 0; j < label_count; j++)
		if (x[j] > -1)
			fprintf (fp, "%d -> %d\n", x[j], j);
	if (!strcmp(name, "domi.dot"))
		fprintf(fp, "%d[shape = \"box\" label = \"Dominator Tree\\n\"]", j);
	else
		fprintf(fp, "%d[shape = \"box\" label = \"Semi Dominator Tree\\n\"]", j);
	fprintf (fp, "}\n");
}

int main (int argc, char *argv[]) {
		
	int i, j, k;
	FILE *fp;
	if (argc != 2) {
		printf ("Usage: %s number_of_statements (per nesting).\n", argv[0]);
		exit (1);
	}
	
	srand (time (NULL));
	
	genir_random ( 0, atoi (argv[1]));

	label_count++;
	
	for (i = 0; i < MAXIMUM_VERTICES; i++)
	{
		variables[i] = NONE;
		for (j = 0; j < MAXIMUM_VERTICES; j++)
		{
			Aorig[i][j] = NONE;
			DF[i][j] = NONE;
		}
	}	
		printf("\n");
	
	print_dot_input_cfg("cfg.dot");
	
	i = build_dominator_tree();
	
	print_dot_input_matrix( semi, "semi.dot");
	print_dot_input_matrix( idom, "domi.dot");
		
		
	printf("\n");		    
	computeDF( 0);	   
	
	fp = fopen("DominanceFrontier.dot", "w");
	
	fprintf (fp, "digraph cfg {\n");
	
	for (i = 0; i < label_count; i++) {
		
		fprintf(fp, "%d[label = \"Basic Block %d\\n", i,i);
		for (j = 0; DF[i][j] != NONE; j++) 
			fprintf(fp, "%d ", DF[i][j]);
		
		fprintf(fp, "\"]\n");
	}
	fprintf(fp, "%d[shape = \"box\" label = \"Dominance Frontier for each Basic block\\n\"]", i);
	fprintf(fp, "}");

	insert_phi();	
	
	for (i = 0; i < MAXIMUM_VERTICES; i++)
		for (j = 0; j < MAXIMUM_VERTICES; j++)
		{
			stack[i][j] = NONE;
			for (k = 0; k < 3; k++)
			{
				defined_variables[i][j][k] = NONE;
				used_variables[i][j][k] = NONE;			
			}
		}	
	for (j = 0; j < MAXIMUM_VERTICES; j++)
		stack[j][0] = 0;	
		
	def_and_use();
	renaming(0);
	
	fp = fopen("rename.dot","w");
	fprintf(fp,"digraph cfg{\n");
	for (i = 0; i < label_count; i++)
	{
		for (j = 0; j < label_count; j++)
		{	
			if (cfg[i][j] == 1)
				fprintf (fp, "%d -> %d \n", i, j);
		}
		print_statement (i, fp);		
	}
	fprintf(fp, "%d[shape = \"box\" label = \"Renaming Variables\\n\"]", i);
	fprintf (fp, "}\n");
	return 0;
}
