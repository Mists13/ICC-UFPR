#!/bin/bash

# Compila e gera os inputs
cd "T1/"
make 
cd "../T2/"
make
cd "../"
./broyden.sh

mkdir "tables"

# Monta cabeçalhos das tables
echo "TAMANHO MAT_DERIV_PACIAL_DP_T1 MAT_DERIV_PACIAL_AVX_T1 TOTAL_T1_DP TOTAL_T1_AVX MAT_JACOBI_T1_DP MAT_JACOBI_T1_AVX RESOLUCAO_T1_DP RESOLUCAO_T1_AVX" > "tables/table_FLOPS.dat"
echo "TAMANHO MAT_DERIV_PACIAL_T1 TOTAL_T1 MAT_JACOBI_T1 RESOLUCAO_T1" > "tables/table_L3.dat"
cp "tables/table_L3.dat" "tables/table_L2CACHE.dat"
cp "tables/table_L3.dat" "tables/table_RUNTIME.dat"

# Gera tabelas
for N in 10 32 50 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096
do
    TIME_T1=$(likwid-perfctr -C 3 -m -g FLOPS_DP T1/newtonSNL < inputs_outputs/input_$N.in | grep "RDTSC Runtime" | awk -F'|' '{ printf "%s ", $3}')
    FLOPS_T1=$(likwid-perfctr -C 3 -m -g FLOPS_DP T1/newtonSNL < inputs_outputs/input_$N.in | grep "MFLOP" | awk -F'|' '{ printf "%s ", $3}')
    L3_T1=$(likwid-perfctr -C 3 -m -g L3 T1/newtonSNL < inputs_outputs/input_$N.in | grep "L3 bandwidth" | awk -F'|' '{ printf "%s ", $3}')
    L2_CACHE=$(likwid-perfctr -C 3 -m -g L2CACHE T1/newtonSNL < inputs_outputs/input_$N.in |  grep "L2 miss ratio" | awk -F'|' '{ printf "%s ", $3}')

    echo "$N $TIME_T1" >> "tables/table_RUNTIME.dat"
    echo "$N $FLOPS_T1" >> "tables/table_FLOPS.dat"
    echo "$N $L3_T1" >> "tables/table_L3.dat"
    echo "$N $L2_CACHE" >> "tables/table_L2CACHE.dat"
done

# Gera gŕaficos através das tabelas
./graphics.sh


# Limpa diretórios
cd "T1/"
make clean 
cd "../T2/"
make clean