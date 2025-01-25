#include <stdio.h>

main(){
	
    int t, h, n, s;
    

    for (t = 5; t >= 1; t--){
    
        for (h = 1; h <= 5 - t; h++){
            printf("  ");  
        }
        
        for (s = 1; s <= t; s++){
            printf("* ");
        }
        printf(" \n");
    }
}

