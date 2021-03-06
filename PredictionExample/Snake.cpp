#include "Snake.h"

#include <iostream>

Snake::Snake(std::string color) : sf::Sprite()
{
	//Snake image from: https://commons.wikimedia.org/wiki/File:Snake_-_Top_-_Animated_Green_and_Blue_-_friendly_cartoon.png
	m_BodyTexture.loadFromFile("Assets/" + color + "Snake.png");
	setTexture(m_BodyTexture);

	setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);

	m_GhostSprite.setTexture(m_BodyTexture);
	m_GhostSprite.setColor(sf::Color(255, 255, 255, 128));
	m_GhostSprite.setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);
	setGhostPosition(getPosition());

	times[0] = 0.0f;
	times[1] = 0.0f;
	times[2] = 0.0f;
}


Snake::~Snake()
{
}

//Sets the Snake's position to the latest network position
void Snake::Update(float dt)
{
	if (m_Messages.size() < 1)
		return;
	latestMessage = m_Messages.back();
//	setPosition(latestMessage.x, latestMessage.y);
//	setPosition(RunPrediction(dt));
	setRotation(latestMessage.Rotataion);
	setGhostRotation(latestMessage.Rotataion);
}

void Snake::setPosition(float x, float y) {
	sf::Sprite::setPosition(x, y);
}

void Snake::setPosition(sf::Vector2f pos) {
	sf::Sprite::setPosition(pos);
}

void Snake::setRotation(float Rotation) {
	sf::Sprite::setRotation(Rotation);
}

void Snake::setScore(int value) {
	m_Score = value;
}

sf::Vector2f Snake::GetPosition() {
	sf::Vector2f Position = sf::Sprite::getPosition();
	return Position;
}

float Snake::GetRotation() {
	 float Rotation = sf::Sprite::getRotation();
	 return Rotation;
}

int Snake::GetScore()
{
	return m_Score;
}

void Snake::Move()
{
	//Convert angle to radians
	float angleRADS = (3.1415926536 / 180) * (GetRotation());

	//Set x and y
	float xValue = 0.25f * cos(angleRADS);
	float yValue = 0.25f * sin(angleRADS);

	setPosition(GetPosition().x + xValue,GetPosition().y + yValue);
}

//Use this to set the prediction position
void Snake::setGhostPosition(sf::Vector2f pos) {
	m_GhostSprite.setPosition(pos);
}

void Snake::setGhostRotation(float Rotation) {
	m_GhostSprite.setRotation(Rotation);
}

//Draw the Snake / or the ghost / or both
const void Snake::Render(sf::RenderWindow* window) {
	if (m_RenderMode > 0)
		window->draw(m_GhostSprite);
	if (m_RenderMode != 1) {
		window->draw(*this);
	}
}

//Add a message to the Snake's network message queue
void Snake::AddMessage(const SnakeMessage& msg, float time) {
	//if we have NOT received this already
	if ((msg.x != latestMessage.x && msg.y != latestMessage.y)&& (msg.time > latestMessage.time))
	{//save to vector for prediction
		m_Messages.push_back(msg);
		std::cout << "\n update vector\n";

		//set time values
		times[2] = times[1];
		times[1] = times[0];
		times[0] = time;
	}
	//else ignore it
	std::cout << "\n repeat data\n";
}

//This method calculates and stores the position, but also returns it immediately for use in the main loop
//This is my where prediction would be... IF I HAD ANY
sf::Vector2f Snake::RunPrediction(float gameTime) {
	float predictedX = -1.0f;
	float predictedY = -1.0f;

	const int msize = m_Messages.size();
	if (msize < 3) {
		return sf::Vector2f(predictedX, predictedX);
	}
	const SnakeMessage& msg0 = m_Messages[msize - 1];
	const SnakeMessage& msg1 = m_Messages[msize - 2];
	const SnakeMessage& msg2 = m_Messages[msize - 3];

	// FIXME: Implement prediction here!
	// You have:
	// - the history of position messages received, in "m_Messages"
	//   (msg0 is the most recent, msg1 the 2nd most recent, msg2 the 3rd most recent)
	// - the current time, in "gameTime"
	//
	// You need to update:
	// - the predicted position at the current time, in "predictedX" and "predictedY"

	//No model prediction
	predictedX = msg0.x;
	predictedY = msg0.y;


	//Linear prediction
	//speed = distance/ Time

	sf::Vector2f velocity;
	sf::Vector2f distanceBetweenLastMessages;
	//float timesinceLastMessages = gameTime - msg0.time;
	float timesinceLastMessages = gameTime - times[0];
	float timeBetweenLastMessages;


	distanceBetweenLastMessages.x = msg0.x - msg1.x;
	distanceBetweenLastMessages.y = msg0.y - msg1.y;
	//timeBetweenLastMessages = msg0.time - msg1.time;
	timeBetweenLastMessages = times[0] - times[1];

	if (timeBetweenLastMessages != 0.0f)
	{
		velocity = distanceBetweenLastMessages / timeBetweenLastMessages;
	}
	else
	{
		velocity = distanceBetweenLastMessages /(float) 1.0;
	}

	sf::Vector2f lastPosition = sf::Vector2f(msg0.x, msg0.y);

	//Displacement = speed * time
	sf::Vector2f Dispalcement;
	//Dispalcement.x = velocity.x * (gameTime - msg0.time);
	//Dispalcement.y = velocity.y * (gameTime - msg0.time);
	
	Dispalcement.x = velocity.x * (gameTime - times[0]);
	Dispalcement.y = velocity.y * (gameTime - times[0]);


	predictedX = lastPosition.x + Dispalcement.x;
	predictedY = lastPosition.y + Dispalcement.y;

	return sf::Vector2f( predictedX, predictedY );


	////Quadratic

	//sf::Vector2f previousVelocity;
	//sf::Vector2f distanceBetweenOlderMessages;
	//float timeBetweenOlderMessages;

	//distanceBetweenOlderMessages.x = msg1.x - msg2.x;
	//distanceBetweenOlderMessages.y = msg1.y - msg2.y;

	//timeBetweenOlderMessages = msg1.time - msg2.time;

	//previousVelocity = distanceBetweenOlderMessages / timeBetweenOlderMessages;

	////acceleration = change in speed/change in time (between last 2 messages)
	//sf::Vector2f acceleration;
	//acceleration = (velocity - previousVelocity) / timeBetweenLastMessages;

	////displacement = velocity * timesinceLastMessages+0.5f * acceleration *(timesinceLastMessages^2)
	//// s=ut+1/2at^2

	//Dispalcement = (velocity * timesinceLastMessages) + (0.5f * acceleration * (timesinceLastMessages * timesinceLastMessages));

	//predictedX = lastPosition.x + Dispalcement.x;
	//predictedY = lastPosition.y + Dispalcement.y;

	//return sf::Vector2f(predictedX, predictedY);
}

void Snake::Reset() {
	m_Messages.clear();
	setScore(0);
}
