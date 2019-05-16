/********************************************
*	Author: Nicolas Leo						*
*	PeopleSoft: 3466098						*
*	Class: CS 0449, Summer 2018				*
*	Project 1: Blackjack game				*
********************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char const * RANK [] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", 
						"Queen", "King"};

int hand_total(int []);
void print_hand(int [], int);

int main(int argc, char const *argv[])
{
	/* 
		2 arrays are used to hold the number of each type of card that a player or dealer 
		is dealt. [0] = Aces, [1] = 2's, [2] = 3's, ... [9] = 10's, [10] = Jack's, 
		[11] = Queens, [12] = Kings 
	*/
	int i, //Variable used for loop counters
		player [] = {0,0,0,0,0,0,0,0,0,0,0,0,0}, //Each element holds # of each type of card
		dealer [] = {0,0,0,0,0,0,0,0,0,0,0,0,0},
		face_down = 0; //Value of the face down card


	srand((unsigned int)time(NULL)); //Seed the random # generator

	//Player & dealer dealt 2 cards each
	player[rand() % 13]+=1;
	face_down = rand() % 13; 	//First card of the dealer is face down
	dealer[face_down]+=1;		//Add face down card to the dealer's hand
	player[rand() % 13]+=1;
	dealer[rand() % 13]+=1;

	printf("\033[H\033[2J"); //clears the console window
	printf("Welcome to the blackjack table!\n");
	printf("Please have a seat and let's begin the game.\n\n");
	
	printf("Dealing the first hand.\n\n");

	printf("The dealer:\n");
	print_hand(dealer, face_down);
	
	printf("\nYou:\n");
	print_hand(player, -1);


	/*
		The following if-else block is the player and dealer decision structure.

		After the initial 2 cards are dealt, if neither the player or the dealer 
		has blackjack (and there isn't a tie), then the player will be offered a choice of 
		hitting another card or staying. If the player busts, then the dealer 
		automatically wins. 
		
		After the player takes his or her turn, it's the dealer's turn.If the dealer's 
		score is <17, then the dealer will draw cards until the score >16. If the dealer 
		busts, then the player automatically wins.

		After drawing cards, if scores for both "players" are between [17,21],
		then the program will compare the hands to determine who the winner is,
		or if there is a tie.
	*/

	//Game decision structure

	int tempC,							  // Value of temporary card
		dealer_hand = hand_total(dealer), // Value of dealer's hand
		player_hand = hand_total(player); // Value of player's hand

	char player_response [10]; // Hit or stand

	// Check for Blackjack
	if (dealer_hand  == 21 || player_hand == 21)
	{
		printf("\nDealer's hand: ");
		print_hand(dealer, -1);
		printf("\nResult: ");


		if(dealer_hand == player_hand)
		{
			printf("Push!\n");
		}
		else 
		{
			printf("Blackjack!\n");
			if (dealer_hand == 21)
				printf("Dealer wins!\n");
			else
				printf("You win!\n");
		}
	}
	else // No Blackjack, game continues
	{
		do // Ask user if her or she would like to hit or stand
		{
			printf("\nWould you like to \"hit\" or \"stand?\" ");
			scanf("%s", player_response);

			tempC = rand() % 13; 	// Dealer draws a card from the shoe
			player[tempC]+=1;		// Card presented to the player

			if (player_response[0] == 'h' || player_response[0] == 'H')
			{
				printf("\nYou hit: %s\n", RANK[tempC]);
				player_hand = hand_total(player);
				printf("Your new hand total = %d\n", player_hand);
			}
		} while (player_hand < 21 && 
			(player_response[0] == 'h' ||player_response[0] == 'H'));


		if (player_hand > 21) //Player busts
		{
			printf("You BUSTED!\n\n");
			printf("Dealer's hand: ");
			print_hand(dealer, -1);
			printf("\nResult: Dealer Wins!\n");
		}
		else if (dealer_hand >= 17) //Player stands & dealer check's hand
		{								  //Both stand.
			printf("\n    You STAND: %d\n", player_hand);
			printf("Dealer STANDS: ");
			print_hand(dealer, -1);
			printf("\nResult: ");

			if (dealer_hand == player_hand) //Tie/Push
			{
				printf("Push!\n");
			}
			else 
			{
				if (player_hand>dealer_hand) //Player has larger value
					printf("You win!\n");
				else //Dealer value > player's value. Dealer won.	 
					printf("Dealer wins!\n");
			}
		}
		else //Player stands. Dealer's hand < 17, game continues
		{
			printf("You STAND: %d\n", player_hand);

			while (dealer_hand < 17)
			{
				tempC = rand() % 13; 	//Dealer draws a card from the shoe
				dealer[tempC]+=1;		//Card put in the dealer's hand face up
				dealer_hand = hand_total(dealer);

				printf("\nDealer hits: %s", RANK[tempC]);
			}

			if (dealer_hand > 21)//Dealer busts
			{
				printf("\n\nDealer BUSTS: ");
				print_hand(dealer, -1);
				printf("\nResult: You Win!\n");
			}
			else  //Dealer stands & hands are compared.
			{	
				printf("\n\nDealer STANDS: ");
				print_hand(dealer, -1);

				printf("\nResult: ");

				if (player_hand == dealer_hand) //Tie
				{
					printf("Push!\n");
				}
				else
				{
					if (dealer_hand > player_hand) //Dealer wins
						printf("Dealer wins!\n");
					else //Player wins
						printf("You win!\n");
				}
			} 
		} 
	}//End of game decision structure


	return 0;
}

