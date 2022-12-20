#include <SFML/Graphics.hpp>
#include <math.h> 
#define M_PI 3.14159265358979323846

class Boid
{
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_size;
	sf::Color m_color;
	
	
public:
	Boid(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f acceleration, sf::Vector2f size, sf::Color color)
	{
		m_position = position;
		m_velocity = velocity;
		m_acceleration = acceleration;
		m_size = size;
		m_color = color;
	}

	float radius = 20;
	void insertCircle(sf::VertexArray& array, sf::Vector2f position, float radius, sf::Color color) {
		float perimeter = 2 * M_PI * radius;
		size_t count = perimeter / 10;
		for (int i = 0; i < count; i += 1) {
			sf::Vertex v0;
			v0.position = sf::Vector2f(position.x, position.y);
			v0.color = color;
			array.append(v0);
			sf::Vertex v1;
			float angle = i * 2 * M_PI / count;
			v1.position = sf::Vector2f(position.x + cos(angle) * radius, position.y + sin(angle) * radius);
			v1.color = color;
			array.append(v1);
			sf::Vertex v2;
			angle = (i + 1) * 2 * M_PI / count;
			v2.position = sf::Vector2f(position.x + cos(angle) * radius, position.y + sin(angle) * radius);
			v2.color = color;
			array.append(v2);
		}
	}
	void insertBoid(sf::VertexArray& array, int x, int y, float angle) {
		int width = 5, height = 13;
		// Set the triangle shape. Origin is the front of the Boid.
		sf::Vertex v1(sf::Vector2f(x, y));
		sf::Vertex v2(sf::Vector2f(v1.position.x - width, v1.position.y - height));
		sf::Vertex v3(sf::Vector2f(v1.position.x + width, v1.position.y - height));
		// Setting color
		v1.color = v2.color = v3.color = m_color;
		// Set the orientation
		sf::Transform transform;
		angle = angle * 180 / M_PI;
		angle -= 90; // because the angle is calculated from the x axis
		transform.rotate(angle, (v2.position.x + v3.position.x) / 2, v1.position.y - height / 2);
		v1.position = transform.transformPoint(v1.position);
		v2.position = transform.transformPoint(v2.position);
		v3.position = transform.transformPoint(v3.position);
		// Appending them into vertex array
		array.append(v1);
		array.append(v2);
		array.append(v3);
		// Add radius
		sf::Color c = m_color;
		c.a = 50;
		insertCircle(array, v1.position, radius, c);
	}
	
	void draw(sf::RenderWindow& window)
	{
		/*
		sf::ConvexShape shape(3);
		shape.setPoint(0, sf::Vector2f(m_position.x, m_position.y));
		shape.setPoint(1, sf::Vector2f(m_position.x + m_size.x, m_position.y));
		shape.setPoint(2, sf::Vector2f(m_position.x + m_size.x / 2, m_position.y + m_size.y));
		shape.setFillColor(m_color);*/
		sf::VertexArray array(sf::Triangles);
		insertBoid(array, m_position.x, m_position.y, atan2(m_velocity.y, m_velocity.x));
		window.draw(array);
		
		
	}
	
	/*void calculate()
	{
		m_position.x += 0.5 * m_acceleration.x * m_velocity.x * m_velocity.x + m_velocity.x * m_position.x;
		m_position.y += 0.5 * m_acceleration.y * m_velocity.y * m_velocity.y + m_velocity.y * m_position.y;
		m_velocity.x += m_acceleration.x;
		m_velocity.y += m_acceleration.y;


		velocity += accelertion * dt;
		pos += velocity * dt;
	}*/
	
	void update(int windowWidth, int windowHeight, float dt)
	{
		m_position += m_velocity * dt;
		
		if (m_position.x > windowWidth)
		{
			m_position.x = windowWidth;
			m_velocity.x = -m_velocity.x;
		}
		if (m_position.x < 0)
		{
			m_position.x = 0;
			m_velocity.x = -m_velocity.x;
		}
		if (m_position.y > windowHeight)
		{
			m_position.y = windowHeight;
			m_velocity.y = -m_velocity.y;
		}
		if (m_position.y < 0)
		{
			m_position.y = 0;
			m_velocity.y = -m_velocity.y;
		}
		
	}
	
};

int main()
{
	int windowWidth = 1700;
	int windowHeight = 1000;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Boids!");
	window.setFramerateLimit(60);
	
	int numBoids = 5000;
	std::vector<Boid> boids;
	int speed = 100 ; 
	
	for (int i = 0; i < numBoids; i++)
	{
		boids.push_back(Boid(sf::Vector2f(rand() % windowWidth, rand() % windowHeight), sf::Vector2f(rand() % speed, rand() % speed),
			sf::Vector2f(0, 0), sf::Vector2f(10, 10), sf::Color(sf::Color::Blue)));
	}
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (int i = 0; i < numBoids; i++)
		{
			boids[i].update(windowWidth, windowHeight, dt.asSeconds());
			boids[i].draw(window);
		}
		window.display();
	}

	return 0;
}