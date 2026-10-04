#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//int main()
//{
//	int day = 0;
//	scanf("%d", &day);
//	switch (day)
//	{
//	case 1:
//		printf("Monday\n");
//		break;
//	case 2:
//		printf("Tuesday\n");
//		break;
//	case 3:
//		printf("Wednesday\n");
//		break;
//	case 4:
//		printf("Thursday\n");
//		break;
//	case 5:
//		printf("Friday\n");
//		break;
//	case 6:
//		printf("Saturday\n");
//		break;
//	case 7:
//		printf("Sunday\n");
//		break;
//	default :
//		printf("请输入正确的数字\n");
//		break;
//
//
//	}
//	return 0;
//}

//int main()
//{
//	int i = 0;
//	while (i <= 10)
//	{
//		printf("%d\n", i);
//		i = i + 1;
//	}
//	return 0;
//}

//for(初始化;判断;调整)  但是这三部分可以省略不写

//int main()
//{
//	int num = 0;
//	scanf("%d", &num);
//	int count = 0;
//	for (; num != 0; count++)
//	{
//		num = num / 10;
//	}
//	printf("%d\n", count );
//	return 0;
//}

//写出2-100所有质数
//int main()
//{
//	int num = 2;
//	for (num = 2;num <= 100;num++)
//	{
//		int m = 2;
//			for (m = 2;m <= num - 1;m++)
//			{
//				if (num % m == 0)
//					break;
//			}
//		if (num == m)
//		printf("%d ", num);
//	}
//	return 0;
//}


//从键盘输入一个整数，判断它是奇数还是偶数并输出。
//int main()
//{
//	int num = 0;
//	scanf("%d", &num);
//	if (num % 2 == 0)
//		printf("偶数 %d\n", num);
//	else
//		printf("奇数 %d\n", num);
//	return 0;
//}


//输入一个 1~7 的数字，用 switch 输出：1~5 输出"工作日"，6~7 输出"休息日"，其它数字输出"输入错误"。
//int main()
//{
//	int num = 0;
//	scanf("%d", &num);
//	switch (num)
//	{
//	case 1:
//	case 2:
//	case 3:
//	case 4:
//	case 5:
//		printf("工作日\n");
//		break;
//	case 6:
//	case 7:
//		printf("休息日\n");
//		break;
//	default :
//		printf("输入错误\n");
//		
//
//
//	}
//	return 0;



//输入一个正整数，逆序打印它的每一位。例如输入 1234，输出 4 3 2 1；输入 521，输出 1 2 5。
//int main()
//{
//	int num = 0;
//	scanf("%d", &num);
//	for (;num != 0;num /= 10)
//	{
//		printf("%d ", num %10);
//	}
//	return 0;
//}


//用循环嵌套找出 100~200 之间的所有素数并打印出来。

//int main()
//{
//	int num = 0;
//	for (num = 100;num <= 200;num++)
//	{
//		int a = 2;
//			for (;a < num;a++)
//			{
//				if (num % a == 0)
//					break;
//			}
//		if (num == a)
//			printf("%d ", num);
//	}
//	return 0;
//}

//
//① 用 for 循环计算 1~100 之间所有 3 的倍数的数字之和。
//② 用条件操作符（三目操作符）找出两个整数中的较大值。

//int main()
//{
//	int a = 1;
//	int sum = 0;
//
//	for (;a <= 100;a++)
//	{
//		if (a % 3 == 0)
//		{
//		
//		sum = a + sum;
//		}
//	}
//	printf("%d ", sum);
//
//	return 0;
//}

