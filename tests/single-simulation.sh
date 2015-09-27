#!/bin/bash

NS3_DIR="/home/klaus/ccn/ndnSIM2.1/ns-3"

if [ ${#1} == 0 ]
then
      PAR="100"
	else
	  PAR=$1
fi


cd $NS3_DIR

echo ""
echo "Running simulations"
./waf --run="cache-decision-linear --params=$PAR"
./waf --run="cache-decision-tree --params=$PAR"

echo "Finished!"
