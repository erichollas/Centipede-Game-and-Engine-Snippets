#include "StateCentiMoveLeft.h"

void StateCentiMoveLeft::Execute(Centipede* game_object) {
	game_object->MoveLeftRight();
}
void StateCentiMoveLeft::Exit(Centipede* game_object) {
	// will not be called

}
bool StateCentiMoveLeft::OnMessage(Centipede* game_object, Message& msg) {
	// will not be called for this object

	return false;
}

StateCentiMoveLeft* StateCentiMoveLeft::inst() {
	static StateCentiMoveLeft inst;
	return &inst;
}