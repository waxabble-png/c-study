#define _CRT_SECURE_NO_WARNINGS
#include "game.h"

void print_array(char arr[ROWS][LINS], int rows, int lins)
{
	int i = 0;
	printf("扫雷游戏正式开始：\n");
	for (i = 0;i <= rows; i++)
	{
		printf("%d ", i);
	}
	printf("\n");

	for (i = 1; i <= rows; i++)
	{
		printf("%d ", i);
		int r = 0;
		for (r = 1;r <= lins; r++)
		{
			printf("%c ", arr[i][r]);
		}
	printf("\n");
	}
}

void set_array(char arr[ROWS][LINS], int rows, int lins, char bit)
{
	int a = 0;
	for (a = 0;a < rows ;a++)
	{
		int b = 0;
		for (b = 0; b < lins; b++)
		{
			arr[a][b] = bit;
		}
	}
}

//埋雷 ： 一共十个雷，先判断随机选取的位子是不是雷，然后再埋雷
void set_array_to_one(char arr[ROWS][LINS])
{
	int timess = 10;
	while (timess)
	{
		int random_row = rand() % ROW + 1;
		int random_lin = rand() % LIN + 1;

		if (arr[random_row][random_lin] == '0')
		{
			arr[random_row][random_lin] = '1';
			timess--;
		}
	}
}

//翻页，根据周围雷的数量来确定数字
int lei_de_shuliang(char arr[ROWS][LINS], int x, int y)
{
	return (arr[x - 1][y] + arr[x - 1][y + 1] + arr[x - 1][y - 1] + arr[x][y - 1]
		+ arr[x][y + 1] + arr[x + 1][y - 1] + arr[x + 1][y] + arr[x + 1][y + 1] - 8 * '0');
}

// 计算两个时间点之间经过了多少秒。
// tv_sec 保存整秒，tv_nsec 保存纳秒；两部分合起来可以得到更精确的用时。
double get_elapsed_seconds(struct timespec start, struct timespec end)
{
	// 先算整秒差，再把纳秒差换算成秒；即使纳秒部分借位，结果仍然正确。
	return (double)(end.tv_sec - start.tv_sec)
		+ (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

void find_lei(char shade[ROWS][LINS], char show[ROWS][LINS], int rows, int lins, struct timespec start_time)
{
	int x, y, win = 0;
	int timess = 10;
	// 标记本局是否还没有成功选过坐标。只在第一次输入合法坐标后改为 0，
	// 所以“第一次不会踩雷”只保护本局的第一步，后续选格仍按原规则判断。
	int first_choose = 1;
	while (win < rows * lins - timess)
	{
		// ===== 单次选取坐标计时开始 =====
		// 每一轮循环代表玩家输入一次坐标，所以在这里重新记录起点。
		// 这样每次显示的“本次选取坐标用时”只包含当前这一次操作，
		// 不会把之前输入坐标的时间重复算进去。
		struct timespec move_start_time;
		timespec_get(&move_start_time, TIME_UTC);

		printf("请输入你要排查的坐标先纵再横，并用空格隔开：\n");
		if (scanf("%d %d", &x, &y) != 2)
		{
			printf("输入的坐标不合法，请重新输入！\n");
			while (getchar() != '\n');
			continue;
		}

		// ===== 单次选取坐标计时结束 =====
		// 坐标输入成功后立即记录结束时间，并算出两个用时：
		// 1. move_seconds：从本次提示输入到完成本次输入所花的时间；
		// 2. total_seconds：从本局 game() 开始到当前时刻的累计时间。
		// 时间显示放在坐标合法性判断之前，因此即使输入越界或重复坐标，
		// 玩家也能看到这一次操作和本局累计分别花了多久。
		struct timespec move_end_time;
		timespec_get(&move_end_time, TIME_UTC);
		double move_seconds = get_elapsed_seconds(move_start_time, move_end_time);
		double total_seconds = get_elapsed_seconds(start_time, move_end_time);
		printf("本次选取坐标用时：%.1f 秒\n", move_seconds);
		printf("本次游戏累计用时：%.1f 秒\n", total_seconds);

		if (x >= 1 && y >= 1 && x <= rows && y <= lins)
		{
			// ===== 第一次选坐标保护 =====
			// 如果这是本局第一次输入合法坐标，并且这个位置正好有雷，
			// 就先把这个雷拿掉，再随机找一个“不是刚才这个格子”的空位置补回去。
			// 这样既保证第一次不会踩雷，也保证棋盘上的雷仍然是原来的 10 颗，
			// 不会通过直接删雷来降低游戏难度。
			if (first_choose)
			{
				if (shade[x][y] == '1')
				{
					int new_x = 0;
					int new_y = 0;
					shade[x][y] = '0';
					// 新位置必须不是玩家刚选的格子，否则雷又被放回原处，
					// 第一次仍然会踩雷；同时只能选择当前没有雷的位置。
					do
					{
						new_x = rand() % ROW + 1;
						new_y = rand() % LIN + 1;
					} while ((new_x == x && new_y == y) || shade[new_x][new_y] != '0');
					shade[new_x][new_y] = '1';
				}
				// 第一次合法选择已经处理完，之后的坐标恢复原来的踩雷规则。
				first_choose = 0;
			}

			if (shade[x][y] == '1')
			{
				printf("很遗憾，你踩到雷了\n");
				print_array(shade, ROW, LIN);
				break;
			}
			else
			{
				if (show[x][y] == '*')
				{
					int number = lei_de_shuliang(shade, x, y);
					show[x][y] = number + '0';
					print_array(show, ROW, LIN);
					win++;
				}
				else
				{
					printf("该坐标已经排查过了，请重新输入！\n");
				}
			}
		
		}
		else
		{
			printf("输入的坐标不合法，请重新输入！\n");
		}
	}
	if (win == rows * lins - timess)
	{
		printf("恭喜你扫雷成功\n");
		print_array(shade, ROW, LIN);
	}
}