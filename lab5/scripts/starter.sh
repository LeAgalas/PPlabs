#!/bin/bash

rm parallelmpi_results.txt

for threads in {1..12}
do
    echo -ne "Results for $threads threads \n" >> parallelmpi_results.txt 
    for attempts in {1..50}
    do
        mpirun -np $threads --oversubscribe ./parallelmpi.o >> parallelmpi_results.txt
    done
    echo -ne "\n" >> parallelmpi_results.txt
done