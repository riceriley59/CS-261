#include <stdio.h>
#include <stdlib.h>

int min_child(int* nums, int i, int size){
	//if the right child is out of bounds of array then return -1
	if(((i * 2) + 2) > size - 1){
		return -1;
	}

	//check to see which node has lower priority
	if(nums[(i * 2) + 1] > nums[(i * 2) + 2]){
		//return right childs index
		return (i * 2) + 1;
	}else{
		//return left childs index
		return (i * 2) + 2;
	}
}

void percolate_down(int* nums, int i, int size){
	//get the index for the minimum child of the current index
	int min = min_child(nums, i, size);

	//if the index is out of bounds then break
	if(min == -1) return;

	//get the nodes at the current index and minimum child's index
	int minvalue = nums[min];
	int ivalue = nums[i];

	//if the priority of the minimum child is less than the root's priority 
	//then swap nodes and loop again otherwise break
	if(minvalue > ivalue){
		//swap nodes
        nums[min] = ivalue;
        nums[i] = minvalue;

		//set i to current min so that next iteration we can find the next
		//min child one depth down
		percolate_down(nums, min, size);
	}else return;
}

int* heapify(int* nums, int numsSize){
    for(int i = (numsSize / 2) - 1; i >= 0; i--){
        percolate_down(nums, i, numsSize);
    }
}

int findKthLargest(int* nums, int numsSize, int k)
{
    heapify(nums, numsSize);

    int c = numsSize - 1;

    for(int i = 0; i < k; i++){
        int ival = nums[c];
        int firstval = nums[0];

        nums[c] = firstval;
        nums[0] = ival;

        percolate_down(nums, 0, c);

        c--;
    }

    return nums[numsSize - k];
}


int main(int argc, char **argv)
{
    
    printf("\nTest 1...\n");
    int num1[6] = {3,2,1,5,6,4};
    printf("2nd largest in [3 2 1 5 6 4]\n");
    printf("Expected: 5\n");
    printf("Actual: %d\n", findKthLargest(num1, 6, 2));

    printf("\nTest 2...\n");
    int num2[9] = {3,2,3,1,2,4,5,5,6};
    printf("4th largest in [3 2 3 1 2 4 5 5 6]\n");
    printf("Expected: 4\n");
    printf("Actual: %d\n", findKthLargest(num2, 9, 4));


    printf("\nTest 3...\n");
    int num3[5] = {10,10,10,10,10};
    printf("5th largest in [10 10 10 10 10]\n");
    printf("Expected: 10\n");
    printf("Actual: %d\n", findKthLargest(num3, 5, 5));

    printf("\nTest 4...\n");
    int num4[7] = {-10,10,-10,10,-10,10,0};
    printf("4th largest in [-10 10 -10 10 -10 10 0]\n");
    printf("Expected: 0\n");
    printf("Actual: %d\n", findKthLargest(num4, 7, 4));

    printf("\nTest 5...\n");
    int num5[10] = {10,9,8,7,6,5,4,3,2,1};
    printf("1st largest in [10 9 8 7 6 5 4 3 2 1]\n");
    printf("Expected: 10\n");
    printf("Actual: %d\n", findKthLargest(num5, 10, 1));


    return 0;
}