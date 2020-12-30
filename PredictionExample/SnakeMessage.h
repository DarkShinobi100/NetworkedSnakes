#pragma once
/** A position update message. */
struct SnakeMessage {
	/** basic data check */
	float TotalData;

	/** The coordinates of this snake within the game world. */
	float x, y;

	/** The rotation of this snake within the game world. */
	float Rotataion;

	/** The score of this snake within the game world. */
	int score;

	//The currently active Apple
	int activeApple;

	//the time the packet was created(opponents time)
	float time;

	//test which type of data this is
	int ID;
};