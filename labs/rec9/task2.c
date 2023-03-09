#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * return true (1) if n is happy, false (0) otherwise
 */
bool isHappy (int n){
	if(n == 1) return 1;

	int map[10000];

	while(n > 0){
		int sum = 0;

		while(n > 0){
			int x = n % 10;
			sum += x * x;
			n = n / 10;
		}

		n = sum;
		map[n]++;
		
		if(map[n] > 1) return 0;
		if(n == 1) return 1;
	}

	return 0;
}


/*
 * Below are testing functions... DO NOT MODIFY!!!
 */

void show_result(int e, int r){
	printf("Expected: %d\nActual: %d...", e, r);
	if (r != e) 
        printf("FAIL\n\n\n");
    else 
        printf("OK\n\n\n");
}



int main()
{
	printf("\nTEST 1...19\n");
	show_result(1, isHappy(19));

	printf("\nTEST 2...2\n");
	show_result(0, isHappy(2));

	printf("\nTEST 3...7\n");
	show_result(1, isHappy(7));

	printf("\nTEST 4...176\n");
	show_result(1, isHappy(176));

	printf("\nTEST 5...36\n");
	show_result(0, isHappy(36));

	

	return 0;
}
