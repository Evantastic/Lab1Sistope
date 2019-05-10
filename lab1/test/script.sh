#! /bin/zsh
OUTPUT="../resources/output"
LOGOUTPUT="../resources/log"
INPUT="../resources/input.csv"
FAILIOUTPUT="../resources/failOutput"
SEPARADOR="####################\n"
DISCNUMBER=8
RADIUM=25
cd bin
printf "\n>Prueba con -b\n"
./lab1 -i $INPUT -o $OUTPUT -d $RADIUM -n $DISCNUMBER -b>$LOGOUTPUT
printf "\n>Contenidos del archivo creado\n"
printf $SEPARADOR
printf "Contenido del output\n"
cat $OUTPUT
printf $SEPARADOR
printf "\n>Prueba sin -b\n"
./lab1 -i $INPUT -o $OUTPUT -d $RADIUM -n $DISCNUMBER
printf $SEPARADOR
printf "Contenido del output\n"
cat $OUTPUT
printf $SEPARADOR
printf "\n>Prueba con parametros menos\n"
./lab1 -i $INPUT -o $FAILOUTPUT -d $RADIUM 
printf "\n>Prueba con parametros sin valor\n"
./lab1 -i $INPUT -o $FAILOUTPUT -d $RADIUM -n
cd ..
