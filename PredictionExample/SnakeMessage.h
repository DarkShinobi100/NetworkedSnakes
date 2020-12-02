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

	/** The time at which this message was sent.
	(Not the time at which it was received!) */
	float time;
};