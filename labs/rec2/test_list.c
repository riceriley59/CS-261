#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ll.h"


struct node* list_build(const char *digits)
{
    struct node *res, *p, *prev;
    int first = 1;
    int len = strlen(digits);
    const char *c = digits + len - 1;
    prev = NULL;
    while (len-- > 0) {
        p = malloc(sizeof(*p));
        if (first) {
            first = 0;
            res = p;
        }
        p->val = *c-- - '0';
        p->next = NULL;
        if (prev != NULL) {
            prev->next = p;
        }
        prev = p;
    }

    return res;
}



int show(struct node* ln)
{
    int sum = 0, factor = 1;
    while (ln != NULL) {
    	if (ln->val >= 10){
    		printf("Error: some node(s) in your list contains a value >= 10\n");
    		return 0;
    	}
        sum += ln->val * factor;
        factor *= 10;
        ln = ln->next;
    }
    printf("%d\n", sum);
    return sum;
}

void free_list(struct node* ln)
{
	struct node* temp;
	
	while(ln != NULL)
	{
		temp = ln;
		ln = ln->next;
		free(temp);
	}
	
}

int main(int argc, char const *argv[])
{
	struct node *l1, *l2, *res;
	int first, second, result;
	char* str = ""; 

	//case 1: 123 + 456 = 579
	l1 = list_build("123");
	l2 = list_build("456");
	res = add_two_num(l1, l2);

	first = show(l1);
	second = show(l2);
	result = show(res);

	str = first + second == result? "PASS" : "FAILED";
	printf("Test case 1: %s\n\n\n", str);

	free_list(l1);
	free_list(l2);
	free_list(res);

	//case 2: 342 + 465 = 807
	l1 = list_build("342");
	l2 = list_build("465");
	res = add_two_num(l1, l2);

	first = show(l1);
	second = show(l2);
	result = show(res);

	str = first + second == result? "PASS" : "FAILED";
	printf("Test case 2: %s\n\n\n", str);

	free_list(l1);
	free_list(l2);
	free_list(res);

	//case 3: 0 + 0 = 0
	l1 = list_build("0");
	l2 = list_build("0");
	res = add_two_num(l1, l2);

	first = show(l1);
	second = show(l2);
	result = show(res);

	str = first + second == result? "PASS" : "FAILED";
	printf("Test case 3: %s\n\n\n", str);

	free_list(l1);
	free_list(l2);
	free_list(res);

	//case 4: 9999999 + 999 = 0
	l1 = list_build("9999999");
	l2 = list_build("999");
	res = add_two_num(l1, l2);

	first = show(l1);
	second = show(l2);
	result = show(res);

	str = first + second == result? "PASS" : "FAILED";
	printf("Test case 4: %s\n\n\n", str);

	free_list(l1);
	free_list(l2);
	free_list(res);

	return 0;
}