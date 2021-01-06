#pragma once

/*
 * Written by Eric Hollas
 *
 * StateCentiMoveLeft is the state for the centipede when it sweeps across the playing
 * field in the positive x-direction.
 *
 * This class is a singleton and will be defined in the centipede.cpp file with a #define
 *
 * The OnMessage function will call the function that will change the state of the object
 * passed by parameter according to the message passed by parameter. However, in this state, 
 * the change of state is handled when it is moving.
 *
 */

#include "BaseState.h"
#include "Centipede.h"

class StateCentiMoveLeft : public BaseState<Centipede> {
public:
	virtual void Execute(Centipede* game_object) sealed;
	virtual void Exit(Centipede* game_object) sealed;

	virtual bool OnMessage(Centipede* game_object, Message&) sealed;

	static StateCentiMoveLeft* inst();
};
