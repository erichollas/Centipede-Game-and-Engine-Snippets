#pragma once

/*
 * Written by Eric Hollas
 *
 * StateMachine defines the variable in each character that maintains the state.
 *
 * The p_state_curr is a virtual pointer to the current state and will be called on
 * when the HandleMSG and Update functions are called.
 *
 */

#include "BaseState.h"

template <class character_type>
class StateMachine {
public:
	StateMachine(character_type* possessor) {
		p_owner = possessor;
		p_state_curr = nullptr;
	}
	// the p_owner will be deleted elsewhere
	~StateMachine() {
		p_owner = nullptr;
		p_state_curr = nullptr;
		delete p_state_curr;
	}

	// This variable should be unique to each character. Thus the copy
	// constructor and assingment operator should not be allowed.
	StateMachine(const StateMachine&) = delete;
	StateMachine& operator=(const StateMachine&) = delete;

	// This function returns true when there is a change to the matrix data
	// This function is called every frame to update the character.
	bool Update() {
		if (p_state_curr) {
			p_state_curr->Execute(p_owner);
			return true;
		}
		return false;
	}
	// This function returns true when there is a change to the matrix data
	// This function is called for a message event and is passed such message.
	bool HandleMSG(Message& msg) {
		if (p_state_curr) {
			p_state_curr->OnMessage(p_owner, msg);
			return true;
		}
		return false;
	}

	// The rest of the functions are general getter/setter functions
	void SetCurrState(BaseState<character_type>* st) {
		p_state_curr = st;
	}

	BaseState<character_type>* GetCurrState() const {
		return p_state_curr;
	}

private:
	character_type* p_owner;

	BaseState<character_type>* p_state_curr;
};