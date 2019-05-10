#include "header.h"

int main(int argc, char **argv){
  struct flags *options;
  FILE **files;
  struct process *viss;
  int status;

  options = processArgv(argc, argv);
  files = openFiles(options);
  if(options == NULL){
    EXIT("Error procesando los parametros", 1);
  }
  else if(!checkOptions(options)){
    EXIT("Error procesando los parametros", 1);
  }
  else if(files == NULL){
    EXIT("Error procesando archivos", 2);
  }
  viss = createProcess(options);
  if(viss == NULL){
    EXIT("Error creando procesos", 3);
  }
  readFile(options, files[INPUT], viss);
  ENDOUTPUTS(options->discQuantity, viss);
  getStatistics(options, viss);
  ENDINPUTS(options->discQuantity, viss);
  WAITPIDS(options->discQuantity, viss, &status);
  printStatistics(options, files[OUTPUT], viss);
  printQuantities(options, viss);
  freeMemory(options, viss);
  closeFiles(files);
  EXIT("Ejecucion completada", 0);
}
