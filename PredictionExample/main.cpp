#include <SFML\Graphics.hpp>
#include<SFML/Network.hpp>
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
			apple[i].setPosition(50 + (60 * i), 85);
		}
		if (i >= 10 && i < 20)
		{
			apple[i].setPosition(50 + (60 * (i-10)), 300);
		}
		if (i >= 20 && i < 30)
		{
			apple[i].setPosition(50 + (60 * (i - 20)), 425);
		}

		apple[i].SetRenderMode(Apple::RenderMode::REAL_ONLY);
	}

	int player2Score = 0;

	int RNG = 1;
	apple[RNG].SetActive(true);

	bool playerMoved = true;
	int PlayerNumber = 0;
	int EnemyNumber = 0;
	bool GameOver = false;
	//Create two Snakes (Can also accept "black" and "red")
	Snake Snakes[2]{ Snake("black"), Snake("red") };

	Snakes[0].setPosition(64, 256);
	Snakes[0].SetRenderMode(Snake::RenderMode::REAL_ONLY);
	Snakes[0].setScore(0);

	Snakes[1].setPosition(416, 128);
	Snakes[1].SetRenderMode(Snake::RenderMode::REAL_ONLY);
	Snakes[1].setScore(0);

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
	debugText.setPosition(250, 250);

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
	ScoreP2Text.setPosition(425, 25);

	//Clock for timing the 'dt' value
	sf::Clock clock;
	sf::Clock Timingclock;
	float sendRate	= 0.5f;
	float latency	= 0.3f;
	float gameSpeed = 1.0f;
	bool togglePrediction = true;
	float timeLastReceived = 10000000000.0f;
	float MessageTime = 0.0f;

	//Create a network simulator with that "sends" a message every 0.5 seconds and has a latency of 0.1 seconds
	Networking netSimulator(sendRate, latency);
	float TimeLastSent = netSimulator.Time();
	//Ask user for inputs
	netSimulator.StartConnection();

	if (netSimulator.GetPlayerPort() > netSimulator.GetEnemyPort())
	{
		printf("Bigger,, we are the Grey snake \n");
		PlayerNumber = 0;
		EnemyNumber = 1;
	}
	else
	{
		printf("smaller, we are the Red snake \n");
		EnemyNumber = 0;
		PlayerNumber = 1;
	}
	netSimulator.m_MyID = PlayerNumber;	//On the network, we are Snake 0
	netSimulator.Reset();
	
	while (window.isOpen() && !GameOver) {
		//Get the time since the last frame in milliseconds
		float dt = clock.restart().asSeconds() * gameSpeed;

		sf::Event event;
		while (window.pollEvent(event))	{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Key::Escape)
					window.close();
				if (event.key.code == sf::Keyboard::Key::A) {

					Snakes[PlayerNumber].setRotation(Snakes[PlayerNumber].GetRotation() - 1.0);
					playerMoved = true;
				}
				if (event.key.code == sf::Keyboard::Key::D) {

					Snakes[PlayerNumber].setRotation(Snakes[PlayerNumber].GetRotation() + 1.0);
					playerMoved = true;
				}
				if (event.key.code == sf::Keyboard::Key::P) {
					if(togglePrediction)
					{
						togglePrediction = false;
					}
					else
					{
						togglePrediction = true;
					}
					 
				}
			}			
		}

		for (int i = 0; i < 30; i++)
		{
			//collision with apples
			if (apple[i].GetActive())
			{
				//collision Detection
				//based on example available here: https://www.sonarlearning.co.uk/questions.php?question-topic=630
				sf::FloatRect shape1 = Snakes[PlayerNumber].getGlobalBounds();
				sf::FloatRect shape2 = apple[i].getGlobalBounds();

				if (Snakes[PlayerNumber].getPosition().x < apple[i].getPosition().x + shape2.width &&
					Snakes[PlayerNumber].getPosition().x + shape1.width > apple[i].getPosition().x &&
					Snakes[PlayerNumber].getPosition().y < apple[i].getPosition().y + shape2.height &&
					shape1.height + Snakes[PlayerNumber].getPosition().y > apple[i].getPosition().y)
				{
					//Collition detected with player 1
					Snakes[PlayerNumber].setScore(Snakes[PlayerNumber].GetScore()+1);
					apple[i].SetActive(false);
					RNG = rand() % 30;
					apple[RNG].SetActive(true);

					if (Snakes[PlayerNumber].GetScore() >= 5)
					{
						//game Over
						GameOver = true;
					}
				}					
			}
		}
		//declare message type
		SnakeMessage msg;

		//update time
		netSimulator.Update(dt);

		//send any updated data
		if (playerMoved || netSimulator.Time() > TimeLastSent + 1.0)
		{
			//basic way to test if data has been tampered with
			float TotalData = 0.0f;
			TotalData = (Snakes[PlayerNumber].getPosition().x + Snakes[PlayerNumber].getPosition().y + Snakes[PlayerNumber].GetRotation() + Snakes[PlayerNumber].GetScore() + RNG + netSimulator.Time());
			sf::Packet SentData;
			if (netSimulator.Time() > TimeLastSent + 1.0)
			{
				SentData << TotalData << Snakes[PlayerNumber].getPosition().x << Snakes[PlayerNumber].getPosition().y << Snakes[PlayerNumber].GetRotation() << Snakes[PlayerNumber].GetScore() << RNG<< netSimulator.Time()<<1;
			}
			else 
			{
				SentData << TotalData << Snakes[PlayerNumber].getPosition().x << Snakes[PlayerNumber].getPosition().y << Snakes[PlayerNumber].GetRotation() << Snakes[PlayerNumber].GetScore() << RNG << netSimulator.Time() << 2;
			}
			netSimulator.SendData(SentData);
			SentData.clear();

			TimeLastSent = netSimulator.Time();

			//receive enemy players data
			sf::Packet ReceivedEnemyData;
			if (netSimulator.ReceiveData(ReceivedEnemyData))
			{
				float receivedDataTotal = 0.0f;
				ReceivedEnemyData >> msg.TotalData >> msg.x >> msg.y >> msg.Rotataion >> msg.score >> msg.activeApple>>msg.time >>msg.ID;
				
				//check not same data again
				if (MessageTime != msg.time)
				{
					//basic data receive check for corruption
					receivedDataTotal = msg.x + msg.y + msg.Rotataion + msg.score + msg.activeApple + msg.time;
					if (receivedDataTotal == msg.TotalData)
					{
						Snakes[EnemyNumber].AddMessage(msg, netSimulator.Time());
						Snakes[EnemyNumber].Update(dt);

						printf("Received safe message: ID= %d, Pos = (%.2f, %.2f), rotation = %.2f,score = %i \n", msg.ID, msg.x, msg.y, msg.Rotataion, msg.score);
						timeLastReceived = netSimulator.Time();
						MessageTime = msg.time;
						//if player score has updated
						if (player2Score < msg.score)
						{
							player2Score = msg.score;
							Snakes[EnemyNumber].setScore(msg.score);

							//if the active apple is different change to match
							for (int i = 0; i < 30; i++)
							{
								apple[i].SetActive(false);
							}
							apple[msg.activeApple].SetActive(true);
						}

						if (!togglePrediction)
						{
							Snakes[EnemyNumber].setPosition(msg.x, msg.y);
						}
					}
					else
					{
						printf("\n Corrupt data\n");
					}
				}
				ReceivedEnemyData.clear();
			}
			
			//it has been more than 10 seconds since last successful receive
			if (netSimulator.Time() >= (timeLastReceived + 10.0f))
			{
				printf("\n\n--------Enemy Disconnect--------\n\n");
				//you win
				GameOver = true;
			}
			
			playerMoved = false;			
		}
		//Move player 1 forward at all times
		Snakes[PlayerNumber].Move();
		if (togglePrediction)
		{
			//move player 2
			Snakes[EnemyNumber].setPosition(Snakes[EnemyNumber].RunPrediction(netSimulator.Time()));
		}
		
		ScoreP1Text.setString("Player 1 score: " + Stringify(Snakes[PlayerNumber].GetScore()));
		ScoreP2Text.setString("Player 2 score: " + Stringify(player2Score));

		if (Snakes[EnemyNumber].GetScore() >= 5 ||  Snakes[PlayerNumber].GetScore() >= 5)
		{
			//game Over
			GameOver = true;
		}

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
	while(window.isOpen() && GameOver)
	{
		//game over celebrate
		debugText.setString("Game Over ");
		window.clear();
		window.draw(floor);
		for (auto& Snake : Snakes) {
			Snake.Render(&window);
		}

		window.draw(debugText);
		window.draw(ScoreP1Text);
		window.draw(ScoreP2Text);
		window.display();
	}

	return 0;
}