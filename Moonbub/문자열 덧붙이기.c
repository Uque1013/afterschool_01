#include <stdio.h>
#include <string.h>

int main(void)
{
	char str[30] = "score : ";
	//str�� ���� �� ���ϱ�
	int str_len = strlen(str);


	char str2[30] = "300";
	// str2�� ���� �� ���ϱ�
	int str2_len = strlen(str2);

	for (int i = 0; i < str2_len; i++)
		str[i + str_len] = str2[i];
	str[str_len + str2_len] = '\0';
	printf("%s\n", str);

	int score = 300;

	return 0;
}