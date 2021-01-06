#include "Centipede.h"
#include "Clock.h"
#include "Math.h"

#include "StateCentiMoveUp.h"
#include "StateCentiMoveDown.h"
#include "StateCentiMoveLeft.h"
#include "StateCentiMoveRight.h"
#include "StateCentiRotate.h"

#define STATE_MOVE_UP StateCentiMoveUp::inst()
#define STATE_MOVE_DOWN StateCentiMoveDown::inst()
#define STATE_MOVE_LEFT StateCentiMoveLeft::inst()
#define STATE_MOVE_RIGHT StateCentiMoveRight::inst()
#define STATE_ROTATE StateCentiRotate::inst()

float Centipede::s_time_interval = 0.5f;

Centipede::CentiDirection::CentiDirection(const Centipede::CentiDirection& other) {
	previous = other.previous;
	current = other.current;

	previous_position = other.previous_position;
	target_position = other.target_position;

	is_descending = other.is_descending;
	is_poisoned = other.is_poisoned;
}
Centipede::CentiDirection& Centipede::CentiDirection::operator=(const Centipede::CentiDirection& other) {
	if (this != &other) {
		previous = other.previous;
		current = other.current;

		previous_position = other.previous_position;
		target_position = other.target_position;

		is_descending = other.is_descending;
		is_poisoned = other.is_poisoned;
	}
	return *this;
}


Centipede::Centipede() {
	p_move_machine = new StateMachine<Centipede>(this);
	p_rotate_machine = new StateMachine<Centipede>(this);

	p_follower = nullptr;
	p_followed_by = nullptr;

	m_curr_direction = Centipede::CentiDirection();

	m_time_accum = 0.0f;
	
	m_prev_rotation = 0.0f;
	m_target_rotation = 0.0f;

	m_is_update = false;
}
Centipede::~Centipede() {
	delete p_move_machine;
	delete p_rotate_machine;

	p_follower = nullptr;
	p_followed_by = nullptr;
}

Centipede::Centipede(const Centipede& other) {
}
Centipede& Centipede::operator=(const Centipede& other) {
	if (this != &other) {
	}
	return *this;
}

bool Centipede::Update(Message*& response) {
	if (response != nullptr) {
		delete response;
		response = nullptr;
	}

	p_move_machine->Update();

	if (m_is_update) {
		m_is_update = false;
		m_time_accum = 0.0f;
		response = new Message;
		response->Initialize(Message::eMSGType::centipede_waypoint_reached, this->GetID(), -1); //sender doesnt matter
	}

	return true;
}
bool Centipede::HandleMSG() {

	return false;
}
bool Centipede::Restart() {

	return false;
}

Master::Physics::PhysicsCircle Centipede::GetPhysicsCircle() {
	Master::Physics::PhysicsCircle body = Master::Physics::PhysicsCircle();

	body.center.init(this->GetPostion());

	body.radius = 2.45f;

	body.id = this->GetID();

	return body;
}
int Centipede::GetPoints() {
	if (IsHead())
		return 100;
	return 10;
}

void Centipede::SetInitial(const float& rot, CentiDirection& direction) {
	s_time_interval = 1.8f / this->GetEnemySpeed();

	m_init_rotation = rot;
	m_prev_rotation = m_init_rotation;
	m_target_rotation = m_init_rotation;
	m_init_direction = direction;
	m_curr_direction = m_init_direction;

	switch (m_curr_direction.current) {
	case Centipede::direction_type::left:
		p_move_machine->SetCurrState(STATE_MOVE_LEFT);
		p_rotate_machine->SetCurrState(nullptr);
		break;
	case Centipede::direction_type::right:
		p_move_machine->SetCurrState(STATE_MOVE_RIGHT);
		p_rotate_machine->SetCurrState(nullptr);
		break;
	case Centipede::direction_type::up:
		p_move_machine->SetCurrState(STATE_MOVE_UP);
		p_rotate_machine->SetCurrState(nullptr);
		break;
	default:
		p_move_machine->SetCurrState(STATE_MOVE_DOWN);
		p_rotate_machine->SetCurrState(nullptr);
		break;
	}

	this->Initialize(m_curr_direction.previous_position, m_prev_rotation);
}
void Centipede::ResetInitial() {
	m_curr_direction = m_init_direction;

	m_prev_rotation = m_init_rotation;
	m_target_rotation = m_init_rotation;

	m_time_accum = 0.0f;
	m_is_update = false;

	switch (m_curr_direction.current) {
	case Centipede::direction_type::left:
		p_move_machine->SetCurrState(STATE_MOVE_LEFT);
		p_rotate_machine->SetCurrState(nullptr);
		break;
	case Centipede::direction_type::right:
		p_move_machine->SetCurrState(STATE_MOVE_RIGHT);
		p_rotate_machine->SetCurrState(nullptr);
		break;
	case Centipede::direction_type::up:
		p_move_machine->SetCurrState(STATE_MOVE_UP);
		p_rotate_machine->SetCurrState(nullptr);
		break;
	default:
		p_move_machine->SetCurrState(STATE_MOVE_DOWN);
		p_rotate_machine->SetCurrState(nullptr);
		break;
	}

	this->Initialize(m_curr_direction.previous_position, m_prev_rotation);
}

