#! /bin/zsh
printf "Prueba con -b\n"
./bin/lab1 -i ./resources/input1.csv -o ./resources/output1 -d 70 -n 3 -b
printf "Prueba sin -b\n"
./bin/lab1 -i ./resources/input1.csv -o ./resources/output1 -d 70 -n 3
printf "Prueba con parametros menos\n"
./bin/lab1 -i ./resources/input1.csv -o ./resources/output1 -d 70 
printf "Prueba con parametros sin valor\n"
./bin/lab1 -i ./resources/input1.csv -o ./resources/output1 -d 70 -n
