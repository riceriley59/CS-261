#include <stdio.h>
#include <stdlib.h>

/* 
 * return the number that is missing in the array
*/
int missingNumber(int* nums, int size){
	int map[size + 1];

	for(int i = 0; i < size; i++){
		map[nums[i]] = 1;
	}

	for(int i = 0; i < size + 1; i++){
		if(map[i] != 1) return i;
	}

	return -1;
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

void shuffle(int *arr, int size){
	for (int i = 0; i < size; ++i)
	{
		int temp = arr[i];
		int rand_idx = rand() % size;
		arr[i] = arr[rand_idx];
		arr[rand_idx] = temp;
	}
}


void test_case(const char* s, int* arr, int size, int e){
	int r;
	printf("%s", s);
	r = missingNumber(arr, size);
	show_result(e, r);
}

int main()
{
	int r, e, size;
	srand(0);

	int nums1[3] = {3,0,1};
	test_case("\nTEST 1...\n", nums1, 3, 2);
	
	int nums2[2] = {0,1};
	test_case("\nTEST 2...\n", nums2, 2, 2);
	
	int nums3[9] = {9,6,4,2,3,5,7,0,1};
	test_case("\nTEST 3...\n", nums3, 9, 8);
	

	size = 1000;
	int nums4[size];
	for (int i = 0; i < size; ++i){
		nums4[i] = i;
	}
	shuffle(nums4, size);
	e = nums4[size-1];
	test_case("\nTEST 4...\n", nums4, size-1, e);


	size = 100000;
	int nums5[size];
	for (int i = 0; i < size; ++i){
		nums5[i] = i;
	}
	shuffle(nums5, size);
	e = nums5[size-1];
	test_case("\nTEST 5...\n", nums5, size-1, e);
	


	return 0;
}
