#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define MAXLENBUFFER 1000

int main(int argc, char ** argv){
  float realAverage = 0.0, imgAverage = 0.0, power = 0.0, noise = 0.0, k = 0.0;
  char *buffer = (char *) calloc(MAXLENBUFFER, sizeof(char));
  float u, v, r, i, n;
  while(fscanf(stdin, "%f,%f,%f,%f,%f\n", &u, &v, &r, &i, &n) != EOF){
    realAverage += r;
    imgAverage += i;
    //Preguntar sobre la formula de potencia
    power += sqrt(pow(r,2) + pow(i,2));
    noise += n;
    k++;
  }
  realAverage = realAverage/k;
  imgAverage = imgAverage/k;
  sprintf(buffer, "%f,%f,%f,%f,%d", realAverage, imgAverage, power, noise, (int) k);
  printf("%s",buffer);
  free(buffer);
  return 0;
}
