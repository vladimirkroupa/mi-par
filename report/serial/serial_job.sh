#!/bin/sh

# This file need runs with qrun.sh

#  ===========================================================================
# |                                                                           |
# |             COMMAND FILE FOR SUBMITTING SGE JOBS                          |
# |                                                                           |
# |                                                                           |
# | SGE keyword statements begin with #$                                      |
# |                                                                           |
# | Comments begin with #                                                     |
# | Any line whose first non-blank character is a pound sign (#)              |
# | and is not a SGE keyword statement is regarded as a comment.              |
#  ===========================================================================

# Request Bourne shell as shell for job
#$ -S /bin/sh

# Execute the job from the current working directory.
#$ -cwd

# Defines  or  redefines  the  path used for the standard error stream of the job.
#$ -e /mnt/data/stadlja1//errors/n20_k15.4c.log

# The path used for the standard output stream of the job.
#$ -o /mnt/data/stadlja1//outputs/n20_k15.4c.log

./serial.bin ./graphs/n20_k15.txt

