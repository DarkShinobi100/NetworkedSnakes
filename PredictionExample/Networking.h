#pragma once

#include <queue>
#include "SnakeMessage.h"
#include<SFML/Network.hpp>

//This is a network simulation and doesn't actually do any networking!
class Networking
{
public:

	//Create a Network simulator with the designated Send Rate and Latency
	Networking(float sendRate, float latency);
	~Networking();

	void StartConnection();

	/** Receive the next message from the network. Non-blocking.
	Returns true if a message was received, false if there are no more messages to process. */
	bool ReceiveMessage(SnakeMessage& result);

	float Time() { return m_Time; }
	void Update(float dt);

	void SetPosition(float x, float y);
	void SetRotation(float Rotation);

	void SetLatency(float);
	void SetSendRate(float);

	void Reset();

	int m_MyID = -1;

private:
	float m_Time, m_SentTime;
	std::queue<SnakeMessage> m_MessageQueue;
	float m_SentX, m_SentY; //The last sent position
	float m_SentVX;
	float m_SentVY;
	float m_SentRotation;
	float m_XPosition;
	float m_YPosition;
	float m_Rotation;
	int m_Score;
	float m_Latency = 0.1f;
	float m_SendRate = 0.5f;

	sf::UdpSocket PlayerSocket;
	sf::UdpSocket ConnectionSocket;

	sf::IpAddress IpTarget;
	int PortTarget;

	sf::Packet ReceivedData;
	sf::Packet SentData;
};

