#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#define SFML_STATIC
using namespace sf;
class food
{
public:
	void Update(RenderWindow &Window, float ElapsedTime);
	void Render(RenderWindow &Window);
	void setPosition(float X, float Y);
	food(String tex);
	Sprite foodSprite;
	bool Active;

private:
	static Texture Textur;
};



