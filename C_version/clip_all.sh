#!/bin/bash

# This script is use to clip all level

# For download GSHHS data
# No =  0
# Yes = 1
DOWNLOAD=0

if [ $DOWNLOAD -gt 0 ] ; then 
    wget -N -c ftp://ftp.soest.hawaii.edu/pwessel/gshhs/gshhs+wdbii_2.2.0.zip
    unzip gshhs+wdbii_2.2.0.zip
fi

for LEVEL in c l i h f; do
    echo gshhs $LEVEL
    
    ./clip.sh $LEVEL
    
done

echo The end!

