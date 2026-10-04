//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int num = 0;
	scanf("%d", &num);
	int a = num / 100;
	int b = (num - a * 100) / 50;
	int c = (num - a * 100 - b * 50) / 20;
	int d = (num - a * 100 - b * 50 - c * 20) / 10;
	int e = (num - a * 100 - b * 50 - c * 20 - d * 10) / 5;
	int f = num - a * 100 - b * 50 - c * 20 - d * 10 - e * 5;
	printf("100:%d\n50:%d\n20:%d\n10:%d\n5:%d\n1:%d\n", a, b, c, d, e, f);

	return 0;
}