#!/bin/bash

NB_THREADS_TO_CREATE=20
NB_THREADS_TO_YIELD=20

for file in "$@"; do
	numb=$(basename "$file" | cut -c 1-2)
	if [ $(($numb)) -gt 12 ]; then
		if [ $(($numb)) -eq 31 ] || [ $(($numb)) -eq 32 ]; then
			echo $file 10 $NB_THREADS_TO_YIELD
			$file 10 $NB_THREADS_TO_YIELD
		elif [ $((numb)) -eq 33 ]; then
			echo $file $NB_THREADS_TO_CREATE 5
			$file $NB_THREADS_TO_CREATE $NB_THREADS_TO_YIELD
		elif [ $((numb)) -eq 51 ]; then
			echo $file 8
			$file 8
		elif [ $((numb)) -eq 71 ]; then
			echo $file 5
			$file 5
		else
			echo $file $NB_THREADS_TO_CREATE
			$file $NB_THREADS_TO_CREATE
		fi
	else
		echo $file
		$file
	fi
	echo ""
	echo "******************************************************************************"
done

