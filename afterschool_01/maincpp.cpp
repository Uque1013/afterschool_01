/* TODO list
*/

#include <stdio.h>
#include  <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>
#include <Windows.h>

using namespace sf;

struct Player
{
	RectangleShape sprite;
	int speed;
	int speed_max;
	int score;
	int life;
	int change; // 변신상태
	float x, y; // 플레이어 좌표
};

// 총알
struct Bullet
{
	RectangleShape sprite;
	int is_fired; // 발사 여부
};

struct Enemy
{
	RectangleShape sprite;
	int speed;
	int life;
};

enum item_type {
	SPEED, // 0
	DELAY, // 1
	OUTLIFE, // 2
	PLUSLIFE, // 3
	PL2, // 4
	PL3 // 5
};

struct Item
{
	RectangleShape sprite;
	int delay;
	int is_presented; // 아이템이 생성 되었는지
	Sound sound;
	long presented_time;
	enum item_type type;
};

struct Textures
{
	Texture bg; // 배경 이미지
	Texture bg2; // 2번째 배경 이미지
	Texture bg3; // 3번째 배경 이미지
	Texture bg4; // 4번째 배경 이미지
	Texture bullet; // 총알 이미지
	Texture enemy; // 적 이미지
	Texture gameover; // 게임오버 이미지 
	Texture item_delay; // 공속 아이템 이미지
	Texture item_speed; // 이속 아이템 이미지	
	Texture item_outlife; // 플레이어 목숨 뺏는 아이템 이미지
	Texture item_pluslife; // 플레이어 목숨 더해주는 아이템 이미지
	Texture item_ch_pl2; // 플레이어2 이미지 바꿈
	Texture item_ch_pl3; // 플레리어3 이미지 바꿈
	Texture player; // 플레이어 이미지
};

struct SBuffers
{
	SoundBuffer BGM;
	SoundBuffer item_delay;
	SoundBuffer item_speed;
	SoundBuffer item_outlife;
	SoundBuffer item_pluslife;
	SoundBuffer item_ch_pl2;
	SoundBuffer item_ch_pl3;
	SoundBuffer pop;
};

