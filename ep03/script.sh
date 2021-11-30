#!/bin/bash

# L2 CACHE Data Cache Miss Ratio
L2CAHE=$(likwid-perfctr -C 1 -m -g L2CACHE ./gaussJacobi-likwid sistema.dat | grep "L2 miss ratio" | awk -F'|' '{ printf "%s ", $3}')