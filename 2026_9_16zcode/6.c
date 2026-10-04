#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	char name[20];
	int age = 0;
	float grade = 0.00;
	scanf("%s%d%f", name, &age, &grade);
	printf("Name:%-10s\nAge:%5d\nScore:%8.2f\n", name, age, grade);
	return 0;
}

