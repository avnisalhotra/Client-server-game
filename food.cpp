#include "pch.h"
#include "food.h"



Texture food::Textur;

food::food(sf::String tex)
{

	Active = true;

	if (!Textur.loadFromFile(tex))
	{
		//std::cout << "Bilddatei konnte nicht gefunden werden." << std::endl;
	}
	else {
		//std::cout << "food.png wurde erfolgreich geladen." << std::endl;
	}

	Textur.setSmooth(false);
	foodSprite.setTexture(Textur);
	foodSprite.setOrigin(35.f, 35.f);
	foodSprite.setPosition(160.f, 120.f);
	foodSprite.setRotation(180.f);
}

void food::setPosition(float X, float Y)
{
	foodSprite.setPosition(X, Y);
}

void food::Update(RenderWindow &Window, float ElapsedTime)
{
	if (Active)
	{
		float x = foodSprite.getPosition().x;
		float y = foodSprite.getPosition().y;


		foodSprite.setPosition(x, y);
	}
}

void food::Render(sf::RenderWindow &Window)
{
	if (Active)
	{
		Window.draw(foodSprite);
	}
}
