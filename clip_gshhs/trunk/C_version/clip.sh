#!/bin/bash

# This script is use to clip in one operation all data for level
# Suppose that gshhs data are in GSHHS folder

date
mkdir bd

# For clipping GSHHS polygons
# No =  0
# Yes = 1
POLYGONES=0

# For clipping rivers
# No =  0
# Yes = 1
RIVERS=1

# For clipping borders
# No =  0
# Yes = 1
BORDERS=1

# For draw global map
# Use it to validate clipping operation !
# No =  0
# Yes = 1
MAP=1

echo gshhs_$1

if [ $POLYGONES -gt 0 ] ; then 
    ./clipgshhs ./gshhs/gshhs_$1.b ./bd/poly-$1-1.dat
fi
 
if [ $RIVERS -gt 0 ] ; then 
    ./clipline ./gshhs/wdb_rivers_$1.b ./bd/rivers-$1-1.dat
fi

if [ $BORDERS -gt 0 ] ; then 
    ./clipline ./gshhs/wdb_borders_$1.b ./bd/borders-$1-1.dat
fi

if [ $MAP -gt 0 ] ; then 
    ./tiles_g -n1 -x0 -y0 -c ./bd/poly-$1-1.dat -r ./bd/rivers-$1-1.dat -b ./bd/borders-$1-1.dat -d -f -s1048 -t ./bd/map_$1.png
fi

date