#include <stdio.h>

swap(int x, int y)
{
	int swap = x;
	int b = y;
	int a = x;
}

int main(void)
{
	int a = 3;
	int b = 5;

	printf("a:%d b:%d \n", a, b);

	// a�� b�� swap�ϴ� �Լ��� ȣ��

	printf("a:%d b:%d \n", swap(a, b));

	return 0;
}