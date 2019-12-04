/*
Cellsim Game
Class: ECE356
Contributers: Avni Salhotra, Gabriel Moik
Date: Nov. 18 2019
Ver.: 2.1
*/


#include "pch.h"
#include <iostream>
#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>
#include "cell.h"
#include "food.h"
//#include <windows.h>
#include <WS2tcpip.h>
//#include "server.h"

#pragma comment (lib, "ws2_32.lib")
#define SFML_STATIC
using namespace sf;

void Serverinit();
void Clientsend();

char buf[1024];
char clientIp[256];
float score;
int counter;
int x_pos= 10;
int y_pos = 10;
int s_pos = 0; //XXXXX
extern float xs;
extern float ys;

//Vectors to store players and foodsources
std::vector<food> foodvec;
std::vector<cell> cellvec;
cell playercell("player1.png");

int main()
{
	std::thread t1(Serverinit); //since Server is always polling give him on thread

	int hit = 0;
	RenderWindow window(VideoMode(800, 800), "Starting cellgamesim");//open SFML renderwindow
	Clock clock;
	Font font;

	Texture BGTexture;
	Sprite BGSprite;
	Texture BGTexture2;
	Sprite BGSprite2;
		cell cellbgb("Bg_begin.png");
		cellbgb.Sprite.setPosition(0, 0);
		cellbgb.Render(window);
		window.display();
		while (!Keyboard::isKeyPressed(Keyboard::Space));

	BGTexture.loadFromFile("background.png");
	BGSprite.setTexture(BGTexture);
	BGSprite.setPosition(0, 0);
	cell playercell("player1.png");
	cell other_cell("player1.png");

	cellvec.push_back(playercell);
	cellvec.push_back(other_cell);

	int timecount = 0;
	int random_x = 0;
	int random_y = 0;

	food foodinit = food("tex.jpg");
	foodinit.setPosition(900, 900);//non reachable food for collision detection availability

	while (window.isOpen())
	{

			Clientsend();	

		Event event;

		float ElapsedTime = clock.restart().asMilliseconds(); //generating a time elapsed calculator
		timecount += ElapsedTime;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		// Food creation
		if (timecount >= 300)
		{
			random_x = (rand() % 790) + 1;
			random_y = (rand() % 790) + 1;
			food foody = food("tex.jpg");
			foody.setPosition(random_x, random_y);
			foodvec.push_back(foody);

			timecount = 0;
		}

		
		window.clear(Color(0, 100, 255));
		window.draw(BGSprite);

		for (int i = 0; i < foodvec.size(); i++) //for loop to check collision from cells with food
		{
			if (foodvec[i].Active)
			{
				foodvec[i].Update(window, ElapsedTime);
				foodvec[i].Render(window);
				hit = foodinit.collision(cellvec[cellvec.size() - 1].Sprite, foodvec);
			}
			else {
				foodvec.erase(foodvec.begin() + i);
			}
			if (hit)
				score++;
			if (score >= 50) //this section gets called when a player reached 50 score, so this is the end I guess
			{
				cell cellbg("Bg_end.png");
				cellbg.Sprite.setPosition(0, 0);
				cellbg.Render(window);
				window.display();
				while (!Keyboard::isKeyPressed(Keyboard::Space));
				window.close();
			}
			else if (s_pos >= 50)
			{
				cell cellbg2("Bg_end2.png");
				cellbg2.Sprite.setPosition(0, 0);
				cellbg2.Render(window);
				window.display();
				while (!Keyboard::isKeyPressed(Keyboard::Space));
				window.close();
			}
			
		}

		int seperator=0;
		int seperator2=0;
		int seperator3 = 0; //XXXXX

		if(buf[0] != NULL) //reading position form other cell
		{
			for (int i = 0; i < 1024; i++)
			{
				if (buf[i] == 'x')
				{
					seperator = i;
				}
				if (buf[i] == 'y')
				{
					seperator2 = i;
				}
			}
			for (int j = 0; j < seperator; j++)
			{
				x_pos = x_pos + buf[j] * pow(10, seperator -1 - j);
			}
			for (int j = seperator + 1; j < seperator2; j++)
			{
				y_pos = y_pos + buf[j] * pow(10, seperator2 - 1 - j);
			}
			for (int j = seperator2 + 1; j < seperator3; j++)
			{
				s_pos = s_pos + buf[j] * pow(10, seperator3 - 1 - j);//XXXXX
			}
		}

		


		
		cellvec[cellvec.size()-1].Sprite.setScale(score/25+1, score/25+1); //scale size according to score
		cellvec[cellvec.size() - 1].Update(window, ElapsedTime);
		cellvec[cellvec.size() - 1].Render(window);
		for (int i = 0; i< cellvec.size()-1; i++)
		{
			if (x_pos != 0 & y_pos != 0) //update other player pos with data that was send by him
			{
				if (x_pos > 800) //XXXXX
					x_pos = 799;
				if (y_pos > 800)
					y_pos = 799;
				cellvec[i].PosUpdate(window, x_pos, y_pos);
				cellvec[i].Render(window);
				x_pos = 0;
				y_pos = 0;
			}
			
		}
		window.display();
	}
	
	return 0;
}

