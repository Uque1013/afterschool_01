/* TODO list
* 1) ������ : �ӵ� ����, ������ ����
* 2) �Ѿ� �ý��� ����
* 3) sound effect ����
* 4) �Ѿ� ��ġ ü��������
*/

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
	int is_fired; // �߻� ����
};

struct Enemy
{
	RectangleShape sprite;
	int speed;
	int life;
};

struct Item
{
	RectangleShape sprite;
	int delay;
	int is_presented; // �������� ���� �Ǿ�����
	long presented_time;
};

struct Textures
{
	Texture bg; // ��� �̹���
	Texture bullet; // �Ѿ� �̹���
	Texture enemy; // �� �̹���
	Texture gameover; // ���ӿ��� �̹��� 
	Texture item_delay; // ���� ������ �̹���
	Texture item_speed; // �̼� ������ �̹���	
	Texture player; // �÷��̾� �̹���
};

struct SBuffers
{
	SoundBuffer BGM;
	SoundBuffer pop;
};

//obj1�� 2�� �浹����, �浹�ϸ� 1 ��ȯ, ���ϸ� 0 ��ȯ                                                                                                                                            
int is_collide(RectangleShape obj1, RectangleShape obj2)
{
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// ��������
const int ENEMY_NUM = 10;								// enemy�� �ִ밳��
const int BULLET_NUM = 50;							// bullet�� �ִ밳��
const int ITEM_NUM = 2;									// item�� �ִ�����
const int W_WIDTH = 1200, W_HEIGHT = 680; // â�� ũ��
const int GO_WIDTH = 320, GO_HEIGHT = 240; // gameover �׸��� ũ��


int main(void) {

	struct Textures t;
	t.bg.loadFromFile("./resources/images/background.png");
	t.bullet.loadFromFile("./resources/images/3d_blueheart.png");
	t.enemy.loadFromFile("./resources/images/mongi.png");
	t.gameover.loadFromFile("./resources/images/gameover.png"); 
	t.item_delay.loadFromFile("./resources/images/item_delay.png");
	t.item_speed.loadFromFile("./resources/images/item_speed.png");
	t.player.loadFromFile("./resources/images/player.png");

	struct SBuffers sb;
	sb.BGM.loadFromFile("./resources/sounds/BGM.ogg");
	sb.pop.loadFromFile("./resources/sounds/pop.flac");

	// ������ â ����
	RenderWindow window(VideoMode(W_WIDTH , W_HEIGHT), "AfterSchool"); 
	window.setFramerateLimit(60); // 1�ʿ� 60���� ������

	srand(time(0));

	long start_time = clock(); // ���� ���� �ð�
	long spent_time;	 // ���� ���� �ð�
	long fired_time = 0; // �ֱٿ� �߻��� �ð�
	int is_gameover = 0;

	// BGM
	Sound BGM_sound;
	BGM_sound.setBuffer(sb.BGM);
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
	gameover_sprite.setPosition((W_WIDTH- GO_WIDTH)/2, 60);

	// player
	struct Player player;
	player.sprite.setTexture(&t.player);
	player.sprite.setSize(Vector2f(201, 203));
	player.sprite.setPosition(80, 80);
	player.x = player.sprite.getPosition().x;
	player.y = player.sprite.getPosition().y;
	player.speed = 5;
	player.score = 0;
	player.life = 10;
	player.sprite.setScale(-1, 1); // �̹��� �¿� ����

	// �Ѿ�
	int bullet_speed = 20;
	int bullet_idx = 0;
	int bullet_delay = 500; // ������ 0.5��
	Sound bullet_sound;
	bullet_sound.setBuffer(sb.pop);

	struct Bullet bullet[BULLET_NUM];

	for (int i = 0; i < BULLET_NUM; i++) 
	{
		bullet[i].sprite.setTexture(&t.bullet);
		bullet[i].is_fired = 0;
		bullet[i].sprite.setSize(Vector2f(100, 100));
		bullet[i].sprite.setPosition(player.x + 50, player.y + 15); // �ӽ� �׽�Ʈ
	}

	// ��(enemy)
	struct Enemy enemy[ENEMY_NUM];
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(sb. pop);
	int enemy_score = 100;
	int enemy_respawn_time = 8;

	//  enemy �ʱ�ȭ
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].sprite.setTexture(&t.enemy);
		enemy[i].sprite.setSize(Vector2f(100, 100));
		enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 + 1);
	}

	// item
	struct Item item[2];
	item[0].sprite.setTexture(&t.item_speed);
	item[0].delay = 25000; // 25��
	item[1].sprite.setTexture(&t.item_delay);
	item[1].delay = 20000; // 20��

	for (int i = 0; i < ITEM_NUM; i++) 
	{
		item[i].sprite.setSize(Vector2f(70, 70));
		item[i].is_presented = 0;
		item[i].presented_time = 0;
	}

	//�����찡 �������� ������ �ݺ�
	while (window.isOpen())
	{
		spent_time = clock() - start_time;
		player.x = player.sprite.getPosition().x;
		player.y = player.sprite.getPosition().y;

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

		/* ���� ���� update */
		if (player.life <= 0)
		{
			is_gameover = 1;
		}

		/* Player update */
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

		// Player �̵����� ����
		if (player.x < 201) // 201(�׸��� �ʺ�)
			player.sprite.setPosition(201, player.y);
		else if (player.x > W_WIDTH) 
			player.sprite.setPosition(W_WIDTH, player.y);
		
		if (player.y < 0) 
			player.sprite.setPosition(player.x, 0);
		else if (player.y > W_HEIGHT-203) // 203(�׸��� ����)
			player.sprite.setPosition(player.x, W_HEIGHT-203);

		/* Bullet update */
		printf("bullet_idx %d\n", bullet_idx);
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			// �����ð� üũ 
			if (spent_time - fired_time > bullet_delay)
			{
					// �Ѿ��� �߻�Ǿ����� �ʴٸ�
					if (!bullet[bullet_idx].is_fired)
					{
							bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
							bullet[bullet_idx].is_fired = 1;
							bullet_idx++; // ���� �Ѿ��� �߻��� �� �ֵ��� 
							bullet_idx = bullet_idx % BULLET_NUM;
							bullet_sound.play();
							fired_time = spent_time; // �Ѿ� ����
					}
				}

		}

		for (int i = 0; i < BULLET_NUM; i++) {
			if (bullet[i].is_fired)
			{
				bullet[i].sprite.move(bullet_speed, 0);
				if (bullet[i].sprite.getPosition().x > W_WIDTH)
					bullet[i].is_fired = 0;
			}
		}

		/* Enemy update */
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 10�ʸ��� enemy ������
			if (spent_time % (1000 * enemy_respawn_time) < 1000 / 60 + 1)
			{
				// ������ �������϶��� �� ������ ��Ŵ
				if (!is_gameover) {
					enemy[i].sprite.setSize(Vector2f(100, 100));
					enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
					enemy[i].life = 1;
					// 10�ʸ��� enemy �ӵ� + 1
					enemy[i].speed = -(rand() % 10 + 1 + (spent_time / 1000 / enemy_respawn_time));
				}
			}

			if (enemy[i].life > 0)
			{
				// player, enemy �浹
				if (is_collide(player.sprite, enemy[i].sprite))
				{
					enemy[i].life -= 1;
					player.score += enemy_score;

					if (enemy[i].life == 0)
					{
						enemy_explosion_sound.play();
					}
				}
				// ���� ���� ���� �����Ϸ��� ����
				else if (enemy[i].sprite.getPosition().x < 0)
				{
					player.life -= 1;
					enemy[i].life = 0;
				}

				// �Ѿ˰� enemy�� �浹
				for (int j = 0; j < BULLET_NUM; j++) {
					if (is_collide(bullet[j].sprite, enemy[i].sprite))
					{
						if (bullet[j].is_fired) {
							enemy[i].life -= 1;
							player.score += enemy_score;

							if (enemy[i].life == 0)
							{
								enemy_explosion_sound.play();
							}
							bullet[j].is_fired = 0;
						}
					}
				}
				enemy[i].sprite.move(enemy[i].speed, 0);
			}
		}

		printf("[1] %d > %d ?", spent_time - item[1].presented_time, item[1].delay);
		printf("%d (%f, %f)\n", item[1].is_presented, item[1].sprite.getPosition().x);
		// item update TODO : item[1]�� �ȶ�
		for (int i = 0; i < ITEM_NUM; i++) 
		{
			if (!item[i].is_presented)
			{
				if (spent_time - item[i].presented_time > item[i].delay)
				{
					item[i].sprite.setPosition((rand() % W_WIDTH) * 0.8, (rand() % W_HEIGHT) * 0.8);
					item[i].is_presented = 1;
				}
			}

			if (item[i].is_presented)
			{
				// TODO : �浹 �� ������ ȿ���� �ְ� �������
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
		if (item[0].is_presented)
			window.draw(item[0].sprite);

		window.draw(player.sprite);
		window.draw(text);
		for (int i = 0; i < BULLET_NUM; i++) {
			if (bullet[i].is_fired)
				window.draw(bullet[i].sprite);
		}
		if (is_gameover)
		{
			window.draw(gameover_sprite);
			// TODO : ������ ���ߴ� ���� ������ �� 
		}

		window.display();
	}
		

	return 0;
}
