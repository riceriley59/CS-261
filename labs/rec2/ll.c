#include <stdlib.h>

#include "ll.h"

// struct node
// {
// 	int val;
// 	struct node *next;
// };


struct node* add_two_num (struct node* l1, struct node* l2)
{
	struct node* head = (struct node*)malloc(sizeof(struct node*));
	struct node* curr = head;

	int carry = 0;

	while(l1 || l2 || carry){
		int sum = 0;
		
		if(l1){
			sum += l1->val;
			l1 = l1->next;
		}
		if(l2){
			sum += l2->val;
			l2 = l2->next;
		}

		sum += carry;

		struct node* new = (struct node*)malloc(sizeof(struct node*));

		carry = sum / 10;
		new->val = sum % 10;

		curr->next = new;
		curr = curr->next;
	}

	return head->next;
}


