#ifndef __LL_H
#define __LL_H 

struct node
{
	int val;
	struct node *next;
};

struct node* list_build(const char *digits);
int show(struct node* ln);
void free_list(struct node* ln);

struct node* add_two_num (struct node* l1, struct node* l2);

#endif