#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//int main()
//{
//	char arr1[] = "Welcome to Bite !!!";
//	char arr2[] = "###################";
//	int left = 0;
//	int right = strlen(arr2) - 1;
//	printf("%s\r", arr2);
//	while (right >= left)
//	{
//		Sleep(1000);
//		arr2[left] = arr1[left];
//		arr2[right] = arr1[right];
//		left++;
//		right--;
//		printf("%s\r", arr2);
//	}
//	return 0;
//}

//int main()
//{
//	int arr1[] = {1,2,3,4,5,6,7,8,9,10};
//	int guess = 0;
//	scanf("%d", &guess);
//	int left = 0;
//	int right = sizeof(arr1) / sizeof(arr1[0]) - 1;
//	do
//	{
//		int mid = (right - left) / 2 + left;
//		if (guess > arr1[mid])
//		{
//			left = mid + 1;
//		}
//		else if (guess < arr1[mid])
//		{
//			right = mid - 1;
//		}
//		else
//		{
//			printf("找到了，下标是%d\n", mid);
//			break;
//		}
//	} while (left <= right);
//	if (left > right)
//		printf("找不到\n");
//	return 0;
//}

//int main()
//{
//	int n = 0;
//	while (scanf ("%d",&n) == 1)
//	{
//		srand((unsigned int)time(NULL));
//		char name[20][30] =
//		{
//			"林知远", "沈砚清", "苏晚棠", "陆时安", "叶疏桐", "顾星澜", "江枕月", "温屿川", "谢云舒", "裴照野", "许流萤", "宋鹤书", "楚听澜", "言安之", "白鹭洲", "孟昭雪", "蔺秋辞", "沈砚书", "余晚舟", "程知微"
//		};
//		int random1 = 0;
//		int random2 = 0;
//		do
//		{
//			random1 = rand() % 20;
//			random2 = rand() % 20;
//		} while (random1 == random2);
//		printf("学生1（%d）:%s\n", random1, name[random1]);
//		printf("学生2（%d）:%s\n", random2, name[random2]);
//	}
//	return 0;
//}

int mian()
{
	return 0;
}