#!/bin/bash

# Compila e gera os inputs
cd "T1/"
make 
cd "../T2/"
make avx
cd "../"

./broyden.sh

mkdir "tables"

# TAMANHO MAT_DERIV_PACIAL_DP_T1 MAT_DERIV_PACIAL_AVX_T1 TOTAL_T1_DP TOTAL_T1_AVX MAT_JACOBI_T1_DP MAT_JACOBI_T1_AVX RESOLUCAO_T1_DP RESOLUCAO_T1_AVX

# Monta cabeçalhos das tables
echo "TAMANHO MAT_DERIV_PARCIAL_DP_T1 MAT_DERIV_PARCIAL_AVX_T1 MAT_DERIV_PARCIAL_DP_T2 MAT_DERIV_PARCIAL_AVX_T2" > "tables/table_mat_deriv_parcial_flops.dat"
echo "TAMANHO MAT_DERIV_PARCIAL_T1 MAT_DERIV_PARCIAL_T2" > "tables/table_mat_deriv_parcial_runtime.dat"  
cp "tables/table_mat_deriv_parcial_runtime.dat" "tables/table_mat_deriv_parcial_l3.dat" 
cp "tables/table_mat_deriv_parcial_runtime.dat" "tables/table_mat_deriv_parcial_l2cache.dat"

echo "TAMANHO TOTAL_T1 TOTAL_AVX_T1 TOTAL_DP_T2 TOTAL_AVX_T2" > "tables/table_total_flops.dat"
echo "TAMANHO TOTAL_T1 TOTAL_T2" > "tables/table_total_runtime.dat"
cp "tables/table_total_runtime.dat" "tables/table_total_l3.dat"
cp "tables/table_total_runtime.dat"  "tables/table_total_l2cache.dat"

echo "TAMANHO JACOBIANA_T1 JACOBIANA_AVX_T1 JACOBIANA_DP_T2 JACOBIANA_AVX_T2" > "tables/table_jacobiana_flops.dat"
echo "TAMANHO JACOBIANA_T1 JACOBIANA_T2" > "tables/table_jacobiana_runtime.dat"
cp "tables/table_total_runtime.dat" "tables/table_jacobiana_l3.dat"
cp "tables/table_total_runtime.dat" "tables/table_jacobiana_l2cache.dat"

echo "TAMANHO RESOLUCAO_T1 RESOLUCAO_AVX_T1 RESOLUCAO_DP_T2 RESOLUCAO_AVX_T2" > "tables/table_resolucao_flops.dat"
echo "TAMANHO RESOLUCAO_T1 RESOLUCAO_T2" > "tables/table_resolucao_runtime.dat"
cp "tables/table_resolucao_runtime.dat" "tables/table_resolucao_l3.dat"
cp "tables/table_resolucao_runtime.dat" "tables/table_resolucao_l2cache.dat"

