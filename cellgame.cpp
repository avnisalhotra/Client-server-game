#include "pch.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "cell.h"
#include "food.h"

#define SFML_STATIC
using namespace sf;

int main()
{
	RenderWindow window(VideoMode(800, 800), "Unser erstes SFML Projekt");
	Clock clock;
	Font font;

	Texture BGTexture;
	Sprite BGSprite;

	std::vector<food> foodvec;

	BGTexture.loadFromFile("background.png");
	BGSprite.setTexture(BGTexture);
	BGSprite.setPosition(0, 0);
	cell playercell("cellpic.png");

	int timecount = 0;
	int random_x = 0;
	int random_y = 0;

	while (window.isOpen())
	{
		Event event;

		float ElapsedTime = clock.restart().asMilliseconds();
		timecount += ElapsedTime;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		// Asteroiden Erstellung
		if (timecount >= 100)
		{
			random_x = (rand() % 700) + 1;
			random_y = (rand() % 700) + 1;

			food foody = food("tex.jpg");
			foody.setPosition(random_x, random_y);
			foodvec.push_back(foody);

			timecount = 0;
		}

		
		window.clear(Color(0, 100, 255));
		window.draw(BGSprite);

		for (int i = 0; i < foodvec.size(); i++)
		{
			if (foodvec[i].Active)
			{
				foodvec[i].Update(window, ElapsedTime);
				foodvec[i].Render(window);
			}
			else {
				foodvec.erase(foodvec.begin() + i);
			}
		}
		/*
		// schuesse darstellen
		for (it = bullets.begin(); it != bullets.end(); )
		{
			if (!it->Active)
			{
				it = bullets.erase(it);
			}
			else {
				kollision.box2box(it->BulletSprite, foody);
				it->Update(window, ElapsedTime);
				it->Render(window);
				++it;
			}
		}

		Schiff.Update(window, ElapsedTime);
		Schiff.Render(window);*/

		window.display();
	}

	return 0;
}