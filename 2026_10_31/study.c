#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

//int factorial(int n)
//{
//	if (n == 0)
//		return 1;
//	else if (n > 0)
//	{
//		int ret = 1;
//		int i = 0;
//		for (i = 1;i <= n;i++)
//		{
//			ret *= i;
//		}
//		return ret;
//	}
//	else
//		return 0;
//}

//int factorial(int n)
//{
//	if (n == 0)
//		return 1;
//	else if (n >= 1)
//		return n * factorial(n - 1);
//	else
//		return 0;
//}
//int main()
//{
//	int x = 0;
//	scanf("%d", &x);
//	int a = factorial(x);
//	printf("%d", a);
//	return 0;
//}

//void Print(int n)
//{
//	if (n / 10 != 0)
//		Print(n / 10);
//		printf("%d ", n % 10);
//}
//
//int main()
//{
//	int n = 0;
//	scanf("%d", &n);
//	Print(n);
//	return 0;
//}

int feibonaqie(int n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	if (n >= 2)
		return feibonaqie(n - 1) + feibonaqie(n - 2);
}

int main()
{
	int n = 0;
	while (scanf("%d", &n) == 1)
	{
		printf("%d\n", feibonaqie(n));
	}
	return 0;
}