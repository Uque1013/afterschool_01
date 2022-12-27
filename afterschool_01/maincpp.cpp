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

};

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 640*480 사이즈의 윈도우 창을 만듬
	window.setFramerateLimit(60); // 1초에 60으로 제한함

	srand(time(0));

	long start_time = clock(); // 게임 시작 시간
	long spent_time;					// 게임 진행 시간

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


	// 적(enemy)
	const int ENEMY_NUM = 10;
	RectangleShape enemy[ENEMY_NUM];
	int enemy_life[ENEMY_NUM];
	int enemy_speed[ENEMY_NUM];
	int enemy_score = 100; // 적 잡을 때 얻는 점수
	SoundBuffer enemy_explosion_buffer;
	enemy_explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(enemy_explosion_buffer);

	//  enemy 초기화
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setFillColor(Color::Yellow);
		enemy_life[i] = 1;
		enemy[i].setPosition(rand()%300+300, rand()%380);
		enemy_speed[i] = -(rand() % 10 + 1);
	}
	

	//윈도우가 열려있을 때까지 반복
	while (window.isOpen())
	{
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
				// 스페이스 누르면 모든 enemy 다시 출현
				if (event.key.code == Keyboard::Space)
				{
					for (int i = 0; i < ENEMY_NUM; i++)
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


		
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy_life[i] > 0)
			{
				//enemy와의 충돌
				if (player.sprite.getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
				{
					printf("enemy[%d]와 충돌\n", i);
					enemy_life[i] -= 1;
					player.score += enemy_score;
					// TODO : 코드 refactoring 필요
					if (enemy_life[i] == 0)
					{
						enemy_explosion_sound.play();
					}
				}
				enemy[i].move(enemy_speed[i], 0);
			}
			
		}

		sprintf(info, "score:%d  time:%d", player.score,spent_time/1000);
		text.setString(info);

		window.clear(Color::Black); // 검정색으로 지워줌
		window.draw(bg_sprite);

		// draw는 나중에 호출할수록 우선순위가 높아짐
		for (int i = 0; i < ENEMY_NUM; i++)
			if (enemy_life[i] > 0)
				window.draw(enemy[i]);
		window.draw(player.sprite);
		window.draw(text);
		

		window.display();
	}
		

	return 0;
}
