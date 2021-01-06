#pragma once

/*
 * Written by Eric Hollas
 *
 * Centipede defines character for the main villian in the game. This class defines
 * both the head of the centipede and the segment of the centipede.
 *
 * Conceptually, the train of centipede characters is defined as a doubly linked list.
 * Therefore, CentiDirection is used to pass state data from centipede character to
 * centipede character within the doubly linked list. And the member variables
 * p_follower and p_followed_by keep track of adjacent nodes in the doubly linked list.
 * Lastly, direction_type is used to make keeping track of states easier.
 *
 */

#include "BaseCharacter.h"
#include "StateMachine.h"

class Centipede : public BaseCharacter {
public:
	enum class direction_type {
		left,
		right,
		up,
		down,
		poisoned
	};
	struct CentiDirection {
		// previous is needed to ensure the segment goes in the opposite direction 
		// when changing states, i.e. left then down then RIGHT instead of left 
		// then down then LEFT again.
		direction_type previous = direction_type::down;
		direction_type current = direction_type::right;

		// the centipede travels from node to node across the game board. This is 
		// used to calculate change in position.
		DirectX::XMFLOAT3 previous_position = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 target_position = { 0.0f, 0.0f, 0.0f };

		// the other cases are handled by bool flags to make the centipede back 
		// track after reaching the bottom of the game board and to dive down the 
		// game board when contacting a poisoned mushroom.
		bool is_descending = true;
		bool is_poisoned = false;

		CentiDirection() =default;
		~CentiDirection() =default;

		CentiDirection(const CentiDirection& other);
		CentiDirection& operator=(const CentiDirection& other);
	};

public:
	Centipede();
	~Centipede();

	Centipede(const Centipede& other);
	Centipede& operator=(const Centipede& other);

	// the virutal methods follow the expectations described in BaseCharacter.h file
	virtual bool Update(Message*& response);
	virtual bool HandleMSG();
	virtual bool Restart();

	virtual Master::Physics::PhysicsCircle GetPhysicsCircle() sealed;
	virtual int GetPoints() sealed;

	// initiaizes the centipede character
	void SetInitial(const float& rot, CentiDirection& direction);
	// reinitializes the cenitpede to the position and orientation it had at 
	// the beggining of the level. note that the determination of it being a 
	// head or not is not necessary here.
	void ResetInitial();

	// to set event when centipede contacts poisoned muchroom
	void OverridePoison();
	bool IsPoisoned() const;

	// to set event when centipede reaches bottom of game board
	void OverrideAscending();
	bool IsAscending() const;

	// returns true if centipede is in front or a following centipede
	bool IsHead() const;

	// set's the adjacent nodes in the doubly linked list layout of a centipede train
	void SetFollower(Centipede* other);
	void SetFollowedBy(Centipede* other);

	// removes the adjacent node in the doubly linked list layout.
	void RemoveFollower();
	void RemoveFollowedBy();

	// returns the character ID of the follower and followed by.
	int GetFollowerID() const;
	int GetFollowedByID() const;

	// defines the functions to be called in the Execute functions in the various states
	void MoveLeftRight();
	void MoveUpDown();

	// will be called in the update function of the rotation state machine variable to 
	// handle rotations when needed.
	void Rotate();

	// handles the various situations when changing states
	void ProcessDirection();
	// queries the next direction from the character this segment is following
	void ProcessFollowerDirection();

	// getter / setter functions
	void SetDirection(CentiDirection& direction);
	CentiDirection GetDirection() const;

	// when traveling from node to node in the game board, this function returns the 
	// position this segment is leaving instead of pursuing.
	DirectX::XMFLOAT3 GetCurrentPosition() const;

private:
	StateMachine<Centipede>* p_move_machine;
	StateMachine<Centipede>* p_rotate_machine;

	Centipede* p_follower;
	Centipede* p_followed_by;

	CentiDirection m_init_direction;
	CentiDirection m_curr_direction;
	float m_init_rotation;

	static float s_time_interval;
	float m_time_accum;

	float m_prev_rotation;
	float m_target_rotation;

	bool m_is_update;
};