#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
	Because is the same function between real and imaginary, only takes one function to do it.
*/
int media(int* realImaginary,int total){
	int sum = 0;
	for(int i=0;i<total;i++){
		sum = sum + realImaginary[i];
	}
	printf("pase\n");
	int result = sum / total;
	return result;
}

int potencia(int* real,int* imaginary, int total){
	int sum = 0;
	for(int i=0;i<total;i++){
		sum = sum + pow(real[i],2) + pow(imaginary[i],2);
	}
	int result = sqrt(sum);
	return result;
}

int ruidoTotal(int* noise, int total){
	int sum=0;
	for(int i=0;i<total;i++){
		sum = sum+noise[i];
	}
	return sum;
}


int main(int argc, char ** argv){
  	int* test = (int*)malloc(3*sizeof(int));
  	int* test2 = (int*)malloc(3*sizeof(int));
  	int j = 0;
  	for(int i=0;i<3;i++){
  		test[i] =j+1;
  		test2[i] = j +1;
  		j++; 
  	}
  	int n = media(test,3);
  	int n2 = potencia(test,test2,3);
  	printf("%i\n",n );
  	printf("%i\n",n2 );
  printf("Hola desde vis\n");
  return 0;
}
