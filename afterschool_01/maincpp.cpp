#include <stdio.h>
#include  <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 640*480 사이즈의 윈도우 창을 만듬
	window.setFramerateLimit(60); // 1초에 60으로 제한함

	srand(time(0));

	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(50); // 글자크기 설정
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
	int enemy_score = 100; // 적 잡을 때 얻는 점수
	

	//  enemy 초기화
	for (int i = 0; i < 5; i++)
	{
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setFillColor(Color::Yellow);
		enemy_life[i] = 1;
		enemy[i].setPosition(rand()%300+300, rand()%380);
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

		// 방향키 start
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
		} // 방향키 end


		//enemy와의 충돌
		for (int i = 0; i < 5; i++)
		{
			if (enemy_life[i] > 0)
			{
				if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
				{
					printf("enemy[%d]와 충돌\n", i);
					enemy_life[i] -= 1;
					player_score += enemy_score;

				}
			}
		}

		sprintf(info,"score : %d\n", player_score);
		text.setString(info);

		window.clear(Color::Black); // 검정색으로 지워줌

		// draw는 나중에 호출할수록 우선순위가 높아짐
		for (int i = 0; i < 5; i++)

			if (enemy_life[i] > 0)
				window.draw(enemy[i]);
		window.draw(player);
		window.draw(text);

		window.display();
	}
		

	return 0;
}
