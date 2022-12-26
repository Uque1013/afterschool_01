#include <stdio.h>
#include  <SFML/Graphics.hpp>

using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 640*480 �������� ������ â�� ����

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);

	//�����찡 �������� ������ �ݺ�
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// ����(x) ��ư�� ������
			case Event::Closed:
				window.close(); // �����츦 �ݴ´� 
			}
		}
		window.draw(player);
		window.display();
	}

	return 0;
}
