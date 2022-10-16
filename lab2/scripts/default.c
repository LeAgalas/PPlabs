#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv)
{
    const int count = 10000000;     ///< Number of array elements
    const int random_seed = 123123; ///< RNG seed
    const int attempts = 25; 
    const int thread_limit = 25;
    const int target = 1337;


    int* array = 0;                 ///< The array we need to find the max in
    int  index;              ///< The maximal element
    /* Initialize the RNG */
    srand(random_seed);

    /* Generate the random array */
    array = (int*)malloc(count*sizeof(int));
    
    double start_time, end_time, total = 0;

    for(int j=0; j < attempts; j++){
        index = -1;
        for(int i = 0; i < count; i++) { array[i] = rand(); }
        start_time = omp_get_wtime();
        for(int i = 0; i < count; i++){           
            if (array[i] == target){
                index = i;
                break;
            } 
        }
        end_time = omp_get_wtime();
        total += end_time - start_time;
    }
    printf("Average time of work %f seconds\n", total / (double)attempts);

    free(array);    
    return(0);
}