//int main()
//{
//	int a = 0;
//	int b = 0;
//	printf("请输入a b\n");
//	scanf("%d%d", &a, &b);
//	int i = (a > b ? a: b);
//	printf("最大的是%d\n", i);
//
//
//	return 0;
//}
/*  爱心祝福小程序 —— Windows 控制台版
 *  流程：打字机弹出祝福小贴士 -> 爱心逐渐放大铺满屏 -> 心跳三下
 *        -> 满屏撒星星 -> 定格告白
 */
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <time.h>
//#include <windows.h>
//
//HANDLE hOut;
//
//void setColor(int c) { SetConsoleTextAttribute(hOut, c); }
//
//void gotoxy(int x, int y) {
//    COORD p = { (SHORT)x, (SHORT)y };
//    SetConsoleCursorPosition(hOut, p);
//}
//
//void hideCursor(void) {
//    CONSOLE_CURSOR_INFO ci = { 1, FALSE };
//    SetConsoleCursorInfo(hOut, &ci);
//}
//
///* 打字机效果：逐字输出 */
//void typeText(int x, int y, const char* s, int ms) {
//    gotoxy(x, y);
//    while (*s) {
//        putchar(*s++);
//        fflush(stdout);
//        Sleep(ms);
//    }
//}
//
///* 1. 连环祝福小贴士（想说什么自己改这里） */
//void showWishes(void) {
//    const char* wish[] = {
//        "叮咚~ 你的专属祝福已送达：",
//        "祝你天天开心，烦恼清零！",
//        "祝你钱包鼓鼓，好运爆棚！",
//        "祝你吃嘛嘛香，一夜好眠！",
//        "祝你被这个世界温柔以待~"
//    };
//    int i, n = sizeof(wish) / sizeof(wish[0]);
//
//    for (i = 0; i < n; i++) {
//        system("cls");
//        setColor(11);
//        typeText(26, 12, wish[i], 90);  /* 90 = 每个字间隔毫秒 */
//        Sleep(700);                     /* 每条停留时间 */
//    }
//    Sleep(900);
//}
//
///* 2. 画心形：scale 控制大小，用经典心形曲线公式填充 */
//void drawHeart(double scale, int color) {
//    int row, col;
//    double x, y, xx, yy, a;
//
//    gotoxy(0, 0);            /* 覆盖式重画，防止闪烁 */
//    setColor(color);
//
//    for (row = 0; row < 25; row++) {
//        y = 1.35 - row * 0.105;
//        for (col = 0; col < 75; col++) {
//            x = -1.8 + col * 0.048;
//            xx = x / scale;
//            yy = y / scale;
//            a = xx * xx + yy * yy - 1;
//            putchar(a * a * a - xx * xx * yy * yy * yy <= 0 ? '*' : ' ');
//        }
//        putchar('\n');
//    }
//}
//
///* 4. 满屏撒小星星 */
//void starRain(void) {
//    int i;
//    srand((unsigned)time(NULL));
//    for (i = 0; i < 250; i++) {
//        gotoxy(rand() % 75, rand() % 25);
//        setColor(11 + rand() % 4);   /* 青/红/粉/黄 随机 */
//        printf("*");
//        Sleep(15);
//    }
//}
//
//int main(void) {
//    int i, c = 0;
//    double s;
//    int colors[4] = { 12, 13, 14, 11 };  /* 红 粉 黄 青 轮流换色 */
//
//    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//    system("mode con cols=80 lines=30"); /* 设置窗口大小 */
//    SetConsoleTitleA("For You <3");
//    hideCursor();
//
//    showWishes();                        /* 1. 祝福小贴士 */
//
//    system("cls");
//    for (s = 0.25; s <= 1.45; s += 0.15) {  /* 2. 爱心从小变大 */
//        drawHeart(s, colors[c++ % 4]);
//        Sleep(260);
//    }
//
//    for (i = 0; i < 3; i++) {            /* 3. 心跳三下 */
//        drawHeart(1.25, 12); Sleep(160);
//        drawHeart(1.45, 13); Sleep(160);
//    }
//
//    starRain();                          /* 4. 满屏星星 */
//
//    setColor(14);
//    typeText(29, 11, "LOVE YOU FOREVER", 120);  /* 5. 定格告白 */
//
//    gotoxy(0, 28);
//    getchar();   /* 停住画面，回车退出 */
//    return 0;
//}





#include <stdio.h>
#include <windows.h>

int main()
{
    MessageBox(NULL, "好好吃饭", "思念", MB_OK);
    MessageBox(NULL, "我想你了", "思念", MB_OK);
    MessageBox(NULL, "保持好心情", "思念", MB_OK);
    MessageBox(NULL, "代码只有三十多行，我对你的思念却不止于此", "❤", MB_OK);
    return 0;
}