#!/bin/bash

NS3_DIR="/home/klaus/ccn/ndnSIM2.1/ns-3"

cd $NS3_DIR

echo ""
echo "Testing lowest cost strategy!"
./waf --run="cache-decision-test"
