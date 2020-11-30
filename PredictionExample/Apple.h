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

	void SetActive(bool Activate) { m_Active = Activate; }
	bool GetActive() { return m_Active; }
	void SetRenderMode(const RenderMode renderMode) { m_RenderMode = renderMode; }
	void setPosition(float x, float y);
	sf::Vector2f GetPosition();
	void Reset();

private:
	bool m_Active = false;
	sf::Texture m_BodyTexture;
	RenderMode	m_RenderMode = RenderMode::REAL_ONLY;
};

