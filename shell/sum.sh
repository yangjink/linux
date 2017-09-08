#!/bin/bash

str=1
sum=0
j=1
for i in {1..100}
do
	if [ "X$i" == "X1" ]
	then
		((sum+=j))
		let j++
		continue
	fi
	str+='+'$i
	((sum+=j))
	((j++))
done

echo "$str=$sum"
