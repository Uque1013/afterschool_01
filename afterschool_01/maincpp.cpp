#include <stdio.h>
#include  <SFML/Graphics.hpp>

using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 640*480 �������� ������ â�� ����
	window.setFramerateLimit(60); // 1�ʿ� 60���� ������

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);
	int player_speed = 5;

	RectangleShape enemy;
	enemy.setSize(Vector2f(70, 70));
	enemy.setPosition(500, 300);
	enemy.setFillColor(Color::Yellow);
	int enemy_life = 1;

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

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.move(-player_speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.move(player_speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			player.move(0, -player_speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			player.move(0, player_speed);
		}

		//enemy���� �浹
		if (enemy_life > 0)
		{
			if (player.getGlobalBounds().intersects(enemy.getGlobalBounds()))
			{
				printf("enemy�� �浹\n");
				enemy_life -= 1;
			}
		}
		
		window.clear(Color::Black); // ���������� ������

		// draw�� ���߿� ȣ���Ҽ��� �켱������ ������
		if(enemy_life > 0)
			window.draw(enemy);
		window.draw(player);
		
		window.display();
	}

	return 0;
}
