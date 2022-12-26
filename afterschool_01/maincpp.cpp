#include <stdio.h>
#include  <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 640*480 사이즈의 윈도우 창을 만듬
	window.setFramerateLimit(60); // 1초에 60으로 제한함

	srand(time(0));

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);
	int player_speed = 5;

	RectangleShape enemy[5];
	int enemy_life[5];
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

		//enemy와의 충돌
		for (int i = 0; i < 5; i++)
		{
			if (enemy_life[i] > 0)
			{
				if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
				{
					printf("enemy[%d]와 충돌\n", i);
					enemy_life[i] -= 1;
				}
			}
		}


		window.clear(Color::Black); // 검정색으로 지워줌

		// draw는 나중에 호출할수록 우선순위가 높아짐
		for (int i = 0; i < 5; i++)

			if (enemy_life[i] > 0)
				window.draw(enemy[i]);
		window.draw(player);

		window.display();
	}
		

	return 0;
}
