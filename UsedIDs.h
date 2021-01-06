#pragma once

/* 
 * Written by Eric Hollas
 * 
 * UsedIDs is a basic list of int
 * 
 * will be used to keep track of already used id numbers for characters
 * to ensure that id's are not reused by overflow, (there will be lots
 * of mushrooms)
 * 
 * -999 cannot be viable id number to set aside for initial data
 * 
 */

#include "framework.h"

// functions as a basic list
class UsedIDs {
public:
	UsedIDs();
	~UsedIDs();

	UsedIDs(const UsedIDs& other);
	UsedIDs& operator=(const UsedIDs& other);

	// Recycle is the add function in the list paradigm
	void Recycle(const int& id);
	
	// ReUse is the pop function in the list paradigm
	int ReUse();

	// returns true if empty
	bool IsEmpty() const;


private:
	struct node {
		node* next = nullptr;
		int info = -999;
	};

	node* p_first;
};