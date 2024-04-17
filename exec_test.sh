#!/bin/bash

repo="./build"
NB_THREADS_TO_CREATE=10
NB_THREADS_TO_YIELD=10

for file in "$repo"/*
do
    numb=$(basename "$file" | cut -c 1-2)
    if [ $(($numb)) -gt 12 ]; then
        if [ $(($numb)) -eq 31 ] || [ $(($numb)) -eq 32 ] || [ $(($numb)) -eq 33 ]; then
            echo $file $NB_THREADS_TO_CREATE $NB_THREADS_TO_YIELD;
            $file $NB_THREADS_TO_CREATE $NB_THREADS_TO_YIELD;
        else
            echo $file $NB_THREADS_TO_CREATE;
            $file $NB_THREADS_TO_CREATE;
        fi
    else 
        echo $file;
        $file;
    fi
    echo "";
done