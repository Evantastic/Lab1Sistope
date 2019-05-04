#! /bin/zsh
OUTPUT="../resources/output"
INPUT="../resources/input.csv"
SEPARADOR="####################\n"
cd bin
printf "\n>Prueba con -b\n"
./lab1 -i $INPUT -o $OUTPUT -d 70 -n 3 -b
printf "\n>Contenidos del archivo creado\n"
printf $SEPARADOR
printf "Contenido del output\n"
cat $OUTPUT
printf $SEPARADOR
printf "\n>Prueba sin -b\n"
./lab1 -i $INPUT -o $OUTPUT -d 70 -n 3
printf $SEPARADOR
printf "Contenido del output\n"
cat $OUTPUT
printf $SEPARADOR
printf "\n>Prueba con parametros menos\n"
./lab1 -i $INPUT -o $OUTPUT -d 70 
printf "\n>Prueba con parametros sin valor\n"
./lab1 -i $INPUT -o $OUTPUT -d 70 -n
cd ..
