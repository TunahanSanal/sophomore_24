#include<stdio.h>

main(){
	
	int a,b,c;
	
	printf("ENTER THE A,B,C:");
	scanf("%d%d%d",&a,&b,&c);
	
	if(a>b && a>c)
		printf("%d is the greatest",a);
	if(b>a && b>c)
		printf("%d is the greatest",b);
	if(c>a && c>b)
		printf("%d is the greatest",c);
	
	
}
