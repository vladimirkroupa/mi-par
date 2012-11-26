#!/bin/sh

qrun.sh 2 short ./tests/parallel/n70_k35_2_eth.sh
qrun.sh 2 short ./tests/parallel/n70_k35_2_inf.sh
qrun.sh 2 short ./tests/parallel/n70_k50_2_eth.sh
qrun.sh 2 short ./tests/parallel/n70_k50_2_inf.sh
qrun.sh 2 short ./tests/parallel/n70_k60_2_eth.sh
qrun.sh 2 short ./tests/parallel/n70_k60_2_inf.sh

qrun.sh 4 short ./tests/parallel/n70_k35_4_eth.sh
qrun.sh 4 short ./tests/parallel/n70_k35_4_inf.sh
qrun.sh 4 short ./tests/parallel/n70_k50_4_eth.sh
qrun.sh 4 short ./tests/parallel/n70_k50_4_inf.sh
qrun.sh 4 short ./tests/parallel/n70_k60_4_eth.sh
qrun.sh 4 short ./tests/parallel/n70_k60_4_inf.sh

qrun.sh 8 short ./tests/parallel/n70_k35_8_eth.sh
qrun.sh 8 short ./tests/parallel/n70_k35_8_inf.sh
qrun.sh 8 short ./tests/parallel/n70_k50_8_eth.sh
qrun.sh 8 short ./tests/parallel/n70_k50_8_inf.sh
qrun.sh 8 short ./tests/parallel/n70_k60_8_eth.sh
qrun.sh 8 short ./tests/parallel/n70_k60_8_inf.sh

qrun.sh 16 short ./tests/parallel/n70_k35_16_eth.sh
qrun.sh 16 short ./tests/parallel/n70_k35_16_inf.sh
qrun.sh 16 short ./tests/parallel/n70_k50_16_eth.sh
qrun.sh 16 short ./tests/parallel/n70_k50_16_inf.sh
qrun.sh 16 short ./tests/parallel/n70_k60_16_eth.sh
qrun.sh 16 short ./tests/parallel/n70_k60_16_inf.sh
