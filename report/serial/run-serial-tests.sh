#!/bin/bash

SERVERS=( 4c 12c )

for SERVER in ${SERVERS[@]}; do
  for JOB in `ls tests/serial/*sh | grep $SERVER`;do
	echo "qrun.sh $SERVER 1 $QUEUE $JOB"
	qrun.sh $SERVER 1 serial $JOB
  done
done
