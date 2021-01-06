#pragma once

/*
 * Written by Eric Hollas
 *
 * Message is a wrapper object for data to be sent from one character to another
 * to signal an event.
 *
 * eMSGType is an enumerated data type to describe the event to be signalled.
 *
 * the functions defined for the Message datatype are all getters/setters
 *
 */

#include "MemoryAllocator.h"

class Message {
public:
	enum class eMSGType {
		empty,
		spawn_scorpion,
		spawn_spider,
		spawn_fly,
		score_player_death,
		score_game_over,
		score_points_1,
		score_points_5,
		score_points_10,
		score_points_50,
		score_points_100,
		score_points_1000,
		player_move_left,
		player_move_right,
		player_move_forward,
		player_move_backward,
		player_move_stop_left,
		player_move_stop_right,
		player_move_stop_forward,
		player_move_stop_backward,
		player_zoom,
		player_collision,
		shroom_poison,
		ballistic_fire,
		ballistic_cease_fire,
		ballistic_hit,
		scorpion_out_of_bounds,
		scorpion_update,
		fly_plant_mushroom,
		fly_out_of_bounds,
		spider_out_of_bounds,
		spider_random_walk,
		spider_chase,
		centipede_waypoint_reached
	};

	Message();
	~Message() = default;

	Message(const Message&) = default;
	Message& operator=(const Message&) = default;

	void* operator new(size_t);
	void operator delete(void* data);

	void Initialize(const Message::eMSGType& typ,
					const int& reciever_id, 
					const int& sender_id);

	Message::eMSGType GetType() const;
	int GetReceiver() const;
	int GetSender() const;


	void SetMSGPosition(DirectX::XMFLOAT3 position);
	DirectX::XMFLOAT3 GetMSGPosition() const;

private:
	static Master::Memory::MemoryAllocator<Message> mem;

	eMSGType m_type;

	int m_reciever;
	int m_sender;

	DirectX::XMFLOAT3 m_position;

};