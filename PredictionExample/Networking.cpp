#include "Networking.h"

#include <iostream>

Networking::Networking(float sendRate, float latency) :
	m_Time(0),
	m_SendRate(sendRate),
	m_Latency(latency)
{
}


Networking::~Networking()
{
}

void Networking::StartConnection()
{
	//local host: 127.0.0.1

	// used for refernece: https://www.sfml-dev.org/tutorials/2.5/network-socket.php
	//setting up networking
	// bind the socket to a port
	if (PlayerSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		// error...
	}
	std::cout << "\nYour IP:" << sf::IpAddress::getLocalAddress();
	std::cout << "\nYour  open Port Number:" << PlayerSocket.getLocalPort();


	std::cout << "\nEnter your Enemies IP:";
	std::cin >> IpTarget;
	
	//if (IpTarget == 0)
	{
		IpTarget = IpTarget.LocalHost;
	}
	std::cout << "\nEnter your Enemies Port number:";
	std::cin >> PortTarget;
	
}

void Networking::Update(float dt)
{
	m_Time += dt;
}

void Networking::SendData(sf::Packet SentData)
{   //Make unblocking
	PlayerSocket.setBlocking(false);

	if (PlayerSocket.send(SentData, IpTarget, PortTarget) != sf::Socket::Done)
	{
		std::cout << "\nSend Failed:";
		// error...
	}
}

sf::Packet Networking::ReceiveData()
{
	//receive our data
	if (PlayerSocket.receive(ReceivedData, IpTarget, PortTarget) != sf::Socket::Done)
	{
		std::cout << "\nReceive Failed:";
		// error...
	}
	return     ReceivedData;
}

unsigned short Networking::GetPlayerPort()
{
	return PlayerSocket.getLocalPort();
}

unsigned short Networking::GetEnemyPort()
{
	return PortTarget;
}

void Networking::Reset() {
	m_Time = 0.0f;
}