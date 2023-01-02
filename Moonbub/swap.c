//#include <stdio.h>
//
//// -a, _b는 주소값, *_a, *_b는 가리키는 변수의 실제값(int)
//void swap(int* _a, int* _b)
//{
//	int temp;
//	temp = *_a;
//	*_a = *_b;
//	*_b = temp;
//}
//
//int main(void)
//{
//	int a = 3, b = 5;
//
//	printf("a:%d b:%d \n", a, b);
//
//	swap(&a, &b); // _a에게 a의 주소값을, _b에게 b의 주소값을 넘겨줌
//
//	printf("a:%d b:%d \n", a, b);
//
//	return 0;
//}