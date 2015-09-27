#!/bin/bash

NS3_DIR="/home/klaus/ccn/ndnSIM2.1/ns-3"

cd $NS3_DIR

echo ""
echo "Running simulations"
./waf --run="cache-decision-simulation"

echo "Finished!"
