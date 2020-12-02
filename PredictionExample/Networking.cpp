#include "Networking.h"

#include <iostream>

Networking::Networking(float sendRate, float latency) :
	m_Time(0),
	m_SentTime(0),
	m_SentX(128),
	m_SentY(128),
	m_SentVX(15),
	m_SentVY(10),
	m_SentRotation(10),
	m_XPosition(0),
	m_YPosition(0),
	m_Rotation(0),
	m_Score(0),
	m_SendRate(sendRate),
	m_Latency(latency)
{
}


Networking::~Networking()
{
}

void Networking::StartConnection()
{
	//loacla host: 127.0.0.1

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

	std::cout << "\nEnter your Enemies Port number:";
	std::cin >> PortTarget;
	
}

bool Networking::ReceiveMessage(SnakeMessage& result)
{
	if (m_MessageQueue.empty()) {
		return false;
	}
	else {
		result = m_MessageQueue.front();
		m_MessageQueue.pop();
		return true;
	}
}

void Networking::Update(float dt)
{
	m_Time += dt;

	//// Simulate messages being sent from a remote host every "period",
	//// delivered to this host after "latency".
	//while (m_SentTime + m_Latency < m_Time) {
	//	m_MessageQueue.push({ 1, m_SentX, m_SentY,m_SentRotation,m_Score, m_SentTime });

	//	m_SentTime += m_SendRate;

	//	// Change the path of the snake after a while.
	//	if (m_SentTime >= 12.77f) {
	//		m_SentVX = 25.0f;
	//		m_SentVY = -10.0f;
	//	}

	//	m_SentX += (m_SentVX * m_SendRate);
	//	m_SentY += (m_SentVY * m_SendRate);
	//	m_SentRotation += m_SendRate;
	//	m_Score += 1;
	//}

}

void Networking::SetPosition(float x, float y)
{
	m_XPosition = x;
	m_YPosition = y;
}

void Networking::SetRotation(float Rotation)
{
	m_Rotation = Rotation;
}

void Networking::SetLatency(float latency)
{
	m_Latency = latency;
}

void Networking::SetSendRate(float sendRate)
{
	m_SendRate = sendRate;
}

void Networking::SendData(int ID, Snake player)
{   //Make unblocking
	ConnectionSocket.setBlocking(false);
	
	//package data to send
	SentData << ID<< player.getPosition().x << player.getPosition().y << player.GetRotation()<<player.GetScore()<< m_Time;

	if (ConnectionSocket.send(SentData, IpTarget, PortTarget) != sf::Socket::Done)
	{
		std::cout << "\nSend Failed:";
		// error...
	}

}

sf::Packet Networking::ReceiveData()
{
	//receive our data
	if (ConnectionSocket.receive(ReceivedData, IpTarget, PortTarget) != sf::Socket::Done)
	{
		std::cout << "\nReceive Failed:";
		// error...
	}
	return     ReceivedData;
}


void Networking::Reset() {
	m_Time = 0.0f;
	m_SentTime = 0.0f;
	m_SentX = 0.0f;
	m_SentY = 60.0f;
	m_SentVX = 30;
	m_SentVY = 20;
	m_SentRotation = 10;
	m_Score = 0;
}