#include <stdio.h>
#include  <SFML/Graphics.hpp>

using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 640*480 �������� ������ â�� ����
	//�����찡 �������� ������ �ݺ�
	while (window.isOpen()) 
	{
		Event event;
		while (window.pollEvent(event)) 
		{
			// ����(x) ��ư�� ������
			if (event.type == Event::Closed)
				window.close(); // �����츦 �ݴ´� 
		}
	}
	return 0;
}
