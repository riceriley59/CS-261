/* CS261- Recitation 1 - Q.2
 * Solution description: call the function foo using "reference" to see the values before and after the function
 */
 
#include <stdio.h>
#include <stdlib.h>

int foo(int* a, int* b, int c){
    /*Set a to double its original value*/
    *a *= 2;

    /*Set b to half its original value*/
    *b /= 2;

    /*Assign a+b to c*/
    c = *a + *b;

    /*Return c*/
    return c;
}

int main(){
    /*Declare three integers x,y and z and initialize them to 7, 8, 9 respectively*/
    int x = 7, y = 8, z = 9;
    /*Print the values of x, y and z*/
    printf("Value of x: %d\nValue of y: %d\nValue of z: %d\n", x, y, z);
    /*Call foo() appropriately, passing x,y,z as parameters*/
    int temp = foo(&x, &y, z);
    /*Print the value returned by foo*/
    printf("The value returned by foo is: %d\n", temp);
    /*Print the values of x, y and z again*/
    printf("Value of x: %d\nValue of y: %d\nValue of z: %d\n", x, y, z);
    /*Is the return value different than the value of z?  Why? */
    //it's different because z was passed into foo by value and so the function 
    //took z's value and made a copy. This means z wans't touched.
    return 0;
}
    
    
