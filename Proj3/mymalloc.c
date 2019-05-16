#include <unistd.h>
#include <stdio.h>
typedef struct node 
{
	unsigned int free;		// 0 = segment is in use, 1 = segment is free
							// used int instead of char to keep word alignment 
	unsigned int data_size;	// Size (bytes) of the proceeding data segment
	struct node *prev;		// Pointer to the previous node
	struct node *next;		// Pointer to the next node
} Node;

static Node *root = NULL, // Pointer to the 1st node
			*last = NULL, // Pointer to the last node
			*tempN = NULL,// Temporary node pointer used in functions
			*best;		  // Best fitting node

static int cont; //Indicates whether to continue or stop iterating through the list


/*
	my_malloc implements the best fit algorithm for dynamic memory management.
	@param size Integer that represents the number of bytes the user wishes to allocate
	@return Pointer to the beginning of the data segment that the user has requested
*/
void * my_malloc (int size) 
{
	Node *temp_return = NULL;
	//First check for a node in the linked list
	if (root == NULL) // No nodes allocated yet
	{
		// Allocate the requested size
		temp_return = sbrk (size + sizeof(Node));
		// Check if allocation was successful
		if ((int)temp_return != -1)
		{
			root = temp_return;			// Sets root to value of temp_return
			root -> free = 0;			// Marks node as in use (not free)
			root -> data_size = size;	// Sets data size to size given by the user
			root -> prev = NULL;		// Sets previous node to null
			root -> next = NULL;		// Sets next node to null
			last = root;				// last and root are equal since only 1 node exists
			temp_return = ((void *) temp_return + sizeof(Node));
		}
		else
		{
			printf("The requested size of %d bytes was too large\n", &size);
			temp_return = NULL;
		}	
	}
	else // At least 1 node is allocated
	{
		// 1st check the list for any nodes that would work using Best Fit algorithm
		best = NULL; //Initialize best to Null
		tempN = root;
		cont = 1; //1=continue searching list, 0=perfect fit is found.
		while(tempN && cont) //tempN will be NULL at the end of the list
		{
			//Check if node is free & its data size is big enough, otherwise go to next node
			if(tempN -> free == 1 && tempN -> data_size >= size)
			{
				if (tempN -> data_size == size) //Perfect fit found. Stop searching the list
				{
					best = tempN;	// Mark current node as the best fit
					cont = 0;		// Stop searching the list
				}
				else if (best == NULL || tempN -> data_size < best -> data_size)
					best = tempN; //Assign if no best or temp's data_size is closer in fit
			}

			tempN = tempN -> next;
		}

		if (best != NULL) //Best fit is found
		{
			if (cont != 0) // Only need to split if perfect fit wasn't found
			{
				/*
					Check if the best fit node can be split into 2 nodes to reduce 
					internal fragmentation
				*/
				int test_size = (best -> data_size) - size  - sizeof(Node);
				if (test_size > 0)
				{
					Node *new = ((void *) best + sizeof(Node) + size);
					new -> free = 1; 		// Mark new node as free
					new -> data_size = test_size;// Size is remainder of free space
					new -> prev = best;
					new -> next = best -> next;
					best -> next = new;
					best -> data_size = size; 
					//Best is now a perfect fit with no internal fragmentation
				}
			}

			best -> free = 0; // Marks best fit node as in use (not free)
			temp_return = ((void *)best + sizeof(Node));
		}
		else //No nodes were big enough to hold the size requested
		{
			// Allocate the requested size
			temp_return = sbrk (size + sizeof(Node));

			// Check if allocation was successful
			if ((int)temp_return != -1)
			{
				temp_return -> free = 0;		// Marks node as in use (not free)
				temp_return -> data_size = size;// Sets data size to size given by the user
				temp_return -> prev = last;		// Sets previous node to last
				temp_return -> next = NULL;		// Sets next node to null
				last -> next = temp_return;		// Links last node to temp_return node
				last = temp_return;				// Sets last node to temp_return node
				temp_return = ((void *) temp_return + sizeof(Node));
			}
			else
			{
				printf("Insufficient memory to allocate the requested size of %d bytes\n", 
					&size);
				temp_return = NULL;
			}		
		}
	}

	return (void *)temp_return;
} 

/*
	my_free implements a custom version of free() for freeing heap memory
	@param *p Void pinter of the memory region which the user wants to free
*/
void my_free(void *p) 
{
	tempN = (p - sizeof(Node));  

	if (tempN == root) 
	{
		if (root -> next == NULL) //Removing only node (root == last)
		{
			sbrk(-(sizeof(Node) + (root -> data_size))); //brk = _end
			root = NULL;
			last = NULL;
		}
		else // > 1 nodes exist
		{
			root -> free = 1; //Mark root as free
			//Need to check if able to coalesce with next node
			if (((root -> next) -> free )== 1)
			{
				root -> data_size = (root -> data_size) + sizeof(Node) + 
									((root -> next) -> data_size);
				root -> next = ((root -> next) -> next);
			}
		}
	}
	else if (tempN == last)
	{
		//Need to check if able to coalesce with previous node
		if ((last -> prev) -> free == 1) //Check if next to last node is free
		{
			if (last -> prev == root)
			{
				sbrk(-(2 * sizeof(Node) + (last -> data_size) + (root -> data_size)));
				root = NULL;
				last = NULL;
			}
			else // >= 3 nodes exist
			{
				sbrk(-(2 * sizeof(Node) + (last -> data_size) + 
						((last -> prev) -> data_size)));
				last = ((last -> prev) -> prev);
				last -> next = NULL;
			}
		}
		else
		{
			sbrk(-(sizeof(Node) + (last -> data_size)));
			last = 	(last -> prev);
			last -> next = NULL;
		}
	}
	else //Freeing a node in the middle of the list
	{
		tempN -> free = 1; //Mark the node as free

		if ((tempN -> next) -> free == 1) //Check if next node is free
		{	//>3 nodes exist
			tempN -> data_size = ((tempN -> data_size) + sizeof(Node) + 
								 ((tempN -> next) -> data_size));
			tempN -> next = ((tempN -> next) -> next);
			(tempN -> next) -> prev = tempN;
		}

		if ((tempN -> prev) -> free == 1) //Check if previous node is free
		{	// > 2 nodes exist
			(tempN -> prev) -> data_size = (((tempN -> prev) -> data_size) + sizeof(Node) + 
										   (tempN -> data_size));
			(tempN -> prev) -> next = tempN -> next;
			(tempN -> next) -> prev = tempN -> prev;			
		}
	}
}