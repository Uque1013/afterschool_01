#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace sf;

int main(void)
{
	RenderWindow window(VideoMode(1200, 800), "AfterSchool2");
	window.setFramerateLimit(60);

	Vector2i mouse_pos;
	int click_cnt=0; // 마우스 누른 횟수


	while (window.isOpen())
	{
		mouse_pos = Mouse::getPosition(window);

		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
				// 한 번 누르면 한 번만 적용
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Right)
				{
					click_cnt++;
				}
			}
		}
		// 마우스 누른 상태면 여러번 증가함	
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			click_cnt++;
		}
		printf("(%d, %d) 클릭횟수 %d \n"
			, mouse_pos.x, mouse_pos.y,click_cnt);
	}

	return 0;
}