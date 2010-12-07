#!/bin/bash

RESOL=$1

#PATHBD=/home/spenot/gshhs
PATHBD=.


    echo Resolution $RESOL
    
    echo Extraction de Rivers
        ./readlines $PATHBD/gshhs/wdb_rivers_$RESOL.b 1 $PATHBD/bd/rivers-$RESOL-1.dat
    
    echo
    
    echo Extraction de Borders
        ./readlines $PATHBD/gshhs/wdb_borders_$RESOL.b 1 $PATHBD/bd/borders-$RESOL-1.dat


