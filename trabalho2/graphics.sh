#!/bin/bash

mkdir "graphics"

# Gera gráficos referentes a Matriz de Derivadas Parciais
gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L3_CACHE_BANDWIDTH_MAT_DERIVADAS.png";
            set title "L3 - Memory bandwidth [MBytes/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MBytes/s";
            plot "tables/table_mat_deriv_parcial_l3.dat" using 1:2 with lines smooth csplines t"Matriz de Derivadas Parciais - T1", 
                "tables/table_mat_deriv_parcial_l3.dat"  using 1:3 with lines smooth csplines t"Matriz de Derivadas Parcias - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L2_CACHE_MISS_RATIO_MAT_DERIVADAS.png";
            set title "L2 - Data Cache Miss Ratio";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Miss Ratio";
            plot "tables/table_mat_deriv_parcial_l2cache.dat" using 1:2 with lines smooth csplines t"Matriz de Derivadas Parciais - T1", 
                "tables/table_mat_deriv_parcial_l2cache.dat"  using 1:3 with lines smooth csplines t"Matriz de Derivadas Parciais - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/FLOPS_DP_MAT_DERIVADAS.png";
            set title "Flops DP e AVX [MFLOP/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MFLOP/s";
            plot "tables/table_mat_deriv_parcial_flops.dat" using 1:2 with lines smooth csplines t"Matriz de Derivadas Parciais DP - T1",
                "tables/table_mat_deriv_parcial_flops.dat"  using 1:3 with lines smooth csplines t"Matriz de Derivadas Parciais AVX - T1", 
                "tables/table_mat_deriv_parcial_flops.dat"  using 1:4 with lines smooth csplines t"Matriz de Derivadas Parciais DP - T2",
                "tables/table_mat_deriv_parcial_flops.dat"  using 1:5 with lines smooth csplines t"Matriz de Derivadas Parciais AVX - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/RUNTIME_MAT_DERIVADAS.png";
            set title "Runtime";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Segundos";
            plot "tables/table_mat_deriv_parcial_runtime.dat" using 1:2 with lines smooth csplines t"Matriz de Derivadas Parciais - T1", 
                "tables/table_mat_deriv_parcial_runtime.dat"  using 1:3 with lines smooth csplines t"Matriz de Derivadas Parciais - T2",
'


# Gera gráficos referentes a Matriz Jacobiana
gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L3_CACHE_BANDWIDTH_MAT_JACOBIANA.png";
            set title "L3 - Memory bandwidth [MBytes/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MBytes/s";
            plot "tables/table_jacobiana_l3.dat" using 1:2 with lines smooth csplines t"Matriz Jacobiana - T1", 
                "tables/table_jacobiana_l3.dat"  using 1:3 with lines smooth csplines t"Matriz Jacobiana - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L2_CACHE_MISS_RATIO_JACOBIANA.png";
            set title "L2 - Data Cache Miss Ratio";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Miss Ratio";
            plot "tables/table_jacobiana_l2cache.dat" using 1:2 with lines smooth csplines t"Matriz Jacobiana - T1", 
                "tables/table_jacobiana_l2cache.dat"  using 1:3 with lines smooth csplines t"Matriz Jacobiana - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/FLOPS_DP_MAT_JACOBIANA.png";
            set title "Flops DP e AVX [MFLOP/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MFLOP/s";
            plot "tables/table_jacobiana_flops.dat" using 1:2 with lines smooth csplines t"Matriz Jacobiana DP - T1",
                "tables/table_jacobiana_flops.dat"  using 1:3 with lines smooth csplines t"Matriz Jacobiana AVX - T1", 
                "tables/table_jacobiana_flops.dat"  using 1:4 with lines smooth csplines t"Matriz Jacobiana DP - T2",
                "tables/table_jacobiana_flops.dat"  using 1:5 with lines smooth csplines t"Matriz Jacobiana AVX - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/RUNTIME_MAT_JACOBIANA.png";
            set title "L2 - Data Cache Miss Ratio";
            set key outside right;
            set xlabel "Runtime";
            set ylabel "Segundos";
            plot "tables/table_jacobiana_runtime.dat" using 1:2 with lines smooth csplines t"Matriz Jacobiana - T1", 
                "tables/table_jacobiana_runtime.dat"  using 1:3 with lines smooth csplines t"Matriz Jacobiana - T2",
