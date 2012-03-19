#!/bin/bash

date
echo gshhs_c
./clipgshhsM ./gshhs/gshhs_c.b ./bd/poly-c-1M.dat
date
./tiles_g -n1 -x0 -y0 -c ./bd/poly-c-1M.dat -d -f -s1024 -t poly-c-1M.png
echo
date
echo gshhs_l
./clipgshhsM ./gshhs/gshhs_l.b ./bd/poly-l-1M.dat
date
./tiles_g -n1 -x0 -y0 -c ./bd/poly-l-1M.dat -d -f -s1024 -t poly-l-1M.png
echo
date
echo gshhs_i
./clipgshhsM ./gshhs/gshhs_i.b ./bd/poly-i-1M.dat
date
./tiles_g -n1 -x0 -y0 -c ./bd/poly-i-1M.dat -d -f -s1024 -t poly-i-1M.png
echo
date
echo gshhs_h
./clipgshhsM ./gshhs/gshhs_h.b ./bd/poly-h-1M.dat
date
./tiles_g -n1 -x0 -y0 -c ./bd/poly-h-1M.dat -d -f -s1024 -t poly-h-1M.png
echo
date
echo gshhs_f
./clipgshhsM ./gshhs/gshhs_f.b ./bd/poly-f-1M.dat
date
./tiles_g -n1 -x0 -y0 -c ./bd/poly-f-1M.dat -d -f -s1024 -t poly-f-1M.png


