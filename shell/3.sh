#!/bin/bash

count=$#
max=$1
min=$1
j=0
sum=0
for j in $@
do
	if [ $j -lt $min ]
	then
		min=$j
	fi
	if [ $j -gt $max ]
	then
		max=$j
	let sum+=j
	fi
done

echo $sum
