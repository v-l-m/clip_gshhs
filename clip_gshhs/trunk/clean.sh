#!/bin/bash

RESOL=$1

#PATHBD=/home/spenot/gshhs
PATHBD=./

echo Nettoyage
    cd bd
    nice -10 rm -r bd_$RESOL
    rm $RESOL*.dat
    cd ..

