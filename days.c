#include <stdio.h> 




int main(){
    
//convert days to years, weeks, monthe

int days, monthe, years, weeks, day;
days=0;
 printf("Input days:\n");
 scanf("%d",&days);
 years=days/365;
 monthe=(days%365)/30;
 weeks=((days%365)%30)/7;
 day=((days%365)%30)%7;
 printf("In  days have %d Years, %d Monthe, %d Weeks, %d Day\n",years,monthe,weeks,day,__DATE__);
 
	return (0);
}
