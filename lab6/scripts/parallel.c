#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void insertionsort(int a[], int n, int stride) {
    for (int j=stride; j<n; j+=stride) {
        int key = a[j];
        int i = j - stride;
        while (i >= 0 && a[i] > key) {
            a[i+stride] = a[i];
            i-=stride;
        }
        a[i+stride] = key;
    }
}

void shellsort(int a[], int n)
{
    int i, m;
    for(m = n/2; m > 0; m /= 2)
    {
            for(i = 0; i < m; i++)
                insertionsort(&(a[i]), n-i, m);
    }
}


int main(int argc, char** argv)
{
	const int count = 1000000;     ///< Number of array elements
    const int random_seed = 123123; ///< RNG seed
    int size = -1;	
	int rank = -1;
    MPI_Status status;
    double start, end;

    /* Initialize the RNG */
    srand(random_seed);
    
    int* array = 0; 
    /* Generate the random array */
    array = (int*)malloc(count*sizeof(int));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (!rank) {
		/* Generate the random array */
		for (int i = 0; i < count; i++) { array[i] = rand(); }
	}
    int step = count / size;
    int *partArray;


    if (!rank){
         for (int i = 1; i < size; i++) {
            MPI_Send(array + step * (i - 1), step, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        start = MPI_Wtime();
        int partArraySize = count - step * (size - 1);
        partArray = (int*)malloc(partArraySize*sizeof(int));
        for (int i = step * (size - 1); i < count; i++) {
            partArray[i - step * (size - 1)] = array[i];
        }
        shellsort(partArray, partArraySize);
        int *newArray = (int*)malloc(count*sizeof(int));
       
        for (int i = 0; i < partArraySize; i++) {
            newArray[i] = partArray[i];
        }
        for (int i = 1; i < size; i++) {
            MPI_Recv(newArray + step * (i - 1) + partArraySize, step, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        }
        shellsort(newArray, count);
        end = MPI_Wtime();
        printf("%lf ", end-start);
    } else{
        partArray = (int*)malloc(step * sizeof(int));;
        MPI_Recv(partArray, step, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        shellsort(partArray, step);
        MPI_Send(partArray, step, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    free(array);    

	return(0);
}