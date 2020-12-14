#pragma once
/** A position update message. */
struct SnakeMessage {
	/** The object number of this snake within the game world. */
	int id;

	/** The coordinates of this snake within the game world. */
	float x, y;

	/** The rotation of this snake within the game world. */
	float Rotataion;

	/** The score of this snake within the game world. */
	int score;

	//The currently active Apple
	int activeApple;
};