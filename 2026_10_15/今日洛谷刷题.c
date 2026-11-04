#define _CRT_SECURE_NO_WARNINGS

//#include <stdio.h>
//
//int main(void)
//{
//	long long a, b, c;
//	scanf("%lld %lld", &a, &b);
//	c = a + b;
//	printf("%lld\n", c);
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//	int a, b, c;
//	scanf("%d %d %d", &a, &b, &c);
//	printf("%8d %8d %8d", a, b, c);
//	return 0;
//}


//#include <stdio.h>
//
//int main()
//{
//	int x, a, y, b;
//	scanf("%d %d %d %d", &x, &a, &y, &b);
//	double z = (double)(b * y - a * x) / (b - a);
//	printf("% .2f\n", z);
//	return 0;
//}
//#include <stdio.h>
//
//int main()
//{
//    double a;
//    scanf("%lf", &a);
//    double b;
//    int c = a;
//    double d = a - c;
//    b = d * 10 + c / 100 * 0.001 + c % 100 / 10 * 0.01 + c % 10 * 0.1;
//    printf("%.3f\n", b);
//    return 0;
//}

//求三角形面积
//#include <stdio.h>
//#include <math.h>
//
//int main()
//{
//	double a, b, c;
//	scanf("%lf %lf %lf", &a, &b, &c);
//	double p = (a + b + c) / 2;
//	double ss = p * (p - a) * (p - b) * (p - c);
//	double s = sqrt(ss);
//	printf("%.1f\n", s);
//
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//	int s, v;
//	scanf("%d %d", &s, &v);
//	int total = 470;
//	int t = (s + v - 1) / v;
//	int end = total - t;
//	printf("%02d:02%d\n", end / 60, end % 60);
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//	int h, r;
//	scanf("%d %d", &h, &r);
//	double v = 3.14 * r * r * h;
//	int sum = (v + 20000 - 1) / v;
//	printf("%d\n", sum);
//	return 0;
//}


//<#include <stdio.h>
//
//	int main(void)
//{
//	int a, b, c, d, e;
//	scanf("%d %d %d %d", &a, &b, &c, &d);
//	if ((c * 60 + d) >= (a * 60 + b))
//		e = c * 60 + d - a * 60 - b;
//	else
//		e = 60 * 24 + c * 60 + d - a * 60 - b;
//	printf("%d %d", e / 60, e % 60);
//	return 0;
//} 

//#include <stdio.h>
//
//int main()
//{
//    int a, b;
//    scanf("%d %d", &a, &b);
//    int num = (a * 10 + b) / 19;
//    printf("%d\n", num);
//    return 0;
//}

#include <stdio.h>

int main()
{
    unsigned int A, B, C;
    scanf("%u %u %u", &A, &B, &C);
    double total = A * 0.2 + B * 0.3 + C * 0.5;
    printf("%.0f\n", total);
    return 0;
}