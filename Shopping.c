#include <stdio.h>

void calculate(int s);

main(){
	
	double shop_amount,discount=0.0,last_price;
	
	printf("What is the amount of shopping:");
	scanf("%lf",&shop_amount);
	
	calculate(shop_amount);
	
}

void calculate(int s){
	
	double discount=0.0,last_price;
	
	if(s>=500)
    	discount=10.0;
    else if(s>=400)
    	discount=8.0;
    else if(s>=200)
    	discount=6.0;
    else
    	discount=2.0;
    	
    last_price=s*(1-(discount/100));
    
    printf("Shopping Amount = %d\n",s);
    printf("Discount Rate = %lf\n",discount);
    printf("Price with discount = %lf",last_price);
}
