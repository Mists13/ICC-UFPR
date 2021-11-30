#!/bin/bash

# Compila os códigos fontes
gcc geraSL.c -lm 
make avx

# Prepara arquivos que vão receber as tabelas de dados
echo "Tamanho Gauss_Jacobi Gauss_Jacobi_OPT" > "table-l3.dat"
cp table-l3.dat table-l2cache.dat
echo "Tamanho Gauss_Jacobi_FLOPS_DP Gauss_Jacobi_FLOPS_AVX Gauss_Jacobi_OPT_FLOPS_DP Gauss_Jacobi_OPT_FLOPS_AVX" > "table-flops-dp.dat"

# Defini vetor de tamanhos dos SLs
TAMANHOS=("10" "32" "50" "64" "100" "128" "200" "250" "256" "300" "400" "512" "600" "1000" "1024" "2000" "2048" "3000" "4096")

# Gera tabela de dados
for i  in  ${TAMANHOS[*]}; do

    # Gera SL
    ./a.out $i > "input_$i.dat"

    # Insere linha - L3 Memory bandwidth [MBytes/s]: Gauss Jacobi - Gauss Jacobi OPT
    L3=$(likwid-perfctr -C 3 -m -g L3 ./gaussJacobi-likwid input_$i.dat | grep "L3 bandwidth" | awk -F'|' '{ printf "%s ", $3}')
    echo "$i  $L3" >> "table-l3.dat"

    # Insere lina - L2 CACHE Data Cache Miss Ratio: Gauss Jacobi - Gauss Jacobi OPT
    L2CACHE=$(likwid-perfctr -C 3 -m -g L2CACHE ./gaussJacobi-likwid input_$i.dat | grep "L2 miss ratio" | awk -F'|' '{ printf "%s ", $3}')
    echo "$i $L2CACHE" >> "table-l2cache.dat"

    # Insere Linha - FLOPS_DP e FLOPS_AVX em "MFLOP/s": Gauss Jacobi DP - Gauss Jacobi AVS - Gauss Jacobi OPT DP - Gauss Jacobi OPT AVX
    FLOPS_DP=$(likwid-perfctr -C 3 -m -g FLOPS_DP ./gaussJacobi-likwid input_$i.dat | grep "DP \[MFLOP\/s\]" | awk -F'|' '{ printf "%s ", $3}')
    echo "$i $FLOPS_DP" >> "table-flops-dp.dat"
done

# Limpa arquivos 
make purge
rm -rf input*
rm -rf out*