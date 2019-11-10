#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#define SFML_STATIC
using namespace sf;

class cell
{
public:

	void Update(RenderWindow &Window, float ElapsedTime);
	void Render(RenderWindow &Window);
	Vector2f getPosition();
	cell(String tex);

private:
	float Speed;
	Texture Textur;
	Sprite Sprite;
};