void Centipede::OverridePoison() {
	m_curr_direction.is_poisoned = true;
}
bool Centipede::IsPoisoned() const {
	return m_curr_direction.is_poisoned;
}
void Centipede::OverrideAscending() {
	m_curr_direction.is_descending = false;
}
bool Centipede::IsAscending() const {
	return !m_curr_direction.is_descending;
}

bool Centipede::IsHead() const {
	return p_follower == nullptr;
}

void Centipede::SetFollower(Centipede* other) {
	p_follower = other;
}
void Centipede::SetFollowedBy(Centipede* other) {
	p_followed_by = other;
}
void Centipede::RemoveFollower() {
	p_follower = nullptr;
}
void Centipede::RemoveFollowedBy() {
	p_followed_by = nullptr;
}
int Centipede::GetFollowerID() const {
	if (p_follower == nullptr)
		return -1;
	return p_follower->GetID();
}
int Centipede::GetFollowedByID() const {
	if (p_followed_by == nullptr)
		return -1;
	return p_followed_by->GetID();
}

void Centipede::MoveLeftRight() {
	m_time_accum += MASTER_CLOCK->DeltaTime();

	this->ChangePositionToZ(Master::Math::LinearInterpolation(m_time_accum, 0.0f, s_time_interval, m_curr_direction.previous_position.z, m_curr_direction.target_position.z));
	
	p_rotate_machine->Update();

	m_is_update = m_time_accum > s_time_interval;
}
void Centipede::MoveUpDown() {
	m_time_accum += MASTER_CLOCK->DeltaTime();

	this->ChangePositionToX(Master::Math::LinearInterpolation(m_time_accum, 0.0f, s_time_interval, m_curr_direction.previous_position.x, m_curr_direction.target_position.x));

	p_rotate_machine->Update();

	m_is_update = m_time_accum > s_time_interval;
}

void Centipede::Rotate() {
	float rot = Master::Math::LinearInterpolation(m_time_accum, 0.0f, s_time_interval, m_prev_rotation, m_target_rotation);
	this->ChangeRotationTo(rot);

	if (m_time_accum > s_time_interval)
		p_rotate_machine->SetCurrState(nullptr);
}

void Centipede::ProcessDirection() {
	if (m_curr_direction.current == Centipede::direction_type::left)
		p_move_machine->SetCurrState(STATE_MOVE_LEFT);
	else if (m_curr_direction.current == Centipede::direction_type::right)
		p_move_machine->SetCurrState(STATE_MOVE_RIGHT);
	else if (m_curr_direction.current == Centipede::direction_type::up)
		p_move_machine->SetCurrState(STATE_MOVE_UP);
	else
		p_move_machine->SetCurrState(STATE_MOVE_DOWN);

	if (m_curr_direction.previous != m_curr_direction.current) {
		p_rotate_machine->SetCurrState(STATE_ROTATE);
		m_prev_rotation = m_target_rotation;
		switch (m_curr_direction.previous) {
		case Centipede::direction_type::left:
			if (m_curr_direction.current == Centipede::direction_type::down)
				m_target_rotation -= 90.0f;
			else
				m_target_rotation += 90.0f;
			break;
		case Centipede::direction_type::right:
			if (m_curr_direction.current == Centipede::direction_type::down)
				m_target_rotation += 90.0f;
			else
				m_target_rotation -= 90.0f;
			break;
		case Centipede::direction_type::up:
			if (m_curr_direction.current == Centipede::direction_type::left)
				m_target_rotation -= 90.0f;
			else
				m_target_rotation += 90.0f;
			break;
		default:
			if (m_curr_direction.current == Centipede::direction_type::left)
				m_target_rotation += 90.0f;
			else
				m_target_rotation -= 90.0f;
			break;
		}
	}
	else
		p_rotate_machine->SetCurrState(nullptr);
}

void Centipede::ProcessFollowerDirection() {
	if (p_follower == nullptr)
		return;

	Centipede::CentiDirection update_direction = p_follower->GetDirection();
	m_curr_direction.previous = m_curr_direction.current;
	m_curr_direction.current = update_direction.previous;

	m_curr_direction.previous_position = m_curr_direction.target_position;
	m_curr_direction.target_position = update_direction.previous_position;

	m_curr_direction.is_descending = update_direction.is_descending;
	m_curr_direction.is_poisoned = update_direction.is_poisoned;

	ProcessDirection();
}

void Centipede::SetDirection(Centipede::CentiDirection& direction) {
	m_curr_direction = direction;
	ProcessDirection();
}
Centipede::CentiDirection Centipede::GetDirection() const {
	return m_curr_direction;
}

DirectX::XMFLOAT3 Centipede::GetCurrentPosition() const {
	return m_curr_direction.previous_position;
}
