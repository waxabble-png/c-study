#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	char a;
	scanf("%c", &a);
	int b = a;
	char c = b - 32;
	int d = b - 'a'+1;
	char e = a + 3;
	printf("%c\n", c);
	printf("%d\n", d);
	printf("%c\n", e);


	return 0;
}