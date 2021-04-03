#!/bin/bash
files=(*)							#Variable to hold contents of directory (files)
lines=`ls | wc -l` 					#Count number of files within given directory
for ((i = 0; i < lines; i++))		#Run through the loop based on how many files we counted
do	
	file[$i]=`awk -F\n 'NR%2==0' ${files[$i]}`		#Parse each file for only its even numbered lines
	echo -n ${files[$i]}": "; echo "${file[$i]}"	#Format and print the file and its parsed contents
done