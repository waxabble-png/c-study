#define _CRT_SECURE_NO_WARNINGS

#include "game.h"

void menu()
{
	printf("---------------------\n");
	printf("---------1.开始------\n");
	printf("---------0.退出------\n");
	printf("---------------------\n");
	printf("请输入你的选择：\n");
}

void game()
{
	// ===== 本局计时开始 =====
	// 每次从菜单选择“开始”都会重新调用 game()，因此这里的 start_time
	// 也是每一局全新的开始时间，不会把上一局的用时累计进来。
	// timespec_get 可以同时取得整秒和纳秒，后续可以显示到 0.1 秒。
	struct timespec start_time;
	timespec_get(&start_time, TIME_UTC);

	char shade[ROWS][LINS] = { 0 };
	char show[ROWS][LINS] = { 0 };
	set_array(shade, ROWS, LINS, '0');
	set_array(show, ROWS, LINS, '*');
	//print_array(shade, ROW, LIN);
	print_array(show, ROW, LIN);
	set_array_to_one(shade);
	// 把本局开始时间传给扫雷函数；find_lei() 会用它在每次输入坐标后
	// 显示“本次选取坐标用时”和“本次游戏累计用时”，游戏规则不变。
	find_lei(shade, show, ROW, LIN, start_time);

	// ===== 本局计时结束 =====
	// find_lei() 在“踩到雷”和“扫雷成功”两种情况下都会返回，
	// 所以在这里统一停止计时，两种结局都能得到完整的一局用时。
	struct timespec end_time;
	timespec_get(&end_time, TIME_UTC);
	double used_seconds = get_elapsed_seconds(start_time, end_time);
	// 这是整局结束时的最终提示；每次重新开始 game() 都会重新计时。
	printf("本次游戏共花费：%.1f 秒\n", used_seconds);
}

int main()
{
	srand((unsigned int)time(NULL));
	int input = 0;
	do
	{
		menu();
		if (scanf("%d", &input) != 1)
		{
			printf("输入无效请重新输入\n");
			while (getchar() != '\n');
			input = 1;
			continue;
		}
		switch (input)
		{
		case 1:
			game();
			break;
		case 0:
			break;
		default :
			printf("输入无效请重新输入\n");
		}
	} while (input);
	return 0;
}