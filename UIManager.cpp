#include "UIManager.h"

UIManager::UIManager(sf::RenderWindow& newWindow)
	: window(newWindow)
	, fontLoaded(false)
	, timeScale(1.0f)
{
	renderArea = window.getSize();
	renderOffset = sf::Vector2u(0, (unsigned)(renderArea.y * 0.9f));
	renderArea.y = (unsigned)(renderArea.y * 0.1f);

	background.setFillColor(sf::Color::Blue);
	background.setSize(sf::Vector2f(renderArea));
	background.setPosition(sf::Vector2f(renderOffset));
	background.setOutlineColor(sf::Color::Black);
	background.setOutlineThickness(2.f);

	if (font.loadFromFile("gfx/arial.ttf"))
	{
		fontLoaded = true;

	}
	buttons["ToggleThreadingButton"] = (Button::CreateButton("Toggle Threads", font, sf::Vector2f(renderArea.x * 0.06f, (float)renderOffset.y + (renderArea.y / 2.f)), sf::Vector2f(renderArea.x * 0.1f, renderArea.y * 0.75f)));
	buttons["PauseButton"] = (Button::CreateButton("Pause", font, sf::Vector2f(renderArea.x * 0.17f, (float)renderOffset.y + (renderArea.y / 2.f)), sf::Vector2f(renderArea.x * 0.1f, renderArea.y * 0.75f)));
	buttons["DrawFittestButton"] = (Button::CreateButton("Draw Fittest", font, sf::Vector2f(renderArea.x * 0.28f, (float)renderOffset.y + (renderArea.y / 2.f)), sf::Vector2f(renderArea.x * 0.1f, renderArea.y * 0.75f)));

	detailsTextFront.setFont(font);
	detailsTextFront.setLineSpacing(1.1f);
	detailsTextFront.setCharacterSize((int)(18.f * ((float)window.getSize().x / 1920.f)));

	detailsTextBack = detailsTextFront;

	detailsTextFront.setPosition(sf::Vector2f(renderArea.x * 0.34f, (float)renderOffset.y + (renderArea.y * 0.15f)));
	detailsTextBack.setPosition(sf::Vector2f(renderArea.x * 0.45f, (float)renderOffset.y + (renderArea.y * 0.15f)));

	timeScaleSlider = Slider::CreateSlider(sf::Vector2f(renderArea.x * 0.76f, renderOffset.y + (background.getSize().y / 2.f)), sf::Vector2f(renderArea.x * 0.25f, renderArea.y * 0.13f), font, 25.f);
}

void UIManager::Update()
{
	if (simulationDataCallback)
	{
		auto details = simulationDataCallback();
		std::string frontText = "Population Size: " + details["popSize"] + "\nPopulation Alive: " + details["popSizeAlive"] + "\nGeneration: " + details["generationNumber"];
		std::string backText = "Best Fitness: " + details["bestFitness"] + "\nAverage Fitness: " + details["averageFitness"] + "\nTime Left (seconds): " + details["timeLeft"];
		detailsTextFront.setString(frontText);
		detailsTextBack.setString(backText);
	}

	timeScaleSlider.Update(sf::Mouse::getPosition(window));
	timeScale = timeScaleSlider.GetValue();
}

void UIManager::EventUpdate(sf::Event& sfmlEvent)
{
	bool select = false;
	if (sfmlEvent.type == sf::Event::MouseButtonReleased && sfmlEvent.mouseButton.button == sf::Mouse::Button::Left)
		select = true;

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	for (auto& button : buttons)
	{
		button.second.CheckIntersection(select, mousePos);
	}
}

void UIManager::Render()
{
	window.draw(background);

	for (auto& button : buttons)
	{
		button.second.Render(window);
	}

	window.draw(detailsTextFront);
	window.draw(detailsTextBack);
	timeScaleSlider.Render(window);
}

void UIManager::AssignButtonCallback(std::string buttonName, std::function<void(Button&)> callback)
{
	if (buttons.count(buttonName))
	{
		buttons[buttonName].SetConnection(callback);
	}
}