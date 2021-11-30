#!/bin/bash

# Cria estrutura de diretório se não existir
mkdir "tables"
mkdir "inputs_outputs"
mkdir "graphics"


# Compila os códigos fontes
gcc geraSL.c -lm 
make avx


# Prepara arquivos que vão receber as tabelas de dados
echo "Tamanho Gauss_Jacobi Gauss_Jacobi_OPT" > "tables/table-l3.dat"
cp "tables/table-l3.dat" "tables/table-l2cache.dat"
echo "Tamanho Gauss_Jacobi_FLOPS_DP Gauss_Jacobi_FLOPS_AVX Gauss_Jacobi_OPT_FLOPS_DP Gauss_Jacobi_OPT_FLOPS_AVX" > "tables/table-flops-dp.dat"


# Defini vetor de tamanhos dos SLs
TAMANHOS=("10" "32" "50" "64" "100" "128" "200" "250" "256" "300" "400" "512" "600" "1000" "1024" "2000" "2048" "3000" "4096")

# Gera tabela de dados
for i  in  ${TAMANHOS[*]}; do
    # Gera SL
    ./a.out $i > "inputs_outputs/input_$i.dat"

    # Insere linha - L3 Memory bandwidth [MBytes/s]: Gauss Jacobi - Gauss Jacobi OPT
    L3=$(likwid-perfctr -C 3 -m -g L3 ./gaussJacobi-likwid inputs_outputs/input_$i.dat | grep "L3 bandwidth" | awk -F'|' '{ printf "%s ", $3}')
    echo "$i  $L3" >> "tables/table-l3.dat"

    # Insere lina - L2 CACHE Data Cache Miss Ratio: Gauss Jacobi - Gauss Jacobi OPT
    L2CACHE=$(likwid-perfctr -C 3 -m -g L2CACHE ./gaussJacobi-likwid inputs_outputs/input_$i.dat | grep "L2 miss ratio" | awk -F'|' '{ printf "%s ", $3}')
    echo "$i $L2CACHE" >> "tables/table-l2cache.dat"

    # Insere Linha - FLOPS_DP e FLOPS_AVX em "MFLOP/s": Gauss Jacobi DP - Gauss Jacobi AVS - Gauss Jacobi OPT DP - Gauss Jacobi OPT AVX
    FLOPS_DP=$(likwid-perfctr -C 3 -m -g FLOPS_DP ./gaussJacobi-likwid inputs_outputs/input_$i.dat | grep "DP \[MFLOP\/s\]" | awk -F'|' '{ printf "%s ", $3}')
    echo "$i $FLOPS_DP" >> "tables/table-flops-dp.dat"
done

# Gera Gráficos
gnuplot -e '
            reset;
            set terminal png size 900,600;
            set output "graphics/L3_CACHE_BANDWIDTH.png";
            set title "L3 - Memory bandwidth [MBytes/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MBytes/s";
            plot "tables/table-l3.dat" using 1:2 with lines smooth csplines t"Gauss Jacobi", 
                "tables/table-l3.dat"  using 1:3 with lines smooth csplines t"Gauss Jacobi Otimizado";
'
gnuplot -e '
            reset;
            set terminal png size 900,600;
            set output "graphics/L2_CACHE_MISS_RATIO.png";
            set title "L2 - Data Cache Miss Ratio";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "Miss Ratio";
            plot "tables/table-l2cache.dat" using 1:2 with lines smooth csplines t"Gauss Jacobi", 
                "tables/table-l2cache.dat"  using 1:3 with lines smooth csplines t"Gauss Jacobi Otimizado";
'
gnuplot -e '
            reset;
            set terminal png size 900,600;
            set output "graphics/FLOPS_DP.png";
            set title "Flops DP e AVX [MFLOP/s]";
            set key outside right;
            set xlabel "Tamanho";
            set ylabel "MFLOP/s";
            plot "tables/table-flops-dp.dat" using 1:2 with lines smooth csplines t"Gauss Jacobi DP", 
                "tables/table-flops-dp.dat"  using 1:3 with lines smooth csplines t"Gauss Jacobi AVX", 
                "tables/table-flops-dp.dat"  using 1:4 with lines smooth csplines t"Gauss Jacobi Otimizado DP",
                "tables/table-flops-dp.dat"  using 1:5 with lines smooth csplines t"Gauss Jacobi Otimizado AVX";
'


# Limpa estrutura de diretórios
mv out_* "inputs_outputs/"
make purge