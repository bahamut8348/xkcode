#! /bin/bash

for (( i = 1; i <= 5; i ++ ))
do
	echo "$i"
	python meituan.py &
done

