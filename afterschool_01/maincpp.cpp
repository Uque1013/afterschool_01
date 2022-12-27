#include <stdio.h>
#include  <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

struct Player
{
	RectangleShape sprite;
	int speed;
	int score;
	int life;
};

struct Enemy
{
	RectangleShape sprite;
	int speed;
	int score;
	int life;
	SoundBuffer explosion_buffer;
	Sound explosion_sound;
};

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
	struct Player player;
	player.sprite.setSize(Vector2f(40, 40));
	player.sprite.setPosition(100, 100);
	player.sprite.setFillColor(Color::Red);
	player.speed = 5;
	player.score = 0;
	player.life = 3;


	// ��(enemy)
	const int ENEMY_NUM = 10;

	struct Enemy enemy[ENEMY_NUM];

	//  enemy �ʱ�ȭ
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		// TODO : ������ ��ȿ������ �ڵ���. ���߿� refactoring 
		enemy[i].explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
		enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
		enemy[i].score = 100;  // �� ���� �� ��� ����

		enemy[i].sprite.setSize(Vector2f(70, 70));
		enemy[i].sprite.setFillColor(Color::Yellow);
		enemy[i].sprite.setPosition(rand()%300+300, rand()%380);
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 + 1);
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
					for (int i = 0; i < ENEMY_NUM; i++)
					{
						enemy[i].sprite.setSize(Vector2f(70, 70));
						enemy[i].sprite.setFillColor(Color::Yellow);
						enemy[i].sprite.setPosition(rand() % 300 + 300, rand() % 380);
						enemy[i].life = 1;
						enemy[i].speed = -(rand() % 10 + 1);
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
			player.sprite.move(-player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.sprite.move(player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			player.sprite.move(0, -player.speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			player.sprite.move(0, player.speed);
		} // ����Ű end


		
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i].life > 0)
			{
				//enemy���� �浹
				if (player.sprite.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds()))
				{
					printf("enemy[%d]�� �浹\n", i);
					enemy[i].life -= 1;
					player.score += enemy[i].score;

					// TODO : �ڵ� refactoring �ʿ�
					if (enemy[i].life == 0)
					{
						enemy[i].explosion_sound.play();
					}
				}
				// ���� ���� ���� �����Ϸ��� ����
				else if (enemy[i].sprite.getPosition().x < 0)
				{
					player.life -= 1;
				}
				enemy[i].sprite.move(enemy[i].speed, 0);
			}
			
		}

		sprintf(info, "life:%d  score:%d  time:%d", player.life, player.score, spent_time / 1000);
		text.setString(info);

		window.clear(Color::Black); // ���������� ������
		window.draw(bg_sprite);

		// draw�� ���߿� ȣ���Ҽ��� �켱������ ������
		for (int i = 0; i < ENEMY_NUM; i++)
			if (enemy[i].life > 0)
				window.draw(enemy[i].sprite);
		window.draw(player.sprite);
		window.draw(text);
		

		window.display();
	}
		

	return 0;
}