void Serverinit() //Using winsock to create server and have a whileloop keeping it open and checking for input
{

	////////////////////////////////////////////////////////////
	// INITIALIZE WINSOCK
	////////////////////////////////////////////////////////////

	// Structure to store the WinSock version. This is filled in
	// on the call to WSAStartup()
	WSADATA data;

	// To start WinSock, the required version must be passed to
	// WSAStartup(). This server is going to use WinSock version
	// 2 so I created a word that will store 2 and 2 in hex i.e.
	// 0x0202
	WORD version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// Not ok! Get out quickly
		std::cout << "Can't start Winsock! " << wsOk;
		return;
	}

	////////////////////////////////////////////////////////////
	// SOCKET CREATION AND BINDING
	////////////////////////////////////////////////////////////

	// Create a socket, notice that it is a user datagram socket (UDP)
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	// Create a server hint structure for the server
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	serverHint.sin_port = htons(55000); // Convert from little to big endian
	// server_address.sin_addr.S_un.S_addr = inet_addr( "127.0.0.1" );

	// Try and bind the socket to the IP and port
	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
		return;
	}
	sockaddr_in client; // Use to hold the client information (port / ip address)
	int clientLength = sizeof(client); // The size of the client information



	// Enter a loop
	while (true)
	{
		ZeroMemory(&client, clientLength); // Clear the client structure
		ZeroMemory(buf, 1024); // Clear the receive buffer

		// Wait for message
		int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			std::cout << "Error receiving from client " << WSAGetLastError() << std::endl;
			continue;
		}
		

		// Display message and client info
		 // Create enough space to convert the address byte array
		ZeroMemory(clientIp, 256); // to string of characters
		
		// Convert from byte array to chars
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		// Display the message / who sent it
		std::cout << "Message recv from " << clientIp << " : " << buf << std::endl;
	}
}

void Clientsend() // client sends data to server, socket itself is closed after each packet
{
	////////////////////////////////////////////////////////////
	// INITIALIZE WINSOCK
	////////////////////////////////////////////////////////////

	// Structure to store the WinSock version. This is filled in
	// on the call to WSAStartup()
	WSADATA data;

	// To start WinSock, the required version must be passed to
	// WSAStartup(). This server is going to use WinSock version
	// 2 so I create a word that will store 2 and 2 in hex i.e.
	// 0x0202

	WORD version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// error, can't start winsock
		std::cout << "Can't start Winsock! " << wsOk;
		return;
	}

	////////////////////////////////////////////////////////////
	// CONNECT TO THE SERVER
	////////////////////////////////////////////////////////////

	// Create a hint structure for the server
	sockaddr_in server;
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(55000); // Little to big endian conversion
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

	// Socket creation, note that the socket type is datagram
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	// Write out to that socket
	/*auto pos = playercell.getPosition(); //we get the position from the player, make it into a string to be sendable and add limiters to identify parts
	auto s1= std::to_string((int)pos.x);
	auto s2 = std::to_string((int)pos.y);*/
	auto s1 = std::to_string((int)xs);
	auto s2 = std::to_string((int)ys);
	auto s3 = std::to_string((int)score);//XXXXXXXXXX
	std::string s = s1+'x'+s2 + 'y'+s3+'s';
	int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR)
	{
		std::cout << "That didn't work! " << WSAGetLastError() << std::endl;
	}

	//s = ("Hey, I hope this works2"); //just a dummy send packet
	sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR)
	{
		std::cout << "That didn't work! " << WSAGetLastError() << std::endl;
	}
	// Close the socket
	closesocket(out);

	// Close down Winsock
	//WSACleanup();
}