#include <math.h>
#include <stdio.h>

#define DEGREE	3.1415926535/180.0

int main(int argc, char **argv)
{
	int i;
	printf("int tg_fp[90]={");
	for(i=0;i<90;i++)
	{
		int val = (int)(2048*tan(DEGREE*i));
		if(i!=89)
			printf("%d, ", val);
		else
			printf("%d}\n", val);
	}
	printf("int sin_fp[360]={");
	for(i=0;i<360;i++)
	{
		int val = (int)(2048*sin(DEGREE*i));
		if(i!=359)
			printf("%d, ", val);
		else
			printf("%d}\n", val);
	}
	printf("int cos_fp[360]={");
	for(i=0;i<360;i++)
	{
		int val = (int)(2048*cos(DEGREE*i));
		if(i!=359)
			printf("%d, ", val);
		else
			printf("%d}\n", val);
	}
	
}
