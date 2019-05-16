/********************************************
*	Author: Nicolas Leo                     *
*	PeopleSoft: 3466098                     *
*	Class: CS 0449                          *
*	Project 2: mystrings                    *
********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

	FILE *fp;			//Hold file pointer for later use

	char buffer[4] = {'\0', '\0', '\0', '\0'}; //Buffer to hold 1st 4 chars of a word
	char ch; 			//Hold a single character
	int i,				//Counter variable used in for loops
		numchars=0, 	//Number of chars in the string 
		numwords = 0;	// Total number of words read by the program

	if (argc < 2) //Check for minimum # of arguments (2: program & file name)
	{
	      printf("Please enter a valid filename.\n");
	      exit (1);
	}

	fp = fopen(argv[1], "rb"); // Open fp for reading

	//If error opening file
	if(fp == NULL)
	{
		printf("Invalid filename entered.\n");
		return 0;
	}

	while(!feof(fp))
	{
		ch = fgetc(fp); //Get next character from the file

		//Check if it's a valid printable character ASCII [32,126]
		if (31 < ch && ch < 127)
		{
			if (numchars > 3)
			{
				printf("%c", ch);
			}
			else if (numchars == 3)
			{
				printf("%s%c", buffer, ch );
			}
			else
				buffer[numchars] = ch;

			numchars++;
		}
		else //Else not a printable character
		{
			if (numchars > 3)
			{
				printf("\n");
				numwords++;
			}

			numchars = 0;
		}
	}
	
	printf("\n");
	
	//Tell the user how many words were found
	for (i = 0; i < 55; ++i)
		printf("-");

	printf("\nRead %d words\n\n", numwords);

	fclose(fp);
	return 0;
}
