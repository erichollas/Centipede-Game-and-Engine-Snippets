#pragma once

/*
 * Written by Eric Hollas
 *
 * BaseState is the virtual class for all states for all characters in the engine.
 *
 * This class has pure virtual methods that will call the various updating / on state
 * functions for the object that is passed by parameter.
 *
 */

#include "framework.h"
#include "Message.h"

template <class character_type>
class BaseState {
public:
	virtual void Execute(character_type*) = 0;
	virtual void Exit(character_type*) = 0;

	virtual bool OnMessage(character_type*, Message&) = 0;
};