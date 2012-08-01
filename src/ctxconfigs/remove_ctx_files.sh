#!/bin/bash

for bench in GemsFDTD cactusADM  gamess h264ref   libquantum  omnetpp soplex tonto astar calculix gcc hmmer mcf perlbench specrand wrf bwaves calculix-gamess gobmk lbm milc povray sphinx3 xalancbmk bzip2 dealII gromacs leslie3d namd sjeng str zeusmp; do
	./inifile.py ctxconfig.$bench remove "Context 0" IPCs
done
