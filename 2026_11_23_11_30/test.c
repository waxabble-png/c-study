#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//int main(void)
//{
//	int n = 0;
//	scanf("%d", &n); // 给定正整数 n
//
//	int sum[100] = { 0 };  // 存储累加后的数字
//	int fact[100] = { 0 }; // 储存阶乘后的数字
//	fact[0] = 1;           // 0! = 1
//	int len_s = 1, len_f = 1; // 分别记录 sum 和 fact 的数字位数
//
//	for (int i = 1; i <= n; i++) // 计算阶乘
//	{
//		// 1. 先算乘法：fact = fact * i
//		int carry = 0;
//		for (int j = 0; j < len_f; j++)
//		{
//			int temp = fact[j] * i + carry;
//			fact[j] = temp % 10;
//			carry = temp / 10;
//		} 
//
//		// 乘法全部位算完后，再处理剩下的进位
//		while (carry > 0)
//		{
//			fact[len_f] = carry % 10;
//			carry /= 10;
//			len_f++;
//		}
//
//		// 2. 再算加法：sum = sum + fact
//		int max_len = (len_f > len_s) ? len_f : len_s;
//		carry = 0;
//		for (int j = 0; j < max_len; j++) // 换个变量名 j，别和外面的 i 重名
//		{
//			int temp = fact[j] + sum[j] + carry; 
//			sum[j] = temp % 10;
//			carry = temp / 10;
//		} 
//
//		// 加法全部位算完后，再更新位数和处理最高位进位
//		len_s = max_len;
//		while (carry > 0)
//		{
//			sum[len_s] = carry % 10;
//			carry /= 10;
//			len_s++;
//		}
//	}
//
//	// 3. 倒序输出
//	for (; len_s > 0; len_s--)
//	{
//		printf("%d", sum[len_s - 1]);
//	}
//	printf("\n");
//	return 0;
//}

//#include <stdio.h>
//
//int main()
//{
//    int n, x;
//    scanf("%d %d", &n, &x);
//
//    int times = 0;
//    for (int i = 1; i <= n; i++)
//    {
//        int t = i;
//        while (t > 0)
//        {
//            if (t % 10 == x)
//            {
//                times++;
//            }
//            t /= 10;
//        }
//    }
//
//    printf("%d\n", times);
//    return 0;
//}