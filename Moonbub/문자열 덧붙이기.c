#include <stdio.h>

int main(void)
{
	char str[30] = "score : ";
	//str�� ���� �� ���ϱ�
	int str_len = 0;
	while (str[str_len] != '\0')
		str_len++;

	char str2[30] = "300";
	// str2�� ���� �� ���ϱ�
	int str2_len = 0;
	while (str[str2_len] != '\0')
		str2_len++;

	for (int i = 0; i < str2_len; i++)
		str[i + str_len] = str2[i];
	str[str_len + str2_len] = '\0';
	printf("%s\n", str);

	int score = 300;

	return 0;
}