#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"


/*
 * Define your call struct here.
 */
struct call{
	int id;
	char name[255];
	char reason[255];
};

/*
 * This function prints out the values of the given call struct in a formatted manner
 *
 * Params:
 *   call - the call struct that will be printed. May not be NULL.
 */
void print_call(struct call* call){
	printf("Call ID: %d\n", call->id);
	printf("Caller's name: %s\n", call->name);
	printf("Caller's reason: %s\n", call->reason);
}

/*
 * This function should change the value of the quit integer which will
 * in turn quit the program, and then also print out a string telling the user
 * that the  program is quitting.
 * 
 * Params:
 *   quit - int pointer which handles whether or not the program ends. May not be NULL.
 */
void quit_program(int* quit){
	*quit = 1;

	printf("\nQuitting... Thanks for using the program!!\n\n");
}

/*
 * This function creates a new call struct with a unique id and a name and reason
 * that is inputted by the user. Then the new call struct is inserted into a the queue
 * and an success message is outputted. The ID will iterate one for every new call.
 *
 * Params:
 *   queue - queue that will have the new call inserted into it. May not be NULL.
 *   id - id pointer which I reference to keep the id's for each call unique.
 */
void recieve_new_call(int* id, struct queue* queue){
	struct call* newcall = malloc(sizeof(struct call));

	//set id to the id and then iterate it
	newcall->id = *id;
	*id = *id + 1;

	//get call name
	printf("\nEnter Caller's name: ");
	scanf("%s", newcall->name);
	getchar();

	//get call reason
	printf("Enter Caller's reason: ");
	fgets(newcall->reason, sizeof(newcall->reason), stdin);
	printf("\n");

	//push call into the queue
	queue_enqueue(queue, (void*)newcall);

	printf("The call has been successfully added to the queue!!\n");
}

/*
 * This function will take the next call in the queue out, answer it and then push
 * that answered call into the stack. Then print out the call that was answered. 
 *
 * Params:
 *   stack - The stack from which the element will be pushed on to. May not be NULL.
 *   queue - The queue from which the element will be removed from. May not be NULL.
 */
void answer_a_call(struct stack* stack, struct queue* queue){
	//if queue is empty then can not answer call
	if(queue_isempty(queue)){
		printf("\nNo more calls need to be answered at the moment!\n");
		return;
	}

	printf("\nThe following call has been answered and added to the stack!\n\n");

	//get call from queue
	struct call* call = queue_dequeue(queue);

	//push call into stack
	stack_push(stack, (void*)call);

	//print answered call
	print_call(call);
}

/*
 * This prints out the current amount of calls in the stack, the calls answered,
 * and the last call on the stack or the last call that was answered. 
 *
 * Params:
 *   stack - The stack that is getting checked. May not be NULL.
 */
void current_stack(struct stack* stack){
	//print stack_size
	printf("\nNumber of calls answered: %d\n", stack_size(stack));

	//if it isn't empty then print out the last answered call
	if(!stack_isempty(stack)){
		struct call* topcall = stack_top(stack);
		printf("Details of the last call answered: \n\n");
		print_call(topcall);
	}
}

/*
 * This prints out the current amount of calls in the queue, the calls to be 
 * answered and the call on the bottom of the queue, or the next call to be answered.
 *
 * Params:
 *   queue - The queue that is getting checked. May not be NULL.
 */
void current_queue(struct queue* queue){
	//print queue size
	printf("\nNumber of calls to be answered: %d\n", queue_size(queue));

	//if it isn't empty then print out the next call to be answered
	if(!queue_isempty(queue)){
		struct call* topcall = queue_front(queue);
		printf("Details of the next call to be answered: \n\n");
		print_call(topcall);
	}
}

/*
 *  This handles printing out the prompt to ask the user for their next option
 */
void print_prompt(){
	printf("\n1. Receive a new call\n");
	printf("2. Answer a call\n");
	printf("3. Current state of the stack - answered calls\n");
	printf("4. Current state of the queue - calls to be answered\n");
	printf("5. Quit\n");
	printf("Choose an option: ");
}

/*
 * This function will take the users option and call the corresponding function with the correct parameters
 * passed in. This function will be called every time the user picks an option.  
 *
 * Params:
 *   stack - The stack which will be used for keeping track of answered calls. May not be NULL.
 *   queue - The queue which will be used for keeping track of calls to be answered. May not be NULL.
 *   id - This is used for making sure that each call has a unique ID
 *   quit - This will keep track of whether or not the user wants to quit the program
 *   option - This will handle which function is called and it resemblant of the user's option
 */
void handle_option(int option, int* id, int* quit, struct stack* stack, struct queue* queue){
	switch(option){
		case 1:
			recieve_new_call(id, queue);
			break;
		case 2:
			answer_a_call(stack, queue);
			break;
		case 3:
			current_stack(stack);
			break;
		case 4:
			current_queue(queue);
			break;
		case 5:
			quit_program(quit);
			break;
	}
}

/*
 * This is the main function which handles creating our needed data sctructures
 * and then getting user input and calling the corresponding function until the user quits.
 * After the user quits the memory in the data structures will be freed, and the program will
 * end.
 *
 * Params:
 *   argc - This is the amount of arguments
 *   argv - This is the array of the different arguments passed in
 */
int main(int argc, char const *argv[]) {
	//initialize variables needed for the program
	int quit = 0;
	int id = 0;

	//initialize and create stack and queue
	struct stack* stack = stack_create();
	struct queue* queue = queue_create();

	printf("Welcome to the call center!! Here's your options:\n");

	//while the user doesn't want to quit then keep on handling options and calling functions
	while(!quit){
		//reset option
		int option = 0;
		
		//print out prompt and get input
		print_prompt();
		scanf("%d", &option);

		//handle the input
		handle_option(option, &id, &quit, stack, queue);
	}

	//free data structures
	stack_free(stack);
	queue_free(queue);
	
	//exit
	return 0;
}
