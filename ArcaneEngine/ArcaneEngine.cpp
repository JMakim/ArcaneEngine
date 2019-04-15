#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <SFML/System.hpp>
#include <vector>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Arcane.hpp"


using namespace std;

int score = 0;
int lives = 3;









float dot(const sf::Vector2f& lv, const sf::Vector2f& rv)
{
	return lv.x * rv.x + lv.y * rv.y;
}

sf::Vector2f normalise(sf::Vector2f source)
{
	float length = std::sqrt(dot(source, source));
	if (length != 0) source /= length;
	return source;
}

// the physics reflect
sf::Vector2f reflect(const sf::Vector2f& velocity, const sf::Vector2f& normal)
{

	return -2.f * dot(velocity, normal) * normal + velocity;
}

float lengthSquared(const sf::Vector2f& source)
{
	return dot(source, source);
}

class Ball final : public sf::Drawable, public sf::Transformable
{
public:
	explicit Ball(const std::vector<sf::RectangleShape>& solidObjects, const std::vector<sf::CircleShape>& circleObjects, const std::vector<sf::RectangleShape>& groundObject)
		: m_solidObjects(solidObjects),
		m_circleObjects(circleObjects),
		m_groundObject(groundObject),
		m_velocity(-8.2f,- 7.75f)
	{
		m_shape.setRadius(10.f);
		setOrigin(10.f, 10.f);

		m_velocity = normalise(m_velocity);

		// ball behavouir 
	}

	~Ball() = default;

	void update(float dt)
	{

		move(m_velocity * m_speed * dt);

		// collison hit 
		sf::FloatRect overlap;
		sf::FloatRect ballBounds = getTransform().transformRect(m_shape.getLocalBounds());




		for (const auto& o : m_solidObjects)
		{
			if (o.getGlobalBounds().intersects(ballBounds, overlap))
			{
				
				auto collisionNormal = o.getPosition() - getPosition();
				auto manifold = getManifold(overlap, collisionNormal);
				
				resolve(manifold);
				break;
			}
			
		}

		for (const auto& o : m_groundObject)
		{
			if (o.getGlobalBounds().intersects(ballBounds, overlap))
			{

				auto collisionNormal = o.getPosition() - getPosition();
				auto manifold = getManifold(overlap, collisionNormal);

				resolve(manifold);
				lives--;
				break;
			}

		}

		for (const auto& o : m_circleObjects)
		{
			auto collisionNormal = o.getPosition() - getPosition();
			if (lengthSquared(collisionNormal) < ((o.getRadius() * o.getRadius()) + (m_shape.getRadius() * m_shape.getRadius())))
			{

				auto manifold = getManifold(o.getRadius() + m_shape.getRadius(), collisionNormal);
				resolve(manifold);
				score++;
				
				break;
			}
		}

		

	}








	const std::vector<sf::RectangleShape>& m_solidObjects;
	const std::vector<sf::CircleShape>& m_circleObjects;
	const std::vector<sf::RectangleShape>& m_groundObject;

	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;
	const float m_speed = 500.f;

	sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
	{
		//the collision normal is stored in x and y, with the penetration in z
		sf::Vector3f manifold;

		if (overlap.width < overlap.height)
		{
			manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
			manifold.z = overlap.width;
		}
		else
		{
			manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
			manifold.z = overlap.height;
		}

		return manifold;
	}

	sf::Vector3f getManifold(float summedDistance, const sf::Vector2f& collisionNormal)
	{
		auto collisionDistance = std::sqrt(lengthSquared(collisionNormal));
		auto normal = normalise(collisionNormal);
		sf::Vector3f manifold;
		manifold.x = normal.x;
		manifold.y = normal.y;
		manifold.z = collisionDistance - summedDistance;
		return manifold;
	}

	void resolve(const sf::Vector3f& manifold)
	{
		//move the ball out of the solid object by the penetration amount
		sf::Vector2f normal(manifold.x, manifold.y);
		move(normal * manifold.z);

		//reflect the current velocity to make the ball bounce
		m_velocity = reflect(m_velocity, normal);
	}

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override
	{
		states.transform *= getTransform();
		rt.draw(m_shape, states);
	}
};

