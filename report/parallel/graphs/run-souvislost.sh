#!/bin/bash

for i in `ls | grep txt`; do
	./souvislost -t -i $i -o $i
done
