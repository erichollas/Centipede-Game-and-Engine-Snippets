#pragma once

/*
 * Written by Eric Hollas
 *
 * BaseCharacter is the virtual class for all the characters in the game engine.
 *
 * This class has pure virtual methods that will call the on state, the
 * on message, the get physics body, and the the restart functions.
 * 
 * The various details of the intended use of each of these class members is 
 * described below.
 * 
 */

#include "Message.h"
#include "PhysicsLib.h"
#include "UsedIDs.h"

class BaseCharacter {
public:
	BaseCharacter();
	virtual ~BaseCharacter() = 0;

	BaseCharacter(const BaseCharacter&);
	BaseCharacter& operator=(const BaseCharacter&);

	// the pure virtual methods are these next 5 methods

	// Update will be called every frame to call the character's state machine to update
	// the character.
	virtual bool Update(Message*& response) = 0;
	// HandleMSG runs the message events when the pp_mailbox member variable is not empty.
	virtual bool HandleMSG() = 0;
	// Restart is called when a player collision occurs and the character needs to be
	// reset and not destroyed.
	virtual bool Restart() = 0;

	// Each physics body is manually set by pre-calculated data, and we make it required
	// to define for each character class. It was late in the developement process to make
	// every physics body a circle. This was to speed up the process to keep to a schedule.
	virtual Master::Physics::PhysicsCircle GetPhysicsCircle() = 0;
	virtual int GetPoints() = 0;

	// This increases s_enemy_speed, and is intended for increasing the speed for level-ups.
	void IncreaseEnemySpeed();
	float GetEnemySpeed() const;

	// Initializes the character geometric data and allows to override the id for certain
	// objects that have id's set aside for certain characters, Player is 1, Ballistic is 2,
	// and the Game Master is 0.
	void Initialize(const DirectX::XMFLOAT3& position, const float& roation = 0.0f);
	void Initialize(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& roation);
	void OverrideID(const int& id);
	int GetID() const;

	// These functions are the getter/setter for m_matrix_buffer_id
	// This variable is used to interface with the matrix data in the graphics engine.
	// The matrix data is uploaded as a buffer of contiguous memory and character id's
	// are unreliable. Therefore, we use a separate id, the index, in the buffer for 
	// the object in the instance buffer of matrix data. (the graphics engine uses 
	// intance rendering to cut down on draw calls)
	void SetMatrixBufferID(const int& id);
	int GetMatrixBufferID() const;

	// Sends and receives messages. Returns true/false if param msg is set/not set and 
	// is how the message is returned without returning a nullptr.
	bool ReceiveMSG(Message& msg);
	bool RetrieveMSG(Message& msg);

	// The rest of the functions here are the various getter/setter functions for the
	// position and rotation data of the character.
	void ChangePosition(const float& delta_x, const float& delta_y, const float& delta_z);
	void ChangePositionTo(const DirectX::XMFLOAT3& position);
	void ChangePositionToX(const float& position);
	void ChangePositionToY(const float& position);
	void ChangePositionToZ(const float& position);
	void ChangeRotation(const float& rotation);
	void ChangeRotation(const DirectX::XMFLOAT2& rotation);
	void ChangeRotationTo(const float& rotation);
	void ChangeRotationToX(const float& rotation);
	void ChangeRotationToZ(const float& rotation);

	float GetRotation() const;
	DirectX::XMFLOAT2 GetPlayerRotation() const;
	DirectX::XMFLOAT3 GetPostion() const;

private:
	static float s_enemy_speed;

	// m_the_blue_bin recycles character id's
	// m_available_id assigns id when m_the_blue_bin is empty
	static UsedIDs m_the_blue_bin;
	static int m_available_id;


	const int c_mailbox_capacity = 5;

	int m_id;

	int m_matrix_buffer_id;

	int m_msg_count;
	Message** pp_mailbox;

	DirectX::XMFLOAT2 m_rotation;
	DirectX::XMFLOAT3 m_position;

};