//obj1과 2의 충돌여부, 충돌하면 1 반환, 안하면 0 반환                                                                                                                                            
int is_collide(RectangleShape obj1, RectangleShape obj2)
{
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// 전역변수
const int ENEMY_NUM = 10;								// enemy의 최대개수
const int BULLET_NUM = 50;							// bullet의 최대개수
const int ITEM_NUM = 6;									// item의 최대종류
const int W_WIDTH = 1200, W_HEIGHT = 680; // 창의 크기
const int GO_WIDTH = 320, GO_HEIGHT = 240; // gameover 그림의 크기


int main(void) {

	// console 가리는 방법
#ifdef WIN32
	HWND hwnd = GetConsoleWindow();
#endif 


	struct Textures t;
	t.bg.loadFromFile("./resources/images/background.png");
	t.bg2.loadFromFile("./resources/images/background2.png");
	t.bg3.loadFromFile("./resources/images/background3.png");
	t.bg4.loadFromFile("./resources/images/background4.png");
	t.bullet.loadFromFile("./resources/images/3d_blueheart.png");
	t.enemy.loadFromFile("./resources/images/mongi.png");
	t.gameover.loadFromFile("./resources/images/gameover.png"); 
	t.item_delay.loadFromFile("./resources/images/item_delay.png");
	t.item_speed.loadFromFile("./resources/images/item_speed.png");
	t.item_outlife.loadFromFile("./resources/images/item_outlife.png");
	t.item_pluslife.loadFromFile("./resources/images/item_pluslife.png");
	t.item_ch_pl2.loadFromFile("./resources/images/player2.png");
	t.item_ch_pl3.loadFromFile("./resources/images/player3.png");
	t.player.loadFromFile("./resources/images/player.png");

	struct SBuffers sb;
	sb.BGM.loadFromFile("./resources/sounds/BGM.ogg");
	sb.item_delay.loadFromFile("./resources/sounds/delay.wav");
	sb.item_speed.loadFromFile("./resources/sounds/speed.flac");
	sb.item_outlife.loadFromFile("./resources/sounds/speed.flac");
	sb.item_pluslife.loadFromFile("./resources/sounds/delay.wav");
	sb.item_ch_pl2.loadFromFile("./resources/sounds/speed.flac");
	sb.item_ch_pl3.loadFromFile("./resources/sounds/delay.wav");
	sb.pop.loadFromFile("./resources/sounds/pop.flac");

	// 윈도우 창 생성
	RenderWindow window(VideoMode(W_WIDTH , W_HEIGHT), "AfterSchool"); 
	window.setFramerateLimit(60); // 1초에 60으로 제한함

	srand(time(0));

	long start_time = clock(); // 게임 시작 시간
	long spent_time;	 // 게임 진행 시간
	long fired_time = 0; // 최근에 발사한 시간
	int is_gameover = 0;

	// BGM
	Sound BGM_sound;
	BGM_sound.setBuffer(sb.BGM);
	BGM_sound.setLoop(1); // BGM 무한반복
	BGM_sound.play();

	// text
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30); // 글자크기 설정
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	char info[40];
	
	// 배경
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
	player.speed_max = 15;
	player.score = 0;
	player.life = 10;
	player.sprite.setScale(-1, 1); // 이미지 좌우 반전
	int ch_time = 5	; // 플레이어가 변신한 시간

	// 총알
	int bullet_speed = 20;
	int bullet_idx = 0;
	int bullet_delay = 500; // 딜레이 0.5초
	int bullet_delay_max = 100;
	Sound bullet_sound;
	bullet_sound.setBuffer(sb.pop);

	struct Bullet bullet[BULLET_NUM];

	for (int i = 0; i < BULLET_NUM; i++) 
	{
		bullet[i].sprite.setTexture(&t.bullet);
		bullet[i].is_fired = 0;
		bullet[i].sprite.setSize(Vector2f(100, 100));
		bullet[i].sprite.setPosition(player.x + 50, player.y + 15); // 임시 테스트
	}

	// 적(enemy)
	struct Enemy enemy[ENEMY_NUM];
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(sb. pop);
	int enemy_score = 100;
	int enemy_respawn_time = 8;

	//  enemy 초기화
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].sprite.setTexture(&t.enemy);
		enemy[i].sprite.setSize(Vector2f(100, 100));
		enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 + 1);
	}

	// item
	struct Item item[6];
	item[0].sprite.setTexture(&t.item_speed);
	item[0].delay = 15000; // 15초
	item[0].type = SPEED;
	item[0].sound.setBuffer(sb.item_speed);

	item[1].sprite.setTexture(&t.item_delay);
	item[1].delay = 10000; // 10초
	item[1].type = DELAY;
	item[1].sound.setBuffer(sb.item_delay);

	item[2].sprite.setTexture(&t.item_outlife);
	item[2].delay = 18000; // 18초
	item[2].type = OUTLIFE;
	item[2].sound.setBuffer(sb.item_outlife);

	item[3].sprite.setTexture(&t.item_pluslife);
	item[3].delay = 20000; // 20초
	item[3].type = PLUSLIFE;
	item[3].sound.setBuffer(sb.item_pluslife);

	item[4].sprite.setTexture(&t.item_ch_pl2);
	item[4].delay = 20000; // 20초
	item[4].type = PL2;
	item[4].sound.setBuffer(sb.item_ch_pl2);

	item[5].sprite.setTexture(&t.item_ch_pl3);
	item[5].delay = 21000; // 21초
	item[5].type = PL3;
	item[5].sound.setBuffer(sb.item_ch_pl3);


	for (int i = 0; i < ITEM_NUM; i++) 
	{
		item[i].sprite.setSize(Vector2f(70, 70));
		item[i].is_presented = 0;
		item[i].presented_time = 0;
	}

	//윈도우가 열려있을 때까지 반복
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
				// 종료(x) 버튼을 누르면
			case Event::Closed:
				window.close(); // 윈도우를 닫는다 
				break;
				// 키보드를 눌렀을 때 (누른 순간만을 감지)
			case Event::KeyPressed:
			{
				break;
			}

			}
		}

		/* 게임 상태 update */
		if (player.life <= 0)
		{
			is_gameover = 1;
		}

		/* Player update */
		// 방향키 start
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
		} // 방향키 end 

		// Player 이동범위 제한
		if (player.x < 201) // 201(그림의 너비)
			player.sprite.setPosition(201, player.y);
		else if (player.x > W_WIDTH) 
			player.sprite.setPosition(W_WIDTH, player.y);
		
		if (player.y < 0) 
			player.sprite.setPosition(player.x, 0);
		else if (player.y > W_HEIGHT-203) // 203(그림의 높이)
			player.sprite.setPosition(player.x, W_HEIGHT-203);

		/* Bullet update */
		printf("bullet_idx %d\n", bullet_idx);
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			// 장전시간 체크 
			if (spent_time - fired_time > bullet_delay)
			{
					// 총알이 발사되어있지 않다면
					if (!bullet[bullet_idx].is_fired)
					{
							bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
							bullet[bullet_idx].is_fired = 1;
							bullet_idx++; // 다음 총알이 발사할 수 있도록 
							bullet_idx = bullet_idx % BULLET_NUM;
							bullet_sound.play();
							fired_time = spent_time; // 총알 장전
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
		
		// 배경 이미지 설정
		if (player.score >= 4500)
			bg_sprite.setTexture(t.bg2);
		if (player.score >= 8200)
			bg_sprite.setTexture(t.bg3);
		if (player.score >= 10000)
			bg_sprite.setTexture(t.bg4);


		/* Enemy update */
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 10초마다 enemy 리스폰
			if (spent_time % (1000 * enemy_respawn_time) < 1000 / 60 + 1)
			{
				// 게임이 진행중일때만 적 리스폰 시킴
				if (!is_gameover) {
					enemy[i].sprite.setSize(Vector2f(100, 100));
					enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);
					enemy[i].life = 1;
					// 10초마다 enemy 속도 + 1
					enemy[i].speed = -(rand() % 10 + 1 + (spent_time / 1000 / enemy_respawn_time));
				}
			}

			if (enemy[i].life > 0)
			{
				// player, enemy 충돌
				if (is_collide(player.sprite, enemy[i].sprite))
				{
					enemy[i].life -= 1;
					player.score += enemy_score;

					if (enemy[i].life == 0)
					{
						enemy_explosion_sound.play();
					}
				}
				// 적이 왼쪽 끝에 진입하려는 순간
				else if (enemy[i].sprite.getPosition().x < 0)
				{
					player.life -= 1;
					enemy[i].life = 0;
				}

				// 총알과 enemy의 충돌
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

		// item update
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
				if (is_collide(player.sprite, item[i].sprite))
				{
					// 아이템 획득 시 효과를 주고 사라진다
					switch (item[i].type)
					{
					case SPEED: // player 속도
						player.speed += 2;
						if (player.speed > player.speed_max)
							player.speed = player.speed_max;
							break;
					case DELAY: // player 공속
						bullet_delay -= 100;
						if (bullet_delay < bullet_delay_max)
							bullet_delay = bullet_delay_max;
							break;
					case OUTLIFE: // player 목숨 뺏기
						player.life -= 1;
						break;
					case PLUSLIFE: // player 목숨 더하기
						player.life += 1;
						break;
					case PL2:
						player.sprite.setTexture(&t.item_ch_pl2);
						enemy_score += 200;
						break;
					case PL3:
						player.sprite.setTexture(&t.item_ch_pl3);
						enemy_score += 300;
						break;
					}
					item[i].is_presented = 0;
					item[i].presented_time = spent_time;
					item[i].sound.play();
				}
			}
		}

		sprintf(info, "life:%d  score:%d  time:%d", player.life, player.score, spent_time / 1000);
		text.setString(info);

		window.clear(Color::Black); // 검정색으로 지워줌
		window.draw(bg_sprite);

		// draw는 나중에 호출할수록 우선순위가 높아짐
		for (int i = 0; i < ENEMY_NUM; i++)
			if (enemy[i].life > 0)
				window.draw(enemy[i].sprite);
		for(int i=0; i<ITEM_NUM; i++)
			if (item[i].is_presented)
				window.draw(item[i].sprite);

		window.draw(player.sprite);
		window.draw(text);
		for (int i = 0; i < BULLET_NUM; i++) {
			if (bullet[i].is_fired)
				window.draw(bullet[i].sprite);
		}
		if (is_gameover)
		{
			window.draw(gameover_sprite);
			// TODO : 게임이 멈추는 것을 구현할 것 
		}

		window.display();
	}
		

	return 0;
}
