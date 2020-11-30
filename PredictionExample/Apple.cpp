#include "Apple.h"

Apple::Apple() : sf::Sprite()
{
	//apple sprite from: https://www.needpix.com/photo/download/1297545/apple-free-logo-logo-fruit-symbols-structure-design-red-apple-red
	m_BodyTexture.loadFromFile("Assets/Apple.png");
	setTexture(m_BodyTexture);
	setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);
}


Apple::~Apple()
{
}

//Sets the Apple's position to the latest network position
void Apple::Update(float dt)
{

}

void Apple::setPosition(float x, float y) {
	sf::Sprite::setPosition(x, y);
}

sf::Vector2f Apple::GetPosition() {
	sf::Vector2f Position = sf::Sprite::getPosition();
	return Position;
}

//Draw the Apple / or the ghost / or both
const void Apple::Render(sf::RenderWindow* window) 
{
	window->draw(*this);
}


void Apple::Reset() {
	
}
