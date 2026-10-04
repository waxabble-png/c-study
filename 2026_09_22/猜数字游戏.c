#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void game()
{
	srand(time(NULL));
	int neibu = rand() % 100 + 1;
	int guess = 0;
	int cishu = 6;
	while (cishu)
	{
		printf ("你还有%d次机会\n",cishu);
		printf("请输入你猜的数字吧，看看这次你的运气如何：\n");
		scanf("%d", &guess);

		if (guess > neibu)
		{
			printf("废物，你猜大了\n");
		}
		else if (guess < neibu)
		{
			printf("废物，你猜小了\n");
		}
		else
		{
			printf("哟，运气不错居然猜对了\n");
			break;
		}
		cishu--;
	}
	if (cishu == 0)
	printf("废物就是废物啊\n");
	printf("其实数字是%d\n", neibu);
}

int main()
{
	int num = 0;
	do
	{
		printf("---------- 1.开始 ---------\n");
		printf("---------- 0.退出 ---------\n");
		printf("这是猜数字游戏，请输入你的选择：");
		scanf("%d", &num);
		switch (num)
		{
		case 0:
			printf("你爱玩不玩，不玩拉倒\n");
			break;
		case 1:
			game();
			break;
		default:
			printf("你是不是眼瞎，一共就两个数字你™还瞎选\n");
			break;
		}
	
	}while (num);
	return 0;
}