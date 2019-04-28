#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char ** argv){
	double mediaReal = 0.0;
	double mediaImg = 0.0;
	double potencia = 0.0;
	double noise = 0.0;
	double k = 0.0;
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
	printf("mediaReal = %f, mediaImg = %f, potencia = %f,noise = %f,total= %f\n",mediaReal,mediaImg,potencia,noise,k);
	return 0;
}
