#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

static void init(sf::ConvexShape& pointer) {
	pointer.setPointCount(3);
	pointer.setPoint(0, { 40, 0 });
	pointer.setPoint(1, { -20, -20 });
	pointer.setPoint(2, { -20, 20 });
	pointer.setPosition({ 400, 300 });
	pointer.setFillColor(sf::Color(0xFF, 0x80, 0x00));
}

static float toDegrees(float radians) {
	return float(double(radians) * 180 / M_PI);
}

static void onMouseMove(const sf::Event::MouseMoved* event, sf::Vector2f& mousePosition) {
	std::cout << "mouse x=" << event->position.x << ", y=" << event->position.y << std::endl;
	mousePosition = sf::Vector2f(event->position);
}

static void pollEvents(sf::RenderWindow& window, sf::Vector2f& mousePosition) {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
			onMouseMove(mouseMoved, mousePosition);
		}
	}
}

static void update(const sf::Vector2f& mousePosition, sf::ConvexShape& pointer) {
	const sf::Vector2f delta = mousePosition - pointer.getPosition();
	const float angle = atan2(delta.y, delta.x);
	pointer.setRotation(sf::degrees(toDegrees(angle)));
}

static void redrawFrame(sf::RenderWindow& window, sf::ConvexShape& pointer) {
	window.clear();
	window.draw(pointer);
	window.display();
}

int main() {
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;
	sf::RenderWindow window(
		sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
		"Pointer follows mouse", sf::Style::Default, sf::State::Windowed, settings
	);

	sf::ConvexShape pointer;
	sf::Vector2f mousePosition;

	init(pointer);
	while (window.isOpen()) {
		pollEvents(window, mousePosition);
		update(mousePosition, pointer);
		redrawFrame(window, pointer);
	}
}