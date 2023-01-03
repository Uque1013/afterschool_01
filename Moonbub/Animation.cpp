#include <SFML/Graphics.hpp>

using namespace sf;

struct Player
{
	RectangleShape sprite;
	int fps; // frames per sec
	int idx; // 애니메이션 index
};

int main(void)
{
	RenderWindow window(VideoMode(1200, 800), "Animation");
	window.setFramerateLimit(60);

	Texture run[10];
	run[0].loadFromFile("./animation/Run__000.png");
	run[1].loadFromFile("./animation/Run__000.png");
	run[2].loadFromFile("./animation/Run__000.png");
	run[3].loadFromFile("./animation/Run__000.png");
	run[4].loadFromFile("./animation/Run__000.png");
	run[5].loadFromFile("./animation/Run__000.png");
	run[6].loadFromFile("./animation/Run__000.png");
	run[7].loadFromFile("./animation/Run__000.png");
	run[8].loadFromFile("./animation/Run__000.png");
	run[9].loadFromFile("./animation/Run__000.png");

	struct Player player;
	player.fps = 10;
	player.sprite.setTexture(&run[0]);
	player.sprite.setSize(Vector2f(90, 120));
	player.sprite.setPosition(200, 600);

	while (window.isOpen())
	{
		Event event;
		while(window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			}
		}

		window.clear(Color::Magenta);

		window.draw(player.sprite);

		window.display();
	}

	return 0;
}