'

# Gera gráficos referentes a Resolução do SNL
gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L3_CACHE_BANDWIDTH_RESOLUCAO_SNL.png";
            set title "L3 - Memory bandwidth [MBytes/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MBytes/s";
            plot "tables/table_resolucao_l3.dat" using 1:2 with lines smooth csplines t"Resolução SNL - T1", 
                "tables/table_resolucao_l3.dat"  using 1:3 with lines smooth csplines t"Resolução SNL - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L2_CACHE_MISS_RATIO_RESOLUCAO_SNL.png";
            set title "L2 - Data Cache Miss Ratio";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Miss Ratio";
            plot "tables/table_resolucao_l2cache.dat" using 1:2 with lines smooth csplines t"Resolução SNL  - T1", 
                "tables/table_resolucao_l2cache.dat"  using 1:3 with lines smooth csplines t"Resolução SNL  - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/FLOPS_DP_RESOLUCAO_SNL.png";
            set title "Flops DP e AVX [MFLOP/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MFLOP/s";
            plot "tables/table_resolucao_flops.dat" using 1:2 with lines smooth csplines t"Resolução SNL DP - T1",
                "tables/table_resolucao_flops.dat"  using 1:3 with lines smooth csplines t"Resolução SNL AVX - T1", 
                "tables/table_resolucao_flops.dat"  using 1:4 with lines smooth csplines t"Resolução SNL DP - T2",
                "tables/table_resolucao_flops.dat"  using 1:5 with lines smooth csplines t"Resolução SNL AVX - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/RUNTIME_MAT_RESOLUCAO_SNL.png";
            set title "Runtime";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Segundos";
            plot "tables/table_resolucao_runtime.dat" using 1:2 with lines smooth csplines t"Resolução SNL - T1", 
                "tables/table_resolucao_runtime.dat"  using 1:3 with lines smooth csplines t"Resolução SNL - T2",
'

# Gera gŕaficos referentes a aplicação total do Método Newton
gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L3_CACHE_BANDWIDTH_METODO_NEWTON.png";
            set title "L3 - Memory bandwidth [MBytes/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MBytes/s";
            plot "tables/table_total_l3.dat" using 1:2 with lines smooth csplines t"Método Newton - T1", 
                "tables/table_total_l3.dat"  using 1:3 with lines smooth csplines t"Método Newton - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/L2_CACHE_MISS_RATIO_METODO_NEWTON.png";
            set title "L2 - Data Cache Miss Ratio";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Miss Ratio";
            plot "tables/table_total_l2cache.dat" using 1:2 with lines smooth csplines t"Método Newton - T1", 
                "tables/table_total_l2cache.dat"  using 1:3 with lines smooth csplines t"Método Newton - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/FLOPS_DP_METODO_NEWTON.png";
            set title "Flops DP e AVX [MFLOP/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MFLOP/s";
            plot "tables/table_total_flops.dat" using 1:2 with lines smooth csplines t"Método Newton DP - T1",
                "tables/table_total_flops.dat"  using 1:3 with lines smooth csplines t"Método NewtonAVX - T1", 
                "tables/table_total_flops.dat"  using 1:4 with lines smooth csplines t"Método Newton DP - T2",
                "tables/table_total_flops.dat"  using 1:5 with lines smooth csplines t"Método Newton AVX - T2",
'

gnuplot -e '
            reset;
            set terminal png size 1366,600;
            set output "graphics/RUNTIME_MAT_METODO_NEWTON.png";
            set title "Runtime";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Segundos";
            plot "tables/table_total_runtime.dat" using 1:2 with lines smooth csplines t"Método Newton - T1", 
                "tables/table_total_runtime.dat"  using 1:3 with lines smooth csplines t"Método Newton - T2",
'