#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

struct Player {
	RectangleShape sprite;
	int idx;							// 애니메이션 index
	int frames;					// 애니메이션 frame수
	long ani_time;				// 애니메이션이 바뀔 때의 시각
	long ani_delay;			// 화면 전환의 텀

	int speed;
	// 점프 관련 변수
	int is_jumping;			// 점프상태인가?
	int jumping_time;		// 점프를 시작하는 시각
	int jump_speed;
};

int main(void)
{
	RenderWindow window(VideoMode(1200, 800), "Animation");
	window.setFramerateLimit(60);

	long start_time;
	long spent_time;				// 게임진행 시각

	const int GRAVITY = 5; // 중력
	const int PLATFORM_Y = 500; // 땅바닥의 y좌표

	Texture run[10];
	run[0].loadFromFile("./animation/Run__000.png");
	run[1].loadFromFile("./animation/Run__001.png");
	run[2].loadFromFile("./animation/Run__002.png");
	run[3].loadFromFile("./animation/Run__003.png");
	run[4].loadFromFile("./animation/Run__004.png");
	run[5].loadFromFile("./animation/Run__005.png");
	run[6].loadFromFile("./animation/Run__006.png");
	run[7].loadFromFile("./animation/Run__007.png");
	run[8].loadFromFile("./animation/Run__008.png");
	run[9].loadFromFile("./animation/Run__009.png");

	struct Player player;
	player.frames = 10;
	player.idx = 0;
	player.sprite.setTexture(&run[0]);
	player.sprite.setSize(Vector2f(90, 120));
	player.sprite.setPosition(200, 200);
	player.ani_delay = 100 / player.frames / 2; // 0.5초마다 걸음
	player.speed = 5;
	player.jump_speed = GRAVITY * 2; // 일정한 속도로 올라가고 내려감

	start_time = clock();
	player.ani_time = start_time;
	player.jumping_time = start_time;

	while (window.isOpen())
	{
		spent_time = clock() - start_time;
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Space)
				{
					// 점프
					player.jumping_time = spent_time;
					player.is_jumping = 1;
				}
				break;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.sprite.setScale(1, 1);
			player.sprite.move(player.speed, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.sprite.setScale(-1, 1);
			player.sprite.move(-player.speed, 0);
		}

		// 0.1초마다 애니메이션 그림이 바뀜
		while (spent_time - player.ani_time > player.ani_delay)
		{
			player.ani_time = spent_time;
			player.sprite.setTexture(&run[player.idx % player.frames]);
			player.idx++;
		}

		// 필요하면 1000을 변수 처리할 것
		if (spent_time - player.jumping_time > 1000)
		{
			player.is_jumping = 0;
		}

		player.sprite.move(0, GRAVITY); // 중력 작용

		if (player.is_jumping == 1)
		{
			player.sprite.move(0, -player.jump_speed);
		}

		// 플레이어가 땅바닥에 착지하면
		if (player.sprite.getPosition().y + player.sprite.getSize().y > PLATFORM_Y)
		{
			// 더 이상 내려갈 수 없게 함
			player.sprite.setPosition(player.sprite.getPosition().x, 
				PLATFORM_Y - player.sprite.getSize().y);
		}

		window.clear(Color::Magenta);

		window.draw(player.sprite);

		window.display();
	}

	return 0;
}