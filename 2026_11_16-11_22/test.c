#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//int main()
//{
//	int a = 0, j = 0;
//	scanf("%d", &a);
//	for (int i = 0;a != 0;a /= 2)
//	{
//		j++;
//	}
//	printf("%d\n", j);
//	return 0;
//}
//
//#include <stdio.h>
//#include <stdlib.h>
//
//int main()
//{
//    int n = 0;
//    scanf("%d", &n);
//    int num = n * (n + 1) / 2;
//    int* p = (int*)malloc(num * sizeof(int));
//    if (p == NULL)
//        return 1;
//    else
//    {
//        for (int i = 0; i < num; i++)
//        {
//            *(p + i) = i + 1;
//        }
//    }
//    int ptr = 0;
//    for (; n != 0; num--)             
//    {
//        int i = 0, times = 0;
//        for (; i < n; i++)
//        {
//            printf("%02d", *(p + ptr + i));   
//            times++;
//        }
//        printf("\n");
//        ptr += n;
//        n--;
//    }
//    free(p);
//    return 0;
//}

#include <stdio.h>

int jiecheng(int n) 
{
    long long int s = 1;
    for (int i = 0;i < n;i++)
    {
        s *= (i + 1);
    }
    return s;
}


int main()
{
    int n = 0;
    scanf("%d", &n);
    long long int s, S = 0;
    for (int i = 0;i < n;i++)
    {
        s = jiecheng(i + 1);
        S += s;
    }
    printf("%lld\n", S);
    return 0;
}