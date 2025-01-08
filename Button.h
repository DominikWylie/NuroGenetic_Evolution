#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button : public sf::RectangleShape
{
public:
	static Button CreateButton(const std::string text, sf::Font& font, sf::Vector2f position, sf::Vector2f size);
	void Init(const std::string text, sf::Font& font);
	void CheckIntersection(bool selected, sf::Vector2i mousePos);
	void SetConnection(std::function<void(Button& button)>& connection);
	void Render(sf::RenderWindow& window);
	void SetString(const std::string newText);
private:
	sf::Text buttonText;
	std::function<void(Button& button)> signal;
};

