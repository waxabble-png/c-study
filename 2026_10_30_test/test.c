#define _CRT_SECURE_NO_WARNINGS
//#define PI 3.141593
//#include <stdio.h>
//#include <math.h>
//
//int main()
//{
//	int T = 0;
//	scanf("%d", &T);
//	switch (T)
//	{
//	case 1:
//		printf("I love Luogu!\n");
//		break;
//	case 2:
//		printf("6 4\n");
//		break;
//	case 3:
//	{
//		int a = 14 / 4;
//		int b = a * 4;
//		int c = 14 - b;
//		printf("%d %d %d", a, b, c);
//		break;
//	}
//	case 4:
//	{
//		double aver = 500.0 / 3.0;
//		printf("%.6f", aver);
//		break;
//	}
//	case 5:
//	{
//		int times = (260 + 220) / (12 + 20);
//		printf("%d\n", times);
//		break;
//	}
//	case 6:
//	{
//		int diagonal_pingfang = 9 * 9 + 6 * 6;
//		double diagonal = sqrt(diagonal_pingfang);
//		printf("%f", diagonal);
//		break;
//	}
//	case 7:
//	{
//		printf("110\n90\n0\n");
//		break;
//	}
//	case 8:
//	{
//		int r = 5;
//		double cycle = 2 * PI * r;
//		double sroud = PI * r * r;
//		double volum = 4 * PI * r * r * r / 3;
//		printf("%f\n%f\n%f\n", cycle, sroud, volum);
//		break;
//	}
//	case 9:
//	{
//		int number = (((1 + 1) * 2 + 1) * 2 + 1) * 2 + 1;
//		printf("%d\n", number);
//		break;
//	}
//	case 10:
//		printf("%d\n", 9);
//		break;
//	case 11:
//	{
//		double timess = 100 / 3;
//		printf("%f", timess);
//		break;
//	}
//	case 12:
//	{
//		printf("%d\n%c\n", 13, 'R');
//		break;
//	}
//	case 13:
//	{
//		int sum = 4 * 4 * 4 * 4 * PI / 3 + 10 * 10 * 10 * 4 * PI / 3;
//		int lengchang = cbrt(sum);
//		printf("%d\n", lengchang);
//		break;
//	}
//	case 14:
//		printf("%d\n", 50);
//		break;
//	}
//	return 0;
//}

//#include<stdio.h>
//
//int main()
//{
//	int m, t, s;
//	scanf("%d %d %d", &m, &t, &s);
//	if (t == 0)
//	{
//		printf("%d\n", 0);
//	}
//	else
//	{
//		int eaten = (s + t - 1) / t;
//		int rest = (eaten >= m) ? 0 : (m - eaten);
//		printf("%d\n", rest);
//	}
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//	int x = 0;
//	scanf("%d", &x);	
//	if (x % 2 == 0 && (x > 4 && x <= 12))
//		printf("1 ");
//	else
//		printf("0 ");
//	if (x % 2 == 0 || (x > 4 && x <= 12))
//		printf("1 ");
//	else
//		printf("0 ");
//	if ((x % 2 == 0 && (x <= 4 || x > 12)) || (x % 2 == 1 && x > 4 && x <= 12))
//		printf("1 ");
//	else
//		printf("0 ");
//	if (x % 2 == 1 && (x <= 4 || x > 12))
//		printf("1 ");
//	else
//		printf("0 ");
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//	int year = 0;
//	scanf("%d", &year);
//	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
//		printf("1\n");
//	else
//		printf("0\n");
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//	int x = 0;
//	scanf("%d", &x);
//	if (x == 1 || x == 0)
//		printf("Today,I ate %d apple.\n", x);
//	else
//		printf("Today,I ate %d apples.\n", x);
//	return 0;
//}


//#include <stdio.h>
//
//int main()
//{
//	int x = 0;
//	scanf("%d", &x);
//	if (x < 6)
//		printf("Local\n");
//	else
//		printf("Luogu\n");
//	return 0;
//}


//#include <stdio.h>
//
//int main()
//{
//	double m, h;
//	scanf("%lf %lf", &m, &h);
//	double BMI = m / h / h;
//	if (BMI < 18.5)
//		printf("Underweight\n");
//	else if (BMI >= 18.5 && BMI < 24)
//		printf("Normal\n");
//	else 
//	{
//		printf("%6f\n", BMI);
//		printf("Overweight\n");
//	}
//	return 0;
//

//#include<stdio.h>
//
//int main()
//{
//	int a, b, c;
//	scanf("%d %d %d", &a, &b, &c);
//
//	if (a >= b)
//	{
//		if (a >= c)
//		{
//			if (b >= c)
//				printf("%d %d %d", c, b, a);
//			else
//				printf("%d %d %d", b, c, a);
//		}
//		else
//			printf("%d %d %d", b, a, c);
//	}
//	else
//	{
//		if (a >= c)
//			printf("%d %d %d", c, a, b);
//		else 
//		{
//			if(b >= c)
//				printf("%d %d %d", a, c, b);
//			else
//				printf("%d %d %d", a, b, c);
//		}
//	}
//	return 0;
//}

//#include <stdio.h>
//
//void swap(int* x, int* y)//x和y交换位置
//{
//	int temp = 0;
//	temp = *x;
//	*x = *y;
//	*y = temp;
//}
//
//int main(void)
//{
//	int a, b, c;
//	scanf("%d %d %d", &a, &b, &c);
//
//	if (a > b)
//		swap(&a, &b);
//	if (a > c)
//		swap(&a, &c);
//	if (b > c)
//		swap(&b, &c);
//	printf("%d %d %d\n", a, b, c);
//}

//#include <stdio.h>
//
//int main()
//{
//	int year, month;
//	scanf("%d %d", &year, &month);
//	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
//	{
//		switch (month)
//		{
//		case 1:
//		case 3:
//		case 5:
//		case 7:
//		case 8:
//		case 10:
//		case 12:
//			printf("31\n");
//			break;
//		case 4:
//		case 6:
//		case 9:
//		case 11:
//			printf("30\n");
//			break;
//		case 2:
//			printf("29\n");
//			break;
//		default:
//			printf("请正确输入月份！\n");
//			break;
//		}
//	}
//	else
//	{
//		switch (month)
//		{
//		case 1:
//		case 3:
//		case 5:
//		case 7:
//		case 8:
//		case 10:
//		case 12:
//			printf("31\n");
//			break;
//		case 4:
//		case 6:
//		case 9:
//		case 11:
//			printf("30\n");
//			break;
//		case 2:
//			printf("28\n");
//			break;
//		default:
//			printf("请正确输入月份！\n");
//			break;
//		}
//	}
//}

#include <stdio.h>

int main() {
    int school, extra;
    int max_hours = 8; 
    int max_day = 0; 

    for (int i = 1; i <= 7; i++) {
        scanf("%d %d", &school, &extra);
        int total = school + extra; 

        if (total > 8 && total > max_hours) {
            max_hours = total; 
            max_day = i;       
        }
    }

    printf("%d\n", max_day);

    return 0;
}