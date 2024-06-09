#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>


int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

	window.setFramerateLimit(60);

	window.setKeyRepeatEnabled(false);

	//Font file
	sf::Font font;
	if (font.loadFromFile("game_over.ttf") == false) { return -1; }
	//Text
	sf::Text score;
	score.setFont(font);
	score.setCharacterSize(100);
	score.setFillColor(sf::Color::White);
	score.setPosition(375, -50);
	score.setString("0 : 0");

	//Images
	sf::Texture white_texture;
	if (white_texture.loadFromFile("white_box.png") == false) { return -1; }

	//Sounds
	sf::SoundBuffer buff_hit;
	sf::Sound hit;
	hit.setBuffer(buff_hit);
	if (buff_hit.loadFromFile("hit.wav") == false) { return -1; }

	sf::Music music;
	if (music.openFromFile("music.wav") == false) { return -1; }
	music.setVolume(10);
	music.play();

	//States
	bool up = false;
	bool down = false;
	
	//Variables
	int yVelocityPad1 = 0;
	int xVelocityBall = -5;
	int yVelocityBall = -5;
	float yVelocityPad2 = 0;
	int pad1Score = 0;
	int pad2Score = 0;

	//Shapes
	//Background
	sf::RectangleShape background;
	background.setSize(sf::Vector2f(800, 600));
	background.setPosition(0, 0);
	background.setFillColor(sf::Color(15, 27, 49));

	//Pad 1
	sf::RectangleShape pad1;
	pad1.setSize(sf::Vector2f(30, 100));
	pad1.setPosition(50, 200);
	pad1.setTexture(&white_texture);

	//Pad 2
	sf::RectangleShape pad2;
	pad2.setSize(sf::Vector2f(30, 100));
	pad2.setPosition(725, 200);
	pad2.setTexture(&white_texture);

	//Ball
	sf::CircleShape ball(0, 100);
	ball.setRadius(14);
	ball.setPosition(400, 200);
	ball.setTexture(&white_texture);
	
	//EVENTS
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			//Process events

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up) {
					up = true;
				}
				if (event.key.code == sf::Keyboard::Down) {
					down = true;
				}
			}

			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Up) {
					up = false;
				}
				if (event.key.code == sf::Keyboard::Down) {
					down = false;
				}
			}

		}

		//LOGIC
		//Pad 1
		if (up == true) {
			yVelocityPad1 = -5;
		}
		if (down == true) {
			yVelocityPad1 = 5;
		}
		if ((up == true && down == true) || (up == false && down == false)) {
			yVelocityPad1 = 0;
		}

		pad1.move(0, yVelocityPad1);

		//Out of bounds check
		if (pad1.getPosition().y < 0) {
			pad1.setPosition(50, 0);
		}

		if (pad1.getPosition().y > 500) {
			pad1.setPosition(50, 500);
		}

		//Pad 2
		if (ball.getPosition().y < pad2.getPosition().y) {
			yVelocityPad2 = -4;
		}

		if (ball.getPosition().y > pad2.getPosition().y) {
			yVelocityPad2 = 4;
		}

		if (ball.getPosition().x < -50) {
			pad2Score++;
			yVelocityBall = -yVelocityBall;
			ball.setPosition(400, 300);
		}

		if (ball.getPosition().x > 800) {
			pad1Score++;
			yVelocityBall = -yVelocityBall;
			ball.setPosition(400, 300);
		}



		pad2.move(0, yVelocityPad2);

		//Ball
		ball.move(xVelocityBall, yVelocityBall);

		if (ball.getPosition().y < 0) {
			yVelocityBall = -yVelocityBall;
		}

		if (ball.getPosition().y > 572) {
			yVelocityBall = -yVelocityBall;
		}

		//Collision for pad 1
		if (ball.getGlobalBounds().intersects(pad1.getGlobalBounds())) {
			xVelocityBall = -xVelocityBall;
			hit.play();
		}

		//Collision for pad 2
		if (ball.getGlobalBounds().intersects(pad2.getGlobalBounds())) {
			xVelocityBall = -xVelocityBall;
			hit.play();
		}



		//RENDERING
		window.clear();

		window.draw(background);
		window.draw(pad1);
		window.draw(pad2);
		window.draw(ball);

		//Score
		std::stringstream text;
		text << pad1Score << " : " << pad2Score;
		score.setString(text.str());
		window.draw(score);

		window.display();
	}

	return 0;
}