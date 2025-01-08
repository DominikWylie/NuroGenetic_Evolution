#pragma once
#include <SFML/Graphics.hpp>
class Slider : public sf::RectangleShape
{
public:
	Slider();
	static Slider CreateSlider(sf::Vector2f position, sf::Vector2f size, sf::Font& font, float limit, float minimum = 0.1f);
	void Init(sf::Font& font, float newLimit, float newMinimum);
	void Update(sf::Vector2i mousePos);
	void Render(sf::RenderWindow& window);
	float GetValue();
private:
	sf::CircleShape sliderItem;
	sf::Text valueText;
	bool selected;
	float value;
	float limit;
	float minimum;
};

