#include <stdio.h>
#include <string.h> // ���ڿ� ���̺귯��
#include <stdlib.h> // itoa ���

int main(void)
{
	char str[30];
	int score = 300;
	sprintf(str, "score : %d", score);

	printf("%s\n", str);

	return 0;
}