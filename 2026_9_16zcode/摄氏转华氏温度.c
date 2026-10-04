#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	double degree = 0.0lf;
	scanf("%lf", &degree);
	double wen = degree * 9 / 5 + 32;
	printf("Temperature in Fahrenheit: %.2lf", wen);
	return 0;
}