#include <stdio.h>
#include  <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 640*480 �������� ������ â�� ����
	window.setFramerateLimit(60); // 1�ʿ� 60���� ������

	srand(time(0));

	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(50); // ����ũ�� ����
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	char info[40];
	text.setString("score");

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);
	int player_speed = 5;
	int player_score = 0;

	RectangleShape enemy[5];
	int enemy_life[5];
	int enemy_score = 100; // �� ���� �� ��� ����
	

	//  enemy �ʱ�ȭ
	for (int i = 0; i < 5; i++)
	{
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setFillColor(Color::Yellow);
		enemy_life[i] = 1;
		enemy[i].setPosition(rand()%300+300, rand()%380);
	}
	

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
				break;
				// Ű���带 ������ �� (���� �������� ����)
			case Event::KeyPressed:
			{
				// �����̽� ������ ��� enemy �ٽ� ����
				if (event.key.code == Keyboard::Space)
				{
					for (int i = 0; i < 5; i++)
					{
						enemy[i].setSize(Vector2f(70, 70));
						enemy[i].setFillColor(Color::Yellow);
						enemy_life[i] = 1;
						enemy[i].setPosition(rand() % 300 + 300, rand() % 380);
					}
				}
				break;
			}

			}
		}

		// ����Ű start
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
		} // ����Ű end


		//enemy���� �浹
		for (int i = 0; i < 5; i++)
		{
			if (enemy_life[i] > 0)
			{
				if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
				{
					printf("enemy[%d]�� �浹\n", i);
					enemy_life[i] -= 1;
					player_score += enemy_score;

				}
			}
		}

		sprintf(info,"score : %d\n", player_score);
		text.setString(info);

		window.clear(Color::Black); // ���������� ������

		// draw�� ���߿� ȣ���Ҽ��� �켱������ ������
		for (int i = 0; i < 5; i++)

			if (enemy_life[i] > 0)
				window.draw(enemy[i]);
		window.draw(player);
		window.draw(text);

		window.display();
	}
		

	return 0;
}
