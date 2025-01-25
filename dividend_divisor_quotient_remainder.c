#include <stdio.h>

main(){
	
	int dividend,divisor;
	int quotient,remainder;
	
	
	printf("Enter dividend and divisor respectively:");
	scanf("%d%d",&dividend,&divisor);
	
	quotient=dividend/divisor;
	
	remainder=dividend%divisor;
	
	printf("dividend is %d\ndivisor is %d\n",dividend,divisor);
	printf("quotient is %d\n",quotient);
	printf("remainder is %d",remainder);
	
}
