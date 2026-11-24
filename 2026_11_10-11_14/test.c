#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//int main(void)
//{
//	int arr[10] = { 0 };
//	for (int i = 0;i < 10; i++)
//	{
//		scanf("%d", &arr[i]);
//	}
//	int high = 0;
//	scanf("%d", &high);
//
//	int times = 0;
//	for (int i = 0; i < 10; i++)
//	{
//		if (high + 30 >= arr[i])
//			times++;
//	}
//	printf("%d\n", times);
//	return 0;
//}

//#include <stdio.h>
//
//void swap(int* x, int* y)
//{
//	int temp = *x;
//	*x = *y;
//	*y = temp;
//}
//
//int main()
//{
//	int a, b, c;
//	scanf("%d %d %d", &a, &b, &c);
//	char arr[] = "ABC";
//	scanf("%s", &arr);
//	if (a > b)
//	{
//		swap(&a, &b);
//	}
//	if (a > c)
//	{
//		swap(&a, &c);
//	}
//	if (b > c)
//	{
//		swap(&b, &c);
//	}
//	for (int i = 0; i < 3; i++)
//	{
//		if (arr[i] == 'A')
//			printf("%d", a);
//		else if (arr[i] == 'B')
//			printf("%d", b);
//		else
//			printf("%d", c);
//
//		if (i < 2)
//			printf(" ");
//	}
//	printf("\n");
//	return 0;
//}
//
//#include <stdio.h>
//
//int main()
//{
//	int t, v;
//	scanf("%d %d", &t, &v);
//	double time1 = t + 100.0 / v;
//	double time2 = t + 300.0 / v;
//	if (time2 <= 40)
//		printf("Malaban\n");
//	if (time1 <= 40 && time2 > 40)
//		printf("Xiqu\n");
//	if (time1 > 40)
//		printf("NOOOOO\n");
//	return 0;
//}

/*
#include <stdio.h>

int main()
{
    int n = 0;
    scanf("%d", &n);
    char S[10005];
    scanf("%s", S);
    int times = 0;
    scanf("%d", &times);

    int len = n;
    char temp[10005]; // 移到了外面

    while (times)
    {
        int op = 0;
        scanf("%d", &op);
        switch (op)
        {
        case 1:
        {
            int L, R, P;
            scanf("%d %d %d", &L, &R, &P);
            int sub_len = R - L + 1;

            for (int i = 0; i < sub_len; i++) {
                temp[i] = S[L - 1 + i];
            }

            for (int i = len - 1; i >= P; i--) {
                S[i + sub_len] = S[i];
            }

            for (int i = 0; i < sub_len; i++) {
                S[P + i] = temp[i];
            }

            len += sub_len;
            times--; // 补上
            break;
        }
        case 2:
        {
            int L, R, C;
            scanf("%d %d %d", &L, &R, &C);
            if (C % 2 == 1)
            {
                int left = L - 1;
                int right = R - 1;

                while (left < right)
                {
                    char temp_char = S[left];
                    S[left] = S[right];
                    S[right] = temp_char;

                    left++;
                    right--;
                }
            }
            times--;
            break;
        }
        case 3:
        {
            int L, R;
            scanf("%d %d", &L, &R);
            int sub_len = R - L + 1;

            for (int i = R; i < len; i++) {
                S[i - sub_len] = S[i];
            }

            len -= sub_len;
            times--; // 补上
            break;
        }
        }

        S[len] = '\0'; // 每次操作完立刻加上结束符
    }

    printf("%s\n", S);
    return 0;
}
*/

//#include <stdio.h>
//
//int main()
//{
//    int I, S, B;
//    char N;
//    scanf("%d-%d-%d-%c", &I, &S, &B, &N);
//    int sum1 = I + S / 100 * 2 + (S / 10 % 10) * 3 + S % 10 * 4;
//    int sum2 = B / 10000 * 5 + (B % 10000 / 1000) * 6 + (B % 1000 / 100) * 7
//        + (B % 100 / 10) * 8 + (B % 10) * 9;
//    int n = (sum1 + sum2) % 11;
//    if (N == 'n')
//        printf("Right\n");
//    else
//        printf("%d-%d-%d-%d", I, S, B, n);
//    return 0;
//}


