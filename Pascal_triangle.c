#include <stdio.h>

void pascal(int n);

int main(void){
	int row,i,value;
	
	printf("How many row are there:");
	scanf("%d",&row);
	
	if(row<=10){
		pascal(row);
	}
	else
		printf("Wrong Choice, Select 1-10");
		
	return 0;	
}

void pascal(int n){
	
	int	value=1,i;
		
	printf("Triangle row = %d\n",n);
		
	for (i=0;i<=n;i++){
		printf("%d",value);
		value=value*(n-i)/(i+1);
	}
	printf("\n");
	}