/* 
	Calculates the total value of the hand.
	@param hand Integer array of the player's hand.
	@return Integer representing the total value of the hand
*/
int hand_total(int hand[])
{
	int	temp = 0; //Holds total value of the hand

	temp += hand[1]*2;		// # of 2's
	temp += hand[2]*3;		// # of 3's
	temp += hand[3]*4;		// # of 4's
	temp += hand[4]*5;		// # of 5's
	temp += hand[5]*6;		// # of 6's
	temp += hand[6]*7;		// # of 7's
	temp += hand[7]*8;		// # of 8's
	temp += hand[8]*9;		// # of 9's
	temp += hand[9]*10;		// # of 10's
	temp += hand[10]*10;	// # of Jacks
	temp += hand[11]*10;	// # of Queens
	temp += hand[12]*10;	// # of Kings
	//Temp will be 0 if all cards in the hand are Aces

	if (temp == 0) //This means blackjack since the hand is 2 Aces
		return 12; //Otherwise, the hand total must be calculated

	//If there is at least 1 Ace, then see if it is high or low
	if (hand[0] > 0) 
	{ 	
		if(temp + 11 + (hand[0] - 1) <= 21) //Keep 1 ace high & see if it causes bust
			temp = temp + 11 + (hand[0] - 1); //Temp = 0 if all cards are aces
		else //Keep all aces low, regardless if it causes the hand to bust
			temp += hand[0]; 
	}	

	return temp;
}


/* 
	Prints the cards in the hand, hiding one card if printing the dealer's hand.
	@param hand Integer array of the player's hand.
	@param face_down The value of the face down card in the dealers hand. Pass -1 if
			printing a player's hand.
*/
void print_hand(int hand[], int face_down)
{
	if(face_down > -1) //Printing dealer's hand
	{
		printf("? + ");
		hand[face_down]-=1; //Temporarily remove the face down card from the hand
	}

	int i, 			//Counter
		j, 			//Counter
		total = 0;	//Total number of cards in the hand, will be used to print + symbols
		
	for (i = 0; i < 13; ++i)//Count total number of cards in hard
		total += hand[i]; 

	total--; //There will be 1 less + symbol for every card in the hand

	//printf("total = %d\n", total);
	for (i = 0; i < 13; ++i)
	{
		for (j = 0; j < hand[i]; ++j)
		{
			printf("%s ", RANK[i]);
			if (total > 0) //If there are any cards left to print
				printf("+ ");

			total--; //Decrement for each card printed 
		}
	}

	if(face_down > -1) 		//If printing dealer's hand
	{
		hand[face_down]+=1; //Replace the face down card into the hand
		printf("\n");
	}
	else //print total if printing player's hand
		printf("= %d\n", hand_total(hand));
}