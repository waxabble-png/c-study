#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int num = 0;
	scanf("%d", &num);
	int a = num / 100;
	int b = num / 10 % 10;
	int c = num % 10;
	int num1 = c * 100 + b * 10 + a;
	printf("%d\n", num1);
	return 0;
}