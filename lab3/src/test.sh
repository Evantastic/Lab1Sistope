print ">>> Ejecución correcta con log"
./lab3 -u ../resources/up -d ../resources/down -o ../resources/usual -n 12 -e 1 -b
print "\n"
print ">>> Ejecución correcta sin log"
./lab3 -u ../resources/up -d ../resources/down -o ../resources/usual -n 12 -e 1
print "\n"
print ">>> Ejecución con Parametros extras"
./lab3 -u ../resources/up -d ../resources/down -o ../resources/usual -n 12 -e 1 -b -a wena
print "\n"
print ">>> Ejecución con parametros menos"
./lab3 -u ../resources/up -d ../resources/down -o ../resources/usual -n 12 -b
print "\n"
print ">>> Ejecución con archivos equivocados"
./lab3 -u up -d down -o usual -n 12 -e 1 
print "\n"
