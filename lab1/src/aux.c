#include "header.h"

struct flags *processArgv(int argc, char **argv){
  int currentFlag = 0;
  struct flags *options = (struct flags *) calloc(1, sizeof(struct flags));
  while((currentFlag = getopt(argc, argv, "i:o:d:n:b")) != -1){
    switch(currentFlag){
      case 'i':
        strcpy(options->inputFile, optarg);
        break;
      case 'o':
        strcpy(options->outputFile, optarg);
        break;
      case 'd':
        options->discRadium = atoi(optarg);
        break;
      case 'n':
        options->discQuantity = atoi(optarg);
        break;
      case 'b':
        options->log = TRUE;
        break;
      default:
        return NULL;
    }
  }
  return options;
}

struct process *createProcess(struct flags *options){
  struct process *viss = (struct process *) calloc(options->discQuantity, sizeof(struct process));
  for(int i = 0; i < options->discQuantity; i++){
    pipe(viss[i].pipes[0]);
    pipe(viss[i].pipes[1]);
    viss[i].pid = fork();
    if(viss[i].pid < 0){
      return NULL;
    }
    else if(viss[i].pid > 0){
      close(viss[i].pipes[FIN][OUTPUT]);
      close(viss[i].pipes[FOUT][INPUT]);
    }
    else{
      close(viss[i].pipes[SIN][OUTPUT]);
      close(viss[i].pipes[SOUT][INPUT]);
      dup2(viss[i].pipes[SIN][INPUT], STDIN_FILENO);
      dup2(viss[i].pipes[SOUT][OUTPUT], STDOUT_FILENO);
      execl("./vis","./vis",(char *) NULL);
      return NULL;
    }
  }
  return viss;
}

FILE **openFiles(struct flags *options){
  FILE **files;
  files = (FILE **) calloc(2, sizeof(FILE *));
  files[0] = fopen(options->inputFile, "r");
  files[1] = fopen(options->outputFile, "w");
  if(files[0] == NULL || files[1] == NULL)
    return NULL;
  return files;
}

char checkOptions(struct flags*options){
  char state = TRUE;
  state = state && options->inputFile;
  state = state && options->outputFile;
  state = state && options->discQuantity > 0;
  state = state && options->discRadium > 0;
  return state;
}

void readFile(struct flags *options, FILE *input, struct process *viss){
  float u, v, r, i, n;
  int position;
  char buffer[MAXLENBUFFER];
  while(fgets(buffer, MAXLENBUFFER, input) != NULL){
    sscanf(buffer, "%f,%f,%f,%f,%f\n", &u, &v, &r, &i, &n);
    position = floor(DISC(u, v, options->discRadium));
    position = position >= options->discQuantity ? options->discQuantity - 1: position;
    write(viss[position].pipes[FOUT][OUTPUT], buffer, strlen(buffer));
  }
}

struct process *getStatistics(struct flags *options, struct process *viss){
  char buffer[MAXLENBUFFER];
  for(int i = 0; i < options->discQuantity; i++){
    memset(buffer, 0, MAXLENBUFFER);
    read(viss[i].pipes[FIN][INPUT], buffer, MAXLENBUFFER);
    sscanf(buffer,"%lf,%lf,%lf,%lf,%d", &viss[i].realAverage, &viss[i].imgAverage, &viss[i].power, &viss[i].noise, &viss[i].quantity);
  }
  return viss;
}

void printStatistics(struct flags *options, FILE *output, struct process *viss){
  for(int i = 0; i < options->discQuantity; i++){
    fprintf(output,"Disco %d\n",i+1);
    fprintf(output, "Media Real: %lf\nMedia Imaginaria: %lf\nPotencia: %lf\nRuido Total: %lf\n", viss[i].realAverage, viss[i].imgAverage, viss[i].power, viss[i].noise);
  }
}

void printQuantities(struct flags *options, struct process *viss){
  if(!options->log)
    return;
  for(int i = 0; i < options->discQuantity; i++){
    printf("Proceso con PID %d procesa %d visibilidades\n", viss[i].pid, viss[i].quantity);
  }
}

void freeMemory(struct flags *options, struct process *viss){
  free(options);
  free(viss);
}

void closeFiles(FILE **files){
  fclose(files[INPUT]);
  fclose(files[OUTPUT]);
}

