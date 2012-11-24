#!/bin/sh

#$ -S /bin/sh

#$ -cwd

#$ -e /mnt/data/stadlja1//errors/n70_k30.4c.log

#$ -o /mnt/data/stadlja1//outputs/n70_k30.4c.log


./serial.bin ./graphs/n70_k30.txt