//-------builds scene-----//
void createSolidObjects(std::vector<sf::RectangleShape>& shapes)
{
	//we set the shape origins to the centre of the object
	//so that comparing the position with the ball's position
	//gives us a good idea of the direction the ball is moving

	shapes.emplace_back(sf::Vector2f(1000.f, 20.f)); //top
	shapes.back().setOrigin(shapes.back().getSize() / 2.f);
	shapes.back().setPosition(500.f, 10.f);

	shapes.emplace_back(sf::Vector2f(20.f, 760.f)); //left
	shapes.back().setOrigin(shapes.back().getSize() / 2.f);
	shapes.back().setPosition(10.f, 400.f);

	shapes.emplace_back(sf::Vector2f(20.f, 760.f)); //right
	shapes.back().setOrigin(shapes.back().getSize() / 2.f);
	shapes.back().setPosition(1000.f, 400.f);

	//shapes.emplace_back(sf::Vector2f(1000.f, 20.f)); //bottom
	//shapes.back().setOrigin(shapes.back().getSize() / 2.f);
	//shapes.back().setPosition(500.f, 790.f); 

	//make the paddle 
	shapes.emplace_back(sf::Vector2f(100.f, 20.f));
	shapes.back().setOrigin(shapes.back().getSize() / 2.f);
	shapes.back().setPosition(300.f, 700.f);
}

void createGroundObject(std::vector<sf::RectangleShape>& shapes) {

	shapes.emplace_back(sf::Vector2f(1000.f, 20.f)); //bottom
	shapes.back().setOrigin(shapes.back().getSize() / 2.f);
	shapes.back().setPosition(500.f, 790.f);
}


void createCircleObjects(std::vector<sf::CircleShape>& shapes)
{
	shapes.emplace_back(30.f);
	shapes.back().setPosition(420.f, 80.f);
	shapes.back().setOrigin(shapes.back().getRadius(), shapes.back().getRadius());

	shapes.emplace_back(60.f);
	shapes.back().setPosition(720.f, 180.f);
	shapes.back().setOrigin(shapes.back().getRadius(), shapes.back().getRadius());

	shapes.emplace_back(20.f);
	shapes.back().setPosition(800.f, 580.f);
	shapes.back().setOrigin(shapes.back().getRadius(), shapes.back().getRadius());

	shapes.emplace_back(45.f);
	shapes.back().setPosition(220.f, 340.f);
	shapes.back().setOrigin(shapes.back().getRadius(), shapes.back().getRadius());

	shapes.emplace_back(50.f);
	shapes.back().setPosition(320.f, 410.f);
	shapes.back().setOrigin(shapes.back().getRadius(), shapes.back().getRadius());
}





void Arcane::Start() {
	sf::RenderWindow window(sf::VideoMode(1000, 800), "Pong Collision");

	//delta time clock
	sf::Clock frameClock;

	//vector of static objects
	std::vector<sf::RectangleShape> solidObjects;
	createSolidObjects(solidObjects);

	std::vector<sf::RectangleShape> groundObject;
	createGroundObject(groundObject);

	//get a ref to the paddle shape
	sf::RectangleShape& paddle = solidObjects.back();

	//vector of static circular objects
	std::vector<sf::CircleShape> circleObjects;
	createCircleObjects(circleObjects);

	//the dynamic object
	Ball ball(solidObjects, circleObjects, groundObject);
	ball.setPosition(500.f, 400.f);


	


	while (window.isOpen())
	{



		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			//update the paddle position
			else if (event.type == sf::Event::MouseMoved)
			{
				auto x = std::max(0, std::min(1000, event.mouseMove.x));
				auto position = paddle.getPosition();
				position.x = static_cast<float>(x);
				paddle.setPosition(position);
			}
		}

		if (lives <= 0) {
			window.close();
		}

		

		//sound

		
		


		// Font and letter test
		sf::Font font;
		font.loadFromFile("arial.ttf"); // Need to figure out a way to use font oustide of Arcane folder
		sf::Text text;
		text.setPosition(20, 15);
		text.setFont(font);
		text.setString("Score: " + std::to_string(score)); //this is inportant
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::Red);

		sf::Text life;
		life.setPosition(130, 15);
		life.setFont(font);
		life.setString("Lives: " + std::to_string(lives)); //this is inportant
		life.setCharacterSize(24);
		life.setFillColor(sf::Color::Red);



		//update the dynamic object
		ball.update(frameClock.restart().asSeconds());

		window.clear();
		window.draw(ball);
		for (const auto& o : solidObjects)
		{
			window.draw(o);
		}
		for (const auto& o : circleObjects)
		{
			window.draw(o);
		}
		for (const auto& o : groundObject) {
			window.draw(o);
		}


		window.draw(text);
		window.draw(life);
		
		window.display();
	}
}
