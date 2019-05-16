#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

struct Node
{
	int grade;
	struct Node *next;
};

int main(int argc, char const *argv[])
{
	printf("starting value of brk: %p\n\n", sbrk(0));
	int grade = -1,					//Grade that the user enters
		num_grades = 0; 			//Total number of grades entered
	float	sum_grades = 0; 		//Sum of the grades
	struct Node root = {0,NULL}, 	//Root node
				*temp_node = NULL;	//Temporary node to traverse the list & add new nodes
	printf("Please enter some grades to calculate the mean.\n");
	printf("When you are finished, enter -1.\n\n");
	printf("Enter the 1st grade: ");
	scanf("%d", &grade);

	if (grade != -1) //Only continue if user entered a value other than -1
	{
		root.grade = grade;
		temp_node = &root; //Node pointer used to modify the linked list

		printf("Next grade: ");
		scanf("%d", &grade);

		while (grade != -1)
		{
			//Create a new node & link to end of the list
			temp_node -> next = (struct Node *)my_malloc(sizeof(struct Node)); 
			printf("current value of brk: %p\n", sbrk(0));
			temp_node = temp_node -> next; 	//Move pointer to the new node
			temp_node -> grade = grade;		//Set the grade that the user provided
			temp_node -> next = NULL;		//Set .next of the current node to null

			printf("Next grade: ");			
			scanf("%d", &grade);
		}	
	}

	if (root.next == NULL) //Avoid free() and division by 0 errors.
	{
		printf("\nMean grade = %d\n", root.grade);
	}
	else	//Calculate the mean and free malloc'd memory
	{
		sum_grades = root.grade;
		num_grades = 1;


		temp_node = &root;	//Reassign temp to point to root node
		struct Node *tnode2 = root.next; //Will hold pointer to next node

		while (tnode2) //.next will be NULL/0 at the end of the list
		{
			temp_node = tnode2;			//Set temp to next node
			tnode2 = temp_node -> next;	

			sum_grades += temp_node -> grade;
			num_grades++;

			my_free(temp_node);
		}
		tnode2 = NULL;
		printf("\nMean grade = %0.2f\n", num_grades >0? sum_grades/num_grades: 0);
	}
	printf("ending value of brk: %p\n", sbrk(0));
	return 0;
}