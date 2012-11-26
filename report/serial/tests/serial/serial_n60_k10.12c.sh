#!/bin/sh

#$ -S /bin/sh

#$ -cwd

#$ -e /mnt/data/stadlja1//errors/n60_k10.12c.log

#$ -o /mnt/data/stadlja1//outputs/n60_k10.12c.log


./serial.bin ./graphs/n60_k10.txt

