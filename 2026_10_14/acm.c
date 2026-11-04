#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int a, b, c;
	scanf("%d %d %d", &a, &b, &c);
	if ((a + b) > c)
	{
		if (c * c > a * a + b * b)
			printf("YES\n");
		else
			printf("NO\n");
	}
	else
		printf("NO\n");
	return 0;
}