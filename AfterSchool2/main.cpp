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
	int click_cnt=0; // ���콺 ���� Ƚ��


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
				// �� �� ������ �� ���� ����
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Right)
				{
					click_cnt++;
				}
			}
		}
		// ���콺 ���� ���¸� ������ ������	
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			click_cnt++;
		}
		printf("(%d, %d) Ŭ��Ƚ�� %d \n"
			, mouse_pos.x, mouse_pos.y,click_cnt);
	}

	return 0;
}