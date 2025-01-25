#include<stdio.h>

int main(void){
	
	int lesson,i,grade,total=0,passing_factor,average=0;
	
	
	printf("How many lessons do you have:\t");
	scanf("%d",&lesson);
	
	
	for(i=0;i<lesson;i++){
		printf("Please enter the grade:\t");
		scanf("%d",&grade);
		total=total+grade;
	}
	
	printf("The total grade is %d",total);
	
	passing_factor=45;
	
	average=total/lesson;
	
	printf("\nThe average grade is %d",average);
	
	if(average>passing_factor)
		printf("\nCONGRATS, YOU PASSED");
	else
		printf("\nSORRY, YOU FAILED");
	
	return 0;
}
