#include <stdio.h>

int main(void) 
{
	int a = 1;

	switch (a)
	{
	case 1:
		printf("apple");
		break;
	case 2:
		printf("banana");
		break;
	default:
		printf("mango");
		break; // 생략 가능
	}
	
	return 0;
}