//#include <stdio.h>
//#define MAXI 200000;
//
//int main()
//{
//    int n, m, s;
//    int color[200000];
//    scanf("%d %d %d", &n, &m, &s);
//    for (int i = 0; i < n;i++)
//    {
//        scanf("%d", &color[i]);
//    }
//
//    
//    return 0;
//}


//冒泡排序
//#include <stdio.h>
//
//void bubble_sort(int arr[], int sz)
//{
//    int i = 0;
//    for (i = 0;i < sz - 1;i++)
//    {
//        int j = 0;
//        for (j = 0;j < sz - i - 1;j++)
//        {
//            if (arr[j] > arr[j + 1])
//            {
//                int temp = arr[j];
//                arr[j] = arr[j + 1];
//                arr[j + 1] = temp;
//            }
//        }
//    }
//}
//
//void Print_int(int arr[], int sz) 
//{
//    for (int i = 0;i < sz;i++)
//    {
//        printf("%d ", arr[i]);
//    }
//    printf("\n");
//}
//
//int main()
//{
//    int arr[] = { 10,9,8,7,6,5,4,3,2,1 };
//    int sz = sizeof(arr) / sizeof(arr[0]);
//    Print_int(arr, sz);
//    bubble_sort(arr, sz);
//    Print_int(arr, sz);
//    return 0;
//}

//#include <stdio.h>
//#include <stdlib.h>
//
//int main()
//{
//    int* p = (int*)malloc(10 * sizeof(int));
//    int i = 0;
//    for (i = 0;i < 10;i++)
//    {
//        *(p + i) = i;
//    }
//
//    for (i = 0;i < 10;i++)
//    {
//        printf("%d ", *(p + i));
//    }
//    free(p);
//    p = NULL;
//    return 0;
//}

//#include <stdio.h>
//#include <stdlib.h>
//
//int bubble_sort(int* p, int n)
//{
//    int i = 0;
//        for (i = 0;i < n - 1;i++)
//        {
//            int j = 0;
//            for (j = 0;j < n - i - 1;j++)
//            {
//                if (*(p + j) > *( p + j + 1))
//                {
//                    int temp = *(p + j);
//                    *(p + j) = *(p + j + 1);
//                    *(p + j + 1) = temp;
//                }
//            }
//        }
//        return *(p);
//}
//
//int main()
//{
//    int n = 0;
//    scanf("%d", &n);
//    int* p = (int*)malloc(n * sizeof(int));
//    for (int i = 0;i < n;i++)
//    {
//        scanf("%d", (p + i));
//    }
//
//    int min = bubble_sort(p, n);
//    printf("%d\n", min);
//    free(p);
//    p = NULL;
//    return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//    int arr[10000];
//    int n = 0;
//    int k = 0;
//    scanf("%d %d", &n, &k);
//    int times = 0;
//    int j = 0;
//    int zhengchu[10000];
//    int buneng[10000];
//    for (int i = 0;i < n;i++)
//    {
//        arr[i] = i + 1;
//    }
//    int sum1 = 0;
//    int sum2 = 0;
//    for (int i = 0;i < n;i++)
//    {
//        if (arr[i] % k == 0)
//        {
//            zhengchu[times] = arr[i];
//            sum1 += zhengchu[times];
//            times++;
//        }
//        else
//        {
//            buneng[j] = arr[i];
//            sum2 += buneng[j];
//            j++;
//        }
//    }
//    double n1 = (double)sum1 / times;
//    double n2 = (double)sum2 / j;
//    printf("%.1f %.1f\n", n1, n2);
//    return 0;
//}

