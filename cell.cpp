#include "pch.h"
#include "cell.h"



cell::cell(sf::String tex)
{
	Speed = 1.0f;

	if (!Textur.loadFromFile(tex))
	{
		std::cout << "Textur konnte nicht geladen werden." << std::endl;
	}
	else {
		std::cout << "Textur wurde erfolgreich geladen." << std::endl;
	}

	Textur.setSmooth(false);
	Sprite.setTexture(Textur);
	Sprite.setOrigin(17.5f, 17.5f);
	Sprite.setPosition(160.f, 120.f);
}

void cell::Update(RenderWindow &Window, float ElapsedTime)
{
	float x = Sprite.getPosition().x;
	float y = Sprite.getPosition().y;

	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		if (x <= 0)
		{
			x = Window.getSize().x;
		}
		else {
			x = x - (Speed*ElapsedTime);
		}

	}

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		if (x >= Window.getSize().x)
		{
			x = 0;
		}
		else {
			x = x + (Speed*ElapsedTime);
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		if (y <= 0)
		{
			y = Window.getSize().y;
		}
		else {
			y = y - (Speed*ElapsedTime);
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		if (y >= Window.getSize().y)
		{
			y = 0;
		}
		else {
			y = y + (Speed*ElapsedTime);
		}
	}

	Sprite.setPosition(x, y);
}

void cell::Render(RenderWindow &Window)
{
	Window.draw(Sprite);
}

Vector2f cell::getPosition()
{
	return Sprite.getPosition();
}