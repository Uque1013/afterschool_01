#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
	int arr[5];
	srand(time(0));

	for (int i = 0; i < 5; i++) {
		//1~100까지 랜덤으로
		arr[i] = (rand()%100)+1;
		pritnf("%d", arr[i]);
	}

	return 0;
}