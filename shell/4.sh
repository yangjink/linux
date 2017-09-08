#!/bin/bash

count=$1

i=1
j=1
count=$#
sum=0
z=0
for ((z=2;z<count;z++))
do
	i=$j
	j=$sum
	
	((sum=i+j))

done
echo $sum
