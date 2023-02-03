/*
 * This file contains a program that calculates how long it takes to insert
 * values into a dynamic array.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "dynarray.h"

int main(int argc, char** argv) {
  struct dynarray* da;
  int* values;
  clock_t t0, t1;
  int i, n;

  /*
  * Test dynamic array insertion timing for dataset sizes increasing from
  * 10,000 to 1,000,000 in steps of 10,000.
  */
  for (n = 10000; n <= 1000000; n += 10000) {
    printf("%d\t", n);
    fflush(stdout);

    /*
    * Allocate an array to store the dataset, and fill it with random numbers.
    */
    values = malloc(n * sizeof(int));
    assert(values);
    for (i = 0; i < n; i++) {
      values[i] = rand();
    }

    da = dynarray_create();

    /*
    * Insert all dataset values into the dynamic array, and compute the time
    * it takes to perform all of the insertions.
    */
    t0 = clock();
    for (i = 0; i <n; i++) {
      dynarray_insert(da, values + i);  /* "values + i" == "&values[i]" */
    }
    t1 = clock();
    printf("%f\n", (t1 - t0) / (float)CLOCKS_PER_SEC);

    dynarray_free(da);
    free(values);
  }
}
