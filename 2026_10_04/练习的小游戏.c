#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mune()
{
	printf("--------------------------\n");
	printf("---------1.开始游戏-------\n");
	printf("---------0.退出游戏-------\n");
	printf("--------------------------\n");
	printf("请输入你的选择；");
}

void game()
{
	int r = rand() % 100 + 1;   // 目标数字
	int guess = 0;              // 玩家猜的数字
	int num = 10;               // 剩余机会
	double shijian = 0.0;       // 时间变量：放在循环外，只初始化一次

	clock_t start = clock();    // 开始计时

	printf("请输入1~100 的数字：\n");
	scanf("%d", &guess);

	while (num > 0)
	{
		if (guess > r)
			printf("猜大了\n");
		else if (guess < r)
			printf("猜小了\n");
		else
		{
			// 猜对，算出总用时
			shijian = (double)(clock() - start) / CLOCKS_PER_SEC;
			printf("恭喜你猜对了！\n");
			printf("共用时%.2f秒\n", shijian);
			break;
		}

		num--;
		if (num == 0)
		{
			shijian = (double)(clock() - start) / CLOCKS_PER_SEC;
			printf("机会用完了，正确答案是%d\n", r);
			printf("共用时%.2f秒\n", shijian);
			break;
		}

		printf("你还有%d次机会\n", num);
		printf("请输入你猜的数字：\n");
		scanf("%d", &guess);
	}
}

int main()
{
	int input = 0;

	srand((unsigned int)time(NULL));   // 只播种一次，放在循环外

	do
	{
		mune();
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			game();
			break;
		case 0:
			printf("退出\n");
			break;
		default:
			printf("输入错误，请从新输入\n");
			break;
		}

	} while (input);

	return 0;
}