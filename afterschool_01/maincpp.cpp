#include <stdio.h>
#include  <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 640*480 �������� ������ â�� ����
	window.setFramerateLimit(60); // 1�ʿ� 60���� ������

	srand(time(0));

	long start_time = clock(); // ���� ���� �ð�
	long spent_time;					// ���� ���� �ð�

	// text
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30); // ����ũ�� ����
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	char info[40];
	
	// ���
	Texture bg_texture;
	bg_texture.loadFromFile("./resources/images/background.jpg");
	Sprite bg_sprite;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);

	// player
	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);
	int player_speed = 5;
	int player_score = 0;


	// ��(enemy)
	RectangleShape enemy[5];
	int enemy_life[5];
	int enemy_score = 100; // �� ���� �� ��� ����
	SoundBuffer enemy_explosion_buffer;
	enemy_explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(enemy_explosion_buffer);

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

		spent_time = clock() - start_time;

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
					// TODO : �ڵ� refactoring �ʿ�
					if (enemy_life[i] == 0)
					{
						enemy_explosion_sound.play();
					}
				}
			}
			
		}

		sprintf(info, "score:%d  time:%d", player_score,spent_time/1000);
		text.setString(info);

		window.clear(Color::Black); // ���������� ������
		window.draw(bg_sprite);

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
