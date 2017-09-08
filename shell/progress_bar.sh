#!/bin/bash
i=1
sign=#
index=0
myarr=('|' '/' '-' '\\')
while [ $i -le 100 ]
do
	printf "[%-100s][%d%%][%c]\r" "$sign" "$i" "${myarr[$index]}"
	sign+="#"
	let index++
	((index=index%4))
	let i++
	usleep 0.1
done
