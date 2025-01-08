#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Slider.h"
#include <map>

class UIManager
{
public:
	UIManager(sf::RenderWindow& newWindow);
	void Update();
	void EventUpdate(sf::Event& sfmlEvent);
	void Render();
	void AssignButtonCallback(std::string buttonName, std::function<void(Button&)> callback);
	void SetSimulationDataCallback(std::function<std::map<std::string, std::string>()> callback) { simulationDataCallback = callback; }
	float GetTimeScale() { return timeScale; }
private:
	sf::RenderWindow& window;
	sf::Font font;
	sf::Text detailsTextFront;
	sf::Text detailsTextBack;
	bool fontLoaded;
	sf::Vector2u renderOffset;
	sf::Vector2u renderArea;
	sf::RectangleShape background;
	std::map<std::string, Button> buttons;
	std::function<std::map<std::string, std::string>()> simulationDataCallback;
	Slider timeScaleSlider;
	float timeScale;
};

