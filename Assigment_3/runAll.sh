#!/bin/bash

# Definir el valor de 'limit' según tus requerimientos
limit=5
threads_list=(1 8 16 24 32 45 60 80)

# Arrays para guardar los tiempos mínimos y valores de n, y otras métricas
declare -a seqTimes
declare -a parallelTimes
declare -a nValues
declare -a speedUps

# Iniciar índice para arrays
index=0

# Iterar sobre los diferentes tamaños de n usando potencias de 2
for ((n=1<<13; n<=1<<22; n<<=1)); do
    nValues[index]=$n
    echo ""
    echo "Testing with n = $n"

    min_time_seq=999999
    for i in {1..3}; do
        # Ejecutar versión secuencial
        time_seq=$(./evenOddMergeSortSeq $n | grep -oP '(?<=Tiempo total: )\d+\.\d+')
        if (( $(echo "$time_seq < $min_time_seq" | bc -l) )); then
            min_time_seq=$time_seq
        fi
    done
    seqTimes[index]=$min_time_seq
    echo "Min Time Sequential: $min_time_seq s"
    echo "-------------------------------------"

    # Iterar a través de la lista de hilos
    for threads in ${threads_list[@]}; do
        min_time_par=999999
        for i in {1..3}; do
            # Ejecutar versión paralela
            time_par=$(./evenOddMergeSortPar $n $limit $threads | grep -oP '(?<=Tiempo total: )\d+\.\d+')
            if (( $(echo "$time_par < $min_time_par" | bc -l) )); then
                min_time_par=$time_par
            fi
        done
        parallelTimes[index]=$min_time_par

        # Calcular el speedUp
        speedUp=$(echo "$min_time_seq / $min_time_par" | bc -l)
        speedUps[index]=$speedUp

        echo "Number of threads: $threads"
        echo "Min Time Parallel: $min_time_par s"
        echo "SpeedUp: $speedUp"
        echo "-------------------------------------"
        ((index++))
    done
done

# Formatear y escribir los resultados en un archivo
printf "n = [%s]\n" "$(IFS=,; echo "${nValues[*]}")" > results.txt
printf "Threads = [%s]\n" "$(IFS=,; echo "${threads_list[*]}")" >> results.txt
printf "Seq = [%s]\n" "$(IFS=,; echo "${seqTimes[*]}")" >> results.txt
printf "Par = [%s]\n" "$(IFS=,; echo "${parallelTimes[*]}")" >> results.txt
printf "SpeedUp = [%s]\n" "$(IFS=,; echo "${speedUps[*]}")" >> results.txt

