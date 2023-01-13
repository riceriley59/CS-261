#include <stdio.h>
 
void function (int *x, int n) { //takes in integer array and size of array
    int i, t, j = n, s = 1;
    
    //keep running while the list isn't sorted
    while (s) {
        s = 0;

        //loop through array check the value before the current index to see which one is bigger
        //and if a number is larger put it in front or change it
        for (i = 1; i < j; i++) {
            //if larger change it
            if (x[i] < x[i - 1]) {
                t = x[i]; // store current value in temp
                x[i] = x[i - 1]; //put current value equal to value before it
                x[i - 1] = t; // set previous value to temp value
                s = 1; //set s = 1 and keep going so that the while loop will run again
            }
        }

        //after each iteration take the sorted part and decrement that
        j--;
    }
}
 
int main () {
    int x[] = {15, 56, 12, -21, 1, 659, 3, 83, 51, 3, 135, 0};
    int n = sizeof(x) / sizeof(x[0]);
    int i;
    for (i = 0; i < n; i++)
        printf("%d%s", x[i], i == n - 1 ? "\n" : " ");
    function(x, n);
    for (i = 0; i < n; i++)
        printf("%d%s", x[i], i == n - 1 ? "\n" : " ");
    return 0;
}
