#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv)
{
    const int count = 10000000;     ///< Number of array elements
    const int threads = 16;         ///< Number of parallel threads to use
    const int random_seed = 123123; ///< RNG seed
    const int attempts = 50; 

    int* array = 0;                 ///< The array we need to find the max in
    int  max;              ///< The maximal element
    int operations = 0;
    /* Initialize the RNG */
    srand(random_seed);

    /* Generate the random array */
    array = (int*)malloc(count*sizeof(int));
    for(int j=0; j < attempts; j++){
        max = -1;
        for(int i = 0; i < count; i++) { array[i] = rand(); }
        for(int i = 0; i < count; i++){            
            if (array[i] > max){
                max = array[i];
                operations++;
            } 
            operations++;
        }
    }
    printf("Average number of operations %d\n", operations / attempts);
    
    double start_time, end_time, total = 0;
    for(int j=0; j < attempts; j++){
        max = -1;
        for(int i = 0; i < count; i++) { array[i] = rand(); }
        start_time = omp_get_wtime();
        for(int i = 0; i < count; i++){            
            if (array[i] > max){
                max = array[i];
            } 
        }
        end_time = omp_get_wtime();
        total += end_time - start_time;
    }
    
    printf("Average time of work %f seconds\n", total / (double)attempts);


    free(array);    
    return(0);
}