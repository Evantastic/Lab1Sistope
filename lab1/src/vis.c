#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define MAXLENBUFFER 1000

int main(int argc, char ** argv){
	double mediaReal = 0.0, mediaImg = 0.0, potencia = 0.0, noise = 0.0, k = 0.0;
  char *buffer = (char *) calloc(MAXLENBUFFER, sizeof(char));
	float u, v, r, i, n;
	while(fscanf(stdin, "%f,%f,%f,%f,%f\n", &u, &v, &r, &i, &n) != EOF){
		mediaReal = mediaReal + r;
		mediaImg = mediaImg + i;
		potencia = pow(r,2) + pow(i,2);
		noise = noise + n;
		k++;
	}
	mediaReal = mediaReal/k;
	mediaImg = mediaImg/k;
	potencia = sqrt(potencia);
  sprintf(buffer, "%lf,%lf,%lf,%lf,%d", mediaReal, mediaImg, potencia, noise, (int) k);
	printf("%s",buffer);
  free(buffer);
	return 0;
}
