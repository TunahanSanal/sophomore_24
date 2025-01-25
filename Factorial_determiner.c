#include<stdio.h>

int main(void){
	
	int n,t,factorial;
	
	printf("Enter the t:\t");
	scanf("%d",&t);
	
	
	for(n=1;n<=t;n++)
		factorial=factorial*n;
		
		
	printf("Factorial = %d",factorial);
	
	return 0;
}