# Gera tabelas
for N in 10 32 50 # 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096
do
    # Armazena parâmetros da execução do T1 e T2 de acordo com a entrada e grupos
    FLOPS_T1=$(likwid-perfctr -C 3 -m -g FLOPS_DP T1/newtonSNL < inputs_outputs/input_$N.in | grep "MFLOP" | awk -F'|' '{ printf "%s ", $3}')
    FLOPS_T2=$(likwid-perfctr -C 3 -m -g FLOPS_DP T2/newtonSNL < inputs_outputs/input_$N.in | grep "MFLOP" | awk -F'|' '{ printf "%s ", $3}')
    TIME_T1=$(likwid-perfctr -C 3 -m -g FLOPS_DP T1/newtonSNL < inputs_outputs/input_$N.in | grep "RDTSC Runtime" | awk -F'|' '{ printf "%s ", $3}')
    TIME_T2=$(likwid-perfctr -C 3 -m -g FLOPS_DP T2/newtonSNL < inputs_outputs/input_$N.in | grep "RDTSC Runtime" | awk -F'|' '{ printf "%s ", $3}')
    L3_T1=$(likwid-perfctr -C 3 -m -g L3 T1/newtonSNL < inputs_outputs/input_$N.in | grep "L3 bandwidth" | awk -F'|' '{ printf "%s ", $3}')
    L3_T2=$(likwid-perfctr -C 3 -m -g L3 T2/newtonSNL < inputs_outputs/input_$N.in | grep "L3 bandwidth" | awk -F'|' '{ printf "%s ", $3}')
    L2_CACHE_T1=$(likwid-perfctr -C 3 -m -g L2CACHE T1/newtonSNL < inputs_outputs/input_$N.in |  grep "L2 miss ratio" | awk -F'|' '{ printf "%s ", $3}')
    L2_CACHE_T2=$(likwid-perfctr -C 3 -m -g L2CACHE T2/newtonSNL < inputs_outputs/input_$N.in |  grep "L2 miss ratio" | awk -F'|' '{ printf "%s ", $3}')

    # Gera tabelas de parâmetros referente a Matriz de Derivada Parcial
    echo "$N $(echo $TIME_T1 | awk '{ printf $1 }') $(echo $TIME_T2 | awk '{ printf $1 }')" >> "tables/table_mat_deriv_parcial_runtime.dat" 
    echo "$N $(echo $L3_T1 | awk '{ printf $1 }') $(echo $L3_T2 | awk '{ printf $1 }')" >> "tables/table_mat_deriv_parcial_l3.dat" 
    echo "$N $(echo $L2_CACHE_T1 | awk '{ printf $1 }') $(echo $L2_CACHE_T2 | awk '{ printf $1 }')" >> "tables/table_mat_deriv_parcial_l2cache.dat" 
    echo "$N $(echo $FLOPS_T1 | awk '{ printf "%s %s", $1, $2 }') $(echo $FLOPS_T2 | awk '{ printf "%s %s", $1, $2 }')" >> "tables/table_mat_deriv_parcial_flops.dat"

    # Gera tabelas de parâmetros referente a apliacação total do Método de Newton
    echo "$N $(echo $TIME_T1 | awk '{ printf $2 }') $(echo $TIME_T2 | awk '{ printf $2 }')" >> "tables/table_total_runtime.dat"
    echo "$N $(echo $L3_T1 | awk '{ printf $2 }') $(echo $L3_T2 | awk '{ printf $2 }')" >> "tables/table_total_l3.dat" 
    echo "$N $(echo $L2_CACHE_T1 | awk '{ printf $2 }') $(echo $L2_CACHE_T2 | awk '{ printf $2 }')" >> "tables/table_total_l2cache.dat"
    echo "$N $(echo $FLOPS_T1 | awk '{ printf "%s %s", $3, $4 }') $(echo $FLOPS_T2 | awk '{ printf "%s %s", $3, $4 }')" >> "tables/table_total_flops.dat"

    # Gera tabelas de parâmetros referente a Matriz Jacobiana
    echo "$N $(echo $TIME_T1 | awk '{ printf $3 }') $(echo $TIME_T2 | awk '{ printf $3 }')" >> "tables/table_jacobiana_runtime.dat"
    echo "$N $(echo $L3_T1 | awk '{ printf $3 }') $(echo $L3_T2 | awk '{ printf $3 }')" >> "tables/table_jacobiana_l3.dat"
    echo "$N $(echo $L2_CACHE_T1 | awk '{ printf $3 }') $(echo $L2_CACHE_T2 | awk '{ printf $3 }')" >> "tables/table_jacobiana_l2cache.dat"
    echo "$N $(echo $FLOPS_T1 | awk '{ printf "%s %s", $5, $6 }') $(echo $FLOPS_T2 | awk '{ printf "%s %s", $5, $6 }')" >> "tables/table_jacobiana_flops.dat"

    # Gera tabelas de parâmetros referente a resolucao do SNL
    echo "$N $(echo $TIME_T1 | awk '{ printf $4 }') $(echo $TIME_T2 | awk '{ printf $4 }')" >> "tables/table_resolucao_runtime.dat"
    echo "$N $(echo $L3_T1 | awk '{ printf $4 }') $(echo $L3_T2 | awk '{ printf $4 }')" >> "tables/table_resolucao_l3.dat"
    echo "$N $(echo $L2_CACHE_T1 | awk '{ printf $4 }') $(echo $L2_CACHE_T2 | awk '{ printf $4 }')" >> "tables/table_resolucao_l2cache.dat"
    echo "$N $(echo $FLOPS_T1 | awk '{ printf "%s %s", $7, $8 }') $(echo $FLOPS_T2 | awk '{ printf "%s %s", $7, $8 }')" >> "tables/table_resolucao_flops.dat"
done

# Limpa diretórios
cd "T1/"
make purge 
cd "../T2/"
make purge