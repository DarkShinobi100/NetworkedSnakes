#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "SnakeMessage.h"

class Snake : public sf::Sprite
{
public:
	Snake(std::string color);
	~Snake();

	enum RenderMode {
		REAL_ONLY,
		PREDICTED_ONLY,
		REAL_AND_PREDICTED
	};

	void Update(float dt);
	const void Render(sf::RenderWindow* window);

	void AddMessage(const SnakeMessage& msg);
	sf::Vector2f RunPrediction(float gameTime);

	void SetRenderMode(const RenderMode renderMode) { m_RenderMode = renderMode; }
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);
	void setRotation(float Rotation);
	void setScore(int value);
	int GetScore();
	sf::Vector2f GetPosition();
	float GetRotation();

	void Move();

	void setGhostPosition(sf::Vector2f pos);
	void setGhostRotation(float Rotation);
	void Reset();

private:
	sf::Sprite	m_GhostSprite;
	sf::Texture m_BodyTexture;
	float		m_BodyRotation;
	int		m_Score = 0;
	RenderMode	m_RenderMode = RenderMode::REAL_AND_PREDICTED;

	std::vector<SnakeMessage> m_Messages;
	SnakeMessage latestMessage;
};

