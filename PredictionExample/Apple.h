#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class Apple : public sf::Sprite
{
public:
	Apple();
	~Apple();

	enum RenderMode {
		REAL_ONLY,
		PREDICTED_ONLY,
		REAL_AND_PREDICTED
	};

	void Update(float dt);
	const void Render(sf::RenderWindow* window);

	void SetRenderMode(const RenderMode renderMode) { m_RenderMode = renderMode; }
	void setPosition(float x, float y);
	void Reset();

private:
	sf::Texture m_BodyTexture;
	RenderMode	m_RenderMode = RenderMode::REAL_ONLY;
};

