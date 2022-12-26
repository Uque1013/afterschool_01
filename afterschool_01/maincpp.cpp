#include <stdio.h>
#include  <SFML/Graphics.hpp>

using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 640*480 사이즈의 윈도우 창을 만듬

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);

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
		window.draw(player);
		window.display();
	}

	return 0;
}
