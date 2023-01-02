#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace sf;

const int S = 4;
const int CARD_W = 200;
const int CARD_H = 200;

struct Card
{
	RectangleShape sprite;
	int id;
	int type;
	int is_clicked;
};

int main(void)
{
	RenderWindow window(VideoMode(1200, 800), "AfterSchool2");
	window.setFramerateLimit(60);

	Vector2i mouse_pos;
	int flipped_num=0; // 현재 뒤집혀진 카드의 갯수 

	long start_time;
	long spent_time;
	long delay_time; // 바로 다시 ?로 뒤집혀지지 않도록 딜레이 줌

	Texture t[8 + 1];
	t[0].loadFromFile("./resources/images/9.png");
	t[1].loadFromFile("./resources/images/2.jpg");
	t[2].loadFromFile("./resources/images/3.jpg");
	t[3].loadFromFile("./resources/images/4.jpg");
	t[4].loadFromFile("./resources/images/5.jpg");
	t[5].loadFromFile("./resources/images/6.jpg");
	t[6].loadFromFile("./resources/images/7.jpg");
	t[7].loadFromFile("./resources/images/8.jpg");
	t[8].loadFromFile("./resources/images/1.jpg");

	Font font;
	font.loadFromFile("c:/Windows/Fonts/arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::White);
	text.setPosition(0, 0);
	char info[40];

	struct Card cards[S][S];
	int n = 0;
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < S; j++)
		{
			cards[i][j].sprite.setSize(Vector2f(CARD_W, CARD_H));
			cards[i][j].sprite.setPosition(j * CARD_W, i * CARD_H);
			cards[i][j].sprite.setTexture(&t[0]);
			cards[i][j].type = 1 + n / 2;
			cards[i][j].id = n+1;
			cards[i][j].is_clicked = 0;
			n++;
		}
	}

	start_time = clock();
	delay_time = start_time;

	while (window.isOpen())
	{
		mouse_pos = Mouse::getPosition(window);
		spent_time = clock() - start_time;

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
				if (event.mouseButton.button == Mouse::Left)
				{
					for (int i = 0; i < S; i++)
					{
						for (int j = 0; j < S; j++)
						{
							if (flipped_num < 2) 
							{
								// 마우스 위치가 cards[i][j]의 위치에 해당한다면?
								if (cards[i][j].sprite.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
								{
									// 안 뒤집힌 카드만 뒤집음
									if (cards[i][j].is_clicked == 0)
									{
										cards[i][j].is_clicked = 1;
										flipped_num++;
										// 두 개를 뒤집었다면
										if (flipped_num == 2)
										{
											delay_time = spent_time;
										}
									}
								}
							}

						}
					}
				}
			}
		}

		for (int i = 0; i < S; i++)
		{
			
			for (int j = 0; j < S; j++)
			{
				// 클릭이 된 상태면
				if (cards[i][j].is_clicked == 1)
					// 그림이 있는 스프라이트로 변경 (카드 뒤집겠다는 의미)
					cards[i][j].sprite.setTexture(&t[cards[i][j].type]);
				else 
					// 카드 ?상태
					cards[i][j].sprite.setTexture(&t[0]);
			}
		}

		// 뒤집힌 카드가 2개라면
		if (flipped_num == 2)
		{
			// 두 카드가 뒤집힌지 2초 이내라면
			if (spent_time - delay_time <= 1000)
			{
			}
			else 
			{ // 다 ?상태로 만들어 버리겠다
				for (int i = 0; i < S; i++)
					for (int j = 0; j < S; j++)
						cards[i][j].is_clicked == 0;
					flipped_num = 0;
			}
		}

		sprintf(info, "(%d, %d) clicks %d %d \n"
			, mouse_pos.x, mouse_pos.y, spent_time / 1000, delay_time / 1000);
		text.setString(info);

		window.clear(Color::Black);
		for (int i = 0; i < S; i++)
		{
			for (int j = 0; j < S; j++)
			{
				window.draw(cards[i][j].sprite);
			}
		}

		window.draw(text);

		window.display();
	}

	return 0;
}