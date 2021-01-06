#include "UsedIDs.h"

UsedIDs::UsedIDs() {
	p_first = nullptr;
}
UsedIDs::~UsedIDs() {
	while (p_first != nullptr) {
		node* temp = p_first;
		p_first = p_first->next;
		delete temp;
	}
}

UsedIDs::UsedIDs(const UsedIDs& other) {
	p_first = other.p_first;
}
UsedIDs& UsedIDs::operator=(const UsedIDs& other) {
	if (this != &other)
		p_first = other.p_first;
	return *this;
}

void UsedIDs::Recycle(const int& id) {
	node* temp = new node();
	temp->info = id;
	temp->next = p_first;
	p_first = temp;
}

int UsedIDs::ReUse() {
	int id = p_first->info;

	node* temp = p_first;
	p_first = p_first->next;
	delete temp;

	return id;
}

bool UsedIDs::IsEmpty() const {
	return p_first == nullptr;
}