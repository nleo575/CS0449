/****************************************************************
*	Author: Nicolas Leo                    						*
*	Class: CS 0449, 8/3/18                						*
*	Project 4: Craps 				        					*
*	Description: Craps game using simulated	6-sided dice device	*
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

char roll(int fd); 		//Function signature for rolling the dice

int main() 
{
	char name[100],		// Player's name
		 response[8],	// Player's choice to play, quit, or play again
		 play = 1,		// "Boolean" to tell whether to play or not
		 loop = 1,		// "Boolean" to tell whether valid input was given. 
		 die [2],		// 2 element array to hold the results of throwing 2 dice
 		 total = 0;		// Sum of the two dices' values

	int dicefd = open("/dev/dice", O_RDONLY);	// File descriptor for the dice device
	// Opens the device as read only

	if(dicefd == -1) //Check if the device properly opened
	{
		puts("Unable to open the dice device. Now exiting.");
		return -1;
	}

	puts("Welcome to Nic's Casino!");
	printf("Please enter your name: ");
	scanf("%s", name);
	printf("%s, would you like to play or quit? ", name);

	while(loop) 
	{
		scanf("%s", response);

		// Verify correct response or loop until the user enters a valid response
		if(strcmpicase(response, "play") == 0)
		{
			loop = 0; play = 1;
		}
		else if(strcmpicase(response, "quit") == 0)
		{
			loop = 0; play = 0;
		}
		else
			printf("Invalid input! Please enter \"play\" or \"quit\": ");
	}

	// Main game loop.
	while(play) 
	{
		read(dicefd, die, 2, 0L);	// Throw the die
		total = die[0] + die[1];	// Calculate the sum
		
		printf("\nYou rolled %d + %d = %u\n", die[0], die[1], total);
		
		// Check if the player lost, won, or will continue to play
		if(total == 7 || total == 11) 
			puts("You Win!");
		else if(total == 2 || total == 3 || total == 12)
			puts("You Lost!");
		else //Otherwise the player didn't win or lose
		{
			printf("\nThe point is %d\n", total);
			loop = 1;
			char point = total;
			while(loop) 
			{
				read(dicefd, die, 2, 0L);	// Throw the dice
				total = die[0] + die[1];	// Calculate the sum
				
				printf("\nYou rolled %d + %d = %u\n", die[0], die[1], total);
				
				if(total == 7) 
				{
					puts("You Lost!"); loop = 0;
				}
				else if(point == total) 
				{
					puts("You Win!"); loop = 0;
				}
				else
					puts("Rolling again.");
			}	
		}
		
		loop = 1;
		printf("\nWould you like to play again? ");

		// Verify correct response or loop until the user enters a valid response
		while(loop) 
		{
			scanf("%s", response);
			if(strcmpicase(response, "yes") == 0)
				loop = 0;
			else if (strcmpicase(response, "no") == 0)
			{
				loop = 0; play = 0;
			}
			else
				printf("Invalid input! Please enter \"yes\" or \"no\": ");
		}
	} // End of game loop
	
	printf("\nGoodbye, %s!\n", name);
	close(dicefd);
	return 0;
}

/*
	Compares two strings & ignores case.
*/
int strcmpicase(char * str1, char * str2)
{
	if (str1 == str2) // Check if the points are equal
		return 0;

	int z; unsigned char c1, c2;
	while(*str1 && *str2) // Check string until 1 or both chars is null \0
	{
		if(96 < *str1 && *str1 < 123) 	// Check if char is lower case
			c1 = *str1 - 32; 			// Change the char to upper case
		else
			c1 = *str1;

		if(96 < *str2 && *str2 < 123)	// Check if char is lower case
			c2 = *str2 - 32; 			// Change the char to upper case
		else 
			c2 = *str2;

		z = c1 - c2; // Calculate difference to see order or if they're equal
		if (z == 0)
		{
			str1++; str2++; // Check next chars
		}
		else 
			return z; // Not 0 means that one word comes before the other
	}

	return *str1 - *str2; // Otherwise, comparison is only based on the last char
}