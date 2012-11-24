#!/bin/bash


HOME=/mnt/data/stadlja1/
OUTPUT_DIR=$HOME/tests/serial/

rm $OUTPUT_DIR/*

SERVERS=( 4c 12c )
for SERVER in ${SERVERS[@]}; do
for GRAPH in ` ls graphs/ | grep txt | cut -d. -f1`; do
	#vygenerovat serial_nINT_nINT.sh

	
	FILENAME="$GRAPH.$SERVER"
	OUTPUT_FILE="$OUTPUT_DIR/serial_$FILENAME.sh";
	echo "Generating $OUTPUT_FILE"

	echo "#!/bin/sh" > $OUTPUT_FILE
	echo "" >> $OUTPUT_FILE;

	# Request Bourne shell as shell for job
	echo '#$ -S /bin/sh' >> $OUTPUT_FILE
	echo "" >> $OUTPUT_FILE

	# Execute the job from the current working directory.
	echo "#$ -cwd" >> $OUTPUT_FILE
	echo "" >> $OUTPUT_FILE

	# Defines  or  redefines  the  path used for the standard error stream of the job.
	echo "#$ -e $HOME/errors/$FILENAME.log" >> $OUTPUT_FILE
	echo "" >> $OUTPUT_FILE

	# The path used for the standard output stream of the job.
	echo "#$ -o $HOME/outputs/$FILENAME.log" >> $OUTPUT_FILE
	echo "" >> $OUTPUT_FILE
	echo "" >> $OUTPUT_FILE

	echo "./serial.bin ./graphs/$GRAPH.txt" >> $OUTPUT_FILE
	echo "" >> $OUTPUT_FILE

	chmod +x $OUTPUT_FILE

done
done
