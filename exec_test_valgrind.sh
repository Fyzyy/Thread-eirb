#!/bin/bash

repo="./build"
NB_THREADS_TO_CREATE=20
NB_THREADS_TO_YIELD=20

for file in "$repo"/*
do
    numb=$(basename "$file" | cut -c 1-2);
    if [ $(($numb)) -gt 12 ]; then
        if [ $(($numb)) -eq 31 ] || [ $(($numb)) -eq 32 ]; then
            echo $file 10 $NB_THREADS_TO_YIELD;
            valgrind --leak-check=full --show-reachable=yes --track-origins=yes $file 10 $NB_THREADS_TO_YIELD; \
        elif [ $((numb)) -eq 33 ]; then
            echo $file $NB_THREADS_TO_CREATE 5;
            valgrind --leak-check=full --show-reachable=yes --track-origins=yes $file $NB_THREADS_TO_CREATE $NB_THREADS_TO_YIELD; \
        elif [ $((numb)) -eq 51 ]; then
            echo $file 8;
            valgrind --leak-check=full --show-reachable=yes --track-origins=yes $file 8; \
        elif [ $((numb)) -eq 71 ]; then
            echo $file 5;
            valgrind --leak-check=full --show-reachable=yes --track-origins=yes $file 5; \
        else
            echo $file $NB_THREADS_TO_CREATE;
            valgrind --leak-check=full --show-reachable=yes --track-origins=yes $file $NB_THREADS_TO_CREATE; \
        fi
    else 
        echo $file;
        valgrind --leak-check=full --show-reachable=yes --track-origins=yes $file; \
    fi
    echo "";
    echo "******************************************************************************";
done