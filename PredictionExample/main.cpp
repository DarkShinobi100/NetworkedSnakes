#include <SFML\Graphics.hpp>
#include <sstream>
#include <iomanip>
#include "Snake.h"
#include "Apple.h"
#include "Networking.h"
#include "SnakeMessage.h"

//Rounds a float to two decimal places and turns it into a string
std::string Stringify( float value ) {
	std::stringstream sStream;
	sStream << std::fixed << std::setprecision( 2 ) << value;
	std::string t;
	sStream >> t;
	return t;
}

int main() {
	sf::RenderWindow window(sf::VideoMode(720, 640), "Snake Networked");
	window.setFramerateLimit(60);	//Request 60 frames per second
	
	Apple apple[30]; //set up apples
	for (int i = 0; i < 30; i++)
	{
		if (i < 10) 
		{
			apple[i].setPosition(50 * i, 10);
		}
		if (i >= 10 && i < 20)
		{
			apple[i].setPosition(50 * i, 10 * i);
		}
		if (i >= 20 && i < 30)
		{
			apple[i].setPosition(10 * i, 20 * i);
		}

		apple[i].SetRenderMode(Apple::RenderMode::REAL_ONLY);
		apple[i].SetActive(true);
	}

	int player1Score = 0;
	int player2Score = 0;

	//Create two Snakes (Can also accept "black" and "red")
	Snake Snakes[2]{ Snake("black"), Snake("red") };

	Snakes[0].setPosition(64, 256);

	Snakes[1].setPosition(416, 128);
	Snakes[1].SetRenderMode(Snake::RenderMode::REAL_AND_PREDICTED);

	//Initialise the background texture and sprite
	sf::Texture floorTexture;
	sf::Sprite floor;
	floorTexture.loadFromFile("Assets/tileSand1.png");
	floorTexture.setRepeated(true);
	floor.setTexture(floorTexture);
	floor.setTextureRect(sf::IntRect(0, 0, 720, 640));

	//Initialise font and text
	sf::Font montserrat;
	sf::Text debugText;
	montserrat.loadFromFile("Assets/Montserrat-Regular.ttf");
	debugText.setFont(montserrat);
	debugText.setOutlineColor(sf::Color::Black);
	debugText.setOutlineThickness(1.f);

	sf::Text ScoreP1Text;
	montserrat.loadFromFile("Assets/Montserrat-Regular.ttf");
	ScoreP1Text.setFont(montserrat);
	ScoreP1Text.setOutlineColor(sf::Color::Black);
	ScoreP1Text.setOutlineThickness(1.f);
	ScoreP1Text.setPosition(25, 25);

	sf::Text ScoreP2Text;
	montserrat.loadFromFile("Assets/Montserrat-Regular.ttf");
	ScoreP2Text.setFont(montserrat);
	ScoreP2Text.setOutlineColor(sf::Color::Black);
	ScoreP2Text.setOutlineThickness(1.f);
	ScoreP2Text.setPosition(450, 25);

	//Clock for timing the 'dt' value
	sf::Clock clock;
	float sendRate	= 0.5f;
	float latency	= 0.3f;
	float gameSpeed = 1.0f;
	float startTime = sendRate * 3.0f;

	//When are we next printing the predicted position (so we don't spam the console)
	float nextPrint = startTime;

	//Create a network simulator with that "sends" a message every 0.5 seconds and has a latency of 0.1 seconds
	Networking netSimulator(sendRate, latency);
	netSimulator.m_MyID = 0;	//On the network, we are Snake 0
	
	while (window.isOpen()) {
		//Get the time since the last frame in milliseconds
		float dt = clock.restart().asSeconds() * gameSpeed;

		sf::Event event;
		while (window.pollEvent(event))	{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Key::Escape)
					window.close();
				if( event.key.code == sf::Keyboard::Key::R ) {
					Snakes[0].Reset(); Snakes[1].Reset();
					netSimulator.Reset();
					nextPrint = startTime;
					printf( "\n\n--------RESET--------\n\n" );
				}

				if (event.key.code == sf::Keyboard::Key::A) {

					Snakes[0].setRotation(Snakes[0].GetRotation() - 1.0);
				}
				if (event.key.code == sf::Keyboard::Key::D) {

					Snakes[0].setRotation(Snakes[0].GetRotation() + 1.0);
				}
			}
			//Move player 1 forward at all times
			Snakes[0].Move();
			for (int i = 0; i < 30; i++)
			{
				if (apple[i].GetActive())
				{
					//collision Detection
					//based on example available here: https://www.sonarlearning.co.uk/questions.php?question-topic=630
					sf::FloatRect shape1 = Snakes[0].getGlobalBounds();
					sf::FloatRect shape2 = apple[i].getGlobalBounds();
					sf::FloatRect shape3 = Snakes[1].getGlobalBounds();

					if (Snakes[0].getPosition().x < apple[i].getPosition().x + shape2.width &&
						Snakes[0].getPosition().x + shape1.width > apple[i].getPosition().x &&
						Snakes[0].getPosition().y < apple[i].getPosition().y + shape2.height &&
						shape1.height + Snakes[0].getPosition().y > apple[i].getPosition().y)
					{
						//Collition detected with player 1
						player1Score++;
						apple[i].SetActive(false);
					}
					else 
					if (Snakes[1].getPosition().x < apple[i].getPosition().x + shape2.width &&
						Snakes[1].getPosition().x + shape3.width > apple[i].getPosition().x &&
						Snakes[1].getPosition().y < apple[i].getPosition().y + shape2.height &&
						shape3.height + Snakes[0].getPosition().y > apple[i].getPosition().y)
					{
					//Collition detected with player 2
					player2Score++;
					apple[i].SetActive(false);
					}
				}
			}


		}

		//If we're at the start, just advance the time by 3.5 seconds, so we have a few packets in the queue already
		if( netSimulator.Time() < 1.0f ) {
			printf( "BEGIN SIMULATION\n" );
			netSimulator.Update(startTime);
		}

		if (netSimulator.Time() < 18.0f) {
			SnakeMessage msg;

			//Update the network simulation
			netSimulator.Update(dt);
			//Get any 'network' messages that are available
			while (netSimulator.ReceiveMessage(msg)) {
				printf("Received message: ID= %d, Pos = (%.2f, %.2f), rotation = %.2f, Time =%.2f\n", msg.id, msg.x, msg.y,msg.Rotataion, msg.time);
				Snakes[msg.id].AddMessage(msg);
			}

			//Update the Snakes
			for( int i = 0; i < sizeof( Snakes ) / sizeof( Snake ); i++ ) {
				Snakes[i].Update( dt );	//Update the real position of the Snake with the info from the latest packet
				if( i != netSimulator.m_MyID ) {
					//Get the predicted position of the Snake at the current Game Time and move the ghost to that position
					Snakes[i].setGhostPosition( Snakes[i].RunPrediction( netSimulator.Time() ) );

					if( netSimulator.Time() > nextPrint ) {
						//Get the predicted position of the Snake at a specific interval and print it to the console
						sf::Vector2f predictedPosition = Snakes[i].RunPrediction( nextPrint );
						printf( "\tPredicted positiion:  (%.2f, %.2f), Time =%.2f\n", predictedPosition.x, predictedPosition.y, nextPrint );
						nextPrint = nextPrint + (sendRate * 0.25f);	//Print 4 times per packet
					}
				}
			}

		}
		debugText.setString( "Game Time: " + Stringify( netSimulator.Time() ));
		
		ScoreP1Text.setString("Player 1 score: " + Stringify(player1Score));

		ScoreP2Text.setString("Player 2 score: " + Stringify(player2Score));


		//Render the scene
		window.clear();
		window.draw(floor);
		for (auto& Snake : Snakes) {
			Snake.Render(&window);
		}

		for (int i = 0; i < 30; i++)
		{
			//is this apple visible
			if (apple[i].GetActive())
			{
				apple[i].Render(&window);
			}
		}

		window.draw(debugText);
		window.draw(ScoreP1Text);
		window.draw(ScoreP2Text);
		window.display();		
	}

	return 0;
}