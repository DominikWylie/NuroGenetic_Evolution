#include "Button.h"

Button Button::CreateButton(const std::string text, sf::Font& font, sf::Vector2f position, sf::Vector2f size)
{
	Button newButton;
	newButton.setSize(size);
	newButton.setOrigin(size.x / 2.f, size.y / 2.f);
	newButton.setPosition(position);
	newButton.setFillColor(sf::Color::Green);
	newButton.setOutlineColor(sf::Color::Black);
	newButton.setOutlineThickness(2.f);
	newButton.Init(text, font);

	return newButton;
}

void Button::Init(const std::string text, sf::Font& font)
{
	buttonText.setFont(font);
	buttonText.setCharacterSize((int)(24.f * (getSize().x / 192.f)));
	buttonText.setStyle(sf::Text::Bold);
	SetString(text);
}

void Button::CheckIntersection(bool selected, sf::Vector2i mousePos)
{
	auto green = sf::Color::Green;
	setFillColor(green);

	if (mousePos.x >= (int)getPosition().x - (getSize().x / 2.f) && mousePos.x <= (int)(getPosition().x + (getSize().x / 2.f)))
	{
		if (mousePos.y >= (int)getPosition().y - (getSize().y / 2.f) && mousePos.y <= (int)(getPosition().y + (getSize().y / 2.f)))
		{
			setFillColor({ 0, 200, 0, 255 });
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				setFillColor(sf::Color::Red);

			if (selected && signal)
				signal(*this);
		}
	}
}

void Button::SetConnection(std::function<void(Button& button)>& connection)
{
	signal = connection;
}

void Button::Render(sf::RenderWindow& window)
{
	window.draw(*this);
	window.draw(buttonText);
}

void Button::SetString(const std::string newText)
{
	buttonText.setString(newText);
	buttonText.setOrigin(buttonText.getGlobalBounds().width / 2.f, buttonText.getGlobalBounds().height / 2.f);
	buttonText.setPosition(getPosition().x, getPosition().y - (buttonText.getGlobalBounds().height / 2.f));
}