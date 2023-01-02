#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// _arr의 내용물을 무작위로 섞는다
void shuffle(int*_arr)
{
	srand(time(0));
	for (int i = 0; i < 100; i++)
	{
		swap(_arr[rand() % 16], _arr[rand() % 16]);
	}
}

int main(void)
{
	const int S = 4;
	int arr[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	shuffle(arr);

	for (int i = 0; i < 16; i++)
		printf("%d", arr[i]);
	return 0;
}