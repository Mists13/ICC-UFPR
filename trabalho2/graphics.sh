#!/bin/bash

mkdir "graphics"

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L3_CACHE_BANDWIDTH.png";
            set title "L3 - Memory bandwidth [MBytes/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MBytes/s";
            plot "tables/table_L3.dat" using 1:2 with lines smooth csplines t"Matriz de Derivadas Parciais - T1", 
                "tables/table_L3.dat"  using 1:3 with lines smooth csplines t"Método Newton - T1",
                "tables/table_L3.dat"  using 1:4 with lines smooth csplines t"Matriz Jacobiana - T1",
                "tables/table_L3.dat"  using 1:5 with lines smooth csplines t"Resolução SNL - T1";
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L2_CACHE_MISS_RATIO.png";
            set title "L2 - Data Cache Miss Ratio";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Miss Ratio";
            plot "tables/table_L2CACHE.dat" using 1:2 with lines smooth csplines t"Matriz de Derivadas Parciais - T1", 
                "tables/table_L2CACHE.dat"  using 1:3 with lines smooth csplines t"Método Newton - T1",
                "tables/table_L2CACHE.dat"  using 1:4 with lines smooth csplines t"Matriz Jacobiana - T1",
                "tables/table_L2CACHE.dat"  using 1:5 with lines smooth csplines t"Resolução SNL - T1";
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/FLOPS_DP.png";
            set title "Flops DP e AVX [MFLOP/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MFLOP/s";
            plot "tables/table_FLOPS.dat" using 1:2 with lines smooth csplines t"Matriz de Derivadas Parciais DP - T1",
                "tables/table_FLOPS.dat" using 1:3 with lines smooth csplines t"Matriz de Derivadas Parciais AVX - T1", 
                "tables/table_FLOPS.dat"  using 1:4 with lines smooth csplines t"Método Newton DP - T1",
                "tables/table_FLOPS.dat"  using 1:5 with lines smooth csplines t"Método Newton AVX - T1",
                "tables/table_FLOPS.dat"  using 1:6 with lines smooth csplines t"Matriz Jacobiana DP- T1",
                "tables/table_FLOPS.dat"  using 1:7 with lines smooth csplines t"Matriz Jacobiana AVX- T1",
                "tables/table_FLOPS.dat"  using 1:8 with lines smooth csplines t"Resolução SNL DP - T1",
                "tables/table_FLOPS.dat"  using 1:9 with lines smooth csplines t"Resolução SNL AVX - T1";
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/RUNTIME.png";
            set title "L2 - Data Cache Miss Ratio";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Miss Ratio";
            plot "tables/table_RUNTIME.dat" using 1:2 with lines smooth csplines t"Matriz de Derivadas Parciais - T1", 
                "tables/table_RUNTIME.dat"  using 1:3 with lines smooth csplines t"Método Newton - T1",
                "tables/table_RUNTIME.dat"  using 1:4 with lines smooth csplines t"Matriz Jacobiana - T1",
                "tables/table_RUNTIME.dat"  using 1:5 with lines smooth csplines t"Resolução SNL - T1";
'

