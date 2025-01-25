#include <stdio.h>
#include <math.h>

main(){
	
	int n,x,i,first=1,second=1;
	double y,fib;
	
	printf("Please enter the n:");
	scanf("%d",&n);
	
	printf("Please enter the x:");
	scanf("%d",&x);
	
	for (i=1 ; i<=n ; i++){
		
		if (i==1)
			fib=1;
		
		else if (i==2)
			fib=1;
			
		else{
			fib=first+second;
			first=second;
			second=fib;
		}
			
		y=y+pow(i,x)/fib;
	}
	
	printf("The Results\n");
	printf("-------------------------\n");
	printf("Element of equation is %lf",y);
}
