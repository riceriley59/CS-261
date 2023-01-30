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

void print_call(struct call* call){
	printf("Call ID: %d\n", call->id);
	printf("Caller's name: %s\n", call->name);
	printf("Caller's reason: %s\n", call->reason);
}

void quit_program(int* quit){
	*quit = 1;

	printf("\nQuitting... Thanks for using the program!!\n\n");
}

void recieve_new_call(int* id, struct queue* queue){
	struct call* newcall = malloc(sizeof(struct call));

	newcall->id = *id;
	*id = *id + 1;

	printf("\nEnter Caller's name: ");
	scanf("%s", newcall->name);
	getchar();

	printf("Enter Caller's reason: ");
	fgets(newcall->reason, sizeof(newcall->reason), stdin);
	printf("\n");

	queue_enqueue(queue, (void*)newcall);

	printf("The call has been successfully added to the queue!!\n");
}

void answer_a_call(struct stack* stack, struct queue* queue){
	if(queue_isempty(queue)){
		printf("\nNo more calls need to be answered at the moment!\n");
		return;
	}

	printf("\nThe following call has been answered and added to the stack!\n\n");

	struct call* call = queue_dequeue(queue);

	stack_push(stack, (void*)call);

	print_call(call);
	free(call);
}

void current_stack(struct stack* stack){
	printf("\nNumber of calls answered: %d\n", stack_size(stack));

	if(!stack_isempty(stack)){
		struct call* topcall = stack_top(stack);
		printf("Details of the last call answered: \n\n");
		print_call(topcall);
	}
}

void current_queue(struct queue* queue){
	printf("\nNumber of calls to be answered: %d\n", queue_size(queue));

	if(!queue_isempty(queue)){
		struct call* topcall = queue_front(queue);
		printf("Details of the next call to be answered: \n\n");
		print_call(topcall);
	}
}

void print_prompt(){
	printf("\n1. Receive a new call\n");
	printf("2. Answer a call\n");
	printf("3. Current state of the stack - answered calls\n");
	printf("4. Current state of the queue - calls to be answered\n");
	printf("5. Quit\n");
	printf("Choose an option: ");
}

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


int main(int argc, char const *argv[]) {
	int quit = 0;
	int id = 0;

	struct stack* stack = stack_create();
	struct queue* queue = queue_create();

	printf("Welcome to the call center!! Here's your options:\n");

	while(!quit){
		int option = 0;
		
		print_prompt();
		scanf("%d", &option);

		handle_option(option, &id, &quit, stack, queue);
	}

	stack_free(stack);
	queue_free(queue);
  
	return 0;
}
