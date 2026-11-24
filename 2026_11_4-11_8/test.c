#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//struct S 
//{
//	int data[1000];
//	int num;
//};
//
//void Print_s(struct S* s)
//{
//	int i = 0;
//	for (i = 0; i < 9; i++)
//	{
//		printf("%d ", s->data[i]);
//	}
//	printf("\n");
//	printf("%d\n", s->num);
//}
//
//int main()
//{
//	struct S s = { { 1,2,3,4,5,6,7,8,9 },123 };
//	Print_s(&s);
//}


//#include <stdio.h>
//
//void swap(int* x, int* y)
//{
//	int temp;
//	temp = *x;
//	*x = *y;
//	*y = temp;
//}
//
//int main()
//{
//	int a, b, c;
//	scanf("%d %d %d", &a, &b, &c);
//	if (a > b)
//		swap(&a, &b);
//	if (a > c)
//		swap(&a, &b);
//	if (b > c)
//		swap(&b, &c);
//	if (c < a + b)
//	{
//		if (c * c == a * a + b * b)
//			printf("Right triangle\n");
//		if (c * c < a * a + b * b)
//			printf("Acute triangle\n");
//		if (c * c > a * a + b * b)
//			printf("Obtuse triangle\n");
//		if (a == b || b == c || a == c)
//			printf("Isosceles triangle\n");
//		if (a == b && b == c && a == c)
//			printf("Equilateral triangle\n");
//	}
//	else
//		printf("Not triangle\n");
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//	int a = 0;
//	while (scanf("%d", &a) == 1)
//	{
//		int count = 0;
//		for (a; a / 10 != 0; a / 10)
//		{
//			if (a % 10 == 1)
//				count++;
//			if (a / 10 == 1)
//				count++;
//			a /= 10;
//		}
//		printf("%d\n", count);
//	}
//	return 0;
//}

//#include<stdio.h>
//
//int main()
//{
//	int sum = 0;
//	scanf("%d", &sum);
//	int A1, A2;
//	scanf("%d %d", &A1, &A2);
//	int B1, B2;	
//	scanf("%d %d", &B1, &B2);
//	int C1, C2;
//	scanf("%d %d", &C1, &C2);
//	int a, b, c;
//	a = (sum + A1 - 1) / A1;
//	b = (sum + B1 - 1) / B1;
//	c = (sum + C1 - 1) / C1;
//	int a_1, b_1, c_1;
//	a_1 = a * A2;
//	b_1 = b * B2;
//	c_1 = c * C2;
//	int min = a_1;
//	if (min > b_1)
//	{
//		min = b_1;	}
//	if (min > c_1)
//	{
//		min = c_1;
//	}
//	printf("%d\n", min);
//	return 0;
//}


//#include <stdio.h>
//#include <limits.h>
//
//int main()
//{
//	int sum, min;
//	scanf("%d", &sum);
//	int timess = 3;
//	min = INT_MAX;
//	for (timess = 3;timess > 0;timess--)
//	{
//		int number, price;
//		scanf("%d %d", &number, &price);
//		int cost, num;
//		num = (sum + number - 1) / number;
//		cost = num * price;
//		if (cost < min)
//			min = cost;
//	}
//	printf("%d\n", min);
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//	double sum, cost;
//	scanf("%lf", &sum);
//	if (sum > 150)
//	{
//		if (sum > 400)
//		{
//			cost = (sum - 400) * 0.5663 + 183.52;
//		}
//		else
//		{
//			cost = (sum - 150) * 0.4663 + 66.945;
//		}
//	}
//	else
//	{
//		cost = sum * 0.4463;
//	}
//	printf("%.1f\n", cost);
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//	int x, n;
//	int sum = 0;
//	scanf("%d %d", &x, &n);
//	for (int i = 0; i < n; i++)
//	{
//		if (x % 7 <= 5 && x % 7 >= 1)
//			sum += 250;
//		x++;
//	}
//	printf("%d\n", sum);
//	return 0;
//}

#include <stdio.h>

int gcd(int x, int y)
{
	if (y == 0)
		return x;
	return gcd(y, x % y);
}

int main()
{
	int a, b, c, min, max;
	scanf("%d %d %d", &a, &b, &c);
	min = a;
	max = a;
	if (b < min)
		min = b;
	if (c < min)
		min = c;
	if (b > max)
		max = b;
	if (c > max)
		max = c;
	int gcc = gcd(max, min);
	printf("%d/%d\n", min/gcc, max/gcc);
	return 0;
}