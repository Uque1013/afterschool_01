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
	float x, y; // �÷��̾� ��ǥ
};

// �Ѿ�
struct Bullet
{
	RectangleShape sprite;
	int speed;
	int is_fired; // �߻� ����
};

struct Enemy
{
	RectangleShape sprite;
	int speed;
	int score;
	int life;
	SoundBuffer explosion_buffer;
	Sound explosion_sound;
	int  respawn_time;
};

struct Textures
{
	Texture bg; // ��� �̹���
	Texture gameover; // ���ӿ��� �̹��� 
	Texture player; // �÷��̾� �̹���
};

//obj1�� 2�� �浹����, �浹�ϸ� 1 ��ȯ, ���ϸ� 0 ��ȯ                                                                                                                                            
int is_collide(RectangleShape obj1, RectangleShape obj2)
{
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// ��������
const int ENEMY_NUM = 10;								// enemyh�� �ִ밳��
const int W_WIDTH = 1200, W_HEIGHT = 680; // â�� ũ��
const int GO_WIDTH = 320, GO_HEIGHT = 240; // gameover �׸��� ũ��



int main(void) {

	struct Textures t;
	t.bg.loadFromFile("./resources/images/background.png");
	t.gameover.loadFromFile("./resources/images/gameover.png"); 
	t.player.loadFromFile("./resources/images/player.png");

	RenderWindow window(VideoMode(W_WIDTH , W_HEIGHT), "AfterSchool"); // 640*480 �������� ������ â�� ����
	window.setFramerateLimit(60); // 1�ʿ� 60���� ������

	srand(time(0));

	long start_time = clock(); // ���� ���� �ð�
	long spent_time;					// ���� ���� �ð�
	int is_gameover = 0;

	// BGM
	SoundBuffer BGM_buffer;
	BGM_buffer.loadFromFile("./resources/sounds/BGM.ogg");
	Sound BGM_sound;
	BGM_sound.setBuffer(BGM_buffer);
	BGM_sound.setLoop(1); // BGM ���ѹݺ�
	BGM_sound.play();

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

	Sprite bg_sprite;
	bg_sprite.setTexture(t.bg);
	bg_sprite.setPosition(0, 0);

	//gameover
	Sprite gameover_sprite;
	gameover_sprite.setTexture(t.gameover);
	gameover_sprite.setPosition((W_WIDTH- GO_WIDTH)/2, (W_HEIGHT-GO_HEIGHT)/2);

	// player
	struct Player player;
	player.sprite.setTexture(&t.player);
	player.sprite.setSize(Vector2f(211, 213));
	player.sprite.setPosition(100, 100);
	player.x = player.sprite.getPosition().x;
	player.y = player.sprite.getPosition().y;
	player.speed = 5;
	player.score = 0;
	player.life = 10;
	player.sprite.setScale(-1, 1);

	// �Ѿ�
	struct Bullet bullet;
	bullet.sprite.setSize(Vector2f(10, 10));
	bullet.sprite.setPosition(player.x + 50, player.y + 15); // �ӽ� �׽�Ʈ
	bullet.speed = 20;
	bullet.is_fired = 0;

	// ��(enemy)
	struct Enemy enemy[ENEMY_NUM];

	//  enemy �ʱ�ȭ
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		// TODO : ������ ��ȿ������ �ڵ���. ���߿� refactoring 
		enemy[i].explosion_buffer.loadFromFile("./resources/sounds/pop.flac");
		enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
		enemy[i].score = 100;  // �� ���� �� ��� ����
		enemy[i].respawn_time = 8;

		enemy[i].sprite.setSize(Vector2f(70, 70));
		enemy[i].sprite.setFillColor(Color::Yellow);
		enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
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
				//// �����̽� ������ ��� enemy �ٽ� ����
				//if (event.key.code == Keyboard::Space)
				//{
				//	for (int i = 0; i < ENEMY_NUM; i++)
				//	{
				//		enemy[i].sprite.setSize(Vector2f(70, 70));
				//		enemy[i].sprite.setFillColor(Color::Yellow);
				//		enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
				//		enemy[i].life = 1;
				//		enemy[i].speed = -(rand() % 10 + 1);
				//	}
				//}
				break;
			}

			}
		}

		spent_time = clock() - start_time;
		player.x = player.sprite.getPosition().x;
		player.y = player.sprite.getPosition().y;

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

		// �Ѿ� �߻�
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			// �Ѿ��� �߻�Ǿ����� �ʴٸ�
			if (!bullet.is_fired)
			{
				bullet.sprite.setPosition(player.x + 50, player.y + 15);
				bullet.is_fired = 1;
			}
		}

		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 10�ʸ��� enemy ����
			if (spent_time % (1000 * enemy[i].respawn_time) < 1000 / 60 + 1)
			{
					enemy[i].sprite.setSize(Vector2f(70, 70));
					enemy[i].sprite.setFillColor(Color::Yellow);
					enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
					enemy[i].life = 1;
					// 10�ʸ��� enemy �ӵ� + 1
					enemy[i].speed = -(rand() % 10 + 1 + (spent_time / 1000 / enemy[i].respawn_time));
			}

			if (enemy[i].life > 0)
			{
				// player, enemy �浹
				if (is_collide(player.sprite, enemy[i].sprite))
				{
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
					enemy[i].life = 0;
				}

				// �Ѿ˰� enemy�� �浹
				if (is_collide(bullet.sprite, enemy[i].sprite))
				{
					enemy[i].life -= 1;
					player.score += enemy[i].score;

					// TODO : �ڵ� refactoring �ʿ�
					if (enemy[i].life == 0)
					{
						enemy[i].explosion_sound.play();
					}
					bullet.is_fired = 0;
				}

				enemy[i].sprite.move(enemy[i].speed, 0);
			}
		}

		// TODO : �Ѿ��� ��� �� ���� �߻�Ǵ� ���� �����ϱ�
		if (bullet.is_fired)
		{
			bullet.sprite.move(bullet.speed, 0);
			if (bullet.sprite.getPosition().x > W_WIDTH)
				bullet.is_fired = 0;
		}

		if (player.life <= 0)
		{
			is_gameover = 1;
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
		if(bullet.is_fired)
			window.draw(bullet.sprite);
		
		if (is_gameover)
		{
			window.draw(gameover_sprite);
			// TODO : ������ ���ߴ� ���� ������ �� 
		}

		window.display();
	}
		

	return 0;
}
