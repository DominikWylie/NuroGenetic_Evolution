#include "Slider.h"
#include <cmath>

Slider::Slider() : value(0.5f), selected(false), limit(1.0f), minimum(0.1f) { /* Nothing else to init */ }

Slider Slider::CreateSlider(sf::Vector2f position, sf::Vector2f size, sf::Font& font, float limit, float minimum)
{
	Slider newSlider;
	newSlider.setSize(size);
	newSlider.setOrigin(size.x / 2.f, size.y / 2.f);
	newSlider.setPosition(position);
	newSlider.Init(font, limit, minimum);

	return newSlider;
}

void Slider::Init(sf::Font& font, float newLimit, float newMinimum)
{
	limit = newLimit;
	minimum = newMinimum;
	sliderItem.setRadius(getSize().y * 1.25f);
	//sliderItem.setOrigin(sliderItem.getGlobalBounds().width / 2.f, sliderItem.getGlobalBounds().height / 2.f);
	sliderItem.setOrigin(sliderItem.getGlobalBounds().width / 2.f, sliderItem.getGlobalBounds().height / 2.f);
	sliderItem.setFillColor(sf::Color::Black);
	sliderItem.setPosition(getPosition() - sf::Vector2f(getSize().x / 8.f, 0));
	value = ((sliderItem.getPosition().x - getPosition().x) + (getSize().x / 2.f)) / getSize().x;

	// = sliderItem.getPosition().x;

	valueText.setFont(font);
	valueText.setCharacterSize((int)(18 * (getSize().x / 480.f)));
	valueText.setString("Time Scale:\n" + std::to_string(GetValue()));
	valueText.setOrigin(0.f, valueText.getGlobalBounds().height / 2.f);
	valueText.setPosition(getPosition().x + ((getSize().x / 2.f) * 1.05f), getPosition().y);
}

void Slider::Update(sf::Vector2i mousePos)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (!selected)
		{
			sf::Vector2f difference = sf::Vector2f(mousePos.x - sliderItem.getPosition().x, mousePos.y - sliderItem.getPosition().y);
			float distance = sqrtf(difference.x * difference.x + difference.y * difference.y);
			if (distance <= sliderItem.getRadius())
			{
				sliderItem.setFillColor({ 64, 64, 64, 255 });
				selected = true;
			}
		}

		if (selected)
		{
			sliderItem.setPosition(sf::Vector2f(std::max(std::min(getPosition().x + getSize().x / 2.f, (float)mousePos.x), (getPosition().x - getSize().x / 2.f) + ((minimum / limit) * getSize().x)), sliderItem.getPosition().y));
			value = ((sliderItem.getPosition().x - getPosition().x) + (getSize().x / 2.f)) / getSize().x;
			valueText.setString("Time Scale:\n" + std::to_string(GetValue()));
		}
	}
	else
	{
		sliderItem.setFillColor(sf::Color::Black);
		selected = false;
	}
}

void Slider::Render(sf::RenderWindow& window)
{
	window.draw(*this);
	window.draw(sliderItem);
	window.draw(valueText);
}

float Slider::GetValue()
{
	return (value * (limit));
}
