#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

//int main()
//{
//	int a = 0;
//	int* pa = &a;
//	int* p = NULL;
//	printf("%d\n%p\n", *pa, (void*)pa);
//	return 0;
//}

int main()
{
	int n = 0x11223344;
	char* cn = (char*)&n;
	short* sn = (short*)&n;
	void* ln = &n;
	//*cn = 0;
	//*sn = 0;
	//*ln = 0;--error
	((void*)ln) = 1;
	return 0;
}