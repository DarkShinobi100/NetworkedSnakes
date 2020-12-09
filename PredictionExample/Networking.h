#pragma once

#include "SnakeMessage.h"
#include<SFML/Network.hpp>
#include "Snake.h"

//This is a network controller for peer to peer networking!
class Networking
{
public:

	//Create a Network simulator with the designated Send Rate and Latency
	Networking(float sendRate, float latency);
	~Networking();

	void StartConnection();

	float Time() { return m_Time; }
	void Update(float dt);

	bool SendData(sf::Packet& SentData);

	unsigned short GetPlayerPort();
	unsigned short GetEnemyPort();

	bool ReceiveData(sf::Packet& ReceivedData);

	void Reset();

	int m_MyID = -1;

private:
	float m_Time;
	float m_Latency = 0.1f;
	float m_SendRate = 0.5f;

	sf::UdpSocket PlayerSocket;

	sf::IpAddress IpTarget;
	unsigned short PortTarget = 0;

	sf::Packet ReceivedData;
	sf::Packet SentData